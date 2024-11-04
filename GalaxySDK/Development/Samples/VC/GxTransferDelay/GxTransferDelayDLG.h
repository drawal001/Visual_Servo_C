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
#define   TRANSFER_COUNT          1     ///< 传输的帧数
#define   MAX_NUM_CAMERA          4     ///< 最多支持4台相机
#define   CAMERA0                 0     ///< 0号相机
#define   CAMERA1                 1     ///< 1号相机
#define   CAMERA2                 2     ///< 2号相机
#define   CAMERA3                 3     ///< 3号相机
#define   CAM0_WAIT_TIME          2000  ///< 0号相机等待时间
#define   CAM1_WAIT_TIME          4000  ///< 1号相机等待时间
#define   CAM2_WAIT_TIME          6000  ///< 2号相机等待时间
#define   CAM3_WAIT_TIME          8000  ///< 3号相机等待时间

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

	/// 设置相机参数
	GX_STATUS SetCameraParam(GX_DEV_HANDLE hDevice);

	/// 注册回调函数
	GX_STATUS RegisterCallback(int nDevNum);

	/// 0号相机采集回调函数
	static void __stdcall OnFrameCallbackFun0(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 1号采集回调函数
	static void __stdcall OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 2号采集回调函数
	static void __stdcall OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 3号采集回调函数
	static void __stdcall OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 0号相机曝光结束事件回调函数
    static void __stdcall OnFeatureCallbackFun0(const char* chFeature, void* pUserParam);

	/// 1号相机曝光结束事件回调函数
    static void __stdcall OnFeatureCallbackFun1(const char* chFeature, void* pUserParam);

	/// 2号相机曝光结束事件回调函数
    static void __stdcall OnFeatureCallbackFun2(const char* chFeature, void* pUserParam);

	/// 3号相机曝光结束事件回调函数
    static void __stdcall OnFeatureCallbackFun3(const char* chFeature, void* pUserParam);

	/// 获取错误信息描述
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 初始化资源
	bool InitResources();

	/// 释放资源
	void ReleaseResource();

	/// 更新UI界面
	void UpdateUI();

	/// 打印信息窗口
	void PrintInfotoWnd(string &strInfo);

private:
	int                         m_nDevNum;              ///< 设备个数
	GX_DEV_HANDLE*	            m_pDeviceHandle;	    ///< 设备句柄指针
	GX_DS_HANDLE*			    m_pStream;			    ///< 设备流句柄
	GX_FEATURE_CALLBACK_HANDLE* m_pCBHandle;            ///< 曝光回调函数句柄
	bool                        m_bIsOpen;              ///< 相机打开标志 
	bool                        m_bIsSnap;              ///< 相机开始采集标志
	HANDLE                      m_hCam0StartTranEvent;  ///< 0号相机启动传输事件
	HANDLE                      m_hCam1StartTranEvent;  ///< 1号相机启动传输事件
	HANDLE                      m_hCam2StartTranEvent;  ///< 2号相机启动传输事件
	HANDLE                      m_hCam3StartTranEvent;  ///< 3号相机启动传输事件
	HANDLE                      m_hCam0StopSnapEvent;   ///< 0号相机停止采集事件
	HANDLE                      m_hCam1StopSnapEvent;   ///< 1号相机停止采集事件
	HANDLE                      m_hCam2StopSnapEvent;   ///< 2号相机停止采集事件
	HANDLE                      m_hCam3StopSnapEvent;   ///< 3号相机停止采集事件
	void                        *m_pLogWnd;             ///< 指向日志显示窗口的指针
	HWND                        m_hWndHandle[4];        ///< 显示图像的窗口句柄
	CGXBitmap                   *m_pBitmap[4];           ///< 显示及画图对象
};

inline std::string  MsgComposer( const char* pFormat, ... )
{
	char pBuffer[256] = {0};
	va_list vap;
	va_start( vap, pFormat );
	
#if _MSC_VER >= 1400 //如果是VS2005或者以上版本
	_vsnprintf_s( pBuffer, 256, pFormat, vap );
#else if _MSC_VER == 1200 //如果是VC6
	_vsnprintf( pBuffer, 256, pFormat, vap );
#endif
	
	va_end(vap);
	return std::string( pBuffer);
	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxTransferDelayDLG_H__F071B8A4_DEDE_4E41_961A_11776A84A4F3__INCLUDED_)
