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

/*ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CBasicDlg* 		pThis = (CBasicDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	int				iCaptureCounts = 0;
	int				i;


	CameraSetTriggerMode(pThis->m_hCamera,1);//�������Ϊ����ģʽ
	
	CameraSetExposureTime(pThis->m_hCamera,pThis->m_dExp2);
		
	//������һ֡���ý���������CameraGetImageBuffer���ᳬʱ��
	CameraSoftTrigger(pThis->m_hCamera);


	while (!pThis->m_bExit)
    {   
		

		if( CameraGetImageBuffer(pThis->m_hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			iCaptureCounts++;
			//ƹ�Ҳ�����ÿ��һ֡���л��ع�ʱ�������
			if (iCaptureCounts % 2)
			{
				CameraSetExposureTime(pThis->m_hCamera,pThis->m_dExp1);
				CameraSetAnalogGain(pThis->m_hCamera,pThis->m_uGain1);
			}
			else
			{
				CameraSetExposureTime(pThis->m_hCamera,pThis->m_dExp2);
				CameraSetAnalogGain(pThis->m_hCamera,pThis->m_uGain2);
			}
			
			//���ú��ع�ʱ�������󣬾Ϳ��Կ�ʼ�����ˣ�����SDK�ڲ��ɼ��̣߳�Ȼ���ٿ�ʼ�����ͼ�������������֡�ʡ�
			CameraSoftTrigger(pThis->m_hCamera);

			status = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer,&sFrameInfo);//����ģʽ
	
			if(status == CAMERA_STATUS_SUCCESS)
            {
            	//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);

			
					if (iCaptureCounts % 2)//ͨ��2�������ʾ��ͨ��1���ұ���ʾ
					{
						for (i = 0;i < sFrameInfo.iHeight;i++)
						{
							memcpy(pThis->m_pDisplayBuffer + i*sFrameInfo.iWidth*2*3,pThis->m_pFrameBuffer + i*sFrameInfo.iWidth*3,sFrameInfo.iWidth*3);
						}

					}
					else
					{
						for (i = 0;i < sFrameInfo.iHeight;i++)
						{
							memcpy(pThis->m_pDisplayBuffer + i*sFrameInfo.iWidth*2*3 + sFrameInfo.iWidth*3,pThis->m_pFrameBuffer + i*sFrameInfo.iWidth*3,sFrameInfo.iWidth*3);
						}
					}

					sFrameInfo.iWidth = sFrameInfo.iWidth*2;
					CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pDisplayBuffer, &sFrameInfo);
				
                pThis->m_iDispFrameNum++;
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����ֱ�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
            CameraReleaseImageBuffer(pThis->m_hCamera,pbyBuffer);
		
			memcpy(&pThis->m_sFrInfo,&sFrameInfo,sizeof(tSdkFrameHead));
		}
		else
		{
			//�����ʱ�������´���һ�Σ���ֹ��֡������
			CameraSoftTrigger(pThis->m_hCamera);
		}
		
    }
	
	_endthreadex(0);
    return 0;
}


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
, m_dExp1(15000)
, m_dExp2(5000)
, m_bMergeImage(FALSE)
, m_uGain1(28)
, m_uGain2(14)
, m_sGainRange(_T(""))
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
}

void CBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicDlg)
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_cPreview);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LONG_EXP, m_dExp1);
	DDX_Text(pDX, IDC_EDIT_SHORT_EXP, m_dExp2);
	DDX_Text(pDX, IDC_EDIT_LONG_GAIN1, m_uGain1);
	DDX_Text(pDX, IDC_EDIT_SHORT_GAIN2, m_uGain2);
	DDX_Text(pDX, IDC_STATIC_GAIN_RANGE, m_sGainRange);
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
	ON_EN_CHANGE(IDC_EDIT_LONG_EXP, OnEnChangeEditLongExp)
	ON_EN_CHANGE(IDC_EDIT_SHORT_EXP, OnEnChangeEditShortExp)
	ON_EN_CHANGE(IDC_EDIT_LONG_GAIN1, OnEnChangeEditLongGain1)
	ON_EN_CHANGE(IDC_EDIT_SHORT_GAIN2, OnEnChangeEditShortGain2)
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
		
		//Init the SDK��0:Ӣ�İ� 1:���İ� �������������������Ϣ��SDK���ɵ��豸���ý�����
		SDK_TRACE(CameraSdkInit(gLanguage),gLanguage?"��ʼ��SDK":"Init SDK");

		if (!InitCamera())
		{
			break;
		}
		
		UpdateData(FALSE);

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

void CBasicDlg::OnButtonAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CBasicDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

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
	
	//�ر��Զ��ع⹦�ܣ������ֶ����ó����ع�ķ�ʽ
	CameraSetAeState(m_hCamera,FALSE);

	//Get properties description for this camera.
	SDK_TRACE(CameraGetCapability(m_hCamera,&sCameraInfo),gLanguage?"��ø��������������":"Get the capability of the camera");

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*3,16);	
	m_pDisplayBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*3*2,16);	
	ASSERT(m_pFrameBuffer);

	if (sCameraInfo.sIspCapacity.bMonoSensor)
	{
		// �ڰ�������8λ�Ҷ�
		// Black and white camera output 8-bit grayscale
		CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
	}
	
	//���gain�������õķ�Χֵ
	m_sGainRange.Format(gLanguage?"ģ���������÷�Χ:[%d] - [%d]":"Gain range:[%d] - [%d]",sCameraInfo.sExposeDesc.uiAnalogGainMin,sCameraInfo.sExposeDesc.uiAnalogGainMax);
	//ʹ��SDK��װ�õ���ʾ�ӿ�
	//Use  SDK to display camera images.
	SDK_TRACE(CameraDisplayInit(m_hCamera,m_cPreview.GetSafeHwnd()),gLanguage?"��ʼ����ʾ�ӿ�":"Init display module");
	m_cPreview.GetClientRect(&rect);
	//Set display window size
	SDK_TRACE(CameraSetDisplaySize(m_hCamera,rect.right - rect.left,rect.bottom - rect.top),gLanguage?"������ʾ���ڴ�С":"Set display size");
	
	//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
	//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
	//Create the settings window for the camera
	SDK_TRACE(CameraCreateSettingPage(m_hCamera,GetSafeHwnd(),
								sCameraList[0].acFriendlyName,NULL,(PVOID)this,-1 & ~(1 << PROP_SHEET_INDEX_EXPOSURE))
								,gLanguage?"֪ͨSDK�ڲ��������������ҳ��":"Create camera config page");
    


	//Tell the camera begin to sendding image
	SDK_TRACE(CameraPlay(m_hCamera),gLanguage?"��ʼԤ��":"Start preview");
	m_bPause = FALSE;
	GetDlgItem(IDC_BUTTON_PREVIEW)->SetWindowText(gLanguage?"��ͣ":"Pause");
	

#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
	//Set the callback for image capture
	SDK_TRACE(CameraSetCallbackFunction(m_hCamera,GrabImageCallback,(PVOID)this,NULL),gLanguage?"����ͼ��ץȡ�Ļص�����":"Set image grab call back function");
#else
	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0,  &m_threadID);
	ASSERT (m_hDispThread); 
	SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
	#endif


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
		CameraSetTriggerMode(m_hCamera,0);
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



void CBasicDlg::OnEnChangeEditLongExp()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
}


void CBasicDlg::OnEnChangeEditShortExp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

}


void CBasicDlg::OnEnChangeEditLongGain1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}


void CBasicDlg::OnEnChangeEditShortGain2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}
