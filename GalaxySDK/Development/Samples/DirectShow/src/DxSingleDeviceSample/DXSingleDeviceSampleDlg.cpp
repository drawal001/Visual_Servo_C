
// DXSingleDeviceSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DXSingleDeviceSample.h"
#include "DXSingleDeviceSampleDlg.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDXSingleDeviceSampleDlg dialog



//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CDXSingleDeviceSampleDlg::CDXSingleDeviceSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDXSingleDeviceSampleDlg::IDD, pParent)
	, m_nDeviceID(DEVICE_ID_INVAILD)
	, m_pCamDevice(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CDXSingleDeviceSampleDlg::~CDXSingleDeviceSampleDlg()
{
	if (NULL != m_pCamDevice)
	{
		delete m_pCamDevice;
		m_pCamDevice = NULL;
	}
}

void CDXSingleDeviceSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDXSingleDeviceSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_OPEN_DEVICE, &CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenDevice)
    ON_BN_CLICKED(IDC_BUTTON_START_ACQUISITION, &CDXSingleDeviceSampleDlg::OnBnClickedButtonStartAcquisition)
    ON_BN_CLICKED(IDC_BUTTON_STOP_ACQUISITION, &CDXSingleDeviceSampleDlg::OnBnClickedButtonStopAcquisition)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEVICE, &CDXSingleDeviceSampleDlg::OnBnClickedButtonCloseDevice)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CDXSingleDeviceSampleDlg::OnEnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE, &CDXSingleDeviceSampleDlg::OnEnKillfocusEditExposure)
	ON_CBN_SELCHANGE(IDC_COMBO_WHITE_BALANCE, &CDXSingleDeviceSampleDlg::OnCbnSelchangeComboWhiteBalance)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_FILTER_PAGE, &CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenFilterPage)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_PIN_PAGE, &CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenPinPage)
    ON_BN_CLICKED(IDC_CHECK_SAVE_PICTURE, &CDXSingleDeviceSampleDlg::OnBnClickedCheckSavePicture)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDXSingleDeviceSampleDlg message handlers

BOOL CDXSingleDeviceSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_pCamDevice = new(std::nothrow) CDevice();
	if (NULL == m_pCamDevice)
	{
		MessageBox("The submodule instantiation failed");
		exit(-1);
	}

	SetTimer(0,1000,NULL);    //set timer

	__UpdateUI();
	__UpdateDevice();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDXSingleDeviceSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDXSingleDeviceSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDXSingleDeviceSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//---------------------------------------------------
/**
\brief		Click to open the device button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenDevice()
{
    // TODO: Add your control notification handler code here

	//Prevent a repeat entry before the first one is completed
	GetDlgItem(IDC_BUTTON_OPEN_DEVICE)->EnableWindow(FALSE);

	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->OpenDevice(m_nDeviceID))
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayExposureTime())
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayGain())
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayWhiteBalance())
		{
			break;
		}
	} while (0);

	__UpdateUI();
}

//---------------------------------------------------
/**
\brief		Click the close device button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonCloseDevice()
{
	// TODO: Add your control notification handler code here
	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->CloseDevice())
		{
			break;
		}
	} while (0);

	__UpdateUI();  
}

//---------------------------------------------------
/**
\brief		Click the Start Acquisition button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonStartAcquisition()
{
    // TODO: Add your control notification handler code here
	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->StartAcquisition(GetDlgItem(IDC_PICTURE_CONTROL)))
		{
			break;
		}
	} while (0);

	__UpdateUI();
}

//---------------------------------------------------
/**
\brief		Click to stop collecting button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonStopAcquisition()
{
	// TODO: Add your control notification handler code here
	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->StopAcquisition())
		{
			break;
		}
	} while (0);

	__UpdateUI();
}

//---------------------------------------------------
/**
\brief		Exposure time edit box loses focus
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnEnKillfocusEditExposure()
{
    // TODO:  Add your control notification handler code here
	CString strExposureTime = "";

	do 
	{
		GetDlgItem(IDC_EDIT_EXPOSURE)->GetWindowTextA(strExposureTime);
		if(DEVICE_SUCCESS != m_pCamDevice->SetExposureTimeToDevice(atoi(strExposureTime)))
		{
			__UpdateMessageBox("Setting exposure time failed");
			break;
		}

		if(DEVICE_SUCCESS != __DisplayExposureTime())
		{
			break;
		}
	} while (0); 
}

//---------------------------------------------------
/**
\brief		Gain edit box loses focus
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnEnKillfocusEditGain()
{
    // TODO: Add your control notification handler code here
	CString strGain = "";

	do 
	{
		GetDlgItem(IDC_EDIT_GAIN)->GetWindowTextA(strGain);
		if(DEVICE_SUCCESS != m_pCamDevice->SetGainToDevice(atoi(strGain)))
		{
			__UpdateMessageBox("Setting gain failed");
			break;
		}

		if(DEVICE_SUCCESS != __DisplayGain())
		{
			break;
		}
	} while (0); 
}

//---------------------------------------------------
/**
\brief		Select the value in the white balance drop-down box
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnCbnSelchangeComboWhiteBalance()
{
	// TODO: Add your control notification handler code here
	long lWhiteBalance = 0;

	do 
	{
		lWhiteBalance = ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->GetCurSel();
		if(DEVICE_SUCCESS != m_pCamDevice->SetWhiteBalanceToDevice(lWhiteBalance))
		{
			__UpdateMessageBox("Setting white balance failed");
			break;
		}

		if(DEVICE_SUCCESS != __DisplayWhiteBalance())
		{
			break;
		}
	} while (0);
}

//---------------------------------------------------
/**
\brief		Click the Filter property page button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenFilterPage()
{
	// TODO: Add your control notification handler code here
	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->GetFilterProperty(this->m_hWnd))
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayExposureTime())
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayGain())
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayWhiteBalance())
		{
			break;
		}
	} while (0); 
}

//---------------------------------------------------
/**
\brief		Click the Pin property page button
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedButtonOpenPinPage()
{
	// TODO: Add your control notification handler code here
	do 
	{
		if(DEVICE_SUCCESS != m_pCamDevice->GetPinProperty(this->m_hWnd))
		{
			break;
		}

        if(DEVICE_SUCCESS != __DisplayExposureTime())
		{
            break;
        }

		if(DEVICE_SUCCESS != __DisplayGain())
		{
			break;
		}

		if(DEVICE_SUCCESS != __DisplayWhiteBalance())
		{
			break;
		}
	} while (0);  
}

//---------------------------------------------------
/**
\brief		Click the save image control
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnBnClickedCheckSavePicture()
{
    // TODO: Add your control notification handler code here
	bool          bFlg        = false;
	DEVICE_STATUS emStatus;
	string        strSavePath = SAVE_IMAGE_PATH;

	if (1 == ((CButton *)GetDlgItem(IDC_CHECK_SAVE_PICTURE))->GetCheck())
	{
		bFlg = true;
	}
	
	CreateDirectory(strSavePath.c_str(),NULL);

	emStatus = m_pCamDevice->SaveImage(bFlg);
	if (emStatus != DEVICE_SUCCESS)
	{
		return;
	}
}

//---------------------------------------------------
/**
\brief		Preprocess message
*/
//----------------------------------------------------
BOOL CDXSingleDeviceSampleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CString strText = "";
	CWnd *pWnd = GetFocus();
	int  nStart = 0;
	int  nEnd = 0;
	bool bIsSelectAll = false;

	if(pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	if(    ((pWnd == (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE)) 
		|| (pWnd == (CEdit *)GetDlgItem(IDC_EDIT_GAIN))) 
		&& (pMsg->wParam == VK_RETURN))
	{
		SetFocus();
		return TRUE;
	}

	if((pWnd == (CEdit*)GetDlgItem(IDC_EDIT_EXPOSURE)) && (pMsg->message == WM_CHAR))
	{
		((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE))->GetWindowTextA(strText);
		((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE))->GetSel(nStart, nEnd);
		if ((nStart == 0) && (((nEnd == 0) && (strText.Find('-') == -1)) || (nStart != nEnd)))
		{
			bIsSelectAll = true;
		}
		return !__IsNumber(pMsg, strText, bIsSelectAll);
	}

	if((pWnd == (CEdit*)GetDlgItem(IDC_EDIT_GAIN)) && (pMsg->message == WM_CHAR))
	{
		((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->GetWindowTextA(strText);
		((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->GetSel(nStart, nEnd);
		if ((nStart == 0) && (((nEnd == 0) && (strText.Find('-') == -1)) || (nStart != nEnd)))
		{
			bIsSelectAll = true;
		}
		return !__IsNumber(pMsg, strText, bIsSelectAll);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//---------------------------------------------------
/**
\brief Bullet box error message
\param strError Enter error message
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateMessageBox(std::string strError)
{
	MessageBox(strError.c_str());
}

//---------------------------------------------------
/**
\brief		Update control enable status
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateUI(void)
{
	bool bOpenFlag          = false;
	bool bAcquisitionFlag   = false;
	bool bColorFlag	        = false;
	std::string strExposureTime = "";
	std::string strGainName     = "";

	bOpenFlag = m_pCamDevice->GetOpenFlag();
	bAcquisitionFlag = m_pCamDevice->GetAcquisitionFlag();
	bColorFlag = m_pCamDevice->GetColorFlag();	

	GetDlgItem(IDC_BUTTON_OPEN_DEVICE)->EnableWindow(!bOpenFlag);
	GetDlgItem(IDC_BUTTON_CLOSE_DEVICE)->EnableWindow(bOpenFlag);
	GetDlgItem(IDC_BUTTON_START_ACQUISITION)->EnableWindow(bOpenFlag && !bAcquisitionFlag);
	GetDlgItem(IDC_BUTTON_STOP_ACQUISITION)->EnableWindow(bOpenFlag && bAcquisitionFlag);
	GetDlgItem(IDC_EDIT_EXPOSURE)->EnableWindow(bOpenFlag);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(bOpenFlag);
	GetDlgItem(IDC_COMBO_WHITE_BALANCE)->EnableWindow(bColorFlag);
	GetDlgItem(IDC_BUTTON_OPEN_FILTER_PAGE)->EnableWindow(bOpenFlag);
	GetDlgItem(IDC_BUTTON_OPEN_PIN_PAGE)->EnableWindow(bOpenFlag);
	GetDlgItem(IDC_CHECK_SAVE_PICTURE)->EnableWindow(bOpenFlag);

	if(!bOpenFlag)
	{
		strExposureTime = "Exposure Time:";
		__UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE), strExposureTime);
		strGainName = "Gain:";
		__UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_GAIN), strGainName);

		((CButton *)GetDlgItem(IDC_CHECK_SAVE_PICTURE))->SetCheck(0);
	}

	if(!bOpenFlag && !bAcquisitionFlag)
	{
		((CStatic*)GetDlgItem(IDC_PICTURE_CONTROL))->SetBitmap(NULL);
		((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE))->SetWindowTextA("");
		((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetWindowTextA("");
		((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->SetCurSel(-1);

		RedrawWindow();
	}
}

//---------------------------------------------------
/**
\brief  Display exposure time parameter
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDXSingleDeviceSampleDlg::__DisplayExposureTime(void)
{
	long                        lExposureTime   = 0;
	std::string                 strExposureName = "";
	DEVICE_STATUS               emStatus        = DEVICE_SUCCESS;
	DEVICE_EXPOSURE_TIME_RANGE  stExposureTimeRange;

	memset(&stExposureTimeRange, 0, sizeof(DEVICE_EXPOSURE_TIME_RANGE));

	do 
	{
		emStatus = m_pCamDevice->GetExposureTimeRangeFromDevice(stExposureTimeRange);
		if(DEVICE_SUCCESS != emStatus)
		{
			__UpdateMessageBox("Failed to get exposure time range");
			break;
		}

		emStatus = m_pCamDevice->GetExposureTimeFromDevice(lExposureTime);
		if(DEVICE_SUCCESS != emStatus)
		{
			__UpdateMessageBox("Failed to get exposure time");
			break;
		}

		strExposureName = "Exposure Time(" + __LongToString(stExposureTimeRange.lExposureTimeMin) + "~" 
			              + __LongToString(stExposureTimeRange.lExposureTimeMax) + ")us:";
		__UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE), strExposureName);
		__UpdateExposureTimeParam(m_pCamDevice->GetExposureTime());
	} while (0);

	return emStatus;
}

//---------------------------------------------------
/**
\brief  Display gain parameter
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDXSingleDeviceSampleDlg::__DisplayGain(void)
{
	long                lGain       = 0;
	std::string         strGainName = "";
	DEVICE_STATUS       emStatus    = DEVICE_SUCCESS;
	DEVICE_GAIN_RANGE   stGainRange;

	memset(&stGainRange, 0, sizeof(DEVICE_GAIN_RANGE));

	do 
	{
		emStatus = m_pCamDevice->GetGainRangeFromDevice(stGainRange);
		if(emStatus != DEVICE_SUCCESS)
		{
			__UpdateMessageBox("Failed to get gain range");
			break;
		}

		emStatus = m_pCamDevice->GetGainFromDevice(lGain);
		if(emStatus != DEVICE_SUCCESS)
		{
			__UpdateMessageBox("Failed to get gain");
			break;
		}

		strGainName = "Gain(" + __LongToString(stGainRange.lGainMin) 
			        + "~" + __LongToString(stGainRange.lGainMax) + ")dB:";
		
		__UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_GAIN), strGainName);
		__UpdateGainParam(m_pCamDevice->GetGain());
	} while (0);

	return emStatus;
}

//---------------------------------------------------
/**
\brief  Display white balance parameters
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDXSingleDeviceSampleDlg::__DisplayWhiteBalance(void)
{
	long                        lWhiteBalance   = 0;
	bool                        bColorFlag      = false;
	DEVICE_STATUS               emStatus        = DEVICE_SUCCESS;
	DEVICE_WHITE_BALANCE_RANGE  stWhiteBalanceRange;

	memset(&stWhiteBalanceRange, 0, sizeof(DEVICE_WHITE_BALANCE_RANGE));

	do 
	{
		emStatus = m_pCamDevice->GetColorFromDevice(bColorFlag);
		if (emStatus != DEVICE_SUCCESS)
		{
			 __UpdateMessageBox("Failed to get black and white camera flag");
			break;
		}

		if (bColorFlag == false)
		{
			__UpdateWhiteBalanceEnable(bColorFlag);
			break;
		}

		emStatus = m_pCamDevice->GetWhiteBalanceRangeFromDevice(stWhiteBalanceRange);
		if(emStatus != DEVICE_SUCCESS)
		{
			__UpdateMessageBox("Failed to get white balance range");
			break;
		}
		__UpdateWhiteBalanceRange(stWhiteBalanceRange);

		emStatus = m_pCamDevice->GetWhiteBalanceFromDevice(lWhiteBalance);
		if(emStatus != DEVICE_SUCCESS)
		{
			__UpdateMessageBox("Failed to get white balance");
			break;
		}

		__UpdateWhiteBalanceParam(m_pCamDevice->GetWhiteBalance());
	} while (0);

	return emStatus;
}

//---------------------------------------------------
/**
\brief Update current parameters in the exposure time control
\param lExposureTime Enter the exposure time to display
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateExposureTimeParam(long lExposureTime)
{
	((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE))->SetWindowTextA(__LongToString(lExposureTime).c_str());
}

//---------------------------------------------------
/**
\brief Update the current parameters in the gain control
\param lGain Enter the gain value to display
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateGainParam(long lGain)
{
	((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetWindowTextA(__LongToString(lGain).c_str());
}

//---------------------------------------------------
/**
\brief Update the current parameters in the white balance control
\param lWhiteBalance Enter the white balance value to display
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateWhiteBalanceParam(long lWhiteBalance)
{
	((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->SetCurSel(lWhiteBalance);
}

//---------------------------------------------------
/**
\brief Update the range of parameters displayed on the control
\param pDlgItem     Input control handle
\param strDlgName   Enter what to display
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateParamRange(CStatic *pDlgItem, std::string strDlgName)
{
	pDlgItem->SetWindowTextA(strDlgName.c_str());
}

//---------------------------------------------------
/**
\brief Update the value displayed in the white balance drop-down box
\param stWhiteBalances Enter the white balance range value
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateWhiteBalanceRange(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange)
{
	int nCount = ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->GetCount();
	for(int i = nCount - 1; i >= 0; i--)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->DeleteString(i);
	}

	for(int i = stWhiteBalanceRange.lWhiteBalanceMin; 
		i <= stWhiteBalanceRange.lWhiteBalanceMax; 
		i = i + stWhiteBalanceRange.lWhiteBalanceStep)
	{
		switch(i)
		{
		case 0:
			((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->InsertString(i, "Off");
			continue;
		case 1:
			((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->InsertString(i, "Continuous");
			continue;
		case 2:
			((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->InsertString(i, "Once");
			continue;
		default:
			continue;
		}
	}
}

//---------------------------------------------------
/**
\brief Update the white balance control enable
\param bEnable Input control enable state
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateWhiteBalanceEnable(bool bEnable)
{
	if(bEnable == false)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->SetCurSel(-1);
	}

	GetDlgItem(IDC_COMBO_WHITE_BALANCE)->EnableWindow(bEnable);
	RedrawWindow();
}


//---------------------------------------------------
/**
\brief Update the controlled Device
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::__UpdateDevice()
{
	std::string    strDevName       = "";
	CDeviceManager objDeviceManager;

	int nDevNum = objDeviceManager.EnumDevice();
	for(int i = 0;i < nDevNum;i++)
	{
		if(DEVICE_SUCCESS == objDeviceManager.GetDeviceInfo(i, strDevName))
		{
			m_nDeviceID = i;
			break;
		}
	}
}

//---------------------------------------------------
/**
\brief  Determine if the current input is a number
\param  pMsg Enter keyboard input message
\return BOOL  
*/
//----------------------------------------------------
BOOL CDXSingleDeviceSampleDlg::__IsNumber(MSG* pMsg, const CString strText, bool bIsSelectAll)
{
	BOOL bRet = TRUE;

	do
	{
		if ((pMsg->wParam == '-') && ((strText.GetLength() == 0) || bIsSelectAll))
		{
			break;
		}

		if((pMsg->wParam == '.') && (strText.Find('.') == -1) && (strText.GetLength() != 0))
		{
			break;
		}

		if((pMsg->wParam <= '9') && (pMsg->wParam >= '0')) 
		{
			break;
		}

		if(pMsg->wParam == VK_BACK)
		{
			break;
		}

		bRet = FALSE;
	} while (0);

	return bRet;
}

//---------------------------------------------------
/**
\brief  Long type to string type
\param  lTemp   Enter the long value to convert
\return string  Converted string type
*/
//----------------------------------------------------
std::string CDXSingleDeviceSampleDlg::__LongToString(long lTemp)
{
	std::ostringstream objOutStream;  
	objOutStream << lTemp;  
	std::string strRet;  
	std::istringstream objInStream(objOutStream.str());  
	objInStream >> strRet;  
	return strRet; 
}


void CDXSingleDeviceSampleDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

//---------------------------------------------------
/**
\brief  Close the program
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	KillTimer(0);

	if (NULL != m_pCamDevice)
	{
		delete m_pCamDevice;
		m_pCamDevice = NULL;
	}

	CDialog::OnClose();
}

//---------------------------------------------------
/**
\brief  Timer refreshes the White Balance
*/
//----------------------------------------------------
void CDXSingleDeviceSampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	long lWhiteBalance = -1;

	do
	{
		if (NULL == m_pCamDevice)
		{
			break;
		}

		if (!m_pCamDevice->GetColorFlag())
		{
			break;
		}

		if(DEVICE_SUCCESS != m_pCamDevice->GetWhiteBalanceFromDevice(lWhiteBalance))
		{
			break;
		}

		if (lWhiteBalance != 0)
		{
			break;
		}

		__UpdateWhiteBalanceParam(lWhiteBalance);		
	} 
	while (0);

	CDialog::OnTimer(nIDEvent);
}
