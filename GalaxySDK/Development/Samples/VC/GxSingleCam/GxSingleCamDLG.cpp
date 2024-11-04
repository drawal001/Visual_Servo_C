// GxSingleCamColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CGxSingleCamDlg dialog

CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bIsSnap(FALSE)
	, m_bIsSaveImg(FALSE)
	, m_bIsImproveImg(FALSE)
	, m_pGammaLut(NULL)
	, m_pContrastLut(NULL)
	, m_nLutLength(0)
	, m_nContrast(0)
	, m_nColorCorrection(0)
	, m_dGamma(0.0)
	, m_bTriggerMode(false)
	, m_bTriggerActive(false)
	, m_bTriggerSource(false)
	, m_bBalanceRatioSelector(false)
	, m_nTriggerMode(GX_TRIGGER_MODE_OFF)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_nRatioSelector(-1)
	, m_nAutoWhiteBalance(0)
	, m_strFilePath("")
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxSingleCamDlg)
	m_dBalanceRatio = 0.0;
	m_dGainValue = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ������
	memset(&m_stGainFloatRange,0,sizeof(GX_FLOAT_VALUE));
	memset(&m_stShutterFloatRange,0,sizeof(GX_FLOAT_VALUE));
	memset(&m_stFloatRange,0,sizeof(GX_FLOAT_VALUE));
}

void CGxSingleCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxSingleCamDlg)
	DDX_Text(pDX, IDC_EDIT_BALANCE_RATIO, m_dBalanceRatio);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dShutterValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	//{{AFX_MSG_MAP(CGxSingleCamDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SOFTTRIGGER, OnBnClickedBtnSofttrigger)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnCbnSelchangeComboTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnCbnSelchangeComboWriteBlance)
    ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, OnCbnSelchangeComboTriggerSource)
    ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_ACTIVE, OnCbnSelchangeComboTriggerActive)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERPOLATION_ALGORITHM, OnCbnSelchangeComboInterpolationAlgorithm)
    ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
    ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
    ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
    ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
    ON_BN_CLICKED(IDC_CHECK_SAVE_BMP, OnBnClickedCheckSaveBmp)
	ON_BN_CLICKED(IDC_CHECK_IMAGE_IMPROVE, OnBnClickedCheckImageImprove)
    ON_CBN_SELCHANGE(IDC_COMBO_RATIO_SELECTOR, OnCbnSelchangeComboRatioSelector)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, OnEnKillfocusEditBalanceRatio)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxSingleCamDlg message handlers

BOOL CGxSingleCamDlg::OnInitDialog()
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
	//  when the application's main window is not a dialog
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

	// ��ȡ��ǰ����ִ��·��,����ͼ�񱣴��Ĭ��·��
	CString strDate = "";
	char    szName[MAX_PATH] = {0};
	CString strAppFullName = ""; 
	GetModuleFileName(NULL, szName, MAX_PATH);
	strAppFullName = szName;
	int nPos = strAppFullName.ReverseFind('\\');
	m_strFilePath += strAppFullName.Left(nPos);
	m_strFilePath += "\\GxSingleCamImages";

	//��ȡͼ����ʾ���ڵ�ָ��ͻ�ͼDC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_FRAME);


	// ���½���ؼ�
	UpDateUI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSingleCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxSingleCamDlg::OnPaint() 
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
HCURSOR CGxSingleCamDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   �ر�Ӧ�ó�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnClose() 
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
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;
	}

	// δ�ر��豸��ر��豸
	if (m_bDevOpened)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = FALSE;
		m_hDevice    = NULL;

		if (m_pGammaLut != NULL)
		{
			delete[] m_pGammaLut;
			m_pGammaLut = NULL;
		}
		if (m_pContrastLut != NULL)
		{
			delete[] m_pContrastLut;
			m_pContrastLut = NULL;
		}
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
void CGxSingleCamDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// ����ģʽ����ѡ��Combox�ؼ�
	InitTriggerModeUI();

	// ��ʼ������ԴCombox�ؼ�
	InitTriggerSourceUI();

	// ��ʼ����������Combox�ؼ�
	InitTriggerActivationUI();

	if (m_bColorFilter) 
	{
		// ��ʼ����ƽ�����Combox�ؼ�
		InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);

		// ��ʼ����ƽ��ͨ��Combox�ؼ�
		InitEnumUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR), m_bBalanceRatioSelector);

		// ��ʼ����ƽ��ϵ����ؿؼ�
		InitWhiteRatioUI();

		// ��ʼ����ֵ�㷨��ؿؼ�
		InitInterpolationAlgorithmUI((CComboBox *)GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM), m_bColorFilter);
	}
	else 
	{
		//��ƽ�����Combox�ؼ�����ƽ��ͨ��Combox�ؼ�����ƽ��ϵ����ؿؼ���ͼ������������ť����ֵ�㷨�ؼ���ʹ��
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(FALSE);
	}

	// ��ʼ���ع�ʱ����ؿؼ�
	InitShutterUI();

	// ��ʼ��������ؿؼ�
	InitGainUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����ƽ��ϵ����ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitWhiteRatioUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CString   strTemp  = _T("");

	if (!m_bBalanceRatioSelector)
	{
		return;
	}

	// ��ȡϵ����Χ
	emStatus = GXGetFloatValue(m_hDevice,"BalanceRatio",&m_stFloatRange);
    GX_VERIFY(emStatus);
	strTemp.Format("��ƽ��ϵ��(%.4f~%.4f)",m_stFloatRange.dMin,m_stFloatRange.dMax);
	SetDlgItemText(IDC_STATIC_WHITE_RATIO,strTemp);

	// ˢ����ʾ��ǰ��ƽ��ϵ��
	RefreshWhiteRatio();
}

//---------------------------------------------------------------------------------
/**
\brief   ˢ����ʾ��ƽ��ϵ��

\return  ��
*/
//---------------------------------------------------------------------------------
void CGxSingleCamDlg::RefreshWhiteRatio()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_FLOAT_VALUE  stValue;
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
\brief   ��ʼ������ģʽCombox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�ִ���ģʽ
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bTriggerMode = true;
	}
	
	if (!m_bTriggerMode)
	{
		return;
	}

	// ��ʼ������ģʽCombox��
	InitEnumUI("TriggerMode", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE), m_bTriggerMode);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������ԴCombox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerSourceUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�ִ���Դ
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerSource", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bTriggerSource = true;
	}
	
	if (!m_bTriggerSource)
	{
		return;
	}

	// ��ʼ������ԴCombox��
	InitEnumUI("TriggerSource", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),m_bTriggerSource);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����������Combox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerActivationUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�ִ�������
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerActivation", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bTriggerActive = true;
	}
	
	if (!m_bTriggerActive)
	{
		return;
	}

	// ��ʼ����������Combox��
	InitEnumUI("TriggerActivation", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE), m_bTriggerActive);
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
void CGxSingleCamDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement)
{
	// �жϿؼ�
	if ((pComboBox == NULL) || !bIsImplement)
	{
		return;
	}

	GX_ENUM_VALUE stEnumValue;
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	uint32_t  nEntryNum   = 0;
	int       nCursel     = 0;
	double    dValue      = 0;

	// ��ȡ���ܵ�ö����
	emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
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
\brief   ��ʼ����ֵ�㷨UI����
\param   pComboBox       [in]    �ؼ�ָ��
\param   bIsColorCamera  [in]    �豸�Ƿ��ɫ���

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitInterpolationAlgorithmUI(CComboBox *pComboBox, bool bIsColorCamera)
{
	// �жϿؼ�
	if (pComboBox == NULL)
	{
		return;
	}

	// ��տؼ�����
	pComboBox->ResetContent();

	// ����ǲ�ɫ���
	if (bIsColorCamera)
	{
		int nCurSel = 0;
		pComboBox->SetItemData(pComboBox->AddString("2��2�����ֵ(�ٶȿ�)"), 0);
		pComboBox->SetItemData(pComboBox->AddString("5��5��Ե����Ӧ(Ч����)"), 1);
		pComboBox->SetItemData(pComboBox->AddString("3��3�����ֵ(����)"), 2);
		pComboBox->SetItemData(pComboBox->AddString("5��5Ȩ�ز�ֵ(Ч���Ϻ�)"), 3);

		// ���õ�ǰֵΪ�������ʾֵ
		pComboBox->SetCurSel(nCurSel);
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �ص�����
\param   pFrame         �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxSingleCamDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxSingleCamDlg *pDlg = (CGxSingleCamDlg*)(pFrame->pUserParam);

	if (pFrame->status == 0)
	{
		if (pDlg->m_bIsImproveImg) 
		{
			pDlg->GetImageImproveParam();
			pDlg->m_pBitmap->ShowAfterImprovment(pFrame, pDlg->m_nColorCorrection, pDlg->m_pContrastLut, pDlg->m_pGammaLut);
		}
		else 
		{
			pDlg->m_pBitmap->Show(pFrame);
		}


		// ͼ�񱣴�
		if (pDlg->m_bIsSaveImg)
		{
			pDlg->SaveImage();
		}
	}
}


//---------------------------------------------------------------------------------
/**
\brief   ��ȡͼ��������������

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::GetImageImproveParam()
{
	if (!m_bColorFilter)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	VxInt32   nStatus = DX_OK;
	bool      bIsImplemented = false;
	GX_INT_VALUE stIntValue;
	GX_FLOAT_VALUE stFloatValue;

	// �ж�����Ƿ�֧�ֶԱȶȻ�ȡ
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "ContrastParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡ�ԱȶȲ���
		emStatus = GXGetIntValue(m_hDevice, "ContrastParam", &stIntValue);
		GX_VERIFY(emStatus);
		m_nContrast = stIntValue.nCurValue;
	}
	else
	{
		m_nContrast = DEFAULT_CONTRAST;
	}

	// �ж�����Ƿ�֧��Gamma��ȡ
	emStatus = GXGetNodeAccessMode(m_hDevice, "GammaParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡGamma����
		emStatus = GXGetFloatValue(m_hDevice, "GammaParam", &stFloatValue);
		GX_VERIFY(emStatus);
		m_dGamma = stFloatValue.dCurValue;
	}
	else
	{
		m_dGamma = DEFAULT_GAMMA;
	}

	// �ж�����Ƿ�֧����ɫУ��������ȡ
	emStatus = GXGetNodeAccessMode(m_hDevice, "ColorCorrectionParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡ��ɫУ������
		emStatus = GXGetIntValue(m_hDevice, "ColorCorrectionParam", &stIntValue);
		GX_VERIFY(emStatus);
		m_nColorCorrection = stIntValue.nCurValue;
	}
	else
	{
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	do
	{
		// ��ȡGamma���ұ���
		nStatus = DxGetGammatLut(m_dGamma, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		m_pGammaLut = new BYTE[m_nLutLength];
		if (m_pGammaLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// ����Gamma���ұ�
		nStatus = DxGetGammatLut(m_dGamma, m_pGammaLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		// ��ȡ�ԱȶȲ��ұ���
		nStatus = DxGetContrastLut((int)m_nContrast, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		m_pContrastLut = new BYTE[m_nLutLength];
		if (m_pContrastLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// ����ԱȶȲ��ұ�
		nStatus = DxGetContrastLut((int)m_nContrast, m_pContrastLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

	} while (0);

	// ���ò��ұ�ʧ�ܣ��ͷ���Դ
	if (nStatus != DX_OK)
	{
		if (m_pGammaLut != NULL)
		{
			delete[] m_pGammaLut;
			m_pGammaLut = NULL;
		}
		if (m_pContrastLut != NULL)
		{
			delete[] m_pContrastLut;
			m_pContrastLut = NULL;
		}
		MessageBox("��ȡͼ������ұ����ʧ�ܣ�");
		return;
	}
}

//----------------------------------------------------------------------------------
/**
\brief   ��m_pBufferRGBͼ�����ݱ����BMPͼƬ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::SaveImage() 
{
	CString strFileName = "";               // ͼ��洢·��
	SYSTEMTIME  sysTime;                    // ��ȡ��ǰʱ��
	GetLocalTime(&sysTime);

	//��������ͼ����ļ���
	BOOL bRet = CreateDirectory(m_strFilePath, NULL);
	if (bRet)
	{
		SetFileAttributes(m_strFilePath, FILE_ATTRIBUTE_NORMAL);
	}

	// ��ȡ��ǰʱ��Ϊͼ�񱣴��Ĭ������
	strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.bmp",
		m_strFilePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);

	char *pFilePath = strFileName.GetBuffer(strFileName.GetLength());
	m_pBitmap->SaveBmp(pFilePath);
	m_strFilePath.ReleaseBuffer();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"������ͼ"��ť��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnSofttrigger() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//������������(�ڴ���ģʽ����ʱ��Ч)
	emStatus = GXSetCommandValue(m_hDevice, "TriggerSoftware");
    GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"����ģʽ"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool bIsWritable = TRUE;
	GX_ENUM_VALUE stEnumValue;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE);

	int      nIndex    = pBox->GetCurSel();                     // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // ��ȡ��ǰѡ���Ӧ��ö����ֵ


	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsWritable = true;
	}

	emStatus = GXGetEnumValue(m_hDevice, "TriggerMode", &stEnumValue);
	GX_VERIFY(emStatus);
	if (bIsWritable)
	{
		emStatus = GXSetEnumValue(m_hDevice, "TriggerMode", nEnumVal);
		GX_VERIFY(emStatus);

		// ���µ�ǰ����ģʽ
		m_nTriggerMode = nEnumVal;
	}
	else
	{
		MessageBox("��ǰ״̬����д����ֹͣ�ɼ�������������!");
		if (stEnumValue.stCurValue.nCurValue != nEnumVal)
		{
			if (GX_TRIGGER_MODE_ON == nEnumVal)
			{
				for (int i = 0;i < pBox->GetCount();i++)
				{
					if (GX_TRIGGER_MODE_OFF == pBox->GetItemData(i))
					{
						pBox->SetCurSel(i);
						break;
					}
				}
			}
			else
			{
				for (int i = 0;i < pBox->GetCount();i++)
				{
					if (GX_TRIGGER_MODE_ON == pBox->GetItemData(i))
					{
						pBox->SetCurSel(i);
						break;
					}
				}
			}
		}
		else
		{
			return;
		}
	}

	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"�Զ���ƽ��"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
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
void CGxSingleCamDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;

	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
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
void CGxSingleCamDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE)->EnableWindow(m_bDevOpened && m_bTriggerActive);
	GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(m_bDevOpened && m_bTriggerMode);
	GetDlgItem(IDC_COMBO_TRIGGER_SOURCE)->EnableWindow(m_bDevOpened && m_bTriggerSource);
    

	if (m_bColorFilter) 
	{
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bDevOpened && m_bColorFilter);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(m_bDevOpened && m_bBalanceRatioSelector);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(m_bDevOpened);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(m_bDevOpened
			&& (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
			&& m_bBalanceRatioSelector);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(m_bDevOpened && m_bColorFilter);
	}
	else 
	{
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(FALSE);
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ͼ�������������أ��������˿���ʱ�����ڻص������ж�ÿһ֡ͼ�����
        ��ɫУ�����ԱȶȺ�Gamma�ĵ��ڣ��������ͼ���������

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedCheckImageImprove() 
{
	// TODO: Add your control notification handler code here
	m_bIsImproveImg = !m_bIsImproveImg;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus = GX_STATUS_SUCCESS;
	uint32_t       nDevNum  = 0;
	uint32_t	  nDSNum	  = 0;
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
	m_bDevOpened = TRUE;

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}

	// �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
	// �������������Ĳɼ�����,���÷����ο����´��롣
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
		GX_VERIFY(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
			&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
			&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
		{
			bImplementPacketSize = true;
		}
		

		if (bImplementPacketSize)
		{
			// ��ȡ��ǰ���绷�������Ű���ֵ
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			GX_VERIFY(emStatus);

			// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	do
	{
		// ��ʼ��ͼ����ʾģ��
		m_pBitmap = new CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// ��ȡ�豸��������Ϣ
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

	// ��ȡͼ��������������
	GetImageImproveParam();

	//���ö�ʱ�����½���
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
GX_STATUS CGxSingleCamDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ŀ�ߵ�������Ϣ

\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxSingleCamDlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bColorFliter   = false;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;
	int64_t i64PixelFormat = 0;

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
	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(i64PixelFormat));

	//�ж�����Ƿ�֧�ְ�ƽ��ͨ��
	emStatus = GXGetNodeAccessMode(m_hDevice, "BalanceRatioSelector", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);
	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		//��ȡ�Զ���ƽ��ö��ֵ
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

		m_bBalanceRatioSelector = true;
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// δͣ��ʱ��ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		//����ֹͣ�ɼ�����
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

		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;

	}

	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	if (m_pGammaLut != NULL)
	{
		delete[] m_pGammaLut;
		m_pGammaLut = NULL;
	}
	if (m_pContrastLut != NULL)
	{
		delete[] m_pContrastLut;
		m_pContrastLut = NULL;
	}
	m_bDevOpened = FALSE;
	m_hDevice    = NULL;

	if (NULL != m_pBitmap) 
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	KillTimer(0);

	// ���½���UI
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	//ע���ص�
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	m_bIsSnap = FALSE;

	// ���½���UI
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// Ϊͼ����ʾ׼����Դ
	emStatus = m_pBitmap->PrepareForShowImg();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}
	//ע��ص�
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	//����ʼ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	m_bIsSnap = TRUE;

	// ���½���UI
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"����Դ"ѡ����Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	emStatus = GXSetEnumValue(m_hDevice, "TriggerSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"��������"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	emStatus = GXSetEnumValue(m_hDevice, "TriggerActivation", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"����BMPͼ��"CheckBox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedCheckSaveBmp()
{
	// TODO: Add your control notification handler code here
	m_bIsSaveImg = !m_bIsSaveImg;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ�: Static:��ʾ��Χ Edit:����ֵ
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitGainUI()
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
	double    dGainVal = 0;

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
void CGxSingleCamDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);
	int          nPos            = 0;

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString         strTemp = "";
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
\brief   �л�"��ƽ��ͨ��"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboRatioSelector()
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
void CGxSingleCamDlg::OnEnKillfocusEditBalanceRatio()
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
BOOL CGxSingleCamDlg::PreTranslateMessage(MSG* pMsg)
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
void CGxSingleCamDlg::OnKillfocusEditShutter() 
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
	
	status = GXSetFloatValue(m_hDevice,"ExposureTime",m_dShutterValue);
	GX_VERIFY(status);


	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   ��������ֵ��Edit��ʧȥ�������Ӧ����

\return  ��
*/
//--------------------------------------------------------------------
void CGxSingleCamDlg::OnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
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
	
	status = GXSetFloatValue(m_hDevice,"Gain",m_dGainValue);
	GX_VERIFY(status);
	
	UpdateData(FALSE);
}

//----------------------------------------------------------------
/**
\brief  ��ʱ������:��ʱ��ȡ�Զ���ƽ���ö��ֵʵ������Once�ɹ�������Զ�����Ϊoff
\param  ��ʱ���¼�ID

\return ��
*/
//----------------------------------------------------------------
void CGxSingleCamDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS status = GX_STATUS_ERROR;
	GX_ENUM_VALUE stEnumValue;

	//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
    //����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		int64_t nWBEnumVal = 0;   //< �����ƽ���ö��ֵ

		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);

		//��ȡ�Զ���ƽ��ö��ֵ
		status = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (status != GX_STATUS_SUCCESS)
		{
			return;
		}

		nWBEnumVal = stEnumValue.stCurValue.nCurValue;
		m_nAutoWhiteBalance = nWBEnumVal;

		//�ж��豸�Զ���ƽ���Ƿ��ΪOFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
					if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
					{
						// ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
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
\brief   �л�"��ֵ�㷨"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboInterpolationAlgorithm()
{
	// TODO: Add your control notification handler code here
	int nIndex = 0;
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM);
	nIndex = pBox->GetCurSel();                   // ��ȡ��ǰѡ��

	m_pBitmap->SetInterpolationAlgorithm((DX_BAYER_CONVERT_TYPE)nIndex);
}
