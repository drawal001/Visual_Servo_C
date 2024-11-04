// AdvancedDlg.h : header file
//
//BIG5 TRANS ALLOWED

#if !defined(AFX_ADVANCEDDLG_H__4B7A0F2F_62FA_4C67_AB8B_8DE471779F2C__INCLUDED_)
#define AFX_ADVANCEDDLG_H__4B7A0F2F_62FA_4C67_AB8B_8DE471779F2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SdkCallTrace.h"//������ʾ������SDK���ú�������־��Ϣ
/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg dialog

//���������SDK����lib�ļ���·���������Ŀ��������޸ġ�
#include "CameraApi.h"	 
#ifdef _WIN64
#pragma comment(lib, "..\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "..\\MVCAMSDK.lib")
#endif
/*��������е��������SDK�ӿ���־��Ϣ*/
#define SDK_TRACE(_FUNC_,TXT) \
{\
	CameraSdkStatus status;\
	CString msg;\
	CString FuncName;\
	FuncName = #_FUNC_;\
	FuncName = FuncName.Left(FuncName.FindOneOf("("));\
\
	status = _FUNC_;\
	if (status != CAMERA_STATUS_SUCCESS)\
	{\
	msg.Format(gLanguage?"����:[%s] ����ʧ��!":"Function:[%s] return error",FuncName);\
	m_DlgLog.AppendLog(msg);\
	msg.Format(gLanguage?"������:%d. ��ο�CameraStatus.h�д��������ϸ����":"Error code:%d.refer to CameraStatus.h for more information",status);\
	m_DlgLog.AppendLog(msg);\
	}\
	else\
	{\
	msg.Format(gLanguage?"����:[%s] ���óɹ�!":"Function:[%s] success",FuncName);\
	m_DlgLog.AppendLog(msg);\
	msg.Format(gLanguage?"����:%s.":"Action:%s",TXT);\
	m_DlgLog.AppendLog(msg);\
	}\
	msg = "";\
	m_DlgLog.AppendLog(msg);\
}
class CAdvancedDlg : public CDialog
{
// Construction
public:
	CAdvancedDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAdvancedDlg)
	enum { IDD = IDD_ADVANCED_DIALOG_CN };
	CComboBox	m_FrameSpeedList;	//֡��ѡ���б�
	CComboBox	m_cClrTmpList;      //ɫ���б�ؼ�
	CSliderCtrl	m_cSldAeTarget;		//�Զ��ع�����Ŀ�������
	CSliderCtrl	m_cSldSaturation;	//���Ͷȵ��ڹ�����
	CSliderCtrl	m_cSldRedGain;		//��ɫ����������ڹ�����
	CSliderCtrl	m_cSldGreenGain;	//��ɫ����������ڹ�����
	CSliderCtrl	m_cSldGamma;		//gamma���ڹ�����
	CSliderCtrl	m_cSldExposureLines;//֡�ع��������ڹ�����
	CSliderCtrl	m_cSldSharppen;		//�񻯵��ڹ�����
	CSliderCtrl	m_cSldContrast;		//�Աȶȵ��ڹ�����
	CSliderCtrl	m_cSldBlueGain;		//��ɫ����������ڹ�����
	CSliderCtrl	m_cSldAnalogGain;	//ģ��������ڹ�����	
	CComboBox	m_cPresetLutList;	//Ԥ��LUT�б�
	CComboBox	m_cResolutionList;	//�ֱ����б�
	CStatic	m_cPreview;				//��ʾͼ��Ŀؼ�
	int		m_iTriggerModeSel;		//����ѡ��ģʽ
	int		m_iSnapfileTypeSel;		//ץ���ļ������ʽ
	int		m_iSnapshotResSel;		//ץ�·ֱ���ģʽ(���Ժ�Ԥ��ʱ��ͬ��Ҳ���Ե�������)
	int		m_iWbModeSel;			//��ƽ��ģʽ(��Ϊ�ֶ����Զ�����)
	int		m_iLutModeSel;			//LUT����ģʽ
	int		m_iExposureModeSel;		//�ع�ģʽ(��Ϊ�Զ����ֶ�����)
	int		m_iAntiflickFreqSel;	//�Զ��ع�ʱ��Ƶ����Ƶ��ѡ��(��Ϊ50��60��������)
	int		m_iCrossPositionX;		//ʮ�������������Xֵ
	int		m_iCrossPositionY;		//ʮ�������������Yֵ
	int		m_iHardwareTriggerDelayUs;	//Ӳ����ʱ����ʱ����λus
	int		m_iTriggerCount;		//����ģʽ�£�һ�δ����ź���������֡��
	int     m_iClrTempSel;          //��ǰѡ���ɫ�����б��е�������
	CString	m_sSnapFilePath;		//ץ��ͼ�񱣴��·��
	BOOL	m_bVflip;				//ͼ��ֱ�����־
	BOOL	m_bHflip;				//ͼ��ˮƽ�����־
	BOOL	m_bAntiFlick;			//��Ƶ��ʹ�ܱ�־
	BOOL	m_bNoiseReduce;			//����ʹ�ܱ�־
	BOOL	m_bDisplayWbWindow;		//��ƽ�ⴰ����ʾ��־
	BOOL	m_bDisplayAeWindow;		//�Զ��عⴰ����ʾ��־
	BOOL	m_bDisplayCrosshair;	//ʮ������ʾʹ�ܱ�־
	int		m_iAutoSnapCycleMs;		//�Զ�ץ�����ڣ���λ����
	int		m_iParamGroupSel;		//��ǰ�Ĳ�����������
	BOOL	m_bSaveParamOnExit;		//�˳�����ʱ���������ʹ�ܱ�־
	BOOL	m_bSaveParamOnSwitch;	//�л�������ʱ���������ʹ�ܱ�־
	UINT	m_uCrosshairColor;		//ʮ���ߵ���ɫ
	CSdkCallTrace m_DlgLog;			//��Ϣ��ʾ����
	BOOL	m_bNotOverlayOnSnap;	//ץ��ʱ�Ƿ����ʮ���ߵ���Ϣ
	tSdkCameraCapbility m_sCameraInfo; //�����������
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void MySetDlgItemText(UINT uItemID, LPCSTR pFmt, ...);

	// Generated message map functions
	//{{AFX_MSG(CAdvancedDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonRoiReset();
	afx_msg void OnSelchangeComboResolution();
	afx_msg void OnButtonSoftTrigOnce();
	afx_msg void OnButtonSnapshot();
	afx_msg void OnButtonAeWinSet();
	afx_msg void OnBtnWbOnce();
	afx_msg void OnBtnWbWinSet();
	afx_msg void OnButtonCustomResSnapshot();
	afx_msg void OnButtonBrowseSnapshot();
	afx_msg void OnRadioTriggerMode();
	afx_msg void OnChangeEdtTriggerCount();
	afx_msg void OnChangeEdtTriggerDelay();
	afx_msg void OnSetCrosshair();
	afx_msg void OnRadioExposureMode();
	afx_msg void OnChkAntiFlick();
	afx_msg void OnAntiFilckhz();
	afx_msg void OnCheckDisplayAeWin();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckNoiseReduce();
	afx_msg void OnButtonLutLoadCustom();
	afx_msg void OnButtonEditLut();
	afx_msg void OnSelchangeComboPresetLut();
	afx_msg void OnRadioLutMode();
	afx_msg void OnRadioWbMode();

#ifdef _WIN64
	afx_msg void OnTimer(UINT_PTR nIDEvent); 
#else
	afx_msg void OnTimer(UINT nIDEvent);
#endif
	
	afx_msg void OnClose();
	afx_msg void OnButtonSaveParam();
	afx_msg void OnButtonLoadDefault();
	afx_msg void OnButtonLoadParamFromFile();
	afx_msg void OnRadioChangeParamGroup();
	afx_msg void OnCheckDisWbWin();
	afx_msg void OnButtonCustomCt();
	afx_msg void OnSelchangeComboCt();
	afx_msg void OnSelchangeComboFrameSpeed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	CameraHandle    m_hCamera;	//������豸���|the handle of the camera we use
	tSdkFrameHead   m_sFrInfo;//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ

	int	            m_iDispFrameNum;//���ڼ�¼��ǰ�Ѿ���ʾ��ͼ��֡������
	float           m_fDispFps;//��ʾ֡��
	float           m_fCapFps;//����֡��
    tSdkFrameStatistic  m_sFrameCount;
    tSdkFrameStatistic  m_sFrameLast;
    int		        m_iTimeLast;
	 
	BYTE*           m_pFrameBuffer;//���ڽ�ԭʼͼ������ת��ΪRGB�Ļ��������ɼ��߳��У�
	BOOL	        m_bPause;//�Ƿ���ͣͼ��

    float           m_fAnalogGainStep;//�������������ڵĲ���ֵ,��ͬ�ͺŵ��������ֵ��һ��
	UINT            m_uiMaxExpTime;//����ع��ʱ����ֵ����λΪ ��,��ͬ�ͺŵ��������ֵ��һ��

	UINT            m_threadID;//ͼ��ץȡ�̵߳�ID
	HANDLE          m_hDispThread;//ͼ��ץȡ�̵߳ľ��
	BOOL            m_bExit;//����֪ͨͼ��ץȡ�߳̽���

	double			m_fExpTime;//��ǰ��֡�ع�ʱ�䣬��λΪus
	double			m_fExpLineTime;//��ǰ�����ع�ʱ�䣬��λΪus
	INT 			m_iAnalogGain;//��ǰ���ź�ģ������ֵ������ֵΪm_fAnalogGainStep

	BYTE*			m_pbImgBuffer;//��������ԭʼ��ʽ������RGB��ʽ���ͼ�����ݣ�ץ��ģʽ�£�
	
	tSdkImageResolution	m_tRoiResolution;//�Զ���ķֱ��ʣ�Ԥ��ģʽ�£�
	tSdkImageResolution m_tRoiResolutionSnapshot;//�Զ���ķֱ��ʣ�Ԥ��ģʽ�£�

    BOOL            m_bMonoSensor;//ΪTRUEʱ��ʾ�����Ϊ�ڰ����
	//�����ʼ��
	BOOL InitCamera();

	//��ʼ���ؼ�
	BOOL InitControls(tSdkCameraCapbility * pCameraInfo);

	//���¿ؼ�����ʾ
	VOID UpdateControls();

	//�����ع�ؼ�����ʾ
	VOID UpdateExposure();

	//����ļ�·���Ƿ���Ч
	BOOL CheckPath();
	
	//�ݹ鷽ʽ����һ������·��
	BOOL mkdirEx(const char* lpPath);
	afx_msg void OnBnClickedAutoSnap();
	BOOL m_bAutoSnapshot;
	afx_msg void OnEnChangeEditExptext();
	afx_msg void OnEnChangeEditGaintext();
	afx_msg void OnBnClickedChkHfip();
	afx_msg void OnBnClickedChkVflip();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDDLG_H__4B7A0F2F_62FA_4C67_AB8B_8DE471779F2C__INCLUDED_)
