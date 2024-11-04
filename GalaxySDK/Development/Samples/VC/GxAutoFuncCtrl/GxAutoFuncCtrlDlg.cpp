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

	//初始化参数
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

	//初始化设备库
	emStatus = GXInitLib();  
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}
	
	//获取图像显示窗口的指针和绘图DC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);

	// 更新界面控件
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
\brief   点击"打开设备"按钮消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
  	GX_STATUS     emStatus    = GX_STATUS_SUCCESS;
	uint32_t      nDevNum     = 0;
	GX_OPEN_PARAM stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// 初始化设备打开参数
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

	// 如果设备已经打开则关闭,保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
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

		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

		// 判断设备是否支持流通道数据包功能
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
		GX_VERIFY(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
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
	
	// 设置相机的默认参数:采集模式:连续采集,数据格式:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// 初始化图像显示模块
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
\brief   点击"开始采集"按钮消息响应函数

\return  无
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

	SetTimer(0,1000,NULL);

	// 更新界面UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数:进行图像获取和显示操作
\param   pFrame   回调参数

\return  无
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
\brief   点击"停止采集"按钮消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnStopSnap() 
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
    
	m_pBitmap->UnPrepareForShowImg();

	// 停止Timer
	KillTimer(0);

	// 更新界面UI
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"关闭设备"按钮消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnBnClickedBtnCloseDevice() 
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
		m_bIsSnap = FALSE;

		// 停止Timer
		KillTimer(0);
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

//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   chFeature    [in]    功能码ID
\param   pComboBox      [in]    控件指针
\param   bIsImplemented [in]    标识设备是否支持chFeature代表的功能

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitEnumUI(char* strFeatureName, CComboBox *pComboBox, bool bIsImplemented)
{
	// 判断控件
	if ((pComboBox == NULL) || (!bIsImplemented))
	{
		return;
	}
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	uint32_t  nEntryNum      = 0;
	int       nCurcel        = 0;

	// 获取功能的枚举数
	GX_ENUM_VALUE stEnumValue;
	emStatus = GXGetEnumValue(m_hDevice,  strFeatureName, &stEnumValue);
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
\brief   相机初始化

\return  无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxAutoFuncCtrlDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice,"AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   UI界面初始化

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// 初始化自动增益Combox控件
    InitAutoGainUI();

	// 初始化自动曝光Combox控件
	InitAutoShutterUI();

	// 初始化2A光照环境Combox控件
	InitLightEnvironmentUI();

	// 初始化期望灰度值相关控件的
    InitGrayUI();

	// 初始化感兴趣区域的初始值
	InitROIUI();

	// 初始化曝光时间相关控件
	InitShutterUI();

	// 初始化增益值相关控件
	InitGainUI();
}

//---------------------------------------------------------------------------------
/**
\brief   初始化自动增益Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitAutoGainUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;

	// 判断设备是否支持自动增益
	emStatus = GXGetNodeAccessMode(m_hDevice, "GainAuto", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementAutoGain = true;
	}

	//获取自动增益模式
	emStatus = GXGetEnumValue(m_hDevice,"GainAuto",&stEnumValue);
	GX_VERIFY(emStatus);

	m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)stEnumValue.nArrySupportedValue->nCurValue;

	// 初始化用户自动增益Combox框
	InitEnumUI("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN), m_bImplementAutoGain);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化自动曝光Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitAutoShutterUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;

	// 判断设备是否支持自动曝光模式	
	emStatus = GXGetNodeAccessMode(m_hDevice, "ExposureAuto", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementAutoShutter = true;
	}
	
	//获取自动曝光模式
	emStatus = GXGetEnumValue(m_hDevice,"ExposureAuto",&stEnumValue);
	GX_VERIFY(emStatus);

	m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)stEnumValue.nArrySupportedValue->nCurValue;

	// 初始化用户IO输出模式Combox框
	InitEnumUI("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER), m_bImplementAutoShutter);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化2A光照环境Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitLightEnvironmentUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
    GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

	// 判断设备是否支持2A光照环境	
	emStatus = GXGetNodeAccessMode(m_hDevice, "AALightEnvironment", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementLight = true;
	}
	

	// 初始化2A光照环境
	InitEnumUI("AALightEnvironment", (CComboBox *)GetDlgItem(IDC_COMBO_LIGHT), m_bImplementLight);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光相关控件:编辑框及静态框

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STATIC_SHUTTER);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_CUR_SHUTTER);

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	GX_FLOAT_VALUE stValue;

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);
    
	// UI显示处理
	strTemp.Format("曝光(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// 获取当前值并将当前值更新到界面
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	// 获取自动曝光最大值的当前值
	emStatus = GXGetFloatValue(m_hDevice, "AutoExposureTimeMax", &stValue);
	GX_VERIFY(emStatus);
	m_dAutoShutterMax = stValue.dCurValue;
	strTemp.Format("%.4f",m_dAutoShutterMax);
	SetDlgItemText(IDC_EDIT_AUTO_SHUTTER_MAX,strTemp);

	// 获取自动曝光最小值的当前值
	emStatus = GXGetFloatValue(m_hDevice, "AutoExposureTimeMin", &stValue);
	GX_VERIFY(emStatus);
	m_dAutoShutterMin = stValue.dCurValue;
	strTemp.Format("%.4f",m_dAutoShutterMin);
	SetDlgItemText(IDC_EDIT_AUTO_SHUTTER_MIN,strTemp);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件 
 
\return  无
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

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// UI显示处理
	strTemp.Format("增益(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strTemp);
	
	// 获取当前值并更新到界面
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &stGainVal);
	GX_VERIFY(emStatus);
	m_dGainValue= m_stGainFloatRange.dCurValue;

	// 获取自动增益最大值的当前值
    emStatus = GXGetFloatValue(m_hDevice, "AutoGainMax", &stGainVal);
	GX_VERIFY(emStatus);
	m_dAutoGainMax = stGainVal.dCurValue;
    strTemp.Format("%.4f",m_dAutoGainMax);
 	SetDlgItemText(IDC_EDIT_AUTO_GAIN_MAX,strTemp);

	// 获取自动增益最小值的当前值
	emStatus = GXGetFloatValue(m_hDevice, "AutoGainMin", &stGainVal);
	GX_VERIFY(emStatus);
	m_dAutoGainMin = (int)stGainVal.dCurValue;
	strTemp.Format("%.4f",m_dAutoGainMin);
	SetDlgItemText(IDC_EDIT_AUTO_GAIN_MIN,strTemp);
	
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化期望灰度值相关控件
 
\return  无
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

	// 获取感兴趣区域参数的范围并显示到界面
	UpdateROIRange();

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   获取ROI的范围显示到界面
 
\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::UpdateROIRange()
{
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE  nIntRange;
	CString       strTmep = _T("");

	emStatus = GXGetIntValue(m_hDevice, "AAROIWidth", &nIntRange);
	GX_VERIFY(emStatus)
    strTmep.Format("2A感兴趣区域宽度(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
    SetDlgItemText(IDC_STATIC_ROI_W,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIHeight", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A感兴趣区域高度(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_H,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetX", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A感兴趣区域X坐标(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_X,strTmep);

	emStatus = GXGetIntValue(m_hDevice, "AAROIOffsetY", &nIntRange);
	GX_VERIFY(emStatus)
	strTmep.Format("2A感兴趣区域Y坐标(%I64d~%I64d)",nIntRange.nMin,nIntRange.nMax);
	SetDlgItemText(IDC_STATIC_ROI_Y,strTmep);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化期望灰度值相关控件
 
\return  无
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

	// 获取期望灰度值范围
	emStatus = GXGetIntValue(m_hDevice, "ExpectedGrayValue", &stIntRange);
	GX_VERIFY(emStatus);
	pSliderCtrl->SetRange((int)stIntRange.nMin, (int)stIntRange.nMax, true);

	// 显示范围
    strRange.Format("期望灰度值(%I64d~%I64d)", stIntRange.nMin, stIntRange.nMax);
	pStcGray->SetWindowText(strRange);

	pSliderCtrl->SetPos((int)stIntRange.nCurValue);
	m_nGray = (int)stIntRange.nCurValue;
   
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"光照环境"Combox选项消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboLight() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_LIGHT);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值


	emStatus = GXSetEnumValue(m_hDevice, "AALightEnvironment", nEnumVal);	
	GX_VERIFY(emStatus);
	
	if (((GX_AA_LIGHT_ENVIRMENT_AC50HZ == nEnumVal) 
		|| (GX_AA_LIGHT_ENVIRMENT_AC60HZ == nEnumVal))
		&&((m_emAutoShutterMode != GX_EXPOSURE_AUTO_CONTINUOUS) 
		|| (m_emAutoGainMode != GX_GAIN_AUTO_CONTINUOUS)))
	{
		MessageBox("50hz交流电和60hz交流电情况下，\n必须同时开启自动曝光和自动增益，此时自动功能才能生效.","Msg");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   切换"自动增益"Combox选项消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboAutoGain() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
    
	emStatus = GXSetEnumValue(m_hDevice, "GainAuto", nEnumVal);	
	GX_VERIFY(emStatus);
    m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nEnumVal;

 	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   切换"自动曝光"Combox选项消息响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnCbnSelchangeComboAutoShutter() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
    
	emStatus = GXSetEnumValue(m_hDevice, "ExposureAuto", nEnumVal);	
	GX_VERIFY(emStatus);
	m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nEnumVal;
 
 	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   滑动滑动条消息响应函数
\param   nSBCode     指定卷轴滚动请求的代码
\param   nPos        滑动条位置
\param   pScrollBar  滑动条控件指针

\return  无
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

	// 获取当前滑动条位置
	nValue = pSlider->GetPos();

	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_GRAY:  // 调节期望灰度滑动条
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
\brief   消息分派函数(主要处理回车消息)
\param   pMsg  消息结构体

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
		//获取当前拥有输入焦点的窗口(控件)指针
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

			//失去焦点
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
\brief   Edit控件失去焦点函数:设置2A感兴趣区域X坐标

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiX() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIOffsetX", m_nRoiX);
	GX_VERIFY(emStatus);

	// 获取感兴趣区域参数的范围并显示到界面
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit控件失去焦点函数:设置2A感兴趣区域Y坐标

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRioY() 
{
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIOffsetY", m_nRoiY);
	GX_VERIFY(emStatus);

	// 获取感兴趣区域参数的范围并显示到界面
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit控件失去焦点函数:设置2A感兴趣区域宽

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiW() 
{
	// TODO: Add your control notification handler code here	
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIWidth", m_nRoiW);
	GX_VERIFY(emStatus);

	// 获取感兴趣区域参数的范围并显示到界面
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit控件失去焦点函数:设置2A感兴趣区域高

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditRoiH() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	emStatus = GXSetIntValue(m_hDevice, "AAROIHeight", m_nRoiH);
	GX_VERIFY(emStatus);

	// 获取感兴趣区域参数的范围并显示到界面
	UpdateROIRange();
}

//---------------------------------------------------------------------------------
/**
\brief   Edit控件失去焦点函数:设置自动增益最大值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoGainMax() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//若输入的值大于自动增益的最大值则将自动增益的最大值设置成增益的最大值
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
\brief   Edit控件失去焦点函数:设置自动增益最小值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoGainMin() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

    GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//若输入的值小于自动增益的最小值则将自动增益的最小值设置成增益的最小值
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
\brief   Edit控件失去焦点函数:设置自动曝光最大值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoShutterMax() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//若输入的值大于自动曝光的最大值则将自动曝光的最大值设置成曝光的最大值
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
\brief   Edit控件失去焦点函数:设置自动曝光最小值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditAutoShutterMin() 
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	UpdateData(TRUE);

	//若输入的值小于自动曝光的最小值则将自动曝光的最小值设置成曝光的最小值
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
\brief   刷新显示当前增益、曝光值

\return  无
*/
//---------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::RefreshCurValue()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CString   strTemp  = "";
	GX_FLOAT_VALUE    dValue;

	if (m_emAutoShutterMode != GX_EXPOSURE_AUTO_OFF)
	{
		//如果当前选择曝光为ONCE时，获取设备何时变为OFF，并更新界面
		if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_ONCE)
		{
			GX_ENUM_VALUE  nShutterEnumVal;   //<   保存曝光的枚举值
			
			//获取自动曝光组合框控件的指针
			CComboBox *pAutoShutterBox    = (CComboBox *)GetDlgItem(IDC_COMBO_ATUO_SHUTTER);
			
			//获取自动曝光的枚举值
			emStatus = GXGetEnumValue(m_hDevice, "ExposureAuto", &nShutterEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}
			
			//类型转换
			m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nShutterEnumVal.stCurValue.nCurValue;
			
			// 判断设备曝光模式是否变为OFF
			if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_OFF)
			{
				for (int i = 0;i < pAutoShutterBox->GetCount();i++)
				{
					if ((int64_t)pAutoShutterBox->GetItemData(i) == GX_EXPOSURE_AUTO_OFF)
					{
						//选中自动曝光控件中的OFF项,即由ONCE变为OFF
						pAutoShutterBox->SetCurSel(i);
						break;
					}
				}
				
				UpDateUI();
			}
		}

		//获取当前曝光值，更新界面
		emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &dValue);
		m_dShutterValue = dValue.dCurValue;
		strTemp.Format("%.4f", m_dShutterValue);
		SetDlgItemText(IDC_EDIT_CUR_SHUTTER, strTemp);
	}
	
	if (m_emAutoGainMode != GX_GAIN_AUTO_OFF)
	{
		//如果当前选择增益为ONCE时，获取设备何时变为OFF，并更新界面
		if (m_emAutoGainMode == GX_GAIN_AUTO_ONCE)
		{
			GX_ENUM_VALUE  nGainEnumVal;   //<   保存增益的枚举值
			
			//获取自动增益组合框控件的指针
			CComboBox *pAutoGainBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);
			
			//获取自动增益的枚举值
			emStatus = GXGetEnumValue(m_hDevice, "GainAuto", &nGainEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}
			
			// 类型转换
			m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nGainEnumVal.stCurValue.nCurValue;
			
			//判断设备增益模式是否变为OFF
			if (m_emAutoGainMode == GX_GAIN_AUTO_OFF)
			{
				for (int i = 0;i < pAutoGainBox->GetCount();i++)
				{
					if ((int64_t)pAutoGainBox->GetItemData(i) == GX_GAIN_AUTO_OFF)
					{
						//选中自动曝光控件中的OFF项，即由ONCE变为OFF
						pAutoGainBox->SetCurSel(i);
						break;
					}
				}
				
				UpDateUI();		
			}
		}

		// 获取当前增益值，更新界面
		emStatus  = GXGetFloatValue(m_hDevice, "Gain", &dValue);
		m_dGainValue = dValue.dCurValue;
		strTemp.Format("%.4f", m_dGainValue);
		SetDlgItemText(IDC_EDIT_CUR_GAIN, strTemp);
	}	
}

//-----------------------------------------------------------
/**
\brief   控制自动曝光和增益的ONCE变为OFF及刷新界面值

\return  无
*/
//-----------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default	

	// 刷新显示当前增益、曝光值
	RefreshCurValue();

	CDialog::OnTimer(nIDEvent);
}

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序调用函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 停止Timer
	KillTimer(0);
	
	// 若未停采则先停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// 注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		m_bIsSnap = FALSE;

		m_pBitmap->UnPrepareForShowImg();
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

//---------------------------------------------------------------------------------
/**
\brief   刷新UI界面

\return  无
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
\brief  弹出错误信息
\param  emErrorStatus  [in] 错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CGxAutoFuncCtrlDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// 获取错误信息长度，并申请内存资源
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// 释放申请的内存资源
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//---------------------------------------------------
/**
\brief   曝光Edit框控件失去焦点的响应函数

\return  无
*/
//---------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditCurShutter() 
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

	status = GXSetFloatValue(m_hDevice,"ExposureTime", m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//-------------------------------------------------------
/**
\brief    增益Edit框控件失去焦点的响应函数

\return   无
*/
//-------------------------------------------------------
void CGxAutoFuncCtrlDlg::OnKillfocusEditCurGain() 
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

	status = GXSetFloatValue(m_hDevice,"Gain",m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}
