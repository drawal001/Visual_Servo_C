// GxGetImageDlg.h : header file
//

#if !defined(AFX_GXGETIMAGEDLG_H__EA4AE0E8_97C9_47D9_B720_3F7E53EFC867__INCLUDED_)
#define AFX_GXGETIMAGEDLG_H__EA4AE0E8_97C9_47D9_B720_3F7E53EFC867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>

using namespace std;


//----------------------------------------------------------------------------------
/**
\brief  时间统计类
*/
//----------------------------------------------------------------------------------
class CStatistics
{
public:
	/// 构造函数
	CStatistics()
	{
		m_dSumTime = 0.0;  
		m_dMax     = 0.0;      
		m_dMin     = 0.0;    
		m_dAverage = 0.0; 
		m_nNum     = 0;      
	}

	/// 增加统计计数并计算相关数据
	void AddStatisticalData(double dData)
	{
		__IncSumTimeData(dData);
		__UpdateData(dData);
	}

	/// 获取最大值
	double GetMax()
	{
		return m_dMax;
	}

    /// 获取最小值
	double GetMin()
	{
		return m_dMin;
	}

	/// 获取平均值
	double GetAverage()
	{
		if (m_nNum == 0)
		{
			return 0;
		}

		return m_dSumTime / m_nNum;
	}

	/// 数据重置函数
	void Reset()
	{
		m_dSumTime = 0.0;  
		m_dMax     = 0.0;      
		m_dMin     = 0.0;      
		m_dAverage = 0.0; 
		m_nNum     = 0;  
	}

private:
	/// 增加统计总时间计数
	void __IncSumTimeData(double dData)
	{
		m_dSumTime = m_dSumTime + dData;
		m_nNum++;
	}

	/// 数据比较更新最大值和最小值
	void __UpdateData(double dData)
	{
		if (m_nNum == 1)
		{
			// 统计次数为1时
			m_dMax = dData;
			m_dMin = dData;
			return;
		}

		if (dData > m_dMax)
		{
			m_dMax = dData;
		}

		if (dData < m_dMin)
		{
			m_dMin = dData;
		}
	}

private:
	double  m_dSumTime;  ///< 总时间
	double  m_dMax;      ///< 最大值
	double  m_dMin;      ///< 最小值
	double  m_dAverage;  ///< 平均值
	int64_t m_nNum;      ///< 统计次数计数
};

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
		return (double)(timeDifference * 1000/ ticksPerSecond); 
	}
protected: 
	LARGE_INTEGER m_StartTime;   ///< 开始时间
    LARGE_INTEGER m_StopTime;    ///< 停止时间 
};

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg dialog

class CGxGetImageDlg : public CDialog
{
// Construction
public:
	CGxGetImageDlg(CWnd* pParent = NULL);	// standard constructor

	/// 打开相机
	GX_STATUS OpenDevice();

	/// 关闭相机
	GX_STATUS CloseDevice();

	/// 初始化设备的默认参数
	GX_STATUS InitDevice();

	/// 为图像显示准备资源,分配Buffer
	bool PrepareForShowImg();
    
	/// 释放资源
	void UnPrepareForShowImg();

	/// 刷新UI
	void UpdateUI();

	/// 错误信息提示
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 更新界面的统计数据
	void UpdateStatisticalData(double dData);
	
// Dialog Data
	//{{AFX_DATA(CGxGetImageDlg)
	enum { IDD = IDD_GXGETIMAGE_DIALOG };
	UINT	m_nEditTimeOut;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxGetImageDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxGetImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnSendSoftCommand();
	afx_msg void OnBnClickedBtnCleanTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	GX_DEV_HANDLE                 m_hDevice;            ///< 设备句柄
	GX_DS_HANDLE                  m_hStream;		    ///< 设备流句柄
	bool                          m_bIsOpen;            ///< 设备打开状态
	bool                          m_bIsSnap;            ///< 发送开采命令标识

	GX_FRAME_DATA                 m_stFrameData;        ///< GetImage获取的图像地址	

	CStatistics                   m_objStatistic;       ///< 数据统计类对象用于处理统计时间
	CStopWatch                    m_objStopTime;        ///< 定义时间差类对象

	CWnd                          *m_pWnd;              ///< 显示图像窗口(控件)指针
	CGXBitmap                     *m_pBitmap;            ///< 显示及画图对象
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXGETIMAGEDLG_H__EA4AE0E8_97C9_47D9_B720_3F7E53EFC867__INCLUDED_)
