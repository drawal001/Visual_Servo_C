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
	m_hDevice               = NULL;            // �豸���
	m_hStream				= NULL;			   // �����
	m_bIsOpen               = false;           // ����򿪱�־
	m_bIsSnap               = false;           // ����ɼ���־
	m_pBitmap = NULL;

	// ��ʼ����
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
	// �ر��豸��
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
\brief   �ر�Ӧ�ó�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// δͣ����ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	
		// ע���ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		
		// �ͷ�ͼ����ʾ��Դ
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// δ�ر��豸��ر��豸
	if (m_bIsOpen)
	{
		// �ر����
		emStatus = GXCloseDevice(m_hDevice);
		m_bIsOpen = false;
	}		
	CFrameWnd::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ���"���豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnOpenDevice() 
{
	// TODO: Add your command handler code here
	GX_STATUS      emStatus   = GX_STATUS_SUCCESS;
	uint32_t       nDeviceNum = 0;
	uint32_t	  nDSNum   = 0;
	GX_OPEN_PARAM  stOpenParam;

	// ö���豸����
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDeviceNum <= 0)
	{
		MessageBox("δ�����豸!");
		return;
	}

	// �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ��ö���б��еĵ�һ̨�豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_INDEX;
	stOpenParam.pszContent = "1";
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	GX_VERIFY(emStatus);

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		GX_VERIFY(emStatus);
	}

	// �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
	// �������������Ĳɼ�����,���÷����ο����´��롣
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
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
			// ��ȡ��ǰ���绷�������Ű���ֵ
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			GX_VERIFY(emStatus);

			// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			GX_VERIFY(emStatus);
		}
	}

	do
	{
		// ��ʼ��ͼ����ʾģ��
		m_pBitmap = new CGXBitmap(m_hDevice, &m_hWnd);
		if (NULL == m_pBitmap)
		{
			ShowErrorString(GX_STATUS_ERROR);
			break;
		}

		// ��ʼ������
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
\brief  ���"��ʼ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnStartSnap() 
{
	// TODO: Add your command handler code here
	bool      bReturn  = false;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

	m_pBitmap->PrepareForShowImg();
	// ע��ص�����
	emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		m_pBitmap->UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	// ����ʼ�ɼ�����
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
\brief  ���"ֹͣ�ɼ�"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnStopSnap() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// ����ֹͣ�ɼ�����
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	GX_VERIFY(emStatus);
	
	// ע���ص�����
	emStatus = GXUnregisterCaptureCallback(m_hDevice);
	GX_VERIFY(emStatus);
    
	m_bIsSnap = false;	
}

//----------------------------------------------------------------------------------
/**
\brief  ���"�ر��豸"�ؼ���Ӧ����

\return ��
*/
//----------------------------------------------------------------------------------
void CMainFrame::OnCloseDevice() 
{
	// TODO: Add your command handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// δͣ��ʱ��ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		// ����ֹͣ�ɼ�����
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		
		// ע���ص�����
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}
		
		// �ͷ�Ϊͼ����ʾ׼�����ڴ�
		m_pBitmap->UnPrepareForShowImg();
		m_bIsSnap = false;
	}


	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// �ر����
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}
	m_hDevice = NULL;
	m_bIsOpen = false;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CMainFrame::InitParam()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnumValueByString(m_hDevice,"TriggerMode", "Off");	
	VERIFY_STATUS_RET(emStatus);
	
	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ�����ã���8λͼ�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8Ϊ��:
	// emStatus = GXSetEnumValue(m_hDevice, "PixelFormat", "BayerGR8");

	return emStatus;
}

//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����:ͼ���ȡ����ʾ����
\param   pFrame   �ص�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CMainFrame::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	CMainFrame *pDlg   = (CMainFrame*)(pFrame->pUserParam);

	// ��ǰ֡ͼ��ķ���״̬���ж�
	if (pFrame->status != 0)
	{
		return;
	} 
	pDlg->m_pBitmap->Show(pFrame);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������Ϣ����
\param  emErrorStatus  ������

\return void
*/
//----------------------------------------------------------------------------------
void CMainFrame::ShowErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_ERROR;

	// ��ȡ����������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (pchErrorInfo == NULL)
	{
		return;
	}
	
	// ��ȡ������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);

	// ��ʾ����������Ϣ
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}

	// �ͷ���Դ
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ���²˵�����"���豸"���Ƿ�Ϊ����״̬
\param  pCmdUI  ָ����豸�˵����ָ��

\return ��
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
\brief  ���²˵�����"��ʼ�ɼ�"���Ƿ�Ϊ����״̬
\param  pCmdUI  ָ��ʼ�ɼ��˵����ָ��

\return ��
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
\brief  ���²˵�����"ֹͣ�ɼ�"���Ƿ�Ϊ����״̬
\param  pCmdUI  ָ��ֹͣ�ɼ��˵����ָ��

\return ��
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
\brief  ���²˵�����"�ر��豸"���Ƿ�Ϊ����״̬
\param  pCmdUI  ָ��ر��豸�˵����ָ��

\return ��
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

