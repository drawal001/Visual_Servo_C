// GxImageProcessDlg.h : header file
//

#if !defined(AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_)
#define AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGxImageProcessDlg dialog
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "StdAfx.h"
#include <vector>
#include <stdio.h>

using namespace std;

#define   CALCCCPARAM_LENGTH               (sizeof(VxInt16) * 9)      ///< ɫ�ʵ������鳤��  
#define   DEFAULT_LIGHTNESS                0                          ///< Ĭ������ֵ
#define   DEFAULT_SHARPEN                  0.1f                       ///< Ĭ����ֵ
#define   DEFAULT_SATURATION               64                         ///< Ĭ�ϱ��Ͷ�ֵ
#define   DEFAULT_CONTRAST                 0                          ///< Ĭ�϶Աȶ�ֵ
#define   DEFAULT_GAMMA                    1                          ///< Ĭ��Gammaֵ
#define   DEFAULT_COLOR_CORRECTION         0                          ///< Ĭ����ɫУ��ֵ
#define   WHITE_BALANCE_RNTRY              3                          ///< �Զ���ƽ��ö����


///< �жϷ���ֵ�궨��
#define VERIFY_STATUS_BREAK(emStatus) \
    if(emStatus != GX_STATUS_SUCCESS) \
    {\
        break;\
    }

class CGxImageProcessDlg : public CDialog
{
// Construction
public:
	explicit CGxImageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGxImageProcessDlg)
	enum { IDD = IDD_GXIMAGEPROCESS_DIALOG };
	CSliderCtrl	m_sliderGamma;
	CSliderCtrl	m_sliderContrast;
	CSliderCtrl	m_sliderLightness;
	CSliderCtrl	m_sliderSaturation;
	CSliderCtrl	m_sliderSharpen;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxImageProcessDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL



	// Generated message map functions
	//{{AFX_MSG(CGxImageProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	afx_msg void OnSelchangeComboAwb();
	afx_msg void OnSelchangeComboAwbLight();
	afx_msg void OnSelchangeComboPixelFormat();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOpenBadPixelCorrect();
	afx_msg void OnOpenDenoise();
	afx_msg void OnOpenAccelerate();
	afx_msg void OnOpenSharpen();
	afx_msg void OnOpenColorCorrect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	/// ��ʼ������
	GX_STATUS InitParam();

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ����ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// ��ʼ��ö������UI����
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement);

	/// ͼ����ز���
	GX_STATUS GetImageParam();

	/// ��ʼ��ͼ�����ܽṹ��
	bool InitImageProcessStruct();

	/// �ͷ�ͼ�������������Դ
	void UnInitImageProcessStruct();

	/// ��ȡͼ��������������
	void GetImageImproveParam();
	/// �ͷ�ͼ��������������ռ�õ��ڴ�
	void ReleaseImageImproveParam();
	
	/// ��ʼ��UI����
	void InitUI();

	/// ����UI����
	void UpdateUI();


private:
    
    /// �豸�Ƿ�����ɫУ��
    GX_STATUS __IsEnableDeviceColorCorrect();

    /// �豸�Ƿ���Gammaʹ��
    GX_STATUS __IsEnableDeviceGamma();

    /// �豸�Ƿ�����
    GX_STATUS __IsEnableDeviceSharpness();

	/// ��ʼ�����б�ؼ�
	void __InitCombo();

	/// ��ʼ����ɫ������ֲ���
	GX_STATUS __InitColorParam();

	/// ��ʼ����ɫͼ����ṹ��
	bool __InitColorStruct();

	/// ��ʼ���ڰ�ͼ����ṹ��
	bool __InitMonoStruct();

	/// �ص������кڰ�ͼ�����㷨
	void OnMonoImageProcess();

	/// �ص������в�ɫͼ�����㷨
	void OnColorImageProcess();


	GX_DEV_HANDLE       m_hDevice;              ///< �豸���
	GX_DS_HANDLE	    m_hStream;			    ///< �豸�����
	void                *m_pBufferRaw;          ///< ԭʼͼ������
	int64_t             m_nPayLoadSize;         ///< ͼ���С
	int64_t             m_nImageHeight;         ///< ԭʼͼ���
    int64_t             m_nImageWidth;          ///< ԭʼͼ���
	int64_t             m_nPixelColorFilter;    ///< Bayer��ʽ
	int64_t             m_i64PixelFormat;       ///< ��ǰ���ظ�ʽ
	bool                m_bColorFilter;         ///< ��ʶ�Ƿ��ǲ�ɫ���ظ�ʽ
	bool                m_bAwbLampHouse;        ///< ��ʾ�Ƿ�֧�ֹ�Դѡ��
	bool                m_bColorCorrection;     ///< ��ʾ�Ƿ�֧����ɫУ��
	bool                m_bIsOpenCC;            ///< ��ʶ��ɫУ����ѡ���Ƿ�ѡ��
	bool                m_bIsOpen;              ///< ����򿪱�־ 
	bool                m_bIsSnap;              ///< ����ɼ���־
	int64_t             m_nGetColorCorrection;  ///< ��ȡ����ɫУ��ֵ
	int64_t             m_nColorCorrection;     ///< ��ɫУ��ֵ
	int64_t             m_nContrast;            ///< �Աȶ�ֵ
	int64_t             m_nAutoWhiteBalance;    ///< �Զ���ƽ���ֵ
	double              m_dGamma;               ///< The value of Gamma 
	int                 m_nLightness;           ///< ����ֵ
	int                 m_nSaturation;          ///< ���Ͷ�ֵ
	int                 m_nPreSaturation;       ///< ���Ͷȸ���ǰ��ֵ
	int64_t             m_nPreContrast;         ///< �Աȶȸ���ǰ��ֵ
	int64_t             m_nPreColorCorrection;  ///< ��ɫ����ǰ��ֵ
	int                 m_nPreLightness;        ///< ���ȸ���ǰ��ֵ
	double              m_dPreGamma;            ///< Gammma����ǰ��ֵ	
	float               m_fSharpen;             ///< ��ֵ
	COLOR_IMG_PROCESS   m_objImageProcess;      ///< ��ɫͼ���������ýṹ��
	bool                m_bUpData;              ///< ����CPU��ǰ��ʾ
	HWND                m_hWndHandle;           ///< ��ʾͼ��Ĵ��ھ��
    bool                m_bEnableGamma;         ///< ��ʶ�豸�Ƿ���Gamma
    bool                m_bEnableSharpness;     ///< ��ʶ�豸�Ƿ�����
	bool                m_bEnableOpen;          ///< ���豸����ʹ�ܱ�־λ
	MONO_IMG_PROCESS    m_objMonoImgProc;       ///< �ڰ�ͼ���������ýṹ��
	bool                m_bSharpness;           ///< ��ʶ�񻯸�ѡ���Ƿ�ѡ��
	bool                m_bAccelerate;          ///< ��ʶ�㷨���ٸ�ѡ���Ƿ�ѡ��
	bool                m_bDenoise;				///< ��ʶ�㷨���ٸ�ѡ���Ƿ�ѡ��
	bool                m_bDefectivePixelCorrect; ///< ��ʶ����У����ѡ���Ƿ�ѡ��
	bool                m_bImageProcess;        ///< ��ʶ�����Ƿ����ڽ���ͼ����
	vector<GX_DEVICE_BASE_INFO>   m_vecDevInfo; ///< �豸������Ϣ�ṹ������
	GX_DEVICE_BASE_INFO           *m_pBaseinfo; ///< �豸������Ϣ�ṹ��ָ��

	CGXBitmap          *m_pBitmap;            ///< ��ʾ����ͼ����
	BYTE                *m_pGammaLut;           ///< Gamma���ұ�
	BYTE                *m_pContrastLut;        ///< �ԱȶȲ��ұ�
	int                 m_nLutLength;           ///< ���ұ���
	// Implementation
	HICON m_hIcon;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_comboChooseDevice;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXIMAGEPROCESSDLG_H__FA4E5FCA_79E2_40DF_9164_D9FDEE73A8DB__INCLUDED_)
