// GxImageProcessDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "GxImageProcess.h"
#include "GxImageProcessDlg.h"
#include "string"


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
// CGxImageProcessDlg dialog

CGxImageProcessDlg::CGxImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxImageProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxImageProcessDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hDevice             = NULL;            // �豸���
	m_hStream			  = NULL;			 // �豸�����
	m_pBufferRaw          = NULL;            // ԭʼͼ������
	m_nPixelColorFilter   = 0;               // Bayer��ʽ
	m_bColorFilter        = false;           // ��ʶ�Ƿ�֧��Bayer��ʽ
	m_bAwbLampHouse       = false;           // ��ʾ�Ƿ�֧�ֹ�Դѡ��
	m_bColorCorrection    = false;           // ��ʾ�Ƿ�֧����ɫУ��
	m_bIsOpenCC           = false;           // ��ʶ��ɫУ����ѡ���Ƿ�ѡ��
	m_bIsOpen             = false;           // ����򿪱�־
	m_bIsSnap             = false;           // ����ɼ���־
	m_nGetColorCorrection = 0;               // ��ȡ����ɫУ��ֵ
	m_nColorCorrection    = 0;               // ��ɫУ��ֵ
	m_nContrast           = 0;               // �Աȶ�ֵ
	m_dGamma              = 0;               // The value of Gamma 
	m_nLightness          = 0;               // ����ֵ
	m_nSaturation         = 0;               // ���Ͷ�ֵ
    m_nPreColorCorrection = 0;               // ��ɫ����ǰ��ֵ
	m_nPreSaturation      = 0;               // ���Ͷȸ���ǰ��ֵ
	m_nPreContrast        = 0;               // �Աȶȸ���ǰ��ֵ
	m_nPreLightness       = 0;               // ���ȸ���ǰ��ֵ
	m_dPreGamma           = 0;               // Gammma����ǰ��ֵ
	m_fSharpen            = 0;               // ��ֵ
	m_bUpData             = true;            // ����CPU��ǰ��ʾ
	m_hWndHandle          = NULL;            // ��ʾͼ��Ĵ��ھ�� 
	m_nAutoWhiteBalance   = 0;               // �Զ���ƽ��ĵ�ǰֵ
    m_bEnableGamma        = false;           // ��ʶ�豸�Ƿ���Gamma
    m_bEnableSharpness    = false;           // ��ʶ�豸�Ƿ�����
	m_i64PixelFormat      = 0;               // ��ǰ���ظ�ʽ
	m_pGammaLut           = NULL;            // Gamma���ұ�
	m_pContrastLut        = NULL;            // �ԱȶȲ��ұ�
	m_nLutLength          = 0;               // ���ұ���

	m_bAccelerate = false;
	m_bColorCorrection = false;
	m_bDefectivePixelCorrect = false;
	m_bSharpness = false;
	m_bDenoise = false;
	m_bImageProcess = false;


	m_pBaseinfo = NULL;

	// ��ɫͼ���������ýṹ����ָ���ʼ��
	m_objImageProcess.parrCC  = NULL;
	m_objImageProcess.pProLut = NULL;
	
	// �ڰ�ͼ���������ýṹ����ָ���ʼ��
	m_objMonoImgProc.pProLut = NULL;

	m_pBitmap = NULL;
}

void CGxImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxImageProcessDlg)
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContrast);
	DDX_Control(pDX, IDC_SLIDER_LIGHTNESS, m_sliderLightness);
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_sliderSaturation);
	DDX_Control(pDX, IDC_SLIDER_SHARPEN, m_sliderSharpen);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ChooseDevice, m_comboChooseDevice);
}

BEGIN_MESSAGE_MAP(CGxImageProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CGxImageProcessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DEVICE, OnOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, OnStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, OnStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, OnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnSelchangeComboAwb)
	ON_CBN_SELCHANGE(IDC_COMBO_AWB_LIGHT, OnSelchangeComboAwbLight)
	ON_CBN_SELCHANGE(IDC_COMBO_PIXEL_FORMAT, OnSelchangeComboPixelFormat)
	
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OPEN_BAD_PIXEL_CORRECT, OnOpenBadPixelCorrect)
	ON_BN_CLICKED(IDC_OPEN_DENOISE, OnOpenDenoise)
	ON_BN_CLICKED(IDC_OPEN_ACCELERATE, OnOpenAccelerate)
	ON_BN_CLICKED(IDC_OPEN_SHARPEN, OnOpenSharpen)
	ON_BN_CLICKED(IDC_OPEN_COLOR_CORRECT, OnOpenColorCorrect)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxImageProcessDlg message handlers

BOOL CGxImageProcessDlg::OnInitDialog()
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
	
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ��ʼ����
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0); 
	}

	// ��ȡ��ͼ����ָ��;��
	GetDlgItem(IDC_STATIC_SHOW_FRAME, &m_hWndHandle);

	// ��ʼ�����б�ؼ�
	__InitCombo();
	
	// ����UI����
	UpdateUI();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//----------------------------------------------------------------
/**
\brief  ��ʼ�����б�ؼ�
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::__InitCombo()
{
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum = 0;

	// ö���豸����
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDeviceNum <= 0)
	{
		MessageBox("û��ö�ٵ��豸��������������������");
		m_bEnableOpen = false;
		return;
	}
	else
	{
		// ��ȡ�����豸�Ļ�����Ϣ
		m_pBaseinfo = new(std::nothrow) GX_DEVICE_BASE_INFO[nDeviceNum];
		if(NULL == m_pBaseinfo)
		{
			MessageBox("���仺����ʧ�ܣ�");
			return;
		}
		
		size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);
		emStatus = GXGetAllDeviceBaseInfo(m_pBaseinfo, &nSize);
		GX_VERIFY(emStatus);
		

		GX_DEVICE_BASE_INFO *pTmp = m_pBaseinfo;
		for (unsigned int i = 0; i < nDeviceNum; i++)
		{
			m_comboChooseDevice.AddString(pTmp->szDisplayName);
			m_vecDevInfo.push_back(*pTmp);
			pTmp++;
		}

		// Ĭ�������б�ѡ���һ̨���
		m_comboChooseDevice.SetCurSel(0);
		m_bEnableOpen = true;
	}
}

void CGxImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxImageProcessDlg::OnPaint() 
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
HCURSOR CGxImageProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGxImageProcessDlg::OnClose() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// δͣ����ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		
		// ע���ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		// �ͷ�ͼ����ʾ��Դ
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}
	
	if (m_bIsOpen)
	{
		// �ر����
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
		m_hDevice = NULL;

		// �ͷ�ͼ�������������Դ
	    UnInitImageProcessStruct();

		if (NULL != m_pBitmap)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
	}

	// �ر��豸��
	emStatus = GXCloseLib();

	if (NULL != m_pBaseinfo)
	{
		delete []m_pBaseinfo;
		m_pBaseinfo = NULL;
	}

	if (m_pGammaLut != NULL)
	{
		delete[] m_pGammaLut;
		m_pGammaLut = NULL;
	}
	if (m_pContrastLut != NULL)
	{
		delete[] m_pContrastLut;
		m_pContrastLut = NULL;
	}

	CDialog::OnClose();
}


//----------------------------------------------------------------
/**
\brief  ���豸��Ӧ����
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::OnOpenDevice() 
{
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	bool           bReturn    = false;
	GX_OPEN_PARAM  stOpenParam;
	bool	       bImplementPacketSize = false;
	uint32_t       unPacketSize		  = 0;
	uint32_t	  nDSNum	  = 0;

	// �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ��ѡ���豸������ѡ�е��豸
	int nSel = m_comboChooseDevice.GetCurSel();
	if (nSel < 0)
	{
		MessageBox("����������");
		return;
	}

	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_SN;
	stOpenParam.pszContent = m_vecDevInfo[nSel].szSN;

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
	
	// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
	if (GX_STATUS_SUCCESS != emStatus)
	{
		GXCloseDevice(m_hDevice);
		ShowErrorString(emStatus); 
		return ;
	}

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
		if (GX_STATUS_SUCCESS != emStatus)
	    {
		    GXCloseDevice(m_hDevice);
		    ShowErrorString(emStatus); 
		    return ;
	    }

		// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
		emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
		if (GX_STATUS_SUCCESS != emStatus)
	    {
		    GXCloseDevice(m_hDevice);
		    ShowErrorString(emStatus); 
		    return ;
	    }
	}

	// �񻯡��㷨���١�����У����ͼ����Ĭ�ϲ�����
	m_bSharpness = false;
	m_bAccelerate = false;
	m_bDefectivePixelCorrect = false;
	m_bDenoise = false;

	// ��ʼ������
	emStatus = InitParam();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		GXCloseDevice(m_hDevice);
		ShowErrorString(emStatus); 
		return ;
	}
	
	// ��ʼ��ͼ�����ܽṹ��
	bReturn = InitImageProcessStruct();
	if (!bReturn)
	{
		GXCloseDevice(m_hDevice);
		MessageBox("��ʼ��ͼ�����ܽṹ��ʧ�ܣ�");
		return;
	}

	//��ʼ��ͼ����ʾģ��
	m_pBitmap = new(std::nothrow) CGXBitmap(m_hDevice, &m_hWndHandle);
	if (NULL == m_pBitmap)
	{
		ShowErrorString(GX_STATUS_ERROR);
		return;
	}

	// ��ʼ��UI����ؼ�
	InitUI();
	m_bIsOpen = true;	
	m_bEnableOpen = false;

	//���ö�ʱ�����½���
	SetTimer(0, 1000, NULL);
	
	// ����UI����
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::InitParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE stIntValue;
	GX_ENUM_VALUE stEnumValue;
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;

	// �ɼ�ģʽ����Ϊ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// ��ȡͼ���С
	uint32_t   ui32PayLoadSize = 0;     // ԭʼRawͼ���С
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	VERIFY_STATUS_RET(emStatus);
	m_nPayLoadSize = ui32PayLoadSize;
	// ��ȡͼ����
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_nImageWidth = stIntValue.nCurValue;
	
	// ��ȡͼ��߶�
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_nImageHeight = stIntValue.nCurValue;


	// ��ȡ������ظ�ʽ
	emStatus = GXGetEnumValue(m_hDevice, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus)
	{
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus)
	{
		// ��ȡ��ǰ���ظ�ʽ
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		return GX_STATUS_ERROR;
	}

	m_bColorFilter = m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	// ��ȡͼ��Bayer��ʽ
	if (m_bColorFilter)
	{
		emStatus = GXGetEnumValue(m_hDevice, "PixelColorFilter", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelColorFilter = stEnumValue.stCurValue.nCurValue;
	}
	else
	{
		// ��ȡ�豸֧�ֵ����ظ�ʽ��ö�������
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_nPixelColorFilter = stEnumValue.stCurValue.nCurValue;
	}
	
	// �ж�����Ƿ�֧�ְ�ƽ���Դѡ��
	emStatus = GXGetNodeAccessMode(m_hDevice, "AWBLampHouse", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		m_bAwbLampHouse = true;
	}
	
	// ��ȡͼ�����
	emStatus = GetImageParam();
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ȡͼ�����

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::GetImageParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	bool      bIsImplemented = false;
	GX_FLOAT_VALUE stFloatValue;

	m_nPreLightness  = m_nLightness  = DEFAULT_LIGHTNESS;          // ����ֵ
	m_nPreSaturation = m_nSaturation = DEFAULT_SATURATION;         // ���Ͷ�ֵ
	m_fSharpen       = DEFAULT_SHARPEN;                            // ��ֵ
	m_nContrast      = DEFAULT_CONTRAST;                           // �Աȶ�


	// �ж�����Ƿ���Gammaʹ��
	emStatus = __IsEnableDeviceGamma();
	VERIFY_STATUS_RET(emStatus);
	
	// �ж�����Ƿ�֧��Gamma��ȡ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "GammaParam", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
		&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
		&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
	{
		bIsImplemented = true;
	}
	
	if (bIsImplemented)
	{
		// ��ȡGamma����
		emStatus = GXGetFloatValue(m_hDevice, "GammaParam", &stFloatValue);
		VERIFY_STATUS_RET(emStatus);
		m_dGamma = stFloatValue.dCurValue;
	}
	else
	{
		m_dGamma = DEFAULT_GAMMA;
	}
	m_dPreGamma = m_dGamma;
	
	
	// �ж�����Ƿ���������
	emStatus = __IsEnableDeviceSharpness();
	VERIFY_STATUS_RET(emStatus);

	if (m_bColorFilter)
	{
		emStatus = __InitColorParam();
		VERIFY_STATUS_RET(emStatus);
	} 
	else
	{
		// ����Ǻڰ����
		m_bColorCorrection = false;
		m_nGetColorCorrection = DEFAULT_COLOR_CORRECTION;
		m_nPreColorCorrection = DEFAULT_COLOR_CORRECTION;
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}
	
	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����ɫ������ֲ���

\return  ��ʼ�����״ֵ̬
*/
//----------------------------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__InitColorParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	GX_INT_VALUE stIntValue;

	// �ж�����Ƿ�����ɫУ������
	emStatus = __IsEnableDeviceColorCorrect();
	VERIFY_STATUS_RET(emStatus);


	// �ж�����Ƿ�֧����ɫУ��������ȡ
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevice, "ColorCorrectionParam", &emAccessMode);
	VERIFY_STATUS_RET(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bColorCorrection = true;
	}
	else
	{
	    m_bColorCorrection = false;
	}
	
	if (m_bColorCorrection)
	{
		// ��ȡ��ɫУ������
		emStatus = GXGetIntValue(m_hDevice, "ColorCorrectionParam", &stIntValue);
		VERIFY_STATUS_RET(emStatus);
		m_nGetColorCorrection = stIntValue.nCurValue;
	}
	else
	{
		m_nGetColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	// �Ƿ�ѡ����ɫУ��
	if (m_bIsOpenCC)
	{
		m_nPreColorCorrection = m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nPreColorCorrection = m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	return GX_STATUS_SUCCESS;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����ɫͼ����ṹ��

\return  true:���óɹ���false:����ʧ��
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::__InitColorStruct()
{
	VxInt32 nStatus = DX_OK;

	do 
	{
		// ����ͼ����ɫ�ʵ�������
		m_objImageProcess.nCCBufLength = CALCCCPARAM_LENGTH;
		m_objImageProcess.parrCC = new(std::nothrow) VxInt16[CALCCCPARAM_LENGTH];
		if (m_objImageProcess.parrCC == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}
		nStatus = DxCalcCCParam(m_nColorCorrection, m_nSaturation, m_objImageProcess.parrCC, CALCCCPARAM_LENGTH);
		if (nStatus != DX_OK)
		{
			break;
		}

		// ����ͼ����Lut���ұ�
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, NULL, &m_objImageProcess.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}
		m_objImageProcess.pProLut = new(std::nothrow) VxUint8[m_objImageProcess.nLutLength];
		if (m_objImageProcess.pProLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objImageProcess.pProLut, &m_objImageProcess.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

	} while (0);

	// ͼ������ұ��ʼ��ʧ�ܣ��ͷ���Դ
	if (nStatus != DX_OK)
	{
		if (m_objImageProcess.parrCC != NULL)
		{
			delete[]m_objImageProcess.parrCC;
			m_objImageProcess.parrCC = NULL;
		}
		if (m_objImageProcess.pProLut != NULL)
		{
			delete[]m_objImageProcess.pProLut;
			m_objImageProcess.pProLut = NULL;
		}
		return false;
	}

	// ��ɫͼ���������ýṹ������������ʼ��
	m_objImageProcess.bAccelerate            = m_bAccelerate;
	m_objImageProcess.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objImageProcess.bDenoise               = m_bDenoise;
	m_objImageProcess.bFlip                  = true;
	m_objImageProcess.bSharpness             = m_bSharpness;
	m_objImageProcess.fSharpFactor           = m_fSharpen;
	m_objImageProcess.cvType                 = RAW2RGB_NEIGHBOUR;
	m_objImageProcess.emLayOut               = (DX_PIXEL_COLOR_FILTER)m_nPixelColorFilter;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ڰ�ͼ����ṹ��

\return  true:���óɹ���false:����ʧ��
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::__InitMonoStruct()
{
	VxInt32 nStatus = DX_OK;

	do 
	{
		// ����ͼ����Lut���ұ�
		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, NULL, &m_objMonoImgProc.nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}
		m_objMonoImgProc.pProLut = new(std::nothrow) VxUint8[m_objMonoImgProc.nLutLength];
		if(m_objMonoImgProc.pProLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		nStatus = DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objMonoImgProc.pProLut, &m_objMonoImgProc.nLutLength);
	} while (0);

	// ͼ������ұ��ʼ��ʧ�ܣ��ͷ���Դ
	if (nStatus != DX_OK)
	{
		if (m_objMonoImgProc.pProLut != NULL)
		{
			delete[]m_objMonoImgProc.pProLut;
			m_objMonoImgProc.pProLut = NULL;
		}
		return false;
	}

	// �ڰ�ͼ���������ýṹ������������ʼ��
	m_objMonoImgProc.bAccelerate            = m_bAccelerate;
	m_objMonoImgProc.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objMonoImgProc.bSharpness             = m_bSharpness;
	m_objMonoImgProc.fSharpFactor           = m_fSharpen;
	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��ͼ����ṹ��

\return  true:���óɹ���false:����ʧ��
*/
//----------------------------------------------------------------------------------
bool CGxImageProcessDlg::InitImageProcessStruct()
{
	if (m_bColorFilter)
	{
		return __InitColorStruct();
	} 
	else
	{
		return __InitMonoStruct();
	}
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::InitUI()
{
	char chPutText[10] = {0};
	
	// ��ʼ����ƽ��Combox�ؼ�
    InitEnumUI("BalanceWhiteAuto", (CComboBox *)GetDlgItem(IDC_COMBO_AWB), m_bColorFilter);
	
	// ��ʼ����ƽ����ջ���Combox�ؼ�
	InitEnumUI("AWBLampHouse", (CComboBox *)GetDlgItem(IDC_COMBO_AWB_LIGHT), m_bAwbLampHouse);

	// ��ʼ�����ظ�ʽCombox�ؼ�
	InitEnumUI("PixelFormat", (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT), true);
	
	// ��ʼ���������ķ�Χ
	m_sliderSharpen.SetRange(1, 50, TRUE);
	m_sliderGamma.SetRange(1, 100, TRUE);
	m_sliderContrast.SetRange(-50, 100, TRUE);
	m_sliderLightness.SetRange(-150, 150, TRUE);
	m_sliderSaturation.SetRange(0, 128, TRUE);
	
	// ��ʼ���軬������λ��
	m_sliderSharpen.SetPos(int(m_fSharpen * 10));
	m_sliderGamma.SetPos(int(m_dGamma * 10));
	m_sliderContrast.SetPos(int(m_nContrast));	
	m_sliderLightness.SetPos(m_nLightness);
	m_sliderSaturation.SetPos(m_nSaturation);

	// ��ʼ��edit��ֵ
	_snprintf(chPutText, sizeof(chPutText), "%f", m_fSharpen);
	GetDlgItem(IDC_EDIT_SHARPEN)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%f", m_dGamma);
	GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%lld", m_nContrast);
	GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%d", m_nLightness);
	GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(chPutText);
	_snprintf(chPutText, sizeof(chPutText), "%d",m_nSaturation);
	GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(chPutText);
	
	// ��ʼ����check��
	if (m_bColorCorrection)
	{
		((CButton*)GetDlgItem(IDC_OPEN_COLOR_CORRECT))->SetCheck(m_bIsOpenCC);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_OPEN_COLOR_CORRECT))->SetCheck(false);
	}

	// ��ʼ����ѡ��
	((CButton*)GetDlgItem(IDC_OPEN_DENOISE))->SetCheck(m_bDenoise);
	((CButton*)GetDlgItem(IDC_OPEN_SHARPEN))->SetCheck(m_bSharpness);
	((CButton*)GetDlgItem(IDC_OPEN_ACCELERATE))->SetCheck(m_bAccelerate);
	((CButton*)GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT))->SetCheck(m_bDefectivePixelCorrect);

	
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   chFeature   [in]    ������ID
\param   pComboBox     [in]    �ؼ�ָ��
\param   bIsImplement  [in]    �豸�Ƿ�֧��chFeature������

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::InitEnumUI(char* chFeature, CComboBox *pComboBox, bool bIsImplement)
{
	// �жϿؼ�
	if ((pComboBox == NULL) || (!bIsImplement))
	{
		return;
	}

	GX_STATUS emStatus    = GX_STATUS_ERROR;
	GX_ENUM_VALUE stEnumValue;
	uint32_t  nEntryNum   = 0;
	int       nCursel     = 0;
	
	// ��ȡ���ܵ�ö����
	emStatus = GXGetEnumValue(m_hDevice, chFeature, &stEnumValue);
	GX_VERIFY(emStatus);
	
	// Ϊ��ȡö���͵Ĺ�����������ռ�
	nEntryNum = stEnumValue.nSupportedNum;

	std::string strFrature = chFeature;

	// ������Զ���ƽ��������Զ���ƽ���ֵ
	if (strFrature.compare("BalanceWhiteAuto"))
	{
		m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;
	}

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCursel = i;
		}
	}
	
	// ������ʾ��ǰֵ
	pComboBox->SetCurSel(nCursel);
}

void CGxImageProcessDlg::OnStartSnap() 
{
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	emStatus = m_pBitmap->PrepareForShowImg();
	if (GX_STATUS_SUCCESS != emStatus)
	{
		ShowErrorString(emStatus);
		return;
	}

	// ע��ص�����
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);

	if (GX_STATUS_SUCCESS != emStatus)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");
	
	// ����ʼ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (GX_STATUS_SUCCESS != emStatus)
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
\brief   ��ȡͼ��������������

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::GetImageImproveParam()
{
	if (!m_bColorFilter)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	VxInt32   nStatus = DX_OK;
	bool      bIsImplemented = false;

	// �ж�����Ƿ�֧�ֶԱȶȻ�ȡ
	GX_NODE_ACCESS_MODE emAccessMode;
	emStatus = GXGetNodeAccessMode(m_hDevice, "ContrastParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡ�ԱȶȲ���
		m_nContrast = m_nContrast;
	}
	else
	{
		m_nContrast = DEFAULT_CONTRAST;
	}

	// �ж�����Ƿ�֧��Gamma��ȡ
	emStatus = GXGetNodeAccessMode(m_hDevice, "GammaParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡGamma����
		m_dGamma = m_dGamma;
	}
	else
	{
		m_dGamma = DEFAULT_GAMMA;
	}

	// �ж�����Ƿ�֧����ɫУ��������ȡ
	emStatus = GXGetNodeAccessMode(m_hDevice, "ColorCorrectionParam", &emAccessMode);
	GX_VERIFY(emStatus);

	if ((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplemented = true;
	}

	if (bIsImplemented)
	{
		// ��ȡ��ɫУ������
		m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}

	do
	{
		// ��ȡGamma���ұ���
		nStatus = DxGetGammatLut(m_dGamma, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		if (NULL != m_pGammaLut) 
		{
			delete[] m_pGammaLut;
			m_pGammaLut = NULL;
		}
		if (NULL == m_pGammaLut) 
		{
			m_pGammaLut = new(std::nothrow) BYTE[m_nLutLength];
		}

		if (m_pGammaLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// ����Gamma���ұ�
		nStatus = DxGetGammatLut(m_dGamma, m_pGammaLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		// ��ȡ�ԱȶȲ��ұ���
		nStatus = DxGetContrastLut((int)m_nContrast, NULL, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

		if (NULL != m_pContrastLut) 
		{
			delete[] m_pContrastLut;
			m_pContrastLut = NULL;
		}
		if (NULL == m_pContrastLut) 
		{
			m_pContrastLut = new(std::nothrow) BYTE[m_nLutLength];
		}

		if (m_pContrastLut == NULL)
		{
			nStatus = DX_NOT_ENOUGH_SYSTEM_MEMORY;
			break;
		}

		// ����ԱȶȲ��ұ�
		nStatus = DxGetContrastLut((int)m_nContrast, m_pContrastLut, &m_nLutLength);
		if (nStatus != DX_OK)
		{
			break;
		}

	} while (0);

	// ���ò��ұ�ʧ�ܣ��ͷ���Դ
	if (nStatus != DX_OK)
	{
		if (m_pGammaLut != NULL)
		{
			delete[] m_pGammaLut;
			m_pGammaLut = NULL;
		}
		if (m_pContrastLut != NULL)
		{
			delete[] m_pContrastLut;
			m_pContrastLut = NULL;
		}
		MessageBox("��ȡͼ������ұ����ʧ�ܣ�");
		return;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �ͷ�ͼ��������������ռ�õ��ڴ�

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::ReleaseImageImproveParam()
{
	if (m_pGammaLut != NULL)
	{
		delete[] m_pGammaLut;
		m_pGammaLut = NULL;
	}
	if (m_pContrastLut != NULL)
	{
		delete[] m_pContrastLut;
		m_pContrastLut = NULL;
	}
}


//---------------------------------------------------------------------------------
/**
\brief   �ص������в�ɫͼ���㷨����
\param   pDlg   �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnColorImageProcess()
{
	VxInt32  emStatus = DX_OK;

	m_objImageProcess.bAccelerate = m_bAccelerate;
	m_objImageProcess.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objImageProcess.bSharpness = m_bSharpness;
	m_objImageProcess.bDenoise = m_bDenoise;

	// �ж��Ƿ����¼���ͼ����ɫ�ʵ�������
	if ((m_nPreSaturation != m_nSaturation) || (m_nColorCorrection != m_nPreColorCorrection))
	{
		DxCalcCCParam(m_nColorCorrection, m_nSaturation, m_objImageProcess.parrCC, CALCCCPARAM_LENGTH);
		m_nPreSaturation      = m_nSaturation;
		m_nPreColorCorrection = m_nColorCorrection;
	}

	// �ж��Ƿ���Ҫ���¼���ͼ����8λ���ұ�
	if ((m_nPreContrast != m_nContrast) || (m_nPreLightness != m_nLightness) 
		|| (m_dPreGamma != m_dGamma))
	{
		DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objImageProcess.pProLut, &m_objImageProcess.nLutLength);
		m_nPreContrast  = m_nContrast;
		m_nPreLightness = m_nLightness;
		m_dPreGamma     = m_dGamma;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   �ص������кڰ�ͼ���㷨����
\param   pDlg   �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnMonoImageProcess()
{
	VxInt32  emStatus = DX_OK;
	m_objMonoImgProc.bAccelerate = m_bAccelerate;
	m_objMonoImgProc.bDefectivePixelCorrect = m_bDefectivePixelCorrect;
	m_objMonoImgProc.bSharpness = m_bSharpness;
	m_objMonoImgProc.fSharpFactor = m_fSharpen;
	

	// �ж��Ƿ���Ҫ���¼���ͼ����8λ���ұ�
	if ((m_nPreContrast != m_nContrast) || (m_nPreLightness != m_nLightness) || (m_dPreGamma != m_dGamma))
	{
		DxGetLut((VxInt32)m_nContrast, m_dGamma, m_nLightness, m_objMonoImgProc.pProLut, &m_objMonoImgProc.nLutLength);
		m_nPreContrast  = m_nContrast;
		m_nPreLightness = m_nLightness;
		m_dPreGamma     = m_dGamma;
	}
}


//---------------------------------------------------------------------------------
/**
\brief   �ص�����
\param   pFrame   �ص�����

\return  ��
*/
//----------------------------------------------------------------------------------
void __stdcall CGxImageProcessDlg::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CGxImageProcessDlg *pDlg = (CGxImageProcessDlg*)(pFrame->pUserParam);
	
	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		return;
	}
	
	//��ʶ�������ڽ���ͼ����
	pDlg->m_bImageProcess = true;

	if ((GX_PIXEL_FORMAT_RGB8 == pFrame->nPixelFormat)
		|| (GX_PIXEL_FORMAT_BGR8 == pFrame->nPixelFormat)) 
	{
		pDlg->GetImageImproveParam();
		pDlg->m_pBitmap->ShowAfterImprovment(pFrame,pDlg->m_nColorCorrection, pDlg->m_pContrastLut, pDlg->m_pGammaLut);
	}
	else 
	{
		pDlg->m_pBufferRaw = const_cast<void*>(pFrame->pImgBuf);
		// �ص������в��Ƽ�����ͼ�����˴���Ϊ��ʾͼ����ӿڵ���
		bool bIsColor = pDlg->m_pBitmap->IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(pDlg->m_i64PixelFormat));
		if (bIsColor)
		{
			pDlg->OnColorImageProcess();
			pDlg->m_pBitmap->ShowAfterProcess(pFrame, &pDlg->m_objImageProcess);
		}
		else
		{
			pDlg->OnMonoImageProcess();
			pDlg->m_pBitmap->ShowAfterProcess(pFrame, &pDlg->m_objMonoImgProc);
		}

	}
	//��ʶ����û�н���ͼ����
	pDlg->m_bImageProcess = false;
}

void CGxImageProcessDlg::OnStopSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// ע���ص�����
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);

	if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat) || (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
	{
		ReleaseImageImproveParam();
	}

	m_bIsSnap = false;
	
	// ����UI����
	UpdateUI();	
}

void CGxImageProcessDlg::OnCloseDevice() 
{
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

		if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat) || (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
		{
			ReleaseImageImproveParam();
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

	m_pBitmap->UnPrepareForShowImg();
	// �ͷ�ͼ����ʾ��Դ
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// �ͷ�ͼ�������������Դ
	UnInitImageProcessStruct();

	m_bIsOpen = false;
	m_bEnableOpen = true;

	// ֹͣTimer
	KillTimer(0);
	
	// ����UI����
	UpdateUI();
}

//---------------------------------------------------------------------------------
/**
\brief    �ͷ�ͼ�������������Դ

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::UnInitImageProcessStruct()
{
	if (m_objImageProcess.parrCC != NULL)
	{
		delete[]m_objImageProcess.parrCC;
		m_objImageProcess.parrCC = NULL;
	}
	if (m_objImageProcess.pProLut != NULL)
	{
		delete[]m_objImageProcess.pProLut;
		m_objImageProcess.pProLut = NULL;
	}
	if (m_objMonoImgProc.pProLut != NULL)
	{
		delete[]m_objMonoImgProc.pProLut;
		m_objMonoImgProc.pProLut = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡGxIAPI��������,������������ʾ�Ի���
\param  emErrorStatus  [in]   ������

\return ��
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// ��ȡ����������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// ��ȡ������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	
	// ��ʾ����������Ϣ
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ��!");
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
void CGxImageProcessDlg::UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(m_bEnableOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(m_bIsOpen && m_bColorFilter);
	GetDlgItem(IDC_COMBO_AWB_LIGHT)->EnableWindow(m_bIsOpen && m_bAwbLampHouse && m_bColorFilter);
	GetDlgItem(IDC_COMBO_ChooseDevice)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_COMBO_PIXEL_FORMAT)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_OPEN_COLOR_CORRECT)->EnableWindow(m_bIsOpen && m_bColorCorrection && m_bColorFilter);

	GetDlgItem(IDC_SLIDER_CONTRAST)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(m_bIsOpen && !m_bEnableGamma);
	GetDlgItem(IDC_EDIT_GAMMA)->EnableWindow(m_bIsOpen && !m_bEnableGamma);

	if ((GX_PIXEL_FORMAT_RGB8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat))
	{
		GetDlgItem(IDC_OPEN_ACCELERATE)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_DENOISE)->EnableWindow(false);
		GetDlgItem(IDC_OPEN_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SHARPEN)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_LIGHTNESS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(false);
	}
	else 
	{
		GetDlgItem(IDC_OPEN_ACCELERATE)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_OPEN_BAD_PIXEL_CORRECT)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_OPEN_DENOISE)->EnableWindow(m_bIsOpen && m_bColorFilter);
		GetDlgItem(IDC_OPEN_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness);
		GetDlgItem(IDC_SLIDER_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness && m_bSharpness);
		GetDlgItem(IDC_EDIT_SHARPEN)->EnableWindow(m_bIsOpen && !m_bEnableSharpness && m_bSharpness);
		GetDlgItem(IDC_SLIDER_LIGHTNESS)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->EnableWindow(m_bIsOpen);
		GetDlgItem(IDC_SLIDER_SATURATION)->EnableWindow(m_bIsOpen && m_bColorFilter);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(m_bIsOpen && m_bColorFilter);
	}
}

void CGxImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char PutText[10] = {0};
	int  nValue      = 0;

	if (pScrollBar == (CScrollBar*)&m_sliderLightness)            // ���Ȼ�����
	{
		m_nLightness = m_sliderLightness.GetPos();
		_snprintf(PutText, sizeof(PutText), "%d", m_nLightness);
		GetDlgItem(IDC_EDIT_LIGHTNESS)->SetWindowText(PutText);	
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderContrast)         // �ԱȶȻ�����
	{
		m_nContrast = m_sliderContrast.GetPos();
		_snprintf(PutText, sizeof(PutText), "%lld", m_nContrast);
		GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(PutText);	
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderSaturation)       // ���ͶȻ�����
	{
		m_nSaturation = m_sliderSaturation.GetPos();
		_snprintf(PutText, sizeof(PutText), "%d", m_nSaturation);
		GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(PutText);	
	}	
	else if (pScrollBar == (CScrollBar*)&m_sliderSharpen)          // �񻯻�����
	{
		nValue = m_sliderSharpen.GetPos();
		m_fSharpen = nValue / 10.0F;
		m_objImageProcess.fSharpFactor = m_fSharpen;
		_snprintf(PutText, sizeof(PutText), "%7f", m_fSharpen);
		GetDlgItem(IDC_EDIT_SHARPEN)->SetWindowText(PutText);		
	}
	else if (pScrollBar == (CScrollBar*)&m_sliderGamma)            // Gamma������
	{
		nValue = m_sliderGamma.GetPos();
		m_dGamma = nValue / 10.0;
		_snprintf(PutText, sizeof(PutText), "%7f", m_dGamma);
		GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(PutText);		
	}
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGxImageProcessDlg::OnOpenBadPixelCorrect() 
{
	m_bDefectivePixelCorrect = !m_bDefectivePixelCorrect;
}

void CGxImageProcessDlg::OnOpenDenoise() 
{
	m_bDenoise = !m_bDenoise;
}

void CGxImageProcessDlg::OnOpenAccelerate() 
{
	m_bAccelerate = !m_bAccelerate;
}

void CGxImageProcessDlg::OnOpenSharpen() 
{
	m_bSharpness = !m_bSharpness;
	UpdateUI();
	
}

void CGxImageProcessDlg::OnOpenColorCorrect() 
{
	m_bIsOpenCC = !m_bIsOpenCC;
	if (m_bIsOpenCC)
	{
		m_nColorCorrection = m_nGetColorCorrection;
	}
	else
	{
		m_nColorCorrection = DEFAULT_COLOR_CORRECTION;
	}	
}

void CGxImageProcessDlg::OnSelchangeComboAwb() 
{
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nEnumVal = 0;
	int       nIndex   = 0;
	
	nIndex   = pBox->GetCurSel();                    // ��ȡ��ǰѡ��
	nEnumVal = (int64_t)pBox->GetItemData(nIndex);   // ��ȡ��ǰѡ���Ӧ��ö����ֵ
	
	// ���ð�ƽ��
	emStatus = GXSetEnumValue(m_hDevice, "BalanceWhiteAuto", nEnumVal);
	GX_VERIFY(emStatus);

	m_nAutoWhiteBalance = nEnumVal;
}

void CGxImageProcessDlg::OnSelchangeComboAwbLight() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox    = (CComboBox *)GetDlgItem(IDC_COMBO_AWB_LIGHT);
	
	int      nIndex    = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal  = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ
	
	// ���ð�ƽ���Դ
	emStatus = GXSetEnumValue(m_hDevice, "AWBLampHouse", nEnumVal);
	GX_VERIFY(emStatus);
}

//----------------------------------------------------------------------------------
/**
\brief   �ı����ظ�ʽ����Ӧ����

\return  void
*/
//----------------------------------------------------------------------------------
void CGxImageProcessDlg::OnSelchangeComboPixelFormat()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CComboBox *pBox = (CComboBox *)GetDlgItem(IDC_COMBO_PIXEL_FORMAT);

	int      nIndex = pBox->GetCurSel();                   // ��ȡ��ǰѡ��
	int64_t  nEnumVal = (int64_t)pBox->GetItemData(nIndex);  // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	// ���ð�ƽ���Դ
	emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", nEnumVal);
	GX_VERIFY(emStatus);
	m_i64PixelFormat = nEnumVal;
	UpdateUI();
}


BOOL CGxImageProcessDlg::PreTranslateMessage(MSG* pMsg) 
{
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

//----------------------------------------------------------------
/**
\brief  ��ʱ������:��ʱ��ȡ�Զ���ƽ���ö��ֵʵ������Once�ɹ�������Զ�����Ϊoff
\param  ��ʱ���¼�ID

\return ��
*/
//----------------------------------------------------------------
void CGxImageProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	GX_STATUS status = GX_STATUS_ERROR;
	GX_ENUM_VALUE stEnumValue;

	//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
	//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		int64_t nWBEnumVal = 0;   //< �����ƽ���ö��ֵ

		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);

		//��ȡ�Զ���ƽ��ö��ֵ
		status = GXGetEnumValue(m_hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (status != GX_STATUS_SUCCESS)
		{
			return;
		}
		
		nWBEnumVal = stEnumValue.stCurValue.nCurValue;
		m_nAutoWhiteBalance = nWBEnumVal;

		//�ж��豸�Զ���ƽ���Ƿ��ΪOFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
					pAutoWBBox->SetCurSel(i);
					break;
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ƿ�����ɫУ��

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceColorCorrect()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;

    do 
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "ColorTransformationEnable", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetBoolValue(m_hDevice, "ColorTransformationEnable", &m_bColorCorrection);
        VERIFY_STATUS_BREAK(emStatus);
    } while (0);
    
    return emStatus;
}

//----------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ƿ���Gammaʹ��

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceGamma()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;
    m_bEnableGamma = false;

    do 
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GammaEnable", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetBoolValue(m_hDevice, "GammaEnable", &m_bEnableGamma);
        VERIFY_STATUS_BREAK(emStatus);
    } while (0);

    return emStatus;
}

//----------------------------------------------------------------
/**
\brief  ��ȡ�豸�Ƿ�����

\return GX_STATUS
*/
//----------------------------------------------------------------
GX_STATUS CGxImageProcessDlg::__IsEnableDeviceSharpness()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    bool bIsImplement = false;
    m_bEnableSharpness = false;
	GX_ENUM_VALUE stEnumValue;

    do
    {
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "SharpnessMode", &emAccessMode);
		VERIFY_STATUS_BREAK(emStatus);

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
		{
			bIsImplement = true;
		}
        

        if(false == bIsImplement)
        {
            break;
        }

        emStatus = GXGetEnumValue(m_hDevice, "SharpnessMode", &stEnumValue);
        VERIFY_STATUS_BREAK(emStatus);

        if(GX_SHARPNESS_MODE_OFF == stEnumValue.stCurValue.nCurValue)
        {
            m_bEnableSharpness = false;
        }else if(GX_SHARPNESS_MODE_ON)
        {
            m_bEnableSharpness = true;
        }
    } while (0);

    return emStatus;
}


