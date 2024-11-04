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
, m_iOutputFormat(1)
, m_iSaveFormat(0)
, m_sFileSavePath(_T("C:\\Camera Capture Image\\CaptureImage"))
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
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO6, m_iOutputFormat);
	DDX_Radio(pDX, IDC_RADIO1, m_iSaveFormat);
	DDX_Text(pDX, IDC_EDIT1, m_sFileSavePath);
}

BEGIN_MESSAGE_MAP(CBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_SETTINGS, OnButtonCameraSettings)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadioOutPutFormat)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadioOutPutFormat)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadioOutPutFormat)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadioSaveFormat)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadioSaveFormat)
	ON_BN_CLICKED(IDC_RADIO4, OnBnClickedRadioSaveFormat)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadioSaveFormat)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadioSaveFormat)
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

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*4,16);	//���ISP�����RGBA��ʽ����һ������ռ4���ֽڣ���������*4��С���ڴ�

	ASSERT(m_pFrameBuffer);
	
	if (sCameraInfo.sIspCapacity.bMonoSensor == TRUE)
	{
		//�ڰ����������ͼ�񱣴��ʽΪ8bit�Ҷ�bmpλͼ��SDK�����ʽΪ8bit
		m_iSaveFormat = 2;
		m_iOutputFormat = 0;
		SDK_TRACE(CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8),gLanguage?"�ڰ��������Ϊ8λ�Ҷȸ�ʽ���":"Set mono8bit as isp format");
	}
	else
	{
		//��ɫ���������ͼ�񱣴��ʽΪ24λ�Ҷ�bmpλͼ��SDK�����ʽΪ24��RGB����,�����ʹ��opencv��labview��Ҳ��������Ϊ32λ�Ĳ�ɫ��ʽ��ͼ�����ݰ�4�ֽڶ��룬����ʹ��Ӳ�����١�
		m_iSaveFormat = 1;
		m_iOutputFormat = 1;
		SDK_TRACE(CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_RGB8),gLanguage?"��ɫ�������ΪRGB24��ʽ���":"Set RGB24 as isp format");
	}

	//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
	//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
	//Create the settings window for the camera
	SDK_TRACE(CameraCreateSettingPage(m_hCamera,GetSafeHwnd(),
								sCameraList[0].acFriendlyName,NULL,(PVOID)this,0)
								,gLanguage?"֪ͨSDK�ڲ��������������ҳ��":"Create camera config page");
 
	
	//Tell the camera begin to sendding image
	SDK_TRACE(CameraPlay(m_hCamera),gLanguage?"��ʼԤ��":"Start preview");

	
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

//Capture and save image to a file
void CBasicDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	int status;
	tSdkFrameHead tImageHead;
	BYTE *pbyBuffer;
	CString sFileName;
	UpdateData(TRUE);

	//���һ֡ԭʼͼ������
	if(CameraGetImageBuffer(m_hCamera,&tImageHead,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
	{	
		//Ҫ����RAW���ݣ��������CameraImageProcess��RAW���ݿ�����photoshop�򿪡�
		if (m_iSaveFormat == 0)
		{
			sFileName = m_sFileSavePath + ".raw";
			CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),pbyBuffer,&tImageHead,FILE_RAW,100);
		}
		else
		{
			//����CameraImageProcess����ͼ������������ͼ�����ݸ�ʽ����CameraSetIspOutFormat�����������ĸ�ʽ������tImageHead.uiMediaType�б���
			status = CameraImageProcess(m_hCamera, pbyBuffer, m_pFrameBuffer,&tImageHead);//����ģʽ

			ASSERT(status == CAMERA_STATUS_SUCCESS);

			//����ʮ���ߡ�AE��AWB���ڣ�����Ҫ��ɺ�����һ��
			CameraImageOverlay(m_hCamera, m_pFrameBuffer, &tImageHead);


			//���ݽ�����ѡ��ĸ�ʽ������ͼ��
			switch(m_iSaveFormat)
			{
			case 1:
			default:
				sFileName = m_sFileSavePath + "(24bit).bmp";
				CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),m_pFrameBuffer,&tImageHead,FILE_BMP,100);
				break;

			case 2:
				//�����Ҫ�����8λbmp��ͼ�����ȵ��ú�CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8);
				sFileName = m_sFileSavePath + "(8bit).bmp";
				CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),m_pFrameBuffer,&tImageHead,FILE_BMP_8BIT,100);
				break;

			case 3:
				sFileName = m_sFileSavePath + ".png";
				CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),m_pFrameBuffer,&tImageHead,FILE_PNG,100);
				break;

			case 4:
				sFileName = m_sFileSavePath + ".jpg";
				CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),m_pFrameBuffer,&tImageHead,FILE_JPG,100);
				break;
			}
		}
		

		//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
		//�����ٴε���CameraGetImageBufferʱ�����򽫱�����������ֱ�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
		CameraReleaseImageBuffer(m_hCamera,pbyBuffer);

	}
	else
	{
		MessageBox("ץͼ��ʱ���Ƿ��趨�ڴ���ģʽ�£�");
	}
}


//����SDK��ISP������ͼ�������ʽ
void CBasicDlg::OnBnClickedRadioOutPutFormat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	switch(m_iOutputFormat)
	{
		//����Ϊ8bit�Ҷȸ�ʽ���ڰ����������Ϊ8bit�ĻҶ�ͼ��������Խ�Լ�ڴ�洢�ռ䣬ͬʱ�����ͼ���㷨������ٶȡ�����halcon��labview�Լ�opencv��֧��8bit�ĻҶ�λͼֱ�Ӵ���
	case 0:
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8);
		break;

		//����Ϊ24bitλͼ��ʽ�����Ĭ������ø�ʽ���ʺϲ�ɫ�����
	case 1:
	default:
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_RGB8);
		break;

		//����Ϊ32bitλͼ��ʽ,opencv��labview��halcon��ͼ����������п�����Ҫ32λ��ȵĲ�ɫͼ���ʽ��һ�����ذ�4���ֽڶ��룬�������SSE�㷨���м���
	case 2:
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_RGBA8);
		break;

	}

}



void CBasicDlg::OnBnClickedRadioSaveFormat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//�������Ҫ����BMP 8λ��ȵ�ͼ������Ҫ��SDK��ISP��ʽ����л�8bit�������ڱ���ͼ��ʱ������������ת��������
	if (m_iSaveFormat == 2)
	{
		m_iOutputFormat = 0;
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_MONO8);
	}
	else if (m_iSaveFormat == 1 || m_iSaveFormat == 3 || m_iSaveFormat == 4)//����Ǳ���24λλ���ͼƬ�����л�SDK ��ISP�����ʽΪ24bit
	{
		m_iOutputFormat = 1;
		CameraSetIspOutFormat(m_hCamera,CAMERA_MEDIA_TYPE_RGB8);
	}

	UpdateData(FALSE);
}

