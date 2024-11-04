
// DxMultipleDevicesSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DxMultipleDevicesSample.h"
#include "DxMultipleDevicesSampleDlg.h"

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


// CDxMultipleDevicesSampleDlg dialog



//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CDxMultipleDevicesSampleDlg::CDxMultipleDevicesSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxMultipleDevicesSampleDlg::IDD, pParent)
    , m_nSelectID(DEVICE_ID_INVAILD)
    , m_nSelectDeviceID(DEVICE_ID_INVAILD)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CDxMultipleDevicesSampleDlg::~CDxMultipleDevicesSampleDlg()
{
    map<int, CDevice *>::iterator iter;  
    for(iter = m_mapDevice.begin(); iter != m_mapDevice.end(); iter++)
    {
        delete iter->second;
        iter->second = NULL;
    }

    m_mapDevice.clear(); 
}

void CDxMultipleDevicesSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDxMultipleDevicesSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CBN_SELCHANGE(IDC_COMBO_ENUM_DEVICE, &CDxMultipleDevicesSampleDlg::OnCbnSelchangeComboEnumDevice)
    ON_BN_CLICKED(IDC_BUTTON_OPEN_DEVICE, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonOpenDevice)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEVICE, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonCloseDevice)
    ON_BN_CLICKED(IDC_BUTTON_START_ACQUISITION, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonStartAcquisition)
    ON_BN_CLICKED(IDC_BUTTON_STOP_ACQUISITION, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonStopAcquisition)
    ON_EN_KILLFOCUS(IDC_EDIT_EXPOSURE_TIME, &CDxMultipleDevicesSampleDlg::OnEnKillfocusEditExposureTime)
    ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CDxMultipleDevicesSampleDlg::OnEnKillfocusEditGain)
    ON_CBN_SELCHANGE(IDC_COMBO_WHITE_BALANCE, &CDxMultipleDevicesSampleDlg::OnCbnSelchangeComboWhiteBalance)
    ON_BN_CLICKED(IDC_BUTTON_FILTER_PROPERTY, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonFilterProperty)
    ON_BN_CLICKED(IDC_BUTTON_PIN_PROPERTY, &CDxMultipleDevicesSampleDlg::OnBnClickedButtonPinProperty)
    ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDxMultipleDevicesSampleDlg message handlers

BOOL CDxMultipleDevicesSampleDlg::OnInitDialog()
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
	SetTimer(0,1000,NULL);    //set timer
    __UpdateUI();
    __UpdateEnumDeviceComboBox();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDxMultipleDevicesSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDxMultipleDevicesSampleDlg::OnPaint()
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
HCURSOR CDxMultipleDevicesSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//---------------------------------------------------
/**
\brief Switch the selected drop-down box
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnCbnSelchangeComboEnumDevice()
{
    // TODO: Add your control notification handler code here
    int                           nCnt      = 0;
    map<int, CDevice *>::iterator iter;

    m_nSelectID = ((CComboBox *)GetDlgItem(IDC_COMBO_ENUM_DEVICE))->GetCurSel();
    
    for(iter = m_mapDevice.begin(); iter != m_mapDevice.end(); iter++)
    {
        if (nCnt == m_nSelectID)
        {
            m_nSelectDeviceID = iter->first;
            break;
        }
        nCnt++;
    }

    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(false == m_mapDevice[m_nSelectDeviceID]->GetOpenFlag())
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
\brief Click to open the device button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonOpenDevice()
{
    // TODO: Add your control notification handler code here

	//Prevent a repeat entry before the first one is completed
	GetDlgItem(IDC_BUTTON_OPEN_DEVICE)->EnableWindow(FALSE);

    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }
         
        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->OpenDevice(m_nSelectDeviceID))
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
\brief Click the close device button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonCloseDevice()
{
    // TODO: Add your control notification handler code here
    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->CloseDevice())
        {
            break;
        }
    } while (0);

    __UpdateUI();  
}

//---------------------------------------------------
/**
\brief Click the Start Acquisition button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonStartAcquisition()
{
    // TODO: Add your control notification handler code here
    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->StartAcquisition(GetDlgItem(IDC_STATIC_DEVICE_FIRST + m_nSelectID)))
        {
            break;
        }
    } while (0);

    __UpdateUI();
}

//---------------------------------------------------
/**
\brief Click to stop collecting button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonStopAcquisition()
{
    // TODO: Add your control notification handler code here
    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->StopAcquisition())
        {
            break;
        }
    } while (0);

    __UpdateUI();
}

//---------------------------------------------------
/**
\brief Exposure time edit box loses focus
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnEnKillfocusEditExposureTime()
{
    // TODO: Add your control notification handler code here
    CString strExposureTime = "";

    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->GetWindowTextA(strExposureTime);
        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->SetExposureTimeToDevice(atoi(strExposureTime)))
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
\brief Gain edit box loses focus
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnEnKillfocusEditGain()
{
    // TODO: Add your control notification handler code here
    CString strGain = "";

    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        GetDlgItem(IDC_EDIT_GAIN)->GetWindowTextA(strGain);
        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->SetGainToDevice(atoi(strGain)))
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
\brief Select the value in the white balance drop-down box
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnCbnSelchangeComboWhiteBalance()
{
    // TODO: Add your control notification handler code here
    long lWhiteBalance = 0;

    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        lWhiteBalance = ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->GetCurSel();
        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->SetWhiteBalanceToDevice(lWhiteBalance))
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
\brief Click the Filter property page button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonFilterProperty()
{
    // TODO: Add your control notification handler code here
    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->GetFilterProperty(this->m_hWnd))
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
\brief Click the Pin property page button
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnBnClickedButtonPinProperty()
{
    // TODO: Add your control notification handler code here
    do 
    {
        if(false == __CheckDeviceID())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->GetPinProperty(this->m_hWnd))
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
\brief Preprocess message
*/
//----------------------------------------------------
BOOL CDxMultipleDevicesSampleDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    CString strText = "";
    CWnd *pWnd  = GetFocus();
	int  nStart = 0;
	int  nEnd   = 0;
	bool bIsSelectAll = false;

    if(pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }

    if(    ((pWnd == (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME)) 
        || (pWnd == (CEdit *)GetDlgItem(IDC_EDIT_GAIN))) 
        && (pMsg->wParam == VK_RETURN))
    {
        SetFocus();
        return TRUE;
    }

    if((pWnd == (CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME)) && (pMsg->message == WM_CHAR))
    {
        ((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME))->GetWindowTextA(strText);
		((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME))->GetSel(nStart, nEnd);
		if ((nStart == 0) && (((nEnd == 0) && (strText.Find('-') == -1)) || (nStart != nEnd)))
		{
			bIsSelectAll = true;
		}
		return !__IsNumber(pMsg, strText, bIsSelectAll);
    }

    if((pWnd == (CEdit *)GetDlgItem(IDC_EDIT_GAIN)) && (pMsg->message == WM_CHAR))
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

void CDxMultipleDevicesSampleDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialog::OnOK();
}

//---------------------------------------------------
/**
\brief Update the contents of the enumeration device drop-down box
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateEnumDeviceComboBox()
{
    int            nCnt             = 0;
    CDevice        *pDevice         = NULL;
    std::string    strDevName       = "";
    CDeviceManager objDeviceManager;

    int nDevNum = objDeviceManager.EnumDevice();
    for(int i = 0;i < nDevNum;i++)
    {
        if(DEVICE_SUCCESS != objDeviceManager.GetDeviceInfo(i, strDevName))
        {
            continue;
        }  

        pDevice = new(std::nothrow) CDevice;
        if(pDevice == NULL)
        {
            continue;
        }

        if (nCnt < 4)
        {
            m_mapDevice.insert(pair<int, CDevice *>(i, pDevice));
            ((CComboBox *)GetDlgItem(IDC_COMBO_ENUM_DEVICE))->AddString(strDevName.c_str());
            nCnt++;
        }
        else
        {
            break;
        }        
    }

    if(m_mapDevice.size() > 0)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_ENUM_DEVICE))->SetCurSel(0);
        m_nSelectID = 0;
        m_nSelectDeviceID = m_mapDevice.begin()->first;
    }
}

//---------------------------------------------------
/**
\brief Bullet box error message
\param strError Enter error message
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateMessageBox(std::string strError)
{
    MessageBox(strError.c_str());
}

//---------------------------------------------------
/**
\brief Update control enable status
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateUI()
{
    bool bOpenFlag          = false;
    bool bAcquisitionFlag   = false;
    bool bColorFlag         = false;
    std::string strExposureTime = "";
    std::string strGainName     = "";

    if(m_nSelectDeviceID > DEVICE_ID_INVAILD)
    {
        bOpenFlag = m_mapDevice[m_nSelectDeviceID]->GetOpenFlag();
        bAcquisitionFlag = m_mapDevice[m_nSelectDeviceID]->GetAcquisitionFlag();
        bColorFlag = m_mapDevice[m_nSelectDeviceID]->GetColorFlag();
    }

    GetDlgItem(IDC_BUTTON_OPEN_DEVICE)->EnableWindow(!bOpenFlag);
    GetDlgItem(IDC_BUTTON_CLOSE_DEVICE)->EnableWindow(bOpenFlag);
    GetDlgItem(IDC_BUTTON_START_ACQUISITION)->EnableWindow(bOpenFlag && !bAcquisitionFlag);
    GetDlgItem(IDC_BUTTON_STOP_ACQUISITION)->EnableWindow(bOpenFlag && bAcquisitionFlag);
    GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->EnableWindow(bOpenFlag);
    GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(bOpenFlag);
    GetDlgItem(IDC_COMBO_WHITE_BALANCE)->EnableWindow(bColorFlag);
    GetDlgItem(IDC_BUTTON_FILTER_PROPERTY)->EnableWindow(bOpenFlag);
    GetDlgItem(IDC_BUTTON_PIN_PROPERTY)->EnableWindow(bOpenFlag);

    if(!bOpenFlag)
    {
        strExposureTime = "Exposure Time:";
        __UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME), strExposureTime);
        strGainName = "Gain:";
        __UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_GAIN), strGainName);
    }

    if(!bOpenFlag && !bAcquisitionFlag)
    {
        switch(m_nSelectID)
        {
        case 0:
            ((CStatic*)GetDlgItem(IDC_STATIC_DEVICE_FIRST))->SetBitmap(NULL);
            break;
        case 1:
            ((CStatic*)GetDlgItem(IDC_STATIC_DEVICE_SECOND))->SetBitmap(NULL);
            break;
        case 2:
            ((CStatic*)GetDlgItem(IDC_STATIC_DEVICE_THIRD))->SetBitmap(NULL);
            break;
        case 3:
            ((CStatic*)GetDlgItem(IDC_STATIC_DEVICE_FOURTH))->SetBitmap(NULL);
            break;
        default:
            break;
        }

        ((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME))->SetWindowTextA("");
        ((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetWindowTextA("");
        ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->SetCurSel(-1);

        RedrawWindow();
    }
}

//---------------------------------------------------
/**
\brief Update the range of parameters displayed on the control
\param pDlgItem     Input control handle
\param strDlgName   Enter what to display
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateParamRange(CStatic *pDlgItem, std::string strDlgName)
{
    pDlgItem->SetWindowTextA(strDlgName.c_str());
}

//---------------------------------------------------
/**
\brief Update current parameters in the exposure time control
\param lExposureTime Enter the exposure time to display
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateExposureTimeParam(long lExposureTime)
{
    ((CEdit *)GetDlgItem(IDC_EDIT_EXPOSURE_TIME))->SetWindowTextA(__LongToString(lExposureTime).c_str());
}

//---------------------------------------------------
/**
\brief Update the current parameters in the gain control
\param lGain Enter the gain value to display
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateGainParam(long lGain)
{
    ((CEdit *)GetDlgItem(IDC_EDIT_GAIN))->SetWindowTextA(__LongToString(lGain).c_str());
}

//---------------------------------------------------
/**
\brief Update the current parameters in the white balance control
\param lWhiteBalance Enter the white balance value to display
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateWhiteBalanceParam(long lWhiteBalance)
{
    ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->SetCurSel(lWhiteBalance);
}

//---------------------------------------------------
/**
\brief Update the white balance control enable
\param bEnable Input control enable state
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateWhiteBalanceEnable(bool bEnable)
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
\brief Update the value displayed in the white balance drop-down box
\param stWhiteBalances Enter the white balance range value
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::__UpdateWhiteBalanceRange(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange)
{
    int nCount = ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->GetCount();
    for(int i = nCount - 1; i >= 0; i--)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_WHITE_BALANCE))->DeleteString(i);
    }

    for(int i = stWhiteBalanceRange.lWhiteBalanceMin; i <= stWhiteBalanceRange.lWhiteBalanceMax; i = i + stWhiteBalanceRange.lWhiteBalanceStep)
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
\brief  Display exposure time parameter
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDxMultipleDevicesSampleDlg::__DisplayExposureTime()
{
    long                        lExposureTime   = 0;
    std::string                 strExposureName = "";
    DEVICE_STATUS               emStatus        = DEVICE_SUCCESS;
    DEVICE_EXPOSURE_TIME_RANGE  stExposureTimeRange;

    memset(&stExposureTimeRange, 0, sizeof(DEVICE_EXPOSURE_TIME_RANGE));

    do 
    {
        emStatus = m_mapDevice[m_nSelectDeviceID]->GetExposureTimeRangeFromDevice(stExposureTimeRange);
        if(DEVICE_SUCCESS != emStatus)
        {
            __UpdateMessageBox("Failed to get exposure time range");
            break;
        }

        emStatus = m_mapDevice[m_nSelectDeviceID]->GetExposureTimeFromDevice(lExposureTime);
        if(DEVICE_SUCCESS != emStatus)
        {
            __UpdateMessageBox("Failed to get exposure time");
            break;
        }

        strExposureName = "Exposure Time(" + __LongToString(stExposureTimeRange.lExposureTimeMin) + "~" + __LongToString(stExposureTimeRange.lExposureTimeMax) + ")us:";
        __UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_EXPOSURE_TIME), strExposureName);
        __UpdateExposureTimeParam(m_mapDevice[m_nSelectDeviceID]->GetExposureTime());
    } while (0);

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Display gain parameter
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDxMultipleDevicesSampleDlg::__DisplayGain()
{
    long                    lGain       = 0;
    std::string             strGainName = "";
    DEVICE_STATUS           emStatus    = DEVICE_SUCCESS;
    DEVICE_GAIN_RANGE       stGainRange;

    memset(&stGainRange, 0, sizeof(DEVICE_GAIN_RANGE));

    do 
    {
        emStatus = m_mapDevice[m_nSelectDeviceID]->GetGainRangeFromDevice(stGainRange);
        if(emStatus != DEVICE_SUCCESS)
        {
            __UpdateMessageBox("Failed to get gain range");
            break;
        }

        emStatus = m_mapDevice[m_nSelectDeviceID]->GetGainFromDevice(lGain);
        if(emStatus != DEVICE_SUCCESS)
        {
            __UpdateMessageBox("Failed to get gain");
            break;
        }

        strGainName = "Gain(" + __LongToString(stGainRange.lGainMin) + "~" + __LongToString(stGainRange.lGainMax) + ")dB:";
        __UpdateParamRange((CStatic *)GetDlgItem(IDC_STATIC_GAIN), strGainName);
        __UpdateGainParam(m_mapDevice[m_nSelectDeviceID]->GetGain());
    } while (0);

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Display white balance parameters
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDxMultipleDevicesSampleDlg::__DisplayWhiteBalance()
{
    long                        lWhiteBalance   = 0;
    bool                        bColorFlag      = false;
    DEVICE_STATUS               emStatus        = DEVICE_SUCCESS;
    DEVICE_WHITE_BALANCE_RANGE  stWhiteBalanceRange;

    memset(&stWhiteBalanceRange, 0, sizeof(DEVICE_WHITE_BALANCE_RANGE));
    
    do 
    {
        emStatus = m_mapDevice[m_nSelectDeviceID]->GetColorFromDevice(bColorFlag);
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

        emStatus = m_mapDevice[m_nSelectDeviceID]->GetWhiteBalanceRangeFromDevice(stWhiteBalanceRange);
        if(emStatus != DEVICE_SUCCESS)
        {
            __UpdateMessageBox("Failed to get white balance range");
            break;
        }
        __UpdateWhiteBalanceRange(stWhiteBalanceRange);

        emStatus = m_mapDevice[m_nSelectDeviceID]->GetWhiteBalanceFromDevice(lWhiteBalance);
        if(emStatus != DEVICE_SUCCESS)
        {
            __UpdateMessageBox("Failed to get white balance");
            break;
        }

        __UpdateWhiteBalanceParam(m_mapDevice[m_nSelectDeviceID]->GetWhiteBalance());
    } while (0);

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Long type to string type
\param  lTemp   Enter the long value to convert
\return string  Converted string type
*/
//----------------------------------------------------
std::string CDxMultipleDevicesSampleDlg::__LongToString(long lTemp)
{
    std::ostringstream objOutStream;  
    objOutStream << lTemp;  
    std::string strRet;  
    std::istringstream objInStream(objOutStream.str());  
    objInStream >> strRet;  
    return strRet; 
}

//---------------------------------------------------
/**
\brief  Determine whether the current device ID is legal
\return bool 
*/
//----------------------------------------------------
bool CDxMultipleDevicesSampleDlg::__CheckDeviceID()
{
    //If the DX component is not registered, the enumerated DX device is zero, 
    //and the current value in the enumeration drop-down box is -1, that is, the selected device ID is -1, 
    //to determine the correctness.
    if(((size_t)m_nSelectID >= m_mapDevice.size()) || (m_nSelectID <= DEVICE_ID_INVAILD))
    {
        return false;
    }

    return true;
}

//---------------------------------------------------
/**
\brief  Determine if the current input is a number
\param  pMsg Enter keyboard input message
\return BOOL  
*/
//----------------------------------------------------
BOOL CDxMultipleDevicesSampleDlg::__IsNumber(MSG* pMsg, const CString strText, bool bIsSelectAll)
{
    BOOL bRet = TRUE;

    do 
    {
        if((pMsg->wParam == '-') && ((strText.GetLength() == 0) || bIsSelectAll))
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
\brief  Close the program
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    map<int, CDevice *>::iterator iter;

	KillTimer(0);

    for(iter = m_mapDevice.begin(); iter != m_mapDevice.end(); iter++)
    {
        delete iter->second;
        iter->second = NULL;
    }

    m_mapDevice.clear(); 

    CDialog::OnClose();
}

//---------------------------------------------------
/**
\brief  Timer refreshes the White Balance
*/
//----------------------------------------------------
void CDxMultipleDevicesSampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
    long lWhiteBalance = -1;

    do
    {
		if (NULL == m_mapDevice[m_nSelectDeviceID])
		{
			break;
		}

        if (!m_mapDevice[m_nSelectDeviceID]->GetColorFlag())
        {
            break;
        }

        if(DEVICE_SUCCESS != m_mapDevice[m_nSelectDeviceID]->GetWhiteBalanceFromDevice(lWhiteBalance))
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
