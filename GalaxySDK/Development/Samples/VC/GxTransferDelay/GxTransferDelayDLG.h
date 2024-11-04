// GxTransferDelayDlg.h : header file
//

#if !defined(AFX_GxTransferDelayDLG_H__F071B8A4_DEDE_4E41_961A_11776A84A4F3__INCLUDED_)
#define AFX_GxTransferDelayDLG_H__F071B8A4_DEDE_4E41_961A_11776A84A4F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "FileVersion.h"
#include <string>
#include <iostream>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CGxTransferDelayDlg dialog
#define   TRANSFER_COUNT          1     ///< �����֡��
#define   MAX_NUM_CAMERA          4     ///< ���֧��4̨���
#define   CAMERA0                 0     ///< 0�����
#define   CAMERA1                 1     ///< 1�����
#define   CAMERA2                 2     ///< 2�����
#define   CAMERA3                 3     ///< 3�����
#define   CAM0_WAIT_TIME          2000  ///< 0������ȴ�ʱ��
#define   CAM1_WAIT_TIME          4000  ///< 1������ȴ�ʱ��
#define   CAM2_WAIT_TIME          6000  ///< 2������ȴ�ʱ��
#define   CAM3_WAIT_TIME          8000  ///< 3������ȴ�ʱ��

class CGxTransferDelayDlg : public CDialog
{
// Construction
public:
	CGxTransferDelayDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGxTransferDelayDlg)
	enum { IDD = IDD_GxTransferDelay_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxTransferDelayDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxTransferDelayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	/// �����������
	GX_STATUS SetCameraParam(GX_DEV_HANDLE hDevice);

	/// ע��ص�����
	GX_STATUS RegisterCallback(int nDevNum);

	/// 0������ɼ��ص�����
	static void __stdcall OnFrameCallbackFun0(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 1�Ųɼ��ص�����
	static void __stdcall OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 2�Ųɼ��ص�����
	static void __stdcall OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 3�Ųɼ��ص�����
	static void __stdcall OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 0������ع�����¼��ص�����
    static void __stdcall OnFeatureCallbackFun0(const char* chFeature, void* pUserParam);

	/// 1������ع�����¼��ص�����
    static void __stdcall OnFeatureCallbackFun1(const char* chFeature, void* pUserParam);

	/// 2������ع�����¼��ص�����
    static void __stdcall OnFeatureCallbackFun2(const char* chFeature, void* pUserParam);

	/// 3������ع�����¼��ص�����
    static void __stdcall OnFeatureCallbackFun3(const char* chFeature, void* pUserParam);

	/// ��ȡ������Ϣ����
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ��ʼ����Դ
	bool InitResources();

	/// �ͷ���Դ
	void ReleaseResource();

	/// ����UI����
	void UpdateUI();

	/// ��ӡ��Ϣ����
	void PrintInfotoWnd(string &strInfo);

private:
	int                         m_nDevNum;              ///< �豸����
	GX_DEV_HANDLE*	            m_pDeviceHandle;	    ///< �豸���ָ��
	GX_DS_HANDLE*			    m_pStream;			    ///< �豸�����
	GX_FEATURE_CALLBACK_HANDLE* m_pCBHandle;            ///< �ع�ص��������
	bool                        m_bIsOpen;              ///< ����򿪱�־ 
	bool                        m_bIsSnap;              ///< �����ʼ�ɼ���־
	HANDLE                      m_hCam0StartTranEvent;  ///< 0��������������¼�
	HANDLE                      m_hCam1StartTranEvent;  ///< 1��������������¼�
	HANDLE                      m_hCam2StartTranEvent;  ///< 2��������������¼�
	HANDLE                      m_hCam3StartTranEvent;  ///< 3��������������¼�
	HANDLE                      m_hCam0StopSnapEvent;   ///< 0�����ֹͣ�ɼ��¼�
	HANDLE                      m_hCam1StopSnapEvent;   ///< 1�����ֹͣ�ɼ��¼�
	HANDLE                      m_hCam2StopSnapEvent;   ///< 2�����ֹͣ�ɼ��¼�
	HANDLE                      m_hCam3StopSnapEvent;   ///< 3�����ֹͣ�ɼ��¼�
	void                        *m_pLogWnd;             ///< ָ����־��ʾ���ڵ�ָ��
	HWND                        m_hWndHandle[4];        ///< ��ʾͼ��Ĵ��ھ��
	CGXBitmap                   *m_pBitmap[4];           ///< ��ʾ����ͼ����
};

inline std::string  MsgComposer( const char* pFormat, ... )
{
	char pBuffer[256] = {0};
	va_list vap;
	va_start( vap, pFormat );
	
#if _MSC_VER >= 1400 //�����VS2005�������ϰ汾
	_vsnprintf_s( pBuffer, 256, pFormat, vap );
#else if _MSC_VER == 1200 //�����VC6
	_vsnprintf( pBuffer, 256, pFormat, vap );
#endif
	
	va_end(vap);
	return std::string( pBuffer);
	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxTransferDelayDLG_H__F071B8A4_DEDE_4E41_961A_11776A84A4F3__INCLUDED_)
