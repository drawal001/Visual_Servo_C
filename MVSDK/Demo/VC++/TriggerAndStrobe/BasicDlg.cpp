// BasicDlg.cpp : implementation file
//
//BIG5 TRANS ALLOWED
#include "stdafx.h"
#include "Basic.h"

#include "BasicDlg.h"
#include "malloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gLanguage;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
	CBasicDlg *pThis = (CBasicDlg*)pContext;
	
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
		CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer,pFrameHead);
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
	CBasicDlg* 		pThis = (CBasicDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	

	while (!pThis->m_bExit)
    {   

		if(CameraGetImageBuffer(pThis->m_hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer,&sFrameInfo);//����ģʽ

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
	CString	m_sAboutInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sAboutInfo = _T("This example shows how to integrate the camera into your system in a very fast and easy way!");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT_ABOUT, m_sAboutInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicDlg dialog

CBasicDlg::CBasicDlg(CWnd* pParent /*=NULL*/)
: CDialog(gLanguage == 0?IDD_BASIC_DIALOG:IDD_BASIC_DIALOG_CN, pParent)
, m_StrobePolarity(0)
{
	//{{AFX_DATA_INIT(CBasicDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bPause = TRUE;
	m_hCamera = -1;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pFrameBuffer = NULL;
	m_JitterTime = 0;
}

void CBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicDlg)
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_cPreview);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO1, m_ComBoxTrigType);
	DDX_Control(pDX, IDC_COMBO2, m_ComBoxStrobeMode);
	DDX_Radio(pDX, IDC_RADIO1, m_StrobePolarity);
	DDX_Text(pDX, IDC_EDIT1, m_JitterTime);
	DDV_MinMaxUInt(pDX, m_JitterTime, 0, 500);
}

BEGIN_MESSAGE_MAP(CBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_SETTINGS, OnButtonCameraSettings)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadioStrobePolarity)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadioStrobePolarity)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicDlg message handlers

BOOL CBasicDlg::OnInitDialog()
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
		
// 		m_DlgLog.AppendLog("Basic Demo start");
// 		m_DlgLog.AppendLog("LoadSdkApi was called to load SDK api from MVCAMSDK.dll");
// 		m_DlgLog.AppendLog("LoadSdkApi is open source in CameraApiLoad.h ");
// 		m_DlgLog.AppendLog("It shows how to load the api from MVCAMSDK.dll,");
// 		m_DlgLog.AppendLog("you can also use your own way to load MVCAMSDK.dll");


		//Init the SDK��0:Ӣ�İ� 1:���İ� �������������������Ϣ��SDK���ɵ��豸���ý�����
		SDK_TRACE(CameraSdkInit(gLanguage),gLanguage?"��ʼ��SDK":"Init SDK");

		if (!InitCamera())
		{
			break;
		}
		
		SetTimer(0,1000,NULL);//ʹ��һ����ʱ��������֡��

		return TRUE;

	} while(0);
	
	//û���ҵ�������߳�ʼ��ʧ�ܣ��˳�����
	EndDialog(0);
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CBasicDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBasicDlg::OnPaint() 
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
HCURSOR CBasicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBasicDlg::OnButtonPreview() 
{
	
	m_bPause = !m_bPause;
	if (gLanguage == 0)
	{
		GetDlgItem(IDC_BUTTON_PREVIEW)->SetWindowText(m_bPause?"Play":"Pause");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PREVIEW)->SetWindowText(m_bPause?"����":"��ͣ");
	}
	
	if (m_bPause)
	{
		//Set the camera in pause mode
		SDK_TRACE(CameraPause(m_hCamera),gLanguage?"��ͣ�������":"Pause the camera");
	}
	else
	{
		//Set the camera in play mode
		SDK_TRACE(CameraPlay(m_hCamera),gLanguage?"��ʼԤ��":"start preview");
	}
}


void CBasicDlg::OnButtonCameraSettings() 
{
	
	//Show the settings window��
	SDK_TRACE(CameraShowSettingPage(m_hCamera,TRUE),gLanguage?"��ʾ������������ô��ڣ��ô�����SDK�ڲ�����":"show the camera config page");
}

BOOL CBasicDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;
	UINT uMask;

	

	memset(&m_sFrInfo,0,sizeof(tSdkFrameHead ));
	//ö���豸������豸�б�
	//Enumerate camera
	iCameraNums = 10;//����CameraEnumerateDeviceǰ��������iCameraNums = 10����ʾ���ֻ��ȡ10���豸�������Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ
	
	if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("No camera was found!");
		return FALSE;
	}

	//��ʾ���У�����ֻ����������һ���������ˣ�ֻ��ʼ����һ�������(-1,-1)��ʾ�����ϴ��˳�ǰ����Ĳ���������ǵ�һ��ʹ�ø�����������Ĭ�ϲ���.
	//In this demo ,we just init the first camera.
	if ((status = CameraInit(&sCameraList[0],-1,-1,&m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d",status);
		MessageBox(msg);
		return FALSE;
	}
	

	//Get properties description for this camera.
	SDK_TRACE(CameraGetCapability(m_hCamera,&sCameraInfo),gLanguage?"��ø��������������":"Get the capability of the camera");
	SDK_TRACE(CameraGetExtTrigCapability(m_hCamera,&uMask),gLanguage?"��ø�������ⴥ����������":"Get the capability of Trigger settings");



	if (sCameraInfo.iTriggerDesc > 2)
	{
		m_ComBoxTrigType.InsertString(-1,gLanguage?"�����ش���":"Leading edge");
		m_ComBoxTrigType.InsertString(-1,gLanguage?"�½��ش���":"Falling edge");
	}
	else
	{
		MessageBox("This camera do not support external trigger mode !Program will exit");
		return FALSE;
	}

	if ( (uMask&EXT_TRIG_MASK_LEVEL_MODE))
	{
		m_ComBoxTrigType.InsertString(-1,gLanguage?"�ߵ�ƽ���崥��":"High level pulse");
		m_ComBoxTrigType.InsertString(-1,gLanguage?"�͵�ƽ���崥��":"Low level pulse");
	}

	m_ComBoxStrobeMode.InsertString(-1,gLanguage?"�Զ�ģʽ":"Auto mode");
	m_ComBoxStrobeMode.InsertString(-1,gLanguage?"�ֶ�ģʽ":"Manual mode");
	m_ComBoxStrobeMode.InsertString(-1,gLanguage?"GPIO���ģʽ":"GPIO output mode");

	m_ComBoxTrigType.SetCurSel(0);
	m_ComBoxStrobeMode.SetCurSel(0);
	m_StrobePolarity = 0;
	m_JitterTime = 0;

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*3,16);	

	ASSERT(m_pFrameBuffer);
	
	//ʹ��SDK��װ�õ���ʾ�ӿ�
	//Use SDK to display camera images.
	SDK_TRACE(CameraDisplayInit(m_hCamera,m_cPreview.GetSafeHwnd()),gLanguage?"��ʼ����ʾ�ӿ�":"Init display module");
	m_cPreview.GetClientRect(&rect);
	//Set display window size
	SDK_TRACE(CameraSetDisplaySize(m_hCamera,rect.right - rect.left,rect.bottom - rect.top),gLanguage?"������ʾ���ڴ�С":"Set display size");
	
	//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
	//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
	//Create the settings window for the camera
	SDK_TRACE(CameraCreateSettingPage(m_hCamera,GetSafeHwnd(),
								sCameraList[0].acFriendlyName,NULL,(PVOID)this,-1 & ~(1 << PROP_SHEET_INDEX_TRIGGER_SET))
								,gLanguage?"֪ͨSDK�ڲ��������������ҳ��":"Create camera config page");
 
	#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
	//Set the callback for image capture
	SDK_TRACE(CameraSetCallbackFunction(m_hCamera,GrabImageCallback,(PVOID)this,NULL),gLanguage?"����ͼ��ץȡ�Ļص�����":"Set image grab call back function");
	#else
	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0,  &m_threadID);
	ASSERT (m_hDispThread); 
	SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
	#endif
	//Tell the camera begin to sendding image
	SDK_TRACE(CameraPlay(m_hCamera),gLanguage?"��ʼԤ��":"Start preview");
	m_bPause = FALSE;
	GetDlgItem(IDC_BUTTON_PREVIEW)->SetWindowText(gLanguage?"��ͣ":"Pause");

	CameraSetTriggerMode(m_hCamera,EXTERNAL_TRIGGER);//����Ϊ�ⴥ��ģʽ���ȴ������ź�

	return TRUE;

}


void CBasicDlg::OnClose() 
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

		//����ʼ�������
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

    if (m_pFrameBuffer)
    {
        CameraAlignFree(m_pFrameBuffer);
        m_pFrameBuffer = NULL;
    }
	CDialog::OnClose();
}

void CBasicDlg::OnTimer(UINT_PTR nIDEvent)//һ����ˢ��һ��ͼ����Ϣ:�ֱ��ʣ���ʾ֡�ʣ��ɼ�֡��
{
	CString strStatusText;
    int iTimeCurrnet = 0;
	static int iDispNum = 0;

   	//��SDK�ڲ��������Ĳɼ���֡������֡���ȵȡ�
    CameraGetFrameStatistic(m_hCamera, &m_sFrameCount);
    iTimeCurrnet = GetTickCount();

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
	if (gLanguage == 1)//chinese
	{
		strStatusText.Format("| ͼ��ֱ���:%d*%d | ��ʾ֡��:%4.2f FPS | ����֡��:%4.2f FPS |",
			m_sFrInfo.iWidth, m_sFrInfo.iHeight,
        m_fDispFps, m_fCapFps);
	}
	else//english
	{
		strStatusText.Format("| Resolution:%d*%d | Display rate:%4.2f FPS | Capture rate:%4.2f FPS |",
			m_sFrInfo.iWidth, m_sFrInfo.iHeight,
        m_fDispFps, m_fCapFps);
	}
	GetDlgItem(IDC_STATIC_INFORMATION)->SetWindowText(strStatusText);

    m_sFrameLast.iCapture = m_sFrameCount.iCapture;
    m_sFrameLast.iLost = m_sFrameCount.iLost;
    m_sFrameLast.iTotal = m_sFrameCount.iTotal;
    iDispNum = m_iDispFrameNum;
	
    CDialog::OnTimer(nIDEvent);
}


//�л����봥���ź�����
void CBasicDlg::OnCbnSelchangeCombo1()
{
	UpdateData(TRUE);
	CameraSetExtTrigSignalType(m_hCamera,m_ComBoxTrigType.GetCurSel());
	/*
	CameraSetExtTrigSignalType(m_hCamera,0); //����Ϊ�������źŴ�����
	CameraSetExtTrigSignalType(m_hCamera,1); //����Ϊ�½����źŴ�����
	CameraSetExtTrigSignalType(m_hCamera,2); //����Ϊ�ߵ�ƽ�źŴ��������һֱΪ�ߵ�ƽ�����һֱ�������൱������Ԥ����
	CameraSetExtTrigSignalType(m_hCamera,3); //����Ϊ�͵�ƽ�źŴ��������һֱΪ�͵�ƽ�����һֱ�������൱������Ԥ������
	*/
}

//�л�������ź����ģʽ
void CBasicDlg::OnCbnSelchangeCombo2()
{
	UpdateData(TRUE);
	if(m_ComBoxStrobeMode.GetCurSel() < 2)
	{
		CameraSetStrobeMode(m_hCamera,m_ComBoxStrobeMode.GetCurSel());
		CameraSetOutPutIOMode(m_hCamera,0,IOMODE_STROBE_OUTPUT );
	}
	else if(m_ComBoxStrobeMode.GetCurSel() == 2)
	{
		CameraSetOutPutIOMode(m_hCamera,0,IOMODE_GP_OUTPUT );
	}
	
	/*
	CameraSetStrobeMode(m_hCamera,0) ����������ź�Ϊ�Զ�ģʽ������ģʽ�£����ֻ�����ع��ʱ����ϣ������Ч������źţ��ع���ɺ󣬻ص���Ч��ƽ״̬���źŲ�����������ʽ�ġ�
	CameraSetStrobeMode(m_hCamera,1) ����������ź�Ϊ���Զ�ģʽ������ģʽ�£�������źŶ��ӿ�����㶨�ĸߡ��͵�ƽ��
	*/
}

//�л�������źż���
void CBasicDlg::OnBnClickedRadioStrobePolarity()
{
	UpdateData(TRUE);
	if(m_ComBoxStrobeMode.GetCurSel() < 2)
	{
		CameraSetStrobePolarity(m_hCamera,m_StrobePolarity);
	}
	else if(m_ComBoxStrobeMode.GetCurSel() == 2)
	{
		CameraSetIOState(m_hCamera,0,m_StrobePolarity);
	}

	/*
	CameraSetStrobePolarity(m_hCamera,0) ������Ч����Ϊ�͵�ƽ
	CameraSetStrobePolarity(m_hCamera,1) ������Ч����Ϊ�ߵ�ƽ
	*/
}

/*���õ�ƽ������ʽ�µ�ȥ��ʱ��
ֻ���ڵ�ƽ����ģʽ��,ȥ��ʱ�����Ч,�ڱ��ش�����,����ȥ��ʱ�䡣
��˱��ش�����ʽ����һ��Ҫʹ�õ��ӿ��ء���ƽ������ʽ����ȥ��ʱ�䣬����ȥ��
��е���صĶ�����
*/
void CBasicDlg::OnEnChangeEdit1()
{
	CameraSetExtTrigJitterTime(m_hCamera,m_JitterTime);
}
