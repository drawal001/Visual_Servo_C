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
	
	/// ��ʼ������
	GX_STATUS InitParam();

	/// ����ͼ�����ݸ�ʽΪ16λ
	GX_STATUS SetPixelSize16(CString *pstrSymbolic);
	
	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);
	
	/// ��ʾͼ��
	void DrawImage(BYTE *pImageBuf, int nWidth, int nHeight, BITMAPINFO *pBmpInfo);

	/// ����Rawͼ��
	void SaveRawImage(void *pRawBuf);
	
	/// ��ȡ������Ϣ����
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ����UI����
	void UpdateUI();

private: 
	CString                 m_strSavePath;            ///< ͼ�񱣴�·��
	GX_DEV_HANDLE           m_hDevice;                ///< �豸���
	GX_DS_HANDLE			m_hStream;			      ///< �豸�����
	BYTE                   *m_pRawBuf;                ///< ԭʼRawͼ��Buffer
	BYTE                   *m_pRaw8Buf;               ///< 8λRawͼ��Buffer
	int64_t                 m_nPixelFilter;      ///< ��ɫ�����Bayer��ʽ
	bool                    m_bIsColorFilter;         ///< �жϵ�ǰ�Ƿ�Ϊ��ɫ���ظ�ʽ
	bool                    m_bIsOpen;                ///< ����򿪱�ʶ 
	bool                    m_bIsSnap;                ///< �����ʼ�ɼ���ʶ
	bool                    m_bIsSaveImg;             ///< ����ͼ���ʶ
	HWND                    m_hWndHandle;             ///< ��ʾͼ��Ĵ��ھ��
    HDC                     m_hDC;                    ///< ͼ����ʾ���ڵ�DC 
	RECT                    m_objRect;                ///< ͼ����ʾ���ڴ�С
	CGXBitmap               *m_pBitmap;            ///< ��ʾ����ͼ����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GXSNAPRAW16DLG_H__DE201059_C44D_42D2_A515_AE83A37BF98D__INCLUDED_)
