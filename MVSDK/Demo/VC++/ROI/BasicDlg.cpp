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

/*������ô��ڵ���Ϣ�ص�����
hCamera:��ǰ����ľ��
MSG:��Ϣ���ͣ�
	SHEET_MSG_LOAD_PARAM_DEFAULT	= 0,//����Ĭ�ϲ����İ�ť�����������Ĭ�ϲ�����ɺ󴥷�����Ϣ,
	SHEET_MSG_LOAD_PARAM_GROUP		= 1,//�л���������ɺ󴥷�����Ϣ,
	SHEET_MSG_LOAD_PARAM_FROMFILE	= 2,//���ز�����ť��������Ѵ��ļ��м�����������󴥷�����Ϣ
	SHEET_MSG_SAVE_PARAM_GROUP		= 3//���������ť���������������󴥷�����Ϣ
	����μ�CameraDefine.h��emSdkPropSheetMsg����

uParam:��Ϣ�����Ĳ�������ͬ����Ϣ���������岻ͬ��
	�� MSG Ϊ SHEET_MSG_LOAD_PARAM_DEFAULTʱ��uParam��ʾ�����س�Ĭ�ϲ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
	�� MSG Ϊ SHEET_MSG_LOAD_PARAM_GROUPʱ��uParam��ʾ�л���Ĳ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
	�� MSG Ϊ SHEET_MSG_LOAD_PARAM_FROMFILEʱ��uParam��ʾ���ļ��в������ǵĲ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
	�� MSG Ϊ SHEET_MSG_SAVE_PARAM_GROUPʱ��uParam��ʾ��ǰ����Ĳ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
*/
void _stdcall CameraSettingPageCallback(CameraHandle hCamera,UINT MSG,UINT uParam,PVOID pContext)
{
	CBasicDlg *pDialog = (CBasicDlg*)pContext;
	CString sMsg;

	if (MSG == SHEET_MSG_LOAD_PARAM_DEFAULT)
	{
		sMsg.Format("Parameter group[%d] was loaded to default!",uParam);
	}
	else if (MSG == SHEET_MSG_LOAD_PARAM_GROUP)
	{
		sMsg.Format("Parameter group[%d] was loaded!",uParam);
	}
	else if (MSG == SHEET_MSG_LOAD_PARAM_FROMFILE)
	{
		sMsg.Format("Parameter group[%d] was loaded from file!",uParam);
	}
	else if (MSG == SHEET_MSG_SAVE_PARAM_GROUP)
	{
		sMsg.Format("Parameter group[%d] was saved!",uParam);
	}
	else
	{
		return;//unknown message type
	}
	pDialog->m_DlgLog.AppendLog("CallBack: CameraSettingPageCallback");
	pDialog->m_DlgLog.AppendLog(sMsg);
	pDialog->m_DlgLog.AppendLog(" ");

}


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
	m_sAboutInfo = _T("�����ɿ��ӻ���������!");
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
, m_bEnableTransferRoi1(FALSE)
, m_bEnableTransferRoi2(FALSE)
, m_bEnableTransferRoi3(FALSE)
, m_bEnableTransferRoi4(FALSE)
, m_TransforRoi1_x1(0)
, m_TransforRoi1_x2(499)
, m_TransforRoi1_y1(0)
, m_TransforRoi1_y2(499)
, m_TransforRoi2_x1(600)
, m_TransforRoi2_x2(999)
, m_TransforRoi2_y1(600)
, m_TransforRoi2_y2(999)
, m_TransforRoi3_x1(1000)
, m_TransforRoi3_x2(1999)
, m_TransforRoi3_y1(1000)
, m_TransforRoi3_y2(1943)
, m_TransforRoi4_x1(800)
, m_TransforRoi4_x2(2591)
, m_TransforRoi4_y1(0)
, m_TransforRoi4_y2(499)
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
	DDX_Check(pDX, IDC_CHECK1, m_bEnableTransferRoi1);
	DDX_Check(pDX, IDC_CHECK2, m_bEnableTransferRoi2);
	DDX_Check(pDX, IDC_CHECK3, m_bEnableTransferRoi3);
	DDX_Check(pDX, IDC_CHECK4, m_bEnableTransferRoi4);
	DDX_Text(pDX, IDC_AREA1_X1, m_TransforRoi1_x1);
	DDX_Text(pDX, IDC_AREA1_X2, m_TransforRoi1_x2);
	DDX_Text(pDX, IDC_AREA1_Y1, m_TransforRoi1_y1);
	DDX_Text(pDX, IDC_AREA1_Y2, m_TransforRoi1_y2);
	DDX_Text(pDX, IDC_AREA2_X1, m_TransforRoi2_x1);
	DDX_Text(pDX, IDC_AREA2_X2, m_TransforRoi2_x2);
	DDX_Text(pDX, IDC_AREA2_Y1, m_TransforRoi2_y1);
	DDX_Text(pDX, IDC_AREA2_Y2, m_TransforRoi2_y2);
	DDX_Text(pDX, IDC_AREA3_X1, m_TransforRoi3_x1);
	DDX_Text(pDX, IDC_AREA3_X2, m_TransforRoi3_x2);
	DDX_Text(pDX, IDC_AREA3_Y1, m_TransforRoi3_y1);
	DDX_Text(pDX, IDC_AREA3_Y2, m_TransforRoi3_y2);
	DDX_Text(pDX, IDC_AREA4_X1, m_TransforRoi4_x1);
	DDX_Text(pDX, IDC_AREA4_X2, m_TransforRoi4_x2);
	DDX_Text(pDX, IDC_AREA4_Y1, m_TransforRoi4_y1);
	DDX_Text(pDX, IDC_AREA4_Y2, m_TransforRoi4_y2);
}

BEGIN_MESSAGE_MAP(CBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_SETTINGS, OnButtonCameraSettings)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT, OnButtonSnapshot)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedEnableTranferRoi1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedEnableTranferRoi2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedEnableTranferRoi3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedEnableTranferRoi4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButtonUpdateTransferRoi1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButtonUpdateTransferRoi2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButtonUpdateTransferRoi3)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButtonUpdateTransferRoi4)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CBasicDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_COPY_CODE, &CBasicDlg::OnBnClickedButtonCopyCode)
	ON_BN_CLICKED(IDC_BUTTON_SET_ROI, &CBasicDlg::OnBnClickedButtonSetRoi)
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

void CBasicDlg::OnButtonSnapshot() //ץ��һ��ͼƬ
{
	
	tSdkFrameHead FrameInfo;
	BYTE *pRawBuffer;
	BYTE *pRgbBuffer;
	CString sFileName;
	tSdkImageResolution sImageSize;
	CameraSdkStatus status;
	CString msg;
	memset(&sImageSize,0,sizeof(tSdkImageResolution));
	sImageSize.iIndex = 0xff;
	//CameraSetResolutionForSnap����ץ��ʱ�ķֱ��ʣ����Ժ�Ԥ��ʱ��ͬ��Ҳ���Բ�ͬ��
	//sImageSize.iIndex = 0xff; sImageSize.iWidth �� sImageSize.iHeight ��0����ʾץ��ʱ�ֱ��ʺ�Ԥ��ʱ��ͬ��
	//�����ϣ��ץ��ʱΪ�����ķֱ��ʣ���ο����ǵ�Snapshot���̡����߲���SDK�ֲ����й�CameraSetResolutionForSnap�ӿڵ���ϸ˵��
	SDK_TRACE(CameraSetResolutionForSnap(m_hCamera,&sImageSize),gLanguage?"����ץ��ģʽ�µķֱ���":"Set resolution for snapshot");

	//	CameraSnapToBufferץ��һ֡ͼ�����ݵ��������У��û�������SDK�ڲ�����,�ɹ����ú���Ҫ
	// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
	if((status = CameraSnapToBuffer(m_hCamera,&FrameInfo,&pRawBuffer,1000)) != CAMERA_STATUS_SUCCESS)
	{
		m_DlgLog.AppendLog("Function:[CameraSnapToBuffer] failed!");
		msg.Format("Error Code:%d. Get more information about the error in CameraDefine.h",status);
		m_DlgLog.AppendLog(msg);
		m_DlgLog.AppendLog(" ");
		MessageBox("Snapshot failed,is camera in pause mode?");
		return;
	}
	else
	{
		msg.Format("Function:[%s] SUCCESS!","CameraSnapToBuffer");
		m_DlgLog.AppendLog(msg);
		msg.Format("Description:%s.","Capture a image to the buffer in snapshot mode");
		m_DlgLog.AppendLog(msg);
		m_DlgLog.AppendLog(" ");
		
		//�ɹ�ץ�ĺ󣬱��浽�ļ�
		CString msg;
		char sCurpath[128];
		CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
		GetCurrentDirectory(128,sCurpath);
		sFileName.Format("%s\\Snapshot%s",sCurpath ,strTime);//��ʼ�������ļ����ļ���

		//����һ��buffer����������õ�ԭʼ����ת��ΪRGB���ݣ���ͬʱ���ͼ����Ч��
		pRgbBuffer = (BYTE *)CameraAlignMalloc(FrameInfo.iWidth*FrameInfo.iHeight*3,16);
		//Process the raw data,and get the return image in RGB format
		SDK_TRACE(CameraImageProcess(m_hCamera,pRawBuffer,pRgbBuffer,&FrameInfo),
			gLanguage?"����ͼ�񣬲��õ�RGB��ʽ������":"process the raw data to rgb data");
		
		 //Release the buffer which get from CameraSnapToBuffer or CameraGetImageBuffer
		SDK_TRACE(CameraReleaseImageBuffer(m_hCamera,pRawBuffer),
			gLanguage?"�ͷ���CameraSnapToBuffer��CameraGetImageBuffer��õ�ͼ�񻺳���":"Release the buffer malloced by CameraSnapToBuffer or CameraGetImageBuffer");
													  
		//CameraSaveImage ����ͼ�����������ʾ��α���BMPͼ�������Ҫ�����������ʽ�ģ������JPG,PNG,RAW�ȣ�
		//��ο����ǵ�Snapshot���̡����߲���SDK�ֲ����й�CameraSaveImage�ӿڵ���ϸ˵��
		if((status = CameraSaveImage(m_hCamera,sFileName.GetBuffer(1),pRgbBuffer,&FrameInfo,FILE_BMP,100)) != CAMERA_STATUS_SUCCESS)
		{
			m_DlgLog.AppendLog("Function:[CameraSaveImage] failed!");
			msg.Format("Error Code:%d. Get more information about the error in CameraDefine.h",status);
			m_DlgLog.AppendLog(msg);
			m_DlgLog.AppendLog(" ");
			msg.Format("Failed to save image to [%s] ,please check the path",sFileName);
			MessageBox(msg);
		}
		else
		{
			msg.Format("Function:[%s] SUCCESS!","CameraSaveImage");
			m_DlgLog.AppendLog(msg);
			msg.Format("Description:%s.","Save the image data in the buffer to the file");
			m_DlgLog.AppendLog(msg);
			m_DlgLog.AppendLog(" ");

			msg.Format("Snapshot one image to file:[%s.BMP]",sFileName);
			MessageBox(msg);
		}
		
		CameraAlignFree(pRgbBuffer);
		
	}
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
	

	//Get properties description for this camera.
	SDK_TRACE(CameraGetCapability(m_hCamera,&sCameraInfo),gLanguage?"��ø��������������":"Get the capability of the camera");

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*3,16);	

	ASSERT(m_pFrameBuffer);
	memset(&m_sRoiResolution,0,sizeof(m_sRoiResolution));
	m_sRoiResolution.iIndex = 0xff;
	m_sRoiResolution.iHeight = m_sRoiResolution.iHeightFOV = sCameraInfo.sResolutionRange.iHeightMax;
	m_sRoiResolution.iWidth = m_sRoiResolution.iWidthFOV = sCameraInfo.sResolutionRange.iWidthMax;

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
								sCameraList[0].acFriendlyName,CameraSettingPageCallback,(PVOID)this,0)
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
	if (gLanguage != 0)//chinese
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




void CBasicDlg::OnBnClickedEnableTranferRoi1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT uEnableMask = 0;
	UpdateData(TRUE);
	uEnableMask = (m_bEnableTransferRoi1?1:0) | (m_bEnableTransferRoi2?2:0) | (m_bEnableTransferRoi3?4:0) | (m_bEnableTransferRoi4?8:0);

	//ʹ��ǰ�Ȱ�ROI����λ����д������������޷���ȷ����������ROI���书�ܡ�
	if (m_bEnableTransferRoi1)
	{
		OnBnClickedButtonUpdateTransferRoi1();
	}
	

	SDK_TRACE(CameraEnableTransferRoi(m_hCamera,uEnableMask),gLanguage?"ʹ�ܴ���ROI����":"Enable transfer ROI");
}

void CBasicDlg::OnBnClickedButtonUpdateTransferRoi1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iStatus;

	UpdateData(TRUE);

	SDK_TRACE(iStatus = CameraSetTransferRoi(m_hCamera,0,m_TransforRoi1_x1,m_TransforRoi1_y1,m_TransforRoi1_x2,m_TransforRoi1_y2),gLanguage?"���ô���ROI��������":"Set transfer ROI area coordinate");

	if (iStatus == CAMERA_STATUS_NOT_SUPPORTED)
	{
		MessageBox("���ͺŵ������֧�ָù���");
	} 
	else if (iStatus == CAMERA_STATUS_PARAMETER_INVALID)
	{
		MessageBox("�������������ȷ��ROI�����ڵ�ǰ�ֱ��ʵ�ͼ��ߴ緶Χ�ڣ�����ȷ��X2 > X1��Y2 > Y1");
	}
	
}


void CBasicDlg::OnBnClickedEnableTranferRoi2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT uEnableMask = 0;
	UpdateData(TRUE);
	uEnableMask = (m_bEnableTransferRoi1?1:0) | (m_bEnableTransferRoi2?2:0) | (m_bEnableTransferRoi3?4:0) | (m_bEnableTransferRoi4?8:0);

	//ʹ��ǰ�Ȱ�ROI����λ����д������������޷���ȷ����������ROI���书�ܡ�
	if (m_bEnableTransferRoi2)
	{
		OnBnClickedButtonUpdateTransferRoi2();
	}

	SDK_TRACE(CameraEnableTransferRoi(m_hCamera,uEnableMask),gLanguage?"ʹ�ܴ���ROI����":"Enable transfer ROI");
}

void CBasicDlg::OnBnClickedButtonUpdateTransferRoi2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iStatus;

	UpdateData(TRUE);

	SDK_TRACE(iStatus = CameraSetTransferRoi(m_hCamera,1,m_TransforRoi2_x1,m_TransforRoi2_y1,m_TransforRoi2_x2,m_TransforRoi2_y2),gLanguage?"���ô���ROI��������":"Set transfer ROI area coordinate");

	if (iStatus == CAMERA_STATUS_NOT_SUPPORTED)
	{
		MessageBox("���ͺŵ������֧�ָù���");
	} 
	else if (iStatus == CAMERA_STATUS_PARAMETER_INVALID)
	{
		MessageBox("�������������ȷ��ROI�����ڵ�ǰ�ֱ��ʵ�ͼ��ߴ緶Χ�ڣ�����ȷ��X2 > X1��Y2 > Y1");
	}
}

void CBasicDlg::OnBnClickedEnableTranferRoi3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT uEnableMask = 0;
	UpdateData(TRUE);
	uEnableMask = (m_bEnableTransferRoi1?1:0) | (m_bEnableTransferRoi2?2:0) | (m_bEnableTransferRoi3?4:0) | (m_bEnableTransferRoi4?8:0);

	//ʹ��ǰ�Ȱ�ROI����λ����д������������޷���ȷ����������ROI���书�ܡ�
	if (m_bEnableTransferRoi3)
	{
		OnBnClickedButtonUpdateTransferRoi3();
	}

	SDK_TRACE(CameraEnableTransferRoi(m_hCamera,uEnableMask),gLanguage?"ʹ�ܴ���ROI����":"Enable transfer ROI");
}


void CBasicDlg::OnBnClickedButtonUpdateTransferRoi3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iStatus;

	UpdateData(TRUE);

	SDK_TRACE(iStatus = CameraSetTransferRoi(m_hCamera,2,m_TransforRoi3_x1,m_TransforRoi3_y1,m_TransforRoi3_x2,m_TransforRoi3_y2),gLanguage?"���ô���ROI��������":"Set transfer ROI area coordinate");

	if (iStatus == CAMERA_STATUS_NOT_SUPPORTED)
	{
		MessageBox("���ͺŵ������֧�ָù���");
	} 
	else if (iStatus == CAMERA_STATUS_PARAMETER_INVALID)
	{
		MessageBox("�������������ȷ��ROI�����ڵ�ǰ�ֱ��ʵ�ͼ��ߴ緶Χ�ڣ�����ȷ��X2 > X1��Y2 > Y1");
	}
}

void CBasicDlg::OnBnClickedEnableTranferRoi4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UINT uEnableMask = 0;
	UpdateData(TRUE);
	uEnableMask = (m_bEnableTransferRoi1?1:0) | (m_bEnableTransferRoi2?2:0) | (m_bEnableTransferRoi3?4:0) | (m_bEnableTransferRoi4?8:0);

	//ʹ��ǰ�Ȱ�ROI����λ����д������������޷���ȷ����������ROI���书�ܡ�
	if (m_bEnableTransferRoi4)
	{
		OnBnClickedButtonUpdateTransferRoi4();
	}

	SDK_TRACE(CameraEnableTransferRoi(m_hCamera,uEnableMask),gLanguage?"ʹ�ܴ���ROI����":"Enable transfer ROI");
}

void CBasicDlg::OnBnClickedButtonUpdateTransferRoi4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iStatus;

	UpdateData(TRUE);

	SDK_TRACE(iStatus = CameraSetTransferRoi(m_hCamera,3,m_TransforRoi4_x1,m_TransforRoi4_y1,m_TransforRoi4_x2,m_TransforRoi4_y2),gLanguage?"���ô���ROI��������":"Set transfer ROI area coordinate");

	if (iStatus == CAMERA_STATUS_NOT_SUPPORTED)
	{
		MessageBox("���ͺŵ������֧�ָù���");
	} 
	else if (iStatus == CAMERA_STATUS_PARAMETER_INVALID)
	{
		MessageBox("�������������ȷ��ROI�����ڵ�ǰ�ֱ��ʵ�ͼ��ߴ緶Χ�ڣ�����ȷ��X2 > X1��Y2 > Y1");
	}
}


void CBasicDlg::OnBnClickedButtonHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox("[ͼ�񴫸���ROI����]�ǵ������ǰ��ͼ�񴫸����ڲ���ͼ�������������ж�ȡ��ʽ��\
ֻ֧�ֵ�һROI�������ã�ROI����ԽС��ͼ�񴫸��������֡�ʻ�Խ��;\r\n \
[����ü�ROI����]���趨�����ͼ����оƬ�Ĵ�������ͼ����оƬ��ͼ�񴫸����ɼ���ͼ���\r\n \
������ROI�����ڵ�ͼ�����ݣ����Խ�Լ��������Ӷ����֡�ʡ���������£��û���SDK�еõ���ͼ��ߴ���Ȼ\
�ǵ�ǰͼ�񴫸�������ĳߴ�һ����С�ģ�û�б�ָ����������򣬻ᱻ���Ϊ��ɫ��\r\n \
[����ü�ROI����]�У���������ؿ�ȱ�����ż�������������� X2 - X1 + 1 Ϊż���Ĺ�ϵ\
���X2-X1Ϊ������X1�ᱻ�Զ��������������ؿ����ż������Ҫ","����˵��",0);
}



void CBasicDlg::OnBnClickedButtonCopyCode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAboutDlg dlgAbout;
	CString  sTemp;

	dlgAbout.m_sAboutInfo = "";
	
	sTemp.Format("tSdkImageResolution sRoiResolution;\r\n");
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("memset(&sRoiResolution,0,sizeof(sRoiResolution));\r\n");
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iIndex = 0xff;\\\\���ó�0xff��ʾ�Զ���ֱ��ʣ����ó�0��N��ʾѡ��Ԥ��ֱ���\r\n");
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iWidth = %d;\r\n",m_sRoiResolution.iWidth);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iWidthFOV = %d;\r\n",m_sRoiResolution.iWidthFOV);
	dlgAbout.m_sAboutInfo += sTemp;
	
	sTemp.Format("sRoiResolution.iHeight = %d;\r\n",m_sRoiResolution.iHeight);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iHeightFOV = %d;\r\n",m_sRoiResolution.iHeightFOV);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iWidthZoomHd = %d;\r\n",m_sRoiResolution.iWidthZoomHd);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iHeightZoomHd = %d;\r\n",m_sRoiResolution.iHeightZoomHd);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iHOffsetFOV = %d;\r\n",m_sRoiResolution.iHOffsetFOV);
	dlgAbout.m_sAboutInfo += sTemp;
	
	sTemp.Format("sRoiResolution.iVOffsetFOV = %d;\r\n",m_sRoiResolution.iVOffsetFOV);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iWidthZoomSw = %d;\r\n",m_sRoiResolution.iWidthZoomSw);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.iHeightZoomSw = %d;\r\n",m_sRoiResolution.iHeightZoomSw);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.uBinAverageMode = %d;\r\n",m_sRoiResolution.uBinAverageMode);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.uBinSumMode = %d;\r\n",m_sRoiResolution.uBinSumMode);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.uResampleMask = %d;\r\n",m_sRoiResolution.uResampleMask);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("sRoiResolution.uSkipMode = %d;\r\n",m_sRoiResolution.uSkipMode);
	dlgAbout.m_sAboutInfo += sTemp;

	sTemp.Format("CameraSetImageResolution(m_hCamera,&sRoiResolution);\r\n");
	dlgAbout.m_sAboutInfo += sTemp;
	

	dlgAbout.DoModal();

}


void CBasicDlg::OnBnClickedButtonSetRoi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (CameraCustomizeResolution(m_hCamera,&m_sRoiResolution) == CAMERA_STATUS_SUCCESS)
	{
		CameraSetImageResolution(m_hCamera,&m_sRoiResolution);
	}
}
