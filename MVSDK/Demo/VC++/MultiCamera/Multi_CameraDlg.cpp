// Multi_CameraDlg.cpp : implementation file
//
//BIG5 TRANS ALLOWED
#include "stdafx.h"
#include "Multi_Camera.h"
#include "Multi_CameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <process.h>
#include <malloc.h>
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
extern int gLanguage;

/*
USE_CALLBACK_GRAB_IMAGE
�����Ҫʹ�ûص������ķ�ʽ���ͼ�����ݣ���ע�ͺ궨��USE_CALLBACK_GRAB_IMAGE.
���ǵ�SDKͬʱ֧�ֻص��������������ýӿ�ץȡͼ��ķ�ʽ�����ַ�ʽ��������"�㿽��"���ƣ������ĳ̶ȵĽ���ϵͳ���ɣ���߳���ִ��Ч�ʡ�
��������ץȡ��ʽ�Ȼص������ķ�ʽ�������������ó�ʱ�ȴ�ʱ��ȣ����ǽ�����ʹ�� uiDisplayThread �еķ�ʽ
*/
//#define USE_CALLBACK_GRAB_IMAGE 

#ifdef USE_CALLBACK_GRAB_IMAGE
/*���1ͼ��ץȡ�ص�����*/
void _stdcall GrabImageCallback1(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{
	
	CameraSdkStatus status;
	CMulti_CameraDlg *pThis = (CMulti_CameraDlg*)pContext;

	//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
	//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
	status = CameraImageProcess(hCamera, pFrameBuffer, pThis->m_pFrameBuffer[0],pFrameHead);
	
	//�ֱ��ʸı��ˣ���ˢ�±���
	if (pThis->m_sFrInfo[0].iWidth != pFrameHead->iWidth || pThis->m_sFrInfo[0].iHeight != pFrameHead->iHeight)
	{
		pThis->m_sFrInfo[0].iWidth = pFrameHead->iWidth;
		pThis->m_sFrInfo[0].iHeight = pFrameHead->iHeight;
		pThis->InvalidateRect(NULL);//�л��ֱ��ʴ�Сʱ������������
	}
	
	if(status == CAMERA_STATUS_SUCCESS && !pThis->m_bPause)
    {
		//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
		CameraImageOverlay(hCamera, pThis->m_pFrameBuffer[0],pFrameHead);
        CameraDisplayRGB24(hCamera, pThis->m_pFrameBuffer[0], pFrameHead);//��������滻���û��Լ�����ʾ����
    }    
    
	memcpy(&pThis->m_sFrInfo[0],pFrameHead,sizeof(tSdkFrameHead));
	
}

/*���2ͼ��ץȡ�ص�����*/
void _stdcall GrabImageCallback2(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{
	
	CameraSdkStatus status;
	CMulti_CameraDlg *pThis = (CMulti_CameraDlg*)pContext;
	
	//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
	//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
	status = CameraImageProcess(hCamera, pFrameBuffer, pThis->m_pFrameBuffer[1],pFrameHead);
	
	//�ֱ��ʸı��ˣ���ˢ�±���
	if (pThis->m_sFrInfo[1].iWidth != pFrameHead->iWidth || pThis->m_sFrInfo[1].iHeight != pFrameHead->iHeight)
	{
		pThis->m_sFrInfo[1].iWidth = pFrameHead->iWidth;
		pThis->m_sFrInfo[1].iHeight = pFrameHead->iHeight;
		pThis->InvalidateRect(NULL);//�л��ֱ��ʴ�Сʱ������������
	}
	
	if(status == CAMERA_STATUS_SUCCESS && !pThis->m_bPause)
    {
		//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
		CameraImageOverlay(hCamera, pThis->m_pFrameBuffer[1],pFrameHead);
        CameraDisplayRGB24(hCamera, pThis->m_pFrameBuffer[1], pFrameHead);//��������滻���û��Լ�����ʾ����
    }    
    
	memcpy(&pThis->m_sFrInfo[1],pFrameHead,sizeof(tSdkFrameHead));
	
}
/*���3ͼ��ץȡ�ص�����*/
void _stdcall GrabImageCallback3(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{
	
	CameraSdkStatus status;
	CMulti_CameraDlg *pThis = (CMulti_CameraDlg*)pContext;
	
	//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
	//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
	status = CameraImageProcess(hCamera, pFrameBuffer, pThis->m_pFrameBuffer[2],pFrameHead);
	
	//�ֱ��ʸı��ˣ���ˢ�±���
	if (pThis->m_sFrInfo[2].iWidth != pFrameHead->iWidth || pThis->m_sFrInfo[2].iHeight != pFrameHead->iHeight)
	{
		pThis->m_sFrInfo[2].iWidth = pFrameHead->iWidth;
		pThis->m_sFrInfo[2].iHeight = pFrameHead->iHeight;
		pThis->InvalidateRect(NULL);//�л��ֱ��ʴ�Сʱ������������
	}
	
	if(status == CAMERA_STATUS_SUCCESS && !pThis->m_bPause)
    {
		//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
		CameraImageOverlay(hCamera, pThis->m_pFrameBuffer[2],pFrameHead);
        CameraDisplayRGB24(hCamera, pThis->m_pFrameBuffer[2], pFrameHead);//��������滻���û��Լ�����ʾ����
    }    
    
	memcpy(&pThis->m_sFrInfo[2],pFrameHead,sizeof(tSdkFrameHead));
	
}

/*���4ͼ��ץȡ�ص�����*/
void _stdcall GrabImageCallback4(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead,PVOID pContext)
{
	
	CameraSdkStatus status;
	CMulti_CameraDlg *pThis = (CMulti_CameraDlg*)pContext;
	
	//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
	//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
	status = CameraImageProcess(hCamera, pFrameBuffer, pThis->m_pFrameBuffer[3],pFrameHead);
	
	//�ֱ��ʸı��ˣ���ˢ�±���
	if (pThis->m_sFrInfo[3].iWidth != pFrameHead->iWidth || pThis->m_sFrInfo[3].iHeight != pFrameHead->iHeight)
	{
		pThis->m_sFrInfo[3].iWidth = pFrameHead->iWidth;
		pThis->m_sFrInfo[3].iHeight = pFrameHead->iHeight;
		pThis->InvalidateRect(NULL);//�л��ֱ��ʴ�Сʱ������������
	}
	
	if(status == CAMERA_STATUS_SUCCESS && !pThis->m_bPause)
    {
		//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
		CameraImageOverlay(hCamera, pThis->m_pFrameBuffer[3],pFrameHead);
        CameraDisplayRGB24(hCamera, pThis->m_pFrameBuffer[3], pFrameHead);//��������滻���û��Լ�����ʾ����
    }    
    
	memcpy(&pThis->m_sFrInfo[3],pFrameHead,sizeof(tSdkFrameHead));
	
}
#else 
/*���1ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread1(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CMulti_CameraDlg* 		pThis = (CMulti_CameraDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	
	
	while (!pThis->m_bExit[0])
    {   
		
		if(CameraGetImageBuffer(pThis->m_hCamera[0],&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera[0], pbyBuffer, pThis->m_pFrameBuffer[0],&sFrameInfo);//����ģʽ
			
			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo[0].iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo[0].iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo[0].iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo[0].iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}
			
			if(status == CAMERA_STATUS_SUCCESS)
            {
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera[0], pThis->m_pFrameBuffer[0], &sFrameInfo);
                CameraDisplayRGB24(pThis->m_hCamera[0], pThis->m_pFrameBuffer[0], &sFrameInfo);
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
            CameraReleaseImageBuffer(pThis->m_hCamera[0],pbyBuffer);
            
			memcpy(&pThis->m_sFrInfo[0],&sFrameInfo,sizeof(tSdkFrameHead));
		}
		
    }
	
	_endthreadex(0);
    return 0;
}
/*���2ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread2(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CMulti_CameraDlg* 		pThis = (CMulti_CameraDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	
	
	while (!pThis->m_bExit[1])
    {   
		
		if(CameraGetImageBuffer(pThis->m_hCamera[1],&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera[1], pbyBuffer, pThis->m_pFrameBuffer[1],&sFrameInfo);//����ģʽ
			
			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo[1].iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo[1].iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo[1].iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo[1].iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}
			
			if(status == CAMERA_STATUS_SUCCESS)
            {
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera[1], pThis->m_pFrameBuffer[1], &sFrameInfo);
                CameraDisplayRGB24(pThis->m_hCamera[1], pThis->m_pFrameBuffer[1], &sFrameInfo);
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
            CameraReleaseImageBuffer(pThis->m_hCamera[1],pbyBuffer);
            
			memcpy(&pThis->m_sFrInfo[1],&sFrameInfo,sizeof(tSdkFrameHead));
		}
		
    }
	
	_endthreadex(0);
    return 0;
}
/*���3ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread3(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CMulti_CameraDlg* 		pThis = (CMulti_CameraDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	
	
	while (!pThis->m_bExit[2])
    {   
		
		if(CameraGetImageBuffer(pThis->m_hCamera[2],&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera[2], pbyBuffer, pThis->m_pFrameBuffer[2],&sFrameInfo);//����ģʽ
			
			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo[2].iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo[2].iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo[2].iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo[2].iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}
			
			if(status == CAMERA_STATUS_SUCCESS)
            {
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera[2], pThis->m_pFrameBuffer[2], &sFrameInfo);
                CameraDisplayRGB24(pThis->m_hCamera[2], pThis->m_pFrameBuffer[2], &sFrameInfo);
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
            CameraReleaseImageBuffer(pThis->m_hCamera[2],pbyBuffer);
            
			memcpy(&pThis->m_sFrInfo[2],&sFrameInfo,sizeof(tSdkFrameHead));
		}
		
    }
	
	_endthreadex(0);
    return 0;
}
/*���4ͼ��ץȡ�̣߳���������SDK�ӿں�����ȡͼ��*/
UINT WINAPI uiDisplayThread4(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CMulti_CameraDlg* 		pThis = (CMulti_CameraDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;
	
	
	while (!pThis->m_bExit[3])
    {   
		
		if(CameraGetImageBuffer(pThis->m_hCamera[3],&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera[3], pbyBuffer, pThis->m_pFrameBuffer[3],&sFrameInfo);//����ģʽ
			
			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo[3].iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo[3].iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo[3].iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo[3].iHeight = sFrameInfo.iHeight;
				pThis->InvalidateRect(NULL);
			}
			
			if(status == CAMERA_STATUS_SUCCESS)
            {
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera[3], pThis->m_pFrameBuffer[3], &sFrameInfo);
                CameraDisplayRGB24(pThis->m_hCamera[3], pThis->m_pFrameBuffer[3], &sFrameInfo);
            }    
            
			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
            CameraReleaseImageBuffer(pThis->m_hCamera[3],pbyBuffer);
            
			memcpy(&pThis->m_sFrInfo[3],&sFrameInfo,sizeof(tSdkFrameHead));
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
// CMulti_CameraDlg dialog

CMulti_CameraDlg::CMulti_CameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(gLanguage == 0?IDD_MULTI_CAMERA_DIALOG_EN:IDD_MULTI_CAMERA_DIALOG, pParent)
{
	//{{AFX_DATA_INIT(CMulti_CameraDlg)
	

	
	m_bCamera1Pause = FALSE;
	m_bCamera2Pause = FALSE;
	m_bCamera3Pause = FALSE;
	m_bCamera4Pause = FALSE;
	
	for (int i = 0;i < 4;i++)
	{
		m_pFrameBuffer[i] = NULL;
		m_bExit[i] = FALSE;
		m_hDispThread[i] = NULL;
	}
	
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMulti_CameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMulti_CameraDlg)
	DDX_Control(pDX, IDC_STATIC_CM3, m_CameraName3);
	DDX_Control(pDX, IDC_STATIC_CM4, m_CameraName4);
	DDX_Control(pDX, IDC_STATIC_CM2, m_CameraName2);
	DDX_Control(pDX, IDC_STATIC_CM1, m_CameraName1);
	DDX_Control(pDX, IDC_CM_VIEW_4, m_DisplayArea4);
	DDX_Control(pDX, IDC_CM_VIEW_3, m_DisplayArea3);
	DDX_Control(pDX, IDC_CM_VIEW_2, m_DisplayArea2);
	DDX_Control(pDX, IDC_CM_VIEW_1, m_DisplayArea1);
	DDX_Check(pDX, IDC_CHECK_PAUSE_1, m_bCamera1Pause);
	DDX_Check(pDX, IDC_CHECK_PAUSE_2, m_bCamera2Pause);
	DDX_Check(pDX, IDC_CHECK_PAUSE_3, m_bCamera3Pause);
	DDX_Check(pDX, IDC_CHECK_PAUSE_4, m_bCamera4Pause);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMulti_CameraDlg, CDialog)
	//{{AFX_MSG_MAP(CMulti_CameraDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_PAUSE_1, OnCheckPause1)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_2, OnCheckPause2)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_3, OnCheckPause3)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_4, OnCheckPause4)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_1, OnButtonCmSet1)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_1, OnButtonSnapshot1)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_2, OnButtonCmSet2)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_2, OnButtonSnapshot2)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_3, OnButtonCmSet3)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_3, OnButtonSnapshot3)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_4, OnButtonCmSet4)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_4, OnButtonSnapshot4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulti_CameraDlg message handlers

BOOL CMulti_CameraDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	// ���ݵ�ǰ�Ѿ����������ϵ������������ʼ����֧��4�����ͬʱ��ʾ��
	CameraSdkInit(1);//"��ʼ��

	if (!InitCamera())
	{
		EndDialog(0);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BOOL CMulti_CameraDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[4];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;
	int i;
	//ö���豸������豸�б�
	iCameraNums = 4;//����CameraEnumerateDeviceǰ��������iCameraNums = 4����ʾ���ֻ��ȡ4���豸��
					//�����Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ,
					//���֧��16�����
	
	if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("No camera was found!");
		return FALSE;
	}
	
	//���˼����������ʾ����
	for (i = 0; i < iCameraNums ;i++)
	{
		if ((status = CameraInit(&sCameraList[i],-1,-1,&m_hCamera[i])) != CAMERA_STATUS_SUCCESS)
		{
			CString msg;
			msg.Format("Failed to init the camera %s! Error code is %d",status,sCameraList[i].acFriendlyName);
			MessageBox(msg+"��ԭ����" + CameraGetErrorString(status));
		}
		else
		{
			//��ʼ���ɹ������н�һ������
			//Get properties description for this camera.
			CameraGetCapability(m_hCamera[i],&sCameraInfo);//"��ø��������������"
			
			m_pFrameBuffer[i] = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*3,16);	
			
			ASSERT(m_pFrameBuffer[i]);

			if (sCameraInfo.sIspCapacity.bMonoSensor)
			{
				// �ڰ�������8λ�Ҷ�
				// Black and white camera output 8-bit grayscale
				CameraSetIspOutFormat(m_hCamera[i], CAMERA_MEDIA_TYPE_MONO8);
			}
			
			//ʹ��SDK��װ�õ���ʾ�ӿ�
			switch (i)
			{
			case 0:
				m_CameraName1.SetWindowText(sCameraList[i].acFriendlyName);
				CameraDisplayInit(m_hCamera[i],m_DisplayArea1.GetSafeHwnd());
				m_DisplayArea1.GetClientRect(&rect);
				//Set display window size
			    CameraSetDisplaySize(m_hCamera[i],rect.right - rect.left,rect.bottom - rect.top);//"������ʾ���ڴ�С";
#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
				//Set the callback for image capture
				CameraSetCallbackFunction(m_hCamera[i],GrabImageCallback1,(PVOID)this,NULL);//"����ͼ��ץȡ�Ļص�����";
#else
				m_hDispThread[i] = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread1, this, 0,  &m_threadID[i]);
				ASSERT (m_hDispThread[i]); 
				SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
#endif
				break;

			case 1:
				m_CameraName2.SetWindowText(sCameraList[i].acFriendlyName);
				CameraDisplayInit(m_hCamera[i],m_DisplayArea2.GetSafeHwnd());
				m_DisplayArea2.GetClientRect(&rect);
				//Set display window size
			    CameraSetDisplaySize(m_hCamera[i],rect.right - rect.left,rect.bottom - rect.top);//"������ʾ���ڴ�С";
#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
				//Set the callback for image capture
				CameraSetCallbackFunction(m_hCamera[i],GrabImageCallback2,(PVOID)this,NULL);//"����ͼ��ץȡ�Ļص�����";
#else
				m_hDispThread[i] = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread2, this, 0,  &m_threadID[i]);
				ASSERT (m_hDispThread[i]); 
				SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
#endif
				break;

			case 2:
				m_CameraName3.SetWindowText(sCameraList[i].acFriendlyName);
				CameraDisplayInit(m_hCamera[i],m_DisplayArea3.GetSafeHwnd());
				m_DisplayArea3.GetClientRect(&rect);
				//Set display window size
			    CameraSetDisplaySize(m_hCamera[i],rect.right - rect.left,rect.bottom - rect.top);//"������ʾ���ڴ�С";
#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
				//Set the callback for image capture
				CameraSetCallbackFunction(m_hCamera[i],GrabImageCallback3,(PVOID)this,NULL);//"����ͼ��ץȡ�Ļص�����";
#else
				m_hDispThread[i] = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread3, this, 0,  &m_threadID[i]);
				ASSERT (m_hDispThread[i]); 
				SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
#endif
				break;

			case 3:
				m_CameraName4.SetWindowText(sCameraList[i].acFriendlyName);
				CameraDisplayInit(m_hCamera[i],m_DisplayArea4.GetSafeHwnd());
				m_DisplayArea4.GetClientRect(&rect);
				//Set display window size
			    CameraSetDisplaySize(m_hCamera[i],rect.right - rect.left,rect.bottom - rect.top);//"������ʾ���ڴ�С";
#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
				//Set the callback for image capture
				CameraSetCallbackFunction(m_hCamera[i],GrabImageCallback4,(PVOID)this,NULL);//"����ͼ��ץȡ�Ļص�����";
#else
				m_hDispThread[i] = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread4, this, 0,  &m_threadID[i]);
				ASSERT (m_hDispThread[i]); 
				SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
#endif
				break;
			}

		
			
			//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
			//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
			//Create the settings window for the camera
			CameraCreateSettingPage(m_hCamera[i],GetSafeHwnd(),
				sCameraList[i].acFriendlyName,NULL,NULL,0);
				
		
			//Tell the camera begin to sendding image
			CameraPlay(m_hCamera[i]);//"��ʼԤ��";
			
		}
	}

	

	return TRUE;
}


void CMulti_CameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMulti_CameraDlg::OnPaint() 
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
HCURSOR CMulti_CameraDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMulti_CameraDlg::OnCheckPause1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCamera1Pause)
	{
		CameraPause(m_hCamera[0]);
	}
	else
	{
		CameraPlay(m_hCamera[0]);
	}
}

void CMulti_CameraDlg::OnCheckPause2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCamera2Pause)
	{
		CameraPause(m_hCamera[1]);
	}
	else
	{
		CameraPlay(m_hCamera[1]);
	}
}

void CMulti_CameraDlg::OnCheckPause3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCamera3Pause)
	{
		CameraPause(m_hCamera[2]);
	}
	else
	{
		CameraPlay(m_hCamera[2]);
	}
}

void CMulti_CameraDlg::OnCheckPause4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bCamera4Pause)
	{
		CameraPause(m_hCamera[3]);
	}
	else
	{
		CameraPlay(m_hCamera[3]);
	}
}

void CMulti_CameraDlg::OnButtonCmSet1() 
{
	// TODO: Add your control notification handler code here
	if (m_hCamera[0] > 0)//�ж�����Ƿ��
	{
		CameraShowSettingPage(m_hCamera[0],TRUE);//"��ʾ������������ô��ڣ��ô�����SDK�ڲ�����";
	}
	
}

void CMulti_CameraDlg::OnButtonSnapshot1() 
{
	// TODO: Add your control notification handler code here
	Snapshot(m_hCamera[0]);
}

void CMulti_CameraDlg::OnButtonCmSet2() 
{
	// TODO: Add your control notification handler code here
	if (m_hCamera[1] > 0)//�ж�����Ƿ��
	{
		CameraShowSettingPage(m_hCamera[1],TRUE);//"��ʾ������������ô��ڣ��ô�����SDK�ڲ�����";
	}
	
}

void CMulti_CameraDlg::OnButtonSnapshot2() 
{
	// TODO: Add your control notification handler code here
	Snapshot(m_hCamera[1]);
}

void CMulti_CameraDlg::OnButtonCmSet3() 
{
	// TODO: Add your control notification handler code here
	if (m_hCamera[2] > 0)//�ж�����Ƿ��
	{
		CameraShowSettingPage(m_hCamera[2],TRUE);//"��ʾ������������ô��ڣ��ô�����SDK�ڲ�����";
	}
}

void CMulti_CameraDlg::OnButtonSnapshot3() 
{
	// TODO: Add your control notification handler code here
	Snapshot(m_hCamera[2]);
}

void CMulti_CameraDlg::OnButtonCmSet4() 
{
	// TODO: Add your control notification handler code here
	if (m_hCamera[3] > 0)//�ж�����Ƿ��
	{
		CameraShowSettingPage(m_hCamera[3],TRUE);//"��ʾ������������ô��ڣ��ô�����SDK�ڲ�����";
	}
}

void CMulti_CameraDlg::OnButtonSnapshot4() 
{
	// TODO: Add your control notification handler code here
	Snapshot(m_hCamera[3]);
}

void CMulti_CameraDlg::OnClose() 
{
	int i;
	for (i = 0;i < 4;i++)
	{
		//����ʼ�����
		if (m_hCamera[i] > 0)
		{
			if (NULL != m_hDispThread[i])
			{
				//�ȴ��ɼ��߳̽���
				m_bExit[i] = TRUE;
				::WaitForSingleObject(m_hDispThread[i], 2000);
				CloseHandle(m_hDispThread[i]);
				m_hDispThread[i] = NULL;
			}
			
			//����ʼ�������
			CameraUnInit(m_hCamera[i]);
			m_hCamera[i] = 0;
		}
		
		if (m_pFrameBuffer[i])
		{
			CameraAlignFree(m_pFrameBuffer[i]);
			m_pFrameBuffer[i] = NULL;
		}
	}
	
	CDialog::OnClose();
}

void CMulti_CameraDlg::Snapshot(int hCamera)
{
	tSdkFrameHead FrameInfo;
	BYTE *pRawBuffer;
	BYTE *pRgbBuffer;
	CString sFileName;
	tSdkImageResolution sImageSize;
	CameraSdkStatus status;
	CString msg;

	if (hCamera < 1)//��Ч��������
	{
		return;
	}

	memset(&sImageSize,0,sizeof(tSdkImageResolution));
	sImageSize.iIndex = 0xff;
	//CameraSetResolutionForSnap����ץ��ʱ�ķֱ��ʣ����Ժ�Ԥ��ʱ��ͬ��Ҳ���Բ�ͬ��
	//sImageSize.iIndex = 0xff; sImageSize.iWidth �� sImageSize.iHeight ��0����ʾץ��ʱ�ֱ��ʺ�Ԥ��ʱ��ͬ��
	//�����ϣ��ץ��ʱΪ�����ķֱ��ʣ���ο����ǵ�Snapshot���̡����߲���SDK�ֲ����й�CameraSetResolutionForSnap�ӿڵ���ϸ˵��
	CameraSetResolutionForSnap(hCamera,&sImageSize);//"����ץ��ģʽ�µķֱ���";
	
	//	CameraSnapToBufferץ��һ֡ͼ�����ݵ��������У��û�������SDK�ڲ�����,�ɹ����ú���Ҫ
	// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
	if((status = CameraSnapToBuffer(hCamera,&FrameInfo,&pRawBuffer,1000)) != CAMERA_STATUS_SUCCESS)
	{
		MessageBox("Snapshot failed,is camera in pause mode?");
		return;
	}
	else
	{
	
		//�ɹ�ץ�ĺ󣬱��浽�ļ�
		CString msg;
		char sCurpath[128];
		CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
		GetCurrentDirectory(128,sCurpath);
		sFileName.Format("%s\\Snapshot%s",sCurpath ,strTime);//��ʼ�������ļ����ļ���
		
		//����һ��buffer����������õ�ԭʼ����ת��ΪRGB���ݣ���ͬʱ���ͼ����Ч��
		pRgbBuffer = (BYTE *)CameraAlignMalloc(FrameInfo.iWidth*FrameInfo.iHeight*3,16);
		//Process the raw data,and get the return image in RGB format
		CameraImageProcess(hCamera,pRawBuffer,pRgbBuffer,&FrameInfo);//����ͼ�񣬲��õ�RGB��ʽ������";
		
		//Release the buffer which get from CameraSnapToBuffer or CameraGetImageBuffer
		CameraReleaseImageBuffer(hCamera,pRawBuffer);//"�ͷ���CameraSnapToBuffer��CameraGetImageBuffer��õ�ͼ�񻺳���";
		
		//CameraSaveImage ����ͼ�����������ʾ��α���BMPͼ�������Ҫ�����������ʽ�ģ������JPG,PNG,RAW�ȣ�
		//��ο����ǵ�Snapshot���̡����߲���SDK�ֲ����й�CameraSaveImage�ӿڵ���ϸ˵��
		if((status = CameraSaveImage(hCamera,sFileName.GetBuffer(1),pRgbBuffer,&FrameInfo,FILE_BMP,100)) != CAMERA_STATUS_SUCCESS)
		{
			msg.Format("Failed to save image to [%s] ,please check the path",sFileName);
			MessageBox(msg);
		}
		else
		{
			msg.Format("Snapshot one image to file:[%s.BMP]",sFileName);
			MessageBox(msg);
		}
		
		CameraAlignFree(pRgbBuffer);
		
	}
}