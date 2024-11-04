
// RawTransTestDlg.h : ͷ�ļ�
//

#pragma once


// CRawTransTestDlg �Ի���
class CRawTransTestDlg : public CDialogEx
{
// ����
public:
	CRawTransTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RAWTRANSTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_sRawFile;
	CString m_sOutPutFile;
	afx_msg void OnBnClickedButtonTrans();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButton3();
	CString m_sConfigFile;

	PVOID m_pCameraConfig;
	afx_msg void OnBnClickedButtonTrans2();
	CString m_sJpgOutFile;
	afx_msg void OnClose();
};
