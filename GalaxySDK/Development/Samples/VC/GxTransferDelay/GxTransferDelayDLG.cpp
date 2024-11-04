// GxTransferDelayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxTransferDelay.h"
#include "GxTransferDelayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strProductVersion;
	CString	m_strLegalCopyRight;
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
	m_strProductVersion = _T("");
	m_strLegalCopyRight = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxTransferDelayDlg dialog

CGxTransferDelayDlg::CGxTransferDelayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxTransferDelayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxTransferDelayDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nDevNum                     = 0;            // �豸����
	m_pDeviceHandle               = NULL;		  // �豸���ָ��
	m_pStream					  = NULL;	      // �豸�����
	m_pCBHandle                   = NULL;         // �ع�ص��������
	m_bIsOpen                     = false;        // ����򿪱�־ 
	m_bIsSnap                     = false;        // �����ʼ�ɼ���־ 
	m_hCam0StartTranEvent         = NULL;         // 0��������������¼�
	m_hCam1StartTranEvent         = NULL;         // 1��������������¼�
	m_hCam2StartTranEvent         = NULL;         // 2��������������¼�
	m_hCam3StartTranEvent         = NULL;         // 3��������������¼�
	m_hCam0StopSnapEvent          = NULL;         // 0�����ֹͣ�ɼ��¼�
	m_hCam1StopSnapEvent          = NULL;         // 1�����ֹͣ�ɼ��¼�
	m_hCam2StopSnapEvent          = NULL;         // 2�����ֹͣ�ɼ��¼�
	m_hCam3StopSnapEvent          = NULL;         // 3�����ֹͣ�ɼ��¼�
	m_pLogWnd                     = NULL;         // ָ����־����ʾ���ڵ�ָ��

	// ��ʾ������Դ��ʼ��
	for (int i = 0; i < MAX_NUM_CAMERA; i++)
	{
		m_hWndHandle[i] = NULL;
		m_pBitmap[i] = NULL;
	} 

}     

void CGxTransferDelayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxTransferDelayDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxTransferDelayDlg, CDialog)
	//{{AFX_MSG_MAP(CGxTransferDelayDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxTransferDelayDlg message handlers

BOOL CGxTransferDelayDlg::OnInitDialog()
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

	//��ʼ����
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ��ʼ����Դ
	bool bReturn = InitResources();
	if (!bReturn)
	{
		MessageBox("��ʼ����Դʧ�ܣ�");
		exit(0);
	}

	// ��ʾ��־����ָ��
	m_pLogWnd = GetDlgItem(IDC_EDIT_LOG_WND);
	UpdateUI();

	// ��ȡ��ͼ����ָ��;��
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_ZERO, &m_hWndHandle[0]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_ONE, &m_hWndHandle[1]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_TWO, &m_hWndHandle[2]);
	GetDlgItem(IDC_STATIC_SHOW_IMAGE_THREE, &m_hWndHandle[3]);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxTransferDelayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char chFileName[MAX_PATH] = {'\0'};
        GetModuleFileName(NULL, chFileName, MAX_PATH);
        CFileVersion fv(chFileName);
        CAboutDlg dlgAbout;		
        dlgAbout.m_strProductVersion = _T("Version: ") + fv.GetProductVersion();
        dlgAbout.m_strLegalCopyRight = fv.GetLegalCopyright();
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

void CGxTransferDelayDlg::OnPaint() 
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
HCURSOR CGxTransferDelayDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   �ر�Ӧ�ó�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ���û��ͣ�ɣ���ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// �������ͣ���¼�����Ϊ���ź�
		SetEvent(m_hCam0StopSnapEvent);
		SetEvent(m_hCam1StopSnapEvent);
		SetEvent(m_hCam2StopSnapEvent);
		SetEvent(m_hCam3StopSnapEvent);

		// ѭ��ͣ��
		for (i = 0; i < m_nDevNum; i++)
		{
			// ����ֹͣ�ɼ�����
			emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
			
			// �����¼�����
			emStatus = GXFlushEvent(m_pDeviceHandle[i]);
			m_pBitmap[i]->UnPrepareForShowImg();
		}	
	}

	// ���û�йر��������ر����
	if (m_bIsOpen)
	{
		// ѭ���ر����
		for (i = 0; i < m_nDevNum; i++)
		{
			// ע���ص�����
			emStatus = GXUnregisterCaptureCallback(m_pDeviceHandle[i]);
			
			// ע���ع��¼��ص�����
			emStatus = GXUnregisterFeatureCallbackByString(m_pDeviceHandle[i], "EventExposureEnd", m_pCBHandle[i]);
			
			// �ر����
			emStatus = GXCloseDevice(m_pDeviceHandle[i]);
			m_pDeviceHandle[i] = NULL;

			if (NULL != m_pBitmap[i])
			{
				delete m_pBitmap[i];
				m_pBitmap[i] = NULL;
			}
		}
	}
	
	// �ͷ���Դ
	ReleaseResource();

	// �ر��豸��
	emStatus = GXCloseLib(); 
	
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	uint32_t  nDeviceNum  = 0;
	char      chIndex[10] = {0};
	int       i           = 0;
	uint32_t	  nDSNum   = 0;
	bool      bReturn     = false;

	// ���岢��ʼ���豸�򿪲���
    GX_OPEN_PARAM stOpenParam;
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode   = GX_OPEN_INDEX;
    stOpenParam.pszContent = "";

	// ö���豸����
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);
	if (nDeviceNum <= 0)
	{
		MessageBox("���豸����");
		return;
	}

	// ���֧��4ֻ���
	m_nDevNum = min(nDeviceNum, MAX_NUM_CAMERA);

	// �������е����
	for (i = 0; i < m_nDevNum; i++)
	{
		// �����Ŵ�1��ʼ 
		_itoa(i + 1, chIndex, 10);

		// �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
		if (m_pDeviceHandle[i] != NULL)
		{
			emStatus = GXCloseDevice(m_pDeviceHandle[i]);
			GX_VERIFY(emStatus);
			m_pDeviceHandle[i] = NULL;
		}
		
		// �����
		stOpenParam.pszContent = chIndex;
		emStatus = GXOpenDevice(&stOpenParam, &m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);

		 //��ȡ��ͨ����
		emStatus = GXGetDataStreamNumFromDev(m_pDeviceHandle[i], &nDSNum);
		GX_VERIFY(emStatus);

		if(0 < nDSNum)
		{
			// ��ȡ�����
			emStatus = GXGetDataStreamHandleFromDev(m_pDeviceHandle[i], 1, &m_pStream[i]);
			GX_VERIFY(emStatus);
		}

		 //�����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
		 //�������������Ĳɼ�����,���÷����ο����´��롣
		{
			bool	 bImplementPacketSize = false;
			uint32_t unPacketSize		  = 0;

			// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
			GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
			emStatus = GXGetNodeAccessMode(m_pDeviceHandle[i], "GevSCPSPacketSize", &emAccessMode);
			GX_VERIFY(emStatus);

			if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
				&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
				&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
			{
				bImplementPacketSize = true;
			}
			
			if (bImplementPacketSize)
			{
				// ��ȡ��ǰ���绷�������Ű���ֵ
				emStatus = GXGetOptimalPacketSize(m_pDeviceHandle[i],&unPacketSize);
				GX_VERIFY(emStatus);

				// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
				emStatus = GXSetIntValue(m_pDeviceHandle[i], "GevSCPSPacketSize", unPacketSize);
				GX_VERIFY(emStatus);
			}
		}
		
		// �����������
		emStatus = SetCameraParam(m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);
		
		do
		{
			m_pBitmap[i] = new CGXBitmap(m_pDeviceHandle[i], &m_hWndHandle[i]);
			if (NULL == m_pBitmap[i])
			{
				ShowErrorString(GX_STATUS_ERROR);
				break;
			}

			// ע��ص�����
			emStatus = RegisterCallback(i);
			GX_VERIFY_BREAK(emStatus);

		}while(false);
		
		if((GX_STATUS_SUCCESS != emStatus)
			&& NULL != m_pBitmap[i])
		{
			delete m_pBitmap[i];
			m_pBitmap[i] = NULL;
			return;
		}
	}	

	m_bIsOpen = true;
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ע��ص�����
\param   nDevNum   ע���������

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxTransferDelayDlg::RegisterCallback(int nDevNum)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ע��ɼ��ص��������ع�����¼��ص�����
 	switch(nDevNum)
 	{
 	case CAMERA0:
 		// ע���ع�����¼��ص�����
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA0], this, OnFeatureCallbackFun0, 
		                                     "EventExposureEnd", &m_pCBHandle[CAMERA0]);
		VERIFY_STATUS_RET(emStatus); 
		
		// ע��ɼ��ص�����
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA0], this, OnFrameCallbackFun0);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA1:
		// ע���ع�����¼��ص�����
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA1], this, OnFeatureCallbackFun1, 
			                                 "EventExposureEnd", &m_pCBHandle[CAMERA1]);
		VERIFY_STATUS_RET(emStatus);
		
		// ע��ɼ��ص�����
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA1], this, OnFrameCallbackFun1);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA2:
		// ע���ع�����¼��ص�����
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA2], this, OnFeatureCallbackFun2, 
		                           	         "EventExposureEnd", &m_pCBHandle[CAMERA2]);
		VERIFY_STATUS_RET(emStatus);
		
		// ע��ɼ��ص�����
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA2], this, OnFrameCallbackFun2);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	case CAMERA3:
		// ע���ع�����¼��ص�����
		emStatus = GXRegisterFeatureCallbackByString(m_pDeviceHandle[CAMERA3], this, OnFeatureCallbackFun3, 
			                                 "EventExposureEnd", &m_pCBHandle[CAMERA3]);
		VERIFY_STATUS_RET(emStatus);
		
		// ע��ɼ��ص�����
		emStatus = GXRegisterCaptureCallback(m_pDeviceHandle[CAMERA3], this, OnFrameCallbackFun3);
		VERIFY_STATUS_RET(emStatus);
		break;
		
	default:
		break;
 	}
	return emStatus;
}
//---------------------------------------------------------------------------------
/**
\brief   �����������
\param   hDevice   ������ 

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxTransferDelayDlg::SetCameraParam(GX_DEV_HANDLE hDevice)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ�����ã���8λͼ�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8Ϊ��:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");

	// ���òɼ�ģʽΪ�����ɼ�
	emStatus = GXSetEnumValueByString(hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);
	
	// ���ô���ģʽΪ��
	emStatus = GXSetEnumValueByString(hDevice, "TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������
	emStatus = GXSetEnumValueByString(hDevice, "TriggerSelector", "FrameStart");
	VERIFY_STATUS_RET(emStatus);
	
	// ѡ�񴥷�Դ
	emStatus = GXSetEnumValueByString(hDevice, "TriggerSource", "Line0");
	VERIFY_STATUS_RET(emStatus);
	
	// ѡ���ع�����¼�
	emStatus = GXSetEnumValueByString(hDevice, "EventSelector", "ExposureEnd");
	VERIFY_STATUS_RET(emStatus);
	
	// �ع�����¼�ʹ��
	emStatus = GXSetEnumValueByString(hDevice, "EventNotification", "On");
	VERIFY_STATUS_RET(emStatus);

	// ���ô������ģʽ
	emStatus = GXSetEnumValueByString(hDevice, "TransferControlMode", "UserControlled");
	VERIFY_STATUS_RET(emStatus);

	// ���ô������ģʽ
	emStatus = GXSetEnumValueByString(hDevice, "TransferOperationMode", "MultiBlock");
	VERIFY_STATUS_RET(emStatus);

	// ���ô���֡��
	emStatus = GXSetIntValue(hDevice, "TransferBlockCount", TRANSFER_COUNT);
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ��m_nDevNum��������в���
	for (i = 0; i < m_nDevNum; i++)
	{
		emStatus = m_pBitmap[i]->PrepareForShowImg();
		GX_VERIFY(emStatus);

		//��������Buffer����ģʽ
		emStatus = GXSetEnumValueByString(m_pStream[i], "StreamBufferHandlingMode", "OldestFirst");
		GX_VERIFY(emStatus);

		// ����ʼ�ɼ�����
		emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStart");
		GX_VERIFY(emStatus);
	}

	// ��0��������������¼�����Ϊ���źţ������������Ϊ���ź�
	SetEvent(m_hCam0StartTranEvent);
	ResetEvent(m_hCam1StartTranEvent);
	ResetEvent(m_hCam2StartTranEvent);
	ResetEvent(m_hCam3StartTranEvent);


	// �������ͣ���¼��ź�����Ϊ���ź�
	ResetEvent(m_hCam0StopSnapEvent);
	ResetEvent(m_hCam1StopSnapEvent);
	ResetEvent(m_hCam2StopSnapEvent);
	ResetEvent(m_hCam3StopSnapEvent);

	m_bIsSnap = true;	
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief    0������ع�����¼��ص�����
\param    emEventID		�¼�����ID��
\param	  pUserParam    �û�˽�в���

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun0(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam0StopSnapEvent, pDlg->m_hCam0StartTranEvent};

	// �ж��Ƿ����ع�����¼�
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("0����������ع�����¼�ID����ȷ"));
		return;
	}

	// �¼��ж�
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM0_WAIT_TIME))
	{
	// �ȴ���ʱ
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("0������ȴ���ʱ����ֹͣ�ɼ������¿���"));
		break;

	// ������ʱ�¼�
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA0], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// ͣ���¼�
	case WAIT_OBJECT_0:
		break;

	// ����
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    1������ع�����¼��ص�����
\param    emEventID		�¼�����ID��
\param	  pUserParam    �û�˽�в���

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun1(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam1StopSnapEvent, pDlg->m_hCam1StartTranEvent};
	
	// �ж��Ƿ����ع�����¼�
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("1����������ع�����¼�ID����ȷ"));
		return;
	}

	// �¼��ж�
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM1_WAIT_TIME))
	{
	// �ȴ���ʱ
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("1������ȴ���ʱ����ֹͣ�ɼ������¿���"));
		break;

	// ������ʱ�¼�
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA1], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// ͣ���¼�
	case WAIT_OBJECT_0:
		break;

	// ����
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    2������ع�����¼��ص�����
\param    emEventID		�¼�����ID��
\param	  pUserParam    �û�˽�в���

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun2(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam2StopSnapEvent, pDlg->m_hCam2StartTranEvent};

	// �ж��Ƿ����ع�����¼�
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("2����������ع�����¼�ID����ȷ"));
		return;
	}

	// �¼��ж�
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM2_WAIT_TIME))
	{
	// �ȴ���ʱ
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("2������ȴ���ʱ����ֹͣ�ɼ������¿���"));
		break;

	// ������ʱ�¼�
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA2], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// ͣ���¼�
	case WAIT_OBJECT_0:
		break;

	// ����
	default:	
		break;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    3������ع�����¼��ص�����
\param    emEventID		�¼�����ID��
\param	  pUserParam    �û�˽�в���

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFeatureCallbackFun3(const char* chFeature, void* pUserParam)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)pUserParam;
	HANDLE hEevents[2] = {pDlg->m_hCam3StopSnapEvent, pDlg->m_hCam3StartTranEvent};

	// �ж��Ƿ����ع�����¼�
	if (0 != strcmp(chFeature, "EventExposureEnd"))
	{
		pDlg->PrintInfotoWnd(MsgComposer("3����������ع�����¼�ID����ȷ"));
		return;
	}

	// �¼��ж�
	switch(::WaitForMultipleObjects(2, hEevents, FALSE, CAM3_WAIT_TIME))
	{
	// �ȴ���ʱ
	case WAIT_TIMEOUT:
		pDlg->PrintInfotoWnd(MsgComposer("3������ȴ���ʱ����ֹͣ�ɼ������¿���"));
		break;

	// ������ʱ�¼�
	case WAIT_OBJECT_0 + 1:
		emStatus = GXSetCommandValue(pDlg->m_pDeviceHandle[CAMERA3], "TransferStart");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			pDlg->ShowErrorString(emStatus);
		}
		break;

	// ͣ���¼�
	case WAIT_OBJECT_0:
		break;

	// ����
	default:	
		break;
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����
\param   pFrame  �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun0(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("0��������ֲ�֡"));
	}

	// ����1�����
	SetEvent(pDlg->m_hCam1StartTranEvent);

	int		    nID       = 0;             // ID��

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����
\param   pFrame  �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("1��������ֲ�֡"));
	}

	// ����2�����
	SetEvent(pDlg->m_hCam2StartTranEvent);
	
	int		    nID       = 1;             // ID��	

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����
\param   pFrame  �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("2��������ֲ�֡"));
	}

	// ����3�����
	SetEvent(pDlg->m_hCam3StartTranEvent);
		
	int		    nID       = 2;             // ID��	

	pDlg->m_pBitmap[nID]->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����
\param   pFrame  �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxTransferDelayDlg::OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxTransferDelayDlg *pDlg = (CGxTransferDelayDlg*)(pFrame->pUserParam);

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		pDlg->PrintInfotoWnd(MsgComposer("3��������ֲ�֡"));
	}

	// ����0�����
	SetEvent(pDlg->m_hCam0StartTranEvent);
	
	int		    nID       = 3;             // ID��	

	pDlg->m_pBitmap[nID]->Show(pFrame);

}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �������ͣ���¼�����Ϊ���ź�
	SetEvent(m_hCam0StopSnapEvent);
	SetEvent(m_hCam1StopSnapEvent);
	SetEvent(m_hCam2StopSnapEvent);
	SetEvent(m_hCam3StopSnapEvent);

	// ѭ��ͣ��
	for (i = 0; i < m_nDevNum; i++)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
		GX_VERIFY(emStatus);

		// �����¼�����
		emStatus = GXFlushEvent(m_pDeviceHandle[i]);
		GX_VERIFY(emStatus);
	}

	m_bIsSnap = false;
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	int i = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ���û��ͣ�ɣ���ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// �������ͣ���¼�����Ϊ���ź�
		SetEvent(m_hCam0StopSnapEvent);
		SetEvent(m_hCam1StopSnapEvent);
		SetEvent(m_hCam2StopSnapEvent);
		SetEvent(m_hCam3StopSnapEvent);

		// ѭ��ͣ��
		for (i = 0; i < m_nDevNum; i++)
		{
			// ����ֹͣ�ɼ�����
		    emStatus = GXSetCommandValue(m_pDeviceHandle[i], "AcquisitionStop");
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}

		    // �����¼�����
		    emStatus = GXFlushEvent(m_pDeviceHandle[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}
			m_pBitmap[i]->UnPrepareForShowImg();
		}
	}
	m_bIsSnap = false;

	// ѭ���ر����
	for (i = 0; i < m_nDevNum; i++)
	{
		// ע���ɼ��ص�����
		emStatus = GXUnregisterCaptureCallback(m_pDeviceHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		
		// ע���ع��¼��ص�����
		emStatus = GXUnregisterFeatureCallbackByString(m_pDeviceHandle[i], "EventExposureEnd", m_pCBHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		// �ر����
		emStatus = GXCloseDevice(m_pDeviceHandle[i]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		if (NULL != m_pBitmap[i])
		{
			delete m_pBitmap[i];
			m_pBitmap[i] = NULL;
		}

		m_pDeviceHandle[i] = NULL;
	}
	m_bIsOpen = false;

	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼ����Դ(���֧��4ֻ���)

\return ture����ʾ�ɹ���false����ʾʧ��
*/
//----------------------------------------------------------------------------------
bool CGxTransferDelayDlg::InitResources()
{
	// ������
	m_pDeviceHandle = new GX_DEV_HANDLE[MAX_NUM_CAMERA];
	if (m_pDeviceHandle == NULL)
	{
		return false;
	}

	// �����
	m_pStream = new GX_DEV_HANDLE[MAX_NUM_CAMERA];
	if (m_pStream == NULL)
	{
		return false;
	}

	// �ع�ص��������
	m_pCBHandle = new GX_FEATURE_CALLBACK_HANDLE[MAX_NUM_CAMERA];
	if (m_pCBHandle == NULL)
	{
		ReleaseResource();
		return false;
	}

	// ����0��������������¼�
	m_hCam0StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer0StartTranEvent");
	if (m_hCam0StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
   	
	// ����1��������������¼�
	m_hCam1StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer1StartTranEvent");
	if (m_hCam1StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
	
	// ����2��������������¼�
	m_hCam2StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer2StartTranEvent");
	if (m_hCam2StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}
	
	// ����3��������������¼�
	m_hCam3StartTranEvent = CreateEvent(NULL, FALSE, FALSE, "Camer3StartTranEvent");
	if (m_hCam3StartTranEvent == NULL)
	{
		ReleaseResource();
		return false;
	}	

	// ����0�����ֹͣ�ɼ��¼�
	m_hCam0StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer0StopSnapEvent");
	if (m_hCam0StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// ����1�����ֹͣ�ɼ��¼�
	m_hCam1StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer1StopSnapEvent");
	if (m_hCam1StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// ����2�����ֹͣ���¼�
	m_hCam2StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer2StopSnapEvent");
	if (m_hCam2StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}

	// ����3�����ֹͣ�ɼ��¼�
	m_hCam3StopSnapEvent = CreateEvent(NULL, TRUE, FALSE, "Camer3StopSnapEvent");
	if (m_hCam3StopSnapEvent == NULL)
	{
		ReleaseResource();
		return false;
	}		
	
	// ��ʼ����Դ��Ϣ
	for (int i = 0; i < MAX_NUM_CAMERA; i++)
	{
		m_pDeviceHandle[i]             = NULL;
	    m_pStream[i]				   = NULL;
		m_pCBHandle[i]                 = NULL;
	}

	return true;
}

//----------------------------------------------------------------------------------
/**
\brief  �ͷ���Դ

\return void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::ReleaseResource()
{
	// �ͷ��豸���ָ��
	if(m_pDeviceHandle != NULL)
	{
		delete[]m_pDeviceHandle;
		m_pDeviceHandle = NULL;
	}

	// �ͷ������ָ��
	if (m_pStream != NULL)
	{
		delete[]m_pStream;
		m_pStream = NULL;
	}

	// �ͷŻص����ָ��
	if (m_pCBHandle != NULL)
	{
		delete[]m_pCBHandle;
		m_pCBHandle = NULL;
	}

	// �رմ����ӳ��¼����
    if (m_hCam0StartTranEvent != NULL)
    {
        CloseHandle(m_hCam0StartTranEvent);
        m_hCam0StartTranEvent = NULL;
    }
	if (m_hCam1StartTranEvent != NULL)
    {
        CloseHandle(m_hCam1StartTranEvent);
        m_hCam1StartTranEvent = NULL;
    }
	if (m_hCam2StartTranEvent != NULL)
    {
        CloseHandle(m_hCam2StartTranEvent);
        m_hCam2StartTranEvent = NULL;
    }
	if (m_hCam3StartTranEvent != NULL)
    {
        CloseHandle(m_hCam3StartTranEvent);
        m_hCam3StartTranEvent = NULL;
    }

	// �ر�ֹͣ�ɼ��¼����
	if (m_hCam0StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam0StopSnapEvent);
		m_hCam0StopSnapEvent = NULL;
	}
	if (m_hCam1StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam1StopSnapEvent);
		m_hCam1StopSnapEvent = NULL;
	}
	if (m_hCam2StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam2StopSnapEvent);
		m_hCam2StopSnapEvent = NULL;
	}
	if (m_hCam3StopSnapEvent != NULL)
	{
		CloseHandle(m_hCam3StopSnapEvent);
		m_hCam3StopSnapEvent = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������Ϣ����
\param  emErrorStatus  ������

\return void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// ��ȡ����������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// ��ȡ������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	
	// ��ʾ����������Ϣ
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}
	
	// �ͷ���Դ
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief   ˢ��UI����

\return  void
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief  ����־��ʾ���ڴ�ӡ��־��Ϣ
\param  strInfo ָ����־��Ϣ��ָ��

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CGxTransferDelayDlg::PrintInfotoWnd(string &strInfo)
{
	SYSTEMTIME system;
	GetLocalTime(&system);
	string strTime = MsgComposer("%02dh:%02dm:%02ds:%03dms ",system.wHour, system.wMinute, system.wSecond, system.wMilliseconds);
	strInfo = strTime.append(strInfo);
	
	if (m_pLogWnd != NULL)// ������ھ����Ϊ��
	{
		CEdit *pEdit = (CEdit*)m_pLogWnd;
		int textLen = pEdit->GetWindowTextLength();
		
		if (pEdit->GetLineCount() > 300)
		{
			pEdit->Clear();
			pEdit->SetWindowText(_T(""));
			textLen = pEdit->GetWindowTextLength();
		}
		
		pEdit->SetSel(textLen, -1);	
		strInfo.append("\r\n");
		pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());
		
		if(textLen > 0)
		{
			pEdit->SetSel((textLen - pEdit->GetLineCount() + 1), -1);
		}
		else
		{
			pEdit->SetSel(textLen, -1);
		}
	}
}

//-----------------------------------------------------------------------
/**
\brief   �ַ�������Ҫ�����ع�������Edit����Ӧ�س�������Ϣ
\param   pMsg  ��Ϣ�ṹ��

\return  �ɹ�:TRUE   ʧ��:FALSE
*/
//-----------------------------------------------------------------------
BOOL CGxTransferDelayDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))   
	{   
		return  TRUE; 
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}