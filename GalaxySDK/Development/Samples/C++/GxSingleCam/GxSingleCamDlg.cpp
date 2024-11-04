// GxSingleCamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GxSingleCam.h"
#include "GxSingleCamDlg.h"
#include "FileVersion.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strStaticProductVersion;
	CString m_strStaticLegalCopyright;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strStaticProductVersion(_T(""))
, m_strStaticLegalCopyright(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strStaticProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strStaticLegalCopyright);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGxSingleCamDlg 对话框
CGxSingleCamDlg::CGxSingleCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSingleCamDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_bColorFilter(false)
	, m_bTriggerMode(false)
	, m_bTriggerSource(false)
	, m_bTriggerActive(false)
	, m_bBalanceWhiteAuto(false)
	, m_bBalanceWhiteRatioSelect(false)
	, m_strSavePath("")
	, m_strBalanceWhiteAutoMode("Off")
	, m_pWnd(NULL)
    , m_pSampleCaptureEventHandle(NULL)
	, m_pBitmap(NULL)
	, m_bCheckSaveBmp(FALSE)
	, m_dEditShutterValue(0)
	, m_dEditGainValue(0)
	, m_dEditBalanceRatioValue(0)
	, m_nTriggerModeOld(0)
	, m_nTriggerSourceOld(0)
	, m_nTriggerActiveOld(0)
	, m_nBalanceWhiteAutoOld(0)
	, m_nBanlanceWhiteRatioOld(0)
	, m_dShutterValueMax(0)
	, m_dShutterValueMin(0)
	, m_dGainValueMax(0)
	, m_dGainValueMin(0)
	, m_dBalanceWhiteRatioMax(0)
	, m_dBalanceWhiteRatioMin(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxSingleCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SAVE_BMP, m_bCheckSaveBmp);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dEditShutterValue);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dEditGainValue);
	DDX_Text(pDX, IDC_EDIT_BALANCE_WHITE_SERISE, m_dEditBalanceRatioValue);
}

BEGIN_MESSAGE_MAP(CGxSingleCamDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CGxSingleCamDlg::OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CGxSingleCamDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CGxSingleCamDlg::OnBnClickedBtnStopSnap)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_SOURCE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_ACTIVE, &CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive)
	ON_BN_CLICKED(IDC_CHECK_SAVE_BMP, &CGxSingleCamDlg::OnBnClickedCheckSaveBmp)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_AUTO, &CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_SELECT, &CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteSelect)
	ON_BN_CLICKED(IDC_BTN_SOFTTRIGGER, &CGxSingleCamDlg::OnBnClickedBtnSofttrigger)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CGxSingleCamDlg::OnEnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_WHITE_SERISE, &CGxSingleCamDlg::OnEnKillfocusEditBalanceWhiteSerise)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, &CGxSingleCamDlg::OnEnKillfocusEditShutter)

END_MESSAGE_MAP()

// CGxSingleCamDlg 消息处理程序
BOOL CGxSingleCamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	try
	{
		//初始化设备
		IGXFactory::GetInstance().Init();

		//将窗口指针指向picture控件
		m_pWnd = GetDlgItem(IDC_SHOW_PICTURE_STATIC);
		m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();

		//获取可执行程序的当前路径
		char    strFileName[MAX_PATH] = {0};
		string  strSavePath           = "";
		size_t  nPos                  = 0;

		GetModuleFileName(NULL,(LPCH)strFileName,MAX_PATH);
		strSavePath = strFileName;
		nPos = strSavePath.find_last_of('\\');
		m_strSavePath = strSavePath.substr(0,nPos);
		m_strSavePath = m_strSavePath +"\\GxSingleCamImages";

		//设置定时器
		SetTimer(0,1000,NULL);

		//更新界面
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;
		
	}
	catch (std::exception& e)
	{
		if (m_pSampleCaptureEventHandle != NULL)
		{
			delete m_pSampleCaptureEventHandle;
			m_pSampleCaptureEventHandle = NULL;
		}

		MessageBox(e.what());
		return FALSE;

	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGxSingleCamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL, strFileName, MAX_PATH);
		CFileVersion fv(strFileName);
		CAboutDlg dlgAbout;
		dlgAbout.m_strStaticProductVersion = _T("Version: ") + fv.GetProductVersion();
		dlgAbout.m_strStaticLegalCopyright = fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGxSingleCamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGxSingleCamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGxSingleCamDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen = false;         ///< 设备是否打开标志
	bool bIsStreamOpen = false;         ///< 设备流是否打开标志

	try
	{
		//枚举设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);

	    //判断枚举到的设备是否大于零，如果不是则弹框提示
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("未发现设备!");
			return;
		}

		//打开设备
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(),GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//判断画图对象是否为空
		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		//为画图对象分配内存
		m_pBitmap = new CGXBitmap(m_objDevicePtr,m_pWnd);

		//判断设备流是否大于零，如果大于零则打开流
		int nStreamCount = m_objDevicePtr->GetStreamCount();

		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			m_objStreamFeatureControlPtr = m_objStreamPtr->GetFeatureControl();
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("未发现设备流!");
		}

        // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
        // 以提高网络相机的采集性能,设置方法参考以下代码。
        GX_DEVICE_CLASS_LIST objDeviceClass = m_objDevicePtr->GetDeviceInfo().GetDeviceClass();
        if(GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            // 判断设备是否支持流通道数据包功能
            if(true == m_objFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                // 获取当前网络环境的最优包长值
                int nPacketSize = m_objStreamPtr->GetOptimalPacketSize();
                // 将最优包长值设置为当前设备的流通道包长值
                m_objFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            } 
        }

		//初始化设备参数
		__InitParam();

		//初始化界面
		__InitUI();
		m_bIsOpen = true;

		//更新界面
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		MessageBox(e.what());
		return;
	}
	catch (std::exception& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		MessageBox(e.what());
		return;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   初始化设备参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitParam()
{
	bool bBalanceWhiteAutoRead = false;         ///< 白平衡是否可读

	//设置采集模式为连续采集模式
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//是否支持触发模式选择
	m_bTriggerMode = m_objFeatureControlPtr->IsImplemented("TriggerMode");
	if (m_bTriggerMode)
	{
		//设置触发模式关
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
	}

	//是否是彩色
	m_pBitmap->IsColor(m_objDevicePtr,m_bColorFilter);

	//是否支持触发源选择
	m_bTriggerSource = m_objFeatureControlPtr->IsImplemented("TriggerSource");

	//是否支持触发极性选择
	m_bTriggerActive = m_objFeatureControlPtr->IsImplemented("TriggerActivation");

	//是否支持自动白平衡
	m_bBalanceWhiteAuto = m_objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

	//白平衡是否可读
	bBalanceWhiteAutoRead   = m_objFeatureControlPtr->IsReadable("BalanceWhiteAuto");

	//如果支持且可读，则获取设备当前白平衡模式
	if (m_bBalanceWhiteAuto)
	{
		if (bBalanceWhiteAutoRead)
		{
			m_strBalanceWhiteAutoMode = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                              ->GetValue();
		}
	}

	//是否支持自动白平衡通道选择
	m_bBalanceWhiteRatioSelect = m_objFeatureControlPtr->IsImplemented("BalanceRatioSelector");

	//获取曝光时间、增益及自动白平衡系数的最大值和最小值
	m_dShutterValueMax      = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();
	m_dShutterValueMin      = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();
	m_dGainValueMax         = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
	m_dGainValueMin         = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetMin();
	m_dBalanceWhiteRatioMax = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetMax();
	m_dBalanceWhiteRatioMin = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetMin();
}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitUI()
{
	//判断是否为彩色相机
	if (m_bColorFilter)
	{
		//初始化组合框
		m_nTriggerModeOld        = __InitComBox("TriggerMode",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE),
			                                    m_bTriggerMode);
		m_nTriggerSourceOld      = __InitComBox("TriggerSource",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),
			                                     m_bTriggerSource);
		m_nTriggerActiveOld      = __InitComBox("TriggerActivation",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE),
			                                     m_bTriggerActive);
		m_nBanlanceWhiteRatioOld = __InitComBox("BalanceRatioSelector",
			                                     (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT),
			                                     m_bBalanceWhiteRatioSelect);
		m_nBalanceWhiteAutoOld   = __InitComBox("BalanceWhiteAuto",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO),
			                                     m_bBalanceWhiteAuto);
	}
	else
	{
		//初始化组合框
		m_nTriggerModeOld        = __InitComBox("TriggerMode",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE),
			                                    m_bTriggerMode);
		m_nTriggerSourceOld      = __InitComBox("TriggerSource",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE),
			                                    m_bTriggerSource);
		m_nTriggerActiveOld      = __InitComBox("TriggerActivation",
			                                    (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE),
			                                    m_bTriggerActive);
	}
	
	//初始化曝光控件
	__InitShutterTime();
	
	//初始化增益控件
	__InitGain();

	//初始化白平衡系数控件
	__InitBalanceWhiteRatio();

}

//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   strName         [in]    功能名称
\param   pComboBox       [in]    控件指针
\param   bIsImplemented  [in]    设备是否支持emFeatureID代表功能

\return  无
*/
//----------------------------------------------------------------------------------
int CGxSingleCamDlg::__InitComBox(gxstring strName, CComboBox* pComboBox, bool bIsImplemented)
{
	if ((pComboBox == NULL) || (!bIsImplemented))
	{
		return -1;
	}

	int     nCurSel = 0;
	string  strCurrentEnumList = "";                 // 设备当前功能
	GxIAPICPP::gxstring_vector vectorEnumEntryList;  // 设备枚举项
	CEnumFeaturePointer objEnumFeaturePtr = m_objFeatureControlPtr->GetEnumFeature(strName);

	//清除当前控件所有选项
	pComboBox->ResetContent();

	//获取设备当前功能值
	strCurrentEnumList = objEnumFeaturePtr->GetValue().c_str();

	//获取设备所有枚举项
	vectorEnumEntryList = objEnumFeaturePtr->GetEnumEntryList();
	for (uint32_t i = 0; i<vectorEnumEntryList.size(); i++)
	{
		string strEnumList = vectorEnumEntryList[i].c_str();
		pComboBox->SetItemData(pComboBox->AddString(strEnumList.c_str()), i);
		if (strCurrentEnumList == vectorEnumEntryList[i].c_str())
		{
			nCurSel = i;
		}
	}

	pComboBox->SetCurSel(nCurSel);
	return nCurSel;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光时间控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitShutterTime()
{
	CStatic* pStcShutterShow    = (CStatic*)GetDlgItem(IDC_STATIC_SHUTTER_TIME);
	CEdit*   pEditShutterVal    = (CEdit*)GetDlgItem(IDC_EDIT_SHUTTER);

	//判断控件的有效性
	if (pEditShutterVal == NULL || pStcShutterShow == NULL)
	{
		return;
	}

	CString   strShutterTime      = "";       ///< 当前曝光时间
	gxstring  strShutterUnit      = "";       ///< 曝光时间单位

	//获取设备当前曝光时间
	m_dEditShutterValue   = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();

	//获取设备曝光时间单位
    strShutterUnit = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetUnit();
	strShutterTime.Format("曝光(%.4f~%.4f)%s",m_dShutterValueMin,m_dShutterValueMax,strShutterUnit.c_str());
	pStcShutterShow->SetWindowText(strShutterTime);
	
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitGain()
{
	CStatic* pStcGainShow    = (CStatic*)GetDlgItem(IDC_STATIC_GAIN);
	CEdit*   pEditGainVal    = (CEdit*)GetDlgItem(IDC_EDIT_GAIN);

	//判断控件的有效性
	if (pEditGainVal == NULL || pStcGainShow == NULL)
	{
		return;
	}

	CString  strGain      = "";     ///< 当前增益值
	string strGainUnit    = "";     ///< 增益单位

	//获取设备当前增益值
	m_dEditGainValue = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetValue();
	
	//获取设备增益单位
	strGainUnit = m_objFeatureControlPtr->GetFloatFeature("Gain")->GetUnit().c_str();
	strGain.Format("增益(%.4f~%.4f)%s",m_dGainValueMin,m_dGainValueMax,strGainUnit.c_str());
	pStcGainShow->SetWindowText(strGain);
	
	UpdateData(FALSE);

}

//---------------------------------------------------------------------------------
/**
\brief   初始化自动白平衡系数控件

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__InitBalanceWhiteRatio()
{
	//判断是否为黑白相机
	if (!m_bColorFilter)
	{
		return;
	}

	CStatic* pStcBalanceRatioShow    = (CStatic*)GetDlgItem(IDC_STATIC_BALANCE_RATIO);
	CEdit*   pEditBalanceRatioVal    = (CEdit*)GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE);

	//判断控件的有效性
	if (pEditBalanceRatioVal == NULL || pStcBalanceRatioShow == NULL)
	{
		return;
	}

	CString strBalanceRatio      = "";        ///< 当前白平衡系数
	string  strBalanceRatioUnit  = "";        ///< 白平衡系数单位

	//获取白平衡系数当前值
	m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();

	//获取白平衡系数单位
	strBalanceRatioUnit = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetUnit().c_str();

	//设置控件内容
	strBalanceRatio.Format("自动白平衡系数(%.4f~%.4f)%s",m_dBalanceWhiteRatioMin,
		                    m_dBalanceWhiteRatioMax,strBalanceRatioUnit.c_str());
    pStcBalanceRatioShow->SetWindowText(strBalanceRatio);
	
	UpdateData(FALSE);

}
//---------------------------------------------------------------------------------
/**
\brief   更新UI界面

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::__UpdateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bIsOpen && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(m_bIsOpen && m_bTriggerMode);
	GetDlgItem(IDC_COMBO_TRIGGER_SOURCE)->EnableWindow(m_bIsOpen && m_bTriggerSource);
	GetDlgItem(IDC_BTN_SOFTTRIGGER)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE)->EnableWindow(m_bIsOpen && m_bTriggerActive);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_CHECK_SAVE_BMP)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(m_bIsOpen 
		                                                   && m_bBalanceWhiteAuto
														   && m_bColorFilter);

	GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT)->EnableWindow(m_bIsOpen 
		                                                     && m_bBalanceWhiteRatioSelect
															 && m_bColorFilter);

	GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(m_bIsOpen
		                                                   && m_strBalanceWhiteAutoMode == "Off"
		                                                   && m_bBalanceWhiteRatioSelect
														   && m_bColorFilter);


}

void CGxSingleCamDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	//失去焦点
	SetFocus();

	try
	{
		//判断是否已停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销采集回调
			m_objStreamPtr->UnregisterCaptureCallback();

		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
    
	try
	{
		//关闭流对象
		m_objStreamPtr->Close();

	}
	catch(CGalaxyException)
	{
		//do noting
	}
	try
	{
		//关闭设备
		m_objDevicePtr->Close();
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	

	m_bIsOpen = false;
	m_bIsSnap = false;

	//更新界面
	__UpdateUI();

	if (m_pBitmap != NULL)
	{
		//释放为图像显示准备资源
		m_pBitmap->UnPrepareForShowImg();
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}

void CGxSingleCamDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
        try
        {
            //设置Buffer处理模式
            m_objStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
        }
        catch (...)
        {
        }

		//为图像显示准备资源
		m_pBitmap->PrepareForShowImg();

		//注册回调函数
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);

		//开启流层通道
		m_objStreamPtr->StartGrab();

		//发送开采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;

		//更新界面
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());

		//释放为图像显示准备资源
		m_pBitmap->UnPrepareForShowImg();
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());

		//释放为图像显示准备资源
		m_pBitmap->UnPrepareForShowImg();
		return;	
	}
}

void CGxSingleCamDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//发送停采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层通道
		m_objStreamPtr->StopGrab();

		//注销采集回调
		m_objStreamPtr->UnregisterCaptureCallback();
		m_bIsSnap = false;

		//更新界面
		__UpdateUI();
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnBnClickedBtnSofttrigger()
{
	// TODO: Add your control notification handler code here
	try
	{
		//发送软触发命令(在触发模式开启时有效)
		m_objFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;	
	}
}
void CGxSingleCamDlg::OnCbnSelchangeComboTriggerMode()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< 组合框当前值

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_MODE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//将当前功能设置到设备中
		m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue(strCurrentText);
		m_nTriggerModeOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerModeOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerModeOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboTriggerSource()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< 组合框当前值

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_SOURCE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//将当前功能设置到设备中
		m_objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue(strCurrentText);
		m_nTriggerSourceOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerSourceOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerSourceOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboTriggerActive()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";       ///< 组合框当前值

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGER_ACTIVE);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//将当前功能设置到设备中
		m_objFeatureControlPtr->GetEnumFeature("TriggerActivation")->SetValue(strCurrentText);
		m_nTriggerActiveOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nTriggerActiveOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nTriggerActiveOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteAuto()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";      ///< 组合框当前值

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//记录当前白平衡模式
		m_strBalanceWhiteAutoMode = strCurrentText;

		//将当前功能设置到设备中
		m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(strCurrentText);

		if (strCurrentText == "Off")
		{
			GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(TRUE);
			m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
			UpdateData(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(FALSE);
		}

		m_nBalanceWhiteAutoOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nBalanceWhiteAutoOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nBalanceWhiteAutoOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnCbnSelchangeComboBalanceWhiteSelect()
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo =NULL;
	try
	{
		int       nCurSel           = 0;      
		gxstring  strCurrentText    = "";
		CString   strCurrentContent = "";        ///< 组合框当前值

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_SELECT);
		nCurSel = pCombo->GetCurSel();
		pCombo->GetLBText(nCurSel,strCurrentContent);
		strCurrentText = strCurrentContent.GetBuffer(0);

		//将当前功能设置到设备中
		m_objFeatureControlPtr->GetEnumFeature("BalanceRatioSelector")->SetValue(strCurrentText);
		m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
		UpdateData(FALSE);
		m_nBanlanceWhiteRatioOld = nCurSel;
	}
	catch (CGalaxyException& e)
	{
		pCombo->SetCurSel(m_nBanlanceWhiteRatioOld);
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		pCombo->SetCurSel(m_nBanlanceWhiteRatioOld);
		MessageBox(e.what());
		return;	
	}
}

void CGxSingleCamDlg::OnBnClickedCheckSaveBmp()
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
}

void CGxSingleCamDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		//停止OnTimer
		KillTimer(0);

		//判断是否停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层通道
			m_objStreamPtr->StopGrab();

			//注销采集回调
			m_objStreamPtr->UnregisterCaptureCallback();

			m_bIsSnap = false;
		}
	}
	catch (CGalaxyException)
	{

	}
	catch (std::exception)
	{

	}

	try
	{
		//判断是否关闭设备
		if (m_bIsOpen)
		{
			//关闭流对象
			m_objStreamPtr->Close();

			//关闭设备
			m_objDevicePtr->Close();

			m_bIsOpen = false;
		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}

	try
	{
		//释放设备资源
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}

	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}

	//释放为图像显示准备资源
	if (m_pBitmap != NULL)
	{
		m_pBitmap->UnPrepareForShowImg();
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	CDialog::OnClose();
}

void CGxSingleCamDlg::OnEnKillfocusEditShutter()
{
	// TODO: Add your control notification handler code here
	//判断设备句柄是否有效，避免设备掉线时关闭程序出现异常
	if (!m_bIsOpen)
	{
		return;
	}
     
	double dShutterValueOld = m_dEditShutterValue;
	try
	{
		UpdateData(TRUE);

		//判断输入值是否在曝光时间范围内，如果不是则设置与其最近的边界值
		if (m_dEditShutterValue > m_dShutterValueMax )
		{
			m_dEditShutterValue = m_dShutterValueMax;
		}
		if (m_dEditShutterValue < m_dShutterValueMin)
		{
			m_dEditShutterValue = m_dShutterValueMin;
		}

		m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(m_dEditShutterValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditShutterValue = dShutterValueOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditShutterValue = dShutterValueOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);

}

void CGxSingleCamDlg::OnEnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	//判断设备句柄是否有效，避免设备掉线时关闭程序出现异常
	if (!m_bIsOpen)
	{
		return;
	}
    
	double dGainValueOld = m_dEditGainValue;
	try
	{
		UpdateData(TRUE);

		//判断输入值是否在增益值范围内，如果不是则设置与其最近的边界值
		if (m_dEditGainValue > m_dGainValueMax )
		{
			m_dEditGainValue = m_dGainValueMax;
		}
		if (m_dEditGainValue < m_dGainValueMin)
		{
			m_dEditGainValue = m_dGainValueMin;
		}
		m_objFeatureControlPtr->GetFloatFeature("Gain")->SetValue(m_dEditGainValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditGainValue = dGainValueOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditGainValue = dGainValueOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CGxSingleCamDlg::OnEnKillfocusEditBalanceWhiteSerise()
{
	// TODO: Add your control notification handler code here
	//判断设备句柄是否有效，避免设备掉线时关闭程序出现异常
	if (!m_bIsOpen)
	{
		return;
	}

    double dBalanceWhiteRatioOld = m_dEditBalanceRatioValue;
	try
	{
		UpdateData(TRUE);

		//判断输入值是否在自动白平衡系数范围内，如果不是则设置与其最近的边界值
		if (m_dEditBalanceRatioValue > m_dBalanceWhiteRatioMax)
		{
			m_dEditBalanceRatioValue = m_dBalanceWhiteRatioMax;
		}
		if ((m_dEditBalanceRatioValue < m_dBalanceWhiteRatioMin))
		{
			m_dEditBalanceRatioValue = m_dBalanceWhiteRatioMin;
		}
		m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->SetValue(m_dEditBalanceRatioValue);
	}
	catch (CGalaxyException& e)
	{
		m_dEditBalanceRatioValue = dBalanceWhiteRatioOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditBalanceRatioValue = dBalanceWhiteRatioOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CGxSingleCamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		CString strText  = "";    ///> 当前组合框显示的内容
		CComboBox* pComBox = (CComboBox*)GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO);

		//如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
		//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
		if (m_strBalanceWhiteAutoMode == "Once")
		{
			//获取自动白平衡枚举值
			m_strBalanceWhiteAutoMode = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();
			GxIAPICPP::gxstring_vector vecBalanceWhiteAutoEnumCount;
			vecBalanceWhiteAutoEnumCount = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
				                                                 ->GetEnumEntryList();

			//判断白平衡枚举值是否为off
			if (m_strBalanceWhiteAutoMode == "Off")
			{
				for (uint32_t i = 0; i< vecBalanceWhiteAutoEnumCount.size(); i++)
				{
					pComBox->GetLBText(i,strText);
					if (strText == "Off")
					{
						// 选中自动曝光控件中的Off项,即由Once变为Off
						pComBox->SetCurSel(i);
						GetDlgItem(IDC_EDIT_BALANCE_WHITE_SERISE)->EnableWindow(TRUE);
						m_dEditBalanceRatioValue = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
						UpdateData(FALSE);
						break;
					}
				}
			}
		}
	}
	catch (CGalaxyException)
	{
		return;
	}
	catch (std::exception)
	{
		return;
	}
	CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------------
/**
\brief   将图像保存成BMP图片
\param   图像处理参数

\return  无
*/
//----------------------------------------------------------------------------------
void CGxSingleCamDlg::SavePicture(CImageDataPointer& objImageDataPointer)
{
	try
	{
		SYSTEMTIME   sysTime;                   ///< 系统时间
		CString      strFileName = "";          ///< 图像保存路径名称
		CString      strFilePath = "";          ///< 文件路径
		strFilePath = m_strSavePath.c_str();

		//创建保存图像的文件夹
		BOOL bRet = CreateDirectory(strFilePath,NULL);

		//获取当前时间为图像保存的默认名称
		GetLocalTime(&sysTime);
		strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.bmp",strFilePath,  
			                                              sysTime.wYear,
			                                              sysTime.wMonth,
			                                              sysTime.wDay,
			                                              sysTime.wHour,
			                                              sysTime.wMinute,
			                                              sysTime.wSecond,
			                                              sysTime.wMilliseconds);

		//保存图像为BMP
		m_pBitmap->SaveBmp(strFileName.GetBuffer(0));
	}
	catch (std::exception)
	{
		//由于存图是在线程回调中实现的，如果存图抛出异常。采集线程将终止，为了避免线程终止,存图将对异常不做处理
		return;
		
	}

}
BOOL CGxSingleCamDlg::PreTranslateMessage(MSG* pMsg)
{
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
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:
		case IDC_EDIT_BALANCE_WHITE_SERISE:

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
