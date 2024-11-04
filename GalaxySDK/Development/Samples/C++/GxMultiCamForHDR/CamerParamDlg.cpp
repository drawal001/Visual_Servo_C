// CamerParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CamerParamDlg.h"



// CCamerParamDlg 对话框

IMPLEMENT_DYNAMIC(CCamerParamDlg, CDialog)

CCamerParamDlg::CCamerParamDlg(CDeviceProcess* pDeviceProcessCurrent, CWnd* pParent)
	: CDialog(CCamerParamDlg::IDD, pParent)
    , m_pDeviceProcessCurrent(pDeviceProcessCurrent)
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
    m_vecGrabMode.clear();
}

CCamerParamDlg::~CCamerParamDlg()
{
}

void CCamerParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCamerParamDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_HSCROLL()
    ON_WM_TIMER()
    ON_CBN_SELCHANGE(IDC_COMBO_GRAB_MODE, &CCamerParamDlg::OnCbnSelchangeComboGrabMode)
    ON_CBN_SELCHANGE(IDC_COMBO_PIXEL_FORMAT, &CCamerParamDlg::OnCbnSelchangeComboPixelFormat)
    ON_CBN_SELCHANGE(IDC_COMBO_REVERSE_X, &CCamerParamDlg::OnCbnSelchangeComboReverseX)
    ON_CBN_SELCHANGE(IDC_COMBO_REVERSE_Y, &CCamerParamDlg::OnCbnSelchangeComboReverseY)
    ON_CBN_SELCHANGE(IDC_COMBO_GAIN_AUTO, &CCamerParamDlg::OnCbnSelchangeComboGainAuto)
    ON_CBN_SELCHANGE(IDC_COMBO_GAMMA_ENABLE, &CCamerParamDlg::OnCbnSelchangeComboGammaEnable)
    ON_CBN_SELCHANGE(IDC_COMBO_GAMMA_MODE, &CCamerParamDlg::OnCbnSelchangeComboGammaMode)
    ON_CBN_SELCHANGE(IDC_COMBO_AWB_MODE, &CCamerParamDlg::OnCbnSelchangeComboAwbMode)
    ON_CBN_SELCHANGE(IDC_COMBO_WB_SELECTOR, &CCamerParamDlg::OnCbnSelchangeComboWbSelector)
    ON_CBN_SELCHANGE(IDC_COMBO_SATURATION_MODE, &CCamerParamDlg::OnCbnSelchangeComboSaturationMode)
    ON_CBN_SELCHANGE(IDC_COMBO_EXPOSURE_AUTO, &CCamerParamDlg::OnCbnSelchangeComboExposureAuto)
    ON_CBN_SELCHANGE(IDC_COMBO_SHARPNESS_MODE, &CCamerParamDlg::OnCbnSelchangeComboSharpnessMode)
    ON_CBN_SELCHANGE(IDC_COMBO_2D_NOISE_REDUCTION_MODE, &CCamerParamDlg::OnCbnSelchangeCombo2dNoiseReductionMode)
    ON_CBN_SELCHANGE(IDC_COMBO_3D_NOISE_REDUCTION_MODE, &CCamerParamDlg::OnCbnSelchangeCombo3dNoiseReductionMode)
    ON_CBN_SELCHANGE(IDC_COMBO_HDR_MODE, &CCamerParamDlg::OnCbnSelchangeComboHdrMode)
    ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, &CCamerParamDlg::OnEnKillfocusEditGamma)
    ON_EN_KILLFOCUS(IDC_EDIT_BALANCE_RATIO, &CCamerParamDlg::OnEnKillfocusEditBalanceRatio)
    ON_EN_KILLFOCUS(IDC_EDIT_SATURATION, &CCamerParamDlg::OnEnKillfocusEditSaturation)
    ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE_TIME, &CCamerParamDlg::OnEnKillfocusEditExposureTime)
    ON_EN_KILLFOCUS(IDC_EDIT_EXPECTED_GRAY_VALUE, &CCamerParamDlg::OnEnKillfocusEditExpectedGrayValue)
    ON_EN_KILLFOCUS(IDC_EDIT_SHARPNESS, &CCamerParamDlg::OnEnKillfocusEditSharpness)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_LONG_VALUE, &CCamerParamDlg::OnEnKillfocusEditHdrLongValue)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_SHORT_VALUE, &CCamerParamDlg::OnEnKillfocusEditHdrShortValue)
    ON_EN_KILLFOCUS(IDC_EDIT_HDR_MAIN_VALUE, &CCamerParamDlg::OnEnKillfocusEditHdrMainValue)
END_MESSAGE_MAP()

BOOL CCamerParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CString strTemp = _T("");
    
    // 初始化UI
    __InitUI();

    // 设置对话框标题
    m_pDeviceProcessCurrent->GetDevDisplayName(strTemp);
    SetWindowText(strTemp);

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
    try
    {	
        // 判断设备处理对象有效性
        if(NULL == m_pDeviceProcessCurrent)
        {
            return;
        }
        this->m_objFeatureControlPtr = m_pDeviceProcessCurrent->m_objFeatureControlPtr;

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

    try
    {
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
    try
    {
        __SetGrabMode();
        m_pDeviceProcessCurrent->UpdataImgInfo();
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

    try
    {
        m_pDeviceProcessCurrent->UpdataImgInfo();
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
\param  strName     [in]        功能名           
\param  pComboBox   [in]        下拉框控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateEnumComboUI(const gxstring &strName, CComboBox *pComboBox, bool &bIsEnable)
{
    gxstring_vector vectorEnumEntry;        // 功能的枚举项
    gxstring        strCurEnum  = "";       // 枚举当前项
    int32_t         i32CurSe    = 0;
    bool            bIsReadable = false;
    uint32_t        ui32Size    = 0;

    // 判断控件的有效性
    if (NULL == pComboBox)
    {
        return;
    }

    // 判断控件是否使能
    __GetControlStatus(strName, bIsReadable, bIsEnable);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pComboBox->ResetContent();
        return;
    }

    // 获取功能的枚举项
    vectorEnumEntry = m_objFeatureControlPtr->GetEnumFeature(strName)->GetEnumEntryList();
    strCurEnum = m_objFeatureControlPtr->GetEnumFeature(strName)->GetValue();

    // 初始化当前控件的可选项
    pComboBox->ResetContent();
    ui32Size = (uint32_t)vectorEnumEntry.size();
    for (uint32_t i = 0; i < ui32Size; ++i)
    {
        pComboBox->SetItemData(pComboBox->AddString(vectorEnumEntry[i].c_str()), i);
        if (0 == vectorEnumEntry[i].compare(strCurEnum))
        {
            i32CurSe = i;
        }
    }

    // 设置当前值为界面的显示值
    pComboBox->SetCurSel(i32CurSe);
}

//----------------------------------------------------------------
/**
\brief  初始化Bool下拉框UI
\param  strName     [in]        功能名           
\param  pComboBox   [in]        下拉框控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateBoolComboUI(const gxstring &strName, CComboBox *pComboBox, bool &bIsEnable)
{
    bool      bIsReadable       = false;
    bool      bValue            = false;
    int32_t   i32CurSel         = 0;

    // 判断控件的有效性
    if (NULL == pComboBox)
    {
        return;
    }

    // 判断控件是否使能
    __GetControlStatus(strName, bIsReadable, bIsEnable);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pComboBox->ResetContent();
        return;
    }

    // 获取当前值并显示到界面
    bValue = m_objFeatureControlPtr->GetBoolFeature(strName)->GetValue();
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
\param  strName     [in]        功能名           
\param  pStatic     [in]        取值范围
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [in]        滑动条控件
\param  bIsReadable [out]       是否可读
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateFloatEditUI(const gxstring &strName, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsReadable, bool &bIsEnable)
{
    CString   strRange          = "";
    CString   strCurValue       = "";
    double_t  dCurValue         = 0.0;
    double_t  dMaxValue         = 0.0;
    double_t  dMinValue         = 0.0;
    gxstring  strUnit           = "";

    // 判断控件的有效性
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // 判断控件是否使能
    __GetControlStatus(strName, bIsReadable, bIsEnable);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // 重置该控件
        pStatic->SetWindowText(_T(""));
        pEdit->SetWindowText(_T(""));
        pSlider->SetPos(pSlider->GetRangeMin());
        return;
    }

    dMaxValue = m_objFeatureControlPtr->GetFloatFeature(strName)->GetMax();
    dMinValue = m_objFeatureControlPtr->GetFloatFeature(strName)->GetMin();
    strUnit   = m_objFeatureControlPtr->GetFloatFeature(strName)->GetUnit();

    // 初始化static范围
    strRange.Format("(%.4f~%.4f)%s", dMinValue, dMaxValue, strUnit.c_str());
    pStatic->SetWindowText(strRange);

    // 获取当前值并将当前值更新到界面
    dCurValue = m_objFeatureControlPtr->GetFloatFeature(strName)->GetValue();
    strCurValue.Format("%.4f",dCurValue);
    pEdit->SetWindowText(strCurValue);

    // 初始化pSliderCtrl
    pSlider->SetRange((int32_t)(dMinValue * FLOAT_PRECISION), (int32_t)(dMaxValue * FLOAT_PRECISION), true);
    pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));
}

//----------------------------------------------------------------
/**
\brief  初始化Int编辑框UI
\param  strName     [in]        功能名           
\param  pStatic     [in]        取值范围
\param  pEdit       [in]        当前值编辑框
\param  pSlider     [in]        滑动条控件
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateIntEditUI(const gxstring &strName, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsEnable)
{
    bool      bIsReadable       = false;
    CString   strRange          = "";
    CString   strCurValue       = "";
    int64_t   i64CurValue       = 0;
    int64_t   i64MaxValue       = 0;
    int64_t   i64MinValue       = 0;


    // 判断控件的有效性
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // 判断控件是否使能
    __GetControlStatus(strName, bIsReadable, bIsEnable);
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
    i64MaxValue = m_objFeatureControlPtr->GetIntFeature(strName)->GetMax();
    i64MinValue = m_objFeatureControlPtr->GetIntFeature(strName)->GetMin();

    // 初始化static范围
    strRange.Format("(%d~%d)", int32_t(i64MinValue), int32_t(i64MaxValue));
    pStatic->SetWindowText(strRange);

    // 获取当前值并将当前值更新到界面
    i64CurValue = m_objFeatureControlPtr->GetIntFeature(strName)->GetValue();
    strCurValue.Format("%d",int32_t(i64CurValue));
    pEdit->SetWindowText(strCurValue);

    // 初始化pSliderCtrl
    pSlider->SetRange((int32_t)i64MinValue, (int32_t)i64MaxValue, true);
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
    int32_t i32CurSel                   = 0;
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

    try
    {
        switch(pComboBox->GetDlgCtrlID())
        {
        case IDC_COMBO_PIXEL_FORMAT:
            __UpDateEnumComboUI("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE), m_bIsSaturationModeEnable);
            __UpDateIntEditUI("Saturation", (CStatic *)GetDlgItem(IDC_STATIC_SATURATION_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION), m_bIsSaturationEnable);
            __UpDateEnumComboUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE), m_bIsAWBModeEnable);
            __UpDateEnumComboUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR), m_bIsAWBSelectorEnable);
            __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
            __UpDateIntEditUI("ExpectedGrayValue", (CStatic *)GetDlgItem(IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE), m_bIsExpectedGrayValueEnable);
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

//----------------------------------------------------------------
/**
\brief  更新UI使能状态

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateUIEnable(void) const
{
    BOOL bIsDevOpened   = m_pDeviceProcessCurrent->IsOpen();
    BOOL bIsSnap        = m_pDeviceProcessCurrent->IsSnap();

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
\param  strName     [in]        功能名
\param  pComboBox   [in]        下拉框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetEnum(const gxstring &strName, const CComboBox *pComboBox)
{
    int32_t   i32Index      = 0;
    CString   cstrCurValue  = _T("");
    gxstring  strCurValue   = "";

    try
    {
        // 获取当前项对应的值
        i32Index = pComboBox->GetCurSel();
        pComboBox->GetLBText(i32Index, cstrCurValue);
        strCurValue = cstrCurValue.GetBuffer(0);

        m_objFeatureControlPtr->GetEnumFeature(strName)->SetValue(strCurValue);
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

//----------------------------------------------------------------
/**
\brief  将选定的下拉框值设置到设备中
\param  strName     [in]        功能名
\param  pComboBox   [in]        下拉框控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetBool(const gxstring &strName, const CComboBox *pComboBox)
{
    int32_t   i32Index      = 0;
    CString   cstrCurValue  = _T("");
    bool      bCurValue     = false;

    try
    {
        // 获取当前项对应的值
        i32Index = pComboBox->GetCurSel();
        pComboBox->GetLBText(i32Index, cstrCurValue);

        if(0 == cstrCurValue.Compare("true"))
        {
            bCurValue = true;
        }
        else
        {
            bCurValue = false;
        }

        m_objFeatureControlPtr->GetBoolFeature(strName)->SetValue(bCurValue);
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

//----------------------------------------------------------------
/**
\brief  获取节点是否可读，是否使能
\param  strName     [in]        功能名
\param  bIsReadable [out]       是否可读
\param  bIsEnable   [out]       是否使能

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__GetControlStatus(const gxstring &strName, bool &bIsReadable, bool &bIsEnable)
{
    bool    bIsImplement  = false;
    bool    bIsWritable   = false;

    // 是否支持该节点
    bIsImplement = m_objFeatureControlPtr->IsImplemented(strName);
    if(!bIsImplement)
    {
        bIsEnable = false;
        return;
    }
    else
    {
        // 支持该节点
        // 判断该节点是否可读
        bIsReadable = m_objFeatureControlPtr->IsReadable(strName);
        if(!bIsReadable)
        {
            bIsEnable = false;
            return;
        }

        // 判断该节点是否可写
        bIsWritable = m_objFeatureControlPtr->IsWritable(strName);

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
    int64_t   i64WidthMax       = 0;
    int64_t   i64HeightMax      = 0;
    uint32_t  ui32Size          = 0;
    std::vector<char *> vecImgSize;
    std::vector<int64_t> vecImgWidth;
    std::vector<int64_t> vecImgHeight;
    std::vector<int64_t> vecImgOffsetX;
    std::vector<int64_t> vecImgOffsetY;
    GRAB_MODE stGrabMode;

    memset(&stGrabMode, 0, sizeof(stGrabMode));

    // 获取设备的最大宽高
    i64WidthMax = m_objFeatureControlPtr->GetIntFeature("WidthMax")->GetValue();
    i64HeightMax = m_objFeatureControlPtr->GetIntFeature("HeightMax")->GetValue();

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
        strncpy_s(stGrabMode.strImgSize, vecImgSize[i], IMG_FORMAT_SIZE);
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
    int32_t   i32Index      = 0;
    CString   strCurValue   = "";

    // 获取当前项对应的枚举
    const CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_GRAB_MODE);
    i32Index = pBox->GetCurSel();
    pBox->GetLBText(i32Index, strCurValue);

    // 先将图像大小设为最大
    m_objFeatureControlPtr->GetIntFeature("OffsetX")->SetValue(m_vecGrabMode[0].i64ImgOffsetX);
    m_objFeatureControlPtr->GetIntFeature("OffsetY")->SetValue(m_vecGrabMode[0].i64ImgOffsetY);
    m_objFeatureControlPtr->GetIntFeature("Width")->SetValue(m_vecGrabMode[0].i64ImgWidth);
    m_objFeatureControlPtr->GetIntFeature("Height")->SetValue(m_vecGrabMode[0].i64ImgHeight);

    std::vector<GRAB_MODE>::iterator itGrabMode = m_vecGrabMode.begin();
    std::vector<GRAB_MODE>::iterator itGrabModeEnd = m_vecGrabMode.end();
    for (; itGrabMode != itGrabModeEnd; ++itGrabMode)
    {
        if(0 ==  strCurValue.Compare(_T(itGrabMode->strImgSize)))
        {
            m_objFeatureControlPtr->GetIntFeature("Width")->SetValue(itGrabMode->i64ImgWidth);
            m_objFeatureControlPtr->GetIntFeature("Height")->SetValue(itGrabMode->i64ImgHeight);
            m_objFeatureControlPtr->GetIntFeature("OffsetX")->SetValue(itGrabMode->i64ImgOffsetX);
            m_objFeatureControlPtr->GetIntFeature("OffsetY")->SetValue(itGrabMode->i64ImgOffsetY);

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
    try
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

//----------------------------------------------------------------
/**
\brief  刷新自动白平衡

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__RefreshAWB(void)
{
    CString   strTemp       = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    CString   strCurValue   = _T("");
    gxstring  strEnumCurVal = "";
    uint32_t  ui32EnumSize  = 0;
    gxstring_vector vecAWBEnumCount;
    vecAWBEnumCount.clear();

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE);
    pComboBox->GetWindowText(strCurValue);

    //如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
    //程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    {
        __GetControlStatus("BalanceWhiteAuto", bIsReadable, bIsEnable);
        if(bIsReadable)
        {
            //获取自动白平衡枚举值
            strEnumCurVal = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();
        }

        vecAWBEnumCount = m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetEnumEntryList();

        ui32EnumSize = (uint32_t)vecAWBEnumCount.size();
        //判断设备自动白平衡是否变为OFF
        if (0 == strEnumCurVal.compare("Off"))
        {

            for (uint32_t i = 0; i < ui32EnumSize; ++i)
            {
                pComboBox->GetLBText(i,strTemp);
                if ("Off" == strTemp)
                {
                    // 选中自动白平衡控件中的OFF项,即由ONCE变为OFF
                    pComboBox->SetCurSel(i);
                    __GetControlStatus("BalanceRatio", m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);
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
    CString   strTemp       = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    CString   strCurValue   = _T("");
    gxstring  strEnumCurVal = "";
    uint32_t  ui32EnumSize  = 0;
    gxstring_vector vecExposureModeEnumCount;
    vecExposureModeEnumCount.clear();

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //如果自动曝光方式为Once,设置成功后实际的曝光方式会自动变为off
    //程序通过定时读取设备内部曝光的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    { 
        __GetControlStatus("ExposureAuto", bIsReadable, bIsEnable);
        if(bIsReadable)
        {
            //获取自动曝光枚举值
            strEnumCurVal = m_objFeatureControlPtr->GetEnumFeature("ExposureAuto")->GetValue();
        }

        vecExposureModeEnumCount = m_objFeatureControlPtr->GetEnumFeature("ExposureAuto")->GetEnumEntryList();
        ui32EnumSize = (uint32_t)vecExposureModeEnumCount.size();

        //判断设备自动曝光是否变为OFF
        if (0 == strEnumCurVal.compare("Off"))
        {
            for (uint32_t i = 0; i < ui32EnumSize; ++i)
            {
                pComboBox->GetLBText(i,strTemp);
                if ("Off" == strTemp)
                {
                    // 选中自动曝光控件中的OFF项,即由ONCE变为OFF
                    pComboBox->SetCurSel(i);
                    __GetControlStatus("ExposureTime", m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);
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
    CString   strTemp       = "";
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;
    CString   strCurValue   = _T("");
    gxstring  strEnumCurVal = "";
    uint32_t  ui32EnumSize  = 0;
    gxstring_vector vecGainAuto;
    vecGainAuto.clear();

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //如果自动增益方式为Once,设置成功后实际的增益方式会自动变为off
    //程序通过定时读取设备内部增益的当前值实现UI界面的更新
    if (0 == strCurValue.Compare("Once"))
    { 
        __GetControlStatus("GainAuto", bIsReadable, bIsEnable);
        if(bIsReadable)
        {
            //获取自动增益枚举值
            strEnumCurVal = m_objFeatureControlPtr->GetEnumFeature("GainAuto")->GetValue();
        }

        vecGainAuto = m_objFeatureControlPtr->GetEnumFeature("GainAuto")->GetEnumEntryList();
        ui32EnumSize = (uint32_t)vecGainAuto.size();

        //判断设备自动增益是否变为OFF
        if (0 == strEnumCurVal.compare("Off"))
        {
            for (uint32_t i = 0; i < ui32EnumSize; ++i)
            {
                pComboBox->GetLBText(i,strTemp);
                if ("Off" == strTemp)
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
\param  strName [in]        功能名
\param  pEdit   [in]        当前值编辑框
\param  pSlider [in]        滑动条控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetFloatEdit(const gxstring &strName, const CEdit *pEdit, CSliderCtrl *pSlider)
{
    CString     strCurValue = _T("");
    std::string strTmp      = "";
    std::string strConvert = "";
    double_t    dCurValue   = 0.0;
    bool        bIsReadable = false;
    bool        bIsEnable   = false;

    try
    {
        // 获取当前项对应的值
        pEdit->GetWindowText(strCurValue); 
        strTmp = (LPCTSTR)strCurValue;
        
        std::stringstream ssTmp(strTmp);
        std::stringstream ssConvert;

        ssTmp >> dCurValue;
        ssConvert << dCurValue;
        strConvert = ssConvert.str();

        if (0 != strTmp.compare(0, strConvert.size(), strConvert))
        {
            MessageBox(_T("输入字符不合法"));
            return;
        }

        pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));

        // 判断控件是否使能
        __GetControlStatus(strName, bIsReadable, bIsEnable);
        if(bIsEnable)
        {
            m_objFeatureControlPtr->GetFloatFeature(strName)->SetValue(dCurValue);
        }
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

//----------------------------------------------------------------
/**
\brief  设置IntEdit
\param  strName [in]        功能名
\param  pEdit   [in]        当前值编辑框
\param  pSlider [in]        滑动条控件

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetIntEdit(const gxstring &strName, const CEdit *pEdit, CSliderCtrl *pSlider)
{
    CString   strCurValue   = _T("");
    int64_t   i64CurValue   = 0;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    try
    {
        // 获取当前项对应的值
        pEdit->GetWindowText(strCurValue); 
        i64CurValue = atoi(strCurValue);

        pSlider->SetPos((int32_t)i64CurValue);

        __GetControlStatus(strName, bIsReadable, bIsEnable);
        if(bIsEnable)
        {
            m_objFeatureControlPtr->GetIntFeature(strName)->SetValue(i64CurValue);
        }
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

//----------------------------------------------------------------
/**
\brief  设置Float滑动条
\param  strName     [in]        功能名
\param  i64CurValue [in]        当前值
\param  pEdit       [out]       当前值编辑框

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetFloatScroll(const gxstring &strName, int64_t i64CurValue, CEdit *pEdit)
{
    CString     strCurValue = _T("");

    strCurValue.Format("%.4f",(double_t)i64CurValue / FLOAT_PRECISION);
    pEdit->SetWindowText(strCurValue);

    m_objFeatureControlPtr->GetFloatFeature(strName)->SetValue((double_t)i64CurValue / FLOAT_PRECISION);
}

//----------------------------------------------------------------
/**
\brief  设置Int滑动条
\param  strName     [in]        功能名
\param  i64CurValue [in]        当前值
\param  pEdit       [out]       当前值编辑框

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetIntScroll(const gxstring &strName, int64_t i64CurValue, CEdit *pEdit)
{
    CString     strCurValue = _T("");

    strCurValue.Format("%d",int32_t(i64CurValue));
    pEdit->SetWindowText(strCurValue); 
    m_objFeatureControlPtr->GetIntFeature(strName)->SetValue(i64CurValue);
}

//----------------------------------------------------------------
/**
\brief  获取当前采集模式
\param  szCurValue     [out]        视频采集模式的显示格式

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__GetGrabModeValue(char* szCurValue)
{
    int64_t i64ImgWidth     = 0;    // 图像宽 
    int64_t i64ImgHeight    = 0;    // 图像高
    int64_t i64ImgOffsetX   = 0;    // X偏移
    int64_t i64ImgOffsetY   = 0;    // Y偏移

    // 获取图像宽高，偏移
    i64ImgWidth = m_objFeatureControlPtr->GetIntFeature("Width")->GetValue();
    i64ImgHeight = m_objFeatureControlPtr->GetIntFeature("Height")->GetValue();
    i64ImgOffsetX = m_objFeatureControlPtr->GetIntFeature("OffsetX")->GetValue();
    i64ImgOffsetY = m_objFeatureControlPtr->GetIntFeature("OffsetY")->GetValue();

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
            strncpy_s(szCurValue, IMG_FORMAT_SIZE, itGrabMode->strImgSize, sizeof(itGrabMode->strImgSize));
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
    CString   strTemp       = "";
    double_t  dBalanceRatio = 0.0;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    //获取当前白平衡系数，更新界面
    __GetControlStatus("BalanceRatio", bIsReadable, bIsEnable);
    if(bIsReadable)
    {
        dBalanceRatio = m_objFeatureControlPtr->GetFloatFeature("BalanceRatio")->GetValue();
        strTemp.Format("%.4f", dBalanceRatio);
        SetDlgItemText(IDC_EDIT_BALANCE_RATIO, strTemp);

        CSliderCtrl * pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO);
        pSlider->SetPos((int32_t)(dBalanceRatio * FLOAT_PRECISION));
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
    CString   strTemp       = "";
    double_t  dExposureTime = 0.0;
    bool      bIsReadable   = false;
    bool      bIsEnable     = false;

    //获取当前曝光值，更新界面
    __GetControlStatus("ExposureTime", bIsReadable, bIsEnable);
    if(bIsReadable)
    {
        dExposureTime = m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();
        strTemp.Format("%.4f", dExposureTime);
        SetDlgItemText(IDC_EDIT_EXPOSURE_TIME, strTemp);

        CSliderCtrl * pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME);
        pSlider->SetPos((int32_t)(dExposureTime * FLOAT_PRECISION));
    }
}
