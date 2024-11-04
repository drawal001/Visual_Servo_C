// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"
#include "ControlDlg.h"
#include "GxMultiCamForHDRDoc.h"

using namespace std;

// CControlDlg

IMPLEMENT_DYNCREATE(CControlDlg, CFormView)

CControlDlg::CControlDlg()
	: CFormView(CControlDlg::IDD)
	, m_bCheckShowDevice(FALSE)
{

	m_nDeviceListCurrent          = 0;
	m_pDeviceProcessCurrent       = NULL;

	for (int i = 0; i< DEVICE_CONTS; i++)
	{
		m_pDeviceProcess[i]          = NULL;
	}

}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboxDevice);
	DDX_Check(pDX, IDC_CHECK_SHOW_DEVICE_FPS, m_bCheckShowDevice);
}

BEGIN_MESSAGE_MAP(CControlDlg, CFormView)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CControlDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CControlDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CControlDlg::OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CControlDlg::OnBnClickedBtnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnSelchangeComboDeviceList)
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnDropdownComboDeviceList)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DEVICE_FPS, &CControlDlg::OnBnClickedCheckShowDeviceFps)
    ON_WM_TIMER()
	ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_CAMERA_PARAM, &CControlDlg::OnBnClickedButtonCameraParam)
END_MESSAGE_MAP()


// CControlDlg diagnostics

#ifdef _DEBUG
void CControlDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CControlDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	try
	{
		for (int i=0; i<DEVICE_CONTS; i++)
		{
			m_pDeviceProcess[i] = new CDeviceProcess;
		}
		//初始化设备库
		IGXFactory::GetInstance().Init();

		//枚举设备
		IGXFactory::GetInstance().UpdateDeviceList(1000,m_vectorDeviceInfo);

		//未检测到设备
		if (m_vectorDeviceInfo.size() <= 0)
		{
			MessageBox("未检测到设备,请确保设备正常连接后重启程序!");
            GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(false);
			GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(false);
            GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(false);
			GetDlgItem(IDC_BTN_CAMERA_PARAM)->EnableWindow(false);
			GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(false);
			CMainFrame* pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
			pMainFrm->m_objControlDlg = this;
			return;
		}

		//将设备名称显示到设备列表中
		for (uint32_t i =0; i< m_vectorDeviceInfo.size(); i++)
		{
			//如果设备数大于4则只保留4个设备信息
			if (i >= DEVICE_CONTS)
			{
				break;
			}

			gxstring strDeviceInformation = "";
			strDeviceInformation =m_vectorDeviceInfo[i].GetDisplayName();
			m_comboxDevice.SetItemData(m_comboxDevice.AddString(strDeviceInformation.c_str()),i);
			m_mapDeviceInformation.insert(map<int,CDeviceProcess*>::value_type(i,m_pDeviceProcess[i]));

			//更新界面
			__UpdateUI(m_pDeviceProcess[i]);

		}

		m_comboxDevice.SetCurSel(0);

		//获取当前显示的设备处理类对象
		m_pDeviceProcessCurrent = m_mapDeviceInformation[0];

		CMainFrame * pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
		pMainFrm->m_objControlDlg = this;
		
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
	// TODO: Add your specialized code here and/or call the base class
	
}

// CControlDlg message handlers

void CControlDlg::OnBnClickedBtnOpenDevice()
{
	// TODO: Add your control notification handler code here

    try
    {
		//获取显示区域子窗口句柄
		CWnd*  pWnd = NULL;
		CMainFrame * pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
		switch(m_nDeviceListCurrent)
		{
		case 0:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(0, 0);
			break;
		case 1:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(0, 1);
			break;
		case 2:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(1, 0);
			break;
		case 3:
			pWnd = pMainFrm->m_wndSplitterLeft.GetPane(1, 1);
			break;

		default: break;
		}

		//获取设备序号
		int nDeviceIndex = m_nDeviceListCurrent+1;

		//打开设备
		m_pDeviceProcessCurrent->OpenDevice(m_vectorDeviceInfo[m_nDeviceListCurrent].GetSN(),pWnd,nDeviceIndex);

		//初始化界面
		__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

		//更新界面
		__UpdateUI(m_pDeviceProcessCurrent);

    }
    catch (CGalaxyException& e)
    {
		MessageBox(e.what());
		return;
    }
	catch (std::exception& e)
    {
		MessageBox(e.what());
		return;
    }
}
//----------------------------------------------------------------------
/**
\breaf  初始化界面

\param  objFeatureControlPtr[in]      属性控制器

\return void
**/
//----------------------------------------------------------------------
void CControlDlg::__InitUI(CGXFeatureControlPointer objFeatureControlPtr)
{
 
	if (objFeatureControlPtr.IsNull())
	{
		return;
	}

	//将当前设备是否显示帧率标志刷到界面
	m_bCheckShowDevice = m_pDeviceProcessCurrent->GetShowFPS();

	UpdateData(FALSE);

}

//----------------------------------------------------------------------
/**
\breaf  更新界面

\param  pDeviceProcess[in]      设备处理类对象

\return void
**/
//----------------------------------------------------------------------
void CControlDlg::__UpdateUI(CDeviceProcess* pDeviceProcess)
{
	if (pDeviceProcess == NULL)
	{
		return;
	}

	bool IsOpen = pDeviceProcess->IsOpen();
	bool IsSnap = pDeviceProcess->IsSnap();
	GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(!IsOpen);
	GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(IsOpen);
	GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(IsOpen&& !IsSnap);
	GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(IsOpen&& IsSnap);
    GetDlgItem(IDC_BTN_CAMERA_PARAM)->EnableWindow(IsOpen);
	GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(IsOpen);
}

void CControlDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//开始采集
		m_pDeviceProcessCurrent->StartSnap();

		//更新界面
		__UpdateUI(m_pDeviceProcessCurrent);

		//更新设备型号及帧率信息
		m_pDeviceProcessCurrent->RefreshDeviceSNFPS();

		SetTimer(1, 1000, NULL);
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnBnClickedBtnStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//停止采集
		m_pDeviceProcessCurrent->StopSnap();

		//更新界面
		__UpdateUI(m_pDeviceProcessCurrent);

		//关闭定时器1
		KillTimer(1);
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnBnClickedBtnCloseDevice()
{
	// TODO: Add your control notification handler code here
	try
	{
		//关闭设备
		m_pDeviceProcessCurrent->CloseDevice();
		

		//更新界面
		__UpdateUI(m_pDeviceProcessCurrent);
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
		return;
	}
}

void CControlDlg::OnCbnSelchangeComboDeviceList()
{
	// TODO: Add your control notification handler code here
	try
	{
		m_nDeviceListCurrent = m_comboxDevice.GetCurSel();

		m_pDeviceProcessCurrent = m_mapDeviceInformation[m_nDeviceListCurrent];

		//判断当前设备是否已打开
		bool bIsOpen = m_pDeviceProcessCurrent->IsOpen();
		if (!bIsOpen)
		{
			__UpdateUI(m_pDeviceProcessCurrent);
		}

		else
		{
			//刷新界面
			__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

			//更新界面
			__UpdateUI(m_pDeviceProcessCurrent);

		}
	}
	catch (CGalaxyException& e)
	{
		MessageBox(e.what());
		return;
		
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

void CControlDlg::OnBnClickedCheckShowDeviceFps()
{
	// TODO: Add your control notification handler code here
	try
	{
		UpdateData(TRUE);
		m_pDeviceProcessCurrent->SetShowFPS(m_bCheckShowDevice == TRUE);

		if (1 == ((CButton *)GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS))->GetCheck())
		{
			SetTimer(1, 1000, NULL);
		}
		else
		{
			KillTimer(1);
		}
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd  *pWnd   = NULL;
	int   nCtrlID = 0;             //< 保存获取的控件ID

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}
	return CFormView::PreTranslateMessage(pMsg);
}


void CControlDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    switch(nIDEvent)
    {
    case 1:
        //遍历已打开设备，设置刷新FPS操作
        for (map<int,CDeviceProcess*>::iterator it = m_mapDeviceInformation.begin(); 
            it != m_mapDeviceInformation.end(); ++it)
        {
            it->second->RefreshDeviceSNFPS();
        }
        break;

    default:
        break;
    }

    CFormView::OnTimer(nIDEvent);
}

void CControlDlg::OnCbnDropdownComboDeviceList()
{
	// TODO: Add your control notification handler code here
	CComboBox *pWnd = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE_LIST);
	int nNumEntries = pWnd->GetCount();
	int nWidth = 0;
	CString str;

	CClientDC dc(pWnd);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());

	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	for (int i = 0; i < nNumEntries; i++)
	{
		pWnd->GetLBText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = max(nWidth, nLength);
	}

	// Add margin space to the calculations
	nWidth += dc.GetTextExtent("0").cx;

	dc.RestoreDC(nSave);
	pWnd->SetDroppedWidth(nWidth);
}

void CControlDlg::OnDestroy()
{
	try
	{
		//遍历map容器，将已打开设备全部关闭
		map<int,CDeviceProcess*>::iterator it;
		for (it = m_mapDeviceInformation.begin(); it != m_mapDeviceInformation.end(); ++it)
		{
			it->second->CloseDevice();

		}

		m_mapDeviceInformation.clear();

	}
	catch (CGalaxyException)
	{
		//do noting

	}
	catch (std::exception)
	{
		//do noting
	}

	try
	{
		//释放设备库资源
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException)
	{
		//do noting
	}



	for (int i = 0; i <DEVICE_CONTS; i++)
	{
		if (m_pDeviceProcess[i] != NULL)
		{
			delete m_pDeviceProcess[i];
			m_pDeviceProcess[i] = NULL;
		}
	}
	CFormView::OnDestroy();

	// TODO: Add your message handler code here
}

void CControlDlg::OnBnClickedButtonCameraParam()
{
    // TODO: 在此添加控件通知处理程序代码
    CCamerParamDlg dlg(m_pDeviceProcessCurrent);
    dlg.DoModal();
}
