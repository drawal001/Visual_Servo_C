// GxCameraEventsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxCameraEvents.h"
#include "GxCameraEventsDlg.h"

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
// CGxCameraEventsDlg dialog

CGxCameraEventsDlg::CGxCameraEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxCameraEventsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxCameraEventsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDevice                     = NULL;            // �豸���
	m_hStream                     = NULL;			 // �豸�����
	m_pLogWnd                     = NULL;            // ָ���¼ʱ�䴰��
	m_bIsOpen                     = false;           // ����򿪱�ʶ
	m_bIsSnap                     = false;           // ����ɼ���ʶ
	m_bImplementEventSelector     = false;           // ����Ƿ�֧���¼�Դѡ��
	m_bSupportExposureEndEvent    = false;           // �Ƿ�֧���ع�����¼�
	m_bIsTrigValid                = true;            // �����Ƿ���Ч��־
}

void CGxCameraEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxCameraEventsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxCameraEventsDlg, CDialog)
	//{{AFX_MSG_MAP(CGxCameraEventsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_BN_CLICKED(IDC_SEND_SOFT_COMMAND, OnSendSoftCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxCameraEventsDlg message handlers

BOOL CGxCameraEventsDlg::OnInitDialog()
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
	// ��ʼ����
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ָ��ʱ����ʾ����ָ��
	m_pLogWnd = GetDlgItem(IDC_EDIT_TIME_LOG); 

	// ����UI����
	UpdateUI();	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxCameraEventsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxCameraEventsDlg::OnPaint() 
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
HCURSOR CGxCameraEventsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGxCameraEventsDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// δͣ����ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// ע���ɼ��ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		// ע���ع�����¼��ص�����
 		emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);			
 		m_bIsSnap = false;
	}
	
	if (m_bIsOpen)
	{
		// �ر����
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;
	}		
	
	// �ر��豸��
	emStatus = GXCloseLib();
	
	CDialog::OnClose();
}

void CGxCameraEventsDlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus     = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum   = 0;
	int64_t        nPixelFormat = 0;        
	GX_OPEN_PARAM  stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// ö���豸����
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);
	
	// �жϵ�ǰ�����豸����
	if (nDeviceNum <= 0)
	{
		MessageBox("δ�����豸!");
		return;
	}

	// �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}
	
	// ��ö���б��еĵ�һ̨�豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}

	// �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
	// �������������Ĳɼ�����,���÷����ο����´��롣
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
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
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			GX_VERIFY(emStatus);

			// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}
	
	// ��ʼ������
	emStatus = InitParam();
	GX_VERIFY(emStatus);

	// �ж��Ƿ�֧���ع�����¼�
	if (!m_bSupportExposureEndEvent)
	{
		return;
	}

	m_bIsOpen = true;	
	
	// ����UI����
	UpdateUI();	
}

void CGxCameraEventsDlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ע���ع�����¼��ص�����
	emStatus = GXRegisterFeatureCallbackByString(m_hDevice, this, OnFeatureCallbackFun, "EventExposureEnd", &m_hCB);
	GX_VERIFY(emStatus);
		
	// ע��ɼ��ص�����
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	GX_VERIFY(emStatus);

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// ����ʼ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	GX_VERIFY(emStatus);
	m_bIsSnap = true;
	m_bIsTrigValid = true;
	
	// ����UI����
	UpdateUI();	
}

void CGxCameraEventsDlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// ע���ɼ��ص�����
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	// ע���ع�����¼��ص�����
 	emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);
 	GX_VERIFY(emStatus);
	m_bIsSnap = false;
	
	// ����UI����
	UpdateUI();
}

void CGxCameraEventsDlg::OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// δͣ��ʱ��ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		
		// ע���ɼ��ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		// ע���ع�����¼��ص�����
		emStatus = GXUnregisterFeatureCallbackByString(m_hDevice, "EventExposureEnd", m_hCB);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		m_bIsSnap = false;
	}
	
	// �ر����
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}
	m_hDevice = NULL;
	m_bIsOpen = false;
	
	// ����UI����
	UpdateUI();	
}

void CGxCameraEventsDlg::OnSendSoftCommand() 
{
	// TODO: Add your control notification handler code here
	// ����������ص�����ִ�еĹ����У��ٴε�����������󣬴˴ε���ᱻ���ε�
	if (!m_bIsTrigValid)
	{
		return;
	}
	else
	{
		m_bIsTrigValid = false;
	}

	SYSTEMTIME system;
	CString    strTime = "";
	GX_STATUS  emStatus = GX_STATUS_SUCCESS;

	// ��ʼ�������������ʱ��(�˴������ŵ�������ʱ��ʼ֮ǰ�����´�ӡʱ����ڽ���ص�ʱ�䣬��������)
	GetLocalTime(&system);
	strTime.Format("%02dh:%02dm:%02ds:%03dms ", system.wHour, system.wMinute, system.wSecond, system.wMilliseconds);
	PrintInfotoWnd(MsgComposer("\r\n��ʼ�������������ʱ��㣺" + strTime));

	// ���յ�ͼ����ʱʱ�䣺��ʱ��ʼ
	m_objImageTime.Start();

	// ���ع�����¼���ʱʱ�䣺��ʱ��ʼ
	m_objShutterTime.Start();

	// ������������
	emStatus = GXSetCommandValue(m_hDevice,"TriggerSoftware");
	if(emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus); 
		m_bIsTrigValid = true;
		return;
	} 
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxCameraEventsDlg::InitParam()
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;

	// ���òɼ�ģʽΪ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô���ģʽΪ��
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// ѡ�񴥷�ԴΪ����
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerSource", "Software");
	VERIFY_STATUS_RET(emStatus);

	// �ж��Ƿ�֧���¼���ع���
	emStatus = __IsSupportEvent(m_bSupportExposureEndEvent);
	if (!m_bSupportExposureEndEvent)
	{
		MessageBox("�����֧���ع�����¼�����ʾ����Ը������Ч!");
		return emStatus;
	}
	
	// ѡ���ع�����¼�
	emStatus = GXSetEnumValueByString(m_hDevice, "EventSelector", "ExposureEnd");
	VERIFY_STATUS_RET(emStatus);
	
	// �ع�����¼�ʹ��
	emStatus = GXSetEnumValueByString(m_hDevice, "EventNotification", "On");
	VERIFY_STATUS_RET(emStatus);
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����
\param   pFrame   �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxCameraEventsDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxCameraEventsDlg *pDlg = (CGxCameraEventsDlg*)(pFrame->pUserParam);
	double dElapsedtime = 0;
	char   chTemp[8]    = "ms";

	// ������ʱ
	dElapsedtime = pDlg->m_objImageTime.Stop();
	pDlg->PrintInfotoWnd(MsgComposer("��ʼ������������ɹ��յ�ͼ����ʱ��%0.2f %s", dElapsedtime, chTemp));

	pDlg->m_bIsTrigValid = true;
}

//---------------------------------------------------------------------------------
/**
\brief    �ع��¼��ص�����
\param    emEventID		�¼�����ID��
\param	  pUserParam    �û�˽�в���

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxCameraEventsDlg::OnFeatureCallbackFun(const char* chFeature, void* pUserParam)
{
	CGxCameraEventsDlg *pDlg = (CGxCameraEventsDlg*)pUserParam;
    double dElapsedtime = 0;
	char   chTemp[8]    = "ms";

	if (chFeature == "EventExposureEnd")
	{
		// ������ʱ
		dElapsedtime = pDlg->m_objShutterTime.Stop();
		pDlg->PrintInfotoWnd(MsgComposer("��ʼ������������ع�����¼���ʱ��%0.2f %s", dElapsedtime, chTemp));
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʱ����ʾ���ڴ�ӡʱ����Ϣ
\param  strInfo �����ӡ����Ϣ

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::PrintInfotoWnd(string &strInfo)
{
	int    nTextLen = 0;
	CEdit  *pEdit = (CEdit*)m_pLogWnd;

	// ������ھ����Ϊ��
	if (m_pLogWnd != NULL) 
	{
		nTextLen = pEdit->GetWindowTextLength();	
		if (pEdit->GetLineCount() > 300)
		{
			pEdit->Clear();
			pEdit->SetWindowText(_T(""));
			nTextLen = pEdit->GetWindowTextLength();
		}
		
		pEdit->SetSel(nTextLen, -1);	
		strInfo.append("\r\n");
		pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());
		
		if(nTextLen > 0)
		{
			pEdit->SetSel((nTextLen - pEdit->GetLineCount() + 1), -1);
		}
		else
		{
			pEdit->SetSel(nTextLen, -1);
		}
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������Ϣ����
\param  emErrorStatus  ������

 \return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::ShowErrorString(GX_STATUS emErrorStatus)
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
void CGxCameraEventsDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//---------------------------------------------------------------------------------
/**
\brief   ��Ϣ���ɺ���(��Ҫ����س���Ϣ)
\param   pMsg  ��Ϣ�ṹ��

\return  BOOL
*/
//----------------------------------------------------------------------------------
BOOL CGxCameraEventsDlg::PreTranslateMessage(MSG* pMsg) 
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

//---------------------------------------------------------------------------------
/**
\brief   �ж��Ƿ�֧���¼���ع���
\param   bSupportExposureEndEvent [out]  �Ƿ�֧���ع�����¼���־

\return  �ɹ�: GX_STATUS_SUCCESS, ʧ��: ��GX_STATUS_SUCCESS
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxCameraEventsDlg::__IsSupportEvent(bool &bSupportExposureEndEvent)
{
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;

	// �ж��豸�Ƿ�֧���¼�Դѡ��
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "EventSelector", &emAccessMode);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return emStatus;
	}

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bImplementEventSelector = true;
	}


	if (m_bImplementEventSelector)
	{
		GX_ENUM_VALUE stEnumValue;
		size_t    nbufferSize    = 0;
		uint32_t  nEntryNum      = 0;
		int64_t   nEnumValue     = -1;
		int       nCurcel        = 0;
		
		// ��ȡ���ܵ�ö����
		emStatus = GXGetEnumValue(m_hDevice, "EventSelector", &stEnumValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			return emStatus;
		}

		nEntryNum = stEnumValue.nSupportedNum;
		
		// ������ѯ�Ƿ�֧���ع�����¼�
		for (uint32_t i = 0; i < nEntryNum; i++)
		{	
			if (stEnumValue.nArrySupportedValue[i].nCurValue == GX_ENUM_EVENT_SELECTOR_EXPOSUREEND)
			{
				bSupportExposureEndEvent = true;
				break;
			}
		}
	}

	return emStatus;
}