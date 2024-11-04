// GxStoreAVIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxStoreAVI.h"
#include <Windows.h>
#include "GxStoreAVIDlg.h"

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
// CGxStoreAVIDlg dialog

CGxStoreAVIDlg::CGxStoreAVIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxStoreAVIDlg::IDD, pParent)
	, m_bDevOpened(FALSE)
	, m_bEnableSaveAVI(FALSE)
	, m_hDevice(NULL)
	, m_hStream(NULL)
	, m_wPlayFPS(15)
	, m_pWnd(NULL)
	, m_pBitmap(NULL)
	, m_bSelectCompressed(FALSE)
	, m_hConvert(NULL)
{
	//{{AFX_DATA_INIT(CGxStoreAVIDlg)
	m_strAVIFolder = _T("");
	m_bCompressedAVI = FALSE;
	m_bShowImage = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxStoreAVIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxStoreAVIDlg)
	DDX_Text(pDX, IDC_EDIT_SAVEAS, m_strAVIFolder);
	DDX_Check(pDX, IDC_CHECK_COMPRESSED_AVI, m_bCompressedAVI);
	DDX_Check(pDX, IDC_CHECK_SHOW_IMAGE, m_bShowImage);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_FPS, m_wPlayFPS);
}

BEGIN_MESSAGE_MAP(CGxStoreAVIDlg, CDialog)
	//{{AFX_MSG_MAP(CGxStoreAVIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, OnBnClickedBtnCloseDevice)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS, OnBnClickedBtnSaveAs)
	ON_BN_CLICKED(IDC_BTN_AVI_START, OnBnClickedBtnAviStart)
	ON_BN_CLICKED(IDC_BTN_STOP_AVI, OnBnClickedBtnStopAvi)
	ON_BN_CLICKED(IDC_CHECK_COMPRESSED_AVI, OnBnClickedCheckCompressedAvi)
	ON_BN_CLICKED(IDC_CHECK_SHOW_IMAGE, OnBnClickedCheckShowImage)
	ON_BN_CLICKED(IDC_BTN_SELECT_COMPRESSOR, OnBnClickedBtnSelectCompressor)
	ON_EN_KILLFOCUS(IDC_EDIT_FPS, OnEnKillfocusEditFps)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxStoreAVIDlg message handlers

BOOL CGxStoreAVIDlg::OnInitDialog()
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

	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ��ʼ���豸��
	emStatus = GXInitLib();  
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	// ��ȡ��ǰ����ִ��·��,��Ϊͼ�񱣴��Ĭ��·��
	char    szName[MAX_PATH] = {0};
	CString  strAppFullName = ""; 
	GetModuleFileName(NULL, szName, MAX_PATH);
	strAppFullName = szName;
	int nPos = strAppFullName.ReverseFind('\\');
	m_strAVIFolder += strAppFullName.Left(nPos);
    UpdateData(FALSE);

	//�����VC6������֡�error C2065: 'CoInitializeEx' : undeclared identifier��
	//��ô����stdafx.h��ʼλ�����#define _WIN32_WINNT 0x0400
	if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != AVIERR_OK)
	{
		return FALSE;
	}

	//��ȡͼ����ʾ���ڵ�ָ��ͻ�ͼDC
	m_pWnd = GetDlgItem(IDC_STATIC_SHOW_WND);
	
	//ˢ�½���
	UpDateUI();
	 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxStoreAVIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxStoreAVIDlg::OnPaint() 
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

HCURSOR CGxStoreAVIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGxStoreAVIDlg::PreTranslateMessage(MSG* pMsg)
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
		case IDC_EDIT_FPS:
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
\brief  ���"���豸"��ť��Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  nDevNum  = 0;
	uint32_t	  nDSNum   = 0;
	DX_STATUS   emDxStatus     = DX_OK;

	// ö���豸
	emStatus = GXUpdateAllDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		MessageBox("δ�����豸!");
		return;
	}

	// ����豸�Ѿ�����ر�,��֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ���豸
	GX_OPEN_PARAM stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";

	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = true;

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

	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	do
	{
		// ��ʼ��ͼ����ʾģ��
		m_pBitmap = new CGXBitmap(m_hDevice, m_pWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		//׼����ʾͼ�����Դ
		m_pBitmap->PrepareForShowImg();

		//ע��ص�
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
		GX_VERIFY_BREAK(emStatus);

		//��������Buffer����ģʽ
		emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
		GX_VERIFY_BREAK(emStatus);

		//����ʼ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
		GX_VERIFY_BREAK(emStatus);

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pBitmap)
	{
		//�ͷ�Ϊͼ����ʾ׼������Դ 
		m_pBitmap->UnPrepareForShowImg();

		//��ʾ������Ϣ
		ShowErrorString(emStatus);

	    delete m_pBitmap;
		m_pBitmap = NULL;

		return;
	}

	//����ͼ��ת�����
	emDxStatus = (DX_STATUS)DxImageFormatConvertCreate(&m_hConvert);
	DX_VERIFY_RETURN(emDxStatus);

	// ���½���ؼ�
	UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����:����ͼ���ȡ����ʾ����
\param   pFrame   �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxStoreAVIDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CGxStoreAVIDlg *pDlg  = (CGxStoreAVIDlg*)(pFrame->pUserParam);

	DX_STATUS   emDxStatus     = DX_OK;
	BYTE *      pImageRGBBuf   = NULL;
	BYTE *      pRawBuffer     = NULL;
	int32_t     nSrcBufferSize = 0;              
	int32_t     nDstBufferSize = 0;

	do{
		if (pFrame->status != 0)
		{
			return;
		}

		//����ʾͼ���򷵻�
		if (!pDlg->m_bShowImage)
		{
			return;
		}

		pDlg->m_pBitmap->Show(pFrame);

		//��¼����Ƶ�򷵻�
		if(!pDlg->m_bEnableSaveAVI)
		{
			return;
		}

		//¼����Ƶ
		//BGR8��MONO8���������ظ�ʽת��
		if(GX_PIXEL_FORMAT_BGR8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
		{
			nDstBufferSize = pFrame->nWidth * pFrame->nHeight * PIXEL;
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}
			memcpy(pImageRGBBuf,pFrame->pImgBuf,(size_t)nDstBufferSize);

			pRawBuffer = new BYTE[nDstBufferSize];
			if (NULL == pRawBuffer)
			{
				break;
			}

			memcpy(pRawBuffer,pImageRGBBuf,(size_t)nDstBufferSize);

			// BGR��ʽ��Ҫ��ת���ݺ���ʾ
			for (int32_t i = 0; i < pFrame->nHeight; ++i)
			{
				memcpy(pImageRGBBuf + i * pFrame->nWidth * PIXEL, pRawBuffer + (pFrame->nHeight - i - 1) * pFrame->nWidth * PIXEL
					, (size_t)pFrame->nWidth * PIXEL);
			}
		}
		else if((GX_PIXEL_FORMAT_MONO8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_R8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_B8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat)
			||(GX_PIXEL_FORMAT_G8 == (GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat))
		{
			nDstBufferSize = pFrame->nWidth * pFrame->nHeight ;
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}
			memcpy(pImageRGBBuf,pFrame->pImgBuf,(size_t)nDstBufferSize);

			pRawBuffer = new BYTE[nDstBufferSize];
			if (NULL == pRawBuffer)
			{
				break;
			}

			memcpy(pRawBuffer,pImageRGBBuf,(size_t)nDstBufferSize);

			// �ڰ������Ҫ��ת���ݺ���ʾ
			for (int32_t i = 0; i < pFrame->nHeight; ++i)
			{
				memcpy(pImageRGBBuf + i * pFrame->nWidth, pRawBuffer + (pFrame->nHeight - i - 1) * pFrame->nWidth
					, (size_t)pFrame->nWidth);
			}
		}
		else
		{
			//��ɫ���
			if (pDlg->m_pBitmap->IsColor((GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat))
			{
				// ����Ŀ�����ظ�ʽΪBGR8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pDlg->m_hConvert, GX_PIXEL_FORMAT_BGR8);
				DX_VERIFY(emDxStatus);

				// ��ȡĿ��Buffer��С
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
					GX_PIXEL_FORMAT_BGR8,  static_cast<VxUint32>(pFrame->nWidth), 
					static_cast<VxUint32>(pFrame->nHeight), &nDstBufferSize);
				DX_VERIFY(emDxStatus);
			}

			//�ڰ����
			else
			{
				// ����Ŀ�����ظ�ʽΪMono8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pDlg->m_hConvert, GX_PIXEL_FORMAT_MONO8);
				DX_VERIFY(emDxStatus);

				// ��ȡĿ��Buffer��С
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
					GX_PIXEL_FORMAT_MONO8, static_cast<VxUint32>(pFrame->nWidth), 
					static_cast<VxUint32>(pFrame->nHeight), &nDstBufferSize);
				DX_VERIFY(emDxStatus);
			}

			//Ϊת�����ͼ��Buffer�����ڴ�
			pImageRGBBuf = new BYTE[nDstBufferSize];
			if (NULL == pImageRGBBuf)
			{
				break;
			}

			// ������Чλ
			emDxStatus = (DX_STATUS)DxImageFormatConvertSetValidBits(pDlg->m_hConvert, 
				pDlg->m_pBitmap->GetBestValudBit((GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat));
			DX_VERIFY(emDxStatus);


			// ��ȡԴBuffer��С
			emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pDlg->m_hConvert, 
				(GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat, static_cast<VxUint32>(pFrame->nWidth), 
				static_cast<VxUint32>(pFrame->nHeight), &nSrcBufferSize);
			DX_VERIFY(emDxStatus);


			// ���ظ�ʽת��
			emDxStatus = (DX_STATUS)DxImageFormatConvert(pDlg->m_hConvert, (void*)pFrame->pImgBuf, 
				nSrcBufferSize, pImageRGBBuf, nDstBufferSize, 
				(GX_PIXEL_FORMAT_ENTRY)pFrame->nPixelFormat, 
				static_cast<VxUint32>(pFrame->nWidth), static_cast<VxUint32>(pFrame->nHeight), true);
			DX_VERIFY(emDxStatus);
		}

		pDlg->m_pBitmap->SaveAVI(pImageRGBBuf);	

	}while(false);

	if (NULL != pImageRGBBuf)
	{
		delete[] pImageRGBBuf;
		pImageRGBBuf = NULL;
	}

	if (NULL != pRawBuffer)
	{
		delete[] pRawBuffer;
		pRawBuffer = NULL;
	}

	return;
}

//---------------------------------------------------------------------------------
/**
\brief   �����ʼ��

\return  ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxStoreAVIDlg::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice,"AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");
	// VERIFY_STATUS_RET(emStatus);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"��ť��Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;
	DX_STATUS   emDxStatus     = DX_OK;

	// ��δֹͣ¼������ֹͣ¼��
    if (m_bEnableSaveAVI)
    {
		m_bEnableSaveAVI = FALSE;
    }

	//����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	//����ͼ��ת�����
	emDxStatus = (DX_STATUS)DxImageFormatConvertDestroy(m_hConvert);
	DX_VERIFY_RETURN(emDxStatus);

	//ע���ص�
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}
    
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	m_pBitmap->UnPrepareForShowImg();
	if (NULL != m_pBitmap) 
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_hDevice    = NULL;
	m_bDevOpened = FALSE;

	//ˢ�½���
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ѡ��洢·��"��ť��Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnSaveAs() 
{
	// TODO: Add your control notification handler code here
	char arrFilePath[MAX_PATH*2] = {0};

	BROWSEINFO br;
	br.hwndOwner = GetSafeHwnd();
	br.iImage = 0;
	br.pszDisplayName = 0;
	br.lParam = 0 ;
	br.lpfn = 0 ;
	br.lpszTitle = "��ѡ����Ƶ������·��";
	br.pidlRoot = 0 ;
	br.ulFlags = BIF_RETURNONLYFSDIRS;
	if ( SHGetPathFromIDList(SHBrowseForFolder(&br),arrFilePath) != TRUE)
	{
		return ;
	}

	m_strAVIFolder = arrFilePath; 

	UpdateData(FALSE);	
}


//----------------------------------------------------------------------------------
/**
\brief  ���"��ʼ¼��"��ť��Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnAviStart() 
{
	// TODO: Add your control notification handler code here

	if (!m_bSelectCompressed)
	{
		bool bResult = m_pBitmap->InitAVI(const_cast<char*>(m_strAVIFolder.GetBuffer(m_strAVIFolder.GetLength())), m_wPlayFPS);
		if (!bResult) 
		{
			m_pBitmap->UnInitAVI();
			return;
		}
	}

	m_bEnableSaveAVI = TRUE;
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ֹͣ¼��"��ť��Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnStopAvi() 
{
	// TODO: Add your control notification handler code here
	m_bEnableSaveAVI = FALSE;
	m_bSelectCompressed = FALSE;
	m_pBitmap->UnInitAVI();
	UpDateUI();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"��ʾͼ��"Check����Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedCheckShowImage() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ѹ���洢"Check����Ϣ��Ӧ

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedCheckCompressedAvi() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    UpDateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ˢ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void  CGxStoreAVIDlg::UpDateUI()
{
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!m_bDevOpened);
	GetDlgItem(IDC_CHECK_SHOW_IMAGE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(m_bDevOpened);
	GetDlgItem(IDC_BTN_SAVE_AS)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_CHECK_COMPRESSED_AVI)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_STOP_AVI)->EnableWindow(m_bDevOpened && m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_SELECT_COMPRESSOR)->EnableWindow(m_bDevOpened && m_bCompressedAVI && !m_bEnableSaveAVI);
	GetDlgItem(IDC_BTN_AVI_START)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
	GetDlgItem(IDC_EDIT_FPS)->EnableWindow(m_bDevOpened && !m_bEnableSaveAVI);
}
//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in] ������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;

	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
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
\brief  �ر�Ӧ�ó���

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ��δֹͣ¼������ֹͣ¼��
	if (m_bEnableSaveAVI)
	{
		m_bEnableSaveAVI = FALSE;
		m_pBitmap->UnInitAVI();
		m_bSelectCompressed = FALSE;
	}

	if (m_bDevOpened)
	{
		//����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");

		//ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);

		m_pBitmap->UnPrepareForShowImg();

		emStatus = GXCloseDevice(m_hDevice);
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	//�ͷ���Դ��
	emStatus = GXCloseLib();

	CoUninitialize();
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"ѡ����Ƶѹ����"��ť��Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnBnClickedBtnSelectCompressor()
{
	//׼��������Ƶ����Դ
	bool bResult = m_pBitmap->InitAVI(const_cast<char*>(m_strAVIFolder.GetBuffer(m_strAVIFolder.GetLength())), m_wPlayFPS);
	if (!bResult) 
	{
		m_pBitmap->UnInitAVI();
		return;
	}

	bResult = m_pBitmap->SelectCompressor();
	if (!bResult) 
	{
		m_pBitmap->UnInitAVI();
		return;
	}
	m_bSelectCompressed = true;
}

//----------------------------------------------------------------------------------
/**
\brief  ����AVI��Ƶ������֡��ʧȥ������Ӧ����

\return void
*/
//----------------------------------------------------------------------------------
void CGxStoreAVIDlg::OnEnKillfocusEditFps()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

