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
, m_sBlockDataReadOut(_T(""))
, m_sBlockDataWrite(_T(""))
, m_sSerialNumber1(_T(""))
, m_sSerialNumber2(_T(""))
, m_sSerialNumber3(_T(""))
, m_sCameraName(_T(""))
, m_sCameraNameDesc(_T(""))
, m_sSerialNumberDesc(_T(""))
, m_sBlockDataStaticBox(_T(""))
{
	//{{AFX_DATA_INIT(CBasicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera = -1;

}

void CBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_BLOCK_DATA_READOUT, m_sBlockDataReadOut);
	DDX_Text(pDX, IDC_EDIT_BLOCK_DATA_WRITE, m_sBlockDataWrite);
	DDX_Text(pDX, IDC_EDIT_SN1, m_sSerialNumber1);
	DDX_Text(pDX, IDC_EDIT_SN2, m_sSerialNumber2);
	DDX_Text(pDX, IDC_EDIT_SN3, m_sSerialNumber3);
	DDX_Text(pDX, IDC_EDIT_CAMERA_NAME, m_sCameraName);
	DDX_Text(pDX, IDC_EDIT_NAME_DESC, m_sCameraNameDesc);
	DDX_Text(pDX, IDC_EDIT_SN_DESC, m_sSerialNumberDesc);
	DDX_Text(pDX, IDC_STATIC_BLOCK_DATA, m_sBlockDataStaticBox);
}

BEGIN_MESSAGE_MAP(CBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NAME_READ, OnBnClickedButtonNameRead)
	ON_BN_CLICKED(IDC_BUTTON_NAME_WRITE, OnBnClickedButtonNameWrite)
	ON_BN_CLICKED(IDC_BUTTON_SN_READ2, OnBnClickedButtonSnRead2)
	ON_BN_CLICKED(IDC_BUTTON_SN_WRITE2, OnBnClickedButtonSnWrite2)
	ON_BN_CLICKED(IDC_BUTTON_SN_READ3, OnBnClickedButtonSnRead3)
	ON_BN_CLICKED(IDC_BUTTON_SN_WRITE3, OnBnClickedButtonSnWrite3)
	ON_BN_CLICKED(IDC_BUTTON_DATA_WRITE_EDIT, OnBnClickedButtonDataWriteEdit)
	ON_BN_CLICKED(IDC_BUTTON_DATA_WRITE_ZERO, OnBnClickedButtonDataWriteZero)
	ON_BN_CLICKED(IDC_BUTTON_DATA_WRITE_0XFF, OnBnClickedButtonDataWrite0xff)
	ON_BN_CLICKED(IDC_BUTTON_DATA_WRITE_RANDOM, OnBnClickedButtonDataWriteRandom)
	ON_BN_CLICKED(IDC_BUTTON_DATA_WRITE_INC, OnBnClickedButtonDataWriteInc)
	ON_BN_CLICKED(IDC_BUTTON_DATA_READ, OnBnClickedButtonDataRead)
	ON_BN_CLICKED(IDC_BUTTON_SN_READ1, OnBnClickedButtonSnRead1)
	ON_BN_CLICKED(IDC_BUTTON_SN_WRITE1, &CBasicDlg::OnBnClickedButtonSnWrite1)
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




BOOL CBasicDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility tCapability;
	
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

	SDK_TRACE(CameraGetCapability(m_hCamera,&tCapability),gLanguage?"��������������":"Get the capability of the camera");

	m_sBlockDataStaticBox.Format("�Զ������ݿ��д����,������û�����������Ϊ[%d]�ֽ�",tCapability.iUserDataMaxLen);
	
	m_iMaxBlockDatalen = tCapability.iUserDataMaxLen;

	m_sCameraNameDesc = "˵���������ͺŵĹ�ҵ�����֧����������Զ��幦��,�ù��ܿɷ���������ֶ������ͬʱ�����Ҳ������Ψһ�����к�,\
Ҳ�����������ֶ�������������ͨ��ר�õ�API�ӿ��޸ĺ�������Ч���͵����Լ�ʹ�õ�����޹أ��޸�������ƺ�����ʹ����̨\
���ԡ������Ǻ�������ӿڷ������������ƶ��������޸Ĺ������ơ�Ĭ������£���������Ǹ����ͺ��Զ����ɵģ������������2̨MV-U130M����ô���ǵ�\
�����Ʒֱ���MV-U130M#0 �� MV-U130M#1�������޸ĺ���Ҫ������ϵ������������Ч��";

	m_sSerialNumberDesc = "˵�������Ԥ��3�����кſɹ����ο������ʡ����е�1�����к��ǳ���ʱ�Ѿ�д�룬�û��޷��޸ģ�Ϊֻ��״̬��2,3�����кſ���ͨ��\
��������ʾ��API�ӿڽ����޸ģ�������Ҳ����ֱ��ʹ�ø�DEMO�������кŸ�д���������кų������Ϊ32���ֽڡ���Ҳ���Խ�2,3�����к�\
�Ĵ洢�ռ������������Զ������ݡ�";

	//������Ҫע����ǣ�CameraSaveUserData���������ڱ����û��Զ�������ݣ����Ƿ��ʵ���ʼ��ַ������32�ֽڶ���ġ����������ɱ���������Ч��
	return TRUE;
}


void CBasicDlg::OnClose() 
{
	//����ʼ�����
	if (m_hCamera > 0)
	{
	
		//����ʼ�������
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

 
	CDialog::OnClose();
}


void CBasicDlg::OnBnClickedButtonNameRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char frName[32];

	SDK_TRACE(CameraGetFriendlyName(m_hCamera,frName),"����Զ����豸��");

	m_sCameraName = frName;
	//��������������ݲ�����Ч���ַ��������ʾ�������δ�Զ�������ƣ���ʾ�������ܾ������롣��ʱ��������ͨ���ͺ����Զ����ɵġ�

	UpdateData(FALSE);
}


void CBasicDlg::OnBnClickedButtonNameWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	//д��Ӣ�� ��auto�� �������� ���Զ��� �����ʾʹ���ͺ��Զ�����������ƣ��������ʱĬ�Ͼ��Ǹ÷�ʽ��

	SDK_TRACE(CameraSetFriendlyName(m_hCamera,m_sCameraName.GetBuffer(32)),"�����Զ����������");

	m_sCameraName.ReleaseBuffer();

}


void CBasicDlg::OnBnClickedButtonSnRead2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sn[32];
	memset(sn,0,32);
	//Ĭ������£�2,3�������к���û�о�����ʼ���ģ������������ݿ��������롣
	SDK_TRACE(CameraReadSN(m_hCamera,(BYTE*)sn,1),"���������к�");
	m_sSerialNumber2 = sn;
	UpdateData(FALSE);
}


void CBasicDlg::OnBnClickedButtonSnWrite2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SDK_TRACE(CameraWriteSN(m_hCamera,(BYTE*)m_sSerialNumber2.GetBuffer(32),1),"д��������к�");
	m_sSerialNumber2.ReleaseBuffer();

}


void CBasicDlg::OnBnClickedButtonSnRead3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sn[32];
	memset(sn,0,32);
	//Ĭ������£�2,3�������к���û�о�����ʼ���ģ������������ݿ��������롣
	SDK_TRACE(CameraReadSN(m_hCamera,(BYTE*)sn,2),"��ȡ�������к�");

	m_sSerialNumber3 = sn;
	UpdateData(FALSE);
}


void CBasicDlg::OnBnClickedButtonSnWrite3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SDK_TRACE(CameraWriteSN(m_hCamera,(BYTE*)m_sSerialNumber3.GetBuffer(32),2),"д���������к�");
	m_sSerialNumber3.ReleaseBuffer();
}


void CBasicDlg::OnBnClickedButtonDataWriteEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString msg;
	BYTE *pReadBuffer;
	BYTE *pWriteBuffer;
	int i;
	int iWriteDataLen = m_sBlockDataWrite.GetLength();

	if (iWriteDataLen > m_iMaxBlockDatalen)
	{
		msg.Format("�������ݹ�����������û�����������Ϊ%d�ֽڣ�������%d�ֽڣ��������ֽڽ����ᱻд���豸�С�",m_iMaxBlockDatalen,iWriteDataLen);
		MessageBox(msg);
		iWriteDataLen = m_iMaxBlockDatalen;
	}
	
	pReadBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	pWriteBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	memcpy(pWriteBuffer,m_sBlockDataWrite.GetBuffer(iWriteDataLen),iWriteDataLen);
	m_sBlockDataWrite.ReleaseBuffer();

	do 
	{
		if (CameraSaveUserData(m_hCamera,0,pWriteBuffer,iWriteDataLen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("д�����ݳ���");
			break;
		}



		if (CameraLoadUserData(m_hCamera,0,pReadBuffer,iWriteDataLen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("��ȡ���ݳ���");
			break;
		}

		for (i = 0;i < iWriteDataLen;i++)
		{
			if (pReadBuffer[i] != pWriteBuffer[i])
			{
				MessageBox("����У��ʧ�ܣ�");
				break;
			}
		}

		if (i == iWriteDataLen)
		{
			MessageBox("д��ɹ�����У�飩");

			pReadBuffer[iWriteDataLen] = 0;//��һ������������������ַ���������ʾ��

			m_sBlockDataReadOut = pReadBuffer;

			UpdateData(FALSE);
		}
		
	} while (0);
	

	free(pReadBuffer);
	free(pWriteBuffer);
}

//�û�������ȫ��д��0Ȼ���ٶ�ȡ���жԱ�
void CBasicDlg::OnBnClickedButtonDataWriteZero()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString msg;
	BYTE *pReadBuffer;
	BYTE *pWriteBuffer;
	int i;

	pReadBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	pWriteBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);

	memset(pWriteBuffer,0,m_iMaxBlockDatalen);

	do 
	{
		if (CameraSaveUserData(m_hCamera,0,pWriteBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("д�����ݳ���");
			break;
		}



		if (CameraLoadUserData(m_hCamera,0,pReadBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("��ȡ���ݳ���");
			break;
		}

		for (i = 0;i < m_iMaxBlockDatalen;i++)
		{
			if (pReadBuffer[i] != pWriteBuffer[i])
			{
				MessageBox("����У��ʧ�ܣ�");
				break;
			}
		}

		if (i == m_iMaxBlockDatalen)
		{
			MessageBox("д��ɹ�����У�飩");
		}

	} while (0);


	free(pReadBuffer);
	free(pWriteBuffer);
}

//ȫ��д��0XFF��У��
void CBasicDlg::OnBnClickedButtonDataWrite0xff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString msg;
	BYTE *pReadBuffer;
	BYTE *pWriteBuffer;
	int i;

	pReadBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	pWriteBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);

	memset(pWriteBuffer,0xff,m_iMaxBlockDatalen);

	do 
	{
		if (CameraSaveUserData(m_hCamera,0,pWriteBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("д�����ݳ���");
			break;
		}



		if (CameraLoadUserData(m_hCamera,0,pReadBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("��ȡ���ݳ���");
			break;
		}

		for (i = 0;i < m_iMaxBlockDatalen;i++)
		{
			if (pReadBuffer[i] != pWriteBuffer[i])
			{
				MessageBox("����У��ʧ�ܣ�");
				break;
			}
		}

		if (i == m_iMaxBlockDatalen)
		{
			MessageBox("д��ɹ�����У�飩");
		}

	} while (0);


	free(pReadBuffer);
	free(pWriteBuffer);
}

//д����������ȡ����У��
void CBasicDlg::OnBnClickedButtonDataWriteRandom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString msg;
	BYTE *pReadBuffer;
	BYTE *pWriteBuffer;
	int i;

	pReadBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	pWriteBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);

	srand((unsigned) time(NULL)); 

	for (i = 0;i < m_iMaxBlockDatalen;i++)
	{
		pWriteBuffer[i] = rand()&0xff;
	}

	do 
	{
		if (CameraSaveUserData(m_hCamera,0,pWriteBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("д�����ݳ���");
			break;
		}



		if (CameraLoadUserData(m_hCamera,0,pReadBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("��ȡ���ݳ���");
			break;
		}

		for (i = 0;i < m_iMaxBlockDatalen;i++)
		{
			if (pReadBuffer[i] != pWriteBuffer[i])
			{
				MessageBox("����У��ʧ�ܣ�");
				break;
			}
		}

		if (i == m_iMaxBlockDatalen)
		{
			MessageBox("д��ɹ�����У�飩");
		}

	} while (0);


	free(pReadBuffer);
	free(pWriteBuffer);
}


void CBasicDlg::OnBnClickedButtonDataWriteInc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString msg;
	BYTE *pReadBuffer;
	BYTE *pWriteBuffer;
	int i;

	pReadBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);
	pWriteBuffer = (BYTE*)malloc(m_iMaxBlockDatalen);

	for (i = 0;i < m_iMaxBlockDatalen;i++)
	{
		pWriteBuffer[i] = i&0xff;
	}


	do 
	{
		if (CameraSaveUserData(m_hCamera,0,pWriteBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("д�����ݳ���");
			break;
		}



		if (CameraLoadUserData(m_hCamera,0,pReadBuffer,m_iMaxBlockDatalen) != CAMERA_STATUS_SUCCESS)
		{
			MessageBox("��ȡ���ݳ���");
			break;
		}

		for (i = 0;i < m_iMaxBlockDatalen;i++)
		{
			if (pReadBuffer[i] != pWriteBuffer[i])
			{
				MessageBox("����У��ʧ�ܣ�");
				break;
			}
		}

		if (i == m_iMaxBlockDatalen)
		{
			MessageBox("д��ɹ�����У�飩");
		}

	} while (0);


	free(pReadBuffer);
	free(pWriteBuffer);
}


void CBasicDlg::OnBnClickedButtonDataRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT iStatus;
	iStatus = 	CameraLoadUserData(m_hCamera,0,(BYTE*)m_sBlockDataReadOut.GetBuffer(m_iMaxBlockDatalen),m_iMaxBlockDatalen);
	m_sBlockDataReadOut.ReleaseBuffer();

	if (iStatus != CAMERA_STATUS_SUCCESS)
	{
		MessageBox("��ȡʧ��");
		return;
	}

	UpdateData(FALSE);
}


void CBasicDlg::OnBnClickedButtonSnRead1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sn[32];
	SDK_TRACE(CameraReadSN(m_hCamera,(BYTE*)sn,0),"��ȡһ�����к�");
	m_sSerialNumber1 = sn;
	UpdateData(FALSE);
}


void CBasicDlg::OnBnClickedButtonSnWrite1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char sn[32];
	UpdateData(TRUE);
	memcpy(sn,m_sSerialNumber1.GetBuffer(1),31);
	m_sSerialNumber1.ReleaseBuffer();
	sn[31] = 0 ;

	

}
