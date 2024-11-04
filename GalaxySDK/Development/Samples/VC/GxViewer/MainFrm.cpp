// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GxViewer.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_OPEN_DEVICE, OnOpenDevice)
	ON_COMMAND(ID_START_SNAP, OnStartSnap)
	ON_COMMAND(ID_STOP_SNAP, OnStopSnap)
	ON_COMMAND(ID_CLOSE_DEVICE, OnCloseDevice)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_OPEN_DEVICE, OnUpdateOpenDevice)
	ON_UPDATE_COMMAND_UI(ID_START_SNAP, OnUpdateStartSnap)
	ON_UPDATE_COMMAND_UI(ID_STOP_SNAP, OnUpdateStopSnap)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_DEVICE, OnUpdateCloseDevice)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hDevice               = NULL;            // 设备句柄
	m_hStream				= NULL;			   // 流句柄
	m_bIsOpen               = false;           // 相机打开标志
	m_bIsSnap               = false;           // 相机采集标志
	m_pBitmap = NULL;

	// 初始化库
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}
}

CMainFrame::~CMainFrame()
{
	// 关闭设备库
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXCloseLib();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CString strTitle;
	strTitle.Format("GxViewer");
	SetWindowText(strTitle);
	// Do not call CFrameWnd::OnPaint() for painting messages
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

//---------------------------------------------------------------------------------
/**
\brief   关闭应用程序函数

\return  无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 未停采则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	
		// 注销回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		// 释放图像显示资源
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// 未关闭设备则关闭设备
	if (m_bIsOpen)
	{
		// 关闭相机
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
	}		
	CFrameWnd::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"打开设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnOpenDevice() 
{
	// TODO: Add your command handler code here
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum = 0;
	uint32_t	  nDSNum   = 0;
	GX_OPEN_PARAM  stOpenParam;

	// 枚举设备个数
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDeviceNum <= 0)
	{
		MessageBox("未发现设备!");
		return;
	}

	// 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// 打开枚举列表中的第一台设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);

	// 获取流通道数
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// 获取流句柄
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}

	// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
	// 以提高网络相机的采集性能,设置方法参考以下代码。
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// 判断设备是否支持流通道数据包功能
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
			// 获取当前网络环境的最优包长值
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			GX_VERIFY(emStatus);

			// 将最优包长值设置为当前设备的流通道包长值
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	do
	{
		// 初始化图像显示模块
		m_pBitmap = new CGXBitmap(m_hDevice, &m_hWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// 初始化参数
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
}

//----------------------------------------------------------------------------------
/**
\brief  点击"开始采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnStartSnap() 
{
	// TODO: Add your command handler code here
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	m_pBitmap->PrepareForShowImg();
	// 注册回调函数
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// 发开始采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	m_bIsSnap = true;	
}

//----------------------------------------------------------------------------------
/**
\brief  点击"停止采集"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnStopSnap() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 发送停止采集命令
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// 注销回调函数
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);
    
	m_bIsSnap = false;	
}

//----------------------------------------------------------------------------------
/**
\brief  点击"关闭设备"控件响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnCloseDevice() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 未停采时则停止采集
	if (m_bIsSnap)
	{
		// 发送停止采集命令
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		
		// 注销回调函数
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		
		// 释放为图像显示准备的内存
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}


	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// 关闭相机
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}
	m_hDevice = NULL;
	m_bIsOpen = false;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CMainFrame::InitParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// 已知当前相机支持哪个8位图像数据格式可以直接设置，以8位图像数据格式GX_PIXEL_FORMAT_BAYER_GR8为例:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   采集回调函数:图像获取和显示操作
\param   pFrame   回调参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CMainFrame *pDlg   = (CMainFrame*)(pFrame->pUserParam);

	// 当前帧图像的返回状态的判断
	if (pFrame->status != 0)
	{
		return;
	} 
	pDlg->m_pBitmap->Show(pFrame);
}

//----------------------------------------------------------------------------------
/**
\brief  获取错误信息描述
\param  emErrorStatus  错误码

\return void
*/
//----------------------------------------------------------------------------------
void CMainFrame::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_ERROR;

	// 获取错误描述信息长度
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// 获取错误信息描述
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);

	// 显示错误描述信息
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError接口调用失败！");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// 释放资源
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  更新菜单栏中"打开设备"的是否为可用状态
\param  pCmdUI  指向打开设备菜单项的指针

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnUpdateOpenDevice(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (!m_bIsOpen)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
}

//----------------------------------------------------------------------------------
/**
\brief  更新菜单栏中"开始采集"的是否为可用状态
\param  pCmdUI  指向开始采集菜单项的指针

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnUpdateStartSnap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_bIsOpen && (!m_bIsSnap))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
}

//----------------------------------------------------------------------------------
/**
\brief  更新菜单栏中"停止采集"的是否为可用状态
\param  pCmdUI  指向停止采集菜单项的指针

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnUpdateStopSnap(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_bIsOpen && m_bIsSnap)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
}

//----------------------------------------------------------------------------------
/**
\brief  更新菜单栏中"关闭设备"的是否为可用状态
\param  pCmdUI  指向关闭设备菜单项的指针

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnUpdateCloseDevice(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_bIsOpen)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}	
}

