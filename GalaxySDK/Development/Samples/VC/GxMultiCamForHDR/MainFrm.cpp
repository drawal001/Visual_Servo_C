// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"
#include "MainFrm.h"
#include "CamerParamDlg.h"
#include "SltCamDlg.h"
#include "GxMultiCamForHDRDOC.h"
#include "GxMultiCamForHDRVIEW.h"

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
	m_hDevices	 = NULL;									//��ʼ��������
	m_hStream    = NULL;								    //��ʼ�������
	m_pstCam	 = NULL;									//��ʼ����������ṹ��
	m_nCamNum	 = 0;										//��ʼ�������Ŀ
	m_bViewID 	 = FALSE;									//�Ƿ���ʾ���ID
	m_pBaseinfo	 = NULL;									//��ʼ���豸��Ϣ�ṹ��
	m_nOperateID = -1;                                      //����������
	m_pCamsFps   = NULL;                                    //��ʼ���豸Fps��������

	for (int i = 0; i < 4; i++)
	{
		m_pCWnds[i] = NULL;
	}

	//��ʼ����
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

	//�ͷ��ڴ�
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

	//�رտ�
	emStatus = GXCloseLib();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	m_objMenu.LoadMenu(IDR_MENU_CAM);    //��ɫ�Ӵ�Ϊ��Ӵ��룬������Ϊ�Զ���˵�����
	this->SetMenu(&m_objMenu);           //�����Զ���˵��滻���в˵�
	CMenu* pmSub = m_objMenu.GetSubMenu(0); //�ؼ�������ı�ţ�0��ʾ��һ���Ӳ˵�����������
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
	m_strTitle = "GxMultiCamForHDR";
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
\brief   ѡ�����

\return  ��
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuSltcam() 
{
	// TODO: Add your command handler code here
	//�������ѡ��Ի���
	CSltCamDlg		m_SltDlg;			///< ѡ������Ի������
	INT_PTR         nID  = m_SltDlg.DoModal(); 
	if (nID == IDCANCEL)
	{
		return;
	}
	
	if(m_nOperateID < 0)
	{
		return;
	}

	//�ж�����Ƿ��Ѿ���
	if(m_pstCam[m_nOperateID].bIsOpen)
	{
		//����ڶ����˵������
		m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		SetMenu(&m_objMenu);
	}
	else
	{
		//�ر���ڶ����˵�����
		m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		SetMenu(&m_objMenu);
	}	
}

//---------------------------------------------------------------------------------
/**
\brief   �򿪵�ǰ�豸

\return  void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuOpen() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nValue = 0;
	uint32_t  nDSNum = 0;
	bool      bIsImplemented = false;
	char      szIndex[10] = { 0 };

	// ���岢��ʼ���豸�򿪲���
	GX_OPEN_PARAM stOpenParam;
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = "";

	//��һ�������Ŵ�1��ʼ 
	_itoa(m_nOperateID + 1, szIndex, 10);

	// ������ѱ������ȹر�
	if (m_hDevices[m_nOperateID] != NULL)
	{
		emStatus = GXCloseDevice(m_hDevices[m_nOperateID]);
		GX_VERIFY(emStatus);
		m_hDevices[m_nOperateID] = NULL;
	}

	//�����
	stOpenParam.pszContent = szIndex;
	SetWindowText("���ڴ����...");
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevices[m_nOperateID]);
	GX_VERIFY(emStatus);

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevices[m_nOperateID], &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevices[m_nOperateID], 1, &m_hStream[m_nOperateID]);
		GX_VERIFY(emStatus);
	}

	// �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
	// �������������Ĳɼ�����,���÷����ο����´��롣
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
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
			// ��ȡ��ǰ���绷�������Ű���ֵ
			emStatus = GXGetOptimalPacketSize(m_hDevices[m_nOperateID],&unPacketSize);
			GX_VERIFY(emStatus);

			// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
			emStatus = GXSetIntValue(m_hDevices[m_nOperateID], "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	//����Ӧ�ó������Ϊ��ǰ�������豸��������
	SetWindowText(m_pBaseinfo[m_nOperateID].szDisplayName);
	GX_VERIFY(emStatus);

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevices[m_nOperateID], "AcquisitionMode", "Continuous");
	GX_VERIFY(emStatus);

	//���ô���ģʽΪ��
	emStatus = GXSetEnumValueByString(m_hDevices[m_nOperateID], "TriggerMode", "Off");
	GX_VERIFY(emStatus);

	//��ʼ��ͼ����ʾģ��
	m_pstCam[m_nOperateID].m_pBitmap = new CGXBitmap(m_hDevices[m_nOperateID], m_pCWnds[m_nOperateID]);
	if (NULL == m_pstCam[m_nOperateID].m_pBitmap)
	{
		ShowErrorString(GX_STATUS_ERROR);
		return;
	}

	m_pstCam[m_nOperateID].bIsOpen = TRUE;
	m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
	SetMenu(&m_objMenu);
}

//---------------------------------------------------------------------------------
/**
\brief �رյ�ǰ�豸

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
		//ֹͣ�ɼ�
		emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		//ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevices[m_nOperateID]);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		m_pstCam[m_nOperateID].m_pBitmap->UnPrepareForShowImg();
		m_pstCam[m_nOperateID].bIsSnap = FALSE;
	}

	//�ر��豸
	emStatus = GXCloseDevice(m_hDevices[m_nOperateID]);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	if (NULL != m_pstCam[m_nOperateID].m_pBitmap)
	{
		delete m_pstCam[m_nOperateID].m_pBitmap;
		m_pstCam[m_nOperateID].m_pBitmap = NULL;
	}

	m_hDevices[m_nOperateID]       = NULL;	

	m_pstCam[m_nOperateID].bIsOpen = FALSE;
	pMenuSub = m_objMenu.GetSubMenu(0);      //�ؼ�������ı�ţ�0��ʾ��һ���Ӳ˵�����������
	m_objMenu.EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	SetMenu(&m_objMenu);
}

//---------------------------------------------------------------------------------
/**
\brief	�������IDע����Ӧ�Ļص� ����4ֻ���Ĭ�������ע��4���ص�����
\param	CamID   [in]   ���ID

\return ��
*/
//---------------------------------------------------------------------------------
void CMainFrame::RegisterCallback(int nCamID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//ע��ص�
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
\brief Ϊץ����ǰ��׼���������������������ÿ�ߣ�Ȼ�����ץ�ĺ�����ʼץ��
\return void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuStart() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus       = GX_STATUS_ERROR;

	emStatus = m_pstCam[m_nOperateID].m_pBitmap->PrepareForShowImg();
	GX_VERIFY(emStatus);

	//ע��ص�
	RegisterCallback(m_nOperateID);	

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream[m_nOperateID], "StreamBufferHandlingMode", "OldestFirst");
	GX_VERIFY(emStatus);

	//��ʼ�ɼ�
	emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStart");
	GX_VERIFY(emStatus);

	m_pstCam[m_nOperateID].bIsSnap = TRUE;
}

//---------------------------------------------------------------------------------
/**
\brief ֹͣ�ɼ���ע���ص�

\return void
*/
//---------------------------------------------------------------------------------
void CMainFrame::OnMenuStop() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_ERROR;

	//ֹͣ�ɼ�
	emStatus = GXSetCommandValue(m_hDevices[m_nOperateID], "AcquisitionStop");
	GX_VERIFY(emStatus);

	//ע���ص�
	emStatus = GXUnregisterCaptureCallback(m_hDevices[m_nOperateID]);
	GX_VERIFY(emStatus);

	m_pstCam[m_nOperateID].bIsSnap = FALSE;
}

//---------------------------------------------------------------------------------
/**
\brief  1������ص�����
\param  pFrame  �ص�����

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun1(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID = 0;             //ID��
	char  chPutStr[40] = { '\0' };         //��ʾ֡���ַ�������

	//��ʼ������
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //��ʾͼ�񣬲���ʾ֡�ʣ�
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //��ʾͼ����ʾ֡��
	{
		//��ʾ֡�ʣ����к�
		sprintf(chPutStr, "���:1 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame,chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief 2������ص�����
\param pFrame �ص�����

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun2(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID = 1;             //ID��
	char           chPutStr[40] = { '\0' };        //��ʾ֡���ַ�������

	//��ʼ������
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //��ʾͼ�񣬲���ʾ֡�ʣ�
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //��ʾͼ����ʾ֡��
	{
		//��ʾ֡�ʣ����к�
		sprintf(chPutStr, "���:2 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame, chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief 3������ص�����
\param pFrame  �ص�����

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun3(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}
	CMainFrame    *pf = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID = 2;             //ID��
	char          chPutStr[40] = { '\0' };        //��ʾ֡���ַ�������

	//��ʼ������
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //��ʾͼ�񣬲���ʾ֡�ʣ�
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //��ʾͼ����ʾ֡��
	{
		//��ʾ֡�ʣ����к�
		sprintf(chPutStr, "���:3 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame, chPutStr);
	}
}

//---------------------------------------------------------------------------------
/**
\brief  4������ص�����
\param  pFrame  �ص�����

\return void
*/
//---------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun4(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	if (pFrame->status != 0)
	{
		return;
	}

	CMainFrame    *pf = (CMainFrame*)(pFrame->pUserParam);
	const int32_t i32ID = 3;             //ID��
	char          chPutStr[40] = { '\0' };        //��ʾ֡���ַ�������

	//��ʼ������
	pf->m_pCamsFps[i32ID].IncreaseFrameNum();

	if (!pf->m_bViewID)       //��ʾͼ�񣬲���ʾ֡�ʣ�
	{
		pf->m_pstCam[i32ID].m_pBitmap->Show(pFrame);
	}
	else           //��ʾͼ����ʾ֡��
	{
		//��ʾ֡�ʣ����к�
		sprintf(chPutStr, "���:4 SN:%s FPS:%.2f", pf->m_pBaseinfo[i32ID].szSN, pf->m_pstCam[i32ID].fFps);
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
\brief    Timer��������֡��
\param    nIDEvent  ��ʱ��ID

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	uint32_t i = 0;      //ѭ������

	if((m_nCamNum <= 0) || (m_nOperateID < 0))
	{
		CFrameWnd::OnTimer(nIDEvent);
		return;
	}

	//����֡��
	for(i = 0; i < m_nCamNum; i++)
	{
		m_pCamsFps[i].UpdateFps();
		m_pstCam[i].fFps = (float)m_pCamsFps[i].GetFps();
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------------
/**
\brief  �ر�Ӧ�ó������ٿ��ʱ���øú���

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ֹͣTimer
	KillTimer(0);

	// �����豸����ͣ�ɺ͹رղ���
	for(uint32_t i = 0; i < m_nCamNum; i++)
	{
		if(m_pstCam[i].bIsSnap)
		{
			//ֹͣ�ɼ�
			emStatus = GXSetCommandValue(m_hDevices[i], "AcquisitionStop");

			//ע���ص�
			emStatus = GXUnregisterCaptureCallback(m_hDevices[i]);
			m_pstCam[i].m_pBitmap->UnPrepareForShowImg();
		}
		if(m_pstCam[i].bIsOpen)
		{
			//�ر����
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
\brief  ���"����"�˵���Ϣ��Ӧ����

\return ��
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
	
	//�����ĵ����з�Ϊ�ĸ�����
	CSize paneSize(objRect.Width() / 2, objRect.Height() / 2);
	m_objSplitter.CreateStatic(this, 2, 2);

	//������һ������
	m_objSplitter.CreateView(0, 0, RUNTIME_CLASS(CGxMultiCamForHDRView), paneSize, pContext);
    m_pCWnds[0]	= m_objSplitter.GetPane(0, 0);

	//�����ڶ�������
	m_objSplitter.CreateView(0, 1, RUNTIME_CLASS(CGxMultiCamForHDRView), paneSize, pContext);
	m_pCWnds[1] = m_objSplitter.GetPane(0, 1);

	//��������������
	m_objSplitter.CreateView(1, 0, RUNTIME_CLASS(CGxMultiCamForHDRView), paneSize, pContext);
	m_pCWnds[2]	= m_objSplitter.GetPane(1, 0);

	//�������ĸ�����
	m_objSplitter.CreateView(1, 1, RUNTIME_CLASS(CGxMultiCamForHDRView), paneSize, pContext);
	m_pCWnds[3] = m_objSplitter.GetPane(1, 1);

	return TRUE;
}

//------------------------------------------------------------
/**
\brief   ��ȡ���Խڵ��Ƿ�֧���ҿ�д
\param   chFeature    [in]        ������
\param   bIsEnable      [out]       �Ƿ�ʹ��

\return
*/
//------------------------------------------------------------
void CMainFrame::__GetControlStatus(char* chFeature, bool &bIsEnable)
{
    GX_STATUS emStatus          = GX_STATUS_SUCCESS;
    bool      bIsImplement      = false;
    bool      bIsWritable       = false;
    bool      bIsReadable       = false;

    // �Ƿ�֧�ָýڵ�
	GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
	emStatus = GXGetNodeAccessMode(m_hDevices[m_nOperateID], chFeature, &emAccessMode);
	GX_VERIFY(emStatus);

	if((emAccessMode != GX_NODE_ACCESS_MODE_NI) && (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF))
	{
		bIsImplement = true;
	}
    
    if(!bIsImplement)
    {
        bIsEnable = false;
        return;
    }
    else
    {
        // �жϸýڵ��Ƿ�ɶ�
		emStatus = GXGetNodeAccessMode(m_hDevices[m_nOperateID], chFeature, &emAccessMode);
		GX_VERIFY(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsReadable = true;
		}
        
        if(!bIsReadable)
        {
            bIsEnable = true;
            return;
        }

        // �жϸýڵ��Ƿ��д
		emStatus = GXGetNodeAccessMode(m_hDevices[m_nOperateID], chFeature, &emAccessMode);
		GX_VERIFY(emStatus);

		if(emAccessMode == GX_NODE_ACCESS_MODE_RW)
		{
			bIsWritable = true;
		}
        

        // ֧���ҿ�д
        if(bIsWritable)
        {
            bIsEnable = true;
        }
        else
        {
            bIsEnable = false;
        }
    }
}

//------------------------------------------------------------
/**
\brief   �����������

\return
*/
//------------------------------------------------------------
void CMainFrame::UpDateImgInfo(void)
{
	//if (NULL != m_pstCam[m_nOperateID].m_pBitmap) 
	//{
	//	delete m_pstCam[m_nOperateID].m_pBitmap;
	//	m_pstCam[m_nOperateID].m_pBitmap = NULL;
	//}
	//m_pstCam[m_nOperateID].m_pBitmap = new CGXBitmap(m_hDevices[m_nOperateID], m_pCWnds[m_nOperateID]);
	//if (NULL == m_pstCam[m_nOperateID].m_pBitmap)
	//{
	//	ShowErrorString(GX_STATUS_ERROR);
	//	return;
	//}

}

