// GxCameraEventsDlg.h : header file
//

#if !defined(AFX_GxCameraEventsDLG_H__AA549272_EB77_49E3_8F5F_64128182B62A__INCLUDED_)
#define AFX_GxCameraEventsDLG_H__AA549272_EB77_49E3_8F5F_64128182B62A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include <iostream>
using namespace std;

#include "GxIAPI.h"

/////////////////////////////////////////////////////////////////////////////
// CGxCameraEventsDlg dialog
///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
	                           if(emStatus != GX_STATUS_SUCCESS) \
								{\
								ShowErrorString(emStatus); \
								return;\
								} 
///< 判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
	                               if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
								   return emStatus;\
                                   }\

//----------------------------------------------------------------------------------
/**
\brief  获取时间差类
*/
//----------------------------------------------------------------------------------
class CStopWatch
{ 
public: 
	
	/// 构造函数 
	CStopWatch()
	{ 
		Start(); 
	} 
	
	/// 开始计时
	void Start() 
	{ 
		QueryPerformanceCounter(&m_StartTime);
	} 
	
	/// 停止计时,ms
	double Stop() 
	{
		QueryPerformanceCounter(&m_StopTime);
		double theElapsedTime = ElapsedTime();
		
		m_StartTime = m_StopTime; 
		return theElapsedTime;
	} 
	
	/// 获取时间差 
	double ElapsedTime() 
	{ 
		LARGE_INTEGER timerFrequency; QueryPerformanceFrequency(&timerFrequency); 
		__int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;
		__int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart; 
		long double timeDifference = (long double) (newTicks - oldTicks); 
		long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) + (__int64)timerFrequency.LowPart); 
		return (double)(timeDifference * 1000 / ticksPerSecond); 
	}
protected: 
	LARGE_INTEGER m_StartTime;   ///< 开始时间
    LARGE_INTEGER m_StopTime;    ///< 停止时间 
};

class CGxCameraEventsDlg : public CDialog
{
// Construction
public:
	CGxCameraEventsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGxCameraEventsDlg)
	enum { IDD = IDD_GxCameraEvents_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxCameraEventsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxCameraEventsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	afx_msg void OnSendSoftCommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	/// 初始化参数
	GX_STATUS InitParam();

	/// 采集回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 曝光完成事件回调函数	
	static void __stdcall OnFeatureCallbackFun(const char* chFeature, void* pUserParam);
	
	/// 打印时间信息
	void PrintInfotoWnd(string &strInfo);

	/// 获取错误信息描述
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 更新UI界面
	void UpdateUI();

private:
	GX_STATUS __IsSupportEvent(bool &bSupportExposureEndEvent);

private:
	GX_DEV_HANDLE              m_hDevice;                      ///< 设备句柄
	GX_DS_HANDLE			   m_hStream;				       ///< 设备流句柄
	GX_FEATURE_CALLBACK_HANDLE m_hCB;                          ///< 曝光完成事件回调句柄
	void                      *m_pLogWnd;                      ///< 指向记录时间窗口
	bool                       m_bIsOpen;                      ///< 相机打开标识 
	bool                       m_bIsSnap;                      ///< 相机开始采集标识
	CStopWatch                 m_objImageTime;                 ///< 收到图像的时间
	CStopWatch                 m_objShutterTime;               ///< 曝光完成事件时间
	bool                       m_bImplementEventSelector;      ///< 是否支持事件源选择
	bool                       m_bSupportExposureEndEvent;     ///< 是否支持曝光结束事件
	bool                       m_bIsTrigValid;                 ///< 触发是否有效标志:当一次触发正在执行时，将该标志置为false
};

/// 打印日志函数字符串转换函数
inline std::string  MsgComposer(const char* pFormat, ... )
{
	char    chBuffer[256] = {0};
	va_list  vap;
	va_start(vap, pFormat);
	
#if _MSC_VER >= 1400        // 如果是VS2005或者以上版本
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
#else if _MSC_VER == 1200   // 如果是VC6
	_vsnprintf(chBuffer, 256, pFormat, vap );
#endif
	
	va_end(vap);
	return std::string(chBuffer);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxCameraEventsDLG_H__AA549272_EB77_49E3_8F5F_64128182B62A__INCLUDED_)
