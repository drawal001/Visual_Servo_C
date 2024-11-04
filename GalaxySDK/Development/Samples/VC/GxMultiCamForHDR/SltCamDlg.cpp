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
		// ��ʾ�б����֧��4ֻ���
		nMinDevMum = min(nDevNum, MAX_DEVICE_MUN);

		// �豸������Ϊ0���ѻ�ȡ�����豸��Ϣ��ʾ��ö����Ϣ�б�
		m_cmbcam.ResetContent();
		for(uint32_t i = 0; i < nMinDevMum; i++)
		{
			//��ʾ�������
			m_cmbcam.InsertString(-1, m_pMainFrame->m_pBaseinfo[i].szDisplayName);	
		}

		// �����豸�б���ʾ�豸Ϊ��ǰ�����豸
		m_cmbcam.SetCurSel(m_pMainFrame->m_nOperateID);
	}
	else
	{
		// ����豸����Ϊ0������ö��
        UpdateDeviceList();

		// Ϊ�����ڷ�����Դ
        if (AllocBufferForMainFrame())
        {
			// �豸������Ϊ0���ѻ�ȡ�����豸��Ϣ��ʾ��ö����Ϣ�б�
			m_cmbcam.ResetContent();
			for(uint32_t i = 0; i < m_nDeviceNum; i++)
			{
				//��ʾ�������
				m_cmbcam.InsertString(-1, m_pMainFrame->m_pBaseinfo[i].szDisplayName);	
			}
			m_cmbcam.SetCurSel(0);

			m_pMainFrame->m_nCamNum = m_nDeviceNum;
        }
		else
		{
			m_pMainFrame->m_nCamNum = 0;
			MessageBox("������Դʧ��!");		
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
	// ��ȡѡ���������
	m_pMainFrame->m_nOperateID = m_cmbcam.GetCurSel();

	// �ж������Ƿ����
	if (m_pMainFrame->m_nOperateID >= 0)
	{
		//���������ڱ���Ϊ�豸����
		m_pMainFrame->SetWindowText(m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	}
	else
	{
		//���������ڱ���ΪGxMultiCamForHDR.exe
        m_pMainFrame->SetWindowText("GxMultiCamForHDR.exe");
	}
	CDialog::OnOK();
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ�����ڷ�����Դ

\return �ɹ�:TRUE  ʧ��:FALSE
*/
//----------------------------------------------------------------------------------
bool CSltCamDlg::AllocBufferForMainFrame()
{
	uint32_t i = 0;

    //---------------------����ռ�-----------------------------
	//����������
	m_pMainFrame->m_hDevices = new GX_DEV_HANDLE[m_nDeviceNum];
	if (m_pMainFrame->m_hDevices == NULL)
	{
		ClearBuffer();
		return false;
	}

	//������������
	m_pMainFrame->m_hStream = new GX_DEV_HANDLE[m_nDeviceNum];
	if (m_pMainFrame->m_hStream == NULL)
	{
		ClearBuffer();
		return false;
	}

	//����������ݽṹ��
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


	//---------------------��ʼ����Դ��Ϣ-----------------------------
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
\brief  ö���豸,��ȡ�豸����

\return ��
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::UpdateDeviceList()
{
	GX_STATUS  emStatus   = GX_STATUS_SUCCESS;
    uint32_t   nDeviceNum = 0;
	bool       bRet       = true;
	size_t     nSize      = 0;

	//ö���豸
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		return;
	}

	//�豸��������0,������Դ��ȡ�豸��Ϣ
	if (nDeviceNum > 0)
	{
		// Ϊ��ȡ�豸��Ϣ׼����Դ
		m_pMainFrame->m_pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];
		if (m_pMainFrame->m_pBaseinfo == NULL)
		{
			MessageBox("Ϊ��ȡ�豸��Ϣ������Դʧ��");

			// �������ڵ��豸������Ϊ0
			nDeviceNum = 0;

			return;
		}

		//��ȡ����ö���������Ϣ
		nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);  //����ṹ���С
		emStatus = GXGetAllDeviceBaseInfo(m_pMainFrame->m_pBaseinfo, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			ShowErrorString(emStatus);
			delete []m_pMainFrame->m_pBaseinfo;
			m_pMainFrame->m_pBaseinfo = NULL;

			// ���豸������Ϊ0
			nDeviceNum = 0;

			return;
		}
	}

	m_nDeviceNum = min(nDeviceNum, MAX_DEVICE_MUN);
}

//----------------------------------------------------------------------------------
/**
\brief  ���ö���豸��Ӧ����

\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::OnBnClickedBtnEnum() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ֹͣ��ʾ�豸��Ϣ
    m_pMainFrame->m_nOperateID = -1;

	// ���豸�������ȹر�
	emStatus = CloseCamer();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	// �豸������Ϊ0���ѻ�ȡ�����豸��Ϣ��ʾ��ö����Ϣ�б�
	m_cmbcam.ResetContent();
	ClearBuffer();

    // ö���豸
	UpdateDeviceList();

	// �豸����Ϊ0��ֱ�ӷ���
	if (m_nDeviceNum <= 0)
	{
		m_pMainFrame->m_nCamNum = m_nDeviceNum;
		return ;
	}

	// Ϊ�����ڷ�����Դ
	if (AllocBufferForMainFrame())
	{
		// ��ʾ�б����֧��4ֻ���
		for(uint32_t i = 0; i < m_nDeviceNum; i++)
		{
			//��ʾ�������
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

		MessageBox("������Դʧ��!");
		return;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ���Ϊ�����豸�����Ϣ�������Դ

\return ��
*/
//----------------------------------------------------------------------------------
void CSltCamDlg::ClearBuffer()
{
	CMainFrame* pFrame =(CMainFrame*)AfxGetMainWnd();
	CAMER_INFO *pTmpInfo = NULL;

	// �ͷ��Զ����豸��Ϣ�ṹ��ָ�뼰ͼ��ɼ������Դ
	if (pFrame->m_pstCam != NULL)
	{
		delete[]pFrame->m_pstCam;
		pFrame->m_pstCam = NULL;
	}

	// �����豸���
	if(pFrame->m_hDevices != NULL)
	{
		delete[]pFrame->m_hDevices;
		pFrame->m_hDevices = NULL;
	}

	// ���������
	if (pFrame->m_hStream != NULL)
	{
		delete[]pFrame->m_hStream;
		pFrame->m_hStream = NULL;
	}

	// �����豸��Ϣ
	if(pFrame->m_pBaseinfo != NULL)
	{
		delete[]pFrame->m_pBaseinfo;
		pFrame->m_pBaseinfo = NULL;
	}

	// �ͷ�֡����Ϣ
	if (pFrame->m_pCamsFps != NULL)
	{
		delete[] pFrame->m_pCamsFps;
		pFrame->m_pCamsFps = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  �����ر��������

\return emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CSltCamDlg::CloseCamer()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	uint32_t  i = 0;


    //������û�رգ���ر����
	for(i = 0; i < (uint32_t)m_pMainFrame->m_nCamNum; i++)
	{
		if(m_pMainFrame->m_pstCam[i].bIsSnap)
		{
			//ֹͣ�ɼ�
			emStatus = GXSetCommandValue(m_pMainFrame->m_hDevices[i], "AcquisitionStop");
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}
			
			//ע���ص�
			emStatus = GXUnregisterCaptureCallback(m_pMainFrame->m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}
			
			m_pMainFrame->m_pstCam[i].bIsSnap = FALSE;
		}
		if(m_pMainFrame->m_pstCam[i].bIsOpen)
		{
			//�ر����
			emStatus = GXCloseDevice(m_pMainFrame->m_hDevices[i]);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}
			
			m_pMainFrame->m_hDevices[i] = NULL;
			m_pMainFrame->m_pstCam[i].bIsOpen = FALSE;
		}
	}
	return emStatus;
}

