// GxSnapRaw16Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxSnapRaw16.h"
#include "GxSnapRaw16Dlg.h"

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
// CGxSnapRaw16Dlg dialog

CGxSnapRaw16Dlg::CGxSnapRaw16Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxSnapRaw16Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxSnapRaw16Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strSavePath           = "";              // ͼ�񱣴�·��
	m_hDevice               = NULL;            // �豸���
	m_hStream				= NULL;			   // �����
	m_pRawBuf               = NULL;            // ԭʼRawͼ��Buffer
	m_pRaw8Buf              = NULL;            // 8λRawͼ��Buffer
	m_nPixelFilter     = 0;               // ��ɫ�����Bayer��ʽ
	m_bIsColorFilter        = false;           // �ж�����Ƿ�֧��Bayer��ʽ
	m_bIsOpen               = false;           // ����򿪱�ʶ
	m_bIsSnap               = false;           // ����ɼ���ʶ
	m_bIsSaveImg            = false;           // ����ͼ���ʶ
    m_hWndHandle            = NULL;            // ��ʾͼ��Ĵ��ھ�� 
	m_pBitmap               = NULL;
}

void CGxSnapRaw16Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxSnapRaw16Dlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxSnapRaw16Dlg, CDialog)
	//{{AFX_MSG_MAP(CGxSnapRaw16Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_RAW_IMAGE, OnSaveRawImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxSnapRaw16Dlg message handlers

BOOL CGxSnapRaw16Dlg::OnInitDialog()
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
	int     nPos = 0;
	char    chPathName[MAX_PATH] = {0};   
	CString strPathName = ""; 

	// ��ʼ����
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ��ʼ��ͼ�񱣴�·��
	GetModuleFileName(NULL, chPathName, MAX_PATH);
	strPathName = chPathName;
	nPos = strPathName.ReverseFind('\\');
	m_strSavePath += strPathName.Left(nPos);
	m_strSavePath += "\\GxSnapRaw16Images";

	// ��ȡ��ͼ����ָ��;��
	GetDlgItem(IDC_STATIC_SHOW_IMAGE, &m_hWndHandle);
    m_hDC  = ::GetDC(m_hWndHandle); 
	::GetClientRect(m_hWndHandle, &m_objRect);	     
	
	// ����UI����
	UpdateUI();

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxSnapRaw16Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxSnapRaw16Dlg::OnPaint() 
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
HCURSOR CGxSnapRaw16Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------------
/**
\brief   �ر�Ӧ�ó�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// δͣ����ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// ע���ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}
	
	// δ�ر��豸��ر��豸
	if (m_bIsOpen)
	{
		// �ر����
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;
	}
	
	// �ر��豸��
	emStatus = GXCloseLib();
	
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	CDialog::OnClose();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"���豸"�ؼ���Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS      emStatus     = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum   = 0;
	int64_t        nPixelFormat = 0;   
	uint32_t	   nDSNum       = 0;
	GX_OPEN_PARAM  stOpenParam;
	
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

	do
	{
		//��ʼ��ͼ����ʾģ��
		m_pBitmap = new CGXBitmap(m_hDevice, &m_hWndHandle);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// ��ʼ������
		emStatus = InitParam();
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
	    delete m_pBitmap;
		m_pBitmap = NULL;
		return;
	}

	m_bIsOpen = true;	

	// ����UI����
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"��ʼ�ɼ�"�ؼ���Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnStartSnap() 
{
	// TODO: Add your control notification handler code here
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	m_pBitmap->PrepareForShowImg();
	// ע��ص�����
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// ����ʼ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = true;

	// ����UI����
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"ֹͣ�ɼ�"�ؼ���Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnStopSnap() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// ע���ص�����
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);
  
	m_bIsSnap = false;

	// ����UI����
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ���"�ر��豸"�ؼ���Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnCloseDevice() 
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
		
		// ע���ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		
		// �ͷ�ͼ����ʾ��Դ
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
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

//---------------------------------------------------------------------------------
/**
\brief   ���"����Rawͼ��"CheckBox�ؼ���Ӧ����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::OnSaveRawImage() 
{
	// TODO: Add your control notification handler code here
	m_bIsSaveImg = !m_bIsSaveImg;	
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxSnapRaw16Dlg::InitParam()
{
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	CString   strSymbolic = "";
	 GX_ENUM_VALUE stEnumValue;
	
	// ���òɼ�ģʽΪ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// ��֪��ǰ���֧���ĸ���8λ��ͼ�����ݸ�ʽ����ֱ������,�Է�8λ���ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR12Ϊ����
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR12");
    // �Է�8λ���ݸ�ʽGX_PIXEL_FORMAT_MONO10Ϊ��:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "Mono10");
	
	// ��֪����ǰ���֧���ĸ���8λ��ͼ�����ݸ�ʽ�����Ե��ú���SetPixelSize16��ͼ�����ݸ�ʽ����Ϊ��8λ��
	emStatus = SetPixelSize16(&strSymbolic);
	VERIFY_STATUS_RET(emStatus);

	// �����õ����ݸ���ʾ������
	GetDlgItem(IDC_EDIT_PIXEL_FORMAT)->SetWindowText(strSymbolic);

	// ��ȡ������ظ�ʽ
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		m_nPixelFilter = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// ��ȡ��ǰ���ظ�ʽ
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelFilter = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}

	m_bIsColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_nPixelFilter));

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief    ����ͼ�����ݸ�ʽΪ16λ
\param    pstrSymbolic  16λͼ�����ݸ�ʽ�ַ�����

\return   emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
// ----------------------------------------------------------------------------------
GX_STATUS CGxSnapRaw16Dlg::SetPixelSize16(CString  *pstrSymbolic)
{
	uint32_t  i           = 0;                   
	uint32_t  nEnmuEntry  = 0;
	GX_STATUS emStatus    = GX_STATUS_SUCCESS;
	GX_ENUM_VALUE stEnumValue;

	// ��ȡ�豸֧�ֵ����ظ�ʽ��ö�������
	emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		*pstrSymbolic = "\0";
		return emStatus;
	}
	nEnmuEntry = stEnumValue.nSupportedNum;

	for (i = 0; i < nEnmuEntry; i++)
	{
		if ((stEnumValue.nArrySupportedValue[i].nCurValue & GX_PIXEL_16BIT) == GX_PIXEL_16BIT)
		{
			emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", stEnumValue.nArrySupportedValue[i].nCurValue);
			*pstrSymbolic = stEnumValue.nArrySupportedValue[i].strCurSymbolic;
			break;
		}
	}	
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����:ͼ���ȡ���������ʾ����
\param   pFrame   �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGxSnapRaw16Dlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxSnapRaw16Dlg *pDlg   = (CGxSnapRaw16Dlg*)(pFrame->pUserParam);
	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		return;
	}


	// ͼ�񱣴�
	if (pDlg->m_bIsSaveImg)
	{
		void* pImgBuff = const_cast<void*>(pFrame->pImgBuf);
		pDlg->SaveRawImage(pImgBuff);
	}

	// �ص�������ʾ����
	pDlg->m_pBitmap->Show(pFrame);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʾͼ��
\param   pImageBuf   ͼ��Buffer
\param   nWidth      ͼ���
\param   nHeight     ͼ���
\param   pBmpInfo    BITMAPINFO�ṹָ�� 
\param   pWnd        ��ʾͼ�񴰿�

\return  void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::DrawImage(BYTE *pImageBuf, int nWidth, int nHeight, BITMAPINFO *pBmpInfo)
{
	if ((pImageBuf == NULL) || (pBmpInfo == NULL))
	{
		return;
	}

	// ��ʾͼ��Ĵ��ڴ�С                                      
	int nWndWidth  = m_objRect.right - m_objRect.left;      // ͼ����ʾ���ڿ��
	int nWndHeight = m_objRect.bottom - m_objRect.top;      // ͼ����ʾ���ڸ߶�

	// ������ø���䣬����ͼ�����ˮ��
	::SetStretchBltMode(m_hDC, COLORONCOLOR);
	::StretchDIBits(m_hDC,
		0,						
		0,
		nWndWidth,
		nWndHeight,
		0,
		0,
		nWidth,
		nHeight,
		pImageBuf,
		pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);	
}

//---------------------------------------------------------------------------------
/**
\brief   ����Rawͼ��
\param   pRawBuf      Rawͼ��
\param   nImageSize   ͼ���С
\param   srtSavePath  ͼ��洢·��

\return  void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::SaveRawImage(void *pRawBuf)
{
	if ((pRawBuf == NULL) )
	{
		return;
	}

	CString strFileName = "";               // ͼ��洢·��
	SYSTEMTIME  sysTime;                    // ��ȡ��ǰʱ��
	GetLocalTime(&sysTime);

	//��������ͼ����ļ���
	BOOL bRet = ::CreateDirectory(m_strSavePath, NULL);
	if (bRet)
	{
		::SetFileAttributes(m_strSavePath, FILE_ATTRIBUTE_NORMAL);
	}

	// ��ȡ��ǰʱ��Ϊͼ�񱣴��Ĭ������
	strFileName.Format("%s\\%d_%d_%d_%d_%d_%d_%d.raw",
		m_strSavePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);


	char *pFilePath = strFileName.GetBuffer(strFileName.GetLength());
	m_pBitmap->SaveRaw(pRawBuf, pFilePath);	
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������Ϣ����
\param  emErrorStatus  ������

 \return void
*/
//----------------------------------------------------------------------------------
void CGxSnapRaw16Dlg::ShowErrorString(GX_STATUS emErrorStatus)
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
void CGxSnapRaw16Dlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
}

//----------------------------------------------------------------------------------
/**
\brief   �س�����Esc������Ϣ��Ӧ����
\param   pMsg  ��Ϣ�ṹ��
  
\return �ɹ�:TRUE   ʧ��:FALSE
*/
//----------------------------------------------------------------------------------
BOOL CGxSnapRaw16Dlg::PreTranslateMessage(MSG* pMsg) 
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
