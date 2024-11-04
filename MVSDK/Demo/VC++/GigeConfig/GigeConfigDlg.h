
// GigeConfigDlg.h : ͷ�ļ�
//

#pragma once


#include "CameraApi.h"
#include "afxwin.h"
#ifdef _WIN64
#pragma comment(lib, ".\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, ".\\MVCAMSDK.lib")
#endif


// CGigeConfigDlg �Ի���
class CGigeConfigDlg : public CDialogEx
{
// ����
public:
	CGigeConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GIGECONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:

	CArray<tSdkCameraDevInfo> m_CamerasInfo;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBoxCamera;
	CString m_strCamIp;
	CString m_strCamMask;
	CString m_strCamGateway;
	CString m_strEtIp;
	CString m_strEtMask;
	CString m_strEtGateway;
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnLbnSelchangeListCamera();
	afx_msg void OnBnClickedButtonApply();
	CString m_strCamMac;
	CString m_strEtMac;
};
