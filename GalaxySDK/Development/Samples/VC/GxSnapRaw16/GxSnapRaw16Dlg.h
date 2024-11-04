// GxSnapRaw16Dlg.h : header file
//

#if !defined(AFX_GXSNAPRAW16DLG_H__DE201059_C44D_42D2_A515_AE83A37BF98D__INCLUDED_)
#define AFX_GXSNAPRAW16DLG_H__DE201059_C44D_42D2_A515_AE83A37BF98D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGxSnapRaw16Dlg dialog
#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"

class CGxSnapRaw16Dlg : public CDialog
{
// Construction
public:
	CGxSnapRaw16Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGxSnapRaw16Dlg)
	enum { IDD = IDD_GXSNAPRAW16_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxSnapRaw16Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxSnapRaw16Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenDevice();
	afx_msg void OnStartSnap();
	afx_msg void OnStopSnap();
	afx_msg void OnCloseDevice();
	afx_msg void OnClose();
	afx_msg void OnSaveRawImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	/// 初始化参数
	GX_STATUS InitParam();

	/// 设置图像数据格式为16位
	GX_STATUS SetPixelSize16(CString *pstrSymbolic);
	
	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);
	
	/// 显示图像
	void DrawImage(BYTE *pImageBuf, int nWidth, int nHeight, BITMAPINFO *pBmpInfo);

	/// 保存Raw图像
	void SaveRawImage(void *pRawBuf);
	
	/// 获取错误信息描述
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// 更新UI界面
	void UpdateUI();

private: 
	CString                 m_strSavePath;            ///< 图像保存路径
	GX_DEV_HANDLE           m_hDevice;                ///< 设备句柄
	GX_DS_HANDLE			m_hStream;			      ///< 设备流句柄
	BYTE                   *m_pRawBuf;                ///< 原始Raw图像Buffer
	BYTE                   *m_pRaw8Buf;               ///< 8位Raw图像Buffer
	int64_t                 m_nPixelFilter;      ///< 彩色相机的Bayer格式
	bool                    m_bIsColorFilter;         ///< 判断当前是否为彩色像素格式
	bool                    m_bIsOpen;                ///< 相机打开标识 
	bool                    m_bIsSnap;                ///< 相机开始采集标识
	bool                    m_bIsSaveImg;             ///< 保存图像标识
	HWND                    m_hWndHandle;             ///< 显示图像的窗口句柄
    HDC                     m_hDC;                    ///< 图像显示窗口的DC 
	RECT                    m_objRect;                ///< 图像显示窗口大小
	CGXBitmap               *m_pBitmap;            ///< 显示及画图对象
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXSNAPRAW16DLG_H__DE201059_C44D_42D2_A515_AE83A37BF98D__INCLUDED_)
