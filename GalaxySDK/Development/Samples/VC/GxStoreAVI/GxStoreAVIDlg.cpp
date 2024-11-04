// GxStoreAVIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxStoreAVI.h"
#include <Windows.h>
#include "GxStoreAVIDlg.h"

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
// CGxStoreAVIDlg dialog

CGxStoreAVIDlg::CGxStoreAVIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxStoreAVIDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bEnableSaveAVI(FALSE)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_wPlayFPS(15)
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
	, m_bSelectCompressed(FALSE)
	, m_hConvert(NULL)
{
	//{{AFX_DATA_INIT(CGxStoreAVIDlg)
	m_strAVIFolder = _T("");
	m_bCompressedAVI = FALSE;
	m_bShowImage = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxStoreAVIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxStoreAVIDlg)
	DDX_Text(pDX, IDC_EDIT_SAVEAS, m_strAVIFolder);
	DDX_Check(pDX, IDC_CHECK_COMPRESSED_AVI, m_bCompressedAVI);
	DDX_Check(pDX, IDC_CHECK_SHOW_IMAGE, m_bShowImage);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_FPS, m_wPlayFPS);
}

BEGIN_MESSAGE_MAP(CGxStoreAVIDlg, CDialog)
	//{{AFX_MSG_MAP(CGxStoreAVIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS, OnBnClickedBtnSaveAs)
	ON_BN_CLICKED(IDC_BTN_AVI_START, OnBnClickedBtnAviStart)
	ON_BN_CLICKED(IDC_BTN_STOP_AVI, OnBnClickedBtnStopAvi)
	ON_BN_CLICKED(IDC_CHECK_COMPRESSED_AVI, OnBnClickedCheckCompressedAvi)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IMAGE, OnBnClickedCheckShowImage)
	ON_BN_CLICKED(IDC_BTN_SELECT_COMPRESSOR, OnBnClickedBtnSelectCompressor)
	ON_EN_KILLFOCUS(IDC_EDIT_FPS, OnEnKillfocusEditFps)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxStoreAVIDlg message handlers

BOOL CGxStoreAVIDlg::OnInitDialog()
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

	// 初始化设备库
	emStatus = GXInitLib();  
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// 获取当前程序执行路径,作为图像保存的默认路径
	char    szName[MAX_PATH] = {0};
	CString  strAppFullName = ""; 
	GetModuleFileName(NULL, szName, MAX_PATH);
	strAppFullName = szName;
	int nPos = strAppFullName.ReverseFind('\\');
	m_strAVIFolder += strAppFullName.Left(nPos);
    UpdateData(FALSE);

	//如果用VC6编译出现“error C2065: 'CoInitializeEx' : undeclared identifier”
	//那么请在stdafx.h开始位置添加#define _WIN32_WINNT 0x0400
	if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != AVIERR_OK)
	{
		return FALSE;
	}

	//获取图像显示窗口的指针和绘图DC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);
	
	//刷新界面
	UpDateUI();
	 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxStoreAVIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxStoreAVIDlg::OnPaint() 
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

HCURSOR CGxStoreAVIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGxStoreAVIDlg::PreTranslateMessage(MSG* pMsg)
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
		case IDC_EDIT_FPS:
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
\brief  点击"打开设备"按钮消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nDevNum  = 0;
	uint32_t	  nDSNum   = 0;
	DX_STATUS   emDxStatus     = DX_OK;

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
	GX_OPEN_PARAM stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = true;

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

	do
	{
		// 初始化图像显示模块
		m_pBitmap = new CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		//准备显示图像的资源
		m_pBitmap->PrepareForShowImg();

		//注册回调
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
		GX_VERIFY_BREAK(emStatus);

		//设置流层Buffer处理模式
		emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
		GX_VERIFY_BREAK(emStatus);

		//发开始采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
		//释放为图像显示准备的资源 
		m_pBitmap->UnPrepareForShowImg();

		//显示错误信息
		ShowErrorString(emStatus);

	    delete m_pBitmap;
		m_pBitmap = NULL;

		return;
	}

	//创建图像转换句柄
	emDxStatus = (DX_STATUS)DxImageFormatConvertCreate(&m_hConvert);
	DX_VERIFY_RETURN(emDxStatus);

	// 更新界面控件
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数:进行图像获取和显示操作
\param   pFrame   回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall CGxStoreAVIDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxStoreAVIDlg *pDlg  = (CGxStoreAVIDlg*)(pFrame->pUserParam);

	DX_STATUS   emDxStatus     = DX_OK;
	BYTE *      pImageRGBBuf   = NULL;
	BYTE *      pRawBuffer     = NULL;
	int32_t     nSrcBufferSize = 0;              
	int32_t     nDstBufferSize = 0;

	do{
		if (pFrame->status != 0)
		{
			return;
		}

		//不显示图像则返回
		if (!pDlg->m_bShowImage)
		{
			return;
		}

		pDlg->m_pBitmap->Show(pFrame);

		//不录制视频则返回
		if(!pDlg->m_bEnableSaveAVI)
		{
			return;
		}

		//录制视频
		//BGR8和MONO8不进行像素格式转换
		if(GX_PIXEL_FORMAT_BGR8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
		{
			nDstBufferSize = pFrame->nWidth * pFrame->nHeight * PIXEL;
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}
			memcpy(pImageRGBBuf,pFrame->pImgBuf,(size_t)nDstBufferSize);

			pRawBuffer = new BYTE[nDstBufferSize];
			if (NULL == pRawBuffer)
			{
				break;
			}

			memcpy(pRawBuffer,pImageRGBBuf,(size_t)nDstBufferSize);

			// BGR格式需要翻转数据后显示
			for (int32_t i = 0; i < pFrame->nHeight; ++i)
			{
				memcpy(pImageRGBBuf + i * pFrame->nWidth * PIXEL, pRawBuffer + (pFrame->nHeight - i - 1) * pFrame->nWidth * PIXEL
					, (size_t)pFrame->nWidth * PIXEL);
			}
		}
		else if((GX_PIXEL_FORMAT_MONO8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_R8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_B8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_G8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat))
		{
			nDstBufferSize = pFrame->nWidth * pFrame->nHeight ;
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}
			memcpy(pImageRGBBuf,pFrame->pImgBuf,(size_t)nDstBufferSize);

			pRawBuffer = new BYTE[nDstBufferSize];
			if (NULL == pRawBuffer)
			{
				break;
			}

			memcpy(pRawBuffer,pImageRGBBuf,(size_t)nDstBufferSize);

			// 黑白相机需要翻转数据后显示
			for (int32_t i = 0; i < pFrame->nHeight; ++i)
			{
				memcpy(pImageRGBBuf + i * pFrame->nWidth, pRawBuffer + (pFrame->nHeight - i - 1) * pFrame->nWidth
					, (size_t)pFrame->nWidth);
			}
		}
		else
		{
			//彩色相机
			if (pDlg->m_pBitmap->IsColor((GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat))
			{
				// 设置目标像素格式为BGR8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pDlg->m_hConvert, GX_PIXEL_FORMAT_BGR8);
				DX_VERIFY(emDxStatus);

				// 获取目标Buffer大小
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
					GX_PIXEL_FORMAT_BGR8,  static_cast<VxUint32>(pFrame->nWidth), 
					static_cast<VxUint32>(pFrame->nHeight), &nDstBufferSize);
				DX_VERIFY(emDxStatus);
			}

			//黑白相机
			else
			{
				// 设置目标像素格式为Mono8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pDlg->m_hConvert, GX_PIXEL_FORMAT_MONO8);
				DX_VERIFY(emDxStatus);

				// 获取目标Buffer大小
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
					GX_PIXEL_FORMAT_MONO8, static_cast<VxUint32>(pFrame->nWidth), 
					static_cast<VxUint32>(pFrame->nHeight), &nDstBufferSize);
				DX_VERIFY(emDxStatus);
			}

			//为转换后的图像Buffer申请内存
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}

			// 设置有效位
			emDxStatus = (DX_STATUS)DxImageFormatConvertSetValidBits(pDlg->m_hConvert, 
				pDlg->m_pBitmap->GetBestValudBit((GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat));
			DX_VERIFY(emDxStatus);


			// 获取源Buffer大小
			emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
				(GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat, static_cast<VxUint32>(pFrame->nWidth), 
				static_cast<VxUint32>(pFrame->nHeight), &nSrcBufferSize);
			DX_VERIFY(emDxStatus);


			// 像素格式转换
			emDxStatus = (DX_STATUS)DxImageFormatConvert(pDlg->m_hConvert, (void*)pFrame->pImgBuf, 
				nSrcBufferSize, pImageRGBBuf, nDstBufferSize, 
				(GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat, 
				static_cast<VxUint32>(pFrame->nWidth), static_cast<VxUint32>(pFrame->nHeight), true);
			DX_VERIFY(emDxStatus);
		}

		pDlg->m_pBitmap->SaveAVI(pImageRGBBuf);	

	}while(false);

	if (NULL != pImageRGBBuf)
	{
		delete[] pImageRGBBuf;
		pImageRGBBuf = NULL;
	}

	if (NULL != pRawBuffer)
	{
		delete[] pRawBuffer;
		pRawBuffer = NULL;
	}

	return;
}

//---------------------------------------------------------------------------------
/**
\brief   相机初始化

\return  无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxStoreAVIDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice,"AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"按钮消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;
	DX_STATUS   emDxStatus     = DX_OK;

	// 若未停止录制则先停止录制
    if (m_bEnableSaveAVI)
    {
		m_bEnableSaveAVI = FALSE;
    }

	//发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	//销毁图像转换句柄
	emDxStatus = (DX_STATUS)DxImageFormatConvertDestroy(m_hConvert);
	DX_VERIFY_RETURN(emDxStatus);

	//注销回调
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}
    
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	m_pBitmap->UnPrepareForShowImg();
	if (NULL != m_pBitmap) 
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_hDevice    = NULL;
	m_bDevOpened = FALSE;

	//刷新界面
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"选择存储路径"按钮消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnSaveAs() 
{
	// TODO: Add your control notification handler code here
	char arrFilePath[MAX_PATH*2] = {0};

	BROWSEINFO br;
	br.hwndOwner = GetSafeHwnd();
	br.iImage = 0;
	br.pszDisplayName = 0;
	br.lParam = 0 ;
	br.lpfn = 0 ;
	br.lpszTitle = "请选择视频流保存路径";
	br.pidlRoot = 0 ;
	br.ulFlags = BIF_RETURNONLYFSDIRS;
	if ( SHGetPathFromIDList(SHBrowseForFolder(&br),arrFilePath) != TRUE)
	{
		return ;
	}

	m_strAVIFolder = arrFilePath; 

	UpdateData(FALSE);	
}


//----------------------------------------------------------------------------------
/**
\brief  点击"开始录制"按钮消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnAviStart() 
{
	// TODO: Add your control notification handler code here

	if (!m_bSelectCompressed)
	{
		bool bResult = m_pBitmap->InitAVI(const_cast<char*>(m_strAVIFolder.GetBuffer(m_strAVIFolder.GetLength())), m_wPlayFPS);
		if (!bResult) 
		{
			m_pBitmap->UnInitAVI();
			return;
		}
	}

	m_bEnableSaveAVI = TRUE;
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止录制"按钮消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnStopAvi() 
{
	// TODO: Add your control notification handler code here
	m_bEnableSaveAVI = FALSE;
	m_bSelectCompressed = FALSE;
	m_pBitmap->UnInitAVI();
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"显示图像"Check框消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedCheckShowImage() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"压缩存储"Check框消息响应

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedCheckCompressedAvi() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   刷新UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void  CGxStoreAVIDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_CHECK_SHOW_IMAGE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_SAVE_AS)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_CHECK_COMPRESSED_AVI)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_STOP_AVI)->EnableWindow(m_bDevOpened && m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_SELECT_COMPRESSOR)->EnableWindow(m_bDevOpened && m_bCompressedAVI && !m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_AVI_START)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_EDIT_FPS)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
}
//----------------------------------------------------------------------------------
/**
\brief  弹出错误信息
\param  emErrorStatus  [in] 错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;

	// 获取错误信息长度，并申请内存空间
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
\brief  关闭应用程序

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 若未停止录制则先停止录制
	if (m_bEnableSaveAVI)
	{
		m_bEnableSaveAVI = FALSE;
		m_pBitmap->UnInitAVI();
		m_bSelectCompressed = FALSE;
	}

	if (m_bDevOpened)
	{
		//发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		//注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		m_pBitmap->UnPrepareForShowImg();

		emStatus = GXCloseDevice(m_hDevice);
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	//释放资源库
	emStatus = GXCloseLib();

	CoUninitialize();
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"选择视频压缩器"按钮响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnSelectCompressor()
{
	//准备保存视频的资源
	bool bResult = m_pBitmap->InitAVI(const_cast<char*>(m_strAVIFolder.GetBuffer(m_strAVIFolder.GetLength())), m_wPlayFPS);
	if (!bResult) 
	{
		m_pBitmap->UnInitAVI();
		return;
	}

	bResult = m_pBitmap->SelectCompressor();
	if (!bResult) 
	{
		m_pBitmap->UnInitAVI();
		return;
	}
	m_bSelectCompressed = true;
}

//----------------------------------------------------------------------------------
/**
\brief  设置AVI视频流播放帧率失去焦点响应函数

\return void
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnEnKillfocusEditFps()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

