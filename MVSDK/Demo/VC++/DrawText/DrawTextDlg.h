
// DrawTextDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraApi.h"
#ifdef _WIN64
#pragma comment(lib, ".\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, ".\\MVCAMSDK.lib")
#endif

struct TextItem
{
	CString strFontFileName;
	UINT uFontW, uFontH;
	CString strText;
	CRect rcOut;
	UINT uColor;
	UINT uFlags;
};


// CDrawTextDlg �Ի���
class CDrawTextDlg : public CDialogEx
{
// ����
public:
	CDrawTextDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DRAWTEXT_DIALOG };

public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	CWnd*			m_pDisp;
	CameraHandle    m_hCamera;	//������豸���|the handle of the camera we use
	tSdkFrameHead   m_sFrInfo;//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ

	int	            m_iDispFrameNum;//���ڼ�¼��ǰ�Ѿ���ʾ��ͼ��֡������
	float           m_fDispFps;//��ʾ֡��
	float           m_fCapFps;//����֡��
	tSdkFrameStatistic  m_sFrameCount;
	tSdkFrameStatistic  m_sFrameLast;
	int		        m_iTimeLast;

	BYTE*           m_pFrameBuffer;//���ڽ�ԭʼͼ������ת��ΪRGB�Ļ�����

	UINT            m_threadID;//ͼ��ץȡ�̵߳�ID
	HANDLE          m_hDispThread;//ͼ��ץȡ�̵߳ľ��
	BOOL            m_bExit;//����֪ͨͼ��ץȡ�߳̽���

	CDialog*		m_pTextInfoDlg;

	CCriticalSection m_csTextItemArray;
	CArray<TextItem> m_TextItemArray;

	BOOL InitCamera();
	static UINT WINAPI uiDisplayThread(LPVOID lpParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
