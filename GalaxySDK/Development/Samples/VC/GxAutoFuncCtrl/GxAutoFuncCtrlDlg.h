// GxAutoFuncCtrlDlg.h : header file
//

#if !defined(AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_)
#define AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "GxBitmap.h"
#include <string>
#include <iostream>

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CGxAutoFuncCtrlDlg dialog

class CGxAutoFuncCtrlDlg : public CDialog
{
// Construction
public:
	CGxAutoFuncCtrlDlg(CWnd* pParent = NULL);	// standard constructor
public:
    /// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// �豸��ʼ��
	GX_STATUS InitDevice();

	/// ���¸���Ȥ�����ȡֵ��Χ
	void UpdateROIRange();

	/// ��ȡö������Ϣ����ʼ�����Ӧ��Combox��
	void InitEnumUI(char* strFeatureName, CComboBox *pComboBox, bool bIsImplemented);

	/// ��ʼ��UI����
	void InitUI();

	/// ��ʼ���ع���صĿؼ�
	void InitShutterUI();

	/// ��ʼ��������صĿؼ�
	void InitGainUI();

	/// ��ʼ�������Ҷ�ֵ��صĿؼ�
	void InitGrayUI();

	/// ��ʼ���Զ�����Combox�ؼ�
	void InitAutoGainUI();

	/// ��ʼ������Ȥ����Ŀ��ߡ�X�����Y�����ֵ
	void InitROIUI();

	/// ��ʼ���Զ��ع�Combox�ؼ�
	void InitAutoShutterUI();

	/// ��ʼ��2A���ջ���Combox�ؼ�
	void InitLightEnvironmentUI();
	
	/// ˢ�½���
	void UpDateUI();

	/// ˢ����ʾ�ع�����浱ǰֵ
	void RefreshCurValue();

	///��ȡ������Ϣ
	void ShowErrorString(GX_STATUS emErrorStatus);

// Dialog Data
	//{{AFX_DATA(CGxAutoFuncCtrlDlg)
	enum { IDD = IDD_GXAUTOFUNCCTRL_DIALOG };
	int		m_nGray;               ///< �����Ҷ�ֵ
	int		m_nRoiX;               ///< 2A����Ȥ����X����
	int		m_nRoiY;               ///< 2A����Ȥ����Y����
	int		m_nRoiH;               ///< 2A����Ȥ�����
	int		m_nRoiW;               ///< 2A����Ȥ�����
	double	m_dAutoGainMin;        ///< �Զ�������Сֵ
	double  m_dAutoGainMax;        ///< �Զ��������ֵ
	double	m_dAutoShutterMin;     ///< �Զ��ع���Сֵ
	double	m_dAutoShutterMax;     ///< �Զ��ع����ֵ
	double	m_dShutterValue;       ///< �ع�ֵ
	double	m_dGainValue;          ///< ����ֵ       
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxAutoFuncCtrlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxAutoFuncCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnCbnSelchangeComboLight();
	afx_msg void OnCbnSelchangeComboAutoGain();
	afx_msg void OnCbnSelchangeComboAutoShutter();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditRoiX();
	afx_msg void OnKillfocusEditRioY();
	afx_msg void OnKillfocusEditRoiW();
	afx_msg void OnKillfocusEditRoiH();
	afx_msg void OnKillfocusEditAutoGainMax();
	afx_msg void OnKillfocusEditAutoGainMin();
	afx_msg void OnKillfocusEditAutoShutterMax();
	afx_msg void OnKillfocusEditAutoShutterMin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnKillfocusEditCurShutter();
	afx_msg void OnKillfocusEditCurGain();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()    

public:
	GX_DEV_HANDLE                  m_hDevice;                ///< �豸���
	GX_DS_HANDLE			       m_hStream;				 ///< �豸�����

	GX_FLOAT_VALUE			       m_stShutterFloatRange;	 ///< �����ع�ʱ�䷶Χ
	GX_FLOAT_VALUE                 m_stGainFloatRange;       ///< ��������ֵ��Χ           

	BOOL                           m_bDevOpened;             ///< ��ʶ�豸�Ƿ��Ѵ�
	BOOL                           m_bIsSnap;                ///< ��ʶ�豸�Ƿ��ѿ���
	bool                           m_bImplementAutoGain;     ///< �Ƿ�֧���Զ�����
	bool                           m_bImplementAutoShutter;  ///< �Ƿ�֧���Զ��ع�
	bool                           m_bImplementLight;        ///< �Ƿ�֧��2A���ջ�������

	GX_EXPOSURE_AUTO_ENTRY         m_emAutoShutterMode;      ///< �Զ��ع�ģʽ
	GX_GAIN_AUTO_ENTRY             m_emAutoGainMode;         ///< �Զ�����ģʽ

	CWnd                          *m_pWnd;                   ///< ��ʾͼ�񴰿�(�ؼ�)ָ��

	CGXBitmap                     *m_pBitmap;                ///< ��ʾ����ͼ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXAUTOFUNCCTRLDLG_H__48794C28_712D_4E1F_97EA_58B5A231704F__INCLUDED_)
