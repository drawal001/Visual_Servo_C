// SltCamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"
#include "SltCamDlg.h"
#include "GxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSltCamDlg dialog


CSltCamDlg::CSltCamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSltCamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSltCamDlg)
	//}}AFX_DATA_INIT
    m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_nDeviceNum = 0;
}


void CSltCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSltCamDlg)
	DDX_Control(pDX, IDC_CMB_CAM, m_cmbcam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSltCamDlg, CDialog)
	//{{AFX_MSG_MAP(CSltCamDlg)
	ON_CBN_EDITCHANGE(IDC_CMB_CAM, OnEditchangeCmbCam)
	ON_BN_CLICKED(IDC_BTN_REENUM, OnBnClickedBtnEnum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSltCamDlg message handlers

BOOL CSltCamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	uint32_t  nDevNum = m_pMainFrame->m_nCamNum;
	uint32_t  nMinDevMum = 0;
	
	if(nDevNum > 0)
	{ 
		// 显示列表最多支持4只相机
		nMinDevMum = min(nDevNum, MAX_DEVICE_MUN);

		// 设备个数不为0将已获取到的设备信息显示到枚举信息列表
		m_cmbcam.ResetContent();
		for(uint32_t i = 0; i < nMinDevMum; i++)
		{
			//显示相机名称
			m_cmbcam.InsertString(-1, m_pMainFrame->m_pBaseinfo[i].szDisplayName);	
		}

		// 设置设备列表显示设备为当前操作设备
		m_cmbcam.SetCurSel(m_pMainFrame->m_nOperateID);
	}
	else
	{
		// 检测设备个数为0则重新枚举
        UpdateDeviceList();

		// 为主窗口分配资源
        if (AllocBufferForMainFrame())
        {
			// 设备个数不为0将已获取到的设备信息显示到枚举信息列表
			m_cmbcam.ResetContent();
			for(uint32_t i = 0; i < m_nDeviceNum; i++)
			{
				//显示相机名称
				m_cmbcam.InsertString(-1, m_pMainFrame->m_pBaseinfo[i].szDisplayName);	
			}
			m_cmbcam.SetCurSel(0);

			m_pMainFrame->m_nCamNum = m_nDeviceNum;
        }
		else
		{
			m_pMainFrame->m_nCamNum = 0;
			MessageBox("申请资源失败!");		
			return FALSE;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSltCamDlg::OnEditchangeCmbCam() 
{
	// TODO: Add your control notification handler code here
	m_cmbcam.SetCurSel(0);
}

void CSltCamDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 获取选择项的索引
	m_pMainFrame->m_nOperateID = m_cmbcam.GetCurSel();

	// 判断索引是否存在
	if (m_pMainFrame->m_nOperateID >= 0)
	{
		//设置主窗口标题为设备名称
		m_pMainFrame->SetWindowText(m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	}
	else
	{
		//设置主窗口标题为GxMultiCamForHDR.exe
        m_pMainFrame->SetWindowText("GxMultiCamForHDR.exe");
	}
	CDialog::OnOK();
}

//----------------------------------------------------------------------------------
/**
\brief  为主窗口分配资源

\return 成功:TRUE  失败:FALSE
*/
//----------------------------------------------------------------------------------
bool CSltCamDlg::AllocBufferForMainFrame()
{
	uint32_t i = 0;

    //---------------------申请空间-----------------------------
	//创建相机句柄
	m_pMainFrame->m_hDevices = new GX_DEV_HANDLE[m_nDeviceNum];
	if (m_pMainFrame->m_hDevices == NULL)
	{
		ClearBuffer();
		return false;
	}

	//创建相机流句柄
	m_pMainFrame->m_hStream = new GX_DEV_HANDLE[m_nDeviceNum];
	if (m_pMainFrame->m_hStream == NULL)
	{
		ClearBuffer();
		return false;
	}

	//创建相机数据结构体
	m_pMainFrame->m_pstCam=new CAMER_INFO[m_nDeviceNum];
	if (m_pMainFrame->m_pstCam == NULL)
	{
		ClearBuffer();
		return false;
	}

	m_pMainFrame->m_pCamsFps = new CFps[m_nDeviceNum];
	if (m_pMainFrame->m_pCamsFps == NULL)
	{
		ClearBuffer();
		return false;
	}


	//---------------------初始化资源信息-----------------------------
	for (i=0; i<m_nDeviceNum; i++)
	{
		m_pMainFrame->m_hDevices[i]  = NULL;

		m_pMainFrame->m_pstCam[i].bIsOpen        = FALSE;
		m_pMainFrame->m_pstCam[i].bIsSnap        = FALSE;
		m_pMainFrame->m_pstCam[i].fFps           = 0.0;
	}

	return true;
}

//----------------------------------------------------------------------------------
/**
\brief  枚举设备,获取设备个数

\return 无
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::UpdateDeviceList()
{
	GX_STATUS  emStatus   = GX_STATUS_SUCCESS;
    uint32_t   nDeviceNum = 0;
	bool       bRet       = true;
	size_t     nSize      = 0;

	//枚举设备
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//设备个数大于0,分配资源获取设备信息
	if (nDeviceNum > 0)
	{
		// 为获取设备信息准备资源
		m_pMainFrame->m_pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
		if (m_pMainFrame->m_pBaseinfo == NULL)
		{
			MessageBox("为获取设备信息分配资源失败");

			// 将主窗口的设备个数置为0
			nDeviceNum = 0;

			return;
		}

		//获取所有枚举相机的信息
		nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);  //计算结构体大小
		emStatus = GXGetAllDeviceBaseInfo(m_pMainFrame->m_pBaseinfo, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			delete []m_pMainFrame->m_pBaseinfo;
			m_pMainFrame->m_pBaseinfo = NULL;

			// 将设备个数置为0
			nDeviceNum = 0;

			return;
		}
	}

	m_nDeviceNum = min(nDeviceNum, MAX_DEVICE_MUN);
}

//----------------------------------------------------------------------------------
/**
\brief  点击枚举设备响应函数

\return 无返回值
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::OnBnClickedBtnEnum() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 停止显示设备信息
    m_pMainFrame->m_nOperateID = -1;

	// 若设备被打开则先关闭
	emStatus = CloseCamer();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	// 设备个数不为0将已获取到的设备信息显示到枚举信息列表
	m_cmbcam.ResetContent();
	ClearBuffer();

    // 枚举设备
	UpdateDeviceList();

	// 设备个数为0则直接返回
	if (m_nDeviceNum <= 0)
	{
		m_pMainFrame->m_nCamNum = m_nDeviceNum;
		return ;
	}

	// 为主窗口分配资源
	if (AllocBufferForMainFrame())
	{
		// 显示列表最多支持4只相机
		for(uint32_t i = 0; i < m_nDeviceNum; i++)
		{
			//显示相机名称
			m_cmbcam.InsertString(-1, m_pMainFrame->m_pBaseinfo[i].szDisplayName);	
		}
		m_cmbcam.SetCurSel(0);
		m_pMainFrame->m_nOperateID = 0;
		m_pMainFrame->m_nCamNum = m_nDeviceNum;
	}
	else
	{
		m_pMainFrame->m_nOperateID = -1;
		m_pMainFrame->m_nCamNum = 0;

		MessageBox("申请资源失败!");
		return;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  清空为保存设备相关信息分配的资源

\return 无
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::ClearBuffer()
{
	CMainFrame* pFrame =(CMainFrame*)AfxGetMainWnd();
	CAMER_INFO *pTmpInfo = NULL;

	// 释放自定义设备信息结构体指针及图像采集相关资源
	if (pFrame->m_pstCam != NULL)
	{
		delete[]pFrame->m_pstCam;
		pFrame->m_pstCam = NULL;
	}

	// 清理设备句柄
	if(pFrame->m_hDevices != NULL)
	{
		delete[]pFrame->m_hDevices;
		pFrame->m_hDevices = NULL;
	}

	// 清理流句柄
	if (pFrame->m_hStream != NULL)
	{
		delete[]pFrame->m_hStream;
		pFrame->m_hStream = NULL;
	}

	// 清理设备信息
	if(pFrame->m_pBaseinfo != NULL)
	{
		delete[]pFrame->m_pBaseinfo;
		pFrame->m_pBaseinfo = NULL;
	}

	// 释放帧率信息
	if (pFrame->m_pCamsFps != NULL)
	{
		delete[] pFrame->m_pCamsFps;
		pFrame->m_pCamsFps = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  遍历关闭所有相机

\return emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CSltCamDlg::CloseCamer()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  i = 0;


    //如果相机没关闭，则关闭相机
	for(i = 0; i < (uint32_t)m_pMainFrame->m_nCamNum; i++)
	{
		if(m_pMainFrame->m_pstCam[i].bIsSnap)
		{
			//停止采集
			emStatus = GXSetCommandValue(m_pMainFrame->m_hDevices[i], "AcquisitionStop");
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}
			
			//注销回调
			emStatus = GXUnregisterCaptureCallback(m_pMainFrame->m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}
			
			m_pMainFrame->m_pstCam[i].bIsSnap = FALSE;
		}
		if(m_pMainFrame->m_pstCam[i].bIsOpen)
		{
			//关闭相机
			emStatus = GXCloseDevice(m_pMainFrame->m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}
			
			m_pMainFrame->m_hDevices[i] = NULL;
			m_pMainFrame->m_pstCam[i].bIsOpen = FALSE;
		}
	}
	return emStatus;
}

