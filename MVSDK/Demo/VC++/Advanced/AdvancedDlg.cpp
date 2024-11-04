// AdvancedDlg.cpp : implementation file
//
//BIG5 TRANS ALLOWED

#include "stdafx.h"
#include "Advanced.h"
#include "UdefClrTmp.h"
#include "shlwapi.h"
#include "malloc.h"
#pragma comment(lib,"shlwapi.lib")//������shlwapi���е�PathIsDirectory����


//API_LOAD_MAIN ֻ����һ���ļ��ж���һ��,������include "CameraApiLoad.h"֮ǰ���塣��DEMO����BaseDlg.h��inlucde "CameraApiLoad.h",���������ﶨ��ú�
//Do not define API_LOAD_MAIN twice,only one time in one of your file before include "CameraApiLoad.h".
//"CameraApiLoad.h" was included in  "BaseDlg.h",so define API_LOAD_MAIN here.
#define API_LOAD_MAIN 
#include "AdvancedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern int gLanguage;

/*
USE_CALLBACK_GRAB_IMAGE 
�����Ҫʹ�ûص������ķ�ʽ���ͼ�����ݣ���ע�ͺ궨��USE_CALLBACK_GRAB_IMAGE.
���ǵ�SDKͬʱ֧�ֻص��������������ýӿ�ץȡͼ��ķ�ʽ�����ַ�ʽ��������"�㿽��"���ƣ������ĳ̶ȵĽ���ϵͳ���ɣ���߳���ִ��Ч�ʡ�
��������ץȡ��ʽ�Ȼص������ķ�ʽ�������������ó�ʱ�ȴ�ʱ��ȣ����ǽ�����ʹ�� uiDisplayThread �еķ�ʽ
*/
//#define USE_CALLBACK_GRAB_IMAGE 

#ifdef USE_CALLBACK_GRAB_IMAGE
/*ͼ��ץȡ�ص�����*/
void _stdcall GrabImageCallback(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{
	
	CameraSdkStatus status;
	CAdvancedDlg *pThis = (CAdvancedDlg*)pContext;

	
	
	//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
	//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
	status = CameraImageProcess(pThis->m_hCamera, pFrameBuffer, pThis->m_pFrameBuffer,pFrameHead);
	
	//�ֱ��ʸı��ˣ���ˢ�±���
	if (pThis->m_sFrInfo.iWidth != pFrameHead->iWidth || pThis->m_sFrInfo.iHeight != pFrameHead->iHeight)
	{
		pThis->m_sFrInfo.iWidth = pFrameHead->iWidth;
		pThis->m_sFrInfo.iHeight = pFrameHead->iHeight;
		pThis->InvalidateRect(NULL);//�л��ֱ��ʴ�Сʱ������������
	}
	
	if(status == CAMERA_STATUS_SUCCESS && !pThis->m_bPause)
    {
    	//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
		CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, pFrameHead);
        CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pFrameBuffer, pFrameHead);//��������滻���û��Լ�����ʾ����
        pThis->m_iDispFrameNum++;
    }    
    
	memcpy(&pThis->m_sFrInfo,pFrameHead,sizeof(tSdkFrameHead));
	
}

#else 
/*ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CAdvancedDlg* 		pThis = (CAdvancedDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	

	while (!pThis->m_bExit)
    {   

		if(CameraGetImageBufferPriority(pThis->m_hCamera,&sFrameInfo,&pbyBuffer,1000,
			CAMERA_GET_IMAGE_PRIORITY_NEWEST) == CAMERA_STATUS_SUCCESS)
		{

			
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer,&sFrameInfo);
			
			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}
			
			if(status == CAMERA_STATUS_SUCCESS)
            {
            	//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
                CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
                pThis->m_iDispFrameNum++;
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
			//"Release the buffer which get from CameraSnapToBuffer or CameraGetImageBuffer"
            CameraReleaseImageBuffer(pThis->m_hCamera,pbyBuffer);
   
			memcpy(&pThis->m_sFrInfo,&sFrameInfo,sizeof(tSdkFrameHead));
		}
		
    }
	
	_endthreadex(0);
    return 0;
}
#endif



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg dialog

CAdvancedDlg::CAdvancedDlg(CWnd* pParent /*=NULL*/)
: CDialog(gLanguage == 0?IDD_ADVANCED_DIALOG:IDD_ADVANCED_DIALOG_CN, pParent)
, m_bAutoSnapshot(FALSE)
{
	//{{AFX_DATA_INIT(CAdvancedDlg)
	m_iTriggerModeSel = 0;
	m_iSnapfileTypeSel = 2;
	m_iSnapshotResSel = 0;
	m_iWbModeSel = 1;
	m_iLutModeSel = 0;
	m_iExposureModeSel = 0;
	m_iAntiflickFreqSel = 0;
	m_iCrossPositionX = 640;
	m_iCrossPositionY = 480;
	m_iHardwareTriggerDelayUs = 0;
	m_iTriggerCount = 1;
	m_sSnapFilePath = _T(".");
	m_bVflip = FALSE;
	m_bHflip = FALSE;
	m_bAntiFlick = TRUE;
	m_bNoiseReduce = FALSE;
	m_bDisplayWbWindow = FALSE;
	m_bDisplayAeWindow = FALSE;
	m_bDisplayCrosshair = FALSE;
	m_iAutoSnapCycleMs = 1000;
	m_iParamGroupSel = 0;
	m_bSaveParamOnExit = FALSE;
	m_bSaveParamOnSwitch = FALSE;
	m_uCrosshairColor = 0;
	m_hCamera = -1;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pbImgBuffer = NULL;
	m_pFrameBuffer = NULL;
	m_bNotOverlayOnSnap = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedDlg)
	DDX_Control(pDX, IDC_COMBO_FRAME_SPEED, m_FrameSpeedList);
	DDX_Control(pDX, IDC_COMBO_CT, m_cClrTmpList);
	DDX_Control(pDX, IDC_SLD_AE_TARGET, m_cSldAeTarget);
	DDX_Control(pDX, IDC_SLD_SATURATION, m_cSldSaturation);
	DDX_Control(pDX, IDC_SLD_RGAIN, m_cSldRedGain);
	DDX_Control(pDX, IDC_SLD_GGAIN, m_cSldGreenGain);
	DDX_Control(pDX, IDC_SLD_GAMMA, m_cSldGamma);
	DDX_Control(pDX, IDC_SLD_EXPTIME, m_cSldExposureLines);
	DDX_Control(pDX, IDC_SLD_EDGE_ENHANCE, m_cSldSharppen);
	DDX_Control(pDX, IDC_SLD_CONTRAST, m_cSldContrast);
	DDX_Control(pDX, IDC_SLD_BGAIN, m_cSldBlueGain);
	DDX_Control(pDX, IDC_SLD_ANALOG_GAIN, m_cSldAnalogGain);
	DDX_Control(pDX, IDC_COMBO_PRESET_LUT, m_cPresetLutList);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_cResolutionList);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_cPreview);
	DDX_Radio(pDX, IDC_RADIO5, m_iTriggerModeSel);
	DDX_Radio(pDX, IDC_RADIO1, m_iSnapfileTypeSel);
	DDX_Radio(pDX, IDC_RADIO_SNAPSHOT_RES, m_iSnapshotResSel);
	DDX_Radio(pDX, IDC_RADIO_WB_MODE, m_iWbModeSel);
	DDX_Radio(pDX, IDC_RADIO_LUT_MODE_GEN, m_iLutModeSel);
	DDX_Radio(pDX, IDC_RADIO_EXPOSURE_AUTO, m_iExposureModeSel);
	DDX_Radio(pDX, IDC_RDO_50HZ, m_iAntiflickFreqSel);
	DDX_Text(pDX, IDC_EDIT_CROSS_X, m_iCrossPositionX);
	DDX_Text(pDX, IDC_EDIT_CROSS_Y, m_iCrossPositionY);
	DDX_Text(pDX, IDC_EDT_TRIGGER_DELAY, m_iHardwareTriggerDelayUs);
	DDX_Text(pDX, IDC_EDT_TRIGGER_COUNT, m_iTriggerCount);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_sSnapFilePath);
	DDX_Check(pDX, IDC_CHK_VFLIP, m_bVflip);
	DDX_Check(pDX, IDC_CHK_HFIP, m_bHflip);
	DDX_Check(pDX, IDC_CHK_ANTI_FLICK, m_bAntiFlick);
	DDX_Check(pDX, IDC_CHECK_NOISE_REDUCE, m_bNoiseReduce);
	DDX_Check(pDX, IDC_CHECK_DIS_WB_WIN, m_bDisplayWbWindow);
	DDX_Check(pDX, IDC_CHECK_DIS_AE_WIN, m_bDisplayAeWindow);
	DDX_Check(pDX, IDC_CHECK_CROSS_OVERLAY, m_bDisplayCrosshair);
	DDX_Text(pDX, IDC_EDIT_TIME, m_iAutoSnapCycleMs);
	DDX_Radio(pDX, IDC_RADIO11, m_iParamGroupSel);
	DDX_Check(pDX, IDC_CHECK_SAVE_ON_EXIT, m_bSaveParamOnExit);
	DDX_Check(pDX, IDC_CHECK_SAVE_ON_SWITCH, m_bSaveParamOnSwitch);
	DDX_Text(pDX, IDC_EDIT_CROSS_COLOR, m_uCrosshairColor);
	DDV_MinMaxUInt(pDX, m_uCrosshairColor, 0, 16777215);
	DDX_Check(pDX, IDC_CHECK_IGNORE_OVERLAY_SNAPSHOT, m_bNotOverlayOnSnap);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_AUTO_SNAP, m_bAutoSnapshot);
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CDialog)
	//{{AFX_MSG_MAP(CAdvancedDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ROI_RESET, OnButtonRoiReset)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, OnSelchangeComboResolution)
	ON_BN_CLICKED(IDC_BUTTON_SOFT_TRIG_ONCE, OnButtonSoftTrigOnce)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, OnButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_AE_WIN_SET, OnButtonAeWinSet)
	ON_BN_CLICKED(IDC_BTN_WB_ONCE, OnBtnWbOnce)
	ON_BN_CLICKED(IDC_BTN_WB_WIN_SET, OnBtnWbWinSet)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_RES_SNAPSHOT, OnButtonCustomResSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowseSnapshot)
	ON_BN_CLICKED(IDC_RADIO5, OnRadioTriggerMode)
	ON_EN_CHANGE(IDC_EDT_TRIGGER_COUNT, OnChangeEdtTriggerCount)
	ON_EN_CHANGE(IDC_EDT_TRIGGER_DELAY, OnChangeEdtTriggerDelay)
	ON_EN_CHANGE(IDC_EDIT_CROSS_X, OnSetCrosshair)
	ON_BN_CLICKED(IDC_RADIO_EXPOSURE_MANUAL, OnRadioExposureMode)
	ON_BN_CLICKED(IDC_CHK_ANTI_FLICK, OnChkAntiFlick)
	ON_BN_CLICKED(IDC_RDO_50HZ, OnAntiFilckhz)
	ON_BN_CLICKED(IDC_CHECK_DIS_AE_WIN, OnCheckDisplayAeWin)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_NOISE_REDUCE, OnCheckNoiseReduce)
	ON_BN_CLICKED(IDC_BUTTON_LUT_LOAD, OnButtonLutLoadCustom)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_LUT, OnButtonEditLut)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET_LUT, OnSelchangeComboPresetLut)
	ON_BN_CLICKED(IDC_RADIO_LUT_MODE_GEN, OnRadioLutMode)
	ON_BN_CLICKED(IDC_RADIO_WB_MANUAL, OnRadioWbMode)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PARAM, OnButtonSaveParam)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DEFAULT, OnButtonLoadDefault)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PARAM_FROM_FILE, OnButtonLoadParamFromFile)
	ON_BN_CLICKED(IDC_RADIO11, OnRadioChangeParamGroup)
	ON_BN_CLICKED(IDC_CHECK_DIS_WB_WIN, OnCheckDisWbWin)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_CT, OnButtonCustomCt)
	ON_CBN_SELCHANGE(IDC_COMBO_CT, OnSelchangeComboCt)
	ON_BN_CLICKED(IDC_RADIO12, OnRadioChangeParamGroup)
	ON_BN_CLICKED(IDC_RADIO13, OnRadioChangeParamGroup)
	ON_BN_CLICKED(IDC_RADIO6, OnRadioTriggerMode)
	ON_BN_CLICKED(IDC_RADIO7, OnRadioTriggerMode)
	ON_BN_CLICKED(IDC_RADIO8, OnRadioTriggerMode)
	ON_BN_CLICKED(IDC_RADIO9, OnRadioTriggerMode)
	ON_BN_CLICKED(IDC_RADIO10, OnRadioTriggerMode)
	ON_EN_CHANGE(IDC_EDIT_CROSS_Y, OnSetCrosshair)
	ON_BN_CLICKED(IDC_CHECK_CROSS_OVERLAY, OnSetCrosshair)
	ON_BN_CLICKED(IDC_RADIO_EXPOSURE_AUTO, OnRadioExposureMode)
	ON_BN_CLICKED(IDC_RADIO_LUT_MODE_PREV, OnRadioLutMode)
	ON_BN_CLICKED(IDC_RADIO_LUT_MODE_UDEF, OnRadioLutMode)
	ON_BN_CLICKED(IDC_RADIO_WB_MODE, OnRadioWbMode)
	ON_EN_CHANGE(IDC_EDIT_CROSS_COLOR, OnSetCrosshair)
	ON_BN_CLICKED(IDC_RADIO14, OnRadioChangeParamGroup)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAME_SPEED, OnSelchangeComboFrameSpeed)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_AUTO_SNAP, OnBnClickedAutoSnap)
	ON_EN_CHANGE(IDC_EDIT_EXPTEXT, OnEnChangeEditExptext)
	ON_EN_CHANGE(IDC_EDIT_GAINTEXT, OnEnChangeEditGaintext)
	ON_BN_CLICKED(IDC_CHK_HFIP, OnBnClickedChkHfip)
	ON_BN_CLICKED(IDC_CHK_VFLIP, OnBnClickedChkVflip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg message handlers

BOOL CAdvancedDlg::InitControls(tSdkCameraCapbility * pCameraInfo)
{
	int i;
	tSdkImageResolution sResolution;

	m_fAnalogGainStep = pCameraInfo->sExposeDesc.fAnalogGainStep;
	m_uiMaxExpTime = pCameraInfo->sExposeDesc.uiExposeTimeMax;

	//��ʼ����������ȡֵ��Χ
	m_cSldAeTarget.SetRange(pCameraInfo->sExposeDesc.uiTargetMin,pCameraInfo->sExposeDesc.uiTargetMax,FALSE);//�Զ��ع��Ŀ�����ȷ�Χ
    m_cSldAnalogGain.SetRange(pCameraInfo->sExposeDesc.uiAnalogGainMin,pCameraInfo->sExposeDesc.uiAnalogGainMax,FALSE);//�ع�ģ��������趨��Χ
    m_cSldExposureLines.SetRange(pCameraInfo->sExposeDesc.uiExposeTimeMin,pCameraInfo->sExposeDesc.uiExposeTimeMax,FALSE);//�ع�ʱ����趨��Χ������CMOS�������λΪ��
    m_cSldBlueGain.SetRange(pCameraInfo->sRgbGainRange.iBGainMin,pCameraInfo->sRgbGainRange.iBGainMax,FALSE);//��ɫͨ�����������趨��Χ��Ϊ����ߵ��⾫�ȣ�������100��,ʵ�ʱ���Ϊ�趨ֵ����100
    m_cSldGreenGain.SetRange(pCameraInfo->sRgbGainRange.iGGainMin,pCameraInfo->sRgbGainRange.iGGainMax,FALSE);//��ɫͨ�����������趨��Χ
    m_cSldRedGain.SetRange(pCameraInfo->sRgbGainRange.iRGainMin,pCameraInfo->sRgbGainRange.iRGainMax,FALSE);//��ɫͨ�����������趨��Χ
    m_cSldGamma.SetRange(pCameraInfo->sGammaRange.iMin,pCameraInfo->sGammaRange.iMax,FALSE);//٤����ⷶΧ
    m_cSldSharppen.SetRange(pCameraInfo->sSharpnessRange.iMin,pCameraInfo->sSharpnessRange.iMax,FALSE);//�񻯵��ⷶΧ
    m_cSldContrast.SetRange(pCameraInfo->sContrastRange.iMin,pCameraInfo->sContrastRange.iMax,FALSE);//�Աȶȵ��ⷶΧ
    m_cSldSaturation.SetRange(pCameraInfo->sSaturationRange.iMin,pCameraInfo->sSaturationRange.iMax,FALSE);//���Ͷȵ��ⷶΧ

    //��ʼ���ֱ����б���SDK�л�ȡ����ͬ�ͺŵ���������б�һ��
	m_cResolutionList.ResetContent();
	for (i = 0; i<pCameraInfo->iImageSizeDesc; i++)
    {
		m_cResolutionList.InsertString(-1,pCameraInfo->pImageSizeDesc[i].acDescription);
    }

	//����һ���Զ���ֱ��ʵ�ѡ��,�������б������
	m_cResolutionList.InsertString(-1,"Custom(ROI)");
	
	//��������ʼ����ķֱ���
	SDK_TRACE(CameraGetImageResolution(m_hCamera,&sResolution),gLanguage?"��õ�ǰԤ���ֱ���":"Get current preview resolution");
	memcpy(&m_tRoiResolution,&sResolution,sizeof(tSdkImageResolution));//���Զ���ֱ��ʱ������������л�ʱʹ��
	m_tRoiResolution.iIndex = 0XFF;
		
	SDK_TRACE(CameraGetResolutionForSnap(m_hCamera,&sResolution),gLanguage?"���ץ��ģʽ�µķֱ���":"Get current snapshot resolution");

	if (sResolution.iWidth == 0 || sResolution.iHeight == 0)//���Ϊ0��ʾ��Ԥ���ֱ�����ͬ��Ԥ���ֱ����л��󱣳ָ���
	{
		m_iSnapshotResSel = 0;
	}
	else//��Ԥ��ʱ�ķֱ��ʲ�ͬ��ץ��ʱ��SDK���Զ��л��ֱ��ʽ���ץ�ģ���ɺ����л���Ԥ���ķֱ���
	{
		m_iSnapshotResSel = 1;
	}

	//��ʼ��Ԥ��LUT��
	m_cPresetLutList.ResetContent();
	for (i = 0;i < pCameraInfo->iPresetLut;i++)
	{
		m_cPresetLutList.InsertString(-1,pCameraInfo->pPresetLutDesc[i].acDescription);
	}
	
	//��ʼ������ģʽ
	for (i = 0;i < pCameraInfo->iTriggerDesc;i++)
	{
		GetDlgItem(IDC_RADIO5+i)->SetWindowText(pCameraInfo->pTriggerDesc[i].acDescription);
		GetDlgItem(IDC_RADIO5+i)->EnableWindow(TRUE);
	}

    if (m_bMonoSensor)
    {
        //��ǰ���Ϊ�ڰ������������ɫ��صĿؼ�
        GetDlgItem(IDC_RADIO_WB_MANUAL)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_WB_MODE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_WB_WIN_SET)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK_DIS_WB_WIN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_WB_ONCE)->EnableWindow(FALSE);
        GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(FALSE);
        GetDlgItem(IDC_STC_RTEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_STC_SATURATIONTEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_SLD_SATURATION)->EnableWindow(FALSE);
        GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(FALSE);
        GetDlgItem(IDC_STC_GTEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_STC_BTEXT)->EnableWindow(FALSE);
        GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO_CT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_CUSTOM_CT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STC_RGAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATICS_SAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLD_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WB_WIN_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DIS_WB_WIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CUSTOM_CT)->EnableWindow(FALSE);

    }
    else
    {
         //��ʼ��ɫ��ѡ���б�
        m_cClrTmpList.ResetContent();

    	m_cClrTmpList.InsertString(-1,"�Զ�");
    	
    	for (i = 0; i < pCameraInfo->iClrTempDesc;i++)
    	{
    		m_cClrTmpList.InsertString(-1,pCameraInfo->pClrTempDesc[i].acDescription);
    	}

    	m_cClrTmpList.InsertString(-1,"�Զ���");
    }
   

	//����֡�ʵ����÷��� 2013-6-22
	m_FrameSpeedList.ResetContent();
	for (i = 0; i < pCameraInfo->iFrameSpeedDesc;i++)
	{
		m_FrameSpeedList.InsertString(-1,pCameraInfo->pFrameSpeedDesc[i].acDescription);
	}

	return TRUE;
}


VOID CAdvancedDlg::UpdateControls()//��������ĸ��ֲ������趨ֵ�������½����ϸ����ؼ�
{

	BOOL bTemp;
	INT	 iTemp; 
	CString sTemp;
	UINT uTemp;
	tSdkImageResolution sResolution;

	uTemp = 0;
	//���·ֱ����б���Ӧ��ǰ�ķֱ��ʣ������ROIģʽ����ʹ��ROI���ð�ť
	SDK_TRACE(CameraGetImageResolution(m_hCamera,&sResolution),gLanguage?"��õ�ǰԤ���ֱ���":"Get current preview resolution");
	if (sResolution.iIndex == 0XFF)//0XFF��ʾ���Զ���ֱ���(ROI)
	{
		m_cResolutionList.SetCurSel(m_cResolutionList.GetCount() - 1);
		GetDlgItem(IDC_BUTTON_ROI_RESET)->EnableWindow(TRUE);			
	}
	else
	{
		m_cResolutionList.SetCurSel(sResolution.iIndex);
		GetDlgItem(IDC_BUTTON_ROI_RESET)->EnableWindow(FALSE);
	}
	
	//���ץ�ķֱ���
	SDK_TRACE(CameraGetResolutionForSnap(m_hCamera,&m_tRoiResolutionSnapshot),gLanguage?"���ץ��ͨ���ķֱ�������":"Get the resolution of snapshot");

	//�Ƿ�ʹ���Զ��ع�,������ģʽ��ʹ����ؿؼ�
	SDK_TRACE(CameraGetAeState(m_hCamera,&bTemp),gLanguage?"����ع��ģʽ":"Get mode of exposure");
	m_iExposureModeSel = bTemp?0:1;
	
	GetDlgItem(IDC_SLD_AE_TARGET)->EnableWindow(bTemp);
	GetDlgItem(IDC_CHK_ANTI_FLICK)->EnableWindow(bTemp);
	GetDlgItem(IDC_RDO_50HZ)->EnableWindow(bTemp);
	GetDlgItem(IDC_RDO_60HZ)->EnableWindow(bTemp);
	GetDlgItem(IDC_CHECK_DIS_AE_WIN)->EnableWindow(bTemp);
	GetDlgItem(IDC_BUTTON_AE_WIN_SET)->EnableWindow(bTemp);
	
	GetDlgItem(IDC_STC_GAINTEXT_MIN)->EnableWindow(!bTemp);
	GetDlgItem(IDC_STC_GAINTEXT_MAX)->EnableWindow(!bTemp);
	GetDlgItem(IDC_EDIT_GAINTEXT)->EnableWindow(!bTemp);
	GetDlgItem(IDC_STC_EXPTEXT_MIN)->EnableWindow(!bTemp);
	GetDlgItem(IDC_STC_EXPTEXT_MIN)->EnableWindow(!bTemp);
	GetDlgItem(IDC_EDIT_EXPTEXT)->EnableWindow(!bTemp);
	GetDlgItem(IDC_SLD_ANALOG_GAIN)->EnableWindow(!bTemp);
	GetDlgItem(IDC_SLD_EXPTIME)->EnableWindow(!bTemp);
	
	if (!bTemp)//�ֶ��ع⣬���ȡ��ǰ���ع�ֵ��ģ������ֵ
	{
		UpdateExposure();
	}

	//�Զ��ع�ʱ�Ƿ�Ƶ��
	SDK_TRACE(CameraGetAntiFlick(m_hCamera,&m_bAntiFlick),gLanguage?"��ÿ�Ƶ����״̬":"Get the state of anti-flick");

	//��Ƶ����Ƶ��
	SDK_TRACE(CameraGetLightFrequency(m_hCamera,&iTemp),gLanguage?"��ÿ�Ƶ����Ƶ������":"Get the frequency of anti-flick");
	m_iAntiflickFreqSel = iTemp == LIGHT_FREQUENCY_50HZ?0:1;

	//�Զ��ع������Ŀ��ֵ
	SDK_TRACE(CameraGetAeTarget(m_hCamera,&iTemp),gLanguage?"����Զ��ع�ģʽ������Ŀ��ֵ":"Get the target of ligheness on AE mode");
	sTemp.Format("%d",iTemp);
	GetDlgItem(IDC_STC_AETARGETTEXT)->SetWindowText(sTemp);
	m_cSldAeTarget.SetPos(iTemp);
	
	
	if (!m_bMonoSensor)//do not update on monochrome camera
	{
		//�Զ��ع�Ĳο�������ʾ״̬
		SDK_TRACE(CameraIsAeWinVisible(m_hCamera,&m_bDisplayAeWindow),gLanguage?"����Զ��عⴰ�ڵ���ʾ״̬":"Get the visible state of AE window");
		
		//���Ͷȵ��趨ֵ
		SDK_TRACE(CameraGetSaturation(m_hCamera,&iTemp),gLanguage?"��ñ��Ͷ��趨ֵ":"Get the value of saturation");
		m_cSldSaturation.SetPos(iTemp);
		sTemp.Format("%d",iTemp);
		GetDlgItem(IDC_STC_SATURATIONTEXT)->SetWindowText(sTemp);
		
		//R G B ��ͨ��������趨ֵ
		INT iRgain,iGgain,iBgain;
		SDK_TRACE(CameraGetGain(m_hCamera, &iRgain, &iGgain, &iBgain),gLanguage?"��ú졢�̡�������������":"Get R G B digital gain");
		
		m_cSldRedGain.SetPos(iRgain);
		sTemp.Format("X%4.2f",((float)iRgain)/100.0);
		GetDlgItem(IDC_STC_RTEXT)->SetWindowText(sTemp);
		
		m_cSldGreenGain.SetPos(iGgain);
		sTemp.Format("X%4.2f",((float)iGgain)/100.0);
		GetDlgItem(IDC_STC_GTEXT)->SetWindowText(sTemp);
		
		m_cSldBlueGain.SetPos(iBgain);
		sTemp.Format("X%4.2f",((float)iBgain)/100.0);
		GetDlgItem(IDC_STC_BTEXT)->SetWindowText(sTemp);
		
		//��ƽ��ģʽ�Լ���ƽ�ⴰ�ڵ���ʾ״̬
		SDK_TRACE(CameraGetWbMode(m_hCamera,&bTemp),gLanguage?"��ð�ƽ��ģʽ":"Get WB mode");
		
		GetDlgItem(IDC_BTN_WB_WIN_SET)->EnableWindow(!bTemp);//��Щ�ؼ����ݲ�ͬ��ģʽ�����ú�ʹ��
		GetDlgItem(IDC_CHECK_DIS_WB_WIN)->EnableWindow(!bTemp);
		GetDlgItem(IDC_BTN_WB_ONCE)->EnableWindow(!bTemp);
		GetDlgItem(IDC_SLD_RGAIN)->EnableWindow(!bTemp);
		GetDlgItem(IDC_SLD_GGAIN)->EnableWindow(!bTemp);
		GetDlgItem(IDC_SLD_BGAIN)->EnableWindow(!bTemp);
		GetDlgItem(IDC_SLD_SATURATION)->EnableWindow(!bTemp);
		
		SDK_TRACE(CameraIsWbWinVisible(m_hCamera,&m_bDisplayWbWindow),gLanguage?"��ð�ƽ�ⴰ�ڵ���ʾ״̬":"Get the visible state of WB window");

		//���ɫ��ģʽ
		int iClrTempMode;
		
		SDK_TRACE(CameraGetClrTempMode(m_hCamera,&iClrTempMode),gLanguage?"��õ�ǰɫ��ģʽ":"Get the current color temperature");
		
		if (iClrTempMode == CT_MODE_AUTO)//�Զ�ģʽ
		{
			m_cClrTmpList.SetCurSel(0);
		}
		else if (iClrTempMode == CT_MODE_USER_DEF)//�Զ���ģʽ
		{
			m_cClrTmpList.SetCurSel(m_cClrTmpList.GetCount() - 1);
		}
		else//Ԥ��ģʽ
		{
			SDK_TRACE(CameraGetPresetClrTemp(m_hCamera,&m_iClrTempSel),gLanguage?"���ָ����ɫ��ģʽ������":"Get the index of selected color temperature");
			m_cClrTmpList.SetCurSel(m_iClrTempSel + 1);
		}

		//���Զ���ɫ��ģʽʱ��ʾ���ð�ť
		::ShowWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CT)->m_hWnd, (m_cClrTmpList.GetCurSel() == (m_cClrTmpList.GetCount() - 1)));
		::EnableWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CT)->m_hWnd, (m_cClrTmpList.GetCurSel() == (m_cClrTmpList.GetCount() - 1)));
	}

	
	//ͼ����״̬
	SDK_TRACE(CameraGetMirror(m_hCamera, MIRROR_DIRECTION_HORIZONTAL, &m_bHflip),gLanguage?"���ͼ��ˮƽ����״̬":"Get image horizontal mirror state");
    SDK_TRACE(CameraGetMirror(m_hCamera, MIRROR_DIRECTION_VERTICAL, &m_bVflip),gLanguage?"���ͼ��ֱ����״̬":"Get image vertical mirror state");

	//�Ƿ�������
	SDK_TRACE(CameraGetNoiseFilterState(m_hCamera,&m_bNoiseReduce),gLanguage?"���ͼ����ʹ��״̬ ":"Get the state of 2d-noise filter");

	//�񻯵��趨ֵ
	SDK_TRACE(CameraGetSharpness(m_hCamera, &iTemp),gLanguage?"����񻯵��趨ֵ":"Get the value of sharpness");
    m_cSldSharppen.SetPos(iTemp);
	sTemp.Format("%d",iTemp);
	GetDlgItem(IDC_STC_SHARPNESS)->SetWindowText(sTemp);

	//����ģʽ�����������ģʽ����ʹ��������ť
	SDK_TRACE(CameraGetTriggerMode(m_hCamera, &m_iTriggerModeSel),gLanguage?"�������Ĵ���ģʽ":"Get the trigger mode of the camera");
	GetDlgItem(IDC_BUTTON_SOFT_TRIG_ONCE)->EnableWindow(m_iTriggerModeSel == SOFT_TRIGGER);
	GetDlgItem(IDC_EDT_TRIGGER_COUNT)->EnableWindow(m_iTriggerModeSel != CONTINUATION);
	GetDlgItem(IDC_EDT_TRIGGER_DELAY)->EnableWindow(m_iTriggerModeSel >= EXTERNAL_TRIGGER);

	//һ�δ�����֡����Ӳ����������ʱ
	SDK_TRACE(CameraGetTriggerCount(m_hCamera,&m_iTriggerCount),gLanguage?"���Ӳ���������ʱ�趨":"Get trigger delay time");
		
	//��һ��ʮ���ߵ�����λ���Լ���ɫ����ʾ״̬,SDKʵ��֧�ֶ���ʮ���ߣ�DEMOֻ���˵�һ��
	CameraGetCrossLine(m_hCamera,0,&m_iCrossPositionX,&m_iCrossPositionY,&m_uCrosshairColor,&m_bDisplayCrosshair);
	
	//LUT(���任)��ģʽ,������Զ���ģʽ����ʹ�� "Load from file"��ť���������
	SDK_TRACE(CameraGetLutMode(m_hCamera,&iTemp),gLanguage?"���LUTģʽ�趨ֵ":"Get LUT mode");
	GetDlgItem(IDC_BUTTON_LUT_LOAD)->EnableWindow(iTemp == LUTMODE_USER_DEF);

	//٤����趨ֵ
	SDK_TRACE(CameraGetGamma(m_hCamera,&iTemp),gLanguage?"���٤����趨ֵ":"Get the gamma value");
	m_cSldGamma.SetPos(iTemp);
	sTemp.Format("%4.2f",iTemp/100.f);
	GetDlgItem(IDC_STC_GAMMATEXT)->SetWindowText(sTemp);

	//�Աȶȵ��趨ֵ
	SDK_TRACE(CameraGetContrast(m_hCamera,&iTemp),gLanguage?"��öԱȶȵ��趨ֵ":"Get the contrast value");
	m_cSldContrast.SetPos(iTemp);
	sTemp.Format("%d",iTemp);
	GetDlgItem(IDC_STC_CONTRASTTEXT)->SetWindowText(sTemp);

	//Get the select of LUT table in preset lut mode
	SDK_TRACE(CameraGetLutPresetSel(m_hCamera,&iTemp),gLanguage?"���ѡ����Ԥ��LUT������ֵ":"Get the selected LUT index on preset table");
	m_cPresetLutList.SetCurSel(iTemp);

	//��õ�ǰ֡���ٶ�����
	SDK_TRACE(CameraGetFrameSpeed(m_hCamera,&iTemp),gLanguage?"���֡������":"Get the speed mode");
	m_FrameSpeedList.SetCurSel(iTemp);
    	    
	UpdateData(FALSE);	
}

VOID CAdvancedDlg::UpdateExposure()//�����ع�ʱ���ģ���������ʾ
{
	CString sTemp;

	CameraGetExposureTime(m_hCamera, &m_fExpTime);//���֡�ع�ʱ�䣬��λΪus

	double fMinExp, fMaxExp;
	CameraGetExposureTimeRange(m_hCamera, &fMinExp, &fMaxExp, &m_fExpLineTime);//����ع�ʱ�䷶Χ����λΪus.��ͬ�ֱ����£����ع�ʱ�䲻һ����
																		//��ˣ��л��ֱ��ʺ���Ҫ���´˲���

	m_cSldExposureLines.SetPos((int)(m_fExpTime/m_fExpLineTime));
	sTemp.Format("%4.4f",((float)m_fExpTime)/1000.0);
	MySetDlgItemText(IDC_EDIT_EXPTEXT, sTemp);

	sTemp.Format("%.4f", (float)fMinExp / 1000.0f);
	GetDlgItem(IDC_STC_EXPTEXT_MIN)->SetWindowText(sTemp);

	sTemp.Format("%.4f", (float)fMaxExp / 1000.0f);
	GetDlgItem(IDC_STC_EXPTEXT_MAX)->SetWindowText(sTemp);
	
	CameraGetAnalogGain(m_hCamera, &m_iAnalogGain);
	m_cSldAnalogGain.SetPos(m_iAnalogGain);
	sTemp.Format("%.4f",m_fAnalogGainStep*m_iAnalogGain);
	MySetDlgItemText(IDC_EDIT_GAINTEXT, sTemp);

	sTemp.Format("X%4.3f",m_fAnalogGainStep*m_sCameraInfo.sExposeDesc.uiAnalogGainMin);
	GetDlgItem(IDC_STC_GAINTEXT_MIN)->SetWindowText(sTemp);

	sTemp.Format("X%4.3f",m_fAnalogGainStep*m_sCameraInfo.sExposeDesc.uiAnalogGainMax);
	GetDlgItem(IDC_STC_GAINTEXT_MAX)->SetWindowText(sTemp);
}



BOOL CAdvancedDlg::InitCamera()
{	

	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	
	
	CameraSdkInit(gLanguage);//gLanguage = 0 init for English ,else init for Chinese

	//ö���豸������豸�б�
	iCameraNums = 10;//����CameraEnumerateDeviceǰ��������iCameraNums = 10����ʾ���ֻ��ȡ10���豸�������Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ
	if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("û���ҵ����!");
		return FALSE;
	}
	
	//��ʾ���У�����ֻ����������һ���������ˣ�ֻ��ʼ����һ�������(-1,-1)��ʾ�����ϴ��˳�ǰ����Ĳ���������ǵ�һ��ʹ�ø�����������Ĭ�ϲ���.
	//In this demo ,we just init the first camera.
	if ((status = CameraInit(&sCameraList[0],-1,-1,&m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		//msg.Format("Failed to init the camera! Error code is %d",status);
		msg.Format("�����ʼ��ʧ��! ������ %d",status);
		MessageBox(msg+"��ԭ����" + CameraGetErrorString(status));
		return FALSE;
	}
	
	//��ø��������������
	//Get properties description for this camera.
	SDK_TRACE(CameraGetCapability(m_hCamera,&m_sCameraInfo),gLanguage?"����������������":"Get the capability of the camera");
	m_pFrameBuffer = CameraAlignMalloc(m_sCameraInfo.sResolutionRange.iWidthMax*m_sCameraInfo.sResolutionRange.iHeightMax*4,16);	
	m_pbImgBuffer =  CameraAlignMalloc(m_sCameraInfo.sResolutionRange.iWidthMax*m_sCameraInfo.sResolutionRange.iHeightMax*4,16);

	m_bMonoSensor = m_sCameraInfo.sIspCapacity.bMonoSensor; 
	ASSERT(m_pFrameBuffer);

	if (m_sCameraInfo.sIspCapacity.bMonoSensor)
	{
		// �ڰ�������8λ�Ҷ�
		// Black and white camera output 8-bit grayscale
		CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
	}

	//���û�õ�������Ϣ����ʼ���ؼ�
	InitControls(&m_sCameraInfo);
	//ʹ��SDK��װ�õ���ʾ�ӿ�
	//Use  SDK to display camera images.
	SDK_TRACE(CameraDisplayInit(m_hCamera,m_cPreview.GetSafeHwnd()),gLanguage?"��ʼ����ʾ�ӿ�":"init display module");
	m_cPreview.GetClientRect(&rect);
    //Set display window size
	SDK_TRACE(CameraSetDisplaySize(m_hCamera,rect.right - rect.left,rect.bottom - rect.top),gLanguage?"�趨��ʾ���ڵĴ�С":"Set display size");
	
	#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
	//Set the callback for image capture
	SDK_TRACE(CameraSetCallbackFunction(m_hCamera,GrabImageCallback,(PVOID)this,NULL),gLanguage?"����ͼ��ץȡ�Ļص�����":"Set image grab call back function");
	#else
	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0,  &m_threadID);
	ASSERT (m_hDispThread); 
	SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
	#endif

	SDK_TRACE(CameraPlay(m_hCamera),gLanguage?"��ʼ�ɼ�ͼ��!":"Start preview");

	m_bPause = FALSE;

	return TRUE;
}


BOOL CAdvancedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_DlgLog.Create(IDD_DIALOG_LOG,this);//����һ����Ϣ��������ʾ��־
	do 
	{
		m_DlgLog.ShowWindow(SW_SHOW);
		
// 		m_DlgLog.AppendLog("Advanced Demo start");
// 		m_DlgLog.AppendLog("LoadSdkApi was called to load SDK api from MVCAMSDK.dll");
// 		m_DlgLog.AppendLog("LoadSdkApi is open source in CameraApiLoad.h ");
// 		m_DlgLog.AppendLog("It shows how to load the api from MVCAMSDK.dll,");
// 		m_DlgLog.AppendLog("you can also use your own way to load MVCAMSDK.dll");


		//Init SDK��0:Ӣ�İ� 1:���İ� �������������������Ϣ��SDK���ɵ��豸���ý�����
		SDK_TRACE(CameraSdkInit(gLanguage),gLanguage?"��ʼ��SDK":"Int SDK");
		//��ʼ�����
		if (!InitCamera())
		{
			break;
		}
		
		//���½���ؼ���ʾ
		UpdateControls();
		
		
		//ʹ��һ����ʱ��������֡�ʡ����ڸ��½���Ԫ��
		SetTimer(0,1000,NULL);
		
		return TRUE;

	} while(0);
	
	//û���ҵ�������߳�ʼ��ʧ�ܣ��˳�����
	EndDialog(0);
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CAdvancedDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdvancedDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAdvancedDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAdvancedDlg::OnButtonRoiReset() //�Զ���һ���ֱ���
{
	// TODO: Add your control notification handler code here
	tSdkImageResolution sImageSize;
	CString				msg;
	CameraSdkStatus		status;
	
	//�Ȼ��һ�ε�ǰ�ֱ��ʣ�Ϊ����CameraCustomizeResolution��׼��
	SDK_TRACE(CameraGetImageResolution(m_hCamera,&sImageSize),gLanguage?"��õ�ǰ�ķֱ���":"Get current resolution");
	
	//CameraCustomizeResolution�ṩ��һ�ֿ��ӻ��ķ�ʽ���Զ���ֱ��ʡ�
	//������Ѿ��Էֱ��ʵ����ù������Ϥ�������ֱ��ʹ��CameraSetImageResolution
	//���������Զ���ķֱ��ʣ���������CameraGetImageResolution��CameraCustomizeResolution��
	if (CameraCustomizeResolution(m_hCamera,&sImageSize) == CAMERA_STATUS_SUCCESS)
	{
		msg.Format("Function:[CameraCustomizeResolution] SUCCESS!");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Get a [ROI] resolution in a visualization way");
		m_DlgLog.AppendLog(msg);
		m_DlgLog.AppendLog(" ");

		//�������ʹ�õ�CameraCustomizeResolution����õ�ROI�ֱ��ʣ�CameraSetImageResolution�ǿ϶���
		//���سɹ���CameraCustomizeResolution���Զ�����ROI����Ĳ�������Ч�ԡ�
		//�������ֱ��ʹ��CameraSetImageResolution���趨�Զ���ֱ��ʣ���ο�SDK�ֲ���
		//�й��Զ���ֱ��ʲ������޶���Χ
		if ((status = CameraSetImageResolution(m_hCamera,&sImageSize))  == CAMERA_STATUS_SUCCESS)
		{
			msg.Format("Function:[CameraSetImageResolution] SUCCESS!");
			m_DlgLog.AppendLog(msg);
			msg.Format("Description:%s.","Set the new resolution");
			m_DlgLog.AppendLog(msg);
			m_DlgLog.AppendLog(" ");
			
			//������������Ԥ��ģʽ�л����Զ���ʱ�á�
			memcpy(&m_tRoiResolution,&sImageSize,sizeof(tSdkImageResolution));
		}
		
	}
}

void CAdvancedDlg::OnSelchangeComboResolution() 
{
	// TODO: Add your control notification handler code here
	int					iIndex;

	UpdateData(TRUE);

	iIndex = m_cResolutionList.GetCurSel();//�����б�ѡ����������ţ���0��ʼ
	
	if (iIndex == (m_cResolutionList.GetCount() - 1))//ѡ�е������һ������ʾ��ROI�Զ��ֱ���
	{
		//m_tRoiResolution.iIndex == 0XFF����ʾ���Զ���ֱ��� Switch to ROI resolution
		SDK_TRACE(CameraSetImageResolution(m_hCamera,&m_tRoiResolution),gLanguage?"�����Զ���ֱ���":"Set user defined resolution");
		GetDlgItem(IDC_BUTTON_ROI_RESET)->EnableWindow(TRUE);
	}
	else//ѡ�е���Ԥ��ķֱ���
	{
		//Switch to preset resolution
		
		SDK_TRACE(CameraSetImageResolution(m_hCamera,&m_sCameraInfo.pImageSizeDesc[iIndex]),gLanguage?"�л���ָ����Ԥ��ֱ���":"switch to preset resolution");
		GetDlgItem(IDC_BUTTON_ROI_RESET)->EnableWindow(FALSE);
	}

	UpdateExposure();//�ֱ��ʸı����Ҫ���¸����ع�ʱ�䡣ÿ���ֱ����£���Ӧ�����ع�ʱ���ǲ�ͬ�ġ�
}

void CAdvancedDlg::OnButtonSoftTrigOnce() 
{
	// TODO: Add your control notification handler code here

	//Send out a trigger signal in software trigger mode��
	//�����ǰ��������ģʽ����ú����᷵�ش���
	SDK_TRACE(CameraSoftTrigger(m_hCamera),gLanguage?"ִ��һ������":"Do software trigger once");
}

//�ݹ鷽ʽ����һ������·��
BOOL CAdvancedDlg::mkdirEx(const char* lpPath)
{
	
	CString pathname = lpPath;
	
	if(pathname.Right(1) != "\\")
		
		pathname += "\\" ;
	
	int end = pathname.ReverseFind('\\');
	
	int pt = pathname.Find('\\');
	
	if (pathname[pt-1] == ':')
		
		pt = pathname.Find('\\', pt+1);
	
	CString path;
	
	while(pt != -1 && pt<=end)
		
	{
		
		path = pathname.Left(pt+1);
		
		if(!PathIsDirectory(path))
		{
			if(!CreateDirectory(path,0))
			{
				return FALSE;
			}
		}
		
		pt = pathname.Find('\\', pt+1);
		
	}
	
	return TRUE;
}


BOOL CAdvancedDlg::CheckPath()
{

	UpdateData(TRUE);
	
	if (!PathIsDirectory(m_sSnapFilePath))   
	{   
		if (AfxMessageBox("The path is not exist,do you want to create it?", MB_YESNO)   ==   IDYES)   
		{   
			if   (!mkdirEx(m_sSnapFilePath) )   
			{   
                MessageBox("Create path failed!");
				return FALSE;   
            }   
		}   
	}  
	
	return TRUE;
}

void CAdvancedDlg::OnButtonSnapshot() 
{
	// TODO: Add your control notification handler code here
	CString			strFileName;
	BYTE			byFileType;
	tSdkFrameHead	tFrameHead;
	BYTE			*pbyBuffer;
	CString			msg;

	if (!CheckPath())
	{
		return;
	}
	
	CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
    strFileName = m_sSnapFilePath + _T("\\Snapshot-") + strTime;
    
	//�ж�Ҫ�����ļ�������
	switch(m_iSnapfileTypeSel)
	{
	case 0:
		byFileType = FILE_JPG;
		break;
	case 1:
		byFileType = FILE_PNG;
		break;
	case 2:
        byFileType = FILE_BMP;
        break;
    case 3:
        byFileType = FILE_RAW;
        break;
	default:
		byFileType = FILE_BMP;
		break;
	}

	//CameraSnapToBufferץ��һ��ͼ�񱣴浽buffer��
	// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
    if(CameraSnapToBuffer(m_hCamera,&tFrameHead,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
	{
		msg.Format("Function:[CameraSnapToBuffer] SUCCESS!");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Get a frame in snapshot mode");
		m_DlgLog.AppendLog(msg);
		m_DlgLog.AppendLog(" ");

		if (byFileType == FILE_BMP || 
			byFileType == FILE_JPG ||
			byFileType == FILE_PNG)//����ԭʼ���ݸ�ʽ���⣬��Ҫ�ȵ���CameraImageProcessת����RGB��ʽ�ٱ���
		{
			//Process the image before save it
			SDK_TRACE(CameraImageProcess(m_hCamera, pbyBuffer,m_pbImgBuffer,&tFrameHead),gLanguage?"��ͼ����д���ת���Ȳ���!":"Process the raw data");

			if (!m_bNotOverlayOnSnap)
			{
				//Superimpose the cross line��AE or WB window if they are set visible	
				SDK_TRACE(CameraImageOverlay(m_hCamera, m_pbImgBuffer,&tFrameHead),gLanguage?"����ʮ���ߡ��Զ��ع�Ͱ�ƽ�ⴰ��":"Overlay the crossline and AE WB window");
			}
		
			//Save the captured image into file
			SDK_TRACE(CameraSaveImage(m_hCamera, strFileName.GetBuffer(1), 
				m_pbImgBuffer, &tFrameHead, byFileType, 100),gLanguage?"��ͼƬ���浽�ļ���":"Save image to the file");
			
		}
		else
		{
			//Save the captured image into file
			SDK_TRACE(CAMERA_STATUS_SUCCESS != CameraSaveImage(m_hCamera, strFileName.GetBuffer(1), 
				pbyBuffer, &tFrameHead, byFileType, 100),gLanguage?"��ͼƬ���浽�ļ���":"Save image to the file");
			
		}
		
		//Rlease the buffer get from CameraSnapToBuffer
		SDK_TRACE(CameraReleaseImageBuffer(m_hCamera,pbyBuffer),gLanguage?"�ͷ���CameraSnapToBuffer��õĻ�����":"Release the buffer alloced by ameraSnapToBuffer");

		return;
	}

	
	return ;

}

void CAdvancedDlg::OnButtonAeWinSet() 
{
	// TODO: Add your control notification handler code here
	int iWidth,iHeight,iHoff,iVoff;
	CString msg;

	//����ʹ����SDK�ڲ��ĺ���CameraCustomizeReferWin�����ӻ��Ļ�ȡҪ�趨�Ĵ��ڷ�Χ��
	//��Ҳ����ֱ�ӵ���CameraSetAeWindow�����á����ڵ��趨��Χ���ܳ�����ǰ�ֱ��ʡ�
	//CameraSetAeWindow(m_hCamera,0,0,0,0)���ʾ����Ϊ��ǰ�ֱ��ʾ���1/2��С���ڣ��ֱ����л�����Ȼ��Ч

	if (CameraCustomizeReferWin(m_hCamera,REF_WIN_AUTO_EXPOSURE,m_hWnd,&iHoff,&iVoff,&iWidth,&iHeight) == CAMERA_STATUS_SUCCESS)
	{
		msg.Format("Function:[CameraCustomizeReferWin] SUCCESS!");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Get the reference window in a visualization way");
		m_DlgLog.AppendLog(msg);
		//Set the auto exposure reference window
		SDK_TRACE(CameraSetAeWindow(m_hCamera,iHoff,iVoff,iWidth,iHeight),gLanguage?"�����Զ��ع�ο�����":"Set AE reference window");
	}

}

void CAdvancedDlg::OnBtnWbOnce() 
{
	// TODO: Add your control notification handler code here
	CameraSetOnceWB(m_hCamera);
	Sleep(500);
	UpdateControls();
}

void CAdvancedDlg::OnBtnWbWinSet() 
{
	// TODO: Add your control notification handler code here
	int iWidth,iHeight,iHoff,iVoff;
	CString msg;
	
	//����ʹ����SDK�ڲ��ĺ���CameraCustomizeReferWin�����ӻ��Ļ�ȡҪ�趨�Ĵ��ڷ�Χ��
	//��Ҳ����ֱ�ӵ���CameraSetWbWindow�����á����ڵ��趨��Χ���ܳ�����ǰ�ֱ��ʡ�
	//CameraSetWbWindow(m_hCamera,0,0,0,0)���ʾ����Ϊ��ǰ�ֱ���ͬ���Ĵ�С���ڣ��ֱ����л�����Ȼ��Ч

	if (CameraCustomizeReferWin(m_hCamera,REF_WIN_WHITE_BALANCE,m_hWnd,&iHoff,&iVoff,&iWidth,&iHeight) == CAMERA_STATUS_SUCCESS)
	{
		msg.Format("Function:[CameraCustomizeReferWin] SUCCESS!");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Get the reference window in a visualization way");
		m_DlgLog.AppendLog(msg);
		//Set the white balance reference window
		SDK_TRACE(CameraSetWbWindow(m_hCamera,iHoff,iVoff,iWidth,iHeight),gLanguage?"���ð�ƽ��ο�����":"Set WB reference window");
	}
}

void CAdvancedDlg::OnButtonCustomResSnapshot() 
{

	CString				msg;

	CameraGetResolutionForSnap(m_hCamera,&m_tRoiResolutionSnapshot);
	

	if (CameraCustomizeResolution(m_hCamera,&m_tRoiResolutionSnapshot) == CAMERA_STATUS_SUCCESS)
	{
		msg.Format("Function:[CameraCustomizeResolution] SUCCESS!");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Get a ROI resolution for snapshot in a visualization way");
		m_DlgLog.AppendLog(msg);

		//Set a new resolution for snapshot mode
		SDK_TRACE(CameraSetResolutionForSnap(m_hCamera,&m_tRoiResolutionSnapshot),gLanguage?"����ץ��ģʽ�µķֱ���":"Set the resolution for snapshot")
	}
}

void CAdvancedDlg::OnButtonBrowseSnapshot() 
{
	// TODO: Add your control notification handler code here
	char   acDesetDir[MAX_PATH];  
	BROWSEINFO   bi;  
	ITEMIDLIST   *pidl;  
	
	bi.hwndOwner		=   this->m_hWnd;  
	bi.pidlRoot			=   NULL;  
	bi.pszDisplayName   =   acDesetDir;  
	bi.lpszTitle		=   _T("Select a folder to save image");  
	bi.ulFlags			=   BIF_RETURNONLYFSDIRS;  
	bi.lpfn				=   NULL;  
	bi.lParam			=   0;  
	bi.iImage			=   0;  
	
	pidl				=   SHBrowseForFolder(&bi);  
	
	if(pidl == NULL)  
		return;  
	
	if(!SHGetPathFromIDList(pidl, (acDesetDir)))  
	{
		return;
	}
	m_sSnapFilePath = acDesetDir;
	UpdateData(FALSE);
}

void CAdvancedDlg::OnRadioTriggerMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the selectted trigger mode
	SDK_TRACE(CameraSetTriggerMode(m_hCamera, m_iTriggerModeSel),gLanguage?"����Ϊָ���Ĵ���ģʽ":"Switch to the selected trigger mode");
	GetDlgItem(IDC_BUTTON_SOFT_TRIG_ONCE)->EnableWindow(m_iTriggerModeSel == SOFT_TRIGGER);
	GetDlgItem(IDC_EDT_TRIGGER_COUNT)->EnableWindow(m_iTriggerModeSel != CONTINUATION);
	GetDlgItem(IDC_EDT_TRIGGER_DELAY)->EnableWindow(m_iTriggerModeSel >= EXTERNAL_TRIGGER);
}

void CAdvancedDlg::OnChangeEdtTriggerCount() 
{
	UpdateData(TRUE);
	//Set the count of frame in one trigger
	SDK_TRACE(CameraSetTriggerCount(m_hCamera, m_iTriggerCount),"����һ�δ�����֡��");
}

void CAdvancedDlg::OnChangeEdtTriggerDelay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the delay time after the trigger in hardware mode
	SDK_TRACE(CameraSetTriggerDelayTime(m_hCamera, m_iHardwareTriggerDelayUs),gLanguage?"����Ӳ�����ź��յ������ʱִ��ʱ��":"Set trigger delay time");
}

void CAdvancedDlg::OnRadioExposureMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the state of exposure mode
	SDK_TRACE(CameraSetAeState(m_hCamera, m_iExposureModeSel == 0?TRUE:FALSE),"�����ع�ģʽ");
	UpdateControls();
}


void CAdvancedDlg::OnChkAntiFlick() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the state of anti-flick
	SDK_TRACE(CameraSetAntiFlick(m_hCamera, m_bAntiFlick),gLanguage?"���ÿ�Ƶ��ʹ��״̬":"Set the state of anti-flick");
}


void CAdvancedDlg::OnAntiFilckhz() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SDK_TRACE(CameraSetLightFrequency(m_hCamera, m_iAntiflickFreqSel),gLanguage?"ѡ��Ƶ����Ƶ��":"select the anti-flick frequence");
}

void CAdvancedDlg::OnCheckDisplayAeWin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the visible state of AE window
	SDK_TRACE(CameraSetAeWinVisible(m_hCamera, m_bDisplayAeWindow),gLanguage?"�����Զ��ع�ο����ڵ���ʾ״̬":"Set the visible state of AE window");
}

void CAdvancedDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl		*pSlider;
	UINT			uSliderID;
	int				iPos;
	CString			msg;
    double			fTemp;
	pSlider = (CSliderCtrl *)pScrollBar;
	
	uSliderID = pSlider->GetDlgCtrlID();
	
    iPos = pSlider->GetPos();
	
	switch (uSliderID)
	{
		  case IDC_SLD_EDGE_ENHANCE://�����񻯵���
			  m_cSldSharppen.SetPos(iPos);	
			  //Set sharpness level
			  SDK_TRACE(CameraSetSharpness(m_hCamera,m_cSldSharppen.GetPos()),gLanguage?"�����񻯵ȼ�":"Set the value of sharpness");
			  break;
			  
		  case IDC_SLD_RGAIN://��ɫ�����������
			  m_cSldRedGain.SetPos(iPos);	
			  //Set digital gain
			  SDK_TRACE(CameraSetGain(m_hCamera,m_cSldRedGain.GetPos(),m_cSldGreenGain.GetPos(),m_cSldBlueGain.GetPos())
				  ,gLanguage?"������������":"Set the digital gain for red color");
			  msg.Format("X%4.2f",iPos/100.f);
			  GetDlgItem(IDC_STC_RTEXT)->SetWindowText(msg);
			  break;
			  
		  case IDC_SLD_GGAIN://��ɫ�����������
			  m_cSldGreenGain.SetPos(iPos);	
			  //Set digital gain
			  SDK_TRACE(CameraSetGain(m_hCamera,m_cSldRedGain.GetPos(),m_cSldGreenGain.GetPos(),m_cSldBlueGain.GetPos())
										,gLanguage?"������������":"Set the digital gain for green color");
			  msg.Format("X%4.2f",iPos/100.f);
			  GetDlgItem(IDC_STC_GTEXT)->SetWindowText(msg);
			  break;
			  
		  case IDC_SLD_BGAIN://��ɫ�����������
			  m_cSldBlueGain.SetPos(iPos);	
			  //Set digital gain
			  SDK_TRACE(CameraSetGain(m_hCamera,m_cSldRedGain.GetPos(),m_cSldGreenGain.GetPos(),m_cSldBlueGain.GetPos())
										,gLanguage?"������������":"Set the digital gain for blue color");
			  msg.Format("X%4.2f",iPos/100.f);
			  GetDlgItem(IDC_STC_BTEXT)->SetWindowText(msg);
			  break;
			  
		  case IDC_SLD_SATURATION://���Ͷȵ���
			  m_cSldSaturation.SetPos(iPos);
			  //Set saturation
			  SDK_TRACE(CameraSetSaturation(m_hCamera,iPos),gLanguage?"���ñ��Ͷ�":"Set the value of saturation");
			  msg.Format("%d",iPos);	
			  GetDlgItem(IDC_STC_SATURATIONTEXT)->SetWindowText(msg);
			  break;	  

		  case IDC_SLD_ANALOG_GAIN://����ģ��������� ,��Ϊģ������Ĳ���ֵ�ڲ�ͬ�׶��ǲ�һ���ģ�
								   //��ˣ�CameraSetAnalogGain�����ֵ����߲�ͬ�׶���������������Ҫ
								   //�ٴε���CameraGetAnalogGain���ʵ�ʵ��趨ֵ
			  //Set analog gain
			  SDK_TRACE(CameraSetAnalogGain(m_hCamera,iPos),gLanguage?"����ģ������":"Set the analog gain");

			  //Get analog gain,the set value may be correctted ,so we Get the gain after CameraSetAnalogGain
			  SDK_TRACE(CameraGetAnalogGain(m_hCamera,&iPos),gLanguage?"���ģ�������趨ֵ":"Get the analog gain");
			  m_cSldAnalogGain.SetPos(iPos);
			  msg.Format("%.4f",iPos*m_fAnalogGainStep);
			  MySetDlgItemText(IDC_EDIT_GAINTEXT, msg);
			  break;
			  
		  case IDC_SLD_EXPTIME://�ع�ʱ����� 
			  SDK_TRACE(CameraSetExposureTime(m_hCamera,iPos*m_fExpLineTime),gLanguage?"�����ع�ʱ��":"Set exposure time");
			  SDK_TRACE(CameraGetExposureTime(m_hCamera,&fTemp),gLanguage?"����ع�ʱ��":"Get expsoure time");
			  iPos = (int)(fTemp/m_fExpLineTime);
			  m_cSldExposureLines.SetPos(iPos);
			  MySetDlgItemText(IDC_EDIT_EXPTEXT, "%.4f",(iPos*m_fExpLineTime)/1000.f);
			  break;
			  
		  case IDC_SLD_AE_TARGET://�Զ��ع�Ŀ������ֵ���� 
			  //Set auto exposure brightness
			  SDK_TRACE(CameraSetAeTarget(m_hCamera,iPos),gLanguage?"�����Զ��ع�ģʽ�µ�����Ŀ��ֵ":"Set the target of lightness on AE mode");
			  msg.Format("%d",pSlider->GetPos());
			  GetDlgItem(IDC_STC_AETARGETTEXT)->SetWindowText(msg);
			  break;
		  
		  case IDC_SLD_GAMMA://٤����� 
			  msg.Format(_T("%4.2f"),iPos/100.0f);
			  GetDlgItem(IDC_STC_GAMMATEXT)->SetWindowText(msg);
			  //Set gamma
			  SDK_TRACE(CameraSetGamma(m_hCamera,iPos),gLanguage?"�趨٤��ֵ":"Set the value of gamma");
			  break;
			  
		  case IDC_SLD_CONTRAST://�Աȶȵ���
			  msg.Format(_T("%4.2f"),iPos/100.0f);
			  GetDlgItem(IDC_STC_CONTRASTTEXT)->SetWindowText(msg);
			  //Set contrast	
			  SDK_TRACE(CameraSetContrast(m_hCamera,iPos),gLanguage?"�趨�Աȶ�ֵ":"Set the value of contrast");
			  break;
	}	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAdvancedDlg::OnCheckNoiseReduce() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the state of niose reduce
    SDK_TRACE(CameraSetNoiseFilter(m_hCamera,m_bNoiseReduce),gLanguage?"���ý���ʹ��״̬":"Set the state of 2d-noise filter");
}

void CAdvancedDlg::OnButtonLutLoadCustom() 
{
	// TODO: Add your control notification handler code here
	FILE *pFp;
	CString filePath;
	CString msg;
	USHORT* pLUT = new USHORT[4096];
	CFileDialog  dlg(true, NULL,  NULL,  /*OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT*/0, 
		_T("Camera LUT File(*.MvLut)|*.MvLut||")); 

	do
	{
	    if(IDOK == dlg.DoModal()) 
    	{
    		filePath = dlg.GetPathName();
    		//��ȡ�ļ�����
    		
    		pFp = fopen(filePath,"rb");

    		if (!pFp)
    		{
    			MessageBox(msg,"File open error!");
    			break;
    		}
    		
    		//�ļ���С������256�ֽڣ���Ӧ0��255����ɫӳ��ֵ
    		if (fread(pLUT,1,4096,pFp) != 4096)
    		{
    			MessageBox("File format error!");
    			fclose(pFp);
    			break;
    		}
    		//Set the user loaded LUT table
			SDK_TRACE(CameraSetCustomLut(m_hCamera,LUT_CHANNEL_ALL,pLUT),gLanguage?"�����Զ����LUT��":"Set user defined LUT table");
    		fclose(pFp);
    	}
	}while(0);

	delete pLUT;
}

void CAdvancedDlg::OnButtonEditLut() 
{
	// Not surpport yet!
}

void CAdvancedDlg::OnSelchangeComboPresetLut() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the select int preset lut mode
	SDK_TRACE(CameraSelectLutPreset(m_hCamera,m_cPresetLutList.GetCurSel()),gLanguage?"����ѡ����Ԥ��LUT��":"Set the select LUT index on preset table");
}

void CAdvancedDlg::OnRadioLutMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the LUT mode
	SDK_TRACE(CameraSetLutMode(m_hCamera,m_iLutModeSel),gLanguage?"���������LUTģʽ":"Set the LUT mode");
	GetDlgItem(IDC_COMBO_PRESET_LUT)->EnableWindow(m_iLutModeSel == LUTMODE_PRESET);//ֻ���ڶ�Ӧģʽ�£������б���ܼ���
}


void CAdvancedDlg::OnRadioWbMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the white balance mode
	SDK_TRACE(CameraSetWbMode(m_hCamera,m_iWbModeSel),gLanguage?"���ð�ƽ��ģʽ":"Set WB mode");//�������ʧ�ܣ�һ������Ϊ�������֧���Զ���ƽ��ģʽ��
																	   //CameraSetWbMode�᷵��CAMERA_STATUS_NOT_SUPPORTED
	UpdateControls();//���½���
}
#ifdef _WIN64
void CAdvancedDlg::OnTimer(UINT_PTR nIDEvent) 
#else
void CAdvancedDlg::OnTimer(UINT nIDEvent) 
#endif
{
	// TODO: Add your message handler code here and/or call default
	CString strStatusText;
    int iTimeCurrnet = 0;
	static int iDispNum = 0;
	
	if (nIDEvent == 0)
	{
		//��SDK�ڲ��������Ĳɼ���֡������֡���ȵȡ�
		//Get frame statistic information
		CameraGetFrameStatistic(m_hCamera, &m_sFrameCount);
		iTimeCurrnet = GetTickCount();

		//����ʱ������֡��
		if (0 != iTimeCurrnet-m_iTimeLast)
		{
			m_fCapFps = (float)((m_sFrameCount.iCapture - m_sFrameLast.iCapture)*1000.0)/(float)(iTimeCurrnet-m_iTimeLast);
			m_fDispFps = (float)((m_iDispFrameNum - iDispNum)*1000.0)/(float)(iTimeCurrnet-m_iTimeLast);
		}
		else
		{
			return;
		}        

		m_iTimeLast = iTimeCurrnet;

		//Update frame information
		strStatusText.Format("Demo [Advanced]| Image size:%d*%d | Dsiplay FPS:%4.2f FPS | Capture FPS:%4.2f FPS |",
			m_sFrInfo.iWidth, m_sFrInfo.iHeight,
			m_fDispFps, m_fCapFps);

		SetWindowText(strStatusText);

		m_sFrameLast.iCapture = m_sFrameCount.iCapture;
		m_sFrameLast.iLost = m_sFrameCount.iLost;
		m_sFrameLast.iTotal = m_sFrameCount.iTotal;
		iDispNum = m_iDispFrameNum;

		//��������Զ��ع⣬����µ�ǰ���ع�ʱ���ģ������
		if (m_iExposureModeSel == 0)
		{
			UpdateExposure();
		}
	}
	else// ID == 1,�Զ�ץ�Ķ�ʱ����
	{
		BYTE			*pbyBuffer;
		tSdkFrameHead	sFrameInfo;
		BYTE			byFileType;
		CString			filename;

		// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
		if(CameraSnapToBuffer(m_hCamera,&sFrameInfo,&pbyBuffer,10000) == CAMERA_STATUS_SUCCESS)
		{
			CameraImageProcess(m_hCamera, pbyBuffer,m_pbImgBuffer,&sFrameInfo);
			CameraReleaseImageBuffer(m_hCamera,pbyBuffer);
			
			//�ж�Ҫ�����ļ�������
			switch(m_iSnapfileTypeSel)
			{
			case 0:
				byFileType = FILE_JPG;
				break;
			case 1:
				byFileType = FILE_PNG;
				break;
			case 2:
				byFileType = FILE_BMP;
				break;
			case 3:
				byFileType = FILE_RAW;
				break;
			default:
				byFileType = FILE_BMP;
				break;
			}

			filename.Format("%s\\AutoSnap%u",m_sSnapFilePath,GetTickCount());
			CameraSaveImage(m_hCamera,filename.GetBuffer(1),m_pbImgBuffer,&sFrameInfo,byFileType,100);

			return ; 
		}

	}
   

	CDialog::OnTimer(nIDEvent);
}

void CAdvancedDlg::OnClose() 
{
	//����ʼ�����
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//�ȴ��ɼ��߳̽���
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}
		
		if (m_bSaveParamOnExit)
		{
			//Save parameter before exit
			SDK_TRACE(CameraSaveParameter(m_hCamera,m_iParamGroupSel),gLanguage?"���������ָ���Ĳ�������":"Save camera current settings to the file");
		}

		//Uninit the camera
		SDK_TRACE(CameraUnInit(m_hCamera),gLanguage?"����ʼ�������":"Uninit the camera");
		m_hCamera = 0;
	}
	
	if (m_pbImgBuffer)
	{
		CameraAlignFree(m_pbImgBuffer);
		m_pbImgBuffer = NULL;
	}

	if (m_pFrameBuffer)
	{
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}


	CDialog::OnClose();
}

void CAdvancedDlg::OnButtonSaveParam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Save parameter to current group
	SDK_TRACE(CameraSaveParameter(m_hCamera,m_iParamGroupSel),gLanguage?"�����������ǰ�Ĳ�������":"Save the settings to the current parameter group");
}

void CAdvancedDlg::OnButtonLoadDefault() 
{
	// TODO: Add your control notification handler code here
	// Load default parameter
	SDK_TRACE(CameraLoadParameter(m_hCamera,PARAMETER_TEAM_DEFAULT),gLanguage?"�ָ����Ĭ�ϲ���":"Load default parameter for the camera");

	UpdateControls();//���ز�������½�����ʾ
}

void CAdvancedDlg::OnButtonLoadParamFromFile() 
{
	// TODO: Add your control notification handler code here
	CString filePath;
	CFileDialog  dlg(true, NULL,  NULL,  /*OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT*/0, 
		_T("Camera Config File(*.Config)|*.Config||")); 
	
	if(IDOK == dlg.DoModal()) 
	{
		filePath = dlg.GetPathName();
		//Load parameter from a file,usually in path .//camera//Configs
		SDK_TRACE(CameraReadParameterFromFile(m_hCamera,filePath.GetBuffer(1)),gLanguage?"���ļ��м������������ͨ������£�����Ĳ���������$(SETUP)//camera//Configs��":"Load camera settings from the file");
		UpdateControls();//���ز�������½�����ʾ
	}
}

void CAdvancedDlg::OnSetCrosshair() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set position ��color and visible state for corsshair
	SDK_TRACE(CameraSetCrossLine(m_hCamera,0,m_iCrossPositionX,m_iCrossPositionY,m_uCrosshairColor,m_bDisplayCrosshair)
								,gLanguage?"����ʮ����":"Set the crossline");
}

void CAdvancedDlg::OnRadioChangeParamGroup() 
{
	// TODO: Add your control notification handler code here
	
	if (m_bSaveParamOnSwitch)
	{
		//Save parameter before switch to anther group
		SDK_TRACE(CameraSaveParameter(m_hCamera,m_iParamGroupSel),gLanguage?"�����������ǰ��������":"Save the settings to current parameter group");
	}

	UpdateData(TRUE);//m_iParamGroupSel will be update
	
	//Load parameter for the selected group
	SDK_TRACE(CameraLoadParameter(m_hCamera,m_iParamGroupSel),gLanguage?"����ָ���Ĳ�����":"Load the selected parameter group");

	UpdateControls();//������ʾ
}


void CAdvancedDlg::OnCheckDisWbWin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//Set the visible state of WB window
	SDK_TRACE(CameraSetWbWinVisible(m_hCamera, m_bDisplayWbWindow),gLanguage?"���ð�ƽ�ⴰ�ڵ���ʾ״̬":"Set the visible state of WB window");
}


void CAdvancedDlg::OnButtonCustomCt() 
{
	// TODO: Add your control notification handler code here

	int iRgain,iGgain,iBgain,iMax;
    UINT uMin;
    float fMatrix[3][3];
    float fRgain,fGgain,fBgain;
    CUdefClrTmp dlg;
    
    CameraGetUserClrTempGain(m_hCamera,&iRgain,&iGgain,&iBgain);
    
    CameraGetUserClrTempMatrix(m_hCamera,(float*)fMatrix);

    iMax = max(iGgain,iRgain);
    iMax = max(iMax,iBgain);
    
    dlg.m_uRed  = (UINT)(((float)iRgain/(float)iMax)*255.0);
    dlg.m_uGreen = (UINT)(((float)iGgain/(float)iMax)*255.0);
    dlg.m_uBlue  = (UINT)(((float)iBgain/(float)iMax)*255.0);

    dlg.m_fClrMatrix1 = fMatrix[0][0];
    dlg.m_fClrMatrix2 = fMatrix[0][1];
    dlg.m_fClrMatrix3 = fMatrix[0][2];
    dlg.m_fClrMatrix4 = fMatrix[1][0];
    dlg.m_fClrMatrix5 = fMatrix[1][1];
    dlg.m_fClrMatrix6 = fMatrix[1][2];
    dlg.m_fClrMatrix7 = fMatrix[2][0];
    dlg.m_fClrMatrix8 = fMatrix[2][1];
    dlg.m_fClrMatrix9 = fMatrix[2][2];
    
	if (dlg.DoModal() == IDOK)
	{
	    //ͨ����ɫ���õ�����
		uMin = min(dlg.m_uRed,dlg.m_uGreen);
        uMin = min(uMin,dlg.m_uBlue);
        fRgain = ((float)dlg.m_uRed) / (float)uMin; 
        fGgain = ((float)dlg.m_uGreen) / (float)uMin; 
        fBgain = ((float)dlg.m_uBlue) / (float)uMin; 
        iRgain = (int)(fRgain*100.0f);
        iGgain = (int)(fGgain*100.0f);
        iBgain = (int)(fBgain*100.0f);
        
		if (iGgain == 0 || iGgain == 0 || iBgain == 0)
		{
			MessageBox("��Ч��ɫ�£���ɫ��������Ϊ0");
			return;
		}

        CameraSetUserClrTempGain(m_hCamera,iRgain,iGgain,iBgain);

        fMatrix[0][0] = dlg.m_fClrMatrix1;
        fMatrix[0][1] = dlg.m_fClrMatrix2;
        fMatrix[0][2] = dlg.m_fClrMatrix3;
        fMatrix[1][0] = dlg.m_fClrMatrix4;
        fMatrix[1][1] = dlg.m_fClrMatrix5;
        fMatrix[1][2] = dlg.m_fClrMatrix6;
        fMatrix[2][0] = dlg.m_fClrMatrix7;
        fMatrix[2][1] = dlg.m_fClrMatrix8;
        fMatrix[2][2] = dlg.m_fClrMatrix9;
        
        CameraSetUserClrTempMatrix(m_hCamera,(float*)fMatrix);
        
        CameraSetOnceWB(m_hCamera);//���Ҫ�þ����������Ч,��һ�ΰ�ƽ��

        Sleep(500);
        UpdateControls();
	}
}

void CAdvancedDlg::OnSelchangeComboCt() 
{
	// TODO: Add your control notification handler code here
	m_iClrTempSel = m_cClrTmpList.GetCurSel();

	if (m_iClrTempSel == 0)
	{
		SDK_TRACE(CameraSetClrTempMode(m_hCamera,(int)CT_MODE_AUTO),gLanguage?"�л����Զ�ɫ��ģʽ":"Switch to auto colo temprature mode");
	} 
	else if (m_iClrTempSel == (m_cClrTmpList.GetCount() - 1))
	{
		SDK_TRACE(CameraSetClrTempMode(m_hCamera,(int)CT_MODE_USER_DEF),gLanguage?"�л����Զ���ɫ��ģʽ":"Swicth to user defined colo temprature mode");
	}
	else
	{
		SDK_TRACE(CameraSetClrTempMode(m_hCamera,(int)CT_MODE_PRESET),gLanguage?"�л�ɫ��ģʽΪԤ��ģʽ":"Switch to preset colo temprature mode");
		SDK_TRACE(CameraSetPresetClrTemp(m_hCamera,m_iClrTempSel - 1),gLanguage?"�л���ָ����Ԥ��ɫ��":"Set the selected index of preset colo temprature");
	}

	//�л�ɫ�º���һ�ΰ�ƽ��
	OnBtnWbOnce();
	
	::ShowWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CT)->m_hWnd, (m_cClrTmpList.GetCurSel() == (m_cClrTmpList.GetCount() - 1)));
	::EnableWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CT)->m_hWnd, (m_cClrTmpList.GetCurSel() == (m_cClrTmpList.GetCount() - 1)));

    Sleep(500);
    UpdateControls();
        
	
}

void CAdvancedDlg::OnSelchangeComboFrameSpeed() 
{
	// TODO: Add your control notification handler code here
	int i;
	i = m_FrameSpeedList.GetCurSel();
	SDK_TRACE(CameraSetFrameSpeed(m_hCamera,i),gLanguage?"����֡��":"Set the frame speed");
}


void CAdvancedDlg::OnBnClickedAutoSnap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bAutoSnapshot == TRUE)
	{
		SetTimer(1,m_iAutoSnapCycleMs,NULL);
	}
	else
	{
		KillTimer(1);
	}
	
}

void CAdvancedDlg::OnEnChangeEditExptext()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (GetWindowLong(GetDlgItem(IDC_EDIT_EXPTEXT)->m_hWnd, GWLP_USERDATA) != 0)
		return;

	CString str;
	GetDlgItemText(IDC_EDIT_EXPTEXT, str);

	float exp_time = 0;
	sscanf(str, "%f", &exp_time);
	exp_time *= 1000.0f;

	double line_time;
	CameraGetExposureLineTime(m_hCamera, &line_time);

	BOOL bModify = FALSE;
	int num_line = exp_time / line_time;
	if (num_line < m_sCameraInfo.sExposeDesc.uiExposeTimeMin)
		num_line = m_sCameraInfo.sExposeDesc.uiExposeTimeMin;
	if (num_line > m_sCameraInfo.sExposeDesc.uiExposeTimeMax)
	{
		bModify = TRUE;
		num_line = m_sCameraInfo.sExposeDesc.uiExposeTimeMax;
	}

	SDK_TRACE(CameraSetExposureTime(m_hCamera, num_line * line_time),gLanguage?"�����ع�ʱ��":"Set exposure time");
	m_cSldExposureLines.SetPos(num_line);

	if (bModify)
		MySetDlgItemText(IDC_EDIT_EXPTEXT, "%.4f", num_line * line_time / 1000.0f);
}

void CAdvancedDlg::OnEnChangeEditGaintext()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (GetWindowLong(GetDlgItem(IDC_EDIT_GAINTEXT)->m_hWnd, GWLP_USERDATA) != 0)
		return;

	CString str;
	GetDlgItemText(IDC_EDIT_GAINTEXT, str);

	float gain = 0;
	sscanf(str, "%f", &gain);

	BOOL bModify = FALSE;
	m_iAnalogGain = gain / m_fAnalogGainStep;
	if (m_iAnalogGain < m_sCameraInfo.sExposeDesc.uiAnalogGainMin)
		m_iAnalogGain = m_sCameraInfo.sExposeDesc.uiAnalogGainMin;
	if (m_iAnalogGain > m_sCameraInfo.sExposeDesc.uiAnalogGainMax)
	{
		m_iAnalogGain = m_sCameraInfo.sExposeDesc.uiAnalogGainMax;
		bModify = TRUE;
	}

	SDK_TRACE(CameraSetAnalogGain(m_hCamera,m_iAnalogGain),gLanguage?"����ģ������":"Set the analog gain");
	m_cSldAnalogGain.SetPos(m_iAnalogGain);

	if (bModify)
		MySetDlgItemText(IDC_EDIT_GAINTEXT, "%.4f", m_iAnalogGain * m_fAnalogGainStep);
}

void CAdvancedDlg::MySetDlgItemText(UINT uItemID, LPCSTR pFmt, ...)
{
	va_list args;
	va_start(args, pFmt);
	
	CString str;
	str.FormatV(pFmt, args);

	CWnd* pWnd = GetDlgItem(uItemID);
	if (pWnd != NULL)
	{
		SetWindowLong(pWnd->m_hWnd, GWLP_USERDATA, 1);
		pWnd->SetWindowText(str);
		SetWindowLong(pWnd->m_hWnd, GWLP_USERDATA, 0);
	}
}

void CAdvancedDlg::OnBnClickedChkHfip()
{
	UpdateData(TRUE);
	CameraSetMirror(m_hCamera, 0, m_bHflip);
}

void CAdvancedDlg::OnBnClickedChkVflip()
{
	UpdateData(TRUE);
	CameraSetMirror(m_hCamera, 1, m_bVflip);
}
