
// ImageDefinitionDlg.h : ͷ�ļ�
//

#pragma once

#include "CameraApi.h"
#ifdef _WIN64
#pragma comment(lib, ".\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, ".\\MVCAMSDK.lib")
#endif


// CImageDefinitionDlg �Ի���
class CImageDefinitionDlg : public CDialogEx
{
// ����
public:
	CImageDefinitionDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IMAGEDEFINITION_DIALOG };

	protected:
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

	int				m_iAlg;

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
	afx_msg void OnBnClickedButtonSettings();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboAlg();
	afx_msg LRESULT OnSetDefValue(WPARAM wParam, LPARAM lParam);
};
