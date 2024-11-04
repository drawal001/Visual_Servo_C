// GxIOSampleDlg.h : header file
//

#if !defined(AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
#define AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGxIOSampleDlg dialog
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>

using namespace std;

///< �궨���û�IO���ֵ 0:false 1:true
#define OUTPUT_ACTIVATION_FALSE  0
#define OUTPUT_ACTIVATION_TRUE   1

///< �궨�����ŵ�ƽ��תֵ 1:true 0:false
#define LINE_INVERTER_FALSE 0
#define LINE_INVERTER_TRUE  1

class CGxIOSampleDlg : public CDialog
{
// Construction
public:
	CGxIOSampleDlg(CWnd* pParent = NULL);	// standard constructor

public:
	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	/// ��ʼ�����:����Ĭ�ϲ���
	GX_STATUS InitDevice();

	/// ��ʼ��UI����
	void InitUI();

	/// ��ʼ����Ͽ�UI�ؼ�
	void InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplemented);

	/// ��ʼ���������ؿؼ�
	void InitGainUI();

	/// ��ʼ���ع���ؿؼ�
	void InitShutterUI();

	/// ��ʼ���û��Զ������ֵCombox�ؼ�
    void InitOutputValue();

	/// ˢ��UI
	void UpDateUI();

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ��ʼ������ģʽCombox�ؼ�
	void InitTriggerModeUI();

	/// ��ʼ������ԴCombox�ؼ�
	void InitTriggerSourceUI();

    /// ��ʼ����������Combox�ؼ�
	void InitTriggerActivationUI();

	/// ��ʼ������ƿ���Combox�ؼ�
	void InitStrobeSwitchUI();

	/// ��ʼ���û�IO���ģʽCombox�ؼ�
	void InitOutputModeUI();

	/// ��ʼ���û��Զ������ѡ��Combox�ؼ�
	void InitOutputSelectorUI();

	/// ��ʼ������ѡ��ComBox�ؼ�
	void InitLineSelector();

	/// ��ʼ�����ŷ���ComBox�ؼ�
	void InitLineMode();

	/// ��ʼ�����ŵ�ƽ��תComBox�ؼ�
	void InitLineInverter();

	/// ��ʼ���������ԴComBox�ؼ�
	void InitLineSource();

	/// �л�����ѡ��ʱ����������������ŷ��򣬵�ƽ��ת�����Դ���Ӧ��Combo��
	void UpdateLineRelativeInfo();

	/// �л��û��Զ������ѡ��ʱ��������������û��Զ������ֵ���Ӧ��Combo��
	void UpdateUserOutputRelativeInfo();

public:
	GX_DEV_HANDLE           m_hDevice;          ///< �豸���
	GX_DS_HANDLE			m_hStream;		    ///< �豸�����

	GX_FLOAT_VALUE          m_stShutterFloatRange;///< �����ع�ʱ��ķ�Χ
	GX_FLOAT_VALUE          m_stGainFloatRange;   ///< ��������ֵ�ķ�Χ

	int64_t                 m_nTriggerMode;       ///< ��¼��ǰ�Ĵ���ģʽ

    BOOL                    m_bDevOpened;         ///< ��ʶ�豸�Ƿ��Ѵ�
    BOOL                    m_bIsSnap;            ///< ��ʶ�豸�Ƿ��ѿ���

	bool                    m_bTriggerMode;       ///< �Ƿ�֧�ִ���ģʽ
	bool                    m_bTriggerActive;     ///< �Ƿ�֧�ִ�������
	bool                    m_bTriggerSource;     ///< �Ƿ�֧�ִ���Դ
	bool                    m_bOutputSelector;    ///< �Ƿ�֧�����Դ
	bool                    m_bOutputMode;        ///< �Ƿ�֧�����ģʽ
	bool                    m_bOutputValue;       ///< �Ƿ�֧�����ֵ
	bool                    m_bStrobeSwitch;      ///< �Ƿ�֧������ƿ���
	
	bool                    m_bLineSelector;      ///< �Ƿ�֧������ѡ��
	bool                    m_bLineMode;          ///< �Ƿ�֧�����ŷ���
	bool                    m_bLineInverter;      ///< �Ƿ�֧�����ŵ�ƽ��ת
	bool                    m_bLineSource;        ///< �Ƿ�֧���������Դ
	bool                    m_bStrobeSwitchIsUse; ///< ����ƿ����Ƿ����

	// ͼ����ʾ��Դ����
	CWnd                   *m_pWnd;             ///< ͼ����ʾ�ؼ�����ָ��
	CGXBitmap              *m_pBitmap;            ///< ��ʾ����ͼ����

// Dialog Data
	//{{AFX_DATA(CGxIOSampleDlg)
	enum { IDD = IDD_GXIOSAMPLE_DIALOG };
	double	m_dGainValue;
	double	m_dShutterValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxIOSampleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxIOSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnCbnSelchangeComboOutputSource();
	afx_msg void OnCbnSelchangeComboOutputMode();
	afx_msg void OnCbnSelchangeComboOutputValue();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboTriggerSource();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnBnClickedBtnSendSoftTrigger();
	afx_msg void OnCbnSelchangeComboTriggerActivation();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnCbnSelchangeComboStrobeSwitch();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnKillfocusEditShutterValue();
	afx_msg void OnKillfocusEditGainValue();
	afx_msg void OnCbnSelchangeComboLineSelector();
	afx_msg void OnCbnSelchangeComboLineMode();
	afx_msg void OnCbnSelchangeComboLineInverter();
	afx_msg void OnCbnSelchangeComboLineSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXIOSAMPLEDLG_H__26540673_FD9B_4799_889E_E05C3076016C__INCLUDED_)
