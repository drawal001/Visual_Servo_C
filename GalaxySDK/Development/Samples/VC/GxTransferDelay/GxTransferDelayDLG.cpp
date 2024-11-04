// GxTransferDelayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxTransferDelay.h"
#include "GxTransferDelayDlg.h"

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
// CGxTransferDelayDlg dialog

CGxTransferDelayDlg::CGxTransferDelayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxTransferDelayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxTransferDelayDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nDevNum                     = 0;            // 设备个数
	m_pDeviceHandle               = NULL;		  // 设备句柄指针
	m_pStream					  = NULL;	      // 设备流句柄
	m_pCBHandle                   = NULL;         // 曝光回调函数句柄
	m_bIsOpen                     = false;        // 相机打开标志 
	m_bIsSnap                     = false;        // 相机开始采集标志 
	m_hCam0StartTranEvent         = NULL;         // 0号相机启动传输事件
	m_hCam1StartTranEvent         = NULL;         // 1号相机启动传输事件
	m_hCam2StartTranEvent         = NULL;         // 2号相机启动传输事件
	m_hCam3StartTranEvent         = NULL;         // 3号相机启动传输事件
	m_hCam0StopSnapEvent          = NULL;         // 0号相机停止采集事件
	m_hCam1StopSnapEvent          = NULL;         // 1号相机停止采集事件
	m_hCam2StopSnapEvent          = NULL;         // 2号相机停止采集事件
	m_hCam3StopSnapEvent          = NULL;         // 3号相机停止采集事件
	m_pLogWnd                     = NULL;         // 指向日志显显示窗口的指针

	// 显示窗口资源初始化
	for (int i = 0; i < MAX_NUM_CAMERA; i++)
	{
		m_hWndHandle[i] = NULL;
		m_pBitmap[i] = NULL;
	} 

}     

void CGxTransferDelayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxTransferDelayDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxTransferDelayDlg, CDialog)
	//{{AFX_MSG_MAP(CGxTransferDelayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxTransferDelayDlg message handlers

BOOL CGxTransferDelayDlg::OnInitDialog()
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

	//初始化库
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 初始化资源
	bool bReturn = InitResources();
	if (!bReturn)
	{
		MessageBox("初始化资源失败！");
		exit(0);
	}

	// 显示日志窗口指针
	m_pLogWnd = GetDlgItem(IDC_EDIT_LOG_WND);
	UpdateUI();

	// 获取画图窗口指针和句柄
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_ZERO, &m_hWndHandle[0]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_ONE, &m_hWndHandle[1]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_TWO, &m_hWndHandle[2]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_THREE, &m_hWndHandle[3]);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxTransferDelayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char chFileName[MAX_PATH] = {'\0'};
        GetModuleFileName(NULL, chFileName, MAX_PATH);
        CFileVersion fv(chFileName);
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

void CGxTransferDelayDlg::OnPaint() 
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
HCURSOR CGxTransferDelayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 如果没有停采，则停止采集
	if (m_bIsSnap)
	{
		// 将相机的停采事件设置为有信号
		SetEvent(m_hCam0StopSnapEvent);
		SetEvent(m_hCam1StopSnapEvent);
		SetEvent(m_hCam2StopSnapEvent);
		SetEvent(m_hCam3StopSnapEvent);

		// 循环停采
		for (i = 0; i < m_nDevNum; i++)
		{
			// 发送停止采集命令
			emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
			
			// 清理事件队列
			emStatus = GXFlushEvent(m_pDeviceHandle[i]);
			m_pBitmap[i]->UnPrepareForShowImg();
		}	
	}

	// 如果没有关闭相机，则关闭相机
	if (m_bIsOpen)
	{
		// 循环关闭相机
		for (i = 0; i < m_nDevNum; i++)
		{
			// 注销回调函数
			emStatus = GXUnregisterCaptureCallback(m_pDeviceHandle[i]);
			
			// 注销曝光事件回调函数
			emStatus = GXUnregisterFeatureCallbackByString(m_pDeviceHandle[i], "EventExposureEnd", m_pCBHandle[i]);
			
			// 关闭相机
			emStatus = GXCloseDevice(m_pDeviceHandle[i]);
			m_pDeviceHandle[i] = NULL;

			if (NULL != m_pBitmap[i])
			{
				delete m_pBitmap[i];
				m_pBitmap[i] = NULL;
			}
		}
	}
	
	// 释放资源
	ReleaseResource();

	// 关闭设备库
	emStatus = GXCloseLib(); 
	
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"打开设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	uint32_t  nDeviceNum  = 0;
	char      chIndex[10] = {0};
	int       i           = 0;
	uint32_t	  nDSNum   = 0;
	bool      bReturn     = false;

	// 定义并初始化设备打开参数
    GX_OPEN_PARAM stOpenParam;
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode   = GX_OPEN_INDEX;
    stOpenParam.pszContent = "";

	// 枚举设备个数
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);
	if (nDeviceNum <= 0)
	{
		MessageBox("无设备接入");
		return;
	}

	// 最多支持4只相机
	m_nDevNum = min(nDeviceNum, MAX_NUM_CAMERA);

	// 遍历所有的相机
	for (i = 0; i < m_nDevNum; i++)
	{
		// 相机序号从1开始 
		_itoa(i + 1, chIndex, 10);

		// 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
		if (m_pDeviceHandle[i] != NULL)
		{
			emStatus = GXCloseDevice(m_pDeviceHandle[i]);
			GX_VERIFY(emStatus);
			m_pDeviceHandle[i] = NULL;
		}
		
		// 打开相机
		stOpenParam.pszContent = chIndex;
		emStatus = GXOpenDevice(&stOpenParam, &m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);

		 //获取流通道数
		emStatus = GXGetDataStreamNumFromDev(m_pDeviceHandle[i], &nDSNum);
		GX_VERIFY(emStatus);

		if(0 < nDSNum)
		{
			// 获取流句柄
			emStatus = GXGetDataStreamHandleFromDev(m_pDeviceHandle[i], 1, &m_pStream[i]);
			GX_VERIFY(emStatus);
		}

		 //建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
		 //以提高网络相机的采集性能,设置方法参考以下代码。
		{
			bool	 bImplementPacketSize = false;
			uint32_t unPacketSize		  = 0;

			// 判断设备是否支持流通道数据包功能
			GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
			emStatus = GXGetNodeAccessMode(m_pDeviceHandle[i], "GevSCPSPacketSize", &emAccessMode);
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
				emStatus = GXGetOptimalPacketSize(m_pDeviceHandle[i],&unPacketSize);
				GX_VERIFY(emStatus);

				// 将最优包长值设置为当前设备的流通道包长值
				emStatus = GXSetIntValue(m_pDeviceHandle[i], "GevSCPSPacketSize", unPacketSize);
				GX_VERIFY(emStatus);
			}
		}
		
		// 设置相机参数
		emStatus = SetCameraParam(m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);
		
		do
		{
			m_pBitmap[i] = new CGXBitmap(m_pDeviceHandle[i], &m_hWndHandle[i]);
			if (NULL == m_pBitmap[i])
			{
				ShowErrorString(GX_STATUS_ERROR);
				break;
			}

			// 注册回调函数
			emStatus = RegisterCallback(i);
			GX_VERIFY_BREAK(emStatus);

		}while(false);
		
		if((GX_STATUS_SUCCESS != emStatus)
			&& NULL != m_pBitmap[i])
		{
			delete m_pBitmap[i];
			m_pBitmap[i] = NULL;
			return;
		}
	}	

	m_bIsOpen = true;
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   注册回调函数
\param   nDevNum   注册的相机编号

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxTransferDelayDlg::RegisterCallback(int nDevNum)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 注册采集回调函数和曝光结束事件回调函数
 	switch(nDevNum)
 	{
 	case CAMERA0:
 		// 注册曝光结束事件回调函数
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA0], this, OnFeatureCallbackFun0, 
		                                     "EventExposureEnd", &m_pCBHandle[CAMERA0]);
		VERIFY_STATUS_RET(emStatus); 
		
		// 注册采集回调函数
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA0], this, OnFrameCallbackFun0);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA1:
		// 注册曝光结束事件回调函数
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA1], this, OnFeatureCallbackFun1, 
			                                 "EventExposureEnd", &m_pCBHandle[CAMERA1]);
		VERIFY_STATUS_RET(emStatus);
		
		// 注册采集回调函数
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA1], this, OnFrameCallbackFun1);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA2:
		// 注册曝光结束事件回调函数
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA2], this, OnFeatureCallbackFun2, 
		                           	         "EventExposureEnd", &m_pCBHandle[CAMERA2]);
		VERIFY_STATUS_RET(emStatus);
		
		// 注册采集回调函数
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA2], this, OnFrameCallbackFun2);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA3:
		// 注册曝光结束事件回调函数
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA3], this, OnFeatureCallbackFun3, 
			                                 "EventExposureEnd", &m_pCBHandle[CAMERA3]);
		VERIFY_STATUS_RET(emStatus);
		
		// 注册采集回调函数
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA3], this, OnFrameCallbackFun3);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	default:
		break;
 	}
	return emStatus;
}
//---------------------------------------------------------------------------------
/**
\brief   设置相机参数
\param   hDevice   相机句柄 

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxTransferDelayDlg::SetCameraParam(GX_DEV_HANDLE hDevice)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 已知当前相机支持哪个8位图像数据格式可以直接设置，以8位图像数据格式GX_PIXEL_FORMAT_BAYER_GR8为例:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");

	// 设置采集模式为连续采集
	emStatus = GXSetEnumValueByString(hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);
	
	// 设置触发模式为开
	emStatus = GXSetEnumValueByString(hDevice, "TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发类型
	emStatus = GXSetEnumValueByString(hDevice, "TriggerSelector", "FrameStart");
	VERIFY_STATUS_RET(emStatus);
	
	// 选择触发源
	emStatus = GXSetEnumValueByString(hDevice, "TriggerSource", "Line0");
	VERIFY_STATUS_RET(emStatus);
	
	// 选择曝光完成事件
	emStatus = GXSetEnumValueByString(hDevice, "EventSelector", "ExposureEnd");
	VERIFY_STATUS_RET(emStatus);
	
	// 曝光完成事件使能
	emStatus = GXSetEnumValueByString(hDevice, "EventNotification", "On");
	VERIFY_STATUS_RET(emStatus);

	// 设置传输控制模式
	emStatus = GXSetEnumValueByString(hDevice, "TransferControlMode", "UserControlled");
	VERIFY_STATUS_RET(emStatus);

	// 设置传输操作模式
	emStatus = GXSetEnumValueByString(hDevice, "TransferOperationMode", "MultiBlock");
	VERIFY_STATUS_RET(emStatus);

	// 设置传输帧数
	emStatus = GXSetIntValue(hDevice, "TransferBlockCount", TRANSFER_COUNT);
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 对m_nDevNum个相机进行操作
	for (i = 0; i < m_nDevNum; i++)
	{
		emStatus = m_pBitmap[i]->PrepareForShowImg();
		GX_VERIFY(emStatus);

		//设置流层Buffer处理模式
		emStatus = GXSetEnumValueByString(m_pStream[i], "StreamBufferHandlingMode", "OldestFirst");
		GX_VERIFY(emStatus);

		// 发开始采集命令
		emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStart");
		GX_VERIFY(emStatus);
	}

	// 将0号相机启动传输事件设置为有信号，其它相机设置为无信号
	SetEvent(m_hCam0StartTranEvent);
	ResetEvent(m_hCam1StartTranEvent);
	ResetEvent(m_hCam2StartTranEvent);
	ResetEvent(m_hCam3StartTranEvent);


	// 将相机的停采事件信号设置为无信号
	ResetEvent(m_hCam0StopSnapEvent);
	ResetEvent(m_hCam1StopSnapEvent);
	ResetEvent(m_hCam2StopSnapEvent);
	ResetEvent(m_hCam3StopSnapEvent);

	m_bIsSnap = true;	
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief    0号相机曝光完成事件回调函数
\param    emEventID		事件类型ID号
\param	  pUserParam    用户私有参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun0(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam0StopSnapEvent, pDlg->m_hCam0StartTranEvent};

	// 判断是否是曝光结束事件
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("0号相机出现曝光完成事件ID不正确"));
		return;
	}

	// 事件判断
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM0_WAIT_TIME))
	{
	// 等待超时
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("0号相机等待超时，请停止采集，重新开采"));
		break;

	// 传输延时事件
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA0], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// 停采事件
	case WAIT_OBJECT_0:
		break;

	// 其他
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    1号相机曝光完成事件回调函数
\param    emEventID		事件类型ID号
\param	  pUserParam    用户私有参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun1(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam1StopSnapEvent, pDlg->m_hCam1StartTranEvent};
	
	// 判断是否是曝光结束事件
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("1号相机出现曝光完成事件ID不正确"));
		return;
	}

	// 事件判断
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM1_WAIT_TIME))
	{
	// 等待超时
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("1号相机等待超时，请停止采集，重新开采"));
		break;

	// 传输延时事件
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA1], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// 停采事件
	case WAIT_OBJECT_0:
		break;

	// 其他
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    2号相机曝光完成事件回调函数
\param    emEventID		事件类型ID号
\param	  pUserParam    用户私有参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun2(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam2StopSnapEvent, pDlg->m_hCam2StartTranEvent};

	// 判断是否是曝光结束事件
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("2号相机出现曝光完成事件ID不正确"));
		return;
	}

	// 事件判断
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM2_WAIT_TIME))
	{
	// 等待超时
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("2号相机等待超时，请停止采集，重新开采"));
		break;

	// 传输延时事件
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA2], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// 停采事件
	case WAIT_OBJECT_0:
		break;

	// 其他
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    3号相机曝光完成事件回调函数
\param    emEventID		事件类型ID号
\param	  pUserParam    用户私有参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun3(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam3StopSnapEvent, pDlg->m_hCam3StartTranEvent};

	// 判断是否是曝光结束事件
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("3号相机出现曝光完成事件ID不正确"));
		return;
	}

	// 事件判断
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM3_WAIT_TIME))
	{
	// 等待超时
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("3号相机等待超时，请停止采集，重新开采"));
		break;

	// 传输延时事件
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA3], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// 停采事件
	case WAIT_OBJECT_0:
		break;

	// 其他
	default:	
		break;
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数
\param   pFrame  回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun0(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("0号相机出现残帧"));
	}

	// 启动1号相机
	SetEvent(pDlg->m_hCam1StartTranEvent);

	int		    nID       = 0;             // ID号

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数
\param   pFrame  回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("1号相机出现残帧"));
	}

	// 启动2号相机
	SetEvent(pDlg->m_hCam2StartTranEvent);
	
	int		    nID       = 1;             // ID号	

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数
\param   pFrame  回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("2号相机出现残帧"));
	}

	// 启动3号相机
	SetEvent(pDlg->m_hCam3StartTranEvent);
		
	int		    nID       = 2;             // ID号	

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数
\param   pFrame  回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("3号相机出现残帧"));
	}

	// 启动0号相机
	SetEvent(pDlg->m_hCam0StartTranEvent);
	
	int		    nID       = 3;             // ID号	

	pDlg->m_pBitmap[nID]->Show(pFrame);

}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 将相机的停采事件设置为有信号
	SetEvent(m_hCam0StopSnapEvent);
	SetEvent(m_hCam1StopSnapEvent);
	SetEvent(m_hCam2StopSnapEvent);
	SetEvent(m_hCam3StopSnapEvent);

	// 循环停采
	for (i = 0; i < m_nDevNum; i++)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
		GX_VERIFY(emStatus);

		// 清理事件队列
		emStatus = GXFlushEvent(m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);
	}

	m_bIsSnap = false;
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 如果没有停采，则停止采集
	if (m_bIsSnap)
	{
		// 将相机的停采事件设置为有信号
		SetEvent(m_hCam0StopSnapEvent);
		SetEvent(m_hCam1StopSnapEvent);
		SetEvent(m_hCam2StopSnapEvent);
		SetEvent(m_hCam3StopSnapEvent);

		// 循环停采
		for (i = 0; i < m_nDevNum; i++)
		{
			// 发送停止采集命令
		    emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}

		    // 清理事件队列
		    emStatus = GXFlushEvent(m_pDeviceHandle[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}
			m_pBitmap[i]->UnPrepareForShowImg();
		}
	}
	m_bIsSnap = false;

	// 循环关闭相机
	for (i = 0; i < m_nDevNum; i++)
	{
		// 注销采集回调函数
		emStatus = GXUnregisterCaptureCallback(m_pDeviceHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		
		// 注销曝光事件回调函数
		emStatus = GXUnregisterFeatureCallbackByString(m_pDeviceHandle[i], "EventExposureEnd", m_pCBHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		// 关闭相机
		emStatus = GXCloseDevice(m_pDeviceHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		if (NULL != m_pBitmap[i])
		{
			delete m_pBitmap[i];
			m_pBitmap[i] = NULL;
		}

		m_pDeviceHandle[i] = NULL;
	}
	m_bIsOpen = false;

	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  初始化资源(最多支持4只相机)

\return ture：表示成功，false：表示失败
*/
//----------------------------------------------------------------------------------
bool CGxTransferDelayDlg::InitResources()
{
	// 相机句柄
	m_pDeviceHandle = new GX_DEV_HANDLE[MAX_NUM_CAMERA];
	if (m_pDeviceHandle == NULL)
	{
		return false;
	}

	// 流句柄
	m_pStream = new GX_DEV_HANDLE[MAX_NUM_CAMERA];
	if (m_pStream == NULL)
	{
		return false;
	}

	// 曝光回调函数句柄
	m_pCBHandle = new GX_FEATURE_CALLBACK_HANDLE[MAX_NUM_CAMERA];
	if (m_pCBHandle == NULL)
	{
		ReleaseResource();
		return false;
	}

	// 创建0号相机启动传输事件
	m_hCam0StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer0StartTranEvent");
	if (m_hCam0StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
   	
	// 创建1号相机启动传输事件
	m_hCam1StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer1StartTranEvent");
	if (m_hCam1StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
	
	// 创建2号相机启动传输事件
	m_hCam2StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer2StartTranEvent");
	if (m_hCam2StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
	
	// 创建3号相机启动传输事件
	m_hCam3StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer3StartTranEvent");
	if (m_hCam3StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}	

	// 创建0号相机停止采集事件
	m_hCam0StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer0StopSnapEvent");
	if (m_hCam0StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// 创建1号相机停止采集事件
	m_hCam1StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer1StopSnapEvent");
	if (m_hCam1StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// 创建2号相机停止采事件
	m_hCam2StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer2StopSnapEvent");
	if (m_hCam2StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// 创建3号相机停止采集事件
	m_hCam3StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer3StopSnapEvent");
	if (m_hCam3StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}		
	
	// 初始化资源信息
	for (int i = 0; i < MAX_NUM_CAMERA; i++)
	{
		m_pDeviceHandle[i]             = NULL;
	    m_pStream[i]				   = NULL;
		m_pCBHandle[i]                 = NULL;
	}

	return true;
}

//----------------------------------------------------------------------------------
/**
\brief  释放资源

\return void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::ReleaseResource()
{
	// 释放设备句柄指针
	if(m_pDeviceHandle != NULL)
	{
		delete[]m_pDeviceHandle;
		m_pDeviceHandle = NULL;
	}

	// 释放流句柄指针
	if (m_pStream != NULL)
	{
		delete[]m_pStream;
		m_pStream = NULL;
	}

	// 释放回调句柄指针
	if (m_pCBHandle != NULL)
	{
		delete[]m_pCBHandle;
		m_pCBHandle = NULL;
	}

	// 关闭传输延迟事件句柄
    if (m_hCam0StartTranEvent != NULL)
    {
        CloseHandle(m_hCam0StartTranEvent);
        m_hCam0StartTranEvent = NULL;
    }
	if (m_hCam1StartTranEvent != NULL)
    {
        CloseHandle(m_hCam1StartTranEvent);
        m_hCam1StartTranEvent = NULL;
    }
	if (m_hCam2StartTranEvent != NULL)
    {
        CloseHandle(m_hCam2StartTranEvent);
        m_hCam2StartTranEvent = NULL;
    }
	if (m_hCam3StartTranEvent != NULL)
    {
        CloseHandle(m_hCam3StartTranEvent);
        m_hCam3StartTranEvent = NULL;
    }

	// 关闭停止采集事件句柄
	if (m_hCam0StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam0StopSnapEvent);
		m_hCam0StopSnapEvent = NULL;
	}
	if (m_hCam1StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam1StopSnapEvent);
		m_hCam1StopSnapEvent = NULL;
	}
	if (m_hCam2StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam2StopSnapEvent);
		m_hCam2StopSnapEvent = NULL;
	}
	if (m_hCam3StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam3StopSnapEvent);
		m_hCam3StopSnapEvent = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  获取错误信息描述
\param  emErrorStatus  错误码

\return void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// 获取错误描述信息长度
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// 获取错误信息描述
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	
	// 显示错误描述信息
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}
	
	// 释放资源
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief   刷新UI界面

\return  void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief  在日志显示窗口打印日志信息
\param  strInfo 指向日志信息的指针

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::PrintInfotoWnd(string &strInfo)
{
	SYSTEMTIME system;
	GetLocalTime(&system);
	string strTime = MsgComposer("%02dh:%02dm:%02ds:%03dms ",system.wHour, system.wMinute, system.wSecond, system.wMilliseconds);
	strInfo = strTime.append(strInfo);
	
	if (m_pLogWnd != NULL)// 输出窗口句柄不为空
	{
		CEdit *pEdit = (CEdit*)m_pLogWnd;
		int textLen = pEdit->GetWindowTextLength();
		
		if (pEdit->GetLineCount() > 300)
		{
			pEdit->Clear();
			pEdit->SetWindowText(_T(""));
			textLen = pEdit->GetWindowTextLength();
		}
		
		pEdit->SetSel(textLen, -1);	
		strInfo.append("\r\n");
		pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());
		
		if(textLen > 0)
		{
			pEdit->SetSel((textLen - pEdit->GetLineCount() + 1), -1);
		}
		else
		{
			pEdit->SetSel(textLen, -1);
		}
	}
}

//-----------------------------------------------------------------------
/**
\brief   分发函数主要处理曝光和增益的Edit框响应回车键的消息
\param   pMsg  消息结构体

\return  成功:TRUE   失败:FALSE
*/
//-----------------------------------------------------------------------
BOOL CGxTransferDelayDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))   
	{   
		return  TRUE; 
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}