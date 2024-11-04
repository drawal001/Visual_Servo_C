#if !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
#define AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamerParamDlg.h : header file
//
#include "GxIAPI.h"
#include "MainFrm.h"
#include "afxcmn.h"
#include <string>
#include <vector>
#include <sstream>
#include "afxwin.h"

typedef float float_t;
typedef double double_t;

#define TIMER_INTERVAL      (1000)      ///< ��ʱ�����
#define IMG_FORMAT_SIZE     (64)        ///< ��Ƶ�ɼ�ģʽ����ʾ��ʽ��С

#define IMG_SIZE_OFF        ("Off")     ///< ��ʾ��ʽ��Ĭ��
#define IMG_SIZE_1080P      ("1080P")   ///< ��ʾ��ʽ��1080P
#define IMG_SIZE_720P       ("720P")    ///< ��ʾ��ʽ��720P
#define IMG_SIZE_480P       ("480P")    ///< ��ʾ��ʽ��480P

#define IMG_WIDTH_1920      (1920)      ///< 1080Pͼ���
#define IMG_WIDTH_1280      (1280)      ///< 720Pͼ���
#define IMG_WIDTH_640       (640)       ///< 480Pͼ���

#define IMG_HEIGHT_1080     (1080)      ///< 1080Pͼ���
#define IMG_HEIGHT_1024     (1024)      ///< 720Pͼ���
#define IMG_HEIGHT_480      (480)       ///< 480Pͼ���

#define IMG_OFFSET_X_0      (0)         ///< 1080Pˮƽƫ��
#define IMG_OFFSET_X_320    (320)       ///< 720Pˮƽƫ��
#define IMG_OFFSET_X_640    (640)       ///< 480Pˮƽƫ��

#define IMG_OFFSET_Y_0      (0)         ///< 1080P��ֱƫ��
#define IMG_OFFSET_Y_28     (28)        ///< 720P��ֱƫ��
#define IMG_OFFSET_Y_300    (300)       ///< 480P��ֱƫ��

#define TIMER_AWB           (0)         ///< �Զ���ƽ��Timer
#define TIMER_EXPOSURE_AUTO (1)         ///< �Զ��ع�Timer
#define TIMER_GAIN_AUTO     (2)         ///< �Զ�����Timer
#define FLOAT_PRECISION     (100)       ///< ���㾫�ȣ���ֵ���ܳ���10^4��������ܻ��򳬹�
                                        ///< MFC��������Χ���Ͷ����÷�Χʧ��

/// ��Ƶ�ɼ�ģʽ
typedef struct GRAB_MODE
{
    char        strImgSize[IMG_FORMAT_SIZE];    ///< ��ʾ��ʽ��1080P/720P/480P
    int64_t     i64ImgWidth;                    ///< ͼ��� 
    int64_t     i64ImgHeight;                   ///< ͼ���
    int64_t     i64ImgOffsetX;                  ///< Xƫ��
    int64_t     i64ImgOffsetY;                  ///< Yƫ��
}GRAB_MODE;

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg dialog

class CCamerParamDlg : public CDialog
{
// Construction
public:
	CCamerParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamerParamDlg)
	enum { IDD = IDD_DLG_CAMERA_PARAM };

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamerParamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CCamerParamDlg)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClose();
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

public:
    afx_msg void OnCbnSelchangeComboGrabMode();
    afx_msg void OnCbnSelchangeComboPixelFormat();
    afx_msg void OnCbnSelchangeComboReverseX();
    afx_msg void OnCbnSelchangeComboReverseY();
    afx_msg void OnCbnSelchangeComboGainAuto();
    afx_msg void OnCbnSelchangeComboGammaEnable();
    afx_msg void OnCbnSelchangeComboGammaMode();
    afx_msg void OnCbnSelchangeComboAwbMode();
    afx_msg void OnCbnSelchangeComboWbSelector();
    afx_msg void OnCbnSelchangeComboSaturationMode();
    afx_msg void OnCbnSelchangeComboExposureAuto();
    afx_msg void OnCbnSelchangeComboSharpnessMode();
    afx_msg void OnCbnSelchangeCombo2dNoiseReductionMode();
    afx_msg void OnCbnSelchangeCombo3dNoiseReductionMode();
    afx_msg void OnCbnSelchangeComboHdrMode();
    afx_msg void OnEnKillfocusEditGamma();
    afx_msg void OnEnKillfocusEditBalanceRatio();
    afx_msg void OnEnKillfocusEditSaturation();
    afx_msg void OnEnKillfocusEditExposureTime();
    afx_msg void OnEnKillfocusEditExpectedGrayValue();
    afx_msg void OnEnKillfocusEditSharpness();
    afx_msg void OnEnKillfocusEditHdrLongValue();
    afx_msg void OnEnKillfocusEditHdrShortValue();
    afx_msg void OnEnKillfocusEditHdrMainValue();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    /// ��ʼ��UI����
    void __InitUI();

    /// ��ʼ��ͼ���ʽ������ؿؼ�
    void __InitImageFormatUI(void);

    /// ��ʼ��ģ�������ؿؼ�
    void __InitAnalogUI(void);

    /// ��ʼ����ɫת��������ؿؼ�
    void __InitColorTransfUI(void);

    /// ��ʼ���ɼ�������ؿؼ�
    void __InitAcqUI(void);

    /// ��ʼ��ͼ������������ؿؼ�
    void __InitImgQualityUI(void);

    /// ��ʼ��HDR������ؿؼ�
    void __InitHDRUI(void);

    /// ��ʼ���ɼ���Ƶģʽ
    void __InitComboGrabMode(CComboBox *pComboBox, bool &bIsEnable);

    /// ����ö��������UI
    void __UpDateEnumComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable);

    /// ����Bool������UI
    void __UpDateBoolComboUI(char* chFeature, CComboBox *pComboBox, bool &bIsEnable);

    /// ����Float�༭��UI
    void __UpDateFloatEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsReadable, bool &bIsEnable);

    /// ����Int�༭��UI
    void __UpDateIntEditUI(char* chFeature, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsEnable);

    /// ����UIֵ
    void __UpDateUI(const CComboBox *pComboBox);

    /// ����UIʹ��״̬
    void __UpDateUIEnable(void) const;

    /// ����ö����豸��
    void __SetEnum(char* chFeature, const CComboBox *pComboBox);

    /// ����Bool���豸��
    void __SetBool(char* chFeature, const CComboBox *pComboBox);

    /// ����ͼ��ɼ�ģʽ
    void __SetGrabMode(void);

    /// ��ȡ�ڵ��Ƿ�ɶ����Ƿ�ʹ��
    GX_STATUS __GetControlStatus(char* chFeature, bool &bIsReadable, bool &bIsEnable);

    /// �ж��豸֧�ָ÷ֱ���
    void __IsSupportImgSize(bool &bIsEnable);

    /// ��ʼ��ͼ��ɼ�ģʽ
    void __InitGrabMode(void);

    /// ˢ���Զ���ƽ��
    void __RefreshAWB(void);

    /// ˢ�°�ƽ��ϵ��
    void __RefreshWhiteRatio(void);

    /// ˢ���Զ��ع�
    void __RefreshExposureAuto(void);

    /// ˢ���ع�ʱ��
    void __RefreshExposureTime(void);

    /// ˢ���Զ�����
    void __RefreshGainAuto(void);

    /// ����FloatEdit
    void __SetFloatEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider);
    
    /// ����IntEdit
    void __SetIntEdit(char* chFeature, const CEdit *pEdit, CSliderCtrl *pSlider);

    /// ����Float������
    void __SetFloatScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit);

    /// ����Int������
    void __SetIntScroll(char* chFeature, int64_t i64CurValue, CEdit *pEdit);

    /// ��ȡ��ǰ�ɼ�ģʽ
    void __GetGrabModeValue(char* szCurValue);

private:
    CMainFrame                  *m_pMainFrame;                      ///< �����ڿ��ָ��
    GX_DEV_HANDLE               m_hDevice;                          ///< �豸���
    std::vector<GRAB_MODE>      m_vecGrabMode;                      ///< ��Ƶ�ɼ�ģʽ
    GX_BALANCE_WHITE_AUTO_ENTRY m_emAWB;                            ///< �Զ���ƽ�⵱ǰֵ
    GX_EXPOSURE_AUTO_ENTRY      m_emExposureAuto;                   ///< �Զ��ع⵱ǰֵ
    bool                        m_bIsGrabModeEnable;                ///< �ɼ�ģʽʹ��״̬
    bool                        m_bIsPixelFormatEnable;             ///< ���ظ�ʽʹ��״̬
    bool                        m_bIsReverseXEnable;                ///< ˮƽ��תʹ��״̬
    bool                        m_bIsReverseYEnable;                ///< ��ֱ��תʹ��״̬
    bool                        m_bIsExposureTimeEnable;            ///< �ع�ʱ��ʹ��״̬
    bool                        m_bIsExposureAutoEnable;            ///< �Զ��ع�ʹ��״̬
    bool                        m_bIsExpectedGrayValueEnable;       ///< �����Ҷ�ֵʹ��״̬
    bool                        m_bIsGainAutoEnable;                ///< �Զ�����ʹ��״̬
    bool                        m_bIsGammaEnableEnable;             ///< Gammaʹ��ʹ��״̬
    bool                        m_bIsGammaModeEnable;               ///< Gammaģʽʹ��״̬
    bool                        m_bIsGammaEnable;                   ///< Gammaֵʹ��״̬
    bool                        m_bIsAWBModeEnable;                 ///< �Զ���ƽ��ģʽʹ��״̬
    bool                        m_bIsAWBSelectorEnable;             ///< ��ƽ��ͨ��ѡ��ʹ��״̬
    bool                        m_bIsBalanceRatioEnable;            ///< ��ƽ��ϵ��ʹ��״̬
    bool                        m_bIsSaturationModeEnable;          ///< ���Ͷ�ģʽʹ��״̬
    bool                        m_bIsSaturationEnable;              ///< ���Ͷ�ֵʹ��״̬
    bool                        m_bIsSharpnessModeEnable;           ///< ��ģʽʹ��״̬
    bool                        m_bIsSharpnessEnable;               ///< ��ֵʹ��״̬
    bool                        m_bIs2DNoiseReductionModeEnable;    ///< 2D����ģʽʹ��״̬
    bool                        m_bIs3DNoiseReductionModeEnable;    ///< 3D����ģʽʹ��״̬
    bool                        m_bIsHDRModeEnable;                 ///< HDRģʽʹ��״̬
    bool                        m_bIsHDRLongEnable;                 ///< HDR��������ֵʹ��״̬
    bool                        m_bIsHDRShortEnable;                ///< HDR��������ֵʹ��״̬
    bool                        m_bIsHDRMainEnable;                 ///< HDR�ں�����ֵʹ��״̬
    bool                        m_bIsBalanceRatioReadable;          ///< ��ƽ��ϵ��ֻ��״̬
    bool                        m_bIsExposureTimeReadable;          ///< �ع�ʱ��ֻ��״̬

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERPARAMDLG_H__875A1E64_0402_42AB_A20C_F4E36375A831__INCLUDED_)
