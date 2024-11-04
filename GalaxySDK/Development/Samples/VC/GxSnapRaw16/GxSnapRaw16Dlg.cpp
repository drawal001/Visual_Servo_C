// GxSnapRaw16Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxSnapRaw16.h"
#include "GxSnapRaw16Dlg.h"

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
// CGxSnapRaw16Dlg dialog

CGxSnapRaw16Dlg::CGxSnapRaw16Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSnapRaw16Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxSnapRaw16Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strSavePath           = "";              // 图像保存路径
	m_hDevice               = NULL;            // 设备句柄
	m_hStream				= NULL;			   // 流句柄
	m_pRawBuf               = NULL;            // 原始Raw图像Buffer
	m_pRaw8Buf              = NULL;            // 8位Raw图像Buffer
	m_nPixelFilter     = 0;               // 彩色相机的Bayer格式
	m_bIsColorFilter        = false;           // 判断相机是否支持Bayer格式
	m_bIsOpen               = false;           // 相机打开标识
	m_bIsSnap               = false;           // 相机采集标识
	m_bIsSaveImg            = false;           // 保存图像标识
    m_hWndHandle            = NULL;            // 显示图像的窗口句柄 
	m_pBitmap               = NULL;
}

void CGxSnapRaw16Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxSnapRaw16Dlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxSnapRaw16Dlg, CDialog)
	//{{AFX_MSG_MAP(CGxSnapRaw16Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_RAW_IMAGE, OnSaveRawImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxSnapRaw16Dlg message handlers

BOOL CGxSnapRaw16Dlg::OnInitDialog()
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
	int     nPos = 0;
	char    chPathName[MAX_PATH] = {0};   
	CString strPathName = ""; 

	// 初始化库
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 初始化图像保存路径
	GetModuleFileName(NULL, chPathName, MAX_PATH);
	strPathName = chPathName;
	nPos = strPathName.ReverseFind('\\');
	m_strSavePath += strPathName.Left(nPos);
	m_strSavePath += "\\GxSnapRaw16Images";

	// 获取画图窗口指针和句柄
	GetDlgItem(IDC_STATIC_SHOW_IMAGE, &m_hWndHandle);
    m_hDC  = ::GetDC(m_hWndHandle); 
	::GetClientRect(m_hWndHandle, &m_objRect);	     
	
	// 更新UI界面
	UpdateUI();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSnapRaw16Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxSnapRaw16Dlg::OnPaint() 
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
HCURSOR CGxSnapRaw16Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 未停采则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// 注销回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}
	
	// 未关闭设备则关闭设备
	if (m_bIsOpen)
	{
		// 关闭相机
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;
	}
	
	// 关闭设备库
	emStatus = GXCloseLib();
	
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	CDialog::OnClose();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"打开设备"控件响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus     = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum   = 0;
	int64_t        nPixelFormat = 0;   
	uint32_t	   nDSNum       = 0;
	GX_OPEN_PARAM  stOpenParam;
	
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

	do
	{
		//初始化图像显示模块
		m_pBitmap = new CGXBitmap(m_hDevice, &m_hWndHandle);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// 初始化参数
		emStatus = InitParam();
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	m_bIsOpen = true;	

	// 更新UI界面
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"开始采集"控件响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	m_pBitmap->PrepareForShowImg();
	// 注册回调函数
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// 发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = true;

	// 更新UI界面
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"停止采集"控件响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// 注销回调函数
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);
  
	m_bIsSnap = false;

	// 更新UI界面
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"关闭设备"控件响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnCloseDevice() 
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
		
		// 注销回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		
		// 释放图像显示资源
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
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

//---------------------------------------------------------------------------------
/**
\brief   点击"保存Raw图像"CheckBox控件响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnSaveRawImage() 
{
	// TODO: Add your control notification handler code here
	m_bIsSaveImg = !m_bIsSaveImg;	
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxSnapRaw16Dlg::InitParam()
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	CString   strSymbolic = "";
	 GX_ENUM_VALUE stEnumValue;
	
	// 设置采集模式为连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// 已知当前相机支持哪个非8位的图像数据格式可以直接设置,以非8位数据格式GX_PIXEL_FORMAT_BAYER_GR12为例：
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR12");
    // 以非8位数据格式GX_PIXEL_FORMAT_MONO10为例:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "Mono10");
	
	// 不知道当前相机支持哪个非8位的图像数据格式，可以调用函数SetPixelSize16将图像数据格式设置为非8位的
	emStatus = SetPixelSize16(&strSymbolic);
	VERIFY_STATUS_RET(emStatus);

	// 将设置的数据格显示到界面
	GetDlgItem(IDC_EDIT_PIXEL_FORMAT)->SetWindowText(strSymbolic);

	// 获取输出像素格式
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		m_nPixelFilter = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// 获取当前像素格式
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelFilter = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}

	m_bIsColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_nPixelFilter));

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief    设置图像数据格式为16位
\param    pstrSymbolic  16位图像数据格式字符描述

\return   emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxSnapRaw16Dlg::SetPixelSize16(CString  *pstrSymbolic)
{
	uint32_t  i           = 0;                   
	uint32_t  nEnmuEntry  = 0;
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	GX_ENUM_VALUE stEnumValue;

	// 获取设备支持的像素格式的枚举项个数
	emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		*pstrSymbolic = "\0";
		return emStatus;
	}
	nEnmuEntry = stEnumValue.nSupportedNum;

	for (i = 0; i < nEnmuEntry; i++)
	{
		if ((stEnumValue.nArrySupportedValue[i].nCurValue & GX_PIXEL_16BIT) == GX_PIXEL_16BIT)
		{
			emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", stEnumValue.nArrySupportedValue[i].nCurValue);
			*pstrSymbolic = stEnumValue.nArrySupportedValue[i].strCurSymbolic;
			break;
		}
	}	
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数:图像获取、保存和显示操作
\param   pFrame   回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxSnapRaw16Dlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxSnapRaw16Dlg *pDlg   = (CGxSnapRaw16Dlg*)(pFrame->pUserParam);
	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		return;
	}


	// 图像保存
	if (pDlg->m_bIsSaveImg)
	{
		void* pImgBuff = const_cast<void*>(pFrame->pImgBuf);
		pDlg->SaveRawImage(pImgBuff);
	}

	// 回调函数显示方法
	pDlg->m_pBitmap->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   显示图像
\param   pImageBuf   图像Buffer
\param   nWidth      图像宽
\param   nHeight     图像高
\param   pBmpInfo    BITMAPINFO结构指针 
\param   pWnd        显示图像窗口

\return  void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::DrawImage(BYTE *pImageBuf, int nWidth, int nHeight, BITMAPINFO *pBmpInfo)
{
	if ((pImageBuf == NULL) || (pBmpInfo == NULL))
	{
		return;
	}

	// 显示图像的窗口大小                                      
	int nWndWidth  = m_objRect.right - m_objRect.left;      // 图像显示窗口宽度
	int nWndHeight = m_objRect.bottom - m_objRect.top;      // 图像显示窗口高度

	// 必须调用该语句，否则图像出现水纹
	::SetStretchBltMode(m_hDC, COLORONCOLOR);
	::StretchDIBits(m_hDC,
		0,						
		0,
		nWndWidth,
		nWndHeight,
		0,
		0,
		nWidth,
		nHeight,
		pImageBuf,
		pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);	
}

//---------------------------------------------------------------------------------
/**
\brief   保存Raw图像
\param   pRawBuf      Raw图像
\param   nImageSize   图像大小
\param   srtSavePath  图像存储路径

\return  void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::SaveRawImage(void *pRawBuf)
{
	if ((pRawBuf == NULL) )
	{
		return;
	}

	CString strFileName = "";               // 图像存储路径
	SYSTEMTIME  sysTime;                    // 获取当前时间
	GetLocalTime(&sysTime);

	//创建保存图像的文件夹
	BOOL bRet = ::CreateDirectory(m_strSavePath, NULL);
	if (bRet)
	{
		::SetFileAttributes(m_strSavePath, FILE_ATTRIBUTE_NORMAL);
	}

	// 获取当前时间为图像保存的默认名称
	strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.raw",
		m_strSavePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);


	char *pFilePath = strFileName.GetBuffer(strFileName.GetLength());
	m_pBitmap->SaveRaw(pRawBuf, pFilePath);	
}

//----------------------------------------------------------------------------------
/**
\brief  获取错误信息描述
\param  emErrorStatus  错误码

 \return void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::ShowErrorString(GX_STATUS emErrorStatus)
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
void CGxSnapRaw16Dlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief   回车键和Esc键的消息响应函数
\param   pMsg  消息结构体
  
\return 成功:TRUE   失败:FALSE
*/
//----------------------------------------------------------------------------------
BOOL CGxSnapRaw16Dlg::PreTranslateMessage(MSG* pMsg) 
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
