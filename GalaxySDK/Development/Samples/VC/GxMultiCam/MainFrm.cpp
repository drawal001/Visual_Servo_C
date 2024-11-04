// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GxMultiCam.h"
#include "MainFrm.h"
#include "CamerParamDlg.h"
#include "SltCamDlg.h"
#include "GxMultiCamDOC.h"
#include "GxMultiCamVIEW.h"

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
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_SLTCAM, OnMenuSltcam)
	ON_COMMAND(ID_MENU_OPEN, OnMenuOpen)
	ON_COMMAND(ID_MENU_CLOSE, OnMenuClose)
	ON_COMMAND(ID_MENU_START, OnMenuStart)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_STOP, OnMenuStop)
	ON_UPDATE_COMMAND_UI(ID_MENU_OPEN, OnUpdateMenuOpen)
	ON_UPDATE_COMMAND_UI(ID_MENU_START, OnUpdateMenuStart)
	ON_UPDATE_COMMAND_UI(ID_MENU_CLOSE, OnUpdateMenuClose)
	ON_UPDATE_COMMAND_UI(ID_MENU_STOP, OnUpdateMenuStop)
	ON_COMMAND(ID_MENU_CAMID, OnMenuCamid)
	ON_UPDATE_COMMAND_UI(ID_MENU_CAMID, OnUpdateMenuCamid)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_CAMMER_PARAM, OnMenuCammerParam)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hDevices	 = NULL;									//初始化相机句柄
	m_hStream    = NULL;									//初始化流句柄
	m_pstCam	 = NULL;									//初始化相机参数结构体
	m_nCamNum	 = 0;										//初始化相机数目
	m_bViewID 	 = FALSE;									//是否显示相机ID
	m_pBaseinfo	 = NULL;									//初始化设备信息结构体
	m_nOperateID = -1;                                      //操作相机编号
	m_pCamsFps   = NULL;                                    //初始化设备Fps计数对象

	for (int i = 0; i < 4; i++)
	{
		m_pCWnds[i] = NULL;
	}

	//初始化库
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}
}

CMainFrame::~CMainFrame()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//释放内存
	if(m_pBaseinfo != NULL)
	{
		delete[]m_pBaseinfo;
		m_pBaseinfo = NULL;
	}
	if(m_hDevices != NULL)
	{
		delete[]m_hDevices;
		m_hDevices = NULL;
	}
	if (m_hStream != NULL)
	{
		delete[] m_hStream;
		m_hStream = NULL;
	}
	if (m_pCamsFps != NULL)
	{
		delete[] m_pCamsFps;
		m_pCamsFps = NULL;
	}

	if(m_pstCam != NULL)
	{
		delete[]m_pstCam;
        m_pstCam = NULL;
	}

	//关闭库
	emStatus = GXCloseLib();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	m_objMenu.LoadMenu(IDR_MENU_CAM);    //黑色加粗为添加代码，括号内为自定义菜单名。
	this->SetMenu(&m_objMenu);           //设置自定义菜单替换现有菜单
	CMenu* pmSub = m_objMenu.GetSubMenu(0); //关键是里面的标号，0表示第一个子菜单，依次类推
	m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	SetTimer(0, 1000, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs))
	{
		return FALSE;
	}
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	m_strTitle = "GxMultiCam";
	cs.style &= ~FWS_ADDTOTITLE;
	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~WS_MAXIMIZEBOX;

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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//---------------------------------------------------------------------------------
/**
\brief   选择相机

\return  无
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuSltcam() 
{
	// TODO: Add your command handler code here
	//调用相机选择对话框
	CSltCamDlg		m_SltDlg;			///< 选择相机对话框对象
	INT_PTR         nID  = m_SltDlg.DoModal(); 
	if (nID != IDOK)
	{
		return;
	}


	
	if(m_nOperateID < 0)
	{
		return;
	}

	//判断相机是否已经打开
	if(m_pstCam[m_nOperateID].bIsOpen)
	{
		//打开则第二个菜单项变亮
		m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		SetMenu(&m_objMenu);
	}
	else
	{
		//关闭则第二个菜单项变灰
		m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		SetMenu(&m_objMenu);
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   打开当前设备

\return  void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuOpen() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nValue   = 0;
	uint32_t  nDSNum   = 0;
	bool      bIsImplemented = false;
	char      szIndex[10]    = {0};

    // 定义并初始化设备打开参数
    GX_OPEN_PARAM stOpenParam;
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode   = GX_OPEN_INDEX;
    stOpenParam.pszContent = "";

	//第一个相机序号从1开始 
    _itoa(m_nOperateID + 1, szIndex, 10);

	// 若相机已被打开则先关闭
	if (m_hDevices[m_nOperateID] != NULL)
	{
		emStatus = GXCloseDevice(m_hDevices[m_nOperateID]);
		GX_VERIFY(emStatus);
		m_hDevices[m_nOperateID] = NULL;
	}

	//打开相机
	stOpenParam.pszContent = szIndex;
	SetWindowText("正在打开相机...");
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevices[m_nOperateID]);
	GX_VERIFY(emStatus);

	// 获取流通道数
	emStatus = GXGetDataStreamNumFromDev(m_hDevices[m_nOperateID], &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// 获取流句柄
		emStatus = GXGetDataStreamHandleFromDev(m_hDevices[m_nOperateID], 1, &m_hStream[m_nOperateID]);
		GX_VERIFY(emStatus);
	}

	// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
	// 以提高网络相机的采集性能,设置方法参考以下代码。
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// 判断设备是否支持流通道数据包功能
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevices[m_nOperateID], "GevSCPSPacketSize", &emAccessMode);
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
			emStatus = GXGetOptimalPacketSize(m_hDevices[m_nOperateID],&unPacketSize);
			GX_VERIFY(emStatus);

			// 将最优包长值设置为当前设备的流通道包长值
			emStatus = GXSetIntValue(m_hDevices[m_nOperateID], "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	//更新应用程序标题为当前操作的设备对象名称
	SetWindowText(m_pBaseinfo[m_nOperateID].szDisplayName);
	GX_VERIFY(emStatus);

	do
	{
		//初始化图像显示模块
		m_pstCam[m_nOperateID].m_pBitmap = new CGXBitmap(m_hDevices[m_nOperateID], m_pCWnds[m_nOperateID]);
		if (NULL == m_pstCam[m_nOperateID].m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		//设置采集模式连续采集
		emStatus = GXSetEnumValueByString(m_hDevices[m_nOperateID], "AcquisitionMode", "Continuous");
		GX_VERIFY_BREAK(emStatus);

		//设置触发模式为关
		emStatus = GXSetEnumValueByString(m_hDevices[m_nOperateID], "TriggerMode", "Off");
		GX_VERIFY_BREAK(emStatus);		

	}while(false);

	if((GX_STATUS_SUCCESS != emStatus)
		&& NULL != m_pstCam[m_nOperateID].m_pBitmap)
	{
	    delete m_pstCam[m_nOperateID].m_pBitmap;
		m_pstCam[m_nOperateID].m_pBitmap = NULL;
		return;
	}
	
	m_pstCam[m_nOperateID].bIsOpen = TRUE;
	m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);

    SetMenu(&m_objMenu);	
}

//---------------------------------------------------------------------------------
/**
\brief 关闭当前设备

\return  void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuClose() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	CMenu*    pMenuSub = NULL;

	if (m_pstCam[m_nOperateID].bIsSnap)
	{
		//停止采集
		emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		//注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevices[m_nOperateID]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}
		m_pstCam[m_nOperateID].m_pBitmap->UnPrepareForShowImg();
		m_pstCam[m_nOperateID].bIsSnap = FALSE;
	}

	if (NULL != m_pstCam[m_nOperateID].m_pBitmap)
	{
		delete m_pstCam[m_nOperateID].m_pBitmap;
		m_pstCam[m_nOperateID].m_pBitmap = NULL;
	}

	//关闭设备
	emStatus = GXCloseDevice(m_hDevices[m_nOperateID]);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}
	m_hDevices[m_nOperateID]       = NULL;	

	m_pstCam[m_nOperateID].bIsOpen = FALSE;
	pMenuSub = m_objMenu.GetSubMenu(0);      //关键是里面的标号，0表示第一个子菜单，依次类推
	m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	SetMenu(&m_objMenu);
}

//---------------------------------------------------------------------------------
/**
\brief	根据相机ID注册相应的回调 超过4只相机默认最大能注册4个回调函数
\param	CamID   [in]   相机ID

\return 无
*/
//---------------------------------------------------------------------------------
void CMainFrame::RegisterCallback(int nCamID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//注册回调
	switch(nCamID)
	{
	case 0:
		emStatus = GXRegisterCaptureCallback(m_hDevices[nCamID], this, OnFrameCallbackFun1);
		GX_VERIFY(emStatus);
		break;

	case 1:
		emStatus = GXRegisterCaptureCallback(m_hDevices[nCamID], this, OnFrameCallbackFun2);
		GX_VERIFY(emStatus);
		break;

	case 2:
		emStatus = GXRegisterCaptureCallback(m_hDevices[nCamID], this, OnFrameCallbackFun3);
		GX_VERIFY(emStatus);
		break;

	case 3:
		emStatus = GXRegisterCaptureCallback(m_hDevices[nCamID], this, OnFrameCallbackFun4);
		GX_VERIFY(emStatus);
		break;

	default:
		break;
	}

}

//---------------------------------------------------------------------------------
/**
\brief 为抓拍做前期准备，包括创建缓冲区设置宽高，然后调用抓拍函数开始抓拍
\return void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuStart() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus       = GX_STATUS_ERROR;

	emStatus = m_pstCam[m_nOperateID].m_pBitmap->PrepareForShowImg();
	GX_VERIFY(emStatus);
	//注册回调
	RegisterCallback(m_nOperateID);	

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream[m_nOperateID], "StreamBufferHandlingMode", "OldestFirst");
	GX_VERIFY(emStatus);

	//开始采集
	emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStart");
	GX_VERIFY(emStatus);

	m_pstCam[m_nOperateID].bIsSnap = TRUE;
}

//---------------------------------------------------------------------------------
/**
\brief 停止采集并注销回调

\return void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuStop() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//停止采集
	emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStop");
	GX_VERIFY(emStatus);

	//注销回调
	emStatus = GXUnregisterCaptureCallback(m_hDevices[m_nOperateID]);
	GX_VERIFY(emStatus);

	m_pCamsFps[m_nOperateID].Reset();
	m_pstCam[m_nOperateID].bIsSnap = FALSE;
}

//---------------------------------------------------------------------------------
/**
\brief  1号相机回调函数
\param  pFrame  回调参数

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf    = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID  = 0;             //ID号
	char  chPutStr[MAX_PATH]   = {'\0'};         //显示帧率字符串数组

	//初始化参数
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //显示图像，不显示帧率，
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //显示图像，显示帧率
	{
		//显示帧率，序列号
		sprintf(chPutStr, "序号:1 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame, chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief 2号相机回调函数
\param pFrame 回调参数

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf           = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID         = 1;             //ID号
	char           chPutStr[MAX_PATH] = {'\0'};        //显示帧率字符串数组
	
	//初始化参数
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //显示图像，不显示帧率，
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //显示图像，显示帧率
	{
		//显示帧率，序列号
		sprintf(chPutStr, "序号:2 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame, chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief 3号相机回调函数
\param pFrame  回调参数

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}
	CMainFrame    *pf           = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID         = 2;             //ID号
	char          chPutStr[MAX_PATH]  = {'\0'};        //显示帧率字符串数组
	
	//初始化参数
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //显示图像，不显示帧率，
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //显示图像，显示帧率
	{
		//显示帧率，序列号
		sprintf(chPutStr, "序号:3 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame,chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief  4号相机回调函数
\param  pFrame  回调参数 

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun4(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf           = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID         = 3;             //ID号
	char          chPutStr[MAX_PATH]  = {'\0'};        //显示帧率字符串数组
	
	//初始化参数
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //显示图像，不显示帧率，
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //显示图像，显示帧率
	{
		//显示帧率，序列号
		sprintf(chPutStr, "序号:4 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame, chPutStr);
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);	
}

void CMainFrame::OnUpdateMenuOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if((m_nCamNum > 0) && (m_nOperateID >= 0) && (!m_pstCam[m_nOperateID].bIsOpen))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateMenuStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if((m_nCamNum > 0) && (m_nOperateID >= 0) && (m_pstCam[m_nOperateID].bIsOpen) && (!m_pstCam[m_nOperateID].bIsSnap))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateMenuClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if((m_nCamNum > 0) && (m_nOperateID >= 0) && (m_pstCam[m_nOperateID].bIsOpen))
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateMenuStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if((m_nCamNum > 0) && (m_nOperateID >= 0) && (m_pstCam[m_nOperateID].bIsOpen) && (m_pstCam[m_nOperateID].bIsSnap))
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
\brief  弹出错误信息
\param  emErrorStatus   [in] 错误码

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CMainFrame::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize        = 0;
	GX_STATUS emStatus     = GX_STATUS_ERROR;
	
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

void CMainFrame::OnMenuCamid() 
{
	// TODO: Add your command handler code here
	m_bViewID = !m_bViewID;
}

void CMainFrame::OnUpdateMenuCamid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bViewID);
}

//----------------------------------------------------------------------------------
/**
\brief    Timer函数计算帧率
\param    nIDEvent  定时器ID

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	uint32_t i = 0;      //循环变量

	if((m_nCamNum <= 0) || (m_nOperateID < 0))
	{
		CFrameWnd::OnTimer(nIDEvent);
		return;
	}

	//更新帧率
	for(i = 0; i < m_nCamNum; i++)
	{
		m_pCamsFps[i].UpdateFps();
		m_pstCam[i].fFps = (float)m_pCamsFps[i].GetFps();
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------------
/**
\brief  关闭应用程序销毁框架时调用该函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 停止Timer
	KillTimer(0);

	// 遍历设备进行停采和关闭操作
	for(uint32_t i = 0; i < m_nCamNum; i++)
	{
		if(m_pstCam[i].bIsSnap)
		{
			//停止采集
			emStatus = GXSetCommandValue(m_hDevices[i], "AcquisitionStop");

			//注销回调
			emStatus = GXUnregisterCaptureCallback(m_hDevices[i]);
			m_pstCam[i].m_pBitmap->UnPrepareForShowImg();
		}
		if(m_pstCam[i].bIsOpen)
		{
			//关闭相机
			emStatus      = GXCloseDevice(m_hDevices[i]);
			m_hDevices[i] = NULL;

			if (NULL != m_pstCam[i].m_pBitmap)
			{
				delete m_pstCam[i].m_pBitmap;
				m_pstCam[i].m_pBitmap = NULL;
			}
		}

	}
	CFrameWnd::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  点击"参数"菜单消息响应函数

\return 无
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnMenuCammerParam() 
{
	// TODO: Add your command handler code here
	CCamerParamDlg dlg;
	dlg.DoModal();	
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect  objRect;
	GetClientRect(&objRect);
	
	//将单文档分切分为四个窗口
	CSize paneSize(objRect.Width() / 2, objRect.Height() / 2);
	m_objSplitter.CreateStatic(this, 2, 2);

	//创建第一个窗口
	m_objSplitter.CreateView(0, 0, RUNTIME_CLASS(CGxMultiCamView), paneSize, pContext);
    m_pCWnds[0]	= m_objSplitter.GetPane(0, 0);

	//创建第二个窗口
	m_objSplitter.CreateView(0, 1, RUNTIME_CLASS(CGxMultiCamView), paneSize, pContext);
	m_pCWnds[1] = m_objSplitter.GetPane(0, 1);

	//创建第三个窗口
	m_objSplitter.CreateView(1, 0, RUNTIME_CLASS(CGxMultiCamView), paneSize, pContext);
	m_pCWnds[2]	= m_objSplitter.GetPane(1, 0);

	//创建第四个窗口
	m_objSplitter.CreateView(1, 1, RUNTIME_CLASS(CGxMultiCamView), paneSize, pContext);
	m_pCWnds[3] = m_objSplitter.GetPane(1, 1);

	return TRUE;
}

