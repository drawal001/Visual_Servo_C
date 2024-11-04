// GxCameraEventsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxCameraEvents.h"
#include "GxCameraEventsDlg.h"

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
// CGxCameraEventsDlg dialog

CGxCameraEventsDlg::CGxCameraEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxCameraEventsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxCameraEventsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDevice                     = NULL;            // 设备句柄
	m_hStream                     = NULL;			 // 设备流句柄
	m_pLogWnd                     = NULL;            // 指向记录时间窗口
	m_bIsOpen                     = false;           // 相机打开标识
	m_bIsSnap                     = false;           // 相机采集标识
	m_bImplementEventSelector     = false;           // 相机是否支持事件源选择
	m_bSupportExposureEndEvent    = false;           // 是否支持曝光结束事件
	m_bIsTrigValid                = true;            // 触发是否有效标志
}

void CGxCameraEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxCameraEventsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxCameraEventsDlg, CDialog)
	//{{AFX_MSG_MAP(CGxCameraEventsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_BN_CLICKED(IDC_SEND_SOFT_COMMAND, OnSendSoftCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxCameraEventsDlg message handlers

BOOL CGxCameraEventsDlg::OnInitDialog()
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
	// 初始化库
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 指向时间显示窗口指针
	m_pLogWnd = GetDlgItem(IDC_EDIT_TIME_LOG); 

	// 更新UI界面
	UpdateUI();	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxCameraEventsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxCameraEventsDlg::OnPaint() 
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
HCURSOR CGxCameraEventsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGxCameraEventsDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 未停采则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// 注销采集回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		// 注销曝光结束事件回调函数
 		emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);			
 		m_bIsSnap = false;
	}
	
	if (m_bIsOpen)
	{
		// 关闭相机
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;
	}		
	
	// 关闭设备库
	emStatus = GXCloseLib();
	
	CDialog::OnClose();
}

void CGxCameraEventsDlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus     = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum   = 0;
	int64_t        nPixelFormat = 0;        
	GX_OPEN_PARAM  stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// 枚举设备个数
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);
	
	// 判断当前连接设备个数
	if (nDeviceNum <= 0)
	{
		MessageBox("未发现设备!");
		return;
	}

	// 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}
	
	// 打开枚举列表中的第一台设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);

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
	
	// 初始化参数
	emStatus = InitParam();
	GX_VERIFY(emStatus);

	// 判断是否支持曝光结束事件
	if (!m_bSupportExposureEndEvent)
	{
		return;
	}

	m_bIsOpen = true;	
	
	// 更新UI界面
	UpdateUI();	
}

void CGxCameraEventsDlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 注册曝光结束事件回调函数
	emStatus = GXRegisterFeatureCallbackByString(m_hDevice, this, OnFeatureCallbackFun, "EventExposureEnd", &m_hCB);
	GX_VERIFY(emStatus);
		
	// 注册采集回调函数
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	GX_VERIFY(emStatus);

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// 发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	GX_VERIFY(emStatus);
	m_bIsSnap = true;
	m_bIsTrigValid = true;
	
	// 更新UI界面
	UpdateUI();	
}

void CGxCameraEventsDlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// 注销采集回调函数
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	// 注销曝光结束事件回调函数
 	emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);
 	GX_VERIFY(emStatus);
	m_bIsSnap = false;
	
	// 更新UI界面
	UpdateUI();
}

void CGxCameraEventsDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 未停采时则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		
		// 注销采集回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		// 注销曝光结束事件回调函数
		emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		m_bIsSnap = false;
	}
	
	// 关闭相机
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}
	m_hDevice = NULL;
	m_bIsOpen = false;
	
	// 更新UI界面
	UpdateUI();	
}

void CGxCameraEventsDlg::OnSendSoftCommand() 
{
	// TODO: Add your control notification handler code here
	// 如果当触发回调正在执行的过程中，再次点击触发按键后，此次点击会被屏蔽掉
	if (!m_bIsTrigValid)
	{
		return;
	}
	else
	{
		m_bIsTrigValid = false;
	}

	SYSTEMTIME system;
	CString    strTime = "";
	GX_STATUS  emStatus = GX_STATUS_SUCCESS;

	// 开始发送软触发命令的时间(此代码若放到两个计时开始之前，导致打印时间大于进入回调时间，出现问题)
	GetLocalTime(&system);
	strTime.Format("%02dh:%02dm:%02ds:%03dms ", system.wHour, system.wMinute, system.wSecond, system.wMilliseconds);
	PrintInfotoWnd(MsgComposer("\r\n开始发送软触发命令的时间点：" + strTime));

	// 到收到图像历时时间：计时开始
	m_objImageTime.Start();

	// 到曝光结束事件历时时间：计时开始
	m_objShutterTime.Start();

	// 发送软触发命令
	emStatus = GXSetCommandValue(m_hDevice,"TriggerSoftware");
	if(emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus); 
		m_bIsTrigValid = true;
		return;
	} 
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxCameraEventsDlg::InitParam()
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;

	// 设置采集模式为连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发模式为开
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// 选择触发源为软触发
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerSource", "Software");
	VERIFY_STATUS_RET(emStatus);

	// 判断是否支持事件相关功能
	emStatus = __IsSupportEvent(m_bSupportExposureEndEvent);
	if (!m_bSupportExposureEndEvent)
	{
		MessageBox("相机不支持曝光结束事件，演示程序对该相机无效!");
		return emStatus;
	}
	
	// 选择曝光完成事件
	emStatus = GXSetEnumValueByString(m_hDevice, "EventSelector", "ExposureEnd");
	VERIFY_STATUS_RET(emStatus);
	
	// 曝光完成事件使能
	emStatus = GXSetEnumValueByString(m_hDevice, "EventNotification", "On");
	VERIFY_STATUS_RET(emStatus);
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数
\param   pFrame   回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxCameraEventsDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxCameraEventsDlg *pDlg = (CGxCameraEventsDlg*)(pFrame->pUserParam);
	double dElapsedtime = 0;
	char   chTemp[8]    = "ms";

	// 结束计时
	dElapsedtime = pDlg->m_objImageTime.Stop();
	pDlg->PrintInfotoWnd(MsgComposer("开始发送软触发命令到成功收到图像历时：%0.2f %s", dElapsedtime, chTemp));

	pDlg->m_bIsTrigValid = true;
}

//---------------------------------------------------------------------------------
/**
\brief    曝光事件回调函数
\param    emEventID		事件类型ID号
\param	  pUserParam    用户私有参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxCameraEventsDlg::OnFeatureCallbackFun(const char* chFeature, void* pUserParam)
{
	CGxCameraEventsDlg *pDlg = (CGxCameraEventsDlg*)pUserParam;
    double dElapsedtime = 0;
	char   chTemp[8]    = "ms";

	if (chFeature == "EventExposureEnd")
	{
		// 结束计时
		dElapsedtime = pDlg->m_objShutterTime.Stop();
		pDlg->PrintInfotoWnd(MsgComposer("开始发送软触发命令到曝光结束事件历时：%0.2f %s", dElapsedtime, chTemp));
	}
}

//----------------------------------------------------------------------------------
/**
\brief  在时间显示窗口打印时间信息
\param  strInfo 输入打印的信息

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::PrintInfotoWnd(string &strInfo)
{
	int    nTextLen = 0;
	CEdit  *pEdit = (CEdit*)m_pLogWnd;

	// 输出窗口句柄不为空
	if (m_pLogWnd != NULL) 
	{
		nTextLen = pEdit->GetWindowTextLength();	
		if (pEdit->GetLineCount() > 300)
		{
			pEdit->Clear();
			pEdit->SetWindowText(_T(""));
			nTextLen = pEdit->GetWindowTextLength();
		}
		
		pEdit->SetSel(nTextLen, -1);	
		strInfo.append("\r\n");
		pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());
		
		if(nTextLen > 0)
		{
			pEdit->SetSel((nTextLen - pEdit->GetLineCount() + 1), -1);
		}
		else
		{
			pEdit->SetSel(nTextLen, -1);
		}
	}
}

//----------------------------------------------------------------------------------
/**
\brief  获取错误信息描述
\param  emErrorStatus  错误码

 \return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::ShowErrorString(GX_STATUS emErrorStatus)
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
void CGxCameraEventsDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//---------------------------------------------------------------------------------
/**
\brief   消息分派函数(主要处理回车消息)
\param   pMsg  消息结构体

\return  BOOL
*/
//----------------------------------------------------------------------------------
BOOL CGxCameraEventsDlg::PreTranslateMessage(MSG* pMsg) 
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

//---------------------------------------------------------------------------------
/**
\brief   判断是否支持事件相关功能
\param   bSupportExposureEndEvent [out]  是否支持曝光结束事件标志

\return  成功: GX_STATUS_SUCCESS, 失败: 非GX_STATUS_SUCCESS
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxCameraEventsDlg::__IsSupportEvent(bool &bSupportExposureEndEvent)
{
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;

	// 判断设备是否支持事件源选择
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "EventSelector", &emAccessMode);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return emStatus;
	}

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementEventSelector = true;
	}


	if (m_bImplementEventSelector)
	{
		GX_ENUM_VALUE stEnumValue;
		size_t    nbufferSize    = 0;
		uint32_t  nEntryNum      = 0;
		int64_t   nEnumValue     = -1;
		int       nCurcel        = 0;
		
		// 获取功能的枚举数
		emStatus = GXGetEnumValue(m_hDevice, "EventSelector", &stEnumValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			return emStatus;
		}

		nEntryNum = stEnumValue.nSupportedNum;
		
		// 遍历查询是否支持曝光结束事件
		for (uint32_t i = 0; i < nEntryNum; i++)
		{	
			if (stEnumValue.nArrySupportedValue[i].nCurValue == GX_ENUM_EVENT_SELECTOR_EXPOSUREEND)
			{
				bSupportExposureEndEvent = true;
				break;
			}
		}
	}

	return emStatus;
}