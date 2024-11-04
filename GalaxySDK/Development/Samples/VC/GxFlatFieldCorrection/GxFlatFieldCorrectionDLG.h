// GxFlatFieldCorrectionDlg.h : header file
//

#if !defined(AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
#define AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  WHITE_BALANCE_RNTRY               3      ///< �Զ���ƽ��ö����

#define  GRAY_MIN                          0      ///< �Ҷ���Сֵ
#define  GRAY_MAX                          255    ///< �Ҷ����ֵ
#define  GRAY_INVALID                      256    ///< �Ҷ���Чֵ
#define  GRAY_INIT                         127    ///< �Ҷȳ�ʼ��ֵ
#define  BRIGHT_IMG_GRAY_MIN               20     ///< ����ͼ��Ҷ���Сֵ
#define  BRIGHT_IMG_GRAY_MAX               250    ///< ����ͼ��Ҷ����ֵ

#define  CORRECTION_UI_FRAME_COUNT_MIN     0      ///< UI����ɼ�֡����Сֵ
#define  CORRECTION_UI_FRAME_COUNT_1       1      ///< UI����ɼ�֡��1
#define  CORRECTION_UI_FRAME_COUNT_2       2      ///< UI����ɼ�֡��2
#define  CORRECTION_UI_FRAME_COUNT_3       3      ///< UI����ɼ�֡��3
#define  CORRECTION_UI_FRAME_COUNT_MAX     4      ///< UI����ɼ�֡�����ֵ

#define  CORRECTION_ACTUAL_FRAME_COUNT_1   1      ///< ƽ��У��ʵ�ʲɼ�֡��1
#define  CORRECTION_ACTUAL_FRAME_COUNT_2   2      ///< ƽ��У��ʵ�ʲɼ�֡��2
#define  CORRECTION_ACTUAL_FRAME_COUNT_4   4      ///< ƽ��У��ʵ�ʲɼ�֡��4
#define  CORRECTION_ACTUAL_FRAME_COUNT_8   8      ///< ƽ��У��ʵ�ʲɼ�֡��8
#define  CORRECTION_ACTUAL_FRAME_COUNT_16  16     ///< ƽ��У��ʵ�ʲɼ�֡��16

#define  IMAGE_DATA_NUM_MAX                16     ///< ����λ�����ֵ
#define  IMAGE_DATA_NUM_MIN                8      ///< ����λ����Сֵ

/// �ļ�״̬��
typedef enum  FFC_STATUS
{
	FFC_STATUS_SUCCESS          =  0,   ///< �ļ������ɹ�
	FFC_STATUS_FILE_OPEN_ERROR  = -1,   ///< �ļ���ʧ��
	FFC_STATUS_FILE_WRITE_ERROR = -2,   ///< �ļ�д��ʧ��
	FFC_STATUS_FILE_READ_ERROR  = -3,   ///< �ļ���ȡʧ��
	FFC_STATUS_FILE_PATH_ERROR  = -4,   ///< �ļ�·���Ƿ�
	FFC_STATUS_FILE_DATA_ERROR  = -5    ///< �ļ������쳣
} FFC_STATUS;

/////////////////////////////////////////////////////////////////////////////
// CGxFlatFieldCorrectionDlg dialog

#include "GxBitmap.h"
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>
#include <new>

class CGxFlatFieldCorrectionDlg : public CDialog
{
// Construction
public:
	CGxFlatFieldCorrectionDlg(CWnd* pParent = NULL);           ///< standard constructor
	
private:
	GX_DEV_HANDLE       m_hDevice;                             ///< �豸���
	GX_DS_HANDLE	    m_hStream;				               ///< �豸�����

	int64_t             m_nAutoWhiteBalance;                   ///< �Զ���ƽ�⵱ǰѡ��
	int64_t             m_nRatioSelector;                      ///< ��¼��ǰ�İ�ƽ��ͨ��
	int64_t             m_i64ImageHeight;                      ///< ԭʼͼ���
	int64_t             m_i64ImageWidth;                       ///< ԭʼͼ���

	GX_FLOAT_VALUE      m_stShutterFloatRange;                 ///< �����ع�ʱ��ķ�Χ
	GX_FLOAT_VALUE      m_stGainFloatRange;                    ///< ��������ֵ�ķ�Χ
	GX_FLOAT_VALUE      m_stFloatRange;                        ///< �����ƽ��ϵ���ķ�Χ  

	bool                m_bDevOpened;                          ///< ��ʶ�Ƿ��Ѵ��豸
	bool                m_bIsSnap;                             ///< ��ʶ�Ƿ�ʼ�ɼ�
 
	bool                m_bColorFilter;                        ///< ��ʶ��ǰ�Ƿ�Ϊ��ɫ
	bool                m_bBalanceRatioSelector;               ///< ��ʶ�Ƿ�֧�ְ�ƽ��ͨ��

	CWnd                *m_pWnd;                               ///< ��ʾͼ�񴰿�(�ؼ�)ָ��

	BYTE                *m_pBrightBuf;                         ///< ����ͼ��buffer
	BYTE                *m_pDarkBuf;                           ///< ����ͼ��buffer
	bool                m_bDarkImgAcquired;                    ///< ��ʶ�Ƿ�ɼ�����ͼ��
	bool                m_bDarkImgAcquiredOnce;                ///< ����Ƿ��Ѳɼ�����ͼ��
	bool                m_bBrightImgAcquired;                  ///< ��ʶ�Ƿ�ɼ�����ͼ��
	bool                m_bBrightImgAcquiredOnce;              ///< ����Ƿ��Ѳɼ�����ͼ��      
	DX_ACTUAL_BITS      m_nActualBits;                         ///< ����ʵ��λ��
	bool                m_bGetFFCCoefficients;                 ///< ��ʶ�Ƿ���ƽ��У��ϵ��

	bool                m_bExecuteFFC;                         ///< ��ʶ�Ƿ�ִ��ƽ��У��
	bool                m_bIsFFC;                              ///< ��ʶ�Բɼ���ͼ���Ƿ����ƽ��У����FFC��
	bool                m_bShowFFCOK;                          ///< ��ʶ�Ƿ���ʾƽ��У����FFC��ִ�гɹ�

	BYTE                *m_pFFCCoefficients;                   ///< ƽ��У������
	int                 m_nFFClength;                          ///< ƽ��У���������ȣ��ֽ�����
	int                 m_nFFCGray;                            ///< ����ƽ��У�����õ������Ҷ�ֵ
	int                 m_nNewGray;                            ///< �������õ������Ҷ�ֵ
	bool                m_bFFCGray;                            ///< ��ʶ�Ƿ�����û����õ������Ҷ�ֵ
	int                 m_nFrameCount;                         ///< �ɼ�֡��
	int                 m_nBrightImgNum;                       ///< �ɼ�����ͼ����
	int                 m_nAverageGrayDarkImg;                 ///< ����ͼ��ƽ���Ҷ�ֵ
	int                 m_nAverageGrayBrightImg;               ///< ����ͼ��ƽ���Ҷ�ֵ
	VxUint32            *m_pAccumulateImgBuf;                  ///< �����ۼ�ͼ��buffer
	BYTE                *m_pAverageImgBuf;                     ///< ����ƽ��ͼ��buffer

	FLAT_FIELD_CORRECTION_PROCESS    m_stFlatFieldCorrection;  ///< ƽ��У���ṹ��
	CGXBitmap           *m_pBitmap;                            ///< ��ʾ����ͼ����
public:
	///  Ϊͼ����ʾ׼����Դ
	bool PrepareForShowImg();

	/// ��ȡ�豸����,��ʼ������ؼ�
	void InitUI();

    /// ˢ��UI����
	void UpDateUI();

	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);

	///  �ͷ�Ϊͼ����ʾ׼����Դ
	void UnPrepareForShowImg();

	/// ������Ϣ��ʾ
	void ShowErrorString(GX_STATUS emErrorStatus);

	/// ��ʼ��ö������UI����
	void InitEnumUI(char* chFeatire, CComboBox *pComboBox, bool bIsImplement);

	/// ��ʼ����ƽ��ϵ����ؿؼ�
	void InitWhiteRatioUI();

	/// ˢ����ʾ��ǰ��ƽ��ϵ��
	void RefreshWhiteRatio();

	/// ��ʼ���������ؿؼ�
	void InitGainUI();

	/// ��ʼ���ع���ؿؼ�
	void InitShutterUI();

	/// ��ʼ���Ҷ�ֵ����ؿؼ�
	void InitGrayUI();

	/// ��ʼ���ɼ�֡������ؿؼ�
	void InitFrameCountUI();

	/// ��ʼ�����:����Ĭ�ϲ���
	GX_STATUS InitDevice();

	/// ��ȡ�豸��������Ϣ
	GX_STATUS GetDeviceParam();

	/// ͼ���ۼӺ���
	DX_STATUS AccumulateImage(void *pImgBuf, int nDataWidth, int64_t nlength, VxUint32 *pAccumulateImgBuf);

	/// ƽ��ͼ����㺯��
	DX_STATUS GetAverageImage(VxUint32 *pImgBuf, int64_t nlength, int nAccumulateNum, int nDataWidth, void *pAverageImgBuf);

	/// ��ͼ��ƽ���Ҷ�ֵ
	int64_t GetAverageGray8bit(BYTE *pImgBuf, int64_t nImgSize);

	/// �ɼ�����ͼ��
	void AcquireDarkFieldImg(void);
		
	/// �ɼ�����ͼ��
	void AcquireBrightFieldImg(void);

	/// ͼ��ɼ�֡��
	void GetFrameCount(int &nFrameCount);

	/// ���ļ��ж�ȡFFC����
	FFC_STATUS ReadFFCCoefficientsFile(const std::string &strFilePath);

	/// ��FFC�������浽�ļ���
	FFC_STATUS WriteFFCCoefficientsFile(const std::string &strFilePath);

// Dialog Data
	//{{AFX_DATA(CGxFlatFieldCorrectionDlg)
	enum { IDD = IDD_GxFlatFieldCorrection_DIALOG };
	double      m_dBalanceRatio;
	double	    m_dGainValue;
	double	    m_dShutterValue;
	CSliderCtrl m_sliderFrameCount;
	CSliderCtrl m_sliderGrayValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGxFlatFieldCorrectionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGxFlatFieldCorrectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboWriteBlance();
	afx_msg void OnBnClickedBtnOpenDevice();
	afx_msg void OnBnClickedBtnCloseDevice();
	afx_msg void OnBnClickedBtnStopSnap();
	afx_msg void OnBnClickedBtnStartSnap();
	afx_msg void OnCbnSelchangeComboRatioSelector();
	afx_msg void OnEnKillfocusEditBalanceRatio();
	afx_msg void OnKillfocusEditShutter();
	afx_msg void OnKillfocusEditGain();
	afx_msg void OnKillfocusEditGray();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposSpinExpectGrayValue(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonAcquireDarkImg();
	afx_msg void OnBnClickedButtonAcquireBrightImg();
	afx_msg void OnBnClickedButtonExecuteFFC();         
	afx_msg void OnBnClickedCheckPreview();             
	afx_msg void OnBnClickedCheckGray();               
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GxFlatFieldCorrectionDLG_H__D8D963C2_3BE2_44EC_B0EB_FCE016DDD6FA__INCLUDED_)
