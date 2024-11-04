
// SaveImageDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraDlg.h"
#include "afxwin.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


// CSaveImageDlg �Ի���
class CSaveImageDlg : public CDialogEx
{
// ����
public:
	CSaveImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SAVEIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void OpenCameras();
	void CloseCameras();

private:
	void TriggerCameras();

	static void CALLBACK PeriodTrigProc(
		UINT      uTimerID,
		UINT      uMsg,
		DWORD_PTR dwUser,
		DWORD_PTR dw1,
		DWORD_PTR dw2)
	{
		CSaveImageDlg* p = (CSaveImageDlg*)dwUser;
		p->OnPeriodTrig();
	}
	void OnPeriodTrig();

private:

	CArray<CCameraDlg*> m_pCamList;
	CArray<CCameraDlg*> m_pInvalidCamList;

	// ��ʱ����
	MMRESULT m_hPeriodTrigTimer;
	DWORD m_PeriodTrigFrameCount;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_CamGroup;
	CComboBox m_ComboTriggerMode;
	CEdit m_EditTriggerCount;
	CEdit m_EditSaveDir;
	CComboBox m_ComboSaveFmt;
	CEdit m_EditSaveCount;
	CEdit m_EditTrigTimerInterval;
	CEdit m_EditTrigTimerPeriod;
	CEdit m_EditTrigTimerActiveTime;
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboTriggerMode();
	afx_msg void OnEnKillfocusEditTriggerCount();
	afx_msg void OnBnClickedButtonSoftTrigger();
	afx_msg void OnEnKillfocusEditSaveDir();
	afx_msg void OnCbnSelchangeComboSaveFmt();
	afx_msg void OnBnClickedCheckAutoSave();
	afx_msg void OnBnClickedCheckSaveOnlyTrigger();
	afx_msg void OnBnClickedButtonSetSaveCount();
	afx_msg void OnBnClickedButtonTrigTimerStart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnKillfocusEditTrigTimerPeriod();
	afx_msg void OnEnKillfocusEditTrigTimerActive();
	afx_msg void OnBnClickedCheckOverlayDate();
	afx_msg void OnEnKillfocusEditTrigTimerInterval();
};
