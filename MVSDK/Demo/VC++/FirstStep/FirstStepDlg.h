
// FirstStepDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraApi.h"
#include "CameraGrabber.h"
#include "CameraImage.h"
#pragma comment(lib, "MVCAMSDK.lib")


// CFirstStepDlg �Ի���
class CFirstStepDlg : public CDialogEx
{
// ����
public:
	CFirstStepDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIRSTSTEP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void *m_Grabber;
	CameraHandle m_hCamera;
	tSdkCameraDevInfo m_DevInfo;

	static void __stdcall FrameRGBCallbackEntry(void* Grabber, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead, void* Context)
	{
		CFirstStepDlg* pThis = (CFirstStepDlg*)Context;
		pThis->FrameRGBCallback(Grabber, pFrameBuffer, pFrameHead);
	}
	void FrameRGBCallback(void* Grabber, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSettings();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonSnap();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
