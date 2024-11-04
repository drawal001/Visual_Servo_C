// CamerParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"
#include "CamerParamDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg dialog


CCamerParamDlg::CCamerParamDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCamerParamDlg::IDD, pParent)
, m_emAWB(GX_BALANCE_WHITE_AUTO_OFF)
, m_emExposureAuto(GX_EXPOSURE_AUTO_OFF)
, m_bIsGrabModeEnable(false)
, m_bIsPixelFormatEnable(false)
, m_bIsReverseXEnable(false)
, m_bIsReverseYEnable(false)
, m_bIsExposureTimeEnable(false)
, m_bIsExposureAutoEnable(false)
, m_bIsExpectedGrayValueEnable(false)
, m_bIsGainAutoEnable(false)
, m_bIsGammaEnableEnable(false)
, m_bIsGammaModeEnable(false)
, m_bIsGammaEnable(false)
, m_bIsAWBModeEnable(false)
, m_bIsAWBSelectorEnable(false)
, m_bIsBalanceRatioEnable(false)
, m_bIsSaturationModeEnable(false)
, m_bIsSaturationEnable(false)
, m_bIsSharpnessModeEnable(false)
, m_bIsSharpnessEnable(false)
, m_bIs2DNoiseReductionModeEnable(false)
, m_bIs3DNoiseReductionModeEnable(false)
, m_bIsHDRModeEnable(false)
, m_bIsHDRLongEnable(false)
, m_bIsHDRShortEnable(false)
, m_bIsHDRMainEnable(false)
, m_bIsBalanceRatioReadable(false)
, m_bIsExposureTimeReadable(false)
{
	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
    m_hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
    m_vecGrabMode.clear();
}


void CCamerParamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCamerParamDlg)

    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamerParamDlg, CDialog)
	//{{AFX_MSG_MAP(CCamerParamDlg)
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_COMBO_GRAB_MODE, OnCbnSelchangeComboGrabMode)
    ON_CBN_SELCHANGE(IDC_COMBO_PIXEL_FORMAT, OnCbnSelchangeComboPixelFormat)
    ON_CBN_SELCHANGE(IDC_COMBO_REVERSE_X, OnCbnSelchangeComboReverseX)
    ON_CBN_SELCHANGE(IDC_COMBO_REVERSE_Y, OnCbnSelchangeComboReverseY)
    ON_CBN_SELCHANGE(IDC_COMBO_GAIN_AUTO, OnCbnSelchangeComboGainAuto)
    ON_CBN_SELCHANGE(IDC_COMBO_GAMMA_ENABLE, OnCbnSelchangeComboGammaEnable)
    ON_CBN_SELCHANGE(IDC_COMBO_GAMMA_MODE, OnCbnSelchangeComboGammaMode)
    ON_CBN_SELCHANGE(IDC_COMBO_AWB_MODE, OnCbnSelchangeComboAwbMode)
    ON_CBN_SELCHANGE(IDC_COMBO_WB_SELECTOR, OnCbnSelchangeComboWbSelector)
    ON_CBN_SELCHANGE(IDC_COMBO_SATURATION_MODE, OnCbnSelchangeComboSaturationMode)
    ON_CBN_SELCHANGE(IDC_COMBO_EXPOSURE_AUTO, OnCbnSelchangeComboExposureAuto)
    ON_CBN_SELCHANGE(IDC_COMBO_SHARPNESS_MODE, OnCbnSelchangeComboSharpnessMode)
    ON_CBN_SELCHANGE(IDC_COMBO_2D_NOISE_REDUCTION_MODE, OnCbnSelchangeCombo2dNoiseReductionMode)
    ON_CBN_SELCHANGE(IDC_COMBO_3D_NOISE_REDUCTION_MODE, OnCbnSelchangeCombo3dNoiseReductionMode)
    ON_CBN_SELCHANGE(IDC_COMBO_HDR_MODE, OnCbnSelchangeComboHdrMode)
    ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, OnEnKillfocusEditGamma)
    ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, OnEnKillfocusEditBalanceRatio)
    ON_EN_KILLFOCUS(IDC_EDIT_SATURATION, OnEnKillfocusEditSaturation)
    ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE_TIME, OnEnKillfocusEditExposureTime)
    ON_EN_KILLFOCUS(IDC_EDIT_EXPECTED_GRAY_VALUE, OnEnKillfocusEditExpectedGrayValue)
    ON_EN_KILLFOCUS(IDC_EDIT_SHARPNESS, OnEnKillfocusEditSharpness)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_LONG_VALUE, OnEnKillfocusEditHdrLongValue)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_SHORT_VALUE, OnEnKillfocusEditHdrShortValue)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_MAIN_VALUE, OnEnKillfocusEditHdrMainValue)
    ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg message handlers

BOOL CCamerParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 设置对话框标题
	CString strTemp = _T("");
	strTemp.Format("%s相机参数", m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	SetWindowText(strTemp);

	// 初始化UI
	__InitUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI控件参数

\return  
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::__InitUI()
{
	// 判断设备句柄有效性
    if(NULL == m_hDevice)
    {
        return;
    }

    // 初始化图像格式控制相关控件
    __InitImageFormatUI();

    // 初始化模拟控制相关控件
    __InitAnalogUI();

    // 初始化颜色转换控制相关控件
    __InitColorTransfUI();

    // 初始化采集控制相关控件
    __InitAcqUI();

    // 初始化图像质量控制相关控件
    __InitImgQualityUI();

    // 初始化HDR控制相关控件
    __InitHDRUI();

    // 刷新界面控件使能状态
    __UpDateUIEnable();

    SetTimer(TIMER_AWB, TIMER_INTERVAL, NULL);
    SetTimer(TIMER_EXPOSURE_AUTO, TIMER_INTERVAL, NULL);
    SetTimer(TIMER_GAIN_AUTO, TIMER_INTERVAL, NULL);
}

//----------------------------------------------------------------------------------
/**
\brief  关闭设备

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnClose() 
{
	// 关闭定时器
    KillTimer(TIMER_AWB);
    KillTimer(TIMER_EXPOSURE_AUTO);
    KillTimer(TIMER_GAIN_AUTO);

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  调节滑动条消息响应函数
\param  nSBCode     [in]        
\param  nPos        [in]        
\param  pScrollBar  [in]        滑动条

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    int64_t             i64Value    = 0;
    const CSliderCtrl   *pSlider    = (CSliderCtrl *)pScrollBar;

    // 获取当前滑动条位置
    i64Value = pSlider->GetPos();

    switch(pScrollBar->GetDlgCtrlID())
    {
    case IDC_SLIDER_GAMMA:
        __SetFloatScroll("Gamma", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_GAMMA));
        break;
    case IDC_SLIDER_BALANCE_RATIO:
        __SetFloatScroll("BalanceRatio", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO));
        break;
    case IDC_SLIDER_SATURATION:
        __SetIntScroll("Saturation", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_SATURATION));
        break;
    case IDC_SLIDER_EXPOSURE_TIME:
        __SetFloatScroll("ExposureTime", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME));
        break;
    case IDC_SLIDER_EXPECTED_GRAY_VALUE:
        __SetIntScroll("ExpectedGrayValue", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE));
        break;
    case IDC_SLIDER_SHARPNESS:
        __SetFloatScroll("Sharpness", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_SHARPNESS));
        break;
    case IDC_SLIDER_HDR_LONG_VALUE:
        __SetIntScroll("HDRTargetLongValue", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_HDR_LONG_VALUE));
        break;
    case IDC_SLIDER_HDR_SHORT_VALUE:
        __SetIntScroll("HDRTargetShortValue", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE));
        break;
    case IDC_SLIDER_HDR_MAIN_VALUE:
        __SetIntScroll("HDRTargetMainValue", i64Value, (CEdit *)GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE));
        break;
    default:
        break;
    }	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


//----------------------------------------------------------------
/**
\brief  分发消息(主要处理回车消息)
\param  pMsg  消息结构体

\return 成功：TRUE  失败：FALSE
*/
//----------------------------------------------------------------
BOOL CCamerParamDlg::PreTranslateMessage(MSG* pMsg) 
{
	const CWnd  *pWnd       = NULL;
	int32_t     i32CtrlID   = 0;           //  保存获取的控件ID

	// 判断是否是键盘回车消息
	if ((WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam))
	{
		// 获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		// 获取控件ID
		i32CtrlID = pWnd->GetDlgCtrlID();

		// 判断ID类型
		switch(i32CtrlID)
		{
		case IDC_EDIT_GAMMA:
		case IDC_EDIT_BALANCE_RATIO:
        case IDC_EDIT_SATURATION:
        case IDC_EDIT_EXPOSURE_TIME:
        case IDC_EDIT_EXPECTED_GRAY_VALUE:
        case IDC_EDIT_SHARPNESS:
        case IDC_EDIT_HDR_LONG_VALUE:
        case IDC_EDIT_HDR_SHORT_VALUE:
        case IDC_EDIT_HDR_MAIN_VALUE:
			//失去焦点
			SetFocus();

			break;
		default:
			break;
		}

		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//----------------------------------------------------------------------------------
/**
\brief  切换显示格式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGrabMode()
{
    __SetGrabMode();
    m_pMainFrame->UpDateImgInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  切换像素格式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboPixelFormat()
{
    __SetEnum("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT));

    // 向主窗口发送改变像素格式命令
    m_pMainFrame->UpDateImgInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  切换水平翻转Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboReverseX()
{
    __SetBool("ReverseX", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_X));
}

//----------------------------------------------------------------------------------
/**
\brief  切换垂直翻转Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboReverseY()
{
    __SetBool("ReverseY", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_Y));

}

//----------------------------------------------------------------------------------
/**
\brief  切换自动增益Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGainAuto()
{
    __SetEnum("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO));
}

//----------------------------------------------------------------------------------
/**
\brief  切换Gamma使能Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGammaEnable()
{
    __SetBool("GammaEnable", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_ENABLE));

}

//----------------------------------------------------------------------------------
/**
\brief  切换Gamma模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGammaMode()
{
    __SetEnum("GammaMode", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_MODE));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_MODE));
}

//----------------------------------------------------------------------------------
/**
\brief  切换自动白平衡模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboAwbMode()
{
    __SetEnum("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE));
}

//----------------------------------------------------------------------------------
/**
\brief  切换白平衡系数Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboWbSelector()
{
    __SetEnum("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR));

}

//----------------------------------------------------------------------------------
/**
\brief  切换饱和度模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboSaturationMode()
{
    __SetEnum("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  切换自动曝光Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboExposureAuto()
{
    __SetEnum("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO));
}

//----------------------------------------------------------------------------------
/**
\brief  切换锐化模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboSharpnessMode()
{
    __SetEnum("SharpnessMode", (CComboBox *)GetDlgItem(IDC_COMBO_SHARPNESS_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  切换2D降噪模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeCombo2dNoiseReductionMode()
{
    __SetEnum("NoiseReductionMode2D", (CComboBox *)GetDlgItem(IDC_COMBO_2D_NOISE_REDUCTION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  切换3D降噪模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeCombo3dNoiseReductionMode()
{
    __SetEnum("NoiseReductionMode3D", (CComboBox *)GetDlgItem(IDC_COMBO_3D_NOISE_REDUCTION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  切换HDR模式Combox控件

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboHdrMode()
{
    __SetEnum("HDRMode", (CComboBox *)GetDlgItem(IDC_COMBO_HDR_MODE));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_HDR_MODE));

}

//--------------------------------------------------------------------
/**
\brief    Gamma值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditGamma()
{
    __SetFloatEdit("Gamma", (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA));
}

//--------------------------------------------------------------------
/**
\brief    白平衡系数的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditBalanceRatio()
{
    __SetFloatEdit("BalanceRatio", (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO));
}

//--------------------------------------------------------------------
/**
\brief    饱和度值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditSaturation()
{
    __SetIntEdit("Saturation", (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION));
}

//--------------------------------------------------------------------
/**
\brief    曝光时间的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditExposureTime()
{
    __SetFloatEdit("ExposureTime", (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME));
}

//--------------------------------------------------------------------
/**
\brief    期望灰度值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditExpectedGrayValue()
{
    __SetIntEdit("ExpectedGrayValue", (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    锐度的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditSharpness()
{
    __SetFloatEdit("Sharpness", (CEdit *)GetDlgItem(IDC_EDIT_SHARPNESS), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHARPNESS));
}

//--------------------------------------------------------------------
/**
\brief    HDR亮场期望值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrLongValue()
{
    __SetIntEdit("HDRTargetLongValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_LONG_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_LONG_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    HDR暗场期望值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrShortValue()
{
    __SetIntEdit("HDRTargetShortValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_SHORT_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    HDR融合期望值的EDIT控件失去焦点的响应函数

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrMainValue()
{
    __SetIntEdit("HDRTargetMainValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_MAIN_VALUE));
}

//----------------------------------------------------------------
/**
\brief  初始化图像格式控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitImageFormatUI(void)
{
    // 初始化采集视频模式下拉框
    __InitComboGrabMode((CComboBox *)GetDlgItem(IDC_COMBO_GRAB_MODE), m_bIsGrabModeEnable);

    // 初始化像素格式下拉框
    __UpDateEnumComboUI("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT), m_bIsPixelFormatEnable);

    // 初始化水平翻转下拉框
    __UpDateBoolComboUI("ReverseX", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_X), m_bIsReverseXEnable);

    // 初始化垂直翻转下拉框
    __UpDateBoolComboUI("ReverseY", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_Y), m_bIsReverseYEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化模拟控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitAnalogUI(void)
{
    bool bIsReadable = false;

    // 初始化自动增益下拉框
    __UpDateEnumComboUI("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO), m_bIsGainAutoEnable);

    // 初始化Gamma使能下拉框
    __UpDateBoolComboUI("GammaEnable", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_ENABLE), m_bIsGammaEnableEnable);

    // 初始化Gamma模式下拉框
    __UpDateEnumComboUI("GammaMode", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_MODE), m_bIsGammaModeEnable);

    // 初始化Gamma值
    __UpDateFloatEditUI("Gamma", (CStatic *)GetDlgItem(IDC_STATIC_GAMMA_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA), bIsReadable, m_bIsGammaEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化颜色转换控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitColorTransfUI(void)
{
    // 初始化自动白平衡模式下拉框
    __UpDateEnumComboUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE), m_bIsAWBModeEnable);

    // 初始化自动白平衡通道选择下拉框
    __UpDateEnumComboUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR), m_bIsAWBSelectorEnable);

    // 初始化白平衡系数
    __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);

    // 初始化饱和度模式下拉框
    __UpDateEnumComboUI("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE), m_bIsSaturationModeEnable);

    // 初始化饱和度值
    __UpDateIntEditUI("Saturation", (CStatic *)GetDlgItem(IDC_STATIC_SATURATION_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION), m_bIsSaturationEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化采集控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitAcqUI(void)
{
    // 初始化曝光时间
    __UpDateFloatEditUI("ExposureTime",(CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME), m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);

    // 初始化自动曝光
    __UpDateEnumComboUI("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO), m_bIsExposureAutoEnable);

    // 初始化期望灰度值
    __UpDateIntEditUI("ExpectedGrayValue", (CStatic *)GetDlgItem(IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE), m_bIsExpectedGrayValueEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化图像质量控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitImgQualityUI(void)
{
    bool bIsReadable = false;

    // 初始化锐化模式下拉框
    __UpDateEnumComboUI("SharpnessMode", (CComboBox *)GetDlgItem(IDC_COMBO_SHARPNESS_MODE), m_bIsSharpnessModeEnable);
    
    // 初始化锐度值
    __UpDateFloatEditUI("Sharpness", (CStatic *)GetDlgItem(IDC_STATIC_SHARPNESS_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SHARPNESS), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHARPNESS), bIsReadable, m_bIsSharpnessEnable);

    // 初始化2D降噪模式
    __UpDateEnumComboUI("NoiseReductionMode2D", (CComboBox *)GetDlgItem(IDC_COMBO_2D_NOISE_REDUCTION_MODE), m_bIs2DNoiseReductionModeEnable);

    // 初始化3D降噪模式
    __UpDateEnumComboUI("NoiseReductionMode3D", (CComboBox *)GetDlgItem(IDC_COMBO_3D_NOISE_REDUCTION_MODE), m_bIs3DNoiseReductionModeEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化HDR控制相关控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitHDRUI(void)
{
    // 初始化HDR模式下拉框
    __UpDateEnumComboUI("HDRMode", (CComboBox *)GetDlgItem(IDC_COMBO_HDR_MODE), m_bIsHDRModeEnable);

    // 初始化HDR亮场期望值
    __UpDateIntEditUI("HDRTargetLongValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_LONG_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_LONG_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_LONG_VALUE), m_bIsHDRLongEnable);

    // 初始化HDR暗场期望值
    __UpDateIntEditUI("HDRTargetShortValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_SHORT_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_SHORT_VALUE), m_bIsHDRShortEnable);

    // 初始化DHR融合期望值
    __UpDateIntEditUI("HDRTargetMainValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_MAIN_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_MAIN_VALUE), m_bIsHDRMainEnable);

}

//----------------------------------------------------------------
/**
\brief  初始化枚举下拉框UI
\param  chFeature [in]        功能码           
\param  pComboBox   [in]        下拉框控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateEnumComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    uint32_t  nEntryNum         = 0;                    // 功能枚举个数
    int32_t   i32CurSel         = 0;
    bool      bIsReadable       = false;
	GX_ENUM_VALUE stEnumValue;
    
    // 判断控件的有效性
    if (NULL == pComboBox)
    {
        return;
    }

    // 判断控件是否使能
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pComboBox->ResetContent();
        return;
    }

    // 获取功能的枚举项个数
    emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
    GX_VERIFY(emStatus);

	nEntryNum = stEnumValue.nSupportedNum;

    // 初始化当前控件的可选项
    pComboBox->ResetContent();
    for (uint32_t i = 0; i < nEntryNum; ++i)
    {	
        pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
        if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
        {
            i32CurSel = i;
        }
    }

    // 设置当前值为界面的显示值
    pComboBox->SetCurSel(i32CurSel);
}

//----------------------------------------------------------------
/**
\brief  初始化Bool下拉框UI
\param  chFeature [in]        功能码           
\param  pComboBox   [in]        下拉框控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateBoolComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsReadable       = false;
    bool      bValue            = false;
    int32_t   i32CurSel         = 0;

    // 判断控件的有效性
    if (NULL == pComboBox)
    {
        return;
    }

    // 判断控件是否使能
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pComboBox->ResetContent();
        return;
    }

    // 获取当前值并显示到界面
    emStatus = GXGetBoolValue(m_hDevice, chFeature, &bValue);
    GX_VERIFY(emStatus);

    if(bValue)
    {
        i32CurSel = 0;
    }
    else
    {
        i32CurSel = 1;
    }

    // 初始化当前控件的可选项
    pComboBox->ResetContent();
    pComboBox->AddString(_T("true"));
    pComboBox->AddString(_T("false"));
    pComboBox->SetCurSel(i32CurSel);

}

//----------------------------------------------------------------
/**
\brief  初始化Float编辑框UI
\param  chFeature [in]        功能码           
\param  pStatic     [in]        取值范围
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [in]        滑动条控件
\param  bIsReadable [out]       是否可读
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateFloatEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsReadable, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    CString   strRange          = "";
    CString   strCurValue       = "";
    double_t  dCurValue         = 0.0;
	GX_FLOAT_VALUE stFloatValue;

    memset(&stFloatValue, 0, sizeof(stFloatValue));

    // 判断控件的有效性
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // 判断控件是否使能
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pStatic->SetWindowText(_T(""));
        pEdit->SetWindowText(_T(""));
        pSlider->SetPos(pSlider->GetRangeMin());
        return;
    }

    // 获取浮点型范围
    emStatus = GXGetFloatValue(m_hDevice, chFeature, &stFloatValue);
    GX_VERIFY(emStatus);

    // 初始化static范围
    strRange.Format("(%.4f~%.4f)%s", stFloatValue.dMin, stFloatValue.dMax, stFloatValue.szUnit);
    pStatic->SetWindowText(strRange);

    // 获取当前值并将当前值更新到界面
	dCurValue = stFloatValue.dCurValue;
    strCurValue.Format("%.4f",dCurValue);
    pEdit->SetWindowText(strCurValue);

    // 初始化pSliderCtrl
    pSlider->SetRange((int32_t)(stFloatValue.dMin * FLOAT_PRECISION), (int32_t)(stFloatValue.dMax * FLOAT_PRECISION), true);
    pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));
}

//----------------------------------------------------------------
/**
\brief  初始化Int编辑框UI
\param  chFeature [in]        功能码           
\param  pStatic     [in]        取值范围
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [in]        滑动条控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateIntEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsReadable       = false;
    CString   strRange          = "";
    CString   strCurValue       = "";
    int64_t   i64CurValue       = 0;
	GX_INT_VALUE stIntValue;

    memset(&stIntValue, 0, sizeof(stIntValue));

    // 判断控件的有效性
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // 判断控件是否使能
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pStatic->SetWindowText(_T(""));
        pEdit->SetWindowText(_T(""));
        pSlider->SetPos(0);
        return;
    }

    // 获取整型范围
    emStatus = GXGetIntValue(m_hDevice, chFeature, &stIntValue);
    GX_VERIFY(emStatus);

    // 初始化static范围
    strRange.Format("(%d~%d)", int32_t(stIntValue.nMin), int32_t(stIntValue.nMax));
    pStatic->SetWindowText(strRange);

    // 获取当前值并将当前值更新到界面
	i64CurValue = stIntValue.nCurValue;
    strCurValue.Format("%d",int32_t(i64CurValue));
    pEdit->SetWindowText(strCurValue);

    // 初始化pSliderCtrl
    pSlider->SetRange((int32_t)stIntValue.nMin, (int32_t)stIntValue.nMax, true);
    pSlider->SetPos((int32_t)i64CurValue);

}


//----------------------------------------------------------------
/**
\brief  初始化采集视频模式
\param  pComboBox   [in]        下拉框控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitComboGrabMode(CComboBox *pComboBox, bool &bIsEnable)
{
    char szCurValue[IMG_FORMAT_SIZE]    = "";
    int32_t  i32CurSel                  = 0;
    uint32_t ui32GrabModeSize           = 0;

    // 判断控件的有效性
    if (NULL == pComboBox)
    {
        return;
    }
    
    __InitGrabMode();

    // 判断设备是否支持更改采集视频模式
    __IsSupportImgSize(bIsEnable);

    // 判断控件是否使能
    if(!bIsEnable)
    {
        pComboBox->ResetContent();
        return;
    }

    __GetGrabModeValue(szCurValue);

    // 初始化当前控件的可选项
    pComboBox->ResetContent();

    ui32GrabModeSize = (uint32_t)m_vecGrabMode.size();
    for (uint32_t i = 0; i < ui32GrabModeSize; ++i)
    {	
        pComboBox->AddString(m_vecGrabMode[i].strImgSize);
        if(0 == strcmp(szCurValue, m_vecGrabMode[i].strImgSize))
        {
            i32CurSel = i;
        }
    }

    pComboBox->SetCurSel(i32CurSel);
}



//----------------------------------------------------------------
/**
\brief  更新UI值
\param  pComboBox   [in]        下拉框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateUI(const CComboBox *pComboBox)
{
    bool bIsReadable = false;

    switch(pComboBox->GetDlgCtrlID())
    {
    case IDC_COMBO_PIXEL_FORMAT:
        __UpDateEnumComboUI("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE), m_bIsSaturationModeEnable);
        __UpDateIntEditUI("Saturation", (CStatic *)GetDlgItem(IDC_STATIC_SATURATION_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION), m_bIsSaturationEnable);
        __UpDateEnumComboUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE), m_bIsAWBModeEnable);
        __UpDateEnumComboUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR), m_bIsAWBSelectorEnable);
        __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
        __UpDateIntEditUI("ExpectedGrayValue", (CStatic *)GetDlgItem(IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE), m_bIsExpectedGrayValueEnable);
        __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
        break;
    case IDC_COMBO_EXPOSURE_AUTO:
        __UpDateFloatEditUI("ExposureTime", (CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME), m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);
        break;
    case IDC_COMBO_GAMMA_MODE:
        __UpDateFloatEditUI("Gamma", (CStatic *)GetDlgItem(IDC_STATIC_GAMMA_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA), bIsReadable, m_bIsGammaEnable);
        break;
    case IDC_COMBO_AWB_MODE:
        __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
        break;
    case IDC_COMBO_WB_SELECTOR:
        __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO),m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
        break;
    case IDC_COMBO_HDR_MODE:
        __UpDateEnumComboUI("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE), m_bIsSaturationModeEnable);
        __UpDateIntEditUI("Saturation", (CStatic *)GetDlgItem(IDC_STATIC_SATURATION_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION), m_bIsSaturationEnable);
        __UpDateEnumComboUI("GammaMode", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_MODE), m_bIsGammaModeEnable);
        __UpDateFloatEditUI("Gamma", (CStatic *)GetDlgItem(IDC_STATIC_GAMMA_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA), bIsReadable, m_bIsGammaEnable);
        __UpDateEnumComboUI("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO), m_bIsGainAutoEnable);
        __UpDateIntEditUI("ExpectedGrayValue", (CStatic *)GetDlgItem(IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE), m_bIsExpectedGrayValueEnable);
        __UpDateEnumComboUI("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO), m_bIsExposureAutoEnable);
        __UpDateFloatEditUI("ExposureTime", (CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME), m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);
        break;
    default:
        break;
    }

    __UpDateUIEnable();

}

//----------------------------------------------------------------
/**
\brief  更新UI使能状态

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateUIEnable(void) const
{
    BOOL bIsDevOpened   = m_pMainFrame->m_pstCam[m_pMainFrame->m_nOperateID].bIsOpen;
    BOOL bIsSnap        = m_pMainFrame->m_pstCam[m_pMainFrame->m_nOperateID].bIsSnap;

    GetDlgItem(IDC_COMBO_GRAB_MODE)->EnableWindow(bIsDevOpened && (!bIsSnap) && m_bIsGrabModeEnable);
    GetDlgItem(IDC_COMBO_PIXEL_FORMAT)->EnableWindow(bIsDevOpened && (!bIsSnap) && m_bIsPixelFormatEnable);
    GetDlgItem(IDC_COMBO_REVERSE_X)->EnableWindow(bIsDevOpened && m_bIsReverseXEnable);
    GetDlgItem(IDC_COMBO_REVERSE_Y)->EnableWindow(bIsDevOpened && m_bIsReverseYEnable);

    GetDlgItem(IDC_COMBO_GAIN_AUTO)->EnableWindow(bIsDevOpened && m_bIsGainAutoEnable);
    GetDlgItem(IDC_COMBO_GAMMA_ENABLE)->EnableWindow(bIsDevOpened && m_bIsGammaEnableEnable);
    GetDlgItem(IDC_COMBO_GAMMA_MODE)->EnableWindow(bIsDevOpened && m_bIsGammaModeEnable);
    GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(bIsDevOpened && m_bIsGammaEnable);
    GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(bIsDevOpened && m_bIsGammaEnable);

    GetDlgItem(IDC_COMBO_AWB_MODE)->EnableWindow(bIsDevOpened && m_bIsAWBModeEnable);
    GetDlgItem(IDC_COMBO_WB_SELECTOR)->EnableWindow(bIsDevOpened && m_bIsAWBSelectorEnable);
    GetDlgItem(IDC_EDIT_BALANCE_RATIO)->EnableWindow(bIsDevOpened && m_bIsBalanceRatioEnable);
    GetDlgItem(IDC_SLIDER_BALANCE_RATIO)->EnableWindow(bIsDevOpened && m_bIsBalanceRatioEnable);
    GetDlgItem(IDC_COMBO_SATURATION_MODE)->EnableWindow(bIsDevOpened && m_bIsSaturationModeEnable);
    GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(bIsDevOpened && m_bIsSaturationEnable);
    GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(bIsDevOpened && m_bIsSaturationEnable);

    GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->EnableWindow(bIsDevOpened && m_bIsExposureTimeEnable);
    GetDlgItem(IDC_SLIDER_EXPOSURE_TIME)->EnableWindow(bIsDevOpened && m_bIsExposureTimeEnable);
    GetDlgItem(IDC_COMBO_EXPOSURE_AUTO)->EnableWindow(bIsDevOpened && m_bIsExposureAutoEnable);
    GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE)->EnableWindow(bIsDevOpened && m_bIsExpectedGrayValueEnable);
    GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE)->EnableWindow(bIsDevOpened && m_bIsExpectedGrayValueEnable);

    GetDlgItem(IDC_COMBO_SHARPNESS_MODE)->EnableWindow(bIsDevOpened && m_bIsSharpnessModeEnable);
    GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(bIsDevOpened && m_bIsSharpnessEnable);
    GetDlgItem(IDC_SLIDER_SHARPNESS)->EnableWindow(bIsDevOpened && m_bIsSharpnessEnable);
    GetDlgItem(IDC_COMBO_2D_NOISE_REDUCTION_MODE)->EnableWindow(bIsDevOpened && m_bIs2DNoiseReductionModeEnable);
    GetDlgItem(IDC_COMBO_3D_NOISE_REDUCTION_MODE)->EnableWindow(bIsDevOpened && m_bIs3DNoiseReductionModeEnable);

    GetDlgItem(IDC_COMBO_HDR_MODE)->EnableWindow(bIsDevOpened && m_bIsHDRModeEnable);
    GetDlgItem(IDC_EDIT_HDR_LONG_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRLongEnable);
    GetDlgItem(IDC_SLIDER_HDR_LONG_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRLongEnable);
    GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRShortEnable);
    GetDlgItem(IDC_SLIDER_HDR_SHORT_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRShortEnable);
    GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRMainEnable);
    GetDlgItem(IDC_SLIDER_HDR_MAIN_VALUE)->EnableWindow(bIsDevOpened && m_bIsHDRMainEnable);

    ((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME))->SetReadOnly(bIsDevOpened && m_bIsExposureTimeReadable && (!m_bIsExposureTimeEnable));
    ((CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO))->SetReadOnly(bIsDevOpened && m_bIsBalanceRatioReadable && (!m_bIsBalanceRatioEnable));

}

//----------------------------------------------------------------
/**
\brief  将选定的下拉框值设置到设备中
\param  chFeature [in]        功能码
\param  pComboBox   [in]        下拉框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetEnum(char* chFeature, const CComboBox *pComboBox)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    int64_t   i64EnumVal    = 0;
    int32_t   i32Index      = 0;

    // 获取当前项对应的枚举
    i32Index = pComboBox->GetCurSel();
    i64EnumVal = (int64_t)pComboBox->GetItemData(i32Index);

    emStatus = GXSetEnumValue(m_hDevice, chFeature, i64EnumVal);
    GX_VERIFY(emStatus);
}

//----------------------------------------------------------------
/**
\brief  将选定的下拉框值设置到设备中
\param  chFeature [in]        功能码
\param  pComboBox   [in]        下拉框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetBool(char* chFeature, const CComboBox *pComboBox)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    bool      bCurValue     = false;
    int32_t   i32Index      = 0;

    // 获取当前项对应的值
    i32Index = pComboBox->GetCurSel();

    if(0 == i32Index)
    {
        bCurValue = true;
    }
    else
    {
        bCurValue = false;
    }

    emStatus = GXSetBoolValue(m_hDevice, chFeature, bCurValue);
    GX_VERIFY(emStatus);
}

//----------------------------------------------------------------
/**
\brief  获取节点是否可读，是否使能
\param  chFeature [in]        功能码
\param  bIsReadable [out]       是否可读
\param  bIsEnable   [out]       是否使能

\return GX_STATUS_SUCCESS       操作成功，没有发生错误
        其它错误情况请参见GX_STATUS_LIST
*/
//----------------------------------------------------------------
GX_STATUS CCamerParamDlg::__GetControlStatus(char* chFeature, bool &bIsReadable, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsImplement      = false;
    bool      bIsWritable       = false;

    // 是否支持该节点
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, chFeature, &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplement = true;
	}

    if(!bIsImplement)
    {
        bIsEnable = false;
        return emStatus;
    }
    else
    {
        // 支持该节点
        // 判断该节点是否可读
		emStatus = GXGetNodeAccessMode(m_hDevice, chFeature, &emAccessMode);
		VERIFY_STATUS_RET(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsReadable = true;
		}
		

        // 判断该节点是否可写
		emStatus = GXGetNodeAccessMode(m_hDevice, chFeature, &emAccessMode);
		VERIFY_STATUS_RET(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsWritable = true;
		}

        // 支持且可写
        if(bIsWritable)
        {
            bIsEnable = true;
        }
        else
        {
            bIsEnable = false;
        }
    }

    return emStatus;
}

//----------------------------------------------------------------
/**
\brief  判断设备支持该分辨率
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__IsSupportImgSize(bool &bIsEnable)
{
    // 若设备最大宽高大于1080P则支持，否则不支持
    if((m_vecGrabMode[0].i64ImgWidth >= m_vecGrabMode[1].i64ImgWidth) && (m_vecGrabMode[0].i64ImgHeight >= m_vecGrabMode[1].i64ImgHeight))
    {
        bIsEnable  = true;
    }
    else
    {
        bIsEnable = false;
    }
}

//----------------------------------------------------------------
/**
\brief  初始化图像采集模式

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitGrabMode(void)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    int64_t   i64WidthMax       = 0;
    int64_t   i64HeightMax      = 0;
    uint32_t  ui32Size          = 0;
    std::vector<char *> vecImgSize;
    std::vector<int64_t> vecImgWidth;
    std::vector<int64_t> vecImgHeight;
    std::vector<int64_t> vecImgOffsetX;
    std::vector<int64_t> vecImgOffsetY;
    GRAB_MODE stGrabMode;
	GX_INT_VALUE stIntValue;

    memset(&stGrabMode, 0, sizeof(stGrabMode));

    // 获取设备的最大宽高
    emStatus = GXGetIntValue(m_hDevice, "WidthMax", &stIntValue);
    GX_VERIFY(emStatus);
	i64WidthMax = stIntValue.nCurValue;

    emStatus = GXGetIntValue(m_hDevice,"HeightMax", &stIntValue);
    GX_VERIFY(emStatus);
	i64HeightMax = stIntValue.nCurValue;

    vecImgSize.clear();
    vecImgWidth.clear();
    vecImgHeight.clear();
    vecImgOffsetX.clear();
    vecImgOffsetY.clear();

    vecImgSize.push_back(IMG_SIZE_OFF);
    vecImgSize.push_back(IMG_SIZE_1080P);
    vecImgSize.push_back(IMG_SIZE_720P);
    vecImgSize.push_back(IMG_SIZE_480P);

    vecImgWidth.push_back(i64WidthMax);
    vecImgWidth.push_back(IMG_WIDTH_1920);
    vecImgWidth.push_back(IMG_WIDTH_1280);
    vecImgWidth.push_back(IMG_WIDTH_640);

    vecImgHeight.push_back(i64HeightMax);
    vecImgHeight.push_back(IMG_HEIGHT_1080);
    vecImgHeight.push_back(IMG_HEIGHT_1024);
    vecImgHeight.push_back(IMG_HEIGHT_480);

    vecImgOffsetX.push_back(0);
    vecImgOffsetX.push_back(IMG_OFFSET_X_0);
    vecImgOffsetX.push_back(IMG_OFFSET_X_320);
    vecImgOffsetX.push_back(IMG_OFFSET_X_640);

    vecImgOffsetY.push_back(0);
    vecImgOffsetY.push_back(IMG_OFFSET_Y_0);
    vecImgOffsetY.push_back(IMG_OFFSET_Y_28);
    vecImgOffsetY.push_back(IMG_OFFSET_Y_300);

    ui32Size = (uint32_t)vecImgSize.size();
    for(uint32_t i = 0; i < ui32Size; ++i)
    {
		strncpy(stGrabMode.strImgSize, vecImgSize[i], IMG_FORMAT_SIZE);

        stGrabMode.i64ImgWidth      = vecImgWidth[i];
        stGrabMode.i64ImgHeight     = vecImgHeight[i];
        stGrabMode.i64ImgOffsetX    = vecImgOffsetX[i];
        stGrabMode.i64ImgOffsetY    = vecImgOffsetY[i];

        m_vecGrabMode.push_back(stGrabMode);
    }

}

//----------------------------------------------------------------
/**
\brief  设置图像采集模式

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetGrabMode(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    int32_t   i32Index      = 0;
    CString   strCurValue   = "";

    // 获取当前项对应的枚举
    const CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_GRAB_MODE);
    i32Index = pBox->GetCurSel();
    pBox->GetLBText(i32Index, strCurValue);

    // 先将图像大小设为最大
	emStatus = GXSetIntValue(m_hDevice, "OffsetX", m_vecGrabMode[0].i64ImgOffsetX);
    GX_VERIFY(emStatus);

	emStatus = GXSetIntValue(m_hDevice, "OffsetY", m_vecGrabMode[0].i64ImgOffsetY);
    GX_VERIFY(emStatus);

	emStatus = GXSetIntValue(m_hDevice, "Width", m_vecGrabMode[0].i64ImgWidth);
    GX_VERIFY(emStatus);

	emStatus = GXSetIntValue(m_hDevice, "Height", m_vecGrabMode[0].i64ImgHeight);
    GX_VERIFY(emStatus);

    std::vector<GRAB_MODE>::iterator itGrabMode = m_vecGrabMode.begin();
    std::vector<GRAB_MODE>::iterator itGrabModeEnd = m_vecGrabMode.end();
    for (; itGrabMode != itGrabModeEnd; ++itGrabMode)
    {
        if(0 ==  strCurValue.Compare(_T(itGrabMode->strImgSize)))
        {
			emStatus = GXSetIntValue(m_hDevice, "Width", itGrabMode->i64ImgWidth);
            GX_VERIFY(emStatus);

			emStatus = GXSetIntValue(m_hDevice, "Height", itGrabMode->i64ImgHeight);
            GX_VERIFY(emStatus);

			emStatus = GXSetIntValue(m_hDevice, "OffsetX", itGrabMode->i64ImgOffsetX);
            GX_VERIFY(emStatus);

			emStatus = GXSetIntValue(m_hDevice, "OffsetY", itGrabMode->i64ImgOffsetY);
            GX_VERIFY(emStatus);

            break;
        }
    }
}

//----------------------------------------------------------------
/**
\brief  Timer

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
    case TIMER_AWB:
        __RefreshAWB();
        break;
    case TIMER_EXPOSURE_AUTO:
        __RefreshExposureAuto();
        break;
    case TIMER_GAIN_AUTO:
        __RefreshGainAuto();
        break;
    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------
/**
\brief  刷新自动白平衡

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshAWB(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strCurValue   = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    int64_t   i64EnumCurVal = 0;
    uint32_t  nEntryNum     = 0;    // 功能枚举个数
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE);
    pComboBox->GetWindowText(strCurValue);

    //如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
    //程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    {
        emStatus = __GetControlStatus("BalanceWhiteAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //获取自动白平衡枚举值
            emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //判断设备自动白平衡是否变为OFF
        if (GX_BALANCE_WHITE_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_BALANCE_WHITE_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // 选中自动白平衡控件中的OFF项,即由ONCE变为OFF
                    pComboBox->SetCurSel(i);
                    emStatus = __GetControlStatus("BalanceRatio", m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
                    VERIFY_RET(emStatus);
                    __UpDateUIEnable();
                    break;
                }
            }
        }
        __RefreshWhiteRatio();
    }
    else if (0 == strCurValue.Compare("Continuous"))
    {
        //获取当前白平衡系数，更新界面
        __RefreshWhiteRatio();
    }
    else
    {
    }
}

//----------------------------------------------------------------
/**
\brief  刷新自动曝光

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshExposureAuto(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strCurValue   = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    int64_t   i64EnumCurVal = 0;
    uint32_t  nEntryNum     = 0;    // 功能枚举个数
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //如果自动曝光方式为Once,设置成功后实际的曝光方式会自动变为off
    //程序通过定时读取设备内部曝光的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    { 
        emStatus = __GetControlStatus("ExposureAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //获取自动曝光枚举值
            emStatus = GXGetEnumValue(m_hDevice, "ExposureAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //判断设备自动曝光是否变为OFF
        if (GX_EXPOSURE_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_EXPOSURE_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // 选中自动曝光控件中的OFF项,即由ONCE变为OFF
                    pComboBox->SetCurSel(i);
                    emStatus = __GetControlStatus("ExposureTime", m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);
                    VERIFY_RET(emStatus);
                    __UpDateUIEnable();
                    break;
                }
            }
        }
        __RefreshExposureTime();
    }
    else if (0 == strCurValue.Compare("Continuous"))
    {
        __RefreshExposureTime();
    }
    else
    {
    }
}

//----------------------------------------------------------------
/**
\brief  刷新自动增益

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshGainAuto(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strCurValue   = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    int64_t   i64EnumCurVal = 0;
    uint32_t  nEntryNum     = 0;    // 功能枚举个数
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //如果自动增益方式为Once,设置成功后实际的增益方式会自动变为off
    //程序通过定时读取设备内部增益的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    { 
        emStatus = __GetControlStatus("GainAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //获取自动增益枚举值
            emStatus = GXGetEnumValue(m_hDevice, "GainAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //判断设备自动增益是否变为OFF
        if (GX_GAIN_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_GAIN_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // 选中自动增益控件中的OFF项,即由ONCE变为OFF
                    pComboBox->SetCurSel(i);
                    break;
                }
            }
        }
    }
}


//----------------------------------------------------------------
/**
\brief  设置FloatEdit
\param  chFeature [in]        节点字符串
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [out]       滑动条控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetFloatEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider)
{
    GX_STATUS   emStatus    = GX_STATUS_SUCCESS;
    CString     strCurValue = _T("");
	std::string strTmp      = "";
	std::string strConvert  = "";
    double_t    dCurValue   = 0.0;
    bool        bIsReadable = false;
    bool        bIsEnable   = false;

    // 获取当前项对应的值
    pEdit->GetWindowText(strCurValue);
	strTmp = (LPCTSTR)strCurValue;

	std::stringstream ssTmp(strTmp);
	std::stringstream ssConvert;

	ssTmp >> dCurValue;
	ssConvert << dCurValue;
	strConvert = ssConvert.str();

    if (0 != strTmp.compare(0, strConvert.size(),strConvert))
	{
		MessageBox(_T("输入字符不合法"));
		return;
	}

    pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));

    // 判断控件是否使能
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(bIsEnable)
    {
        emStatus = GXSetFloatValue(m_hDevice, chFeature, dCurValue);
        GX_VERIFY(emStatus);
    }
}

//----------------------------------------------------------------
/**
\brief  设置IntEdit
\param  chFeature [in]        功能码
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [out]       滑动条控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetIntEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strCurValue   = _T("");
    int64_t   i64CurValue   = 0;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    // 获取当前项对应的值
    pEdit->GetWindowText(strCurValue); 
    i64CurValue = atoi(strCurValue);

    pSlider->SetPos((int32_t)i64CurValue);

    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(bIsEnable)
    {
        emStatus = GXSetIntValue(m_hDevice, chFeature, i64CurValue);
        GX_VERIFY(emStatus);
    }
}

//----------------------------------------------------------------
/**
\brief  设置FloatScroll
\param  chFeature [in]        功能码
\param  i64CurValue [in]        当前值
\param  pEdit       [out]       编辑框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetFloatScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit)
{
    GX_STATUS   emStatus    = GX_STATUS_SUCCESS;
    CString     strCurValue = _T("");

    strCurValue.Format("%.4f",(double_t)i64CurValue / FLOAT_PRECISION);
    pEdit->SetWindowText(strCurValue);

    emStatus = GXSetFloatValue(m_hDevice, chFeature, (double_t)i64CurValue / FLOAT_PRECISION);
    GX_VERIFY(emStatus);
}

//----------------------------------------------------------------
/**
\brief  设置IntScroll
\param  chFeature [in]        功能码
\param  i64CurValue [in]        当前值
\param  pEdit       [out]       编辑框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetIntScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit)
{
    GX_STATUS   emStatus    = GX_STATUS_SUCCESS;
    CString     strCurValue = _T("");

    strCurValue.Format("%d",int32_t(i64CurValue));
    pEdit->SetWindowText(strCurValue); 

    emStatus = GXSetIntValue(m_hDevice, chFeature, i64CurValue);
    GX_VERIFY(emStatus);
}

//----------------------------------------------------------------
/**
\brief  获取视频采集模式
\param  szCurValue  [out]        视频采集模式的显示格式

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__GetGrabModeValue(char* szCurValue)
{
    GX_STATUS   emStatus        = GX_STATUS_SUCCESS;
    int64_t     i64ImgWidth     = 0;    // 图像宽 
    int64_t     i64ImgHeight    = 0;    // 图像高
    int64_t     i64ImgOffsetX   = 0;    // X偏移
    int64_t     i64ImgOffsetY   = 0;    // Y偏移
	GX_INT_VALUE stIntValue;

    // 获取图像宽高，偏移
    emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
    GX_VERIFY(emStatus);
	i64ImgWidth = stIntValue.nCurValue;

    emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
    GX_VERIFY(emStatus);
	i64ImgHeight = stIntValue.nCurValue;

    emStatus = GXGetIntValue(m_hDevice, "OffsetX", &stIntValue);
    GX_VERIFY(emStatus);
	i64ImgOffsetX = stIntValue.nCurValue;

    emStatus = GXGetIntValue(m_hDevice, "OffsetY", &stIntValue);
    GX_VERIFY(emStatus);
	i64ImgOffsetY = stIntValue.nCurValue;

    std::vector<GRAB_MODE>::iterator itGrabMode = m_vecGrabMode.begin();
    std::vector<GRAB_MODE>::iterator itGrabModeEnd = m_vecGrabMode.end();
    for (; itGrabMode != itGrabModeEnd; ++itGrabMode)
    {
        if((i64ImgWidth == itGrabMode->i64ImgWidth)
            &&(i64ImgHeight == itGrabMode->i64ImgHeight)
            &&(i64ImgOffsetX == itGrabMode->i64ImgOffsetX)
            &&(i64ImgOffsetY == itGrabMode->i64ImgOffsetY))
        {
            // 获取当前GrabMode
			strncpy(szCurValue, itGrabMode->strImgSize, sizeof(itGrabMode->strImgSize));

            return;
        }
    }
}

//----------------------------------------------------------------
/**
\brief  刷新白平衡系数编辑框和滑动条

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshWhiteRatio(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strTemp       = "";
    GX_FLOAT_VALUE  stBalanceRatio;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    //获取当前白平衡系数，更新界面
    emStatus = __GetControlStatus("BalanceRatio", bIsReadable, bIsEnable);
    VERIFY_RET(emStatus);
    if(bIsReadable)
    {
        emStatus = GXGetFloatValue(m_hDevice, "BalanceRatio", &stBalanceRatio);
        VERIFY_RET(emStatus);
        strTemp.Format("%.4f", stBalanceRatio.dCurValue);
        SetDlgItemText(IDC_EDIT_BALANCE_RATIO, strTemp);

        CSliderCtrl * pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO);
        pSlider->SetPos((int32_t)(stBalanceRatio.dCurValue * FLOAT_PRECISION));
    }
}

//----------------------------------------------------------------
/**
\brief  刷新曝光时间编辑框和滑动条

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshExposureTime(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    CString   strTemp       = "";
    GX_FLOAT_VALUE  stExposureTime;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    //获取当前曝光时间，更新界面
    emStatus = __GetControlStatus("ExposureTime", bIsReadable, bIsEnable);
    VERIFY_RET(emStatus);
    if(bIsReadable)
    {
        emStatus = GXGetFloatValue(m_hDevice, "ExposureTime", &stExposureTime);
        VERIFY_RET(emStatus);
        strTemp.Format("%.4f", stExposureTime.dCurValue);
        SetDlgItemText(IDC_EDIT_EXPOSURE_TIME, strTemp);

        CSliderCtrl * pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME);
        pSlider->SetPos((int32_t)(stExposureTime.dCurValue * FLOAT_PRECISION));
    }
}

