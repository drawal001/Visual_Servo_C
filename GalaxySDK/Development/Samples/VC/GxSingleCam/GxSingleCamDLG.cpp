// GxSingleCamColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"


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
// CGxSingleCamDlg dialog

CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bIsSnap(FALSE)
	, m_bIsSaveImg(FALSE)
	, m_bIsImproveImg(FALSE)
	, m_pGammaLut(NULL)
	, m_pContrastLut(NULL)
	, m_nLutLength(0)
	, m_nContrast(0)
	, m_nColorCorrection(0)
	, m_dGamma(0.0)
	, m_bTriggerMode(false)
	, m_bTriggerActive(false)
	, m_bTriggerSource(false)
	, m_bBalanceRatioSelector(false)
	, m_nTriggerMode(GX_TRIGGER_MODE_OFF)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_nRatioSelector(-1)
	, m_nAutoWhiteBalance(0)
	, m_strFilePath("")
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxSingleCamDlg)
	m_dBalanceRatio = 0.0;
	m_dGainValue = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 初始化参数
	memset(&m_stGainFloatRange,0,sizeof(GX_FLOAT_VALUE));
	memset(&m_stShutterFloatRange,0,sizeof(GX_FLOAT_VALUE));
	memset(&m_stFloatRange,0,sizeof(GX_FLOAT_VALUE));
}

void CGxSingleCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxSingleCamDlg)
	DDX_Text(pDX, IDC_EDIT_BALANCE_RATIO, m_dBalanceRatio);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dShutterValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	//{{AFX_MSG_MAP(CGxSingleCamDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SOFTTRIGGER, OnBnClickedBtnSofttrigger)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnCbnSelchangeComboTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnCbnSelchangeComboWriteBlance)
    ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, OnCbnSelchangeComboTriggerSource)
    ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_ACTIVE, OnCbnSelchangeComboTriggerActive)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERPOLATION_ALGORITHM, OnCbnSelchangeComboInterpolationAlgorithm)
    ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
    ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
    ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
    ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
    ON_BN_CLICKED(IDC_CHECK_SAVE_BMP, OnBnClickedCheckSaveBmp)
	ON_BN_CLICKED(IDC_CHECK_IMAGE_IMPROVE, OnBnClickedCheckImageImprove)
    ON_CBN_SELCHANGE(IDC_COMBO_RATIO_SELECTOR, OnCbnSelchangeComboRatioSelector)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, OnEnKillfocusEditBalanceRatio)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxSingleCamDlg message handlers

BOOL CGxSingleCamDlg::OnInitDialog()
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

	//初始化库
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	// 获取当前程序执行路径,设置图像保存的默认路径
	CString strDate = "";
	char    szName[MAX_PATH] = {0};
	CString strAppFullName = ""; 
	GetModuleFileName(NULL, szName, MAX_PATH);
	strAppFullName = szName;
	int nPos = strAppFullName.ReverseFind('\\');
	m_strFilePath += strAppFullName.Left(nPos);
	m_strFilePath += "\\GxSingleCamImages";

	//获取图像显示窗口的指针和绘图DC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_FRAME);


	// 更新界面控件
	UpDateUI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSingleCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxSingleCamDlg::OnPaint() 
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
HCURSOR CGxSingleCamDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 若未停采则先停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		// 注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = FALSE;
	}

	// 未关闭设备则关闭设备
	if (m_bDevOpened)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = FALSE;
		m_hDevice    = NULL;

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
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// 触发模式下拉选项Combox控件
	InitTriggerModeUI();

	// 初始化触发源Combox控件
	InitTriggerSourceUI();

	// 初始化触发极性Combox控件
	InitTriggerActivationUI();

	if (m_bColorFilter) 
	{
		// 初始化白平衡调节Combox控件
		InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);

		// 初始化白平衡通道Combox控件
		InitEnumUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR), m_bBalanceRatioSelector);

		// 初始化白平衡系数相关控件
		InitWhiteRatioUI();

		// 初始化插值算法相关控件
		InitInterpolationAlgorithmUI((CComboBox *)GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM), m_bColorFilter);
	}
	else 
	{
		//白平衡调节Combox控件、白平衡通道Combox控件、白平衡系数相关控件、图像质量提升按钮、插值算法控件不使能
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(FALSE);
	}

	// 初始化曝光时间相关控件
	InitShutterUI();

	// 初始化增益相关控件
	InitGainUI();
}

//---------------------------------------------------------------------------------
/**
\brief   初始化白平衡系数相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitWhiteRatioUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CString   strTemp  = _T("");

	if (!m_bBalanceRatioSelector)
	{
		return;
	}

	// 获取系数范围
	emStatus = GXGetFloatValue(m_hDevice,"BalanceRatio",&m_stFloatRange);
    GX_VERIFY(emStatus);
	strTemp.Format("白平衡系数(%.4f~%.4f)",m_stFloatRange.dMin,m_stFloatRange.dMax);
	SetDlgItemText(IDC_STATIC_WHITE_RATIO,strTemp);

	// 刷新显示当前白平衡系数
	RefreshWhiteRatio();
}

//---------------------------------------------------------------------------------
/**
\brief   刷新显示白平衡系数

\return  无
*/
//---------------------------------------------------------------------------------
void CGxSingleCamDlg::RefreshWhiteRatio()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_FLOAT_VALUE  stValue;
	CString   strTemp  = _T("");

	// 获取白平衡通道对应的白平衡系数
	emStatus = GXGetFloatValue(m_hDevice, "BalanceRatio", &stValue);
	GX_VERIFY(emStatus);

	m_dBalanceRatio = stValue.dCurValue;
	strTemp.Format("%.4f",stValue.dCurValue);
	SetDlgItemText(IDC_EDIT_BALANCE_RATIO, strTemp);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化触发模式Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerModeUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持触发模式
	GX_NODE_ACCESS_MODE emAccessMode;
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

//---------------------------------------------------------------------------------
/**
\brief   初始化触发源Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerSourceUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持触发源
	GX_NODE_ACCESS_MODE emAccessMode;
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

//---------------------------------------------------------------------------------
/**
\brief   初始化触发极性Combox控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitTriggerActivationUI()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 判断设备是否支持触发极性
	GX_NODE_ACCESS_MODE emAccessMode;
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
	InitEnumUI("TriggerActivation", (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE), m_bTriggerActive);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   chFeature   [in]    功能码ID
\param   pComboBox     [in]    控件指针
\param   bIsImplement  [in]    设备是否支持chFeature代表功能

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement)
{
	// 判断控件
	if ((pComboBox == NULL) || !bIsImplement)
	{
		return;
	}

	GX_ENUM_VALUE stEnumValue;
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	uint32_t  nEntryNum   = 0;
	int       nCursel     = 0;
	double    dValue      = 0;

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
			nCursel = i;
		}
	}

	// 设置当前值为界面的显示值
	pComboBox->SetCurSel(nCursel);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化插值算法UI界面
\param   pComboBox       [in]    控件指针
\param   bIsColorCamera  [in]    设备是否彩色相机

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitInterpolationAlgorithmUI(CComboBox *pComboBox, bool bIsColorCamera)
{
	// 判断控件
	if (pComboBox == NULL)
	{
		return;
	}

	// 清空控件内容
	pComboBox->ResetContent();

	// 如果是彩色相机
	if (bIsColorCamera)
	{
		int nCurSel = 0;
		pComboBox->SetItemData(pComboBox->AddString("2×2邻域插值(速度快)"), 0);
		pComboBox->SetItemData(pComboBox->AddString("5×5边缘自适应(效果好)"), 1);
		pComboBox->SetItemData(pComboBox->AddString("3×3邻域插值(适中)"), 2);
		pComboBox->SetItemData(pComboBox->AddString("5×5权重插值(效果较好)"), 3);

		// 设置当前值为界面的显示值
		pComboBox->SetCurSel(nCurSel);
	}
}

//---------------------------------------------------------------------------------
/**
\brief   回调函数
\param   pFrame         回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall CGxSingleCamDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxSingleCamDlg *pDlg = (CGxSingleCamDlg*)(pFrame->pUserParam);

	if (pFrame->status == 0)
	{
		if (pDlg->m_bIsImproveImg) 
		{
			pDlg->GetImageImproveParam();
			pDlg->m_pBitmap->ShowAfterImprovment(pFrame, pDlg->m_nColorCorrection, pDlg->m_pContrastLut, pDlg->m_pGammaLut);
		}
		else 
		{
			pDlg->m_pBitmap->Show(pFrame);
		}


		// 图像保存
		if (pDlg->m_bIsSaveImg)
		{
			pDlg->SaveImage();
		}
	}
}


//---------------------------------------------------------------------------------
/**
\brief   获取图像质量提升参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::GetImageImproveParam()
{
	if (!m_bColorFilter)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	VxInt32   nStatus = DX_OK;
	bool      bIsImplemented = false;
	GX_INT_VALUE stIntValue;
	GX_FLOAT_VALUE stFloatValue;

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
		emStatus = GXGetIntValue(m_hDevice, "ContrastParam", &stIntValue);
		GX_VERIFY(emStatus);
		m_nContrast = stIntValue.nCurValue;
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
		emStatus = GXGetFloatValue(m_hDevice, "GammaParam", &stFloatValue);
		GX_VERIFY(emStatus);
		m_dGamma = stFloatValue.dCurValue;
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
		emStatus = GXGetIntValue(m_hDevice, "ColorCorrectionParam", &stIntValue);
		GX_VERIFY(emStatus);
		m_nColorCorrection = stIntValue.nCurValue;
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

		m_pGammaLut = new BYTE[m_nLutLength];
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

		m_pContrastLut = new BYTE[m_nLutLength];
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

//----------------------------------------------------------------------------------
/**
\brief   将m_pBufferRGB图像数据保存成BMP图片

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::SaveImage() 
{
	CString strFileName = "";               // 图像存储路径
	SYSTEMTIME  sysTime;                    // 获取当前时间
	GetLocalTime(&sysTime);

	//创建保存图像的文件夹
	BOOL bRet = CreateDirectory(m_strFilePath, NULL);
	if (bRet)
	{
		SetFileAttributes(m_strFilePath, FILE_ATTRIBUTE_NORMAL);
	}

	// 获取当前时间为图像保存的默认名称
	strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.bmp",
		m_strFilePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);

	char *pFilePath = strFileName.GetBuffer(strFileName.GetLength());
	m_pBitmap->SaveBmp(pFilePath);
	m_strFilePath.ReleaseBuffer();
}

//---------------------------------------------------------------------------------
/**
\brief   点击"软触发采图"按钮响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnSofttrigger() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//发送软触发命令(在触发模式开启时有效)
	emStatus = GXSetCommandValue(m_hDevice, "TriggerSoftware");
    GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"触发模式"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool bIsWritable = TRUE;
	GX_ENUM_VALUE stEnumValue;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_MODE);

	int      nIndex    = pBox->GetCurSel();                     // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);    // 获取当前选项对应的枚举型值


	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "TriggerMode", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsWritable = true;
	}

	emStatus = GXGetEnumValue(m_hDevice, "TriggerMode", &stEnumValue);
	GX_VERIFY(emStatus);
	if (bIsWritable)
	{
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
			if (GX_TRIGGER_MODE_ON == nEnumVal)
			{
				for (int i = 0;i < pBox->GetCount();i++)
				{
					if (GX_TRIGGER_MODE_OFF == pBox->GetItemData(i))
					{
						pBox->SetCurSel(i);
						break;
					}
				}
			}
			else
			{
				for (int i = 0;i < pBox->GetCount();i++)
				{
					if (GX_TRIGGER_MODE_ON == pBox->GetItemData(i))
					{
						pBox->SetCurSel(i);
						break;
					}
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

//---------------------------------------------------------------------------------
/**
\brief   切换"自动白平衡"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS emStatus  = GX_STATUS_SUCCESS;
	int64_t   nEnumVal  = 0;
	int       nIndex    = 0;
   
	 nIndex    = pBox->GetCurSel();                    // 获取当前选项
	 nEnumVal  = (int64_t)pBox->GetItemData(nIndex);   // 获取当前选项对应的枚举型值

	//设置白平衡
	emStatus = GXSetEnumValue(m_hDevice, "BalanceWhiteAuto", nEnumVal);
    GX_VERIFY(emStatus);
    m_nAutoWhiteBalance = nEnumVal;

	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  获取GxIAPI错误描述,并弹出错误提示对话框
\param  emErrorStatus  [in]   错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::ShowErrorString(GX_STATUS emErrorStatus)
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
\brief  刷新UI界面

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE)->EnableWindow(m_bDevOpened && m_bTriggerActive);
	GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(m_bDevOpened && m_bTriggerMode);
	GetDlgItem(IDC_COMBO_TRIGGER_SOURCE)->EnableWindow(m_bDevOpened && m_bTriggerSource);
    

	if (m_bColorFilter) 
	{
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bDevOpened && m_bColorFilter);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(m_bDevOpened && m_bBalanceRatioSelector);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(m_bDevOpened);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(m_bDevOpened
			&& (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
			&& m_bBalanceRatioSelector);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(m_bDevOpened && m_bColorFilter);
	}
	else 
	{
		GetDlgItem(IDC_COMBO_AWB)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IMAGE_IMPROVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM)->EnableWindow(FALSE);
	}
}

//----------------------------------------------------------------------------------
/**
\brief  图像质量提升开关，当开启此开关时，会在回调函数中对每一帧图像进行
        颜色校正、对比度和Gamma的调节，用以提高图像的质量。

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedCheckImageImprove() 
{
	// TODO: Add your control notification handler code here
	m_bIsImproveImg = !m_bIsImproveImg;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"打开设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus = GX_STATUS_SUCCESS;
	uint32_t       nDevNum  = 0;
	uint32_t	  nDSNum	  = 0;
	GX_OPEN_PARAM  stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// 枚举设备
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// 判断设备个数
	if (nDevNum <= 0)
	{
		MessageBox("没有发现设备!");
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

		// 获取设备的属性信息
		emStatus = GetDeviceParam();
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	// 获取相机参数,初始化界面控件
	InitUI();

	// 获取图像质量提升参数
	GetImageImproveParam();

	//设置定时器更新界面
	SetTimer(0, 1000, NULL);

	// 更新界面控件
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief   初始化相机:设置默认参数

\return  无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxSingleCamDlg::InitDevice()
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
\brief  获取设备的宽高等属性信息

\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxSingleCamDlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bColorFliter   = false;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	GX_ENUM_VALUE stEnumValue;
	int64_t i64PixelFormat = 0;

	// 获取输出像素格式
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// 获取当前像素格式
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}
	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(i64PixelFormat));

	//判断相机是否支持白平衡通道
	emStatus = GXGetNodeAccessMode(m_hDevice, "BalanceRatioSelector", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);
	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		//获取自动白平衡枚举值
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

		m_bBalanceRatioSelector = true;
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnCloseDevice()
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
	m_bDevOpened = FALSE;
	m_hDevice    = NULL;

	if (NULL != m_pBitmap) 
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	KillTimer(0);

	// 更新界面UI
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnStopSnap()
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
\brief  点击"开始采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 为图像显示准备资源
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

//----------------------------------------------------------------------------------
/**
\brief  切换"触发源"选项响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	emStatus = GXSetEnumValue(m_hDevice, "TriggerSource", nEnumVal);
	GX_VERIFY(emStatus);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"触发极性"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE);

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	emStatus = GXSetEnumValue(m_hDevice, "TriggerActivation", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"保存BMP图像"CheckBox框消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnBnClickedCheckSaveBmp()
{
	// TODO: Add your control notification handler code here
	m_bIsSaveImg = !m_bIsSaveImg;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件: Static:显示范围 Edit:输入值
 
\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitGainUI()
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_GAIN);
	int          nPos         = 0;

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strRange = "";
	double    dGainVal = 0;

	// 获取浮点型范围,并初始化SliderCtrl控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// 显示范围
    strRange.Format("增益(%.4f~%.4f)%s", m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strRange);
	
	// 获取当前值并更新到界面
	m_dGainValue = m_stGainFloatRange.dCurValue;
   
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光相关控件:Static:显示范围 Edit:输入值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);
	int          nPos            = 0;

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString         strTemp = "";
	double    dShutterValue = 0.0;

	// 获取浮点型范围,并初始化SliderCtrl控件和static范围提示框
	emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);

	strTemp.Format("曝光(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// 获取当前值并将当前值更新到界面
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"白平衡通道"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboRatioSelector()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR);
	double     dValue  = 0.0;

	int      nIndex    = pBox->GetCurSel();                   // 获取当前选项
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // 获取当前选项对应的枚举型值

	// 设置白平衡通道
	emStatus = GXSetEnumValue(m_hDevice, "BalanceRatioSelector", nEnumVal);
	GX_VERIFY(emStatus);

	// 获取白平衡系数更新界面
	InitWhiteRatioUI();
}

//---------------------------------------------------------------------------------
/**
\brief   "白平衡系数"Edit框失去焦点响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnEnKillfocusEditBalanceRatio()
{
	// TODO: Add your control notification handler code here
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData(TRUE);
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

    //判断输入值是否在白平衡系数的范围内
	//若大于最大值则将白平衡系数设为最大值
	if (m_dBalanceRatio > m_stFloatRange.dMax)
	{
		m_dBalanceRatio = m_stFloatRange.dMax;
	}
	//若小于最小值将白平衡系数设为最小值
	if (m_dBalanceRatio < m_stFloatRange.dMin)
	{
		m_dBalanceRatio = m_stFloatRange.dMin;
	}

	emStatus = GXSetFloatValue(m_hDevice,"BalanceRatio",m_dBalanceRatio);
	GX_VERIFY(emStatus);

    UpdateData(FALSE);
}

//-----------------------------------------------------------------------
/**
\brief   分发函数主要处理曝光和增益的Edit框响应回车键的消息
\param   pMsg  消息结构体

\return 成功:TRUE   失败:FALSE
*/
//-----------------------------------------------------------------------
BOOL CGxSingleCamDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd  *pWnd   = NULL;
	int   nCtrlID = 0;             //< 保存获取的控件ID

	//判断是否是键盘回车消息
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获得当前焦点的控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//判断ID类型
		switch(nCtrlID)
		{
		case IDC_EDIT_BALANCE_RATIO:
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:

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

//-----------------------------------------------------------------------
/**
\brief    控制曝光时间的Edit框失去焦点的响应函数

\return   无
*/
//-----------------------------------------------------------------------
void CGxSingleCamDlg::OnKillfocusEditShutter() 
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
	
	status = GXSetFloatValue(m_hDevice,"ExposureTime",m_dShutterValue);
	GX_VERIFY(status);


	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   控制增益值的Edit框失去焦点的响应函数

\return  无
*/
//--------------------------------------------------------------------
void CGxSingleCamDlg::OnKillfocusEditGain()
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

//----------------------------------------------------------------
/**
\brief  定时器函数:定时读取自动白平衡的枚举值实现设置Once成功后界面自动更新为off
\param  定时器事件ID

\return 无
*/
//----------------------------------------------------------------
void CGxSingleCamDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
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

			UpDateUI();	
			RefreshWhiteRatio();
		}
	}

	//如果自动白平衡方式为Continuous
	//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_CONTINUOUS)
	{
		RefreshWhiteRatio();
	}

	CDialog::OnTimer(nIDEvent);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"插值算法"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::OnCbnSelchangeComboInterpolationAlgorithm()
{
	// TODO: Add your control notification handler code here
	int nIndex = 0;
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_INTERPOLATION_ALGORITHM);
	nIndex = pBox->GetCurSel();                   // 获取当前选项

	m_pBitmap->SetInterpolationAlgorithm((DX_BAYER_CONVERT_TYPE)nIndex);
}
