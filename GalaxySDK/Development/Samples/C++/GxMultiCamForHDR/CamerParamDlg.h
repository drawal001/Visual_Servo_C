#pragma once
#include <iostream>
#include "resource.h"
#include "GalaxyIncludes.h"
#include "DeviceProcess.h"
#include <string>
#include <vector>
#include <sstream>
#include <map>

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

// CCamerParamDlg �Ի���

class CCamerParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CCamerParamDlg)

public:
	CCamerParamDlg(CDeviceProcess* pDeviceProcessCurrent, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCamerParamDlg();

// �Ի�������
	enum { IDD = IDD_CAMERA_PARAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
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
    void __UpDateEnumComboUI(const gxstring &strName, CComboBox *pComboBox, bool &bIsEnable);

    /// ����Bool������UI
    void __UpDateBoolComboUI(const gxstring &strName, CComboBox *pComboBox, bool &bIsEnable);

    /// ����Float�༭��UI
    void __UpDateFloatEditUI(const gxstring &strName, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsReadable, bool &bIsEnable);

    /// ����Int�༭��UI
    void __UpDateIntEditUI(const gxstring &strName, CStatic *pStatic, CEdit *pEdit, CSliderCtrl *pSlider, bool &bIsEnable);

    /// ����UIֵ
    void __UpDateUI(const CComboBox *pComboBox);

    /// ����UIʹ��״̬
    void __UpDateUIEnable(void) const;

    /// ����ö����豸��
    void __SetEnum(const gxstring &strName, const CComboBox *pComboBox);

    /// ����Bool���豸��
    void __SetBool(const gxstring &strName, const CComboBox *pComboBox);

    /// ����ͼ��ɼ�ģʽ
    void __SetGrabMode(void);

    /// ��ȡ�ڵ��Ƿ�ɶ����Ƿ�ʹ��
    void __GetControlStatus(const gxstring &strName, bool &bIsReadable, bool &bIsEnable);

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
    void __SetFloatEdit(const gxstring &strName, const CEdit *pEdit, CSliderCtrl *pSlider);

    /// ����IntEdit
    void __SetIntEdit(const gxstring &strName, const CEdit *pEdit, CSliderCtrl *pSlider);

    /// ����Float������
    void __SetFloatScroll(const gxstring &strName, int64_t i64CurValue, CEdit *pEdit);

    /// ����Int������
    void __SetIntScroll(const gxstring &strName, int64_t i64CurValue, CEdit *pEdit);

    /// ��ȡ��ǰ�ɼ�ģʽ
    void __GetGrabModeValue(char* szCurValue);

private:
    CDeviceProcess             *m_pDeviceProcessCurrent;            ///< ��ǰ�豸���������
    CGXFeatureControlPointer    m_objFeatureControlPtr;             ///< ���Կ�����
    std::vector<GRAB_MODE>      m_vecGrabMode;                      ///< ��Ƶ�ɼ�ģʽ
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
