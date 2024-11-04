// GxImageProcessDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "GxImageProcess.h"
#include "GxImageProcessDlg.h"
#include "string"


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
// CGxImageProcessDlg dialog

CGxImageProcessDlg::CGxImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxImageProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxImageProcessDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hDevice             = NULL;            // 设备句柄
	m_hStream			  = NULL;			 // 设备流句柄
	m_pBufferRaw          = NULL;            // 原始图像数据
	m_nPixelColorFilter   = 0;               // Bayer格式
	m_bColorFilter        = false;           // 标识是否支持Bayer格式
	m_bAwbLampHouse       = false;           // 标示是否支持光源选择
	m_bColorCorrection    = false;           // 标示是否支持颜色校正
	m_bIsOpenCC           = false;           // 标识颜色校正复选框是否被选中
	m_bIsOpen             = false;           // 相机打开标志
	m_bIsSnap             = false;           // 相机采集标志
	m_nGetColorCorrection = 0;               // 获取的颜色校正值
	m_nColorCorrection    = 0;               // 颜色校正值
	m_nContrast           = 0;               // 对比度值
	m_dGamma              = 0;               // The value of Gamma 
	m_nLightness          = 0;               // 亮度值
	m_nSaturation         = 0;               // 饱和度值
    m_nPreColorCorrection = 0;               // 颜色更新前的值
	m_nPreSaturation      = 0;               // 饱和度更新前的值
	m_nPreContrast        = 0;               // 对比度更新前的值
	m_nPreLightness       = 0;               // 亮度更新前的值
	m_dPreGamma           = 0;               // Gammma更新前的值
	m_fSharpen            = 0;               // 锐化值
	m_bUpData             = true;            // 更新CPU当前显示
	m_hWndHandle          = NULL;            // 显示图像的窗口句柄 
	m_nAutoWhiteBalance   = 0;               // 自动白平衡的当前值
    m_bEnableGamma        = false;           // 标识设备是否开启Gamma
    m_bEnableSharpness    = false;           // 标识设备是否开启锐化
	m_i64PixelFormat      = 0;               // 当前像素格式
	m_pGammaLut           = NULL;            // Gamma查找表
	m_pContrastLut        = NULL;            // 对比度查找表
	m_nLutLength          = 0;               // 查找表长度

	m_bAccelerate = false;
	m_bColorCorrection = false;
	m_bDefectivePixelCorrect = false;
	m_bSharpness = false;
	m_bDenoise = false;
	m_bImageProcess = false;


	m_pBaseinfo = NULL;

	// 彩色图像处理功能设置结构体中指针初始化
	m_objImageProcess.parrCC  = NULL;
	m_objImageProcess.pProLut = NULL;
	
	// 黑白图像处理功能设置结构体中指针初始化
	m_objMonoImgProc.pProLut = NULL;

	m_pBitmap = NULL;
}

void CGxImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxImageProcessDlg)
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContrast);
	DDX_Control(pDX, IDC_SLIDER_LIGHTNESS, m_sliderLightness);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_sliderSaturation);
	DDX_Control(pDX, IDC_SLIDER_SHARPEN, m_sliderSharpen);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ChooseDevice, m_comboChooseDevice);
}

BEGIN_MESSAGE_MAP(CGxImageProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CGxImageProcessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnSelchangeComboAwb)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB_LIGHT, OnSelchangeComboAwbLight)
	ON_CBN_SELCHANGE(IDC_COMBO_PIXEL_FORMAT, OnSelchangeComboPixelFormat)
	
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OPEN_BAD_PIXEL_CORRECT, OnOpenBadPixelCorrect)
	ON_BN_CLICKED(IDC_OPEN_DENOISE, OnOpenDenoise)
	ON_BN_CLICKED(IDC_OPEN_ACCELERATE, OnOpenAccelerate)
	ON_BN_CLICKED(IDC_OPEN_SHARPEN, OnOpenSharpen)
	ON_BN_CLICKED(IDC_OPEN_COLOR_CORRECT, OnOpenColorCorrect)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxImageProcessDlg message handlers

BOOL CGxImageProcessDlg::OnInitDialog()
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
	
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 初始化库
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	// 获取画图窗口指针和句柄
	GetDlgItem(IDC_STATIC_SHOW_FRAME, &m_hWndHandle);

	// 初始下拉列表控件
	__InitCombo();
	
	// 更新UI界面
	UpdateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//----------------------------------------------------------------
/**
\brief  初始下拉列表控件
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::__InitCombo()
{
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum = 0;

	// 枚举设备个数
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDeviceNum <= 0)
	{
		MessageBox("没有枚举到设备，请插入相机并重启程序！");
		m_bEnableOpen = false;
		return;
	}
	else
	{
		// 获取所有设备的基础信息
		m_pBaseinfo = new(std::nothrow) GX_DEVICE_BASE_INFO[nDeviceNum];
		if(NULL == m_pBaseinfo)
		{
			MessageBox("分配缓冲区失败！");
			return;
		}
		
		size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
		emStatus = GXGetAllDeviceBaseInfo(m_pBaseinfo, &nSize);
		GX_VERIFY(emStatus);
		

		GX_DEVICE_BASE_INFO *pTmp = m_pBaseinfo;
		for (unsigned int i = 0; i < nDeviceNum; i++)
		{
			m_comboChooseDevice.AddString(pTmp->szDisplayName);
			m_vecDevInfo.push_back(*pTmp);
			pTmp++;
		}

		// 默认下拉列表选择第一台相机
		m_comboChooseDevice.SetCurSel(0);
		m_bEnableOpen = true;
	}
}

void CGxImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxImageProcessDlg::OnPaint() 
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
HCURSOR CGxImageProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGxImageProcessDlg::OnClose() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 未停采则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// 注销回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		// 释放图像显示资源
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}
	
	if (m_bIsOpen)
	{
		// 关闭相机
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;

		// 释放图像处理功能申请的资源
	    UnInitImageProcessStruct();

		if (NULL != m_pBitmap)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
	}

	// 关闭设备库
	emStatus = GXCloseLib();

	if (NULL != m_pBaseinfo)
	{
		delete []m_pBaseinfo;
		m_pBaseinfo = NULL;
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

	CDialog::OnClose();
}


//----------------------------------------------------------------
/**
\brief  打开设备响应函数
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::OnOpenDevice() 
{
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	bool           bReturn    = false;
	GX_OPEN_PARAM  stOpenParam;
	bool	       bImplementPacketSize = false;
	uint32_t       unPacketSize		  = 0;
	uint32_t	  nDSNum	  = 0;

	// 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// 打开选择设备下拉框选中的设备
	int nSel = m_comboChooseDevice.GetCurSel();
	if (nSel < 0)
	{
		MessageBox("请插入相机！");
		return;
	}

	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_SN;
	stOpenParam.pszContent = m_vecDevInfo[nSel].szSN;

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
	
	// 判断设备是否支持流通道数据包功能
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
	if (GX_STATUS_SUCCESS != emStatus)
	{
		GXCloseDevice(m_hDevice);
		ShowErrorString(emStatus); 
		return ;
	}

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
		if (GX_STATUS_SUCCESS != emStatus)
	    {
		    GXCloseDevice(m_hDevice);
		    ShowErrorString(emStatus); 
		    return ;
	    }

		// 将最优包长值设置为当前设备的流通道包长值
		emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
		if (GX_STATUS_SUCCESS != emStatus)
	    {
		    GXCloseDevice(m_hDevice);
		    ShowErrorString(emStatus); 
		    return ;
	    }
	}

	// 锐化、算法加速、坏点校正、图像降噪默认不开启
	m_bSharpness = false;
	m_bAccelerate = false;
	m_bDefectivePixelCorrect = false;
	m_bDenoise = false;

	// 初始化参数
	emStatus = InitParam();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		GXCloseDevice(m_hDevice);
		ShowErrorString(emStatus); 
		return ;
	}
	
	// 初始化图像处理功能结构体
	bReturn = InitImageProcessStruct();
	if (!bReturn)
	{
		GXCloseDevice(m_hDevice);
		MessageBox("初始化图像处理功能结构体失败！");
		return;
	}

	//初始化图像显示模块
	m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, &m_hWndHandle);
	if (NULL == m_pBitmap)
	{
		ShowErrorString(GX_STATUS_ERROR);
		return;
	}

	// 初始化UI界面控件
	InitUI();
	m_bIsOpen = true;	
	m_bEnableOpen = false;

	//设置定时器更新界面
	SetTimer(0, 1000, NULL);
	
	// 更新UI界面
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::InitParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE stIntValue;
	GX_ENUM_VALUE stEnumValue;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

	// 采集模式设置为连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// 获取图像大小
	uint32_t   ui32PayLoadSize = 0;     // 原始Raw图像大小
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	VERIFY_STATUS_RET(emStatus);
	m_nPayLoadSize = ui32PayLoadSize;
	// 获取图像宽度
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_nImageWidth = stIntValue.nCurValue;
	
	// 获取图像高度
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_nImageHeight = stIntValue.nCurValue;


	// 获取输出像素格式
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// 获取当前像素格式
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}

	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	// 获取图像Bayer格式
	if (m_bColorFilter)
	{
		emStatus = GXGetEnumValue(m_hDevice, "PixelColorFilter", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelColorFilter = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		// 获取设备支持的像素格式的枚举项个数
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelColorFilter = stEnumValue.stCurValue.nCurValue;
	}
	
	// 判断相机是否支持白平衡光源选择
	emStatus = GXGetNodeAccessMode(m_hDevice, "AWBLampHouse", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		m_bAwbLampHouse = true;
	}
	
	// 获取图像参数
	emStatus = GetImageParam();
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   获取图像参数

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::GetImageParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bIsImplemented = false;
	GX_FLOAT_VALUE stFloatValue;

	m_nPreLightness  = m_nLightness  = DEFAULT_LIGHTNESS;          // 亮度值
	m_nPreSaturation = m_nSaturation = DEFAULT_SATURATION;         // 饱和度值
	m_fSharpen       = DEFAULT_SHARPEN;                            // 锐化值
	m_nContrast      = DEFAULT_CONTRAST;                           // 对比度


	// 判断相机是否开启Gamma使能
	emStatus = __IsEnableDeviceGamma();
	VERIFY_STATUS_RET(emStatus);
	
	// 判断相机是否支持Gamma获取
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "GammaParam", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		bIsImplemented = true;
	}
	
	if (bIsImplemented)
	{
		// 获取Gamma参数
		emStatus = GXGetFloatValue(m_hDevice, "GammaParam", &stFloatValue);
		VERIFY_STATUS_RET(emStatus);
		m_dGamma = stFloatValue.dCurValue;
	}
	else
	{
		m_dGamma = DEFAULT_GAMMA;
	}
	m_dPreGamma = m_dGamma;
	
	
	// 判断相机是否开启锐化设置
	emStatus = __IsEnableDeviceSharpness();
	VERIFY_STATUS_RET(emStatus);

	if (m_bColorFilter)
	{
		emStatus = __InitColorParam();
		VERIFY_STATUS_RET(emStatus);
	} 
	else
	{
		// 如果是黑白相机
		m_bColorCorrection = false;
		m_nGetColorCorrection = DEFAULT_COLOR_CORRECTION;
		m_nPreColorCorrection = DEFAULT_COLOR_CORRECTION;
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化彩色相机部分参数

\return  初始化结果状态值
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__InitColorParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE stIntValue;

	// 判断相机是否开启颜色校正功能
	emStatus = __IsEnableDeviceColorCorrect();
	VERIFY_STATUS_RET(emStatus);


	// 判断相机是否支持颜色校正参数获取
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "ColorCorrectionParam", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bColorCorrection = true;
	}
	else
	{
	    m_bColorCorrection = false;
	}
	
	if (m_bColorCorrection)
	{
		// 获取颜色校正参数
		emStatus = GXGetIntValue(m_hDevice, "ColorCorrectionParam", &stIntValue);
		VERIFY_STATUS_RET(emStatus);
		m_nGetColorCorrection = stIntValue.nCurValue;
	}
	else
	{
		m_nGetColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	// 是否勾选了颜色校正
	if (m_bIsOpenCC)
	{
		m_nPreColorCorrection = m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nPreColorCorrection = m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	return GX_STATUS_SUCCESS;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化彩色图像处理结构体

\return  true:设置成功，false:设置失败
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::__InitColorStruct()
{
	VxInt32 nStatus = DX_OK;

	do 
	{
		// 计算图像处理色彩调节数组
		m_objImageProcess.nCCBufLength = CALCCCPARAM_LENGTH;
		m_objImageProcess.parrCC = new(std::nothrow) VxInt16[CALCCCPARAM_LENGTH];
		if (m_objImageProcess.parrCC == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}
		nStatus = DxCalcCCParam(m_nColorCorrection, m_nSaturation, m_objImageProcess.parrCC, CALCCCPARAM_LENGTH);
		if (nStatus != DX_OK)
		{
			break;
		}

		// 计算图像处理Lut查找表
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, NULL, &m_objImageProcess.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}
		m_objImageProcess.pProLut = new(std::nothrow) VxUint8[m_objImageProcess.nLutLength];
		if (m_objImageProcess.pProLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objImageProcess.pProLut, &m_objImageProcess.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

	} while (0);

	// 图像处理查找表初始化失败，释放资源
	if (nStatus != DX_OK)
	{
		if (m_objImageProcess.parrCC != NULL)
		{
			delete[]m_objImageProcess.parrCC;
			m_objImageProcess.parrCC = NULL;
		}
		if (m_objImageProcess.pProLut != NULL)
		{
			delete[]m_objImageProcess.pProLut;
			m_objImageProcess.pProLut = NULL;
		}
		return false;
	}

	// 彩色图像处理功能设置结构体其他参数初始化
	m_objImageProcess.bAccelerate            = m_bAccelerate;
	m_objImageProcess.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objImageProcess.bDenoise               = m_bDenoise;
	m_objImageProcess.bFlip                  = true;
	m_objImageProcess.bSharpness             = m_bSharpness;
	m_objImageProcess.fSharpFactor           = m_fSharpen;
	m_objImageProcess.cvType                 = RAW2RGB_NEIGHBOUR;
	m_objImageProcess.emLayOut               = (DX_PIXEL_COLOR_FILTER)m_nPixelColorFilter;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化黑白图像处理结构体

\return  true:设置成功，false:设置失败
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::__InitMonoStruct()
{
	VxInt32 nStatus = DX_OK;

	do 
	{
		// 计算图像处理Lut查找表
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, NULL, &m_objMonoImgProc.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}
		m_objMonoImgProc.pProLut = new(std::nothrow) VxUint8[m_objMonoImgProc.nLutLength];
		if(m_objMonoImgProc.pProLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objMonoImgProc.pProLut, &m_objMonoImgProc.nLutLength);
	} while (0);

	// 图像处理查找表初始化失败，释放资源
	if (nStatus != DX_OK)
	{
		if (m_objMonoImgProc.pProLut != NULL)
		{
			delete[]m_objMonoImgProc.pProLut;
			m_objMonoImgProc.pProLut = NULL;
		}
		return false;
	}

	// 黑白图像处理功能设置结构体其他参数初始化
	m_objMonoImgProc.bAccelerate            = m_bAccelerate;
	m_objMonoImgProc.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objMonoImgProc.bSharpness             = m_bSharpness;
	m_objMonoImgProc.fSharpFactor           = m_fSharpen;
	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化图像处理结构体

\return  true:设置成功，false:设置失败
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::InitImageProcessStruct()
{
	if (m_bColorFilter)
	{
		return __InitColorStruct();
	} 
	else
	{
		return __InitMonoStruct();
	}
}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::InitUI()
{
	char chPutText[10] = {0};
	
	// 初始化白平衡Combox控件
    InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);
	
	// 初始化白平衡光照环境Combox控件
	InitEnumUI("AWBLampHouse", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_LIGHT), m_bAwbLampHouse);

	// 初始化像素格式Combox控件
	InitEnumUI("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT), true);
	
	// 初始化滑动条的范围
	m_sliderSharpen.SetRange(1, 50, TRUE);
	m_sliderGamma.SetRange(1, 100, TRUE);
	m_sliderContrast.SetRange(-50, 100, TRUE);
	m_sliderLightness.SetRange(-150, 150, TRUE);
	m_sliderSaturation.SetRange(0, 128, TRUE);
	
	// 初始化设滑动条的位置
	m_sliderSharpen.SetPos(int(m_fSharpen * 10));
	m_sliderGamma.SetPos(int(m_dGamma * 10));
	m_sliderContrast.SetPos(int(m_nContrast));	
	m_sliderLightness.SetPos(m_nLightness);
	m_sliderSaturation.SetPos(m_nSaturation);

	// 初始化edit框值
	_snprintf(chPutText, sizeof(chPutText), "%f", m_fSharpen);
	GetDlgItem(IDC_EDIT_SHARPEN)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%f", m_dGamma);
	GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%lld", m_nContrast);
	GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%d", m_nLightness);
	GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%d",m_nSaturation);
	GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(chPutText);
	
	// 初始化新check框
	if (m_bColorCorrection)
	{
		((CButton*)GetDlgItem(IDC_OPEN_COLOR_CORRECT))->SetCheck(m_bIsOpenCC);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_OPEN_COLOR_CORRECT))->SetCheck(false);
	}

	// 初始化复选框
	((CButton*)GetDlgItem(IDC_OPEN_DENOISE))->SetCheck(m_bDenoise);
	((CButton*)GetDlgItem(IDC_OPEN_SHARPEN))->SetCheck(m_bSharpness);
	((CButton*)GetDlgItem(IDC_OPEN_ACCELERATE))->SetCheck(m_bAccelerate);
	((CButton*)GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT))->SetCheck(m_bDefectivePixelCorrect);

	
}

//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   chFeature   [in]    功能码ID
\param   pComboBox     [in]    控件指针
\param   bIsImplement  [in]    设备是否支持chFeature代表功能

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement)
{
	// 判断控件
	if ((pComboBox == NULL) || (!bIsImplement))
	{
		return;
	}

	GX_STATUS emStatus    = GX_STATUS_ERROR;
	GX_ENUM_VALUE stEnumValue;
	uint32_t  nEntryNum   = 0;
	int       nCursel     = 0;
	
	// 获取功能的枚举数
	emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
	GX_VERIFY(emStatus);
	
	// 为获取枚举型的功能描述分配空间
	nEntryNum = stEnumValue.nSupportedNum;

	std::string strFrature = chFeature;

	// 如果是自动白平衡则更新自动白平衡的值
	if (strFrature.compare("BalanceWhiteAuto"))
	{
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;
	}

	// 初始化当前控件的可选项
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCursel = i;
		}
	}
	
	// 界面显示当前值
	pComboBox->SetCurSel(nCursel);
}

void CGxImageProcessDlg::OnStartSnap() 
{
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	emStatus = m_pBitmap->PrepareForShowImg();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		ShowErrorString(emStatus);
		return;
	}

	// 注册回调函数
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);

	if (GX_STATUS_SUCCESS != emStatus)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// 发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (GX_STATUS_SUCCESS != emStatus)
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
\brief   获取图像质量提升参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::GetImageImproveParam()
{
	if (!m_bColorFilter)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	VxInt32   nStatus = DX_OK;
	bool      bIsImplemented = false;

	// 判断相机是否支持对比度获取
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "ContrastParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// 获取对比度参数
		m_nContrast = m_nContrast;
	}
	else
	{
		m_nContrast = DEFAULT_CONTRAST;
	}

	// 判断相机是否支持Gamma获取
	emStatus = GXGetNodeAccessMode(m_hDevice, "GammaParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// 获取Gamma参数
		m_dGamma = m_dGamma;
	}
	else
	{
		m_dGamma = DEFAULT_GAMMA;
	}

	// 判断相机是否支持颜色校正参数获取
	emStatus = GXGetNodeAccessMode(m_hDevice, "ColorCorrectionParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// 获取颜色校正参数
		m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	do
	{
		// 获取Gamma查找表长度
		nStatus = DxGetGammatLut(m_dGamma, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		if (NULL != m_pGammaLut) 
		{
			delete[] m_pGammaLut;
			m_pGammaLut = NULL;
		}
		if (NULL == m_pGammaLut) 
		{
			m_pGammaLut = new(std::nothrow) BYTE[m_nLutLength];
		}

		if (m_pGammaLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// 计算Gamma查找表
		nStatus = DxGetGammatLut(m_dGamma, m_pGammaLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		// 获取对比度查找表长度
		nStatus = DxGetContrastLut((int)m_nContrast, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		if (NULL != m_pContrastLut) 
		{
			delete[] m_pContrastLut;
			m_pContrastLut = NULL;
		}
		if (NULL == m_pContrastLut) 
		{
			m_pContrastLut = new(std::nothrow) BYTE[m_nLutLength];
		}

		if (m_pContrastLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// 计算对比度查找表
		nStatus = DxGetContrastLut((int)m_nContrast, m_pContrastLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

	} while (0);

	// 设置查找表失败，释放资源
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
		MessageBox("获取图像处理查找表参数失败！");
		return;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   释放图像质量提升参数占用的内存

\return  无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::ReleaseImageImproveParam()
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
}


//---------------------------------------------------------------------------------
/**
\brief   回调函数中彩色图像算法处理
\param   pDlg   回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnColorImageProcess()
{
	VxInt32  emStatus = DX_OK;

	m_objImageProcess.bAccelerate = m_bAccelerate;
	m_objImageProcess.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objImageProcess.bSharpness = m_bSharpness;
	m_objImageProcess.bDenoise = m_bDenoise;

	// 判断是否重新计算图像处理色彩调节数组
	if ((m_nPreSaturation != m_nSaturation) || (m_nColorCorrection != m_nPreColorCorrection))
	{
		DxCalcCCParam(m_nColorCorrection, m_nSaturation, m_objImageProcess.parrCC, CALCCCPARAM_LENGTH);
		m_nPreSaturation      = m_nSaturation;
		m_nPreColorCorrection = m_nColorCorrection;
	}

	// 判断是否需要重新计算图像处理8位查找表
	if ((m_nPreContrast != m_nContrast) || (m_nPreLightness != m_nLightness) 
		|| (m_dPreGamma != m_dGamma))
	{
		DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objImageProcess.pProLut, &m_objImageProcess.nLutLength);
		m_nPreContrast  = m_nContrast;
		m_nPreLightness = m_nLightness;
		m_dPreGamma     = m_dGamma;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   回调函数中黑白图像算法处理
\param   pDlg   回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnMonoImageProcess()
{
	VxInt32  emStatus = DX_OK;
	m_objMonoImgProc.bAccelerate = m_bAccelerate;
	m_objMonoImgProc.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objMonoImgProc.bSharpness = m_bSharpness;
	m_objMonoImgProc.fSharpFactor = m_fSharpen;
	

	// 判断是否需要重新计算图像处理8位查找表
	if ((m_nPreContrast != m_nContrast) || (m_nPreLightness != m_nLightness) || (m_dPreGamma != m_dGamma))
	{
		DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objMonoImgProc.pProLut, &m_objMonoImgProc.nLutLength);
		m_nPreContrast  = m_nContrast;
		m_nPreLightness = m_nLightness;
		m_dPreGamma     = m_dGamma;
	}
}


//---------------------------------------------------------------------------------
/**
\brief   回调函数
\param   pFrame   回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall CGxImageProcessDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxImageProcessDlg *pDlg = (CGxImageProcessDlg*)(pFrame->pUserParam);
	
	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		return;
	}
	
	//标识程序正在进行图像处理
	pDlg->m_bImageProcess = true;

	if ((GX_PIXEL_FORMAT_RGB8 == pFrame->nPixelFormat)
		|| (GX_PIXEL_FORMAT_BGR8 == pFrame->nPixelFormat)) 
	{
		pDlg->GetImageImproveParam();
		pDlg->m_pBitmap->ShowAfterImprovment(pFrame,pDlg->m_nColorCorrection, pDlg->m_pContrastLut, pDlg->m_pGammaLut);
	}
	else 
	{
		pDlg->m_pBufferRaw = const_cast<void*>(pFrame->pImgBuf);
		// 回调函数中不推荐进行图像处理，此处仅为演示图像处理接口调用
		bool bIsColor = pDlg->m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(pDlg->m_i64PixelFormat));
		if (bIsColor)
		{
			pDlg->OnColorImageProcess();
			pDlg->m_pBitmap->ShowAfterProcess(pFrame, &pDlg->m_objImageProcess);
		}
		else
		{
			pDlg->OnMonoImageProcess();
			pDlg->m_pBitmap->ShowAfterProcess(pFrame, &pDlg->m_objMonoImgProc);
		}

	}
	//标识程序没有进行图像处理
	pDlg->m_bImageProcess = false;
}

void CGxImageProcessDlg::OnStopSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// 注销回调函数
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat) || (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
	{
		ReleaseImageImproveParam();
	}

	m_bIsSnap = false;
	
	// 更新UI界面
	UpdateUI();	
}

void CGxImageProcessDlg::OnCloseDevice() 
{
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

		if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat) || (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
		{
			ReleaseImageImproveParam();
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

	m_pBitmap->UnPrepareForShowImg();
	// 释放图像显示资源
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// 释放图像处理功能申请的资源
	UnInitImageProcessStruct();

	m_bIsOpen = false;
	m_bEnableOpen = true;

	// 停止Timer
	KillTimer(0);
	
	// 更新UI界面
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief    释放图像处理功能申请的资源

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::UnInitImageProcessStruct()
{
	if (m_objImageProcess.parrCC != NULL)
	{
		delete[]m_objImageProcess.parrCC;
		m_objImageProcess.parrCC = NULL;
	}
	if (m_objImageProcess.pProLut != NULL)
	{
		delete[]m_objImageProcess.pProLut;
		m_objImageProcess.pProLut = NULL;
	}
	if (m_objMonoImgProc.pProLut != NULL)
	{
		delete[]m_objMonoImgProc.pProLut;
		m_objMonoImgProc.pProLut = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  获取GxIAPI错误描述,并弹出错误提示对话框
\param  emErrorStatus  [in]   错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// 获取错误描述信息长度
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// 获取错误信息描述
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	
	// 显示错误描述信息
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败!");
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
void CGxImageProcessDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(m_bEnableOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bIsOpen && m_bColorFilter);
	GetDlgItem(IDC_COMBO_AWB_LIGHT)->EnableWindow(m_bIsOpen && m_bAwbLampHouse && m_bColorFilter);
	GetDlgItem(IDC_COMBO_ChooseDevice)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_COMBO_PIXEL_FORMAT)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_OPEN_COLOR_CORRECT)->EnableWindow(m_bIsOpen && m_bColorCorrection && m_bColorFilter);

	GetDlgItem(IDC_SLIDER_CONTRAST)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(m_bIsOpen && !m_bEnableGamma);
	GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(m_bIsOpen && !m_bEnableGamma);

	if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
	{
		GetDlgItem(IDC_OPEN_ACCELERATE)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_DENOISE)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_LIGHTNESS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(false);
	}
	else 
	{
		GetDlgItem(IDC_OPEN_ACCELERATE)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_OPEN_DENOISE)->EnableWindow(m_bIsOpen && m_bColorFilter);
		GetDlgItem(IDC_OPEN_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness);
		GetDlgItem(IDC_SLIDER_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness && m_bSharpness);
		GetDlgItem(IDC_EDIT_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness && m_bSharpness);
		GetDlgItem(IDC_SLIDER_LIGHTNESS)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(m_bIsOpen && m_bColorFilter);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(m_bIsOpen && m_bColorFilter);
	}
}

void CGxImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char PutText[10] = {0};
	int  nValue      = 0;

	if (pScrollBar == (CScrollBar*)&m_sliderLightness)            // 亮度滑动条
	{
		m_nLightness = m_sliderLightness.GetPos();
		_snprintf(PutText, sizeof(PutText), "%d", m_nLightness);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(PutText);	
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderContrast)         // 对比度滑动条
	{
		m_nContrast = m_sliderContrast.GetPos();
		_snprintf(PutText, sizeof(PutText), "%lld", m_nContrast);
		GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(PutText);	
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderSaturation)       // 饱和度滑动条
	{
		m_nSaturation = m_sliderSaturation.GetPos();
		_snprintf(PutText, sizeof(PutText), "%d", m_nSaturation);
		GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(PutText);	
	}	
	else if (pScrollBar == (CScrollBar*)&m_sliderSharpen)          // 锐化滑动条
	{
		nValue = m_sliderSharpen.GetPos();
		m_fSharpen = nValue / 10.0F;
		m_objImageProcess.fSharpFactor = m_fSharpen;
		_snprintf(PutText, sizeof(PutText), "%7f", m_fSharpen);
		GetDlgItem(IDC_EDIT_SHARPEN)->SetWindowText(PutText);		
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderGamma)            // Gamma滑动条
	{
		nValue = m_sliderGamma.GetPos();
		m_dGamma = nValue / 10.0;
		_snprintf(PutText, sizeof(PutText), "%7f", m_dGamma);
		GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(PutText);		
	}
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGxImageProcessDlg::OnOpenBadPixelCorrect() 
{
	m_bDefectivePixelCorrect = !m_bDefectivePixelCorrect;
}

void CGxImageProcessDlg::OnOpenDenoise() 
{
	m_bDenoise = !m_bDenoise;
}

void CGxImageProcessDlg::OnOpenAccelerate() 
{
	m_bAccelerate = !m_bAccelerate;
}

void CGxImageProcessDlg::OnOpenSharpen() 
{
	m_bSharpness = !m_bSharpness;
	UpdateUI();
	
}

void CGxImageProcessDlg::OnOpenColorCorrect() 
{
	m_bIsOpenCC = !m_bIsOpenCC;
	if (m_bIsOpenCC)
	{
		m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}	
}

void CGxImageProcessDlg::OnSelchangeComboAwb() 
{
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nEnumVal = 0;
	int       nIndex   = 0;
	
	nIndex   = pBox->GetCurSel();                    // 获取当前选项
	nEnumVal = (int64_t)pBox->GetItemData(nIndex);   // 获取当前选项对应的枚举型值
	
	// 设置白平衡
	emStatus = GXSetEnumValue(m_hDevice, "BalanceWhiteAuto", nEnumVal);
	GX_VERIFY(emStatus);

	m_nAutoWhiteBalance = nEnumVal;
}

void CGxImageProcessDlg::OnSelchangeComboAwbLight() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB_LIGHT);
	
	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值
	
	// 设置白平衡光源
	emStatus = GXSetEnumValue(m_hDevice, "AWBLampHouse", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief   改变像素格式的响应函数

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnSelchangeComboPixelFormat()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT);

	int      nIndex = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 设置白平衡光源
	emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", nEnumVal);
	GX_VERIFY(emStatus);
	m_i64PixelFormat = nEnumVal;
	UpdateUI();
}


BOOL CGxImageProcessDlg::PreTranslateMessage(MSG* pMsg) 
{
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

//----------------------------------------------------------------
/**
\brief  定时器函数:定时读取自动白平衡的枚举值实现设置Once成功后界面自动更新为off
\param  定时器事件ID

\return 无
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	GX_STATUS status = GX_STATUS_ERROR;
	GX_ENUM_VALUE stEnumValue;

	//如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
	//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		int64_t nWBEnumVal = 0;   //< 保存白平衡的枚举值

		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);

		//获取自动白平衡枚举值
		status = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (status != GX_STATUS_SUCCESS)
		{
			return;
		}
		
		nWBEnumVal = stEnumValue.stCurValue.nCurValue;
		m_nAutoWhiteBalance = nWBEnumVal;

		//判断设备自动白平衡是否变为OFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// 选中自动曝光控件中的OFF项,即由ONCE变为OFF
					pAutoWBBox->SetCurSel(i);
					break;
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------
/**
\brief  获取设备是否开启颜色校正

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceColorCorrect()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;

    do 
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "ColorTransformationEnable", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetBoolValue(m_hDevice, "ColorTransformationEnable", &m_bColorCorrection);
        VERIFY_STATUS_BREAK(emStatus);
    } while (0);
    
    return emStatus;
}

//----------------------------------------------------------------
/**
\brief  获取设备是否开启Gamma使能

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceGamma()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;
    m_bEnableGamma = false;

    do 
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GammaEnable", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetBoolValue(m_hDevice, "GammaEnable", &m_bEnableGamma);
        VERIFY_STATUS_BREAK(emStatus);
    } while (0);

    return emStatus;
}

//----------------------------------------------------------------
/**
\brief  获取设备是否开启锐化

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceSharpness()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;
    m_bEnableSharpness = false;
	GX_ENUM_VALUE stEnumValue;

    do
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "SharpnessMode", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetEnumValue(m_hDevice, "SharpnessMode", &stEnumValue);
        VERIFY_STATUS_BREAK(emStatus);

        if(GX_SHARPNESS_MODE_OFF == stEnumValue.stCurValue.nCurValue)
        {
            m_bEnableSharpness = false;
        }else if(GX_SHARPNESS_MODE_ON)
        {
            m_bEnableSharpness = true;
        }
    } while (0);

    return emStatus;
}


