// CamerParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCam.h"
#include "CamerParamDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg dialog


CCamerParamDlg::CCamerParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCamerParamDlg::IDD, pParent)
	, m_bAutoWhiteBalance(false)
	, m_bSpeedLevel(false)
	, m_nAutoWhiteBalance(0)
{
	//{{AFX_DATA_INIT(CCamerParamDlg)
	m_nSpeedLevel = 0;
	m_dShutterValue = 0.0;
	m_dGainValue = 0.0;
	//}}AFX_DATA_INIT
 
	// 初始化参数
	memset(&m_stGainFloatRange,0,sizeof(GX_FLOAT_VALUE));
	memset(&m_stShutterFloatRange,0,sizeof(GX_FLOAT_VALUE));

	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
}


void CCamerParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamerParamDlg)
	DDX_Text(pDX, IDC_EDIT_ACQ_SPEEDLEVEL, m_nSpeedLevel);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_dShutterValue);
    DDX_Text(pDX, IDC_EDIT_GAIN, m_dGainValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCamerParamDlg, CDialog)
	//{{AFX_MSG_MAP(CCamerParamDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_AWB, OnSelchangeComboWriteBlance)
	ON_EN_KILLFOCUS(IDC_EDIT_SHUTTER, OnKillfocusEditShutter)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, OnKillfocusEditGain)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamerParamDlg message handlers

BOOL CCamerParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 设置对话框标题
	CString strTemp = _T("");
	strTemp.Format("%s相机参数", m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	SetWindowText(strTemp);

	//初始化UI
	InitUI();

	//刷新界面控件使能状态
	UpDataUI();
	
	//开启定时器:当设备的自动白平衡值为Once时做完白平衡后通过定时读取当前值更新UI界面
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------------
/**
\brief   初始化UI控件参数

\return  无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitUI()
{
	// 获取当前操作的设备句柄并判断其有效性
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	int       nValue   = 0;
	
    // 初始化自动白平衡Combox框 
 	InitAutoWhiteBalanceUI(hDevice);
  
	// 初始化曝光相关控件
	InitShutterUI(hDevice);

	// 初始化增益相关控件
	InitGainUI(hDevice);

	// 初始化采集速度级别相关控件
    InitSpeedLevel(hDevice);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化采集速度级别相关控件
\param   hDevice  [in]   设备句柄
 
\return  无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitSpeedLevel(GX_DEV_HANDLE hDevice)
{
	// 获取控件的指针
	CSliderCtrl *pSliderCtrl     = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_ACQ_SPEEDLEVEL);
	CStatic     *pStcSpeedLevel  = (CStatic *)GetDlgItem(IDC_STATIC_ACQ_SPEEDLEVEL);
	CEdit       *pEditSpeedLevel = (CEdit *)GetDlgItem(IDC_EDIT_ACQ_SPEEDLEVEL);

	// 判断获取控件指针的有效性
	if ((pSliderCtrl == NULL) || (pStcSpeedLevel == NULL) || (pEditSpeedLevel == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	int32_t   nStep       = 0;
	CString   strRange    = "";
	int64_t   nSpeedLevel = 0;
	GX_INT_VALUE stIntValue;

	// 查询当前相机是否支持速度级别
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(hDevice, "AcquisitionSpeedLevel", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		m_bSpeedLevel = true;
	}
	if (!m_bSpeedLevel)
	{
		pSliderCtrl->EnableWindow(FALSE);
		return;
	}

	// 获取采集速度级别范围
	emStatus = GXGetIntValue(hDevice, "AcquisitionSpeedLevel", &stIntValue);
	GX_VERIFY(emStatus);
	pSliderCtrl->SetRange((int)stIntValue.nMin, (int)stIntValue.nMax, true);

	// 显示范围
    strRange.Format("采集速度级别(%d~%d)", (int)stIntValue.nMin, (int)stIntValue.nMax);
	pStcSpeedLevel->SetWindowText(strRange);
	
	// 获取当前值并更新到界面
	nSpeedLevel = stIntValue.nCurValue;
	pSliderCtrl->SetPos((int)nSpeedLevel);
	m_nSpeedLevel = (int)nSpeedLevel;
   
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化增益相关控件
\param   hDevice  [in]  设备句柄
 
\return  无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitGainUI(GX_DEV_HANDLE hDevice)
{
	CStatic     *pStcGain     = (CStatic *)GetDlgItem(IDC_STC_GAIN);
	CEdit       *pEditGainVal = (CEdit *)GetDlgItem(IDC_EDIT_GAIN);

	if ((pStcGain == NULL) || (pEditGainVal == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus = GX_STATUS_ERROR;
	CString   strTemp  = "";
	double    dGainVal = 0;

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// 显示范围
    strTemp.Format("增益(%.4f~%.4f)%s",m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strTemp);
	
	// 获取当前值并更新到界面
	m_dGainValue = m_stGainFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化曝光相关控件
\param   hDevice  [in]   设备句柄

\return  无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitShutterUI(GX_DEV_HANDLE hDevice)
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);

	// 判断控件的有效性
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double         dShutterValue   = 0.0;

	// 获取浮点型范围,并初始化Edit控件和static范围提示框
	emStatus = GXGetFloatValue(hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);


	strTemp.Format("曝光(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// 获取当前值并将当前值更新到界面
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   hDevice    [in]   相机句柄

\return  无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitAutoWhiteBalanceUI(GX_DEV_HANDLE hDevice)
{
	CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	if ((pComboBox == NULL) || (hDevice == NULL))
	{
		return;
	}

	GX_ENUM_VALUE stEnumValue;
	GX_STATUS   emStatus       = GX_STATUS_ERROR;
	size_t      nbufferSize    = 0;
	int64_t     nEnumValue     = 0;
	uint32_t    nEntryNum      = 0;
	bool        bImplemented   = false;
	int         nCursel        = 0;

	//是否支持自动白平衡功能
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(hDevice, "BalanceWhiteAuto", &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bImplemented = true;
	}
	
	m_bAutoWhiteBalance = bImplemented;
	if (!bImplemented)
	{
		// 不支持自动白平衡则直接返回
		return;
	}

	// 获取功能的枚举数
	emStatus = GXGetEnumValue(hDevice, "BalanceWhiteAuto", &stEnumValue);
    GX_VERIFY(emStatus);
	nEntryNum = stEnumValue.nSupportedNum;

	// 获取枚举型的当前值,并设为界面当前显示值
	m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

	// 初始化当前控件的可选项
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCursel = i;
		}
	}

	// 设置当前值为界面的显示值
	pComboBox->SetCurSel(nCursel);
}

//----------------------------------------------------------------------------------
/**
\brief  弹出错误信息
\param  emErrorStatus  [in] 错误码

\return 无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize     = 0;
	GX_STATUS emStatus = GX_STATUS_ERROR;
	
	// 获取错误信息长度，并申请内存空间
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}
	
	// 释放申请的内存空间
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

void CCamerParamDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	// 关闭定时器
	KillTimer(0);

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  调节滑动条消息响应函数

  \return 无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	// 获取当前操作的设备句柄并判断其有效性
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}
	
	GX_STATUS   emStatus = GX_STATUS_ERROR;
	int64_t     nValue = 0;
	double      dValue = 0;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	
	// 获取当前滑动条位置
	nValue = pSlider->GetPos();
	
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_ACQ_SPEEDLEVEL: // 调节采集速度级别
		emStatus = GXSetIntValue(hDevice, "AcquisitionSpeedLevel", nValue);
		m_nSpeedLevel = (int)nValue;
		break;
	default:
		break;
	}	
	
	GX_VERIFY(emStatus);
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//----------------------------------------------------------------------------------
/**
\brief  切换自动白平衡调节Combox控件

\return 无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	// 获取当前操作的设备句柄并判断其有效性
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}

	CComboBox    *pBox     = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	int64_t       nEnumVal = 0;
	int           nIndex   = 0;
	
	nIndex    = pBox->GetCurSel();                    // 获取当前选项
	nEnumVal  = (int64_t)pBox->GetItemData(nIndex);   // 获取当前选项对应的枚举型值

	//设置白平衡
	//如果自动白平衡方式为Once,设备做完白平衡后会自动更改该枚举值为off,
	//界面的更新由OnTimer函数定时读取自动白平衡的枚举值完成
	emStatus = GXSetEnumValue(hDevice, "BalanceWhiteAuto", nEnumVal);
	GX_VERIFY(emStatus);

	m_nAutoWhiteBalance = nEnumVal;
}

//----------------------------------------------------------------------------------
/**
\brief  刷新UI界面

\return 无
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::UpDataUI()
{
	BOOL bIsDevOpened   = m_pMainFrame->m_pstCam[m_pMainFrame->m_nOperateID].bIsOpen;
	BOOL bIsSnap        = m_pMainFrame->m_pstCam[m_pMainFrame->m_nOperateID].bIsSnap;

	GetDlgItem(IDC_SLIDER_ACQ_SPEEDLEVEL)->EnableWindow(bIsDevOpened && !bIsSnap && m_bSpeedLevel);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(bIsDevOpened);
	GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(bIsDevOpened);
	GetDlgItem(IDC_COMBO_AWB)->EnableWindow(bIsDevOpened && m_bAutoWhiteBalance);
}
//--------------------------------------------------------------------
/**
\brief    控制曝光时间的EDIT控件失去焦点的响应函数

\return   无
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnKillfocusEditShutter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GX_STATUS status = GX_STATUS_SUCCESS;

	//获得对应窗口设备句柄
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];

	//判断输入值是否在曝光时间的范围内
	//若大于最大值则将曝光值设为最大值
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//若小于最小值将曝光值设为最小值
	if (m_dShutterValue < m_stShutterFloatRange.dMin)
	{
		m_dShutterValue = m_stShutterFloatRange.dMin;
	}

	status = GXSetFloatValue(hDevice,"ExposureTime",m_dShutterValue);
	GX_VERIFY(status);

	UpdateData(FALSE);
}

//---------------------------------------------------------
/**
\brief  控制增益值的Edit控件失去焦点的响应函数

\return 无
*/
//---------------------------------------------------------
void CCamerParamDlg::OnKillfocusEditGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	GX_STATUS status = GX_STATUS_SUCCESS;
	
	//获得对应窗口设备句柄
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	
	//判断输入值是否在增益值的范围内
	//若输入的值大于最大值则将增益值设置成最大值
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

    //若输入的值小于最小值则将增益的值设置成最小值
	if (m_dGainValue < m_stGainFloatRange.dMin)
	{
		m_dGainValue = m_stGainFloatRange.dMin;
	}
	
	status = GXSetFloatValue(hDevice,"Gain",m_dGainValue);
	GX_VERIFY(status);
	
	UpdateData(FALSE);
}

//----------------------------------------------------------------
/**
\brief  分发消息(主要处理回车消息)
\param  pMsg  消息结构体

\return 成功：TRUE  失败：FALSE
*/
//----------------------------------------------------------------
BOOL CCamerParamDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	int nCtrlID = 0;           //<  保存获取的控件ID

	//判断是否是键盘回车消息
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//获取当前拥有输入焦点的窗口(控件)指针
		pWnd = GetFocus();

		//获取控件ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//判断ID类型
		switch(nCtrlID)
		{
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:

			//失去焦点
			SetFocus();

			break;
		default:
			break;
		}

		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//----------------------------------------------------------------
/**
\brief  定时器函数:定时读取自动白平衡的枚举值实现设置Once成功后界面自动更新为off
\param  定时器事件ID

\return 无
*/
//----------------------------------------------------------------
void CCamerParamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	GX_STATUS status = GX_STATUS_SUCCESS;
	GX_ENUM_VALUE stEnumValue;

	//如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
	//程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		int64_t    nWBEnumVal = 0;   //< 保存白平衡的枚举值
		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
		GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];

		//获取自动白平衡枚举值
		status = GXGetEnumValue(hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (status != GX_STATUS_SUCCESS)
		{
			return;
		}
		nWBEnumVal = stEnumValue.stCurValue.nCurValue;
		m_nAutoWhiteBalance = nWBEnumVal;

		//判断设备自动白平衡是否变为OFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// 选中自动曝光控件中的OFF项,即由ONCE变为OFF
					pAutoWBBox->SetCurSel(i);
					break;
				}
			}	
		}
	}

	CDialog::OnTimer(nIDEvent);
}
