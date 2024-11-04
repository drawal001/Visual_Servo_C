// GxUSBSnapPnpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxUSBSnapPnp.h"
#include "GxUSBSnapPnpDlg.h"
#include <DBT.H>

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
// CGxUSBSnapPnpDlg dialog

CGxUSBSnapPnpDlg::CGxUSBSnapPnpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGxUSBSnapPnpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGxUSBSnapPnpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGxUSBSnapPnpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGxUSBSnapPnpDlg)
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGxUSBSnapPnpDlg, CDialog)
	//{{AFX_MSG_MAP(CGxUSBSnapPnpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGxUSBSnapPnpDlg message handlers

BOOL CGxUSBSnapPnpDlg::OnInitDialog()
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
    int i = 0;

	// 初始化可识别设备的GUID
	static GUID CameraGUIDs[] = 
	{	
		//GUID
		{0x889ff8a4, 0x40c8, 0x47bc, {0x99, 0x1e, 0x65, 0xc3, 0xb6, 0x83, 0x1f, 0x68}},
		{0xdbc348a2, 0xca28, 0x43c2, {0x8f, 0xdd, 0x69, 0xc3, 0x7e, 0xd2, 0x50, 0x28}}
	}; 

	// 循环注册设备的GUID
	for(i = 0; i < 2; i++)
	{
		RegHVSnapPnpGUID(m_hWnd,CameraGUIDs[i]);
	}

	/// 初始化List表头信息
	InitListHeader();

	GX_STATUS  emStatus = GX_STATUS_SUCCESS;
	/// 初始化设备库
	emStatus = GXInitLib();  
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("初始化设备库失败！");
		exit(0);
	}

	//初始运行程序时，先枚举一次设备并更新设备信息到界面
	UpdateDeviceList();

	//初始化设备变化状态
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGxUSBSnapPnpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGxUSBSnapPnpDlg::OnPaint() 
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
HCURSOR CGxUSBSnapPnpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGxUSBSnapPnpDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)) 
	{		
		return TRUE;
	}   
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))  
	{   
		return   TRUE; 
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//----------------------------------------------------------------------------------
/**
\brief  初始化列表表头

\return 无
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::InitListHeader()
{
	//设置表格样式
	m_listResult.SetExtendedStyle(LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	//添加表头
	m_listResult.InsertColumn(0 , "序号", LVCFMT_LEFT, 110);
	m_listResult.InsertColumn(1 , "名称", LVCFMT_LEFT, 310);
}

//----------------------------------------------------------------------------------
/**
\brief  枚举设备,并将枚举到的设备信息更新到界面上

\return 无
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::UpdateDeviceList()
{
	GX_STATUS            emStatus     = GX_STATUS_ERROR;  
	size_t               nSize        = 0;
	uint32_t             nDeviceNum   = 0;                   ///< 获取到的设备数
	GX_DEVICE_BASE_INFO *pDeviceInfo  = NULL;                ///< 存储设备信息
	CString              strTemp      = "";

	//清空List表
	m_listResult.DeleteAllItems();

	//枚举设备
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 2000);	
	if (emStatus == GX_STATUS_SUCCESS)		
	{	
		if (nDeviceNum != 0)
		{
			// 为保存设备信息申请资源
			pDeviceInfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
			if (pDeviceInfo == NULL)
			{
				MessageBox("分配缓冲区失败！");
				return;
			}
			nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);	
			
			//获取所有设备的基础信息		
			emStatus = GXGetAllDeviceBaseInfo(pDeviceInfo, &nSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				if (pDeviceInfo != NULL)
				{
					delete[]pDeviceInfo;
					pDeviceInfo = NULL;
				}
				MessageBox("获取设备信息失败!");
				return;
			}

			//将获取到的设备信息更新到界面
			GX_DEVICE_BASE_INFO *pTempBaseInfo = pDeviceInfo;
			for (int i = 0; i < (int)nDeviceNum; i++)
			{
				strTemp.Format("%d",i);
				m_listResult.InsertItem(i,strTemp);
				strTemp.Format("%s",pTempBaseInfo->szDisplayName);
				m_listResult.SetItemText(i,1,strTemp);
				pTempBaseInfo++;
			}

		}
	}
	else
	{
		MessageBox("获取设备信息失败!");
		return;
	}

	// 释放函数资源
	if (pDeviceInfo != NULL)
	{
		delete[]pDeviceInfo;
		pDeviceInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  注册相机型号，在指定窗口返回相关信息
\param  handle   [in]  窗口句柄
\param  stGuid   [in]  设备的GUID

\return false：注册失败；ture：注册成功 
*/
//----------------------------------------------------------------------------------
BOOL CGxUSBSnapPnpDlg::RegHVSnapPnpGUID(HANDLE handle, GUID stGuid)
{
	BOOL bReturn = FALSE;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = stGuid; 
	HDEVNOTIFY hDevInterfaceNotify = ::RegisterDeviceNotification(handle, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if (hDevInterfaceNotify)
	{
		bReturn = TRUE;
	}
	
	return bReturn;
}

//----------------------------------------------------------------------------------
/**
\brief  程序热插拔的核心流程函数

\return 无
*/
//----------------------------------------------------------------------------------
LRESULT CGxUSBSnapPnpDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_DEVICECHANGE)
	{
		switch(wParam) 
		{
		case DBT_DEVICEARRIVAL:                  //检测到有设备接入
			{  
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

                 
				switch (devHdr->dbch_devicetype)
				{
				case DBT_DEVTYP_DEVICEINTERFACE:  //接入的为已注册的设备
					{
						// 枚举设备更新界面
					    UpdateDeviceList();
					}
					break;
				default:
					break;
				}				
			}
			break;
			
		case DBT_DEVICEREMOVECOMPLETE:            //检测到有设备拔出
			{
				PDEV_BROADCAST_HDR  devHdr;
				devHdr = (PDEV_BROADCAST_HDR)lParam;

				switch ( devHdr->dbch_devicetype )
				{
				case DBT_DEVTYP_DEVICEINTERFACE :  // 拔出的为以注册的设备
					{
						// 枚举设备更新界面
						UpdateDeviceList();
					}
					break;
				default:
					break;
				}
			}
			break;
			
		default:
			break;
		}	
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

//----------------------------------------------------------------------------------
/**
\brief  关闭应用程序

\return 无
*/
//----------------------------------------------------------------------------------
void CGxUSBSnapPnpDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 释放资源库
	emStatus = GXCloseLib();

	CDialog::OnClose();
}
