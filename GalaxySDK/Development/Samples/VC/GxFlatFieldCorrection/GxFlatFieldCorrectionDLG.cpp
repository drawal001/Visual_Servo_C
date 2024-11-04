// GxFlatFieldCorrectionDlg.cpp : implementation file
//


#include "stdafx.h"
#include "GxFlatFieldCorrection.h"
#include "GxFlatFieldCorrectionDlg.h" 
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#undef new

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
// CGxFlatFieldCorrectionDlg dialog

CGxFlatFieldCorrectionDlg::CGxFlatFieldCorrectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxFlatFieldCorrectionDlg::IDD, pParent)
	, m_bDevOpened(false)
	, m_bIsSnap(false)
	, m_bIsFFC(false)
	, m_bColorFilter(false)
	, m_bBalanceRatioSelector(false)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_pBrightBuf(NULL)
	, m_pDarkBuf(NULL)
	, m_nActualBits(DX_ACTUAL_BITS_8)
	, m_nRatioSelector(-1)
	, m_nAutoWhiteBalance(0)
	, m_pWnd(NULL)
	, m_nNewGray(0)
	, m_nFrameCount(CORRECTION_ACTUAL_FRAME_COUNT_4)
	, m_bDarkImgAcquired(false)
	, m_bBrightImgAcquired(false)
	, m_bExecuteFFC(false)
	, m_bShowFFCOK(false)
	, m_bGetFFCCoefficients(false)
	, m_nBrightImgNum(0)
	, m_pAccumulateImgBuf(NULL)
	, m_bBrightImgAcquiredOnce(false)
	, m_bDarkImgAcquiredOnce(false)
	, m_pFFCCoefficients(NULL)
	, m_nFFClength(0)
	, m_nFFCGray(0)
	, m_bFFCGray(false)
	, m_nAverageGrayDarkImg(0)
	, m_nAverageGrayBrightImg(0)
	, m_pAverageImgBuf(NULL)
	, m_pBitmap(NULL)
	, m_i64ImageHeight(0)
	, m_i64ImageWidth(0)
{
	//{{AFX_DATA_INIT(CGxFlatFieldCorrectionDlg)
	m_dBalanceRatio = 0.0;
	m_dGainValue    = 0.0;
	m_dShutterValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 初始化参数
	memset(&m_stGainFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stShutterFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stFloatRange, 0, sizeof(GX_FLOAT_VALUE));
	memset(&m_stFlatFieldCorrection, 0, sizeof(FLAT_FIELD_CORRECTION_PROCESS));
}

void CGxFlatFieldCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxFlatFieldCorrectionDlg)
	DDX_Text(pDX, IDC_EDIT_BALANCE_RATIO, m_dBalanceRatio);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dShutterValue);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_FRAME_NUM, m_sliderFrameCount);
	DDX_Text(pDX, IDC_EDIT_GRAY, m_nNewGray);
	DDX_Control(pDX, IDC_SLIDER_GRAY, m_sliderGrayValue);
}

BEGIN_MESSAGE_MAP(CGxFlatFieldCorrectionDlg, CDialog)
	//{{AFX_MSG_MAP(CGxFlatFieldCorrectionDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnCbnSelchangeComboWriteBlance)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, OnBnClickedBtnStartSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_RATIO_SELECTOR, OnCbnSelchangeComboRatioSelector)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, OnEnKillfocusEditBalanceRatio)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP

	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_GRAY, OnKillfocusEditGray)
	ON_BN_CLICKED(IDC_BUTTON_ACQUIRE_DARK_IMG, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireDarkImg)
	ON_BN_CLICKED(IDC_BUTTON_ACQUIRE_BRIGHT_IMG, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireBrightImg)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE_FFC, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonExecuteFFC)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, &CGxFlatFieldCorrectionDlg::OnBnClickedCheckPreview)
	ON_BN_CLICKED(IDC_CHECK_GRAY, &CGxFlatFieldCorrectionDlg::OnBnClickedCheckGray)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CGxFlatFieldCorrectionDlg::OnBnClickedButtonWrite)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXPECT_GRAY_VALUE, &CGxFlatFieldCorrectionDlg::OnDeltaposSpinExpectGrayValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionDlg message handlers

BOOL CGxFlatFieldCorrectionDlg::OnInitDialog()
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
	// when the application's main window is not a dialog
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

	//获取图像显示窗口的指针和绘图DC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_FRAME);

	// 更新界面控件
	UpDateUI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxFlatFieldCorrectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxFlatFieldCorrectionDlg::OnPaint() 
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
HCURSOR CGxFlatFieldCorrectionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnClose() 
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
		m_bIsSnap = false;

		m_pBitmap->UnPrepareForShowImg();
	}

	// 未关闭设备则关闭设备
	if (m_bDevOpened)
	{
		// 清理资源
		UnPrepareForShowImg();
		emStatus = GXCloseDevice(m_hDevice);
		m_bDevOpened = false;
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
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitUI()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int  nValue = 0;

	// 初始化白平衡调节Combox控件
	InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);

	// 初始化白平衡通道Combox控件
	InitEnumUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_RATIO_SELECTOR), m_bBalanceRatioSelector);

	// 初始化曝光时间相关控件
	InitShutterUI();

	// 初始化增益相关控件
	InitGainUI();

	// 初始化白平衡系数相关控件
	InitWhiteRatioUI();

	// 初始化期望灰度值相关控件
	InitGrayUI();

	// 初始化采集帧数相关控件
	InitFrameCountUI();
}

//---------------------------------------------------------------------------------
/**
\brief   初始化白平衡系数相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitWhiteRatioUI()
{
	GX_STATUS      emStatus = GX_STATUS_SUCCESS;
	CString        strTemp  = _T("");

	if (!m_bBalanceRatioSelector)
	{
		return;
	}

	// 获取系数范围
	emStatus = GXGetFloatValue(m_hDevice, "BalanceRatio", &m_stFloatRange);
	GX_VERIFY(emStatus);
	strTemp.Format("白平衡系数(%.4f~%.4f)", m_stFloatRange.dMin, m_stFloatRange.dMax);
	SetDlgItemText(IDC_STATIC_WHITE_RATIO, strTemp);

	// 刷新显示当前白平衡系数
	RefreshWhiteRatio();
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
void CGxFlatFieldCorrectionDlg::InitEnumUI(char* chFeatire, CComboBox *pComboBox, bool bIsImplement)
{
	// 判断控件
	if ((pComboBox == NULL) || !bIsImplement)
	{
		return;
	}

	GX_ENUM_VALUE stEnumValue;
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	size_t    nbufferSize = 0;
	uint32_t  nEntryNum   = 0;
	int64_t   nEnumValue  = -1;
	int       nCursel     = 0;
	double    dValue      = 0;


	// 获取功能的枚举数
	emStatus = GXGetEnumValue(m_hDevice, chFeatire, &stEnumValue);
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
\brief   回调函数
\param   pFrame         回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall CGxFlatFieldCorrectionDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxFlatFieldCorrectionDlg *pDlg        = (CGxFlatFieldCorrectionDlg*)(pFrame->pUserParam);
	VxInt32                   DxStatus     = DX_OK;

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		return ;
	}

	// 采集暗场图像
	if (true == pDlg->m_bDarkImgAcquired)
	{
		memcpy(pDlg->m_pDarkBuf, pFrame->pImgBuf, pFrame->nImgSize);
		pDlg->AcquireDarkFieldImg();
		pDlg->m_bDarkImgAcquired = false;
	}

	// 采集亮场图像
	if (true == pDlg->m_bBrightImgAcquired)
	{
		memcpy(pDlg->m_pBrightBuf, pFrame->pImgBuf, pFrame->nImgSize);
		pDlg->AcquireBrightFieldImg();
	}		

	void* pImgBuf = const_cast<void*>(pFrame->pImgBuf);
	// 判断是否需要对图像进行平场校正
	if (pDlg->m_bIsFFC)
	{
		DxStatus = DxFlatFieldCorrection(pImgBuf
			, pImgBuf
			, pDlg->m_nActualBits
			, (VxUint32)(pFrame->nWidth)
			, (VxUint32)(pFrame->nHeight)
			, pDlg->m_pFFCCoefficients
			, &pDlg->m_nFFClength);


		if (DX_OK != DxStatus)
		{
			pDlg->m_bIsFFC = false;
			pDlg->GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("平场校正系数应用失败");
			pDlg->UpDateUI();
		}
		else
		{
			if (!pDlg->m_bShowFFCOK)
			{
				pDlg->GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("平场校正系数应用成功");  
				pDlg->UpDateUI();
				pDlg->m_bShowFFCOK = true;
			}
		}
	}

	pDlg->m_pBitmap->Show(pFrame);

}

//---------------------------------------------------------------------------------
/**
\brief   为图像显示准备资源

\return  true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool CGxFlatFieldCorrectionDlg::PrepareForShowImg()
{
	bool bRet = true;

	do 
	{
		int64_t i64ImageWidth  = m_i64ImageHeight;
		int64_t i64ImageHeight = m_i64ImageWidth;
		//为存储暗场图像数据申请空间
		m_pDarkBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pDarkBuf == NULL)
		{
			bRet = false;
			break;
		}
		memset(m_pDarkBuf, 0, (size_t)(i64ImageWidth * i64ImageHeight));

		//为存储亮场图像数据申请空间
		m_pBrightBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pBrightBuf == NULL)
		{
			bRet = false;
			break;
		}

		//为存储亮场累加图像数据申请空间
		m_pAccumulateImgBuf = new(std::nothrow) VxUint32[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pAccumulateImgBuf == NULL)
		{
			bRet = false;
			break;
		}

		//为存储亮场平均图像数据申请空间
		m_pAverageImgBuf = new(std::nothrow) BYTE[(size_t)(i64ImageWidth * i64ImageHeight)];
		if (m_pAverageImgBuf == NULL)
		{
			bRet = false;
			break;
		}

		//获取平场校正参数长度（字节数）
		//并为平场校正数据申请空间
		m_stFlatFieldCorrection.pBrightBuf = m_pAverageImgBuf;
		m_stFlatFieldCorrection.pDarkBuf   = m_pDarkBuf;
		DxGetFFCCoefficients(m_stFlatFieldCorrection, NULL, &m_nFFClength);
		m_pFFCCoefficients = new(std::nothrow) BYTE[m_nFFClength];
		if (m_pFFCCoefficients == NULL)
		{
			bRet = false;
			break;
		}
		GX_STATUS emStatus = m_pBitmap->PrepareForShowImg();
		if (GX_STATUS_SUCCESS != emStatus) 
		{
			bRet = false;
			break;
		}

	} while (0);


	// 若分配失败则释放已分配的资源
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//---------------------------------------------------------------------------------
/**
\brief   释放为图像显示准备资源

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::UnPrepareForShowImg()
{

	if (m_pDarkBuf != NULL)
	{
		delete []m_pDarkBuf;
		m_pDarkBuf = NULL;
	}
	if (m_pBrightBuf != NULL)
	{
		delete []m_pBrightBuf;
		m_pBrightBuf = NULL;
	}
	if(m_pAccumulateImgBuf != NULL)
	{
		delete []m_pAccumulateImgBuf;
		m_pAccumulateImgBuf = NULL;
	}
	if(m_pAverageImgBuf != NULL)
	{
		delete []m_pAverageImgBuf;
		m_pAverageImgBuf = NULL;
	}
	if(m_pFFCCoefficients != NULL)
	{
		delete []m_pFFCCoefficients;
		m_pFFCCoefficients = NULL;
	}
	m_pBitmap->UnPrepareForShowImg();
}

//---------------------------------------------------------------------------------
/**
\brief   切换"自动白平衡"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnCbnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	CComboBox *pBox     = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
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
void CGxFlatFieldCorrectionDlg::ShowErrorString(GX_STATUS emErrorStatus)
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
\brief  刷新UI界面

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bDevOpened && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bDevOpened && m_bIsSnap);

	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bDevOpened);

	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bDevOpened && m_bColorFilter);
	GetDlgItem(IDC_COMBO_RATIO_SELECTOR)->EnableWindow(m_bDevOpened && m_bBalanceRatioSelector);

	GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(m_bDevOpened 
		&& (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		&&  m_bBalanceRatioSelector);

	GetDlgItem(IDC_SLIDER_FRAME_NUM)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_CHECK_GRAY)->EnableWindow(m_bDevOpened);               
	((CButton*)GetDlgItem(IDC_CHECK_GRAY))->SetCheck(m_bDevOpened && m_bFFCGray);

	GetDlgItem(IDC_SLIDER_GRAY)->EnableWindow(m_bDevOpened && m_bFFCGray);
	GetDlgItem(IDC_EDIT_GRAY)->EnableWindow(m_bDevOpened && m_bFFCGray);
	GetDlgItem(IDC_SPIN_EXPECT_GRAY_VALUE)->EnableWindow(m_bDevOpened && m_bFFCGray);

	GetDlgItem(IDC_BUTTON_ACQUIRE_DARK_IMG)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BUTTON_ACQUIRE_BRIGHT_IMG)->EnableWindow(m_bDevOpened && m_bIsSnap);
	GetDlgItem(IDC_BUTTON_EXECUTE_FFC)->EnableWindow(m_bDevOpened && m_bIsSnap && m_bBrightImgAcquiredOnce);

	GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(m_bDevOpened && m_bIsSnap && m_bExecuteFFC);           
	((CButton*)GetDlgItem(IDC_CHECK_PREVIEW))->SetCheck(m_bDevOpened && m_bIsSnap && m_bExecuteFFC && m_bIsFFC);  

	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(m_bDevOpened && m_bIsSnap);                              
	GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(m_bDevOpened && m_bIsSnap);                            
}

//----------------------------------------------------------------------------------
/**
\brief  点击"打开设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nDevNum  = 0;
	uint32_t  nDSNum   = 0;

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

	// 设置相机的默认参数:采集模式:连续采集,数据格式:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	do
	{
		m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// 获取设备的宽、高等属性信息
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

	// 设置定时器更新界面
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
GX_STATUS CGxFlatFieldCorrectionDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取设备的属性信息

\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxFlatFieldCorrectionDlg::GetDeviceParam()
{
	GX_STATUS emStatus       = GX_STATUS_ERROR;
	bool      bColorFliter   = false;
	GX_INT_VALUE stIntValue;
	GX_ENUM_VALUE stEnumValue;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	int64_t i64PixelFormat;  //图像像素格式

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
	//判断是否为彩色
	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(i64PixelFormat));

	if (m_bColorFilter)
	{
		// 获取自动白平衡枚举值
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

	}

	// 判断相机是否支持白平衡通道
	emStatus = GXGetNodeAccessMode(m_hDevice, "BalanceRatioSelector", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		m_bBalanceRatioSelector = true;
	}
	else 
	{
		m_bBalanceRatioSelector = false;
	}

	// 获取宽度
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageWidth = stIntValue.nCurValue;

	// 获取高度
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageHeight = stIntValue.nCurValue;
	
	m_stFlatFieldCorrection.nImgWid = (VxUint32)m_i64ImageWidth;
	m_stFlatFieldCorrection.nImgHei = (VxUint32)m_i64ImageHeight;

	// 获取位宽
	m_stFlatFieldCorrection.nActualBits = m_nActualBits;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 清理资源
	UnPrepareForShowImg();

	// 未停采时则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
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

		m_bIsSnap                = false;
		m_bFFCGray               = false;

		m_bExecuteFFC            = false;
		m_bGetFFCCoefficients    = false;
		m_bIsFFC                 = false;
		m_bBrightImgAcquired     = false;
		m_bBrightImgAcquiredOnce = false;
		m_bDarkImgAcquired       = false;
		m_bDarkImgAcquiredOnce   = false;
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

	m_bDevOpened = false;
	m_hDevice    = NULL;

	KillTimer(0);

	// 更新界面UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("");
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;


	// 发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	// 注销回调
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	UnPrepareForShowImg();

	m_bExecuteFFC            = false;
	m_bGetFFCCoefficients    = false;
	m_bIsFFC                 = false;
	m_bBrightImgAcquired     = false;
	m_bBrightImgAcquiredOnce = false;
	m_bDarkImgAcquired       = false;
	m_bDarkImgAcquiredOnce   = false;
	m_bIsSnap                = false;

	// 更新界面UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("");
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 初始化BMP头信息、分配Buffer为图像采集做准备
	if (!PrepareForShowImg())
	{
		MessageBox("为图像显示分配资源失败!");
		return;
	}

	// 注册回调
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// 发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	m_bIsSnap = true;

	// 更新界面UI
	UpDateUI();
	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("1、平场校正操作需分别采集暗场和亮场图像。\r\n2、彩色相机强烈建议先进行白平衡。\r\n3、采集暗场图像时请盖上镜头盖。");
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件: Static:显示范围 Edit:输入值

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitGainUI()
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
	double    stGainVal = 0;

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
void CGxFlatFieldCorrectionDlg::InitShutterUI()
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);
	int          nPos            = 0;

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS emStatus      = GX_STATUS_ERROR;
	CString   strTemp       = "";
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
\brief   初始化期望灰度值相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitGrayUI()
{
	CSliderCtrl     *pSliderCtrl  = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GRAY);
	CEdit           *pEditGrayVal = (CEdit *)GetDlgItem(IDC_EDIT_GRAY);
	CSpinButtonCtrl *pSpin        = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXPECT_GRAY_VALUE);

	if ((pSliderCtrl == NULL) || (pEditGrayVal == NULL) || (pSpin == NULL) )
	{
		return;
	}

	// 设置期望灰度值滑动条范围和初始值、调节按钮范围、编辑框初始值
	pSliderCtrl->SetRange(GRAY_MIN, GRAY_MAX, false);
	pSliderCtrl->SetPos(GRAY_INIT);
	pSpin->SetRange(GRAY_MIN, GRAY_MAX);
	m_nNewGray = GRAY_INIT;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化图像采集帧数相关控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::InitFrameCountUI()
{
	CSliderCtrl *pSliderCtrl  = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_FRAME_NUM);

	if (NULL == pSliderCtrl)
	{
		return;
	}

	// 设置显示范围和默认值
	pSliderCtrl->SetRange(CORRECTION_UI_FRAME_COUNT_MIN, CORRECTION_UI_FRAME_COUNT_MAX, true);
	pSliderCtrl->SetPos(CORRECTION_UI_FRAME_COUNT_2);

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   切换"白平衡通道"选项响应函数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnCbnSelchangeComboRatioSelector()
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
void CGxFlatFieldCorrectionDlg::OnEnKillfocusEditBalanceRatio()
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
BOOL CGxFlatFieldCorrectionDlg::PreTranslateMessage(MSG* pMsg)
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
		case IDC_EDIT_GRAY: 

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
void CGxFlatFieldCorrectionDlg::OnKillfocusEditShutter() 
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

	status = GXSetFloatValue(m_hDevice, "ExposureTime", m_dShutterValue);
	GX_VERIFY(status);


	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   控制增益值的Edit框失去焦点的响应函数

\return  无
*/
//--------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	// 判断句柄是否有效，避免设备掉线后关闭程序出现的异常
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

	status = GXSetFloatValue(m_hDevice, "Gain", m_dGainValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//--------------------------------------------------------------------
/**
\brief   控制平场校正期望灰度值的Edit框失去焦点的响应函数

\return  无
*/
//--------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnKillfocusEditGray()
{
	// TODO: Add your control notification handler code here
	// 判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return;
	}

	UpdateData();
	GX_STATUS status = GX_STATUS_SUCCESS;

	//判断输入值是否在0-255值的范围内
	//若输入的值大于最大值则将平场校正期望值设置成最大值
	if (m_nNewGray > GRAY_MAX)
	{
		m_nNewGray = GRAY_MAX;
	}

	//若输入的值小于最小值则将平场校正期望值设置成最小值
	if (m_nNewGray < GRAY_MIN)
	{
		m_nNewGray = GRAY_MIN;
	}
	m_sliderGrayValue.SetPos(m_nNewGray);

	UpdateData(FALSE);
}

//----------------------------------------------------------------
/**
\brief  定时器函数:定时读取自动白平衡的枚举值实现设置Once成功后界面自动更新为off
\param  定时器事件ID

\return 无
*/
//----------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus  = GX_STATUS_ERROR;

	//如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
	//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		GX_ENUM_VALUE stEnumValue;
		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);

		//获取自动白平衡枚举值
		emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return;
		}

		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue; //< 保存白平衡的枚举值

		//判断设备自动白平衡是否变为OFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// 选中自动白平衡控件中的OFF项,即由ONCE变为OFF
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
\brief   滑动条消息响应函数
\param   nSBCode     指定卷轴滚动请求的代码
\param   nPos        滑动条位置
\param   pScrollBar  滑动条控件指针

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	char PutText[10] = {0};
	int  nValue      = 0;

	if (pScrollBar == (CScrollBar*)&m_sliderFrameCount)            // 采集帧数滑动条
	{
		m_nFrameCount = m_sliderFrameCount.GetPos();
		GetFrameCount(m_nFrameCount);
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderGrayValue)        // 平场校正期望灰度值滑动条
	{
		m_nNewGray = m_sliderGrayValue.GetPos();
		sprintf_s(PutText,"%d", m_nNewGray);
		GetDlgItem(IDC_EDIT_GRAY)->SetWindowText(PutText);	
	}

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//---------------------------------------------------------------------------------
/**
\brief   数值调节按钮消息响应函数
\param   pNMHDR     通知消息句柄
\param   pResult    结果指针

\return  无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnDeltaposSpinExpectGrayValue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	UpdateData();

	if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		m_nNewGray--;

		//若输入的值小于最小值则将平场校正期望值设置成最小值
		if (m_nNewGray < GRAY_MIN)
		{
			m_nNewGray = GRAY_MIN;
		}
		m_sliderGrayValue.SetPos(m_nNewGray);
	}

	else if(pNMUpDown->iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		m_nNewGray++;

		//若输入的值大于最大值则将平场校正期望值设置成最大值
		if (m_nNewGray > GRAY_MAX)
		{
			m_nNewGray = GRAY_MAX;
		}
		m_sliderGrayValue.SetPos(m_nNewGray);
	}

	UpdateData(false);
}

//----------------------------------------------------------------------------------
/**
\brief  点击"采集暗场图像"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireDarkImg()
{
	// TODO: Add your control notification handler code here
	m_bDarkImgAcquired = true;
	m_bExecuteFFC      = false;
	m_bIsFFC           = false;

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  点击"采集亮场图像"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonAcquireBrightImg()
{
	// TODO: Add your control notification handler code here
	memset(m_pAccumulateImgBuf, 0, sizeof(VxUint32) * (size_t)(m_i64ImageWidth * m_i64ImageHeight));

	m_bBrightImgAcquired     = true;
	m_bBrightImgAcquiredOnce = false;
	m_bExecuteFFC            = false;
	m_bIsFFC                 = false; 

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  求取平场校正系数

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonExecuteFFC()
{
	// TODO: Add your control notification handler code here
	VxInt32  emStatus = DX_OK;

	// 判断是否采集过暗场图像
	if (false == m_bDarkImgAcquiredOnce)
	{
		m_stFlatFieldCorrection.pDarkBuf = NULL;
	}
	else
	{
		m_stFlatFieldCorrection.pDarkBuf = m_pDarkBuf;
	}

	// 若期望灰度值单选框被勾选，则执行平场校正时采用该期望灰度值
	m_nFFClength = (int)(4 * m_i64ImageWidth * m_i64ImageHeight);
	if (true == m_bFFCGray)
	{
		m_nFFCGray = m_nNewGray;
		emStatus = DxGetFFCCoefficients(m_stFlatFieldCorrection, m_pFFCCoefficients, &m_nFFClength, &m_nFFCGray);
	}
	else
	{
		emStatus = DxGetFFCCoefficients(m_stFlatFieldCorrection, m_pFFCCoefficients, &m_nFFClength);
	}

	//判断平场校正系数求取结果，并给出显示信息
	if (DX_OK != emStatus)
	{
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("平场校正系数求取失败");
		m_bExecuteFFC         = false;  
		m_bGetFFCCoefficients = false;
		m_bIsFFC              = false; 
	} 
	else
	{
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("平场校正系数求取成功");
		m_bExecuteFFC         = true;      //执行平场校正成功
		m_bShowFFCOK          = false;
		m_bGetFFCCoefficients = true;      //求取平场校正系数成功
		m_bIsFFC              = true;      //应用平场校正系数
	}

	UpDateUI();	
}

//----------------------------------------------------------------------------------
/**
\brief  平场校正开关，当开启此开关时，会在回调函数中对每一对采集的暗场图像和
亮场图像进行平场校正。

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedCheckPreview()
{
	// TODO: Add your control notification handler code here
	m_bIsFFC = !m_bIsFFC;
	if (m_bIsFFC)
	{
		m_bShowFFCOK = false;
	}

	GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText(" ");
}

//-------------------------------------------------------------------
/**
\brief  图像累加函数
\param  pImgBuf           输入图像buf
\param  nDataWidth        图像位宽
\param  nlength           图像长度
\param  pAccumulateImgBuf 累加图像buff

\return  阴影校正错误码
*/
//-------------------------------------------------------------------
DX_STATUS CGxFlatFieldCorrectionDlg::AccumulateImage(void *pImgBuf, int nDataWidth, int64_t nlength, VxUint32 *pAccumulateImgBuf)
{
	if (NULL == pImgBuf || NULL == pAccumulateImgBuf)
	{
		return DX_PARAMETER_INVALID;
	}

	// 数据位宽不满足要求 返回错误
	if ((nDataWidth < IMAGE_DATA_NUM_MIN) || (nDataWidth > IMAGE_DATA_NUM_MAX))
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (nlength < 1)
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (IMAGE_DATA_NUM_MIN == nDataWidth)
	{

		unsigned char * pImgBufTemp = (unsigned char *)pImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAccumulateImgBuf[i] += pImgBufTemp[i];
		}

	}
	else
	{
		unsigned short * pImgBufTemp = (unsigned short *)pImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAccumulateImgBuf[i] += pImgBufTemp[i];
		}
	}

	return DX_OK;
}

//-------------------------------------------------------------------
/**
\brief  平均图像计算函数
\param  pImgBuf                累加图像buff
\param  nlength                图像大小
\param  nAccumulateNum         累加图像张数
\param  nDataWidth             图像位宽
\param  pAverageImgBuf         平均图

\return  阴影校正错误码
*/
//-------------------------------------------------------------------
DX_STATUS CGxFlatFieldCorrectionDlg::GetAverageImage(VxUint32 *pImgBuf, int64_t nlength, int nAccumulateNum, int nDataWidth, void *pAverageImgBuf)
{
	if (NULL == pImgBuf || NULL == pAverageImgBuf)
	{
		return DX_PARAMETER_INVALID;
	}

	// 数据位宽不满足要求 返回错误
	if ((nDataWidth < IMAGE_DATA_NUM_MIN) || (nDataWidth > IMAGE_DATA_NUM_MAX))
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	if (nlength < 1 || nAccumulateNum < 1)
	{
		return DX_PARAMETER_OUT_OF_BOUND;
	}

	int64_t i = 0;
	if (IMAGE_DATA_NUM_MIN == nDataWidth)
	{

		unsigned char *pAverageImgBufTemp = (unsigned char *)pAverageImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAverageImgBufTemp[i] = pImgBuf[i] / nAccumulateNum;
		}

	}
	else
	{
		unsigned short *pAverageImgBufTemp = (unsigned short *)pAverageImgBuf;
		for (int64_t i = 0; i < nlength; i++)
		{
			pAverageImgBufTemp[i] = pImgBuf[i]  / nAccumulateNum;
		}
	}

	return DX_OK;
}

//----------------------------------------------------------------------------------
/**
\brief  点击平场校正期望灰度值控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedCheckGray()
{
	// TODO: Add your control notification handler code here
	m_bFFCGray = !m_bFFCGray;
	UpDateUI();
}

//-------------------------------------------------------------------
/**
\brief   计算图像平均灰度值
\param   pImgBuf                图像buff
\param   nlength                图像大小

\return  阴影校正错误码
*/
//-------------------------------------------------------------------
int64_t CGxFlatFieldCorrectionDlg::GetAverageGray8bit(BYTE *pImgBuf, int64_t nImgSize)
{
	int64_t nGrayAverage = 0;
	int64_t nGraySum     = 0;

	if ((NULL == pImgBuf) || (nImgSize < 1))
	{
		return GRAY_INVALID;
	}

	//计算整张图像的灰度值累加和
	for(int64_t i = 0; i < nImgSize; i++)
	{
		nGraySum += (int64_t) *(pImgBuf + i);
	}

	nGrayAverage = nGraySum / nImgSize;
	return nGrayAverage;
}

//-------------------------------------------------------------------
/**
\brief   计算亮场图像采集帧数
\param   nFrameCount      图像采集帧数        

\return  阴影校正错误码
*/
//-------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::GetFrameCount(int &nFrameCount)
{
	//UI界面采集帧数与实际采集帧数间转换
	switch (nFrameCount)   
	{
	case CORRECTION_UI_FRAME_COUNT_MIN:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_1;
		break;

	case CORRECTION_UI_FRAME_COUNT_1:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_2;
		break;

	case CORRECTION_UI_FRAME_COUNT_2:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_4;
		break;

	case CORRECTION_UI_FRAME_COUNT_3:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_8;
		break;

	case CORRECTION_UI_FRAME_COUNT_MAX:
		nFrameCount = CORRECTION_ACTUAL_FRAME_COUNT_16;
		break;

	default: 
		break;
	}
}

//-------------------------------------------------------------------
/**
\brief   从文件中读取FFC参数
\param   strFilePath   文件读取路径        

\return  文件状态码
*/
//-------------------------------------------------------------------
FFC_STATUS CGxFlatFieldCorrectionDlg::ReadFFCCoefficientsFile(const std::string &strFilePath)
{
	FFC_STATUS emStatus = FFC_STATUS_SUCCESS;
	FILE *fp = NULL;

	do 
	{
		if (NULL == m_pFFCCoefficients)
		{
			emStatus = FFC_STATUS_FILE_DATA_ERROR;
			break;
		}

		//查找.ffc后缀的文件
		size_t nSuffix = strFilePath.rfind(".ffc");
		if (nSuffix != strFilePath.size() - strlen(".ffc"))
		{
			emStatus = FFC_STATUS_FILE_PATH_ERROR;
			break;
		}

		//打开文件对象
		size_t nError = fopen_s(&fp, strFilePath.c_str(), "rb");
		if (0 != nError)
		{
			emStatus = FFC_STATUS_FILE_OPEN_ERROR;
			break;
		}

		//判断FFC系数长度
		fseek(fp, 0L, SEEK_END);
		size_t nFileLength = ftell(fp);
		if (((2 * m_i64ImageWidth * m_i64ImageHeight) != nFileLength)
			&& ((4 * m_i64ImageWidth * m_i64ImageHeight) != nFileLength))
		{
			emStatus = FFC_STATUS_FILE_READ_ERROR;
			break;
		}
		else
		{	
			m_nFFClength = (int)nFileLength;
		}

		//从文件中读取FFC参数
		fseek(fp, 0, SEEK_SET);
		nError = fread(m_pFFCCoefficients, m_nFFClength, 1, fp);
		if (1 != nError)
		{
			emStatus = FFC_STATUS_FILE_READ_ERROR;
			break;
		}

	} while (0);

	if (fp)
	{
		fclose(fp);
	}

	return emStatus;
}

//-------------------------------------------------------------------
/**
\brief   将FFC参数保存到文件中
\param   strFilePath   文件保存路径       

\return  文件状态码
*/
//-------------------------------------------------------------------
FFC_STATUS CGxFlatFieldCorrectionDlg::WriteFFCCoefficientsFile(const std::string &strFilePath)
{
	FFC_STATUS emStatus = FFC_STATUS_SUCCESS;
	FILE       *fp      = NULL;

	do 
	{
		if (NULL == m_pFFCCoefficients)
		{
			emStatus = FFC_STATUS_FILE_DATA_ERROR;
			break;
		}

		// 文件路径是否有效
		size_t nSuffix = strFilePath.rfind(".ffc");
		if (nSuffix != strFilePath.size() - strlen(".ffc"))
		{
			emStatus = FFC_STATUS_FILE_PATH_ERROR;
			break;
		}

		// 文件对象创建
		int nError = fopen_s(&fp, strFilePath.c_str(), "wb");
		if (0 != nError)
		{
			emStatus = FFC_STATUS_FILE_OPEN_ERROR;
			break;
		}

		// 将FFC参数写入文件中
		if (1 != fwrite(m_pFFCCoefficients, m_nFFClength, 1, fp))
		{
			emStatus = FFC_STATUS_FILE_WRITE_ERROR;
			break;
		}
	} while (0);

	if (fp)
	{
		fclose(fp);
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  点击“从文件读取”控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonRead()
{
	// TODO: Add your control notification handler code here
	FFC_STATUS emStatus    = FFC_STATUS_SUCCESS;
	CString    strFilePath = "";

	//读取FFC文件，若读取成功则开启平场校正（预览按钮激活勾选）
	CFileDialog objFileDialog( TRUE, NULL, NULL, 0, "ffc文件 (*.ffc)|*.ffc||", this);
	if( objFileDialog.DoModal() == IDOK )
	{
		strFilePath = objFileDialog.GetPathName();	           //选择的文件路径	
		emStatus    = ReadFFCCoefficientsFile(strFilePath.GetBuffer(0));
		switch (emStatus)   
		{
		case FFC_STATUS_FILE_DATA_ERROR:
			MessageBox("参数无效！");
			break;

		case FFC_STATUS_FILE_PATH_ERROR:
			MessageBox("文件路径无效！");
			break;

		case FFC_STATUS_FILE_OPEN_ERROR:
			MessageBox("打开文件失败！");
			break;

		case FFC_STATUS_FILE_READ_ERROR:
			MessageBox("参数读取失败！");
			break;

		default: 
			break;
		}

		if (FFC_STATUS_SUCCESS == emStatus)
		{
			m_bExecuteFFC         = true;
			m_bShowFFCOK          = false;
			m_bIsFFC              = true;
			m_bGetFFCCoefficients = true;
		}
		else
		{
			m_bExecuteFFC         = false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
			m_bGetFFCCoefficients = false;
			m_bIsFFC              = false;    
			UpDateUI();
			GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText(" ");
		}
	}	

}

//----------------------------------------------------------------------------------
/**
\brief  点击“保存到文件”控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::OnBnClickedButtonWrite()
{
	// TODO: Add your control notification handler code here
	FFC_STATUS emStatus    = FFC_STATUS_SUCCESS;
	CString    strFilePath = "";

	if (false == m_bGetFFCCoefficients)
	{
		MessageBox("参数无效！");
		return ;
	}

	//将FFC系数以二进制形式保存到后缀为.ffc文件中
	CFileDialog objFileDialog( FALSE, ".ffc", "FlatFieldCorrection.ffc", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ffc文件 (*.ffc)|*.ffc||", this);
	if( objFileDialog.DoModal() == IDOK )
	{
		strFilePath = objFileDialog.GetPathName();	           //选择的文件路径	
		emStatus = WriteFFCCoefficientsFile(strFilePath.GetBuffer(0));
		switch (emStatus)   
		{
		case FFC_STATUS_FILE_DATA_ERROR:
			MessageBox("参数无效！");
			break;

		case FFC_STATUS_FILE_PATH_ERROR:
			MessageBox("文件路径无效！");
			break;

		case FFC_STATUS_FILE_OPEN_ERROR:
			MessageBox("打开文件失败！");
			break;

		case FFC_STATUS_FILE_WRITE_ERROR:
			MessageBox("参数写入失败！");
			break;

		default: 
			break;
		} //end of switch (emStatus)
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   刷新显示白平衡系数

\return  无
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::RefreshWhiteRatio()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_FLOAT_VALUE    stValue;
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
\brief   采集暗场图像

\return  无
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::AcquireDarkFieldImg(void)
{
	// 求采集的暗场图像的平均灰度值
	m_nAverageGrayDarkImg = (int)GetAverageGray8bit(m_pDarkBuf, m_i64ImageWidth * m_i64ImageHeight);
	if (GRAY_INVALID == m_nAverageGrayDarkImg)
	{
		m_bDarkImgAcquiredOnce = false;
		MessageBox("暗场图像采集失败，请重新采集！");
	}
	else if (m_nAverageGrayDarkImg > BRIGHT_IMG_GRAY_MIN)
	{
		m_bDarkImgAcquiredOnce = false;
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("暗场图像过亮，平场校正将不使用该暗场图像，这不会影响平场校正的效果");
	} 
	else
	{
		m_bDarkImgAcquiredOnce = true;
		GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("采集暗场图像完成");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   采集亮场图像

\return  无
*/
//---------------------------------------------------------------------------------
void CGxFlatFieldCorrectionDlg::AcquireBrightFieldImg(void)
{
	DX_STATUS emStatus = DX_OK;
	int64_t   nImgSize = 0;
	nImgSize = m_i64ImageWidth * m_i64ImageHeight;

	//求采集的亮场图像的平均灰度值
	do 
	{
		//求取累加亮场图像
		emStatus = AccumulateImage(m_pBrightBuf, m_nActualBits, nImgSize, m_pAccumulateImgBuf);
		if (DX_OK != emStatus)
		{
			m_nBrightImgNum = 0;
			m_bBrightImgAcquired = false;
			MessageBox("累加亮场图像无效，请重新采集亮场图像！");
			break;
		}

		m_nBrightImgNum++;
		if (m_nFrameCount == m_nBrightImgNum)
		{
			//求取平均亮场图像
			emStatus = GetAverageImage(m_pAccumulateImgBuf, nImgSize, m_nBrightImgNum, m_nActualBits, m_pAverageImgBuf);
			if (DX_OK != emStatus)
			{
				m_nBrightImgNum = 0;
				m_bBrightImgAcquired = false;
				MessageBox("平均亮场图像无效，请重新采集亮场图像！");
				break;
			}

			// 求亮场图像平均灰度值并进行判断
			m_nAverageGrayBrightImg = (int)GetAverageGray8bit(m_pAverageImgBuf, nImgSize);
			if (GRAY_INVALID == m_nAverageGrayBrightImg)
			{
				MessageBox("亮场图像采集失败，请重新采集！");
			}
			else if (m_nAverageGrayBrightImg < BRIGHT_IMG_GRAY_MIN)
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("亮场图像过暗，这将会影响平场校正效果，建议将图像亮度值调整到20-250之间");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			} 
			else if (m_nAverageGrayBrightImg > BRIGHT_IMG_GRAY_MAX)
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("亮场图像过亮，这将会影响平场校正效果，建议将图像亮度值调整到20-250之间");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			} 
			else
			{
				GetDlgItem(IDC_STATIC_PROMPT_INFOR)->SetWindowText("采集亮场图像完成");
				m_bBrightImgAcquiredOnce = true;
				UpDateUI();
			}

			m_nBrightImgNum = 0;
			m_bBrightImgAcquired = false;
		}
	} while (0);
}