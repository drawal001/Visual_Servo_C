// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCam.h"
#include "ControlDlg.h"
#include "GxMultiCamDoc.h"

using namespace std;

// CControlDlg

IMPLEMENT_DYNCREATE(CControlDlg, CFormView)

CControlDlg::CControlDlg()
	: CFormView(CControlDlg::IDD)
	, m_dEditShutter(0)
	, m_dEditGain(0)
	, m_nEditSnapSpeed(0)
	, m_bCheckShowDevice(FALSE)
{
	m_bIsSnapSpeed                = false;
	m_bIsColorFilter              = false;
	m_dShutterMax                 = 0;
	m_dShutterMin                 = 0;
	m_dGainMax                    = 0;
	m_dGainMin                    = 0;
	m_nSnapSpeedMax               = 0;
	m_nSnapSpeedMin               = 0;
	m_nDeviceListCurrent          = 0;
	m_nCurrentBalanceAutoWhiteSel = 0;
	m_strBalanceWhiteAuto         = "Off";
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
	DDX_Control(pDX, IDC_COMBO_BALANCE_WHITE_AUTO, m_comboxBalanceWhiteAuto);
	DDX_Control(pDX, IDC_STATIC_SHUTTER_TIME, m_staticShutterTime);
	DDX_Control(pDX, IDC_STATIC_GAIN, m_staticGain);
	DDX_Control(pDX, IDC_STATIC_SNAP_SPEED, m_staticSnapSpeed);
	DDX_Control(pDX, IDC_EDIT_SHUTTER, m_editShutter);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_editGain);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dEditShutter);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_dEditGain);
	DDX_Text(pDX, IDC_EDIT_SNAP_SPEED, m_nEditSnapSpeed);
	DDX_Check(pDX, IDC_CHECK_SHOW_DEVICE_FPS, m_bCheckShowDevice);
}

BEGIN_MESSAGE_MAP(CControlDlg, CFormView)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CControlDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CControlDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CControlDlg::OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CControlDlg::OnBnClickedBtnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnSelchangeComboDeviceList)
	ON_CBN_SELCHANGE(IDC_COMBO_BALANCE_WHITE_AUTO, &CControlDlg::OnCbnSelchangeComboBalanceWhiteAuto)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, &CControlDlg::OnEnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CControlDlg::OnEnKillfocusEditGain)
	ON_EN_KILLFOCUS(IDC_EDIT_SNAP_SPEED, &CControlDlg::OnEnKillfocusEditSnapSpeed)
	ON_WM_TIMER()
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnDropdownComboDeviceList)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DEVICE_FPS, &CControlDlg::OnBnClickedCheckShowDeviceFps)
	ON_WM_DESTROY()
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
			GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_SNAP_SPEED)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(false);
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

		//设置定时器
		SetTimer(0,1000,NULL);
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
    
	bool bIsBalanceWhiteAutoRead = false;        // 自动白平衡是否可读
	bool bBalanceWhiteAuto       = false;        // 是否支持自动白平衡

	//是否支持Bayer格式
	m_bIsColorFilter = m_pDeviceProcessCurrent->GetIsColor();

	if (m_bIsColorFilter)
	{
		//初始化自动白平衡组合框
		//是否支持白平衡
		bBalanceWhiteAuto = objFeatureControlPtr->IsImplemented("BalanceWhiteAuto");

		//白平衡是否可读
		bIsBalanceWhiteAutoRead = objFeatureControlPtr->IsReadable("BalanceWhiteAuto");
		if (bBalanceWhiteAuto)
		{
			if (bIsBalanceWhiteAutoRead)
			{
				m_strBalanceWhiteAuto = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();
			}
			int nCursel  = 0;
			gxstring strCurEnumList = "";
			GxIAPICPP::gxstring_vector vectorEnumEntryList;

			//获取当前设备功能值
			strCurEnumList = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetValue();

			//获取设备所有枚举项
			vectorEnumEntryList = objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->GetEnumEntryList();

			//清空组合框列表
			m_comboxBalanceWhiteAuto.ResetContent();
			for (uint32_t i = 0; i<vectorEnumEntryList.size(); i++)
			{
				std::string strEnumList = vectorEnumEntryList[i].c_str();
				m_comboxBalanceWhiteAuto.SetItemData(m_comboxBalanceWhiteAuto.AddString(strEnumList.c_str()),i);

				if (strCurEnumList == vectorEnumEntryList[i])
				{
					nCursel = i;
				}

			}
			m_nCurrentBalanceAutoWhiteSel = nCursel;
			m_comboxBalanceWhiteAuto.SetCurSel(nCursel);
		}
	}
	

	//初始化曝光时间
	CString  strShutterTimeRange = "";          // 曝光时间范围
	gxstring strShutterTimeUint  = "";          // 曝光时间单位

	m_dEditShutter     = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();
	strShutterTimeUint = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetUnit();
	m_dShutterMax      = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();
	m_dShutterMin      = objFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();

	strShutterTimeRange.Format("曝光(%.4f~%.4f)%s",m_dShutterMin,m_dShutterMax,strShutterTimeUint.c_str());
	m_staticShutterTime.SetWindowText(strShutterTimeRange);

	//初始化增益
	CString  strGainRange = "";                // 增益值范围
	gxstring strGainUint  = "";                // 增益单位

	m_dEditGain   = objFeatureControlPtr->GetFloatFeature("Gain")->GetValue();
	strGainUint   = objFeatureControlPtr->GetFloatFeature("Gain")->GetUnit();
	m_dGainMax    = objFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
	m_dGainMin    = objFeatureControlPtr->GetFloatFeature("Gain")->GetMin();

	strGainRange.Format("增益(%.4f~%.4f)%s",m_dGainMin,m_dGainMax,strGainUint.c_str());
	m_staticGain.SetWindowText(strGainRange);

	//初始化采集速度级别
	//是否支持采集速度级别
	m_bIsSnapSpeed = objFeatureControlPtr->IsImplemented("AcquisitionSpeedLevel");
	if (m_bIsSnapSpeed)
	{
		CString  strSnapSpeedRange = "";  
		m_nEditSnapSpeed   = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetValue();
		m_nSnapSpeedMax    = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetMax();
		m_nSnapSpeedMin    = objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->GetMin();

		strSnapSpeedRange.Format("采集速度级别(%lld~%lld)",m_nSnapSpeedMin,m_nSnapSpeedMax);
		m_staticSnapSpeed.SetWindowText(strSnapSpeedRange);

	}
	else
	{
		//刷新编辑框
		m_nEditSnapSpeed = NULL;
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
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(IsOpen);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(IsOpen);
	GetDlgItem(IDC_EDIT_SNAP_SPEED)->EnableWindow(IsOpen&& !IsSnap&& m_bIsSnapSpeed);
	GetDlgItem(IDC_COMBO_BALANCE_WHITE_AUTO)->EnableWindow(IsOpen&& m_bIsColorFilter);
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

void CControlDlg::OnCbnSelchangeComboBalanceWhiteAuto()
{
	// TODO: Add your control notification handler code here
	try
	{
		
		int      nCurSelBalanceWhiteAuto = 0;      // 自动白平衡组合框当前值
		CString  strCurText              = "";     // 自动白平衡当前枚举项
		gxstring strCurEnumList          = "";     

		nCurSelBalanceWhiteAuto = m_comboxBalanceWhiteAuto.GetCurSel();
		m_comboxBalanceWhiteAuto.GetLBText(nCurSelBalanceWhiteAuto,strCurText);
		strCurEnumList = strCurText.GetBuffer(0);

		//记录当前白平衡模式
		m_strBalanceWhiteAuto = strCurEnumList;

		//将当前功能值设置到相应设备中
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(strCurEnumList); 

		//记录组合框当前值
		m_nCurrentBalanceAutoWhiteSel =nCurSelBalanceWhiteAuto;

	}
	catch (CGalaxyException& e)
	{
		m_comboxBalanceWhiteAuto.SetCurSel(m_nCurrentBalanceAutoWhiteSel);
		MessageBox(e.what());
		return;
	}
	catch (std::exception& e)
	{
		m_comboxBalanceWhiteAuto.SetCurSel(m_nCurrentBalanceAutoWhiteSel);
		MessageBox(e.what());
		return;
	}
}

void CControlDlg::OnEnKillfocusEditShutter()
{
	// TODO: Add your control notification handler code here
	double dShutterOld = m_dEditShutter;           // 记录曝光时间值
 	if (!m_pDeviceProcessCurrent->IsOpen())
	{
		return;
	}
	try
	{
		UpdateData(TRUE);

		//判断编辑框中输入的值是否在曝光时间范围内
		if (m_dEditShutter < m_dShutterMin  )
		{
			m_dEditShutter = m_dShutterMin;
		}

		if (m_dEditShutter > m_dShutterMax)
		{
			m_dEditShutter = m_dShutterMax;
		}

		//将此值设置到设备中
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(m_dEditShutter); 

	}
	catch (CGalaxyException& e)
	{
		m_dEditShutter = dShutterOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditShutter = dShutterOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CControlDlg::OnEnKillfocusEditGain()
{
	// TODO: Add your control notification handler code here
	double dGainOld = m_dEditGain;         // 记录增益值
	if (!m_pDeviceProcessCurrent->IsOpen())
	{
		return;
	}

	try
	{
		UpdateData(TRUE);

		//判断编辑框中输入的值是否在增益范围内
		if (m_dEditGain < m_dGainMin )
		{
			m_dEditGain = m_dGainMin;
		}

		if (m_dEditGain > m_dGainMax)
		{
			m_dEditGain = m_dGainMax;
		}

		//将此值设置到设备中
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetFloatFeature("Gain")->SetValue(m_dEditGain); 

	}
	catch (CGalaxyException& e)
	{
		m_dEditGain = dGainOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_dEditGain = dGainOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
}

void CControlDlg::OnEnKillfocusEditSnapSpeed()
{
	// TODO: Add your control notification handler code here
	int64_t     nSnapSpeedOld    = m_nEditSnapSpeed;      // 记录采集速度级别值

	try
	{
		UpdateData(TRUE);

		//判断编辑框中输入的值是否在采集速度级别范围内
		if (m_nEditSnapSpeed < m_nSnapSpeedMin  )
		{
			m_nEditSnapSpeed = m_nSnapSpeedMin;
		}

		if (m_nEditSnapSpeed > m_nSnapSpeedMax)
		{
			m_nEditSnapSpeed = m_nSnapSpeedMax;
		}
		//将此值设置到设备中
		m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetIntFeature("AcquisitionSpeedLevel")->SetValue(m_nEditSnapSpeed); 

	}
	catch (CGalaxyException& e)
	{
		m_nEditSnapSpeed = nSnapSpeedOld;
		MessageBox(e.what());
	}
	catch (std::exception& e)
	{
		m_nEditSnapSpeed = nSnapSpeedOld;
		MessageBox(e.what());
	}

	UpdateData(FALSE);
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

	//判断是否是键盘回车消息
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))   
	{    
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获得当前焦点的控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//判断ID类型
		switch(nCtrlID)
		{
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:
		case IDC_EDIT_SNAP_SPEED:

			//失去焦点
			SetFocus();
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
	return CFormView::PreTranslateMessage(pMsg);
}

void CControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent)
	{
	case 0:
		try
		{
			//如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
			//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
			CString strCurText = "";
			if (m_strBalanceWhiteAuto == "Once")
			{
				//获取设备当前的白平衡
				m_strBalanceWhiteAuto = m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
					->GetValue();
				//获取设备白平衡枚举项
				GxIAPICPP::gxstring_vector vectorEnumEntryList;
				vectorEnumEntryList = m_pDeviceProcessCurrent->m_objFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")
					->GetEnumEntryList();
				//判断白平衡枚举值是否为Off
				if (m_strBalanceWhiteAuto == "Off")
				{
					for (uint32_t i = 0; i < vectorEnumEntryList.size(); i++)
					{
						m_comboxBalanceWhiteAuto.GetLBText(i,strCurText);
						if (strCurText == "Off")
						{
							m_comboxBalanceWhiteAuto.SetCurSel(i);
							break;
						}
					}

				}

			}
			CFormView::OnTimer(nIDEvent);
		}
		catch (CGalaxyException)
		{
			return;
		}
		catch (std::exception)
		{
			return;
		}

		break;
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
