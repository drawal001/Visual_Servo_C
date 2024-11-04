// GxFlatFieldCorrectionDlg.cpp : implementation file
//


#include "stdafx.h"
#include "GxFlatFieldCorrection.h"
#include "GxFlatFieldCorrectionDlg.h" 
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#undef new

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strProductVersion;
	CString	m_strLegalCopyRight;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strProductVersion = _T("");
	m_strLegalCopyRight = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionDlg dialog

CGxFlatFieldCorrectionDlg::CGxFlatFieldCorrectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxFlatFieldCorrectionDlg::IDD, pParent)
	, m_bDevOpened(false)
	, m_bIsSnap(false)
	, m_bIsFFC(false)
	, m_bColorFilter(false)
	, m_bBalanceRatioSelector(false)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_pBrightBuf(NULL)
	, m_pDarkBuf(NULL)
	, m_nActualBits(DX_ACTUAL_BITS_8)
	, m_nRatioSelector(-1)
	, m_nAutoWhiteBalance(0)
	, m_pWnd(NULL)
	, m_nNewGray(0)
	, m_nFrameCount(CORRECTION_ACTUAL_FRAME_COUNT_4)
	, m_bDarkImgAcquired(false)
	, m_bBrightImgAcquired(false)
	, m_bExecuteFFC(false)
	, m_bShowFFCOK(false)
	, m_bGetFFCCoefficients(false)
	, m_nBrightImgNum(0)
	, m_pAccumulateImgBuf(NULL)
	, m_bBrightImgAcquiredOnce(false)
	, m_bDarkImgAcquiredOnce(false)
	, m_pFFCCoefficients(NULL)
	, m_nFFClength(0)
	, m_nFFCGray(0)
	, m_bFFCGray(false)
	, m_nAverageGrayDarkImg(0)
	, m_nAverageGrayBrightImg(0)
	, m_pAverageImgBuf(NULL)
	, m_pBitmap(NULL)
	, m_i64ImageHeight(0)
	, m_i64ImageWidth(0)
{
	//{{AFX_DATA_INIT(CGxFlatFieldCorrectionDlg)
	m_dBalanceRatio = 0.0;
	m_dGainValue    = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ������
	memset(&m_stGainFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stShutterFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stFlatFieldCorrection, 0, sizeof(FLAT_FIELD_CORRECTION_PROCESS));
}

void CGxFlatFieldCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxFlatFieldCorrectionDlg)
	DDX_Text(pDX, IDC_EDIT_BALANCE_RATIO, m_dBalanceRatio);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dShutterValue);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_FRAME_NUM, m_sliderFrameCount);
	DDX_Text(pDX, IDC_EDIT_GRAY, m_nNewGray);
	DDX_Control(pDX, IDC_SLIDER_GRAY, m_sliderGrayValue);
}

BEGIN_MESSAGE_MAP(CGxFlatFieldCorrectionDlg, CDialog)
	//{{AFX_MSG_MAP(CGxFlatFieldCorrectionDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnCbnSelchangeComboWriteBlance)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_RATIO_SELECTOR, OnCbnSelchangeComboRatioSelector)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, OnEnKillfocusEditBalanceRatio)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP

	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_GRAY, OnKillfocusEditGray)
	ON_BN_CLICKED(IDC_BUTTON_ACQUIRE_DARK_IMG, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireDarkImg)
	ON_BN_CLICKED(IDC_BUTTON_ACQUIRE_BRIGHT_IMG, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireBrightImg)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE_FFC, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonExecuteFFC)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, &CGxFlatFieldCorrectionDlg::OnBnClickedCheckPreview)
	ON_BN_CLICKED(IDC_CHECK_GRAY, &CGxFlatFieldCorrectionDlg::OnBnClickedCheckGray)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonWrite)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXPECT_GRAY_VALUE, &CGxFlatFieldCorrectionDlg::OnDeltaposSpinExpectGrayValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionDlg message handlers

BOOL CGxFlatFieldCorrectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//��ʼ����
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	//��ȡͼ����ʾ���ڵ�ָ��ͻ�ͼDC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_FRAME);

	// ���½���ؼ�
	UpDateUI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxFlatFieldCorrectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, strFileName, MAX_PATH);
		CFileVersion fv(strFileName);
		CAboutDlg dlgAbout;		
		dlgAbout.m_strProductVersion = _T("Version: ") + fv.GetProductVersion();
		dlgAbout.m_strLegalCopyRight = fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGxFlatFieldCorrectionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGxFlatFieldCorrectionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   �ر�Ӧ�ó�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ��δͣ������ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		// ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		m_bIsSnap = false;

		m_pBitmap->UnPrepareForShowImg();
	}

	// δ�ر��豸��ر��豸
	if (m_bDevOpened)
	{
		// ������Դ
		UnPrepareForShowImg();
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = false;
		m_hDevice    = NULL;

		if (NULL != m_pBitmap)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
	}

	// �ر��豸��
	emStatus = GXCloseLib();

	CDialog::OnClose();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// ��ʼ����ƽ�����Combox�ؼ�
	InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);

	// ��ʼ����ƽ��ͨ��Combox�ؼ�
	InitEnumUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR), m_bBalanceRatioSelector);

	// ��ʼ���ع�ʱ����ؿؼ�
	InitShutterUI();

	// ��ʼ��������ؿؼ�
	InitGainUI();

	// ��ʼ����ƽ��ϵ����ؿؼ�
	InitWhiteRatioUI();

	// ��ʼ�������Ҷ�ֵ��ؿؼ�
	InitGrayUI();

	// ��ʼ���ɼ�֡����ؿؼ�
	InitFrameCountUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����ƽ��ϵ����ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitWhiteRatioUI()
{
	GX_STATUS      emStatus = GX_STATUS_SUCCESS;
	CString        strTemp  = _T("");

	if (!m_bBalanceRatioSelector)
	{
		return;
	}

	// ��ȡϵ����Χ
	emStatus = GXGetFloatValue(m_hDevice, "BalanceRatio", &m_stFloatRange);
	GX_VERIFY(emStatus);
	strTemp.Format("��ƽ��ϵ��(%.4f~%.4f)", m_stFloatRange.dMin, m_stFloatRange.dMax);
	SetDlgItemText(IDC_STATIC_WHITE_RATIO, strTemp);

	// ˢ����ʾ��ǰ��ƽ��ϵ��
	RefreshWhiteRatio();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   chFeature   [in]    ������ID
\param   pComboBox     [in]    �ؼ�ָ��
\param   bIsImplement  [in]    �豸�Ƿ�֧��chFeature������

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitEnumUI(char* chFeatire, CComboBox *pComboBox, bool bIsImplement)
{
	// �жϿؼ�
	if ((pComboBox == NULL) || !bIsImplement)
	{
		return;
	}

	GX_ENUM_VALUE stEnumValue;
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	size_t    nbufferSize = 0;
	uint32_t  nEntryNum   = 0;
	int64_t   nEnumValue  = -1;
	int       nCursel     = 0;
	double    dValue      = 0;


	// ��ȡ���ܵ�ö����
	emStatus = GXGetEnumValue(m_hDevice, chFeatire, &stEnumValue);
	GX_VERIFY(emStatus);

	nEntryNum = stEnumValue.nSupportedNum;

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCursel = i;
		}
	}

	// ���õ�ǰֵΪ�������ʾֵ
	pComboBox->SetCurSel(nCursel);
}

//---------------------------------------------------------------------------------
/**
\brief   �ص�����
\param   pFrame         �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxFlatFieldCorrectionDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxFlatFieldCorrectionDlg *pDlg        = (CGxFlatFieldCorrectionDlg*)(pFrame->pUserParam);
	VxInt32                   DxStatus     = DX_OK;

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		return ;
	}

	// �ɼ�����ͼ��
	if (true == pDlg->m_bDarkImgAcquired)
	{
		memcpy(pDlg->m_pDarkBuf, pFrame->pImgBuf, pFrame->nImgSize);
		pDlg->AcquireDarkFieldImg();
		pDlg->m_bDarkImgAcquired = false;
	}

	// �ɼ�����ͼ��
	if (true == pDlg->m_bBrightImgAcquired)
	{
		memcpy(pDlg->m_pBrightBuf, pFrame->pImgBuf, pFrame->nImgSize);
		pDlg->AcquireBrightFieldImg();
	}		

	void* pImgBuf = const_cast<void*>(pFrame->pImgBuf);
	// �ж��Ƿ���Ҫ��ͼ�����ƽ��У��
	if (pDlg->m_bIsFFC)
	{
		DxStatus = DxFlatFieldCorrection(pImgBuf
			, pImgBuf
			, pDlg->m_nActualBits
			, (VxUint32)(pFrame->nWidth)
			, (VxUint32)(pFrame->nHeight)
			, pDlg->m_pFFCCoefficients
			, &pDlg->m_nFFClength);


		if (DX_OK != DxStatus)
		{
			pDlg->m_bIsFFC = false;
			pDlg->GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("ƽ��У��ϵ��Ӧ��ʧ��");
			pDlg->UpDateUI();
		}
		else
		{
			if (!pDlg->m_bShowFFCOK)
			{
				pDlg->GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("ƽ��У��ϵ��Ӧ�óɹ�");  
				pDlg->UpDateUI();
				pDlg->m_bShowFFCOK = true;
			}
		}
	}

	pDlg->m_pBitmap->Show(pFrame);

}

//---------------------------------------------------------------------------------
/**
\brief   Ϊͼ����ʾ׼����Դ

\return  true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool CGxFlatFieldCorrectionDlg::PrepareForShowImg()
{
	bool bRet = true;

	do 
	{
		int64_t i64ImageWidth  = m_i64ImageHeight;
		int64_t i64ImageHeight = m_i64ImageWidth;
		//Ϊ�洢����ͼ����������ռ�
		m_pDarkBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pDarkBuf == NULL)
		{
			bRet = false;
			break;
		}
		memset(m_pDarkBuf, 0, (size_t)(i64ImageWidth * i64ImageHeight));

		//Ϊ�洢����ͼ����������ռ�
		m_pBrightBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pBrightBuf == NULL)
		{
			bRet = false;
			break;
		}

		//Ϊ�洢�����ۼ�ͼ����������ռ�
		m_pAccumulateImgBuf = new(std::nothrow) VxUint32[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pAccumulateImgBuf == NULL)
		{
			bRet = false;
			break;
		}

		//Ϊ�洢����ƽ��ͼ����������ռ�
		m_pAverageImgBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pAverageImgBuf == NULL)
		{
			bRet = false;
			break;
		}

		//��ȡƽ��У���������ȣ��ֽ�����
		//��Ϊƽ��У����������ռ�
		m_stFlatFieldCorrection.pBrightBuf = m_pAverageImgBuf;
		m_stFlatFieldCorrection.pDarkBuf   = m_pDarkBuf;
		DxGetFFCCoefficients(m_stFlatFieldCorrection, NULL, &m_nFFClength);
		m_pFFCCoefficients = new(std::nothrow) BYTE[m_nFFClength];
		if (m_pFFCCoefficients == NULL)
		{
			bRet = false;
			break;
		}
		GX_STATUS emStatus = m_pBitmap->PrepareForShowImg();
		if (GX_STATUS_SUCCESS != emStatus) 
		{
			bRet = false;
			break;
		}

	} while (0);


	// ������ʧ�����ͷ��ѷ������Դ
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//---------------------------------------------------------------------------------
/**
\brief   �ͷ�Ϊͼ����ʾ׼����Դ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::UnPrepareForShowImg()
{

	if (m_pDarkBuf != NULL)
	{
		delete []m_pDarkBuf;
		m_pDarkBuf = NULL;
	}
	if (m_pBrightBuf != NULL)
	{
		delete []m_pBrightBuf;
		m_pBrightBuf = NULL;
	}
	if(m_pAccumulateImgBuf != NULL)
	{
		delete []m_pAccumulateImgBuf;
		m_pAccumulateImgBuf = NULL;
	}
	if(m_pAverageImgBuf != NULL)
	{
		delete []m_pAverageImgBuf;
		m_pAverageImgBuf = NULL;
	}
	if(m_pFFCCoefficients != NULL)
	{
		delete []m_pFFCCoefficients;
		m_pFFCCoefficients = NULL;
	}
	m_pBitmap->UnPrepareForShowImg();
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"�Զ���ƽ��"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnCbnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox     = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS emStatus  = GX_STATUS_SUCCESS;
	int64_t   nEnumVal  = 0;
	int       nIndex    = 0;

	nIndex    = pBox->GetCurSel();                    // ��ȡ��ǰѡ��
	nEnumVal  = (int64_t)pBox->GetItemData(nIndex);   // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	//���ð�ƽ��
	emStatus = GXSetEnumValue(m_hDevice, "BalanceWhiteAuto", nEnumVal);
	GX_VERIFY(emStatus);
	m_nAutoWhiteBalance = nEnumVal;

	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡGxIAPI��������,������������ʾ�Ի���
\param  emErrorStatus  [in]   ������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;

	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// �ͷ�������ڴ�ռ�
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ˢ��UI����

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);

	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bDevOpened && m_bColorFilter);
	GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(m_bDevOpened && m_bBalanceRatioSelector);

	GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(m_bDevOpened 
		&& (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		&&  m_bBalanceRatioSelector);

	GetDlgItem(IDC_SLIDER_FRAME_NUM)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_CHECK_GRAY)->EnableWindow(m_bDevOpened);               
	((CButton*)GetDlgItem(IDC_CHECK_GRAY))->SetCheck(m_bDevOpened && m_bFFCGray);

	GetDlgItem(IDC_SLIDER_GRAY)->EnableWindow(m_bDevOpened && m_bFFCGray);
	GetDlgItem(IDC_EDIT_GRAY)->EnableWindow(m_bDevOpened && m_bFFCGray);
	GetDlgItem(IDC_SPIN_EXPECT_GRAY_VALUE)->EnableWindow(m_bDevOpened && m_bFFCGray);

	GetDlgItem(IDC_BUTTON_ACQUIRE_DARK_IMG)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BUTTON_ACQUIRE_BRIGHT_IMG)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BUTTON_EXECUTE_FFC)->EnableWindow(m_bDevOpened && m_bIsSnap && m_bBrightImgAcquiredOnce);

	GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(m_bDevOpened && m_bIsSnap && m_bExecuteFFC);           
	((CButton*)GetDlgItem(IDC_CHECK_PREVIEW))->SetCheck(m_bDevOpened && m_bIsSnap && m_bExecuteFFC && m_bIsFFC);  

	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(m_bDevOpened && m_bIsSnap);                              
	GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(m_bDevOpened && m_bIsSnap);                            
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nDevNum  = 0;
	uint32_t  nDSNum   = 0;

	GX_OPEN_PARAM  stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// ö���豸
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �ж��豸����
	if (nDevNum <= 0)
	{
		MessageBox("û�з����豸!");
		return;
	}

	// ����豸�Ѿ�����ر�,��֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ���豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = true;

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}

	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�,���ݸ�ʽ:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	do
	{
		m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// ��ȡ�豸�Ŀ��ߵ�������Ϣ
		emStatus = GetDeviceParam();
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	// ��ȡ�������,��ʼ������ؼ�
	InitUI();

	// ���ö�ʱ�����½���
	SetTimer(0, 1000, NULL);

	// ���½���ؼ�
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief   ��ʼ�����:����Ĭ�ϲ���

\return  ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxFlatFieldCorrectionDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�豸��������Ϣ

\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxFlatFieldCorrectionDlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bColorFliter   = false;
	GX_INT_VALUE stIntValue;
	GX_ENUM_VALUE stEnumValue;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	int64_t i64PixelFormat;  //ͼ�����ظ�ʽ

	// ��ȡ������ظ�ʽ
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// ��ȡ��ǰ���ظ�ʽ
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}
	//�ж��Ƿ�Ϊ��ɫ
	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(i64PixelFormat));

	if (m_bColorFilter)
	{
		// ��ȡ�Զ���ƽ��ö��ֵ
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

	}

	// �ж�����Ƿ�֧�ְ�ƽ��ͨ��
	emStatus = GXGetNodeAccessMode(m_hDevice, "BalanceRatioSelector", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		m_bBalanceRatioSelector = true;
	}
	else 
	{
		m_bBalanceRatioSelector = false;
	}

	// ��ȡ���
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageWidth = stIntValue.nCurValue;

	// ��ȡ�߶�
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageHeight = stIntValue.nCurValue;
	
	m_stFlatFieldCorrection.nImgWid = (VxUint32)m_i64ImageWidth;
	m_stFlatFieldCorrection.nImgHei = (VxUint32)m_i64ImageHeight;

	// ��ȡλ��
	m_stFlatFieldCorrection.nActualBits = m_nActualBits;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ������Դ
	UnPrepareForShowImg();

	// δͣ��ʱ��ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		//ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		m_bIsSnap                = false;
		m_bFFCGray               = false;

		m_bExecuteFFC            = false;
		m_bGetFFCCoefficients    = false;
		m_bIsFFC                 = false;
		m_bBrightImgAcquired     = false;
		m_bBrightImgAcquiredOnce = false;
		m_bDarkImgAcquired       = false;
		m_bDarkImgAcquiredOnce   = false;
	}

	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_bDevOpened = false;
	m_hDevice    = NULL;

	KillTimer(0);

	// ���½���UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("");
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	// ����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	// ע���ص�
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	UnPrepareForShowImg();

	m_bExecuteFFC            = false;
	m_bGetFFCCoefficients    = false;
	m_bIsFFC                 = false;
	m_bBrightImgAcquired     = false;
	m_bBrightImgAcquiredOnce = false;
	m_bDarkImgAcquired       = false;
	m_bDarkImgAcquiredOnce   = false;
	m_bIsSnap                = false;

	// ���½���UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("");
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ��ʼ��BMPͷ��Ϣ������BufferΪͼ��ɼ���׼��
	if (!PrepareForShowImg())
	{
		MessageBox("Ϊͼ����ʾ������Դʧ��!");
		return;
	}

	// ע��ص�
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// ����ʼ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	m_bIsSnap = true;

	// ���½���UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("1��ƽ��У��������ֱ�ɼ�����������ͼ��\r\n2����ɫ���ǿ�ҽ����Ƚ��а�ƽ�⡣\r\n3���ɼ�����ͼ��ʱ����Ͼ�ͷ�ǡ�");
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ�: Static:��ʾ��Χ Edit:����ֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitGainUI()
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_GAIN);
	int          nPos         = 0;

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strRange = "";
	double    stGainVal = 0;

	// ��ȡ�����ͷ�Χ,����ʼ��SliderCtrl�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// ��ʾ��Χ
	strRange.Format("����(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);

	// ��ȡ��ǰֵ�����µ�����
	m_dGainValue = m_stGainFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع���ؿؼ�:Static:��ʾ��Χ Edit:����ֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);
	int          nPos            = 0;

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS emStatus      = GX_STATUS_ERROR;
	CString   strTemp       = "";
	double    dShutterValue = 0.0;

	// ��ȡ�����ͷ�Χ,����ʼ��SliderCtrl�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

	strTemp.Format("�ع�(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// ��ȡ��ǰֵ������ǰֵ���µ�����
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ�������Ҷ�ֵ��ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitGrayUI()
{
	CSliderCtrl     *pSliderCtrl  = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GRAY);
	CEdit           *pEditGrayVal = (CEdit *)GetDlgItem(IDC_EDIT_GRAY);
	CSpinButtonCtrl *pSpin        = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXPECT_GRAY_VALUE);

	if ((pSliderCtrl == NULL) || (pEditGrayVal == NULL) || (pSpin == NULL) )
	{
		return;
	}

	// ���������Ҷ�ֵ��������Χ�ͳ�ʼֵ�����ڰ�ť��Χ���༭���ʼֵ
	pSliderCtrl->SetRange(GRAY_MIN, GRAY_MAX, false);
	pSliderCtrl->SetPos(GRAY_INIT);
	pSpin->SetRange(GRAY_MIN, GRAY_MAX);
	m_nNewGray = GRAY_INIT;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��ͼ��ɼ�֡����ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitFrameCountUI()
{
	CSliderCtrl *pSliderCtrl  = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_FRAME_NUM);

	if (NULL == pSliderCtrl)
	{
		return;
	}

	// ������ʾ��Χ��Ĭ��ֵ
	pSliderCtrl->SetRange(CORRECTION_UI_FRAME_COUNT_MIN, CORRECTION_UI_FRAME_COUNT_MAX, true);
	pSliderCtrl->SetPos(CORRECTION_UI_FRAME_COUNT_2);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"��ƽ��ͨ��"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnCbnSelchangeComboRatioSelector()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR);
	double     dValue  = 0.0;

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ���ð�ƽ��ͨ��
	emStatus = GXSetEnumValue(m_hDevice, "BalanceRatioSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// ��ȡ��ƽ��ϵ�����½���
	InitWhiteRatioUI();
}

//---------------------------------------------------------------------------------
/**
\brief   "��ƽ��ϵ��"Edit��ʧȥ������Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnEnKillfocusEditBalanceRatio()
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData(TRUE);
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ��ڰ�ƽ��ϵ���ķ�Χ��
	//���������ֵ�򽫰�ƽ��ϵ����Ϊ���ֵ
	if (m_dBalanceRatio > m_stFloatRange.dMax)
	{
		m_dBalanceRatio = m_stFloatRange.dMax;
	}
	//��С����Сֵ����ƽ��ϵ����Ϊ��Сֵ
	if (m_dBalanceRatio < m_stFloatRange.dMin)
	{
		m_dBalanceRatio = m_stFloatRange.dMin;
	}

	emStatus = GXSetFloatValue(m_hDevice,"BalanceRatio",m_dBalanceRatio);
	GX_VERIFY(emStatus);

	UpdateData(FALSE);
}

//-----------------------------------------------------------------------
/**
\brief   �ַ�������Ҫ�����ع�������Edit����Ӧ�س�������Ϣ
\param   pMsg  ��Ϣ�ṹ��

\return �ɹ�:TRUE   ʧ��:FALSE
*/
//-----------------------------------------------------------------------
BOOL CGxFlatFieldCorrectionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd  *pWnd   = NULL;
	int   nCtrlID = 0;             //< �����ȡ�Ŀؼ�ID

	//�ж��Ƿ��Ǽ��̻س���Ϣ
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		//��õ�ǰ����Ŀؼ�ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//�ж�ID����
		switch(nCtrlID)
		{
		case IDC_EDIT_BALANCE_RATIO:
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:
		case IDC_EDIT_GRAY: 

			//ʧȥ����
			SetFocus();
			break;

		default:
			break;
		}

		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------
/**
\brief    �����ع�ʱ���Edit��ʧȥ�������Ӧ����

\return   ��
*/
//-----------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnKillfocusEditShutter() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();	
	GX_STATUS status = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ����ع�ʱ��ķ�Χ��
	//���������ֵ���ع�ֵ��Ϊ���ֵ
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//��С����Сֵ���ع�ֵ��Ϊ��Сֵ
	if (m_dShutterValue < m_stShutterFloatRange.dMin)
	{
		m_dShutterValue = m_stShutterFloatRange.dMin;
	}

	status = GXSetFloatValue(m_hDevice, "ExposureTime", m_dShutterValue);
	GX_VERIFY(status);


	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   ��������ֵ��Edit��ʧȥ�������Ӧ����

\return  ��
*/
//--------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	// �жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ�������ֵ�ķ�Χ��
	//�������ֵ�������ֵ������ֵ���ó����ֵ
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

	//�������ֵС����Сֵ�������ֵ���ó���Сֵ
	if (m_dGainValue < m_stGainFloatRange.dMin)
	{
		m_dGainValue = m_stGainFloatRange.dMin;
	}

	status = GXSetFloatValue(m_hDevice, "Gain", m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   ����ƽ��У�������Ҷ�ֵ��Edit��ʧȥ�������Ӧ����

\return  ��
*/
//--------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnKillfocusEditGray()
{
	// TODO: Add your control notification handler code here
	// �жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//�ж�����ֵ�Ƿ���0-255ֵ�ķ�Χ��
	//�������ֵ�������ֵ��ƽ��У������ֵ���ó����ֵ
	if (m_nNewGray > GRAY_MAX)
	{
		m_nNewGray = GRAY_MAX;
	}

	//�������ֵС����Сֵ��ƽ��У������ֵ���ó���Сֵ
	if (m_nNewGray < GRAY_MIN)
	{
		m_nNewGray = GRAY_MIN;
	}
	m_sliderGrayValue.SetPos(m_nNewGray);

	UpdateData(FALSE);
}

//----------------------------------------------------------------
/**
\brief  ��ʱ������:��ʱ��ȡ�Զ���ƽ���ö��ֵʵ������Once�ɹ�������Զ�����Ϊoff
\param  ��ʱ���¼�ID

\return ��
*/
//----------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus  = GX_STATUS_ERROR;

	//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
	//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		GX_ENUM_VALUE stEnumValue;
		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);

		//��ȡ�Զ���ƽ��ö��ֵ
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return;
		}

		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue; //< �����ƽ���ö��ֵ

		//�ж��豸�Զ���ƽ���Ƿ��ΪOFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// ѡ���Զ���ƽ��ؼ��е�OFF��,����ONCE��ΪOFF
					pAutoWBBox->SetCurSel(i);
					break;
				}
			}
			UpDateUI();	
			RefreshWhiteRatio();
		}
	}

	//����Զ���ƽ�ⷽʽΪContinuous
	//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_CONTINUOUS)
	{
		RefreshWhiteRatio();
	}

	CDialog::OnTimer(nIDEvent);
}

//---------------------------------------------------------------------------------
/**
\brief   ��������Ϣ��Ӧ����
\param   nSBCode     ָ�������������Ĵ���
\param   nPos        ������λ��
\param   pScrollBar  �������ؼ�ָ��

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	char PutText[10] = {0};
	int  nValue      = 0;

	if (pScrollBar == (CScrollBar*)&m_sliderFrameCount)            // �ɼ�֡��������
	{
		m_nFrameCount = m_sliderFrameCount.GetPos();
		GetFrameCount(m_nFrameCount);
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderGrayValue)        // ƽ��У�������Ҷ�ֵ������
	{
		m_nNewGray = m_sliderGrayValue.GetPos();
		sprintf_s(PutText,"%d", m_nNewGray);
		GetDlgItem(IDC_EDIT_GRAY)->SetWindowText(PutText);	
	}

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ֵ���ڰ�ť��Ϣ��Ӧ����
\param   pNMHDR     ֪ͨ��Ϣ���
\param   pResult    ���ָ��

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnDeltaposSpinExpectGrayValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	UpdateData();

	if(pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����µļ�ͷ
	{
		m_nNewGray--;

		//�������ֵС����Сֵ��ƽ��У������ֵ���ó���Сֵ
		if (m_nNewGray < GRAY_MIN)
		{
			m_nNewGray = GRAY_MIN;
		}
		m_sliderGrayValue.SetPos(m_nNewGray);
	}

	else if(pNMUpDown->iDelta == 1) // �����ֵΪ1, ˵�������Spin�����ϵļ�ͷ
	{
		m_nNewGray++;

		//�������ֵ�������ֵ��ƽ��У������ֵ���ó����ֵ
		if (m_nNewGray > GRAY_MAX)
		{
			m_nNewGray = GRAY_MAX;
		}
		m_sliderGrayValue.SetPos(m_nNewGray);
	}

	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ɼ�����ͼ��"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireDarkImg()
{
	// TODO: Add your control notification handler code here
	m_bDarkImgAcquired = true;
	m_bExecuteFFC      = false;
	m_bIsFFC           = false;

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ɼ�����ͼ��"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireBrightImg()
{
	// TODO: Add your control notification handler code here
	memset(m_pAccumulateImgBuf, 0, sizeof(VxUint32) * (size_t)(m_i64ImageWidth * m_i64ImageHeight));

	m_bBrightImgAcquired     = true;
	m_bBrightImgAcquiredOnce = false;
	m_bExecuteFFC            = false;
	m_bIsFFC                 = false; 

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡƽ��У��ϵ��

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonExecuteFFC()
{
	// TODO: Add your control notification handler code here
	VxInt32  emStatus = DX_OK;

	// �ж��Ƿ�ɼ�������ͼ��
	if (false == m_bDarkImgAcquiredOnce)
	{
		m_stFlatFieldCorrection.pDarkBuf = NULL;
	}
	else
	{
		m_stFlatFieldCorrection.pDarkBuf = m_pDarkBuf;
	}

	// �������Ҷ�ֵ��ѡ�򱻹�ѡ����ִ��ƽ��У��ʱ���ø������Ҷ�ֵ
	m_nFFClength = (int)(4 * m_i64ImageWidth * m_i64ImageHeight);
	if (true == m_bFFCGray)
	{
		m_nFFCGray = m_nNewGray;
		emStatus = DxGetFFCCoefficients(m_stFlatFieldCorrection, m_pFFCCoefficients, &m_nFFClength, &m_nFFCGray);
	}
	else
	{
		emStatus = DxGetFFCCoefficients(m_stFlatFieldCorrection, m_pFFCCoefficients, &m_nFFClength);
	}

	//�ж�ƽ��У��ϵ����ȡ�������������ʾ��Ϣ
	if (DX_OK != emStatus)
	{
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("ƽ��У��ϵ����ȡʧ��");
		m_bExecuteFFC         = false;  
		m_bGetFFCCoefficients = false;
		m_bIsFFC              = false; 
	} 
	else
	{
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("ƽ��У��ϵ����ȡ�ɹ�");
		m_bExecuteFFC         = true;      //ִ��ƽ��У���ɹ�
		m_bShowFFCOK          = false;
		m_bGetFFCCoefficients = true;      //��ȡƽ��У��ϵ���ɹ�
		m_bIsFFC              = true;      //Ӧ��ƽ��У��ϵ��
	}

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  ƽ��У�����أ��������˿���ʱ�����ڻص������ж�ÿһ�Բɼ��İ���ͼ���
����ͼ�����ƽ��У����

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedCheckPreview()
{
	// TODO: Add your control notification handler code here
	m_bIsFFC = !m_bIsFFC;
	if (m_bIsFFC)
	{
		m_bShowFFCOK = false;
	}

	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText(" ");
}

//-------------------------------------------------------------------
/**
\brief  ͼ���ۼӺ���
\param  pImgBuf           ����ͼ��buf
\param  nDataWidth        ͼ��λ��
\param  nlength           ͼ�񳤶�
\param  pAccumulateImgBuf �ۼ�ͼ��buff

\return  ��ӰУ��������
*/
//-------------------------------------------------------------------
DX_STATUS CGxFlatFieldCorrectionDlg::AccumulateImage(void *pImgBuf, int nDataWidth, int64_t nlength, VxUint32 *pAccumulateImgBuf)
{
	if (NULL == pImgBuf || NULL == pAccumulateImgBuf)
	{
		return DX_PARAMETER_INVALID;
	}

	// ����λ������Ҫ�� ���ش���
	if ((nDataWidth < IMAGE_DATA_NUM_MIN) || (nDataWidth > IMAGE_DATA_NUM_MAX))
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (nlength < 1)
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (IMAGE_DATA_NUM_MIN == nDataWidth)
	{

		unsigned char * pImgBufTemp = (unsigned char *)pImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAccumulateImgBuf[i] += pImgBufTemp[i];
		}

	}
	else
	{
		unsigned short * pImgBufTemp = (unsigned short *)pImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAccumulateImgBuf[i] += pImgBufTemp[i];
		}
	}

	return DX_OK;
}

//-------------------------------------------------------------------
/**
\brief  ƽ��ͼ����㺯��
\param  pImgBuf                �ۼ�ͼ��buff
\param  nlength                ͼ���С
\param  nAccumulateNum         �ۼ�ͼ������
\param  nDataWidth             ͼ��λ��
\param  pAverageImgBuf         ƽ��ͼ

\return  ��ӰУ��������
*/
//-------------------------------------------------------------------
DX_STATUS CGxFlatFieldCorrectionDlg::GetAverageImage(VxUint32 *pImgBuf, int64_t nlength, int nAccumulateNum, int nDataWidth, void *pAverageImgBuf)
{
	if (NULL == pImgBuf || NULL == pAverageImgBuf)
	{
		return DX_PARAMETER_INVALID;
	}

	// ����λ������Ҫ�� ���ش���
	if ((nDataWidth < IMAGE_DATA_NUM_MIN) || (nDataWidth > IMAGE_DATA_NUM_MAX))
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (nlength < 1 || nAccumulateNum < 1)
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	int64_t i = 0;
	if (IMAGE_DATA_NUM_MIN == nDataWidth)
	{

		unsigned char *pAverageImgBufTemp = (unsigned char *)pAverageImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAverageImgBufTemp[i] = pImgBuf[i] / nAccumulateNum;
		}

	}
	else
	{
		unsigned short *pAverageImgBufTemp = (unsigned short *)pAverageImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAverageImgBufTemp[i] = pImgBuf[i]  / nAccumulateNum;
		}
	}

	return DX_OK;
}

//----------------------------------------------------------------------------------
/**
\brief  ���ƽ��У�������Ҷ�ֵ�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedCheckGray()
{
	// TODO: Add your control notification handler code here
	m_bFFCGray = !m_bFFCGray;
	UpDateUI();
}

//-------------------------------------------------------------------
/**
\brief   ����ͼ��ƽ���Ҷ�ֵ
\param   pImgBuf                ͼ��buff
\param   nlength                ͼ���С

\return  ��ӰУ��������
*/
//-------------------------------------------------------------------
int64_t CGxFlatFieldCorrectionDlg::GetAverageGray8bit(BYTE *pImgBuf, int64_t nImgSize)
{
	int64_t nGrayAverage = 0;
	int64_t nGraySum     = 0;

	if ((NULL == pImgBuf) || (nImgSize < 1))
	{
		return GRAY_INVALID;
	}

	//��������ͼ��ĻҶ�ֵ�ۼӺ�
	for(int64_t i = 0; i < nImgSize; i++)
	{
		nGraySum += (int64_t) *(pImgBuf + i);
	}

	nGrayAverage = nGraySum / nImgSize;
	return nGrayAverage;
}

//-------------------------------------------------------------------
/**
\brief   ��������ͼ��ɼ�֡��
\param   nFrameCount      ͼ��ɼ�֡��        

\return  ��ӰУ��������
*/
//-------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::GetFrameCount(int &nFrameCount)
{
	//UI����ɼ�֡����ʵ�ʲɼ�֡����ת��
	switch (nFrameCount)   
	{
	case CORRECTION_UI_FRAME_COUNT_MIN:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_1;
		break;

	case CORRECTION_UI_FRAME_COUNT_1:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_2;
		break;

	case CORRECTION_UI_FRAME_COUNT_2:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_4;
		break;

	case CORRECTION_UI_FRAME_COUNT_3:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_8;
		break;

	case CORRECTION_UI_FRAME_COUNT_MAX:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_16;
		break;

	default: 
		break;
	}
}

//-------------------------------------------------------------------
/**
\brief   ���ļ��ж�ȡFFC����
\param   strFilePath   �ļ���ȡ·��        

\return  �ļ�״̬��
*/
//-------------------------------------------------------------------
FFC_STATUS CGxFlatFieldCorrectionDlg::ReadFFCCoefficientsFile(const std::string &strFilePath)
{
	FFC_STATUS emStatus = FFC_STATUS_SUCCESS;
	FILE *fp = NULL;

	do 
	{
		if (NULL == m_pFFCCoefficients)
		{
			emStatus = FFC_STATUS_FILE_DATA_ERROR;
			break;
		}

		//����.ffc��׺���ļ�
		size_t nSuffix = strFilePath.rfind(".ffc");
		if (nSuffix != strFilePath.size() - strlen(".ffc"))
		{
			emStatus = FFC_STATUS_FILE_PATH_ERROR;
			break;
		}

		//���ļ�����
		size_t nError = fopen_s(&fp, strFilePath.c_str(), "rb");
		if (0 != nError)
		{
			emStatus = FFC_STATUS_FILE_OPEN_ERROR;
			break;
		}

		//�ж�FFCϵ������
		fseek(fp, 0L, SEEK_END);
		size_t nFileLength = ftell(fp);
		if (((2 * m_i64ImageWidth * m_i64ImageHeight) != nFileLength)
			&& ((4 * m_i64ImageWidth * m_i64ImageHeight) != nFileLength))
		{
			emStatus = FFC_STATUS_FILE_READ_ERROR;
			break;
		}
		else
		{	
			m_nFFClength = (int)nFileLength;
		}

		//���ļ��ж�ȡFFC����
		fseek(fp, 0, SEEK_SET);
		nError = fread(m_pFFCCoefficients, m_nFFClength, 1, fp);
		if (1 != nError)
		{
			emStatus = FFC_STATUS_FILE_READ_ERROR;
			break;
		}

	} while (0);

	if (fp)
	{
		fclose(fp);
	}

	return emStatus;
}

//-------------------------------------------------------------------
/**
\brief   ��FFC�������浽�ļ���
\param   strFilePath   �ļ�����·��       

\return  �ļ�״̬��
*/
//-------------------------------------------------------------------
FFC_STATUS CGxFlatFieldCorrectionDlg::WriteFFCCoefficientsFile(const std::string &strFilePath)
{
	FFC_STATUS emStatus = FFC_STATUS_SUCCESS;
	FILE       *fp      = NULL;

	do 
	{
		if (NULL == m_pFFCCoefficients)
		{
			emStatus = FFC_STATUS_FILE_DATA_ERROR;
			break;
		}

		// �ļ�·���Ƿ���Ч
		size_t nSuffix = strFilePath.rfind(".ffc");
		if (nSuffix != strFilePath.size() - strlen(".ffc"))
		{
			emStatus = FFC_STATUS_FILE_PATH_ERROR;
			break;
		}

		// �ļ����󴴽�
		int nError = fopen_s(&fp, strFilePath.c_str(), "wb");
		if (0 != nError)
		{
			emStatus = FFC_STATUS_FILE_OPEN_ERROR;
			break;
		}

		// ��FFC����д���ļ���
		if (1 != fwrite(m_pFFCCoefficients, m_nFFClength, 1, fp))
		{
			emStatus = FFC_STATUS_FILE_WRITE_ERROR;
			break;
		}
	} while (0);

	if (fp)
	{
		fclose(fp);
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��������ļ���ȡ���ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonRead()
{
	// TODO: Add your control notification handler code here
	FFC_STATUS emStatus    = FFC_STATUS_SUCCESS;
	CString    strFilePath = "";

	//��ȡFFC�ļ�������ȡ�ɹ�����ƽ��У����Ԥ����ť���ѡ��
	CFileDialog objFileDialog( TRUE, NULL, NULL, 0, "ffc�ļ� (*.ffc)|*.ffc||", this);
	if( objFileDialog.DoModal() == IDOK )
	{
		strFilePath = objFileDialog.GetPathName();	           //ѡ����ļ�·��	
		emStatus    = ReadFFCCoefficientsFile(strFilePath.GetBuffer(0));
		switch (emStatus)   
		{
		case FFC_STATUS_FILE_DATA_ERROR:
			MessageBox("������Ч��");
			break;

		case FFC_STATUS_FILE_PATH_ERROR:
			MessageBox("�ļ�·����Ч��");
			break;

		case FFC_STATUS_FILE_OPEN_ERROR:
			MessageBox("���ļ�ʧ�ܣ�");
			break;

		case FFC_STATUS_FILE_READ_ERROR:
			MessageBox("������ȡʧ�ܣ�");
			break;

		default: 
			break;
		}

		if (FFC_STATUS_SUCCESS == emStatus)
		{
			m_bExecuteFFC         = true;
			m_bShowFFCOK          = false;
			m_bIsFFC              = true;
			m_bGetFFCCoefficients = true;
		}
		else
		{
			m_bExecuteFFC         = false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
			m_bGetFFCCoefficients = false;
			m_bIsFFC              = false;    
			UpDateUI();
			GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText(" ");
		}
	}	

}

//----------------------------------------------------------------------------------
/**
\brief  ��������浽�ļ����ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonWrite()
{
	// TODO: Add your control notification handler code here
	FFC_STATUS emStatus    = FFC_STATUS_SUCCESS;
	CString    strFilePath = "";

	if (false == m_bGetFFCCoefficients)
	{
		MessageBox("������Ч��");
		return ;
	}

	//��FFCϵ���Զ�������ʽ���浽��׺Ϊ.ffc�ļ���
	CFileDialog objFileDialog( FALSE, ".ffc", "FlatFieldCorrection.ffc", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ffc�ļ� (*.ffc)|*.ffc||", this);
	if( objFileDialog.DoModal() == IDOK )
	{
		strFilePath = objFileDialog.GetPathName();	           //ѡ����ļ�·��	
		emStatus = WriteFFCCoefficientsFile(strFilePath.GetBuffer(0));
		switch (emStatus)   
		{
		case FFC_STATUS_FILE_DATA_ERROR:
			MessageBox("������Ч��");
			break;

		case FFC_STATUS_FILE_PATH_ERROR:
			MessageBox("�ļ�·����Ч��");
			break;

		case FFC_STATUS_FILE_OPEN_ERROR:
			MessageBox("���ļ�ʧ�ܣ�");
			break;

		case FFC_STATUS_FILE_WRITE_ERROR:
			MessageBox("����д��ʧ�ܣ�");
			break;

		default: 
			break;
		} //end of switch (emStatus)
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   ˢ����ʾ��ƽ��ϵ��

\return  ��
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::RefreshWhiteRatio()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_FLOAT_VALUE    stValue;
	CString   strTemp  = _T("");

	// ��ȡ��ƽ��ͨ����Ӧ�İ�ƽ��ϵ��
	emStatus = GXGetFloatValue(m_hDevice, "BalanceRatio", &stValue);
	GX_VERIFY(emStatus);

	m_dBalanceRatio = stValue.dCurValue;
	strTemp.Format("%.4f",stValue.dCurValue);
	SetDlgItemText(IDC_EDIT_BALANCE_RATIO, strTemp);
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ�����ͼ��

\return  ��
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::AcquireDarkFieldImg(void)
{
	// ��ɼ��İ���ͼ���ƽ���Ҷ�ֵ
	m_nAverageGrayDarkImg = (int)GetAverageGray8bit(m_pDarkBuf, m_i64ImageWidth * m_i64ImageHeight);
	if (GRAY_INVALID == m_nAverageGrayDarkImg)
	{
		m_bDarkImgAcquiredOnce = false;
		MessageBox("����ͼ��ɼ�ʧ�ܣ������²ɼ���");
	}
	else if (m_nAverageGrayDarkImg > BRIGHT_IMG_GRAY_MIN)
	{
		m_bDarkImgAcquiredOnce = false;
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("����ͼ�������ƽ��У������ʹ�øð���ͼ���ⲻ��Ӱ��ƽ��У����Ч��");
	} 
	else
	{
		m_bDarkImgAcquiredOnce = true;
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("�ɼ�����ͼ�����");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ�����ͼ��

\return  ��
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::AcquireBrightFieldImg(void)
{
	DX_STATUS emStatus = DX_OK;
	int64_t   nImgSize = 0;
	nImgSize = m_i64ImageWidth * m_i64ImageHeight;

	//��ɼ�������ͼ���ƽ���Ҷ�ֵ
	do 
	{
		//��ȡ�ۼ�����ͼ��
		emStatus = AccumulateImage(m_pBrightBuf, m_nActualBits, nImgSize, m_pAccumulateImgBuf);
		if (DX_OK != emStatus)
		{
			m_nBrightImgNum = 0;
			m_bBrightImgAcquired = false;
			MessageBox("�ۼ�����ͼ����Ч�������²ɼ�����ͼ��");
			break;
		}

		m_nBrightImgNum++;
		if (m_nFrameCount == m_nBrightImgNum)
		{
			//��ȡƽ������ͼ��
			emStatus = GetAverageImage(m_pAccumulateImgBuf, nImgSize, m_nBrightImgNum, m_nActualBits, m_pAverageImgBuf);
			if (DX_OK != emStatus)
			{
				m_nBrightImgNum = 0;
				m_bBrightImgAcquired = false;
				MessageBox("ƽ������ͼ����Ч�������²ɼ�����ͼ��");
				break;
			}

			// ������ͼ��ƽ���Ҷ�ֵ�������ж�
			m_nAverageGrayBrightImg = (int)GetAverageGray8bit(m_pAverageImgBuf, nImgSize);
			if (GRAY_INVALID == m_nAverageGrayBrightImg)
			{
				MessageBox("����ͼ��ɼ�ʧ�ܣ������²ɼ���");
			}
			else if (m_nAverageGrayBrightImg < BRIGHT_IMG_GRAY_MIN)
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("����ͼ��������⽫��Ӱ��ƽ��У��Ч�������齫ͼ������ֵ������20-250֮��");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			} 
			else if (m_nAverageGrayBrightImg > BRIGHT_IMG_GRAY_MAX)
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("����ͼ��������⽫��Ӱ��ƽ��У��Ч�������齫ͼ������ֵ������20-250֮��");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			} 
			else
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("�ɼ�����ͼ�����");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			}

			m_nBrightImgNum = 0;
			m_bBrightImgAcquired = false;
		}
	} while (0);
}