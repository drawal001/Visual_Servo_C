// GxSingleCamColorDlg.h : header file
//

#if !defined(AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
#define AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  WHITE_BALANCE_RNTRY        3         ///< �Զ���ƽ��ö����
#define  DEFAULT_CONTRAST           0         ///< Ĭ�϶Աȶ�ֵ
#define  DEFAULT_GAMMA              1         ///< Ĭ��Gammaֵ
#define  DEFAULT_COLOR_CORRECTION   0         ///< Ĭ����ɫУ��ֵ


/////////////////////////////////////////////////////////////////////////////
// CGxSingleCamDlg dialog

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"

class CGxSingleCamDlg : public CDialog
{
// Construction
public:
	CGxSingleCamDlg(CWnd* pParent = NULL);	// standard constructor
	
public:
	GX_DEV_HANDLE       m_hDevice;              ///< �豸���
	GX_DS_HANDLE	    m_hStream;				///< �豸�����

	int64_t             m_nTriggerMode;         ///< ��¼��ǰ�Ĵ���ģʽ
	int64_t             m_nAutoWhiteBalance;    ///< �Զ���ƽ�⵱ǰѡ��
	int64_t             m_nRatioSelector;       ///< ��¼��ǰ�İ�ƽ��ͨ

	GX_FLOAT_VALUE      m_stShutterFloatRange;  ///< �����ع�ʱ��ķ�Χ
	GX_FLOAT_VALUE      m_stGainFloatRange;     ///< ��������ֵ�ķ�Χ
	GX_FLOAT_VALUE      m_stFloatRange;         ///< �����ƽ��ϵ���ķ�Χ  

	BYTE                *m_pGammaLut;           ///< Gamma���ұ�
	BYTE                *m_pContrastLut;        ///< �ԱȶȲ��ұ�
	int                 m_nLutLength;           ///< ���ұ���
	int64_t             m_nContrast;            ///< �Աȶ�ֵ
	int64_t             m_nColorCorrection;     ///< ��ɫУ��ֵ
	double              m_dGamma;               ///< The value of Gamma

    CString             m_strFilePath;          ///< BMPͼ��Ĭ�ϱ���·��
	BOOL                m_bIsSaveImg;           ///< ��ʶ�Ƿ񱣴�ͼ��
	BOOL                m_bDevOpened;           ///< ��ʶ�Ƿ��Ѵ��豸
	BOOL                m_bIsSnap;              ///< ��ʶ�Ƿ�ʼ�ɼ�
	BOOL                m_bIsImproveImg;        ///< ��ʶ�Ƿ����ͼ����������

	bool                m_bTriggerMode;         ///< �Ƿ�֧�ִ���ģʽ
	bool                m_bTriggerActive;       ///< �Ƿ�֧�ִ�������
	bool                m_bTriggerSource;       ///< �Ƿ�֧�ִ���Դ 
	bool                m_bColorFilter;         ///< ��ʶ�Ƿ�֧�ֲ�ɫ
	bool                m_bBalanceRatioSelector;///< ��ʶ�Ƿ�֧�ְ�ƽ��ͨ��

	CWnd                *m_pWnd;                ///< ��ʾͼ�񴰿�(�ؼ�)ָ��
	CGXBitmap           *m_pBitmap;                ///< ��ʾ����ͼ����
public:

	/// ��ȡ�豸����,��ʼ������ؼ�
	void InitUI();

    /// ˢ��UI����
	void UpDateUI();

	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// ��ȡͼ��������������
	void GetImageImproveParam();

	/// ����ͼ��
	void SaveImage();

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ��ʼ��ö������UI����
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement);

	/// ��ʼ����ƽ��ϵ����ؿؼ�
	void InitWhiteRatioUI();

	/// ˢ����ʾ��ǰ��ƽ��ϵ��
	void RefreshWhiteRatio();

	/// ��ʼ���������ؿؼ�
	void InitGainUI();

	/// ��ʼ���ع���ؿؼ�
	void InitShutterUI();

	/// ��ʼ������ģʽ��ؿؼ�
	void InitTriggerModeUI();

	/// ��ʼ������Դ��ؿؼ�
	void InitTriggerSourceUI();

	/// ��ʼ������������ؿؼ�
	void InitTriggerActivationUI();

	/// ��ʼ����ֵ�㷨��ؿؼ�
	void InitInterpolationAlgorithmUI(CComboBox *pComboBox, bool bIsColorCamera);

	/// ��ʼ�����:����Ĭ�ϲ���
	GX_STATUS InitDevice();

	/// ��ȡ�豸�Ŀ��ߵ�������Ϣ
	GX_STATUS GetDeviceParam();

// Dialog Data
	//{{AFX_DATA(CGxSingleCamDlg)
	enum { IDD = IDD_GXSINGLECAMCOLOR_DIALOG };
	double      m_dBalanceRatio;
	double	m_dGainValue;
	double	m_dShutterValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxSingleCamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxSingleCamDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSofttrigger();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnCbnSelchangeComboWriteBlance();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerActive();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedCheckSaveBmp();
	afx_msg void OnBnClickedCheckImageImprove();
	afx_msg void OnCbnSelchangeComboRatioSelector();
	afx_msg void OnEnKillfocusEditBalanceRatio();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboInterpolationAlgorithm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxSingleCamColorDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
