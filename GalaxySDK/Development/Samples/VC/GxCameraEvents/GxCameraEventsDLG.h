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
///< ������ʾ�����궨��
#define  GX_VERIFY(emStatus) \
	                           if(emStatus != GX_STATUS_SUCCESS) \
								{\
								ShowErrorString(emStatus); \
								return;\
								} 
///< �жϷ���ֵ�궨��
#define VERIFY_STATUS_RET(emStatus) \
	                               if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
								   return emStatus;\
                                   }\

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
		return (double)(timeDifference * 1000 / ticksPerSecond); 
	}
protected: 
	LARGE_INTEGER m_StartTime;   ///< ��ʼʱ��
    LARGE_INTEGER m_StopTime;    ///< ֹͣʱ�� 
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

	/// ��ʼ������
	GX_STATUS InitParam();

	/// �ɼ��ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// �ع�����¼��ص�����	
	static void __stdcall OnFeatureCallbackFun(const char* chFeature, void* pUserParam);
	
	/// ��ӡʱ����Ϣ
	void PrintInfotoWnd(string &strInfo);

	/// ��ȡ������Ϣ����
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ����UI����
	void UpdateUI();

private:
	GX_STATUS __IsSupportEvent(bool &bSupportExposureEndEvent);

private:
	GX_DEV_HANDLE              m_hDevice;                      ///< �豸���
	GX_DS_HANDLE			   m_hStream;				       ///< �豸�����
	GX_FEATURE_CALLBACK_HANDLE m_hCB;                          ///< �ع�����¼��ص����
	void                      *m_pLogWnd;                      ///< ָ���¼ʱ�䴰��
	bool                       m_bIsOpen;                      ///< ����򿪱�ʶ 
	bool                       m_bIsSnap;                      ///< �����ʼ�ɼ���ʶ
	CStopWatch                 m_objImageTime;                 ///< �յ�ͼ���ʱ��
	CStopWatch                 m_objShutterTime;               ///< �ع�����¼�ʱ��
	bool                       m_bImplementEventSelector;      ///< �Ƿ�֧���¼�Դѡ��
	bool                       m_bSupportExposureEndEvent;     ///< �Ƿ�֧���ع�����¼�
	bool                       m_bIsTrigValid;                 ///< �����Ƿ���Ч��־:��һ�δ�������ִ��ʱ�����ñ�־��Ϊfalse
};

/// ��ӡ��־�����ַ���ת������
inline std::string  MsgComposer(const char* pFormat, ... )
{
	char    chBuffer[256] = {0};
	va_list  vap;
	va_start(vap, pFormat);
	
#if _MSC_VER >= 1400        // �����VS2005�������ϰ汾
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
#else if _MSC_VER == 1200   // �����VC6
	_vsnprintf(chBuffer, 256, pFormat, vap );
#endif
	
	va_end(vap);
	return std::string(chBuffer);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxCameraEventsDLG_H__AA549272_EB77_49E3_8F5F_64128182B62A__INCLUDED_)
