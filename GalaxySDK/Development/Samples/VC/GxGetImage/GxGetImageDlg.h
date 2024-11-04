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
\brief  ʱ��ͳ����
*/
//----------------------------------------------------------------------------------
class CStatistics
{
public:
	/// ���캯��
	CStatistics()
	{
		m_dSumTime = 0.0;  
		m_dMax     = 0.0;      
		m_dMin     = 0.0;    
		m_dAverage = 0.0; 
		m_nNum     = 0;      
	}

	/// ����ͳ�Ƽ����������������
	void AddStatisticalData(double dData)
	{
		__IncSumTimeData(dData);
		__UpdateData(dData);
	}

	/// ��ȡ���ֵ
	double GetMax()
	{
		return m_dMax;
	}

    /// ��ȡ��Сֵ
	double GetMin()
	{
		return m_dMin;
	}

	/// ��ȡƽ��ֵ
	double GetAverage()
	{
		if (m_nNum == 0)
		{
			return 0;
		}

		return m_dSumTime / m_nNum;
	}

	/// �������ú���
	void Reset()
	{
		m_dSumTime = 0.0;  
		m_dMax     = 0.0;      
		m_dMin     = 0.0;      
		m_dAverage = 0.0; 
		m_nNum     = 0;  
	}

private:
	/// ����ͳ����ʱ�����
	void __IncSumTimeData(double dData)
	{
		m_dSumTime = m_dSumTime + dData;
		m_nNum++;
	}

	/// ���ݱȽϸ������ֵ����Сֵ
	void __UpdateData(double dData)
	{
		if (m_nNum == 1)
		{
			// ͳ�ƴ���Ϊ1ʱ
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
	double  m_dSumTime;  ///< ��ʱ��
	double  m_dMax;      ///< ���ֵ
	double  m_dMin;      ///< ��Сֵ
	double  m_dAverage;  ///< ƽ��ֵ
	int64_t m_nNum;      ///< ͳ�ƴ�������
};

//----------------------------------------------------------------------------------
/**
\brief  ��ȡʱ�����
*/
//----------------------------------------------------------------------------------
class CStopWatch
{ 
public: 

	/// ���캯�� 
	CStopWatch()
	{ 
		Start(); 
	} 

	/// ��ʼ��ʱ
	void Start() 
	{ 
		QueryPerformanceCounter(&m_StartTime);
	} 

	/// ֹͣ��ʱ,ms
	double Stop() 
	{
		QueryPerformanceCounter(&m_StopTime);
		double theElapsedTime = ElapsedTime();

		m_StartTime = m_StopTime; 
		return theElapsedTime;
	} 

	/// ��ȡʱ��� 
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
	LARGE_INTEGER m_StartTime;   ///< ��ʼʱ��
    LARGE_INTEGER m_StopTime;    ///< ֹͣʱ�� 
};

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg dialog

class CGxGetImageDlg : public CDialog
{
// Construction
public:
	CGxGetImageDlg(CWnd* pParent = NULL);	// standard constructor

	/// �����
	GX_STATUS OpenDevice();

	/// �ر����
	GX_STATUS CloseDevice();

	/// ��ʼ���豸��Ĭ�ϲ���
	GX_STATUS InitDevice();

	/// Ϊͼ����ʾ׼����Դ,����Buffer
	bool PrepareForShowImg();
    
	/// �ͷ���Դ
	void UnPrepareForShowImg();

	/// ˢ��UI
	void UpdateUI();

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ���½����ͳ������
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
	GX_DEV_HANDLE                 m_hDevice;            ///< �豸���
	GX_DS_HANDLE                  m_hStream;		    ///< �豸�����
	bool                          m_bIsOpen;            ///< �豸��״̬
	bool                          m_bIsSnap;            ///< ���Ϳ��������ʶ

	GX_FRAME_DATA                 m_stFrameData;        ///< GetImage��ȡ��ͼ���ַ	

	CStatistics                   m_objStatistic;       ///< ����ͳ����������ڴ���ͳ��ʱ��
	CStopWatch                    m_objStopTime;        ///< ����ʱ��������

	CWnd                          *m_pWnd;              ///< ��ʾͼ�񴰿�(�ؼ�)ָ��
	CGXBitmap                     *m_pBitmap;            ///< ��ʾ����ͼ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXGETIMAGEDLG_H__EA4AE0E8_97C9_47D9_B720_3F7E53EFC867__INCLUDED_)
