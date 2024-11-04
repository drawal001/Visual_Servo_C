// GxCameraEventsDlg.cpp : 实现文件
//----------------------------------------------------------------------------------
/**
\brief  CGxCameraEventsDlg class implementation
*/
//----------------------------------------------------------------------------------

#include "stdafx.h"
#include "GxCameraEvents.h"
#include "GxCameraEventsDlg.h"
#include "FileVersion.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

    // 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProductVersion;
	CString m_strLegalCopyRight;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strProductVersion(_T(""))
, m_strLegalCopyRight(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_PRODUCTVERSION, m_strProductVersion);
	DDX_Text(pDX, IDC_STATIC_LEGALCOPYRIGHT, m_strLegalCopyRight);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CGxCameraEventsDlg 对话框
CGxCameraEventsDlg::CGxCameraEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxCameraEventsDlg::IDD, pParent)
	, m_bIsOpen(false)
	, m_bIsSnap(false)
	, m_hCB(NULL)
	, m_pLogWnd(NULL)
	, m_pCaptureEventHandle(NULL)
	, m_pFeatureEventHandle(NULL)
	, m_bIsTrigValid(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}
void CGxCameraEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGxCameraEventsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_DEVICE, &CGxCameraEventsDlg::OnBnClickedOpenDevice)
	ON_BN_CLICKED(IDC_START_SNAP, &CGxCameraEventsDlg::OnBnClickedStartSnap)
	ON_BN_CLICKED(IDC_STOP_SNAP, &CGxCameraEventsDlg::OnBnClickedStopSnap)
	ON_BN_CLICKED(IDC_CLOSE_DEVICE, &CGxCameraEventsDlg::OnBnClickedCloseDevice)
	ON_BN_CLICKED(IDC_SEND_SOFT_COMMAND, &CGxCameraEventsDlg::OnBnClickedSendSoftCommand)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CGxCameraEventsDlg 消息处理程序
BOOL CGxCameraEventsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	try
	{
		//初始化库
		IGXFactory::GetInstance().Init();

		//指向时间显示窗口指针
		m_pLogWnd =GetDlgItem(IDC_EDIT_TIME_LOG);

		//更新界面
		__UpdateUI();
	    
		m_pCaptureEventHandle = new CSampleCaptureEventHandle();
		m_pFeatureEventHandle = new CSampleFeatureEventHandle();
	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return FALSE;
		
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return FALSE;

	}

	//除非将焦点设置到控件，否则返回 TRUE
	return TRUE;  
}

void CGxCameraEventsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		char chFileName[MAX_PATH] = {'\0'};
		GetModuleFileName(NULL,chFileName,MAX_PATH);
		CFileVersion fv(chFileName);
		CAboutDlg dlgAbout;
		dlgAbout.m_strProductVersion = _T("Version:") + fv.GetProductVersion();
		dlgAbout.m_strLegalCopyRight =fv.GetLegalCopyright();
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CGxCameraEventsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGxCameraEventsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGxCameraEventsDlg::OnBnClickedOpenDevice()
{
	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen              = false;       ///< 设备是否已打开标志
	bool bIsStreamOpen              = false;       ///< 设备流是否已打开标志
	bool m_bSupportExposureEndEvent = false;       ///< 是否支持曝光结束标志

    try
    {
		//枚举设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);
		if (vectorDeviceInfo.size() <= 0)
		{
			MessageBox("未发现设备!");
			return;
		}

		//打开设备
		m_objDevicePtr =IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(),GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//获取属性控制器对象 
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

		//是否支持曝光结束事件
		m_bSupportExposureEndEvent = __IsSupportExposureEnd();

		if (!m_bSupportExposureEndEvent)
		{
			throw exception("相机不支持曝光结束事件,演示程序对该相机无效!");
		}

		//获取流通道个数
		uint32_t nStreamCount =m_objDevicePtr->GetStreamCount();

		//打开流
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			m_objStreamFeatureControlPtr = m_objStreamPtr->GetFeatureControl();
			bIsStreamOpen  = true;
		}
		else
		{
			throw exception("未发现设备流!");
		}

        // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
        // 以提高网络相机的采集性能,设置方法参考以下代码。
        GX_DEVICE_CLASS_LIST objDeviceClass = m_objDevicePtr->GetDeviceInfo().GetDeviceClass();
        if(GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            // 判断设备是否支持流通道数据包功能
            if(true == m_objFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                // 获取当前网络环境的最优包长值
                int nPacketSize = m_objStreamPtr->GetOptimalPacketSize();
                // 将最优包长值设置为当前设备的流通道包长值
                m_objFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            } 
        }

		//初始化相机参数
		__InitParam();

		m_bIsOpen = true ;

		//更新界面
		__UpdateUI();
    }
    catch (CGalaxyException &e)
    {
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

    	MessageBox(e.what());
		return;
    }
	catch (std::exception& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		MessageBox(e.what());
		return;
	}
}

void CGxCameraEventsDlg::OnBnClickedStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
        try
        {
            //设置Buffer处理模式
            m_objStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
        }
        catch (...)
        {	
        }

		//注册曝光结束事件回调函数
		m_hCB = m_objFeatureControlPtr->RegisterFeatureCallback("EventExposureEnd",
			                                                     m_pFeatureEventHandle, this);
		
		//注册采集回调函数
		m_objStreamPtr->RegisterCaptureCallback(m_pCaptureEventHandle, this);

		//开启流层采集
		m_objStreamPtr->StartGrab();

		//发送开始采集命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;
		m_bIsTrigValid = true;

		//更新界面
		__UpdateUI();

	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxCameraEventsDlg::OnBnClickedStopSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//发送停采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销曝光结束事件回调函数
		m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

		//注销采集回调函数
		m_objStreamPtr->UnregisterCaptureCallback();
		m_bIsSnap = false;

		//更新界面
		__UpdateUI();

	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		return;	
	}
}

void CGxCameraEventsDlg::OnBnClickedCloseDevice()
{
	// TODO: Add your control notification handler code here
	try
	{
		//判断是否停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销采集回调函数
			m_objStreamPtr->UnregisterCaptureCallback();

			//注销曝光结束事件回调函数
			m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	try
	{
		//关闭流对象
		m_objStreamPtr->Close();

		//关闭设备
		m_objDevicePtr->Close();
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	m_bIsOpen = false;
	m_bIsSnap = false;

	//更新界面
	__UpdateUI();
}

void CGxCameraEventsDlg::OnBnClickedSendSoftCommand()
{
	// TODO: Add your control notification handler code here
	try
	{
		// 如果当触发回调正在执行的过程中，再次点击触发按键后，此次点击会被屏蔽掉
		if (!m_bIsTrigValid)
		{
			return;
		}
		else
		{
			m_bIsTrigValid = false;
		}

		SYSTEMTIME    system;
		CString       strTime = "";      ///< 时间字符串

		//获取开始发送软触发命令时间
		GetLocalTime(&system);
		strTime.Format("%02dh:%0.2dm:%0.2ds:%03dms ",
			system.wHour,system.wMinute,system.wSecond,system.wMilliseconds);

		//打印时间
		CString strInformation = "\r\n开始发送软触发命令的时间点:" + strTime;
		std::string strPrintInformation = strInformation.GetBuffer(0);
		PrintInfotoWnd(strPrintInformation);	

		//收到图像历时时间：开始计时
		m_objImageTime.Start();

		//曝光结束事件历时时间：开始计时
		m_objShutterTime.Start();

		//发送软触发命令
		m_objFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();

	}
	catch (CGalaxyException &e)
	{
		MessageBox(e.what());
		m_bIsTrigValid = true;
		return;	
	}
	catch (std::exception &e)
	{
		MessageBox(e.what());
		m_bIsTrigValid = true;
		return;	
	}
}

void CGxCameraEventsDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		if (m_bIsSnap)
		{
			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销曝光结束事件回调函数
			m_objFeatureControlPtr->UnregisterFeatureCallback(m_hCB);

			//注销采集回调函数
			m_objStreamPtr->UnregisterCaptureCallback();

			m_bIsSnap = false;

		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	try
	{
		//判断是否关闭设备
		if (m_bIsOpen)
		{
			//关闭流对象
			m_objStreamPtr->Close();

			//关闭设备
			m_objDevicePtr->Close();

			m_bIsOpen = false;
		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}

	try
	{
		//释放设备资源
		IGXFactory::GetInstance().Uninit();

	}
	catch(CGalaxyException)
	{
		//do noting
	}

	if (m_pCaptureEventHandle != NULL)
	{
		delete m_pCaptureEventHandle;
		m_pCaptureEventHandle = NULL;
	}

	if (m_pFeatureEventHandle != NULL)
	{
		delete m_pFeatureEventHandle;
		m_pFeatureEventHandle = NULL;
	}

    //关闭进程前释放设备资源
	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  初始化设备

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::__InitParam()
{
	//设置采集模式为连续采集
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//设置触发模式为开
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");

	//设置触发源为软触发
	m_objFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Software");

	//设置事件为曝光结束事件
	m_objFeatureControlPtr->GetEnumFeature("EventSelector")->SetValue("ExposureEnd");

	//打开事件使能
	m_objFeatureControlPtr->GetEnumFeature("EventNotification")->SetValue("On");   

}

//----------------------------------------------------------------------------------
/**
\brief  判断是否支持曝光结束事件

\return 返回true：支持曝光结束事件 false :不支持曝光结束事件
*/
//----------------------------------------------------------------------------------
bool CGxCameraEventsDlg::__IsSupportExposureEnd()
{ 
	bool bIsSupportExposureEnd = false;   ///< 是否支持曝光结束事件标识

	//事件枚举项容器
	GxIAPICPP::gxstring_vector vectorEnumEvent;
	CEnumFeaturePointer pEnumFeature = m_objFeatureControlPtr->GetEnumFeature("EventSelector");
	vectorEnumEvent = pEnumFeature->GetEnumEntryList();
	if (vectorEnumEvent.size() <= 0)
	{
		return bIsSupportExposureEnd;
	}

	//是否支持曝光结束事件
	for (uint32_t i=0; i<vectorEnumEvent.size(); i++)
	{
		if (vectorEnumEvent[i] == "ExposureEnd")
		{
			bIsSupportExposureEnd = true;
			return bIsSupportExposureEnd;
		}
	}

	return bIsSupportExposureEnd;
}

//----------------------------------------------------------------------------------
/**
\brief  更新界面

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::__UpdateUI()
{
	GetDlgItem(IDC_OPEN_DEVICE)->EnableWindow(!m_bIsOpen);
	GetDlgItem(IDC_START_SNAP)->EnableWindow(m_bIsOpen && (!m_bIsSnap));
	GetDlgItem(IDC_STOP_SNAP)->EnableWindow(m_bIsOpen && m_bIsSnap);
	GetDlgItem(IDC_CLOSE_DEVICE)->EnableWindow(m_bIsOpen);
	GetDlgItem(IDC_SEND_SOFT_COMMAND)->EnableWindow(m_bIsOpen && m_bIsSnap);
}

//----------------------------------------------------------------------------------
/**
\brief  在时间显示窗口打印时间信息
\param  strInfo 输入打印的信息

\return void
*/
//----------------------------------------------------------------------------------
void CGxCameraEventsDlg::PrintInfotoWnd(string &strInfo)
{
	try
	{
		int      nTextLen = 0;
		CEdit*   pEdit    = (CEdit*)m_pLogWnd;      ///< 指向编辑框句柄

		//输出窗口句柄不为空
		if (pEdit != NULL)
		{
			nTextLen = pEdit->GetWindowTextLength();
			if (pEdit->GetLineCount() > 300)
			{
				pEdit->Clear();
				pEdit->SetWindowText(_T(""));
				nTextLen = pEdit->GetWindowTextLength();
			}

			pEdit->SetSel(nTextLen,-1);
			strInfo.append("\r\n");
			pEdit->ReplaceSel((LPCTSTR)strInfo.c_str());

			if (nTextLen >0)
			{
				pEdit->SetSel((nTextLen - pEdit->GetWindowTextLength() + 1), -1);
			}
			else
			{
				pEdit->SetSel(nTextLen,-1);
			}

		}
	}
	catch (std::exception& e)
	{
		MessageBox(e.what());
		return;
	}
	
}

BOOL CGxCameraEventsDlg::PreTranslateMessage(MSG* pMsg)
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

		//失去焦点
		SetFocus();

		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return  TRUE; 
	}

	return CDialog::PreTranslateMessage(pMsg);
}
