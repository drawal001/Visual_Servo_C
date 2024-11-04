
// Denoise3DDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraApi.h"
#include "CameraGrabber.h"
#include "CameraImage.h"
#pragma comment(lib, "MVCAMSDK.lib")


// CDenoise3DDlg �Ի���
class CDenoise3DDlg : public CDialogEx
{
// ����
public:
	CDenoise3DDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DENOISE3D_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void* m_Grabber;
	CameraHandle m_hCamera;
	tSdkCameraDevInfo m_DevInfo;

	struct MvImage
	{
		tSdkFrameHead RawHead;
		BYTE* RawData;
		tSdkFrameHead RgbHead;
		BYTE* RgbData;

		MvImage()
		{
			ZeroMemory(&RawHead, sizeof(RawHead));
			RawData = NULL;
			ZeroMemory(&RgbHead, sizeof(RgbHead));
			RgbData = NULL;
		}

		~MvImage()
		{
			CameraAlignFree(RawData);
			CameraAlignFree(RgbData);
		}
	};
	MvImage m_Imgs[4];

private:

	void DoImgCap(int index);
	BOOL DoCompose();
	void DispImg(int index);
	void SaveImg(int index);

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonImgCap1();
	afx_msg void OnBnClickedButtonImgCap2();
	afx_msg void OnBnClickedButtonImgCap3();
	afx_msg void OnBnClickedButtonImgSave1();
	afx_msg void OnBnClickedButtonImgSave2();
	afx_msg void OnBnClickedButtonImgSave3();
	afx_msg void OnBnClickedButtonImgSave4();
	afx_msg void OnBnClickedButtonSettings();
};
