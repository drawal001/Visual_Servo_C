// GxGetImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxGetImage.h"
#include "GxGetImageDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
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
	CString	m_strLegalCopyRight;
	CString	m_strProductVersion;
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
	m_strLegalCopyRight = _T("");
	m_strProductVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg dialog

CGxGetImageDlg::CGxGetImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxGetImageDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
{
	//{{AFX_DATA_INIT(CGxGetImageDlg)
	m_nEditTimeOut = 500;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    // ��ʼ������
	memset(&m_stFrameData,0,sizeof(GX_FRAME_DATA)); 

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxGetImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxGetImageDlg)
	DDX_Text(pDX, IDC_EDIT_TIME_OUT, m_nEditTimeOut);
	DDV_MinMaxUInt(pDX, m_nEditTimeOut, 0, 3600000);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxGetImageDlg, CDialog)
	//{{AFX_MSG_MAP(CGxGetImageDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_START_SNAP,OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP,OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE,OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE,OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_SEND_SOFT_COMMAND,OnBnClickedBtnSendSoftCommand)
	ON_BN_CLICKED(IDC_BTN_CLEAN_TIME,OnBnClickedBtnCleanTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxGetImageDlg message handlers
BOOL CGxGetImageDlg::OnInitDialog()
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
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ��ʼ���豸��
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	//��ȡͼ����ʾ���ڵ�ָ��ͻ�ͼDC���
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);

	//ˢ�½���
	UpdateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxGetImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char strFileName[MAX_PATH] = {'\0'};
        GetModuleFileName(NULL, strFileName, MAX_PATH);
        CFileVersion fv(strFileName);
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
void CGxGetImageDlg::OnPaint() 
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
HCURSOR CGxGetImageDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//----------------------------------------------------------------------------------
/**
\brief  ���豸

\return ���豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::OpenDevice()
{
	GX_STATUS      emStatus    = GX_STATUS_SUCCESS;	
	GX_OPEN_PARAM  stOpenParam;
	uint32_t	  nDSNum	  = 0;

	// ��ʼ���豸�򿪲���,Ĭ�ϴ����Ϊ1���豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	// ��ö���б��еĵ�һ̨�豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	VERIFY_STATUS_RET(emStatus)

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	VERIFY_STATUS_RET(emStatus)

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		VERIFY_STATUS_RET(emStatus)
	}

	return GX_STATUS_SUCCESS;
}

//----------------------------------------------------------------------------------
/**
\brief  �ر��豸

\return �ر��豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::CloseDevice()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// �ͷ���Դ
    UnPrepareForShowImg();
	
	// �ر��豸
	emStatus = GXCloseDevice(m_hDevice);
		
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ����ͼ�����Buffer,Ϊͼ����ʾ׼����Դ

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  CGxGetImageDlg::PrepareForShowImg()
{	
	bool bRet = true;
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// �õ�ͼ���payloadSize
	uint32_t ui32PayLoadSize = 0;
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	//ΪGetImage��������ռ�
	m_stFrameData.pImgBuf     = new(std::nothrow) BYTE[ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		bRet = false;
	}

	emStatus = m_pBitmap->PrepareForShowImg();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		bRet = false;
	}
	// ������ʧ�����ͷ��ѷ������Դ
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  �ͷ�Ϊͼ����ʾ׼����Դ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UnPrepareForShowImg()
{
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete []m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}
	if (NULL != m_pBitmap) 
	{
		m_pBitmap->UnPrepareForShowImg();
	}

}

// ---------------------------------------------------------------------------------
/**
\brief   �����ʼ��

\return  ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxGetImageDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);
	
	//���ô���ģʽΪ��
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "On");
	VERIFY_STATUS_RET(emStatus);

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnumValueByString(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);
 
	//ѡ�񴥷�ԴΪ����
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerSource", "Software");

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   ˢ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UpdateUI()
{
	GetDlgItem(IDC_EDIT_TIME_OUT)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_BTN_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(m_bIsOpen && !m_bIsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(m_bIsSnap);
    GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief  ��Ϣ���ɺ���

\return BOOL
*/
//----------------------------------------------------------------------------------
BOOL CGxGetImageDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd  *pWnd     = NULL;
	int   nCtrlID   = 0;
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		pWnd = GetFocus();
		nCtrlID = pWnd->GetDlgCtrlID();
		switch(nCtrlID)
		{
		case IDC_EDIT_TIME_OUT:
				::SetFocus(NULL);
				break;
		default:
				break;
		}
			
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))   
	{   
		return  TRUE; 
	}
			
	return CDialog::PreTranslateMessage(pMsg);
}

//----------------------------------------------------------------------------------
/**
\brief  �ر�Ӧ�ó�����Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (m_bIsSnap) 
	{
		m_pBitmap->UnPrepareForShowImg();
	}

	// δ�ر��豸��ر�
	if (m_bIsOpen)
	{
		emStatus = CloseDevice();
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
	// �ͷ��豸��
	emStatus = GXCloseLib();

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  status  ������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// �ͷ�������ڴ�ռ�
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//Ϊͼ����ʾ׼����Դ
	bool bRet = PrepareForShowImg();
	if (!bRet)
	{
		MessageBox("Ϊͼ����ʾ׼����Դʧ��!");
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// ʹ��GxGetImage��ȡͼ��,ֻ��Ҫ���Ϳ�������
	// ���Ϳ�������
	emStatus = GXSetCommandValue(m_hDevice,"AcquisitionStart");
	GX_VERIFY(emStatus);

	// ���¿��ɱ�ʶ
	m_bIsSnap = true;

	// ���½���
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ�ɼ�"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//ֹͣ�ɼ�
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);

	// �ͷ���Դ
	UnPrepareForShowImg();

	//���¿��ɱ�ʶΪfalse
	m_bIsSnap = false;

	//��λͳ��ʱ������
	m_objStatistic.Reset();

	//���½���
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bRet     = true;
	uint32_t  nDevNum  = 0;

	// ö���豸
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		MessageBox("δ�����豸!");
		return;
	}

	// ����豸�Ѿ�����رգ���֤����ڳ�ʼ��������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	//�����
	emStatus = OpenDevice();
	GX_VERIFY(emStatus);

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

	do
	{
		//��ʼ��ͼ����ʾģ��
		m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		//�����豸��Ĭ�ϲ������ɼ�ģʽ:�����ɼ�,��������:��,����Դ:����
		emStatus = InitDevice();
		GX_VERIFY_BREAK(emStatus);

	}while(false);
	
	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	// �����豸�򿪱�ʶ
	m_bIsOpen = true;

	//ˢ�½���
	UpdateUI();

	UpdateData(FALSE);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ���δͣ������ֹͣ�ɼ�
	if(m_bIsSnap)
	{
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
        if (emStatus != GX_STATUS_SUCCESS)
        {
			// ������
        }
		m_bIsSnap = false;
	}

	//�ر����
	emStatus = CloseDevice();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	// �ͷ���Դ
	UnPrepareForShowImg();


	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_bIsOpen = false;
	m_hDevice = NULL;

	//ˢ�½���
	UpdateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"������ͼ"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnSendSoftCommand()
{
	// TODO: Add your control notification handler code here
	if (!UpdateData(TRUE))
	{
		m_nEditTimeOut = 500;
		return;
	}

	GX_STATUS emStatus  = GX_STATUS_ERROR;
	double dElapsedtime = 0;

	//ÿ�η��ʹ�������֮ǰ��ղɼ��������
	//��ֹ���ڲ�����֡����ɱ���GXGetImage�õ���ͼ�����ϴη��ʹ����õ���ͼ
	emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);

	//������������
	emStatus = GXSetCommandValue(m_hDevice,"TriggerSoftware");
	GX_VERIFY(emStatus);

	//��ʱ��ʼ
	m_objStopTime.Start();

	//��ȡͼ��
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, m_nEditTimeOut);

	//������ʱ
	dElapsedtime = m_objStopTime.Stop();

	//�ж�GXGetImage����ֵ
	GX_VERIFY(emStatus);

	//��ʾͼ��
	m_pBitmap->Show(&m_stFrameData);

	//���½����ʱ��ͳ������
	UpdateStatisticalData(dElapsedtime);
}

//----------------------------------------------------------------------------------
/**
\brief  ���½����ͳ������
\param  dData  [in]   ʱ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::UpdateStatisticalData(double dData)
{
	CString strTemp = _T("");

	m_objStatistic.AddStatisticalData(dData);

	// ��ȡƽ��ֵ����ʾ
    strTemp.Format("%.3f",m_objStatistic.GetAverage());
	SetDlgItemText(IDC_EDIT_AVERAGE,strTemp);

    // ��ȡ���ֵ����ʾ
	strTemp.Format("%.3f",m_objStatistic.GetMax());
	SetDlgItemText(IDC_EDIT_MAX_VALUE,strTemp);

	// ��ȡ��Сֵ����ʾ
	strTemp.Format("%.3f",m_objStatistic.GetMin());
	SetDlgItemText(IDC_EDIT_MIN_VALUE,strTemp);
}

//----------------------------------------------------------------------------------
/**
\brief  �����ʾʱ���"���"��ť��Ϣ��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxGetImageDlg::OnBnClickedBtnCleanTime()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TIME_WND,"");
}
