// GxGetImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxGetImage.h"
#include "GxGetImageDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
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
	CString	m_strLegalCopyRight;
	CString	m_strProductVersion;
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
	m_strLegalCopyRight = _T("");
	m_strProductVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg dialog

CGxGetImageDlg::CGxGetImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxGetImageDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxGetImageDlg)
	m_nEditTimeOut = 500;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    // 初始化参数
	memset(&m_stFrameData,0,sizeof(GX_FRAME_DATA)); 

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxGetImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxGetImageDlg)
	DDX_Text(pDX, IDC_EDIT_TIME_OUT, m_nEditTimeOut);
	DDV_MinMaxUInt(pDX, m_nEditTimeOut, 0, 3600000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxGetImageDlg, CDialog)
	//{{AFX_MSG_MAP(CGxGetImageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START_SNAP,OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP,OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE,OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE,OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_SEND_SOFT_COMMAND,OnBnClickedBtnSendSoftCommand)
	ON_BN_CLICKED(IDC_BTN_CLEAN_TIME,OnBnClickedBtnCleanTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg message handlers
BOOL CGxGetImageDlg::OnInitDialog()
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
	
	// 初始化设备库
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	//获取图像显示窗口的指针和绘图DC句柄
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);

	//刷新界面
	UpdateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxGetImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CGxGetImageDlg::OnPaint() 
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
HCURSOR CGxGetImageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//----------------------------------------------------------------------------------
/**
\brief  打开设备

\return 打开设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::OpenDevice()
{
	GX_STATUS      emStatus    = GX_STATUS_SUCCESS;	
	GX_OPEN_PARAM  stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// 初始化设备打开参数,默认打开序号为1的设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// 打开枚举列表中的第一台设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	VERIFY_STATUS_RET(emStatus)

	// 获取流通道数
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	VERIFY_STATUS_RET(emStatus)

	if(0 < nDSNum)
	{
		// 获取流句柄
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		VERIFY_STATUS_RET(emStatus)
	}

	return GX_STATUS_SUCCESS;
}

//----------------------------------------------------------------------------------
/**
\brief  关闭设备

\return 关闭设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::CloseDevice()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 释放资源
    UnPrepareForShowImg();
	
	// 关闭设备
	emStatus = GXCloseDevice(m_hDevice);
		
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  为保存图像分配Buffer,为图像显示准备资源

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CGxGetImageDlg::PrepareForShowImg()
{	
	bool bRet = true;
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 得到图像的payloadSize
	uint32_t ui32PayLoadSize = 0;
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	//为GetImage参数分配空间
	m_stFrameData.pImgBuf     = new(std::nothrow) BYTE[ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		bRet = false;
	}

	emStatus = m_pBitmap->PrepareForShowImg();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		bRet = false;
	}
	// 若分配失败则释放已分配的资源
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  释放为图像显示准备资源

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UnPrepareForShowImg()
{
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete []m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}
	if (NULL != m_pBitmap) 
	{
		m_pBitmap->UnPrepareForShowImg();
	}

}

// ---------------------------------------------------------------------------------
/**
\brief   相机初始化

\return  无
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);
	
	//设置触发模式为开
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnumValueByString(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);
 
	//选择触发源为软触发
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerSource", "Software");

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   刷新UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UpdateUI()
{
	GetDlgItem(IDC_EDIT_TIME_OUT)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_BTN_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bIsOpen && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bIsSnap);
    GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief  消息分派函数

\return BOOL
*/
//----------------------------------------------------------------------------------
BOOL CGxGetImageDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd  *pWnd     = NULL;
	int   nCtrlID   = 0;
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		pWnd = GetFocus();
		nCtrlID = pWnd->GetDlgCtrlID();
		switch(nCtrlID)
		{
		case IDC_EDIT_TIME_OUT:
				::SetFocus(NULL);
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

//----------------------------------------------------------------------------------
/**
\brief  关闭应用程序响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (m_bIsSnap) 
	{
		m_pBitmap->UnPrepareForShowImg();
	}

	// 未关闭设备则关闭
	if (m_bIsOpen)
	{
		emStatus = CloseDevice();
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	// 释放设备库
	emStatus = GXCloseLib();

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  弹出错误信息
\param  status  错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// 获取错误信息长度，并申请内存空间
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
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

	// 释放申请的内存空间
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//为图像显示准备资源
	bool bRet = PrepareForShowImg();
	if (!bRet)
	{
		MessageBox("为图像显示准备资源失败!");
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// 使用GxGetImage获取图像,只需要发送开采命令
	// 发送开采命令
	emStatus = GXSetCommandValue(m_hDevice,"AcquisitionStart");
	GX_VERIFY(emStatus);

	// 更新开采标识
	m_bIsSnap = true;

	// 更新界面
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//停止采集
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	// 释放资源
	UnPrepareForShowImg();

	//更新开采标识为false
	m_bIsSnap = false;

	//复位统计时间数据
	m_objStatistic.Reset();

	//更新界面
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"打开设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bRet     = true;
	uint32_t  nDevNum  = 0;

	// 枚举设备
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDevNum <= 0)
	{
		MessageBox("未发现设备!");
		return;
	}

	// 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	//打开相机
	emStatus = OpenDevice();
	GX_VERIFY(emStatus);

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

	do
	{
		//初始化图像显示模块
		m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		//设置设备的默认参数，采集模式:连续采集,触发开关:开,触发源:软触发
		emStatus = InitDevice();
		GX_VERIFY_BREAK(emStatus);

	}while(false);
	
	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	// 更新设备打开标识
	m_bIsOpen = true;

	//刷新界面
	UpdateUI();

	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 如果未停采则先停止采集
	if(m_bIsSnap)
	{
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
        if (emStatus != GX_STATUS_SUCCESS)
        {
			// 错误处理
        }
		m_bIsSnap = false;
	}

	//关闭相机
	emStatus = CloseDevice();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	// 释放资源
	UnPrepareForShowImg();


	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_bIsOpen = false;
	m_hDevice = NULL;

	//刷新界面
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"软触发采图"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnSendSoftCommand()
{
	// TODO: Add your control notification handler code here
	if (!UpdateData(TRUE))
	{
		m_nEditTimeOut = 500;
		return;
	}

	GX_STATUS emStatus  = GX_STATUS_ERROR;
	double dElapsedtime = 0;

	//每次发送触发命令之前清空采集输出队列
	//防止库内部缓存帧，造成本次GXGetImage得到的图像是上次发送触发得到的图
	emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);

	//发送软触发命令
	emStatus = GXSetCommandValue(m_hDevice,"TriggerSoftware");
	GX_VERIFY(emStatus);

	//计时开始
	m_objStopTime.Start();

	//获取图像
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, m_nEditTimeOut);

	//结束计时
	dElapsedtime = m_objStopTime.Stop();

	//判断GXGetImage返回值
	GX_VERIFY(emStatus);

	//显示图像
	m_pBitmap->Show(&m_stFrameData);

	//更新界面的时间统计数据
	UpdateStatisticalData(dElapsedtime);
}

//----------------------------------------------------------------------------------
/**
\brief  更新界面的统计数据
\param  dData  [in]   时间间隔

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UpdateStatisticalData(double dData)
{
	CString strTemp = _T("");

	m_objStatistic.AddStatisticalData(dData);

	// 获取平均值并显示
    strTemp.Format("%.3f",m_objStatistic.GetAverage());
	SetDlgItemText(IDC_EDIT_AVERAGE,strTemp);

    // 获取最大值并显示
	strTemp.Format("%.3f",m_objStatistic.GetMax());
	SetDlgItemText(IDC_EDIT_MAX_VALUE,strTemp);

	// 获取最小值并显示
	strTemp.Format("%.3f",m_objStatistic.GetMin());
	SetDlgItemText(IDC_EDIT_MIN_VALUE,strTemp);
}

//----------------------------------------------------------------------------------
/**
\brief  点击显示时间的"清空"按钮消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnCleanTime()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TIME_WND,"");
}
