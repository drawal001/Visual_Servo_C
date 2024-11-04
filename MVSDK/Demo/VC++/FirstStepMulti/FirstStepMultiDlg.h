
// FirstStepMultiDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraApi.h"
#include "CameraGrabber.h"
#include "CameraImage.h"
#pragma comment(lib, "MVCAMSDK.lib")


// CFirstStepMultiDlg �Ի���
class CFirstStepMultiDlg : public CDialogEx
{
// ����
public:
	CFirstStepMultiDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIRSTSTEPMULTI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void* m_Grabber[4];
	CameraHandle m_hCamera[4];
	tSdkCameraDevInfo m_DevInfo[4];

	static void __stdcall FrameRGBCallbackEntry(void* Grabber, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead, void* Context)
	{
		CFirstStepMultiDlg* pThis = (CFirstStepMultiDlg*)Context;
		pThis->FrameRGBCallback(Grabber, pFrameBuffer, pFrameHead);
	}
	void FrameRGBCallback(void* Grabber, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead);

	static void __stdcall SaveImageCompleteEntry(void* Grabber, void* Image, CameraSdkStatus Status, void* Context)
	{
		CFirstStepMultiDlg* pThis = (CFirstStepMultiDlg*)Context;
		pThis->SaveImageComplete(Grabber, Image, Status);
	}
	void SaveImageComplete(void* Grabber, void* Image, CameraSdkStatus Status);

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
	afx_msg void OnBnClickedCheckPause1();
	afx_msg void OnBnClickedButtonCmSet1();
	afx_msg void OnBnClickedButtonSnapshot1();
	afx_msg void OnBnClickedCheckPause2();
	afx_msg void OnBnClickedButtonCmSet2();
	afx_msg void OnBnClickedButtonSnapshot2();
	afx_msg void OnBnClickedCheckPause3();
	afx_msg void OnBnClickedButtonCmSet3();
	afx_msg void OnBnClickedButtonSnapshot3();
	afx_msg void OnBnClickedCheckPause4();
	afx_msg void OnBnClickedButtonCmSet4();
	afx_msg void OnBnClickedButtonSnapshot4();
};
