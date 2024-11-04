// GxIOSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxIOSample.h"
#include "GxIOSampleDlg.h"

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
// CGxIOSampleDlg dialog

CGxIOSampleDlg::CGxIOSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxIOSampleDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bIsSnap(FALSE)
	, m_nTriggerMode(GX_TRIGGER_MODE_OFF)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_bTriggerMode(false)     
	, m_bTriggerActive(false)   
	, m_bTriggerSource(false)   
	, m_bOutputSelector(false)  
	, m_bOutputMode(false)      
	, m_bOutputValue(false)     
	, m_bStrobeSwitch(false) 
	, m_pWnd(NULL)
	, m_bLineSelector(FALSE)
	, m_bLineMode(FALSE)
	, m_bLineInverter(FALSE)
	, m_bLineSource(FALSE)
	, m_bStrobeSwitchIsUse(TRUE)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxIOSampleDlg)
	m_dGainValue = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ��ʼ������
	memset(&m_stShutterFloatRange, 0, sizeof(m_stShutterFloatRange));
	memset(&m_stGainFloatRange, 0, sizeof(m_stGainFloatRange));

}

void CGxIOSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxIOSampleDlg)
	DDX_Text(pDX, IDC_EDIT_GAIN_VALUE, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER_VALUE, m_dShutterValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxIOSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CGxIOSampleDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_SELECTOR, OnCbnSelchangeComboOutputSource)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_MODE, OnCbnSelchangeComboOutputMode)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_VALUE, OnCbnSelchangeComboOutputValue)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, OnCbnSelchangeComboTriggerSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnCbnSelchangeComboTriggerMode)
	ON_BN_CLICKED(IDC_BTN_SEND_SOFT_TRIGGER, OnBnClickedBtnSendSoftTrigger)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_ACTIVATION, OnCbnSelchangeComboTriggerActivation)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_STROBE_SWITCH, OnCbnSelchangeComboStrobeSwitch)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER_VALUE, OnKillfocusEditShutterValue)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN_VALUE, OnKillfocusEditGainValue)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_SELECTOR, OnCbnSelchangeComboLineSelector)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_MODE, OnCbnSelchangeComboLineMode)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_INVERTER, OnCbnSelchangeComboLineInverter)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_SOURCE, OnCbnSelchangeComboLineSource)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxIOSampleDlg message handlers

BOOL CGxIOSampleDlg::OnInitDialog()
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

	// ����ʼ���豸��ʧ����ֱ���˳�Ӧ�ó���
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ���½���ؼ�
	UpDateUI();

	// ��ȡͼ����ʾ��ص�UI��Դ
	m_pWnd       = GetDlgItem(IDC_STATIC_SHOW_WND);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxIOSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxIOSampleDlg::OnPaint() 
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
HCURSOR CGxIOSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum  = 0;
	uint32_t	  nDSNum   = 0;
    GX_OPEN_PARAM stOpenParam;
    
	// ��ʼ�����豸ʹ�ò���,Ĭ�ϴ����Ϊ1���豸
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

	// ����豸�������ȹر�, ��֤�ڳ�ʼ������ʱ�����´�
	if(m_hDevice != NULL)
	{
		GXCloseDevice(m_hDevice);
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

	//��ʼ��ͼ����ʾģ��
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
\brief  �л�����ѡ��ʱ����������������ŷ��򣬵�ƽ��ת�����Դ���Ӧ��Combo��

\return ��
*/
//---------------------------------------------------------------------------------
void CGxIOSampleDlg::UpdateLineRelativeInfo()
{
	// �л�����ѡ��ʱ����������������ŷ����Combo��
	InitLineMode();

	// �л�����ѡ��ʱ������������ĵ�ƽ��ת��Combo��
	InitLineInverter();

	// �л�����ѡ��ʱ����������������Դ��Combo��
	InitLineSource();
}

//---------------------------------------------------------------------------------
/**
\brief  �л��û��Զ������ѡ��ʱ�����¹������û��Զ������ֵ���Ӧ��Combo��

\return ��
*/
//---------------------------------------------------------------------------------
void CGxIOSampleDlg::UpdateUserOutputRelativeInfo()
{
	// �л��û��Զ������ѡ��ʱ�����¹������û��������ֵ��Ӧ��Combo��
	InitOutputValue();
}

//----------------------------------------------------------------------------------
/**
\brief    ˢ��UI

\return   ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BTN_SEND_SOFT_TRIGGER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_EDIT_GAIN_VALUE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER_VALUE)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION)->EnableWindow(m_bDevOpened  && m_bTriggerActive);
	GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(m_bDevOpened && m_bTriggerMode);
	GetDlgItem(IDC_COMBO_TRIGGER_SOURCE)->EnableWindow(m_bDevOpened  && m_bTriggerSource);
	GetDlgItem(IDC_COMBO_OUTPUT_MODE)->EnableWindow(m_bDevOpened  && m_bOutputMode);
	GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR)->EnableWindow(m_bDevOpened  && m_bOutputSelector);
	GetDlgItem(IDC_COMBO_OUTPUT_VALUE)->EnableWindow(m_bDevOpened  && m_bOutputValue);
	GetDlgItem(IDC_COMBO_STROBE_SWITCH)->EnableWindow(m_bDevOpened  && m_bStrobeSwitch && m_bStrobeSwitchIsUse);

	GetDlgItem(IDC_COMBO_LINE_SELECTOR)->EnableWindow(m_bDevOpened && m_bLineSelector);
	GetDlgItem(IDC_COMBO_LINE_MODE)->EnableWindow(m_bDevOpened && m_bLineMode);
	GetDlgItem(IDC_COMBO_LINE_INVERTER)->EnableWindow(m_bDevOpened && m_bLineInverter);
	GetDlgItem(IDC_COMBO_LINE_SOURCE)->EnableWindow(m_bDevOpened && m_bLineSource);
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in]������

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize      = 0;
	GX_STATUS emStatus   = GX_STATUS_ERROR;
	
	// ��ȡ������ĳ��ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
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
\brief  �л�"����ѡ��"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineSelector() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// ��ȡ����ѡ��ComboBox���ָ��
	CComboBox *pLineSelectorBox = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR);

	// ��ȡ��ǰѡ��
    nIndex   = pLineSelectorBox->GetCurSel();                  
	
	// ��ȡ��ǰѡ���Ӧ��ö����ֵ
    nEnumVal = (int64_t)pLineSelectorBox->GetItemData(nIndex);

	// ����ǰѡ�������ѡ��ö��ֵ���õ��豸��
	emStatus = GXSetEnumValue(m_hDevice, "LineSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// ��������ѡ�����������
	UpdateLineRelativeInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"���ŷ���"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// ��ȡ���ŷ���ComboBox���ָ��
	CComboBox *pLineModeBox = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE);

	// ��ȡ��ǰѡ��
	nIndex   = pLineModeBox->GetCurSel();

	// ��ȡ��ǰѡ���Ӧ��ö����ֵ
	nEnumVal = (int64_t)pLineModeBox->GetItemData(nIndex);

	// ����ǰѡ������ŷ���ö��ֵ���õ��豸��
	emStatus = GXSetEnumValue(m_hDevice, "LineMode", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"���ŵ�ƽ��ת"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineInverter() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	bool      bLineInverterValue = TRUE;

	// ��ȡ���ŵ�ƽ��תComboBox���ָ��
	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);

	// ��ȡ��ǰѡ��
	nIndex   = pLineInverter->GetCurSel();

	// �жϲ����õ�ǰѡ���Ӧ�Ĳ���ֵ
	if (nIndex == LINE_INVERTER_FALSE)
	{
		bLineInverterValue = FALSE;
	}

	if (nIndex == LINE_INVERTER_TRUE)
	{
		bLineInverterValue = TRUE;
	}

	// ����ǰѡ������ŵ�ƽ��תֵ���õ������
	emStatus = GXSetBoolValue(m_hDevice, "LineInverter", bLineInverterValue);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"�������Դ"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineSource() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// ��ȡ�������ԴComboBox���ָ��
	CComboBox *pLineSource = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE);

	// ��ȡ��ǰѡ��
	nIndex   = pLineSource->GetCurSel();

	// ��ȡ��ǰѡ���Ӧ��ö��ֵ
	nEnumVal = (int64_t)pLineSource->GetItemData(nIndex);

	// ����ǰѡ����������Դö��ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "LineSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"�û��Զ������ѡ��"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputSource() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ����ǰѡ����û��Զ������ѡ���ö��ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "UserOutputSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// �����û��Զ������ѡ�����������
	UpdateUserOutputRelativeInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"�û�IO���ģʽ"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ����ǰѡ����û�IO���ģʽ��ö��ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "UserOutputMode", nEnumVal);
	GX_VERIFY(emStatus);
	
	if (nEnumVal == GX_USER_OUTPUT_MODE_STROBE)
	{
		m_bStrobeSwitchIsUse = TRUE;
	}
	if (nEnumVal == GX_USER_OUTPUT_MODE_USERDEFINED)
	{
		m_bStrobeSwitchIsUse = FALSE;
	}

	// ���½���
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"�û��Զ������ֵ"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputValue() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	bool     bValue    = false;
    if (nIndex == OUTPUT_ACTIVATION_FALSE)
    {
		bValue = false;
    }
	else if(nIndex == OUTPUT_ACTIVATION_TRUE)
	{
		bValue = true;
	}

	// ����ǰѡ����û��Զ������ֵ�����õ������
	emStatus = GXSetBoolValue(m_hDevice, "UserOutputValue",bValue);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �ر�Ӧ�ó���

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	// ��δͣ������ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		// ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		// �ͷ�Ϊ��ʾͼ��׼������Դ
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;
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

// ---------------------------------------------------------------------------------
/**
\brief   ��ʼ�����:����Ĭ�ϲ���

\return  GX_STATUS_SUCCESS:��ʼ���豸�ɹ�������:��ʼ���豸ʧ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxIOSampleDlg::InitDevice()
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
\brief ��ʼ������ģʽCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
    
	// �ж��豸�Ƿ�֧�ִ���ģʽ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
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

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ������ԴCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerSourceUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�ִ���Դ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
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

//----------------------------------------------------------------------------------
/**
\brief ��ʼ����������Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerActivationUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�ִ�������
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
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
    InitEnumUI("TriggerActivation", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION), m_bTriggerActive);
}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ������ƿ���Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitStrobeSwitchUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧������ƿ���
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "StrobeSwitch", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bStrobeSwitch = true;
	}
	
	if (!m_bStrobeSwitch)
	{
		return;
	}

	// ��ʼ������ƿ���Combox��
	InitEnumUI("StrobeSwitch", (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH),m_bStrobeSwitch);
}

//----------------------------------------------------------------------------------
/**
\brief ��ʼ���û�IO���ģʽCombox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧���û�IO���ģʽ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "UserOutputMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bOutputMode = true;
	}
	
	if (!m_bOutputMode)
	{
		return;
	}

	// ��ʼ���û�IO���ģʽCombox��
    InitEnumUI("UserOutputMode", (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE), m_bOutputMode);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ���û��Զ������ѡ��Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputSelectorUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧���û�IO���ѡ��
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "UserOutputSelector", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bOutputSelector = true;
	}
	
	if (!m_bOutputSelector)
	{
		return;
	}

	// ��ʼ���û�IO���ѡ��Combox��
	InitEnumUI("UserOutputSelector",(CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR),m_bOutputSelector);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"����ѡ��"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineSelector()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧������ѡ��
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "LineSelector", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bLineSelector = true;
	}
	
	if (!m_bLineSelector)
	{
		return;
	}

	// ��ʼ������ѡ��
	InitEnumUI("LineSelector", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR), m_bLineSelector);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"���ŷ���"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineMode()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧�����ŷ���
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "LineMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bLineMode = true;
	}
	
	if (!m_bLineMode)
	{
		return;
	}

	// ��ʼ�����ŷ���
	InitEnumUI("LineMode", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE), m_bLineMode);

}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"���ŵ�ƽ��ת"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineInverter()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bTemp    = true;

	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);

	// �ж��豸�Ƿ�֧�����ŵ�ƽ��ת
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "LineInverter", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bLineInverter = true;
	}
	
	if (!m_bLineInverter)
	{
		return;
	}

	// ��ʼ�����ŵ�ƽ��ת
	pLineInverter->ResetContent();
	pLineInverter->SetItemData(pLineInverter->AddString("false"), LINE_INVERTER_FALSE);
	pLineInverter->SetItemData(pLineInverter->AddString("true"), LINE_INVERTER_TRUE);

	// ��ȡ��ǰ�豸�����ŵ�ƽ��ת��ֵ����ѡ����µ�������
	emStatus = GXGetBoolValue(m_hDevice, "LineInverter", &bTemp);
	GX_VERIFY(emStatus);
	if (bTemp)
	{
		pLineInverter->SetCurSel(LINE_INVERTER_TRUE);
	}
	else
	{
		pLineInverter->SetCurSel(LINE_INVERTER_FALSE);
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"�������Դ"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineSource()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧���������Դ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "LineSource", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bLineSource = true;
	}
	
	if (!m_bLineSource)
	{
		return;
	}

	// ��ʼ���������Դ
	InitEnumUI("LineSource", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE), m_bLineSource);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;
    
	// ��ʼ���û��Զ������ֵѡ��
    InitOutputValue();

	// ����ģʽ����ѡ��Combox�ؼ�
    InitTriggerModeUI();
	
	// ��ʼ������ԴCombox�ؼ�
	InitTriggerSourceUI();

	// ��ʼ����������Combox�ؼ�
	InitTriggerActivationUI();

	// ��ʼ������ƿ���Combox�ؼ�
	InitStrobeSwitchUI();

	// ��ʼ���û�IO���ģʽCombox�ؼ�
	InitOutputModeUI();

	// ��ʼ���û��Զ������ѡ��ComBox�ؼ�
	InitOutputSelectorUI();
	
	// ��ʼ���ع�ʱ����ؿؼ�
	InitShutterUI();

	// ��ʼ��������ؿؼ�
	InitGainUI();

	// ��ʼ������ѡ��ComBox�ؼ�
	InitLineSelector();

	// ��ʼ������ģʽComBox�ؼ�
	InitLineMode();

	// ��ʼ�����ŵ�ƽ��תComBox�ؼ�
	InitLineInverter();

	// ��ʼ���������ԴComBox�ؼ�
	InitLineSource();

	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ��"�û��Զ������ֵ"Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputValue()
{
	CComboBox *pBox           = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);
	GX_STATUS  emStatus       = GX_STATUS_SUCCESS;
	bool       bVal           = false;

	// ��ʼ���������������б�
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "UserOutputValue", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bOutputValue = true;
	}
	
	if (!m_bOutputValue)
	{
		return;
	}

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pBox->ResetContent();
	pBox->SetItemData(pBox->AddString("false"), OUTPUT_ACTIVATION_FALSE);
	pBox->SetItemData(pBox->AddString("true"), OUTPUT_ACTIVATION_TRUE);

	// ��ȡö���͵ĵ�ǰֵ,����Ϊ���浱ǰ��ʾֵ
	emStatus = GXGetBoolValue(m_hDevice, "UserOutputValue", &bVal);
	GX_VERIFY(emStatus);
	if (bVal)
	{
		pBox->SetCurSel(OUTPUT_ACTIVATION_TRUE);
	}
	else
	{
		pBox->SetCurSel(OUTPUT_ACTIVATION_FALSE);
	}
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   chFeature    [in]    ������ID
\param   pComboBox      [in]    �ؼ�ָ��
\param   bIsImplemented [in]    ��ʶ�Ƿ�֧��chFeature����Ĺ���

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplemented)
{
	// �жϿؼ�
	if ((pComboBox == NULL))
	{
		return;
	}

	GX_STATUS emStatus       = GX_STATUS_ERROR;
	uint32_t  nEntryNum      = 0;
	int       nCurcel        = 0;
	GX_ENUM_VALUE stEnumValue;

	if (!bIsImplemented)
	{
		return;
	}

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
			nCurcel = i;
		}
	}

	// ���õ�ǰֵΪ�������ʾֵ
	pComboBox->SetCurSel(nCurcel);
}

//---------------------------------------------------------------------------------
/**
\brief   �л�"����Դ"ѡ����Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ����ǰѡ��Ĵ���Դ��ö��ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "TriggerSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"����ģʽ"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerMode()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool bIsWritable   = TRUE;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE);
	GX_ENUM_VALUE stEnumValue;

	int      nIndex    = pBox->GetCurSel();                     // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// �жϴ���ģʽ�Ƿ��д
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsWritable = true;
	}

	// ��ȡ����ģʽ�ĵ�ǰֵ
	emStatus = GXGetEnumValue(m_hDevice, "TriggerMode", &stEnumValue);
	GX_VERIFY(emStatus);

	if (bIsWritable)
	{
		// ����ǰѡ��Ĵ���ģʽ��ֵ���õ������
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
			for (int i = 0;i < pBox->GetCount();i++)
			{
				if (stEnumValue.stCurValue.nCurValue == pBox->GetItemData(i))
				{
					pBox->SetCurSel(i);
					break;
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

//----------------------------------------------------------------------------------
/**
\brief  ���"������ͼ"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnSendSoftTrigger()
{
	// TODO: Add your control notification handler code here
	//������������(�ڴ���ģʽΪOn�ҿ�ʼ�ɼ�����Ч)
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXSetCommandValue(m_hDevice, "TriggerSoftware");

	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"��������"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerActivation()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ����ǰѡ��Ĵ������Ե�ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "TriggerActivation", nEnumVal);
	GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع���ؿؼ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
    CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER_VALUE);

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

    GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double      dShutterVal = 0.0;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

    strTemp.Format("�ع�(%.4f~%.4f)%s",m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// ��ȡ��ǰֵ������ǰֵ���µ�����
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ�
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitGainUI()
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STATIC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_GAIN_VALUE);

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strRange = "";
	double    dGainVal = 0;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// ��ʾ��Χ
    strRange.Format("����(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);
	
	// ��ȡ��ǰֵ�����µ�����
	m_dGainValue = m_stGainFloatRange.dCurValue;
   
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

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

//---------------------------------------------------------------------------------
/**
\brief   �ɼ�ͼ��ص�����
\param   pFrame      �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxIOSampleDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxIOSampleDlg *pDlg = (CGxIOSampleDlg*)(pFrame->pUserParam);
	if (pFrame->status == 0) 
	{
		pDlg->m_pBitmap->Show(pFrame);
	}

	
}

//----------------------------------------------------------------------------------
/**
\brief  �л�"����ƿ���"Combox����Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboStrobeSwitch()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH);

	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ����ǰѡ�������ƿ��ص�ֵ���õ������
	emStatus = GXSetEnumValue(m_hDevice, "StrobeSwitch", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnStopSnap()
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
\brief  ���"�ر��豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnCloseDevice()
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

//------------------------------------------------------
/**
\brief   ��Ӧ�ع�Edit�ؼ�ʧȥ����ĺ���

\return  ��
*/
//------------------------------------------------------
void CGxIOSampleDlg::OnKillfocusEditShutterValue() 
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

	// �����ع��ֵ
	status = GXSetFloatValue(m_hDevice,"ExposureTime",m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   ��Ӧ����Edit�ؼ�ʧȥ����ĺ���

  \return  ��
*/
//------------------------------------------------------
void CGxIOSampleDlg::OnKillfocusEditGainValue() 
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

	// ���������ֵ
	status = GXSetFloatValue(m_hDevice,"Gain",m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   �ַ�����(��Ҫ������̵Ļس���Ϣ)
\param   pMsg  ��Ϣ�ṹ��

\return  �ɹ���TRUE   ʧ�ܣ�FALSE
*/
//------------------------------------------------------
BOOL CGxIOSampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd  = NULL;
	int nCtrlID = 0;               //�����ȡ�Ŀؼ�ID

	//�ж��Ƿ��Ǽ��̻س���Ϣ
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		//��ȡ�������λ�õĿؼ�ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//�ж�ID����
		switch (nCtrlID)
		{
		case IDC_EDIT_SHUTTER_VALUE:
		case IDC_EDIT_GAIN_VALUE:

			//ʧȥ����
			SetFocus();

			break;

		default:
			break;
		}

		return TRUE;
	}

	if ((WM_KEYDOWN == pMsg->message) && (VK_ESCAPE == pMsg->wParam))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
