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
	
	// ���öԻ������
	CString strTemp = _T("");
	strTemp.Format("%s�������", m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	SetWindowText(strTemp);

	// ��ʼ��UI
	__InitUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI�ؼ�����

\return  
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::__InitUI()
{
	// �ж��豸�����Ч��
    if(NULL == m_hDevice)
    {
        return;
    }

    // ��ʼ��ͼ���ʽ������ؿؼ�
    __InitImageFormatUI();

    // ��ʼ��ģ�������ؿؼ�
    __InitAnalogUI();

    // ��ʼ����ɫת��������ؿؼ�
    __InitColorTransfUI();

    // ��ʼ���ɼ�������ؿؼ�
    __InitAcqUI();

    // ��ʼ��ͼ������������ؿؼ�
    __InitImgQualityUI();

    // ��ʼ��HDR������ؿؼ�
    __InitHDRUI();

    // ˢ�½���ؼ�ʹ��״̬
    __UpDateUIEnable();

    SetTimer(TIMER_AWB, TIMER_INTERVAL, NULL);
    SetTimer(TIMER_EXPOSURE_AUTO, TIMER_INTERVAL, NULL);
    SetTimer(TIMER_GAIN_AUTO, TIMER_INTERVAL, NULL);
}

//----------------------------------------------------------------------------------
/**
\brief  �ر��豸

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnClose() 
{
	// �رն�ʱ��
    KillTimer(TIMER_AWB);
    KillTimer(TIMER_EXPOSURE_AUTO);
    KillTimer(TIMER_GAIN_AUTO);

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ���ڻ�������Ϣ��Ӧ����
\param  nSBCode     [in]        
\param  nPos        [in]        
\param  pScrollBar  [in]        ������

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    int64_t             i64Value    = 0;
    const CSliderCtrl   *pSlider    = (CSliderCtrl *)pScrollBar;

    // ��ȡ��ǰ������λ��
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
\brief  �ַ���Ϣ(��Ҫ����س���Ϣ)
\param  pMsg  ��Ϣ�ṹ��

\return �ɹ���TRUE  ʧ�ܣ�FALSE
*/
//----------------------------------------------------------------
BOOL CCamerParamDlg::PreTranslateMessage(MSG* pMsg) 
{
	const CWnd  *pWnd       = NULL;
	int32_t     i32CtrlID   = 0;           //  �����ȡ�Ŀؼ�ID

	// �ж��Ƿ��Ǽ��̻س���Ϣ
	if ((WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam))
	{
		// ��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		// ��ȡ�ؼ�ID
		i32CtrlID = pWnd->GetDlgCtrlID();

		// �ж�ID����
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
			//ʧȥ����
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
\brief  �л���ʾ��ʽCombox�ؼ�

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
\brief  �л����ظ�ʽCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboPixelFormat()
{
    __SetEnum("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT));
    __UpDateUI((CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT));

    // �������ڷ��͸ı����ظ�ʽ����
    m_pMainFrame->UpDateImgInfo();
}

//----------------------------------------------------------------------------------
/**
\brief  �л�ˮƽ��תCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboReverseX()
{
    __SetBool("ReverseX", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_X));
}

//----------------------------------------------------------------------------------
/**
\brief  �л���ֱ��תCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboReverseY()
{
    __SetBool("ReverseY", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_Y));

}

//----------------------------------------------------------------------------------
/**
\brief  �л��Զ�����Combox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGainAuto()
{
    __SetEnum("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO));
}

//----------------------------------------------------------------------------------
/**
\brief  �л�Gammaʹ��Combox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboGammaEnable()
{
    __SetBool("GammaEnable", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_ENABLE));

}

//----------------------------------------------------------------------------------
/**
\brief  �л�GammaģʽCombox�ؼ�

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
\brief  �л��Զ���ƽ��ģʽCombox�ؼ�

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
\brief  �л���ƽ��ϵ��Combox�ؼ�

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
\brief  �л����Ͷ�ģʽCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboSaturationMode()
{
    __SetEnum("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  �л��Զ��ع�Combox�ؼ�

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
\brief  �л���ģʽCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeComboSharpnessMode()
{
    __SetEnum("SharpnessMode", (CComboBox *)GetDlgItem(IDC_COMBO_SHARPNESS_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  �л�2D����ģʽCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeCombo2dNoiseReductionMode()
{
    __SetEnum("NoiseReductionMode2D", (CComboBox *)GetDlgItem(IDC_COMBO_2D_NOISE_REDUCTION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  �л�3D����ģʽCombox�ؼ�

\return 
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnCbnSelchangeCombo3dNoiseReductionMode()
{
    __SetEnum("NoiseReductionMode3D", (CComboBox *)GetDlgItem(IDC_COMBO_3D_NOISE_REDUCTION_MODE));

}

//----------------------------------------------------------------------------------
/**
\brief  �л�HDRģʽCombox�ؼ�

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
\brief    Gammaֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditGamma()
{
    __SetFloatEdit("Gamma", (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA));
}

//--------------------------------------------------------------------
/**
\brief    ��ƽ��ϵ����EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditBalanceRatio()
{
    __SetFloatEdit("BalanceRatio", (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO));
}

//--------------------------------------------------------------------
/**
\brief    ���Ͷ�ֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditSaturation()
{
    __SetIntEdit("Saturation", (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION));
}

//--------------------------------------------------------------------
/**
\brief    �ع�ʱ���EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditExposureTime()
{
    __SetFloatEdit("ExposureTime", (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME));
}

//--------------------------------------------------------------------
/**
\brief    �����Ҷ�ֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditExpectedGrayValue()
{
    __SetIntEdit("ExpectedGrayValue", (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    ��ȵ�EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditSharpness()
{
    __SetFloatEdit("Sharpness", (CEdit *)GetDlgItem(IDC_EDIT_SHARPNESS), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHARPNESS));
}

//--------------------------------------------------------------------
/**
\brief    HDR��������ֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrLongValue()
{
    __SetIntEdit("HDRTargetLongValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_LONG_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_LONG_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    HDR��������ֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrShortValue()
{
    __SetIntEdit("HDRTargetShortValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_SHORT_VALUE));
}

//--------------------------------------------------------------------
/**
\brief    HDR�ں�����ֵ��EDIT�ؼ�ʧȥ�������Ӧ����

\return   
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnEnKillfocusEditHdrMainValue()
{
    __SetIntEdit("HDRTargetMainValue", (CEdit *)GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_MAIN_VALUE));
}

//----------------------------------------------------------------
/**
\brief  ��ʼ��ͼ���ʽ������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitImageFormatUI(void)
{
    // ��ʼ���ɼ���Ƶģʽ������
    __InitComboGrabMode((CComboBox *)GetDlgItem(IDC_COMBO_GRAB_MODE), m_bIsGrabModeEnable);

    // ��ʼ�����ظ�ʽ������
    __UpDateEnumComboUI("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT), m_bIsPixelFormatEnable);

    // ��ʼ��ˮƽ��ת������
    __UpDateBoolComboUI("ReverseX", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_X), m_bIsReverseXEnable);

    // ��ʼ����ֱ��ת������
    __UpDateBoolComboUI("ReverseY", (CComboBox *)GetDlgItem(IDC_COMBO_REVERSE_Y), m_bIsReverseYEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ��ģ�������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitAnalogUI(void)
{
    bool bIsReadable = false;

    // ��ʼ���Զ�����������
    __UpDateEnumComboUI("GainAuto", (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO), m_bIsGainAutoEnable);

    // ��ʼ��Gammaʹ��������
    __UpDateBoolComboUI("GammaEnable", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_ENABLE), m_bIsGammaEnableEnable);

    // ��ʼ��Gammaģʽ������
    __UpDateEnumComboUI("GammaMode", (CComboBox *)GetDlgItem(IDC_COMBO_GAMMA_MODE), m_bIsGammaModeEnable);

    // ��ʼ��Gammaֵ
    __UpDateFloatEditUI("Gamma", (CStatic *)GetDlgItem(IDC_STATIC_GAMMA_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_GAMMA), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAMMA), bIsReadable, m_bIsGammaEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ����ɫת��������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitColorTransfUI(void)
{
    // ��ʼ���Զ���ƽ��ģʽ������
    __UpDateEnumComboUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE), m_bIsAWBModeEnable);

    // ��ʼ���Զ���ƽ��ͨ��ѡ��������
    __UpDateEnumComboUI("BalanceRatioSelector", (CComboBox *)GetDlgItem(IDC_COMBO_WB_SELECTOR), m_bIsAWBSelectorEnable);

    // ��ʼ����ƽ��ϵ��
    __UpDateFloatEditUI("BalanceRatio", (CStatic *)GetDlgItem(IDC_STATIC_BALANCE_RATIO_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_BALANCE_RATIO), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BALANCE_RATIO), m_bIsBalanceRatioReadable, m_bIsBalanceRatioEnable);

    // ��ʼ�����Ͷ�ģʽ������
    __UpDateEnumComboUI("SaturationMode", (CComboBox *)GetDlgItem(IDC_COMBO_SATURATION_MODE), m_bIsSaturationModeEnable);

    // ��ʼ�����Ͷ�ֵ
    __UpDateIntEditUI("Saturation", (CStatic *)GetDlgItem(IDC_STATIC_SATURATION_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SATURATION), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SATURATION), m_bIsSaturationEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ���ɼ�������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitAcqUI(void)
{
    // ��ʼ���ع�ʱ��
    __UpDateFloatEditUI("ExposureTime",(CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME), m_bIsExposureTimeReadable, m_bIsExposureTimeEnable);

    // ��ʼ���Զ��ع�
    __UpDateEnumComboUI("ExposureAuto", (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO), m_bIsExposureAutoEnable);

    // ��ʼ�������Ҷ�ֵ
    __UpDateIntEditUI("ExpectedGrayValue", (CStatic *)GetDlgItem(IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_EXPECTED_GRAY_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_EXPECTED_GRAY_VALUE), m_bIsExpectedGrayValueEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ��ͼ������������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitImgQualityUI(void)
{
    bool bIsReadable = false;

    // ��ʼ����ģʽ������
    __UpDateEnumComboUI("SharpnessMode", (CComboBox *)GetDlgItem(IDC_COMBO_SHARPNESS_MODE), m_bIsSharpnessModeEnable);
    
    // ��ʼ�����ֵ
    __UpDateFloatEditUI("Sharpness", (CStatic *)GetDlgItem(IDC_STATIC_SHARPNESS_RANGE), (CEdit *)GetDlgItem(IDC_EDIT_SHARPNESS), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHARPNESS), bIsReadable, m_bIsSharpnessEnable);

    // ��ʼ��2D����ģʽ
    __UpDateEnumComboUI("NoiseReductionMode2D", (CComboBox *)GetDlgItem(IDC_COMBO_2D_NOISE_REDUCTION_MODE), m_bIs2DNoiseReductionModeEnable);

    // ��ʼ��3D����ģʽ
    __UpDateEnumComboUI("NoiseReductionMode3D", (CComboBox *)GetDlgItem(IDC_COMBO_3D_NOISE_REDUCTION_MODE), m_bIs3DNoiseReductionModeEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ��HDR������ؿؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitHDRUI(void)
{
    // ��ʼ��HDRģʽ������
    __UpDateEnumComboUI("HDRMode", (CComboBox *)GetDlgItem(IDC_COMBO_HDR_MODE), m_bIsHDRModeEnable);

    // ��ʼ��HDR��������ֵ
    __UpDateIntEditUI("HDRTargetLongValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_LONG_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_LONG_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_LONG_VALUE), m_bIsHDRLongEnable);

    // ��ʼ��HDR��������ֵ
    __UpDateIntEditUI("HDRTargetShortValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_SHORT_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_SHORT_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_SHORT_VALUE), m_bIsHDRShortEnable);

    // ��ʼ��DHR�ں�����ֵ
    __UpDateIntEditUI("HDRTargetMainValue", (CStatic *)GetDlgItem(IDC_STATIC_HDR_MAIN_VALUE_RANGE),(CEdit *)GetDlgItem(IDC_EDIT_HDR_MAIN_VALUE), (CSliderCtrl *)GetDlgItem(IDC_SLIDER_HDR_MAIN_VALUE), m_bIsHDRMainEnable);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ��ö��������UI
\param  chFeature [in]        ������           
\param  pComboBox   [in]        ������ؼ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateEnumComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    uint32_t  nEntryNum         = 0;                    // ����ö�ٸ���
    int32_t   i32CurSel         = 0;
    bool      bIsReadable       = false;
	GX_ENUM_VALUE stEnumValue;
    
    // �жϿؼ�����Ч��
    if (NULL == pComboBox)
    {
        return;
    }

    // �жϿؼ��Ƿ�ʹ��
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // ���øÿؼ�
        pComboBox->ResetContent();
        return;
    }

    // ��ȡ���ܵ�ö�������
    emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
    GX_VERIFY(emStatus);

	nEntryNum = stEnumValue.nSupportedNum;

    // ��ʼ����ǰ�ؼ��Ŀ�ѡ��
    pComboBox->ResetContent();
    for (uint32_t i = 0; i < nEntryNum; ++i)
    {	
        pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
        if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
        {
            i32CurSel = i;
        }
    }

    // ���õ�ǰֵΪ�������ʾֵ
    pComboBox->SetCurSel(i32CurSel);
}

//----------------------------------------------------------------
/**
\brief  ��ʼ��Bool������UI
\param  chFeature [in]        ������           
\param  pComboBox   [in]        ������ؼ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__UpDateBoolComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsReadable       = false;
    bool      bValue            = false;
    int32_t   i32CurSel         = 0;

    // �жϿؼ�����Ч��
    if (NULL == pComboBox)
    {
        return;
    }

    // �жϿؼ��Ƿ�ʹ��
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // ���øÿؼ�
        pComboBox->ResetContent();
        return;
    }

    // ��ȡ��ǰֵ����ʾ������
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

    // ��ʼ����ǰ�ؼ��Ŀ�ѡ��
    pComboBox->ResetContent();
    pComboBox->AddString(_T("true"));
    pComboBox->AddString(_T("false"));
    pComboBox->SetCurSel(i32CurSel);

}

//----------------------------------------------------------------
/**
\brief  ��ʼ��Float�༭��UI
\param  chFeature [in]        ������           
\param  pStatic     [in]        ȡֵ��Χ
\param  pEdit       [in]        ��ǰֵ�༭��
\param  pSlider     [in]        �������ؼ�
\param  bIsReadable [out]       �Ƿ�ɶ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

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

    // �жϿؼ�����Ч��
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // �жϿؼ��Ƿ�ʹ��
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // ���øÿؼ�
        pStatic->SetWindowText(_T(""));
        pEdit->SetWindowText(_T(""));
        pSlider->SetPos(pSlider->GetRangeMin());
        return;
    }

    // ��ȡ�����ͷ�Χ
    emStatus = GXGetFloatValue(m_hDevice, chFeature, &stFloatValue);
    GX_VERIFY(emStatus);

    // ��ʼ��static��Χ
    strRange.Format("(%.4f~%.4f)%s", stFloatValue.dMin, stFloatValue.dMax, stFloatValue.szUnit);
    pStatic->SetWindowText(strRange);

    // ��ȡ��ǰֵ������ǰֵ���µ�����
	dCurValue = stFloatValue.dCurValue;
    strCurValue.Format("%.4f",dCurValue);
    pEdit->SetWindowText(strCurValue);

    // ��ʼ��pSliderCtrl
    pSlider->SetRange((int32_t)(stFloatValue.dMin * FLOAT_PRECISION), (int32_t)(stFloatValue.dMax * FLOAT_PRECISION), true);
    pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));
}

//----------------------------------------------------------------
/**
\brief  ��ʼ��Int�༭��UI
\param  chFeature [in]        ������           
\param  pStatic     [in]        ȡֵ��Χ
\param  pEdit       [in]        ��ǰֵ�༭��
\param  pSlider     [in]        �������ؼ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

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

    // �жϿؼ�����Ч��
    if ((NULL == pStatic) || (NULL == pEdit) || (NULL == pSlider))
    {
        return;
    }

    // �жϿؼ��Ƿ�ʹ��
    emStatus = __GetControlStatus(chFeature, bIsReadable, bIsEnable);
    GX_VERIFY(emStatus);
    if(!bIsReadable)
    {
        bIsEnable = false;
        // ���øÿؼ�
        pStatic->SetWindowText(_T(""));
        pEdit->SetWindowText(_T(""));
        pSlider->SetPos(0);
        return;
    }

    // ��ȡ���ͷ�Χ
    emStatus = GXGetIntValue(m_hDevice, chFeature, &stIntValue);
    GX_VERIFY(emStatus);

    // ��ʼ��static��Χ
    strRange.Format("(%d~%d)", int32_t(stIntValue.nMin), int32_t(stIntValue.nMax));
    pStatic->SetWindowText(strRange);

    // ��ȡ��ǰֵ������ǰֵ���µ�����
	i64CurValue = stIntValue.nCurValue;
    strCurValue.Format("%d",int32_t(i64CurValue));
    pEdit->SetWindowText(strCurValue);

    // ��ʼ��pSliderCtrl
    pSlider->SetRange((int32_t)stIntValue.nMin, (int32_t)stIntValue.nMax, true);
    pSlider->SetPos((int32_t)i64CurValue);

}


//----------------------------------------------------------------
/**
\brief  ��ʼ���ɼ���Ƶģʽ
\param  pComboBox   [in]        ������ؼ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__InitComboGrabMode(CComboBox *pComboBox, bool &bIsEnable)
{
    char szCurValue[IMG_FORMAT_SIZE]    = "";
    int32_t  i32CurSel                  = 0;
    uint32_t ui32GrabModeSize           = 0;

    // �жϿؼ�����Ч��
    if (NULL == pComboBox)
    {
        return;
    }
    
    __InitGrabMode();

    // �ж��豸�Ƿ�֧�ָ��Ĳɼ���Ƶģʽ
    __IsSupportImgSize(bIsEnable);

    // �жϿؼ��Ƿ�ʹ��
    if(!bIsEnable)
    {
        pComboBox->ResetContent();
        return;
    }

    __GetGrabModeValue(szCurValue);

    // ��ʼ����ǰ�ؼ��Ŀ�ѡ��
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
\brief  ����UIֵ
\param  pComboBox   [in]        ������ؼ�

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
\brief  ����UIʹ��״̬

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
\brief  ��ѡ����������ֵ���õ��豸��
\param  chFeature [in]        ������
\param  pComboBox   [in]        ������ؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetEnum(char* chFeature, const CComboBox *pComboBox)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    int64_t   i64EnumVal    = 0;
    int32_t   i32Index      = 0;

    // ��ȡ��ǰ���Ӧ��ö��
    i32Index = pComboBox->GetCurSel();
    i64EnumVal = (int64_t)pComboBox->GetItemData(i32Index);

    emStatus = GXSetEnumValue(m_hDevice, chFeature, i64EnumVal);
    GX_VERIFY(emStatus);
}

//----------------------------------------------------------------
/**
\brief  ��ѡ����������ֵ���õ��豸��
\param  chFeature [in]        ������
\param  pComboBox   [in]        ������ؼ�

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetBool(char* chFeature, const CComboBox *pComboBox)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    bool      bCurValue     = false;
    int32_t   i32Index      = 0;

    // ��ȡ��ǰ���Ӧ��ֵ
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
\brief  ��ȡ�ڵ��Ƿ�ɶ����Ƿ�ʹ��
\param  chFeature [in]        ������
\param  bIsReadable [out]       �Ƿ�ɶ�
\param  bIsEnable   [out]       �Ƿ�ʹ��

\return GX_STATUS_SUCCESS       �����ɹ���û�з�������
        �������������μ�GX_STATUS_LIST
*/
//----------------------------------------------------------------
GX_STATUS CCamerParamDlg::__GetControlStatus(char* chFeature, bool &bIsReadable, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsImplement      = false;
    bool      bIsWritable       = false;

    // �Ƿ�֧�ָýڵ�
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
        // ֧�ָýڵ�
        // �жϸýڵ��Ƿ�ɶ�
		emStatus = GXGetNodeAccessMode(m_hDevice, chFeature, &emAccessMode);
		VERIFY_STATUS_RET(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsReadable = true;
		}
		

        // �жϸýڵ��Ƿ��д
		emStatus = GXGetNodeAccessMode(m_hDevice, chFeature, &emAccessMode);
		VERIFY_STATUS_RET(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsWritable = true;
		}

        // ֧���ҿ�д
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
\brief  �ж��豸֧�ָ÷ֱ���
\param  bIsEnable   [out]       �Ƿ�ʹ��

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__IsSupportImgSize(bool &bIsEnable)
{
    // ���豸����ߴ���1080P��֧�֣�����֧��
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
\brief  ��ʼ��ͼ��ɼ�ģʽ

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

    // ��ȡ�豸�������
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
\brief  ����ͼ��ɼ�ģʽ

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__SetGrabMode(void)
{
    GX_STATUS emStatus      = GX_STATUS_SUCCESS;
    int32_t   i32Index      = 0;
    CString   strCurValue   = "";

    // ��ȡ��ǰ���Ӧ��ö��
    const CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_GRAB_MODE);
    i32Index = pBox->GetCurSel();
    pBox->GetLBText(i32Index, strCurValue);

    // �Ƚ�ͼ���С��Ϊ���
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
\brief  ˢ���Զ���ƽ��

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
    uint32_t  nEntryNum     = 0;    // ����ö�ٸ���
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB_MODE);
    pComboBox->GetWindowText(strCurValue);

    //����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
    //����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
    if (0 == strCurValue.Compare("Once"))
    {
        emStatus = __GetControlStatus("BalanceWhiteAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //��ȡ�Զ���ƽ��ö��ֵ
            emStatus = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //�ж��豸�Զ���ƽ���Ƿ��ΪOFF
        if (GX_BALANCE_WHITE_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_BALANCE_WHITE_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // ѡ���Զ���ƽ��ؼ��е�OFF��,����ONCE��ΪOFF
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
        //��ȡ��ǰ��ƽ��ϵ�������½���
        __RefreshWhiteRatio();
    }
    else
    {
    }
}

//----------------------------------------------------------------
/**
\brief  ˢ���Զ��ع�

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
    uint32_t  nEntryNum     = 0;    // ����ö�ٸ���
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_EXPOSURE_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //����Զ��عⷽʽΪOnce,���óɹ���ʵ�ʵ��عⷽʽ���Զ���Ϊoff
    //����ͨ����ʱ��ȡ�豸�ڲ��ع�ĵ�ǰֵʵ��UI����ĸ���
    if (0 == strCurValue.Compare("Once"))
    { 
        emStatus = __GetControlStatus("ExposureAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //��ȡ�Զ��ع�ö��ֵ
            emStatus = GXGetEnumValue(m_hDevice, "ExposureAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //�ж��豸�Զ��ع��Ƿ��ΪOFF
        if (GX_EXPOSURE_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_EXPOSURE_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
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
\brief  ˢ���Զ�����

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
    uint32_t  nEntryNum     = 0;    // ����ö�ٸ���
	GX_ENUM_VALUE stEnumValue;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_GAIN_AUTO);
    pComboBox->GetWindowText(strCurValue);

    //����Զ����淽ʽΪOnce,���óɹ���ʵ�ʵ����淽ʽ���Զ���Ϊoff
    //����ͨ����ʱ��ȡ�豸�ڲ�����ĵ�ǰֵʵ��UI����ĸ���
    if (0 == strCurValue.Compare("Once"))
    { 
        emStatus = __GetControlStatus("GainAuto", bIsReadable, bIsEnable);
        VERIFY_RET(emStatus);
        if(bIsReadable)
        {
            //��ȡ�Զ�����ö��ֵ
            emStatus = GXGetEnumValue(m_hDevice, "GainAuto", &stEnumValue);
            VERIFY_RET(emStatus);
			i64EnumCurVal = stEnumValue.stCurValue.nCurValue;
        }

		nEntryNum = stEnumValue.nSupportedNum;

        //�ж��豸�Զ������Ƿ��ΪOFF
        if (GX_GAIN_AUTO_OFF == i64EnumCurVal)
        {
            for (uint32_t i = 0; i < nEntryNum; ++i)
            {
                if (GX_GAIN_AUTO_OFF == (int64_t)pComboBox->GetItemData(i))
                {
                    // ѡ���Զ�����ؼ��е�OFF��,����ONCE��ΪOFF
                    pComboBox->SetCurSel(i);
                    break;
                }
            }
        }
    }
}


//----------------------------------------------------------------
/**
\brief  ����FloatEdit
\param  chFeature [in]        �ڵ��ַ���
\param  pEdit       [in]        ��ǰֵ�༭��
\param  pSlider     [out]       �������ؼ�

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

    // ��ȡ��ǰ���Ӧ��ֵ
    pEdit->GetWindowText(strCurValue);
	strTmp = (LPCTSTR)strCurValue;

	std::stringstream ssTmp(strTmp);
	std::stringstream ssConvert;

	ssTmp >> dCurValue;
	ssConvert << dCurValue;
	strConvert = ssConvert.str();

    if (0 != strTmp.compare(0, strConvert.size(),strConvert))
	{
		MessageBox(_T("�����ַ����Ϸ�"));
		return;
	}

    pSlider->SetPos((int32_t)(dCurValue * FLOAT_PRECISION));

    // �жϿؼ��Ƿ�ʹ��
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
\brief  ����IntEdit
\param  chFeature [in]        ������
\param  pEdit       [in]        ��ǰֵ�༭��
\param  pSlider     [out]       �������ؼ�

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

    // ��ȡ��ǰ���Ӧ��ֵ
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
\brief  ����FloatScroll
\param  chFeature [in]        ������
\param  i64CurValue [in]        ��ǰֵ
\param  pEdit       [out]       �༭��ؼ�

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
\brief  ����IntScroll
\param  chFeature [in]        ������
\param  i64CurValue [in]        ��ǰֵ
\param  pEdit       [out]       �༭��ؼ�

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
\brief  ��ȡ��Ƶ�ɼ�ģʽ
\param  szCurValue  [out]        ��Ƶ�ɼ�ģʽ����ʾ��ʽ

\return
*/
//----------------------------------------------------------------
void CCamerParamDlg::__GetGrabModeValue(char* szCurValue)
{
    GX_STATUS   emStatus        = GX_STATUS_SUCCESS;
    int64_t     i64ImgWidth     = 0;    // ͼ��� 
    int64_t     i64ImgHeight    = 0;    // ͼ���
    int64_t     i64ImgOffsetX   = 0;    // Xƫ��
    int64_t     i64ImgOffsetY   = 0;    // Yƫ��
	GX_INT_VALUE stIntValue;

    // ��ȡͼ���ߣ�ƫ��
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
            // ��ȡ��ǰGrabMode
			strncpy(szCurValue, itGrabMode->strImgSize, sizeof(itGrabMode->strImgSize));

            return;
        }
    }
}

//----------------------------------------------------------------
/**
\brief  ˢ�°�ƽ��ϵ���༭��ͻ�����

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

    //��ȡ��ǰ��ƽ��ϵ�������½���
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
\brief  ˢ���ع�ʱ��༭��ͻ�����

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

    //��ȡ��ǰ�ع�ʱ�䣬���½���
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

