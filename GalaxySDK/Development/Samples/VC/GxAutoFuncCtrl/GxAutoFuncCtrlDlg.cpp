// GxAutoFuncCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxAutoFuncCtrl.h"
#include "GxAutoFuncCtrlDlg.h"

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
// CGxAutoFuncCtrlDlg dialog

CGxAutoFuncCtrlDlg::CGxAutoFuncCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxAutoFuncCtrlDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bIsSnap(FALSE)
	, m_bImplementAutoGain(false)
	, m_bImplementAutoShutter(false)
	, m_bImplementLight(false) 
	, m_nGray(0)
	, m_nRoiX(0)
	, m_nRoiW(0)
	, m_nRoiH(0)
	, m_nRoiY(0)
	, m_dAutoGainMin(0)
	, m_dAutoShutterMax(0)
	, m_dAutoGainMax(0)
	, m_dAutoShutterMin(0)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_emAutoShutterMode(GX_EXPOSURE_AUTO_OFF)
	, m_emAutoGainMode(GX_GAIN_AUTO_OFF)
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxAutoFuncCtrlDlg)
	m_dShutterValue = 0.0;
	m_dGainValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//��ʼ������
	memset(&m_stShutterFloatRange, 0, sizeof(m_stShutterFloatRange));
	memset(&m_stGainFloatRange, 0, sizeof(m_stGainFloatRange));
}

void CGxAutoFuncCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxAutoFuncCtrlDlg)
	DDX_Text(pDX, IDC_EDIT_GRAY, m_nGray);
	DDX_Text(pDX, IDC_EDIT_ROI_X, m_nRoiX);
	DDX_Text(pDX, IDC_EDIT_ROI_Y, m_nRoiY);
	DDX_Text(pDX, IDC_EDIT_ROI_H, m_nRoiH);
	DDX_Text(pDX, IDC_EDIT_ROI_W, m_nRoiW);
	DDX_Text(pDX, IDC_EDIT_AUTO_GAIN_MIN, m_dAutoGainMin);
	DDX_Text(pDX, IDC_EDIT_AUTO_GAIN_MAX, m_dAutoGainMax);
	DDX_Text(pDX, IDC_EDIT_AUTO_SHUTTER_MIN, m_dAutoShutterMin);
	DDX_Text(pDX, IDC_EDIT_AUTO_SHUTTER_MAX, m_dAutoShutterMax);
	DDX_Text(pDX, IDC_EDIT_CUR_SHUTTER, m_dShutterValue);
	DDX_Text(pDX, IDC_EDIT_CUR_GAIN, m_dGainValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxAutoFuncCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CGxAutoFuncCtrlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT, OnCbnSelchangeComboLight)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_GAIN, OnCbnSelchangeComboAutoGain)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTO_SHUTTER, OnCbnSelchangeComboAutoShutter)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_ROI_X, OnKillfocusEditRoiX)
	ON_EN_KILLFOCUS(IDC_EDIT_ROI_Y, OnKillfocusEditRioY)
	ON_EN_KILLFOCUS(IDC_EDIT_ROI_W, OnKillfocusEditRoiW)
	ON_EN_KILLFOCUS(IDC_EDIT_ROI_H, OnKillfocusEditRoiH)
	ON_EN_KILLFOCUS(IDC_EDIT_AUTO_GAIN_MAX, OnKillfocusEditAutoGainMax)
	ON_EN_KILLFOCUS(IDC_EDIT_AUTO_GAIN_MIN, OnKillfocusEditAutoGainMin)
	ON_EN_KILLFOCUS(IDC_EDIT_AUTO_SHUTTER_MAX, OnKillfocusEditAutoShutterMax)
	ON_EN_KILLFOCUS(IDC_EDIT_AUTO_SHUTTER_MIN, OnKillfocusEditAutoShutterMin)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_CUR_SHUTTER, OnKillfocusEditCurShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_CUR_GAIN, OnKillfocusEditCurGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxAutoFuncCtrlDlg message handlers

BOOL CGxAutoFuncCtrlDlg::OnInitDialog()
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
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//��ʼ���豸��
	emStatus = GXInitLib();  
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}
	
	//��ȡͼ����ʾ���ڵ�ָ��ͻ�ͼDC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);

	// ���½���ؼ�
	UpDateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxAutoFuncCtrlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
        GetModuleFileName(NULL, strFileName, MAX_PATH);
        CFileVersion fv(strFileName);
        CAboutDlg dlgAbout;		
        dlgAbout.m_strProductVersion = _T("Version:") + fv.GetProductVersion();
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

void CGxAutoFuncCtrlDlg::OnPaint() 
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
HCURSOR CGxAutoFuncCtrlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   ���"���豸"��ť��Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
  	GX_STATUS     emStatus    = GX_STATUS_SUCCESS;
	uint32_t      nDevNum     = 0;
	GX_OPEN_PARAM stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// ��ʼ���豸�򿪲���
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// ö���豸
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		MessageBox("δ�����豸!");
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

		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
		GX_VERIFY(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
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
	
	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�,���ݸ�ʽ:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// ��ʼ��ͼ����ʾģ��
	m_pBitmap = new CGXBitmap(m_hDevice, m_pWnd);
	if (NULL == m_pBitmap)
	{
		ShowErrorString(GX_STATUS_ERROR);
		return;
	}


	// ��ȡ�������,��ʼ������ؼ�
	InitUI();

	// ���½���ؼ�
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"��ʼ�ɼ�"��ť��Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnStartSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	emStatus = m_pBitmap->PrepareForShowImg();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
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

	SetTimer(0,1000,NULL);

	// ���½���UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����:����ͼ���ȡ����ʾ����
\param   pFrame   �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxAutoFuncCtrlDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxAutoFuncCtrlDlg *pDlg = (CGxAutoFuncCtrlDlg*)(pFrame->pUserParam);

	if (GX_FRAME_STATUS_SUCCESS == pFrame->status)
	{	
		pDlg->m_pBitmap->Show(pFrame);
	}


}

//---------------------------------------------------------------------------------
/**
\brief   ���"ֹͣ�ɼ�"��ť��Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnStopSnap() 
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
    
	m_pBitmap->UnPrepareForShowImg();

	// ֹͣTimer
	KillTimer(0);

	// ���½���UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"�ر��豸"��ť��Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnCloseDevice() 
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
		m_bIsSnap = FALSE;

		// ֹͣTimer
		KillTimer(0);
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

	m_bDevOpened = FALSE;
	m_hDevice    = NULL;

	// ���½���UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   chFeature    [in]    ������ID
\param   pComboBox      [in]    �ؼ�ָ��
\param   bIsImplemented [in]    ��ʶ�豸�Ƿ�֧��chFeature����Ĺ���

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitEnumUI(char* strFeatureName, CComboBox *pComboBox, bool bIsImplemented)
{
	// �жϿؼ�
	if ((pComboBox == NULL) || (!bIsImplemented))
	{
		return;
	}
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	uint32_t  nEntryNum      = 0;
	int       nCurcel        = 0;

	// ��ȡ���ܵ�ö����
	GX_ENUM_VALUE stEnumValue;
	emStatus = GXGetEnumValue(m_hDevice,  strFeatureName, &stEnumValue);
	GX_VERIFY(emStatus);

	nEntryNum = stEnumValue.nSupportedNum;

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{	
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCurcel = i;
		}
	}

	// ���õ�ǰֵΪ�������ʾֵ
	pComboBox->SetCurSel(nCurcel);
}

//---------------------------------------------------------------------------------
/**
\brief   �����ʼ��

\return  ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxAutoFuncCtrlDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice,"AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   UI�����ʼ��

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// ��ʼ���Զ�����Combox�ؼ�
    InitAutoGainUI();

	// ��ʼ���Զ��ع�Combox�ؼ�
	InitAutoShutterUI();

	// ��ʼ��2A���ջ���Combox�ؼ�
	InitLightEnvironmentUI();

	// ��ʼ�������Ҷ�ֵ��ؿؼ���
    InitGrayUI();

	// ��ʼ������Ȥ����ĳ�ʼֵ
	InitROIUI();

	// ��ʼ���ع�ʱ����ؿؼ�
	InitShutterUI();

	// ��ʼ������ֵ��ؿؼ�
	InitGainUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���Զ�����Combox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitAutoGainUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;

	// �ж��豸�Ƿ�֧���Զ�����
	emStatus = GXGetNodeAccessMode(m_hDevice, "GainAuto", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementAutoGain = true;
	}

	//��ȡ�Զ�����ģʽ
	emStatus = GXGetEnumValue(m_hDevice,"GainAuto",&stEnumValue);
	GX_VERIFY(emStatus);

	m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)stEnumValue.nArrySupportedValue->nCurValue;

	// ��ʼ���û��Զ�����Combox��
	InitEnumUI("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN), m_bImplementAutoGain);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���Զ��ع�Combox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitAutoShutterUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;

	// �ж��豸�Ƿ�֧���Զ��ع�ģʽ	
	emStatus = GXGetNodeAccessMode(m_hDevice, "ExposureAuto", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementAutoShutter = true;
	}
	
	//��ȡ�Զ��ع�ģʽ
	emStatus = GXGetEnumValue(m_hDevice,"ExposureAuto",&stEnumValue);
	GX_VERIFY(emStatus);

	m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)stEnumValue.nArrySupportedValue->nCurValue;

	// ��ʼ���û�IO���ģʽCombox��
	InitEnumUI("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER), m_bImplementAutoShutter);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��2A���ջ���Combox�ؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitLightEnvironmentUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
    GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

	// �ж��豸�Ƿ�֧��2A���ջ���	
	emStatus = GXGetNodeAccessMode(m_hDevice, "AALightEnvironment", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementLight = true;
	}
	

	// ��ʼ��2A���ջ���
	InitEnumUI("AALightEnvironment", (CComboBox *)GetDlgItem(IDC_COMBO_LIGHT), m_bImplementLight);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع���ؿؼ�:�༭�򼰾�̬��

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_CUR_SHUTTER);

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	GX_FLOAT_VALUE stValue;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);
    
	// UI��ʾ����
	strTemp.Format("�ع�(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// ��ȡ��ǰֵ������ǰֵ���µ�����
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	// ��ȡ�Զ��ع����ֵ�ĵ�ǰֵ
	emStatus = GXGetFloatValue(m_hDevice, "AutoExposureTimeMax", &stValue);
	GX_VERIFY(emStatus);
	m_dAutoShutterMax = stValue.dCurValue;
	strTemp.Format("%.4f",m_dAutoShutterMax);
	SetDlgItemText(IDC_EDIT_AUTO_SHUTTER_MAX,strTemp);

	// ��ȡ�Զ��ع���Сֵ�ĵ�ǰֵ
	emStatus = GXGetFloatValue(m_hDevice, "AutoExposureTimeMin", &stValue);
	GX_VERIFY(emStatus);
	m_dAutoShutterMin = stValue.dCurValue;
	strTemp.Format("%.4f",m_dAutoShutterMin);
	SetDlgItemText(IDC_EDIT_AUTO_SHUTTER_MIN,strTemp);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ� 
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitGainUI()
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STATIC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_CUR_GAIN);

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strTemp  = "";
	GX_FLOAT_VALUE    stGainVal;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// UI��ʾ����
	strTemp.Format("����(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strTemp);
	
	// ��ȡ��ǰֵ�����µ�����
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &stGainVal);
	GX_VERIFY(emStatus);
	m_dGainValue= m_stGainFloatRange.dCurValue;

	// ��ȡ�Զ��������ֵ�ĵ�ǰֵ
    emStatus = GXGetFloatValue(m_hDevice, "AutoGainMax", &stGainVal);
	GX_VERIFY(emStatus);
	m_dAutoGainMax = stGainVal.dCurValue;
    strTemp.Format("%.4f",m_dAutoGainMax);
 	SetDlgItemText(IDC_EDIT_AUTO_GAIN_MAX,strTemp);

	// ��ȡ�Զ�������Сֵ�ĵ�ǰֵ
	emStatus = GXGetFloatValue(m_hDevice, "AutoGainMin", &stGainVal);
	GX_VERIFY(emStatus);
	m_dAutoGainMin = (int)stGainVal.dCurValue;
	strTemp.Format("%.4f",m_dAutoGainMin);
	SetDlgItemText(IDC_EDIT_AUTO_GAIN_MIN,strTemp);
	
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ�������Ҷ�ֵ��ؿؼ�
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitROIUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE   stValue;
	
	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetY", &stValue);
	GX_VERIFY(emStatus);
	m_nRoiY = (int)stValue.nCurValue;

	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetX", &stValue);
	GX_VERIFY(emStatus);
	m_nRoiX = (int)stValue.nCurValue;

	emStatus = GXGetIntValue(m_hDevice, "AAROIWidth", &stValue);
	GX_VERIFY(emStatus);
	m_nRoiW = (int)stValue.nCurValue;

	emStatus = GXGetIntValue(m_hDevice, "AAROIHeight", &stValue);
	GX_VERIFY(emStatus);
	m_nRoiH = (int)stValue.nCurValue;

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	UpdateROIRange();

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ȡROI�ķ�Χ��ʾ������
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::UpdateROIRange()
{
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE  nIntRange;
	CString       strTmep = _T("");

	emStatus = GXGetIntValue(m_hDevice, "AAROIWidth", &nIntRange);
	GX_VERIFY(emStatus)
    strTmep.Format("2A����Ȥ������(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
    SetDlgItemText(IDC_STATIC_ROI_W,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIHeight", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A����Ȥ����߶�(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_H,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetX", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A����Ȥ����X����(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_X,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetY", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A����Ȥ����Y����(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_Y,strTmep);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ�������Ҷ�ֵ��ؿؼ�
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitGrayUI()
{
	CSliderCtrl *pSliderCtrl  = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GRAY);
	CStatic     *pStcGray     = (CStatic *)GetDlgItem(IDC_STATIC_GRAY);
	CEdit       *pEditGrayVal = (CEdit *)GetDlgItem(IDC_EDIT_GRAY);

	if ((pSliderCtrl == NULL) || (pStcGray == NULL) || (pEditGrayVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int32_t   nStep    = 0;
	CString   strRange = "";
	GX_INT_VALUE stIntRange;

	// ��ȡ�����Ҷ�ֵ��Χ
	emStatus = GXGetIntValue(m_hDevice, "ExpectedGrayValue", &stIntRange);
	GX_VERIFY(emStatus);
	pSliderCtrl->SetRange((int)stIntRange.nMin, (int)stIntRange.nMax, true);

	// ��ʾ��Χ
    strRange.Format("�����Ҷ�ֵ(%I64d~%I64d)", stIntRange.nMin, stIntRange.nMax);
	pStcGray->SetWindowText(strRange);

	pSliderCtrl->SetPos((int)stIntRange.nCurValue);
	m_nGray = (int)stIntRange.nCurValue;
   
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"���ջ���"Comboxѡ����Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboLight() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_LIGHT);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ


	emStatus = GXSetEnumValue(m_hDevice, "AALightEnvironment", nEnumVal);	
	GX_VERIFY(emStatus);
	
	if (((GX_AA_LIGHT_ENVIRMENT_AC50HZ == nEnumVal) 
		|| (GX_AA_LIGHT_ENVIRMENT_AC60HZ == nEnumVal))
		&&((m_emAutoShutterMode != GX_EXPOSURE_AUTO_CONTINUOUS) 
		|| (m_emAutoGainMode != GX_GAIN_AUTO_CONTINUOUS)))
	{
		MessageBox("50hz�������60hz����������£�\n����ͬʱ�����Զ��ع���Զ����棬��ʱ�Զ����ܲ�����Ч.","Msg");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"�Զ�����"Comboxѡ����Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboAutoGain() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
    
	emStatus = GXSetEnumValue(m_hDevice, "GainAuto", nEnumVal);	
	GX_VERIFY(emStatus);
    m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nEnumVal;

 	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"�Զ��ع�"Comboxѡ����Ϣ��Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboAutoShutter() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
    
	emStatus = GXSetEnumValue(m_hDevice, "ExposureAuto", nEnumVal);	
	GX_VERIFY(emStatus);
	m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nEnumVal;
 
 	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ������������Ϣ��Ӧ����
\param   nSBCode     ָ�������������Ĵ���
\param   nPos        ������λ��
\param   pScrollBar  �������ؼ�ָ��

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS    emStatus = GX_STATUS_ERROR;
	int64_t      nValue   = 0;
	double       dValue   = 0;
	CString      strTemp  = "";
	CSliderCtrl *pSlider  = (CSliderCtrl *)pScrollBar;

	// ��ȡ��ǰ������λ��
	nValue = pSlider->GetPos();

	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_GRAY:  // ���������ҶȻ�����
		emStatus = GXSetIntValue(m_hDevice, "ExpectedGrayValue", nValue);
		m_nGray = (int)nValue;
		break;
	default:
		break;
	}	

	GX_VERIFY(emStatus);
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//---------------------------------------------------------------------------------
/**
\brief   ��Ϣ���ɺ���(��Ҫ����س���Ϣ)
\param   pMsg  ��Ϣ�ṹ��

\return  BOOL
*/
//----------------------------------------------------------------------------------
BOOL CGxAutoFuncCtrlDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd  *pWnd     = NULL;
	int   nCtrlID   = 0;

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		nCtrlID = pWnd->GetDlgCtrlID();
		switch(nCtrlID)
		{
		case IDC_EDIT_AUTO_GAIN_MAX:
		case IDC_EDIT_AUTO_GAIN_MIN:
		case IDC_EDIT_AUTO_SHUTTER_MAX:
		case IDC_EDIT_AUTO_SHUTTER_MIN:
		case IDC_EDIT_ROI_X:
		case IDC_EDIT_ROI_Y:
		case IDC_EDIT_ROI_W:
		case IDC_EDIT_ROI_H:
		case IDC_EDIT_GRAY: 
		case IDC_EDIT_CUR_SHUTTER:
		case IDC_EDIT_CUR_GAIN:

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

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:����2A����Ȥ����X����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiX() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIOffsetX", m_nRoiX);
	GX_VERIFY(emStatus);

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:����2A����Ȥ����Y����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRioY() 
{
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIOffsetY", m_nRoiY);
	GX_VERIFY(emStatus);

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:����2A����Ȥ�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiW() 
{
	// TODO: Add your control notification handler code here	
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIWidth", m_nRoiW);
	GX_VERIFY(emStatus);

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:����2A����Ȥ�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiH() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIHeight", m_nRoiH);
	GX_VERIFY(emStatus);

	// ��ȡ����Ȥ��������ķ�Χ����ʾ������
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:�����Զ��������ֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoGainMax() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//�������ֵ�����Զ���������ֵ���Զ���������ֵ���ó���������ֵ
	if (m_dAutoGainMax > m_stGainFloatRange.dMax)
	{
		m_dAutoGainMax = m_stGainFloatRange.dMax;
	}

	emStatus = GXSetFloatValue(m_hDevice, "AutoGainMax", m_dAutoGainMax);
	GX_VERIFY(emStatus);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:�����Զ�������Сֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoGainMin() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

    GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//�������ֵС���Զ��������Сֵ���Զ��������Сֵ���ó��������Сֵ
	if (m_dAutoGainMin < m_stGainFloatRange.dMin)
	{
		m_dAutoGainMin = m_stGainFloatRange.dMin;
	}

	emStatus = GXSetFloatValue(m_hDevice, "AutoGainMin", m_dAutoGainMin);
	GX_VERIFY(emStatus);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:�����Զ��ع����ֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoShutterMax() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//�������ֵ�����Զ��ع�����ֵ���Զ��ع�����ֵ���ó��ع�����ֵ
	if (m_dAutoShutterMax > m_stShutterFloatRange.dMax)
	{
		m_dAutoShutterMax = m_stShutterFloatRange.dMax;
	}

	emStatus =GXSetFloatValue(m_hDevice,"AutoExposureTimeMax", m_dAutoShutterMax);
	GX_VERIFY(emStatus);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   Edit�ؼ�ʧȥ���㺯��:�����Զ��ع���Сֵ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoShutterMin() 
{
	// TODO: Add your control notification handler code here
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//�������ֵС���Զ��ع����Сֵ���Զ��ع����Сֵ���ó��ع����Сֵ
	if (m_dAutoShutterMin < m_stShutterFloatRange.dMin)
	{
		m_dAutoShutterMin = m_stShutterFloatRange.dMin;
	}

	emStatus =GXSetFloatValue(m_hDevice,"AutoExposureTimeMin", m_dAutoShutterMin);
	GX_VERIFY(emStatus);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ˢ����ʾ��ǰ���桢�ع�ֵ

\return  ��
*/
//---------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::RefreshCurValue()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CString   strTemp  = "";
	GX_FLOAT_VALUE    dValue;

	if (m_emAutoShutterMode != GX_EXPOSURE_AUTO_OFF)
	{
		//�����ǰѡ���ع�ΪONCEʱ����ȡ�豸��ʱ��ΪOFF�������½���
		if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_ONCE)
		{
			GX_ENUM_VALUE  nShutterEnumVal;   //<   �����ع��ö��ֵ
			
			//��ȡ�Զ��ع���Ͽ�ؼ���ָ��
			CComboBox *pAutoShutterBox    = (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER);
			
			//��ȡ�Զ��ع��ö��ֵ
			emStatus = GXGetEnumValue(m_hDevice, "ExposureAuto", &nShutterEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}
			
			//����ת��
			m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nShutterEnumVal.stCurValue.nCurValue;
			
			// �ж��豸�ع�ģʽ�Ƿ��ΪOFF
			if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_OFF)
			{
				for (int i = 0;i < pAutoShutterBox->GetCount();i++)
				{
					if ((int64_t)pAutoShutterBox->GetItemData(i) == GX_EXPOSURE_AUTO_OFF)
					{
						//ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
						pAutoShutterBox->SetCurSel(i);
						break;
					}
				}
				
				UpDateUI();
			}
		}

		//��ȡ��ǰ�ع�ֵ�����½���
		emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &dValue);
		m_dShutterValue = dValue.dCurValue;
		strTemp.Format("%.4f", m_dShutterValue);
		SetDlgItemText(IDC_EDIT_CUR_SHUTTER, strTemp);
	}
	
	if (m_emAutoGainMode != GX_GAIN_AUTO_OFF)
	{
		//�����ǰѡ������ΪONCEʱ����ȡ�豸��ʱ��ΪOFF�������½���
		if (m_emAutoGainMode == GX_GAIN_AUTO_ONCE)
		{
			GX_ENUM_VALUE  nGainEnumVal;   //<   ���������ö��ֵ
			
			//��ȡ�Զ�������Ͽ�ؼ���ָ��
			CComboBox *pAutoGainBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);
			
			//��ȡ�Զ������ö��ֵ
			emStatus = GXGetEnumValue(m_hDevice, "GainAuto", &nGainEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}
			
			// ����ת��
			m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nGainEnumVal.stCurValue.nCurValue;
			
			//�ж��豸����ģʽ�Ƿ��ΪOFF
			if (m_emAutoGainMode == GX_GAIN_AUTO_OFF)
			{
				for (int i = 0;i < pAutoGainBox->GetCount();i++)
				{
					if ((int64_t)pAutoGainBox->GetItemData(i) == GX_GAIN_AUTO_OFF)
					{
						//ѡ���Զ��ع�ؼ��е�OFF�����ONCE��ΪOFF
						pAutoGainBox->SetCurSel(i);
						break;
					}
				}
				
				UpDateUI();		
			}
		}

		// ��ȡ��ǰ����ֵ�����½���
		emStatus  = GXGetFloatValue(m_hDevice, "Gain", &dValue);
		m_dGainValue = dValue.dCurValue;
		strTemp.Format("%.4f", m_dGainValue);
		SetDlgItemText(IDC_EDIT_CUR_GAIN, strTemp);
	}	
}

//-----------------------------------------------------------
/**
\brief   �����Զ��ع�������ONCE��ΪOFF��ˢ�½���ֵ

\return  ��
*/
//-----------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	

	// ˢ����ʾ��ǰ���桢�ع�ֵ
	RefreshCurValue();

	CDialog::OnTimer(nIDEvent);
}

//---------------------------------------------------------------------------------
/**
\brief   �ر�Ӧ�ó�����ú���

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ֹͣTimer
	KillTimer(0);
	
	// ��δͣ������ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		m_bIsSnap = FALSE;

		m_pBitmap->UnPrepareForShowImg();
	}
	
	// δ�ر��豸��ر��豸
	if (m_bDevOpened)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = FALSE;
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
\brief   ˢ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);

	GetDlgItem(IDC_COMBO_AUTO_GAIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_COMBO_AUTO_SHUTTER)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_COMBO_LIGHT)->EnableWindow(m_bDevOpened && m_bImplementLight);
	GetDlgItem(IDC_SLIDER_GRAY)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_EDIT_ROI_Y)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_ROI_H)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_ROI_X)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_ROI_W)->EnableWindow(m_bDevOpened);


	GetDlgItem(IDC_EDIT_AUTO_GAIN_MAX)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_AUTO_GAIN_MIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_CUR_GAIN)->EnableWindow(m_bDevOpened && (m_emAutoGainMode == GX_GAIN_AUTO_OFF));

	GetDlgItem(IDC_EDIT_AUTO_SHUTTER_MIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_AUTO_SHUTTER_MAX)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_CUR_SHUTTER)->EnableWindow(m_bDevOpened && (m_emAutoShutterMode == GX_EXPOSURE_AUTO_OFF));
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in] ������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// ��ȡ������Ϣ���ȣ��������ڴ���Դ
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

	// �ͷ�������ڴ���Դ
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//---------------------------------------------------
/**
\brief   �ع�Edit��ؼ�ʧȥ�������Ӧ����

\return  ��
*/
//---------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditCurShutter() 
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

	status = GXSetFloatValue(m_hDevice,"ExposureTime", m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//-------------------------------------------------------
/**
\brief    ����Edit��ؼ�ʧȥ�������Ӧ����

\return   ��
*/
//-------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditCurGain() 
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
