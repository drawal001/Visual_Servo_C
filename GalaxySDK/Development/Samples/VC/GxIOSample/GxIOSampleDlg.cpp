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

	// 初始化参数
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

	// 若初始化设备库失败则直接退出应用程序
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 更新界面控件
	UpDateUI();

	// 获取图像显示相关的UI资源
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
\brief  点击"打开设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum  = 0;
	uint32_t	  nDSNum   = 0;
    GX_OPEN_PARAM stOpenParam;
    
	// 初始化打开设备使用参数,默认打开序号为1的设备
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// 枚举设备
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDevNum <= 0)
	{
		MessageBox("未发现设备!");
		return;
	}

	// 如果设备被打开则先关闭, 保证在初始化错误时能重新打开
	if(m_hDevice != NULL)
	{
		GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// 打开设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = TRUE;

	// 获取流通道数
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// 获取流句柄
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}


	// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
	// 以提高网络相机的采集性能,设置方法参考以下代码。
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// 判断设备是否支持流通道数据包功能
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
			// 获取当前网络环境的最优包长值
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			GX_VERIFY(emStatus);

			// 将最优包长值设置为当前设备的流通道包长值
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	// 设置相机的默认参数:采集模式:连续采集
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	//初始化图像显示模块
	m_pBitmap = new CGXBitmap(m_hDevice, m_pWnd);
	if (NULL == m_pBitmap)
	{
		ShowErrorString(GX_STATUS_ERROR);
		return;
	}

	// 获取相机参数,初始化界面控件
	InitUI();

	// 更新界面控件
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief  切换引脚选择时，更新其关联的引脚方向，电平反转，输出源相对应的Combo框

\return 无
*/
//---------------------------------------------------------------------------------
void CGxIOSampleDlg::UpdateLineRelativeInfo()
{
	// 切换引脚选择时，更新其关联的引脚方向的Combo框
	InitLineMode();

	// 切换引脚选择时，更新其关联的电平反转的Combo框
	InitLineInverter();

	// 切换引脚选择时，更新其关联的输出源的Combo框
	InitLineSource();
}

//---------------------------------------------------------------------------------
/**
\brief  切换用户自定义输出选择时，更新关联的用户自定义输出值相对应的Combo框

\return 无
*/
//---------------------------------------------------------------------------------
void CGxIOSampleDlg::UpdateUserOutputRelativeInfo()
{
	// 切换用户自定义输出选择时，更新关联的用户定义输出值对应的Combo框
	InitOutputValue();
}

//----------------------------------------------------------------------------------
/**
\brief    刷新UI

\return   无
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
\brief  弹出错误信息
\param  emErrorStatus  [in]错误码

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize      = 0;
	GX_STATUS emStatus   = GX_STATUS_ERROR;
	
	// 获取错误码的长度，并分配内存空间
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// 释放申请的内存空间
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  切换"引脚选择"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineSelector() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// 获取引脚选择ComboBox框的指针
	CComboBox *pLineSelectorBox = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR);

	// 获取当前选项
    nIndex   = pLineSelectorBox->GetCurSel();                  
	
	// 获取当前选项对应的枚举型值
    nEnumVal = (int64_t)pLineSelectorBox->GetItemData(nIndex);

	// 将当前选择的引脚选择枚举值设置到设备中
	emStatus = GXSetEnumValue(m_hDevice, "LineSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// 更新引脚选择相关联的项
	UpdateLineRelativeInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  切换"引脚方向"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// 获取引脚方向ComboBox框的指针
	CComboBox *pLineModeBox = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE);

	// 获取当前选项
	nIndex   = pLineModeBox->GetCurSel();

	// 获取当前选项对应的枚举型值
	nEnumVal = (int64_t)pLineModeBox->GetItemData(nIndex);

	// 将当前选择的引脚方向枚举值设置到设备中
	emStatus = GXSetEnumValue(m_hDevice, "LineMode", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  切换"引脚电平反转"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineInverter() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	bool      bLineInverterValue = TRUE;

	// 获取引脚电平反转ComboBox框的指针
	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);

	// 获取当前选项
	nIndex   = pLineInverter->GetCurSel();

	// 判断并设置当前选项对应的布尔值
	if (nIndex == LINE_INVERTER_FALSE)
	{
		bLineInverterValue = FALSE;
	}

	if (nIndex == LINE_INVERTER_TRUE)
	{
		bLineInverterValue = TRUE;
	}

	// 将当前选择的引脚电平反转值设置到相机中
	emStatus = GXSetBoolValue(m_hDevice, "LineInverter", bLineInverterValue);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  切换"引脚输出源"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboLineSource() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int       nIndex   = 0;
	int64_t   nEnumVal = 0;

	// 获取引脚输出源ComboBox框的指针
	CComboBox *pLineSource = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE);

	// 获取当前选项
	nIndex   = pLineSource->GetCurSel();

	// 获取当前选项对应的枚举值
	nEnumVal = (int64_t)pLineSource->GetItemData(nIndex);

	// 将当前选择的引脚输出源枚举值设置到相机中
	emStatus = GXSetEnumValue(m_hDevice, "LineSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  切换"用户自定义输出选择"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputSource() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 将当前选择的用户自定义输出选择的枚举值设置到相机中
	emStatus = GXSetEnumValue(m_hDevice, "UserOutputSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// 更新用户自定义输出选择相关联的项
	UpdateUserOutputRelativeInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  切换"用户IO输出模式"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 将当前选择的用户IO输出模式的枚举值设置到相机中
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

	// 更新界面
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  切换"用户自定义输出值"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboOutputValue() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	bool     bValue    = false;
    if (nIndex == OUTPUT_ACTIVATION_FALSE)
    {
		bValue = false;
    }
	else if(nIndex == OUTPUT_ACTIVATION_TRUE)
	{
		bValue = true;
	}

	// 将当前选择的用户自定义输出值的设置到相机中
	emStatus = GXSetBoolValue(m_hDevice, "UserOutputValue",bValue);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  关闭应用程序

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	// 若未停采则先停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		// 注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		// 释放为显示图像准备的资源
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;
	}

	// 未关闭设备则关闭设备
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

	// 关闭设备库
	emStatus = GXCloseLib();	

	CDialog::OnClose();
}

// ---------------------------------------------------------------------------------
/**
\brief   初始化相机:设置默认参数

\return  GX_STATUS_SUCCESS:初始化设备成功，其它:初始化设备失败
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxIOSampleDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief 初始化触发模式Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
    
	// 判断设备是否支持触发模式
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

	// 初始化触发模式Combox框
	InitEnumUI("TriggerMode", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE), m_bTriggerMode);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化触发源Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerSourceUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持触发源
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

	// 初始化触发源Combox框
    InitEnumUI("TriggerSource", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),m_bTriggerSource);
}

//----------------------------------------------------------------------------------
/**
\brief 初始化触发极性Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitTriggerActivationUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持触发极性
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

	// 初始化触发极性Combox框
    InitEnumUI("TriggerActivation", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION), m_bTriggerActive);
}

//----------------------------------------------------------------------------------
/**
\brief 初始化闪光灯开关Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitStrobeSwitchUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持闪光灯开关
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

	// 初始化闪光灯开关Combox框
	InitEnumUI("StrobeSwitch", (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH),m_bStrobeSwitch);
}

//----------------------------------------------------------------------------------
/**
\brief 初始化用户IO输出模式Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持用户IO输出模式
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

	// 初始化用户IO输出模式Combox框
    InitEnumUI("UserOutputMode", (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_MODE), m_bOutputMode);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化用户自定义输出选择Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputSelectorUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持用户IO输出选择
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

	// 初始化用户IO输出选择Combox框
	InitEnumUI("UserOutputSelector",(CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_SELECTOR),m_bOutputSelector);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚选择"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineSelector()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持引脚选择
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

	// 初始化引脚选择
	InitEnumUI("LineSelector", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SELECTOR), m_bLineSelector);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚方向"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineMode()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持引脚方向
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

	// 初始化引脚方向
	InitEnumUI("LineMode", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_MODE), m_bLineMode);

}

//----------------------------------------------------------------------------------
/**
\brief  初始化"引脚电平反转"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineInverter()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bTemp    = true;

	CComboBox *pLineInverter = (CComboBox *)GetDlgItem(IDC_COMBO_LINE_INVERTER);

	// 判断设备是否支持引脚电平反转
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

	// 初始化引脚电平反转
	pLineInverter->ResetContent();
	pLineInverter->SetItemData(pLineInverter->AddString("false"), LINE_INVERTER_FALSE);
	pLineInverter->SetItemData(pLineInverter->AddString("true"), LINE_INVERTER_TRUE);

	// 获取当前设备的引脚电平反转的值，并选择更新到界面上
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
\brief  初始化"引脚输出源"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitLineSource()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持引脚输出源
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

	// 初始化引脚输出源
	InitEnumUI("LineSource", (CComboBox *)GetDlgItem(IDC_COMBO_LINE_SOURCE), m_bLineSource);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;
    
	// 初始化用户自定义输出值选项
    InitOutputValue();

	// 触发模式下拉选项Combox控件
    InitTriggerModeUI();
	
	// 初始化触发源Combox控件
	InitTriggerSourceUI();

	// 初始化触发极性Combox控件
	InitTriggerActivationUI();

	// 初始化闪光灯开关Combox控件
	InitStrobeSwitchUI();

	// 初始化用户IO输出模式Combox控件
	InitOutputModeUI();

	// 初始化用户自定义输出选择ComBox控件
	InitOutputSelectorUI();
	
	// 初始化曝光时间相关控件
	InitShutterUI();

	// 初始化增益相关控件
	InitGainUI();

	// 初始化引脚选择ComBox控件
	InitLineSelector();

	// 初始化引脚模式ComBox控件
	InitLineMode();

	// 初始化引脚电平反转ComBox控件
	InitLineInverter();

	// 初始化引脚输出源ComBox控件
	InitLineSource();

	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化"用户自定义输出值"Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitOutputValue()
{
	CComboBox *pBox           = (CComboBox *)GetDlgItem(IDC_COMBO_OUTPUT_VALUE);
	GX_STATUS  emStatus       = GX_STATUS_SUCCESS;
	bool       bVal           = false;

	// 初始化触发极性下拉列表
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

	// 初始化当前控件的可选项
	pBox->ResetContent();
	pBox->SetItemData(pBox->AddString("false"), OUTPUT_ACTIVATION_FALSE);
	pBox->SetItemData(pBox->AddString("true"), OUTPUT_ACTIVATION_TRUE);

	// 获取枚举型的当前值,并设为界面当前显示值
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
\brief   初始化组合框UI界面
\param   chFeature    [in]    功能码ID
\param   pComboBox      [in]    控件指针
\param   bIsImplemented [in]    标识是否支持chFeature代表的功能

\return  无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplemented)
{
	// 判断控件
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

	// 获取功能的枚举数
	emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
	GX_VERIFY(emStatus);

	nEntryNum = stEnumValue.nSupportedNum;

	// 初始化当前控件的可选项
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{	
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCurcel = i;
		}
	}

	// 设置当前值为界面的显示值
	pComboBox->SetCurSel(nCurcel);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"触发源"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 将当前选择的触发源的枚举值设置到相机中
	emStatus = GXSetEnumValue(m_hDevice, "TriggerSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  切换"触发模式"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerMode()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool bIsWritable   = TRUE;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE);
	GX_ENUM_VALUE stEnumValue;

	int      nIndex    = pBox->GetCurSel();                     // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // 获取当前选项对应的枚举型值

	// 判断触发模式是否可写
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsWritable = true;
	}

	// 获取触发模式的当前值
	emStatus = GXGetEnumValue(m_hDevice, "TriggerMode", &stEnumValue);
	GX_VERIFY(emStatus);

	if (bIsWritable)
	{
		// 将当前选择的触发模式的值设置到相机中
		emStatus = GXSetEnumValue(m_hDevice, "TriggerMode", nEnumVal);
		GX_VERIFY(emStatus);

		// 更新当前触发模式
		m_nTriggerMode = nEnumVal;
	}
	else
	{
		MessageBox("当前状态不可写，请停止采集后，请重新设置!");
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
\brief  点击"软触发采图"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnSendSoftTrigger()
{
	// TODO: Add your control notification handler code here
	//发送软触发命令(在触发模式为On且开始采集后有效)
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXSetCommandValue(m_hDevice, "TriggerSoftware");

	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  切换"触发极性"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboTriggerActivation()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVATION);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 将当前选择的触发极性的值设置到相机中
	emStatus = GXSetEnumValue(m_hDevice, "TriggerActivation", nEnumVal);
	GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
    CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER_VALUE);

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

    GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double      dShutterVal = 0.0;

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

    strTemp.Format("曝光(%.4f~%.4f)%s",m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// 获取当前值并将当前值更新到界面
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件
 
\return  无
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

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// 显示范围
    strRange.Format("增益(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);
	
	// 获取当前值并更新到界面
	m_dGainValue = m_stGainFloatRange.dCurValue;
   
	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"按钮消息响应函数

\return 无
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
	//注册回调
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	//发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = TRUE;

	// 更新界面UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   采集图像回调函数
\param   pFrame      回调参数

\return  无
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
\brief  切换"闪光灯开关"Combox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnCbnSelchangeComboStrobeSwitch()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_STROBE_SWITCH);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 将当前选择的闪光灯开关的值设置到相机中
	emStatus = GXSetEnumValue(m_hDevice, "StrobeSwitch", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	//注销回调
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);
	m_bIsSnap = FALSE;

	// 更新界面UI
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxIOSampleDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 未停采时则停止采集
	if (m_bIsSnap)
	{
		//发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		//注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;
	}

	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	m_bDevOpened = FALSE;
	m_hDevice    = NULL;

	// 更新界面UI
	UpDateUI();
}

//------------------------------------------------------
/**
\brief   响应曝光Edit控件失去焦点的函数

\return  无
*/
//------------------------------------------------------
void CGxIOSampleDlg::OnKillfocusEditShutterValue() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//判断输入值是否在曝光时间的范围内
	//若大于最大值则将曝光值设为最大值
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//若小于最小值将曝光值设为最小值
	if (m_dShutterValue < m_stShutterFloatRange.dMin)
	{
		m_dShutterValue = m_stShutterFloatRange.dMin;
	}

	// 设置曝光的值
	status = GXSetFloatValue(m_hDevice,"ExposureTime",m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   响应增益Edit控件失去焦点的函数

  \return  无
*/
//------------------------------------------------------
void CGxIOSampleDlg::OnKillfocusEditGainValue() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//判断输入值是否在增益值的范围内
	//若输入的值大于最大值则将增益值设置成最大值
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

	//若输入的值小于最小值则将增益的值设置成最小值
	if (m_dGainValue < m_stGainFloatRange.dMin)
	{
		m_dGainValue = m_stGainFloatRange.dMin;
	}

	// 设置增益的值
	status = GXSetFloatValue(m_hDevice,"Gain",m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//------------------------------------------------------
/**
\brief   分发函数(主要处理键盘的回车消息)
\param   pMsg  消息结构体

\return  成功：TRUE   失败：FALSE
*/
//------------------------------------------------------
BOOL CGxIOSampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd  = NULL;
	int nCtrlID = 0;               //保存获取的控件ID

	//判断是否是键盘回车消息
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获取光标所处位置的控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//判读ID类型
		switch (nCtrlID)
		{
		case IDC_EDIT_SHUTTER_VALUE:
		case IDC_EDIT_GAIN_VALUE:

			//失去焦点
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
