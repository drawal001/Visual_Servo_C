// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GxMultiCamForHDR.h"
#include "ControlDlg.h"
#include "GxMultiCamForHDRDoc.h"

using namespace std;

// CControlDlg

IMPLEMENT_DYNCREATE(CControlDlg, CFormView)

CControlDlg::CControlDlg()
	: CFormView(CControlDlg::IDD)
	, m_bCheckShowDevice(FALSE)
{

	m_nDeviceListCurrent          = 0;
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
	DDX_Check(pDX, IDC_CHECK_SHOW_DEVICE_FPS, m_bCheckShowDevice);
}

BEGIN_MESSAGE_MAP(CControlDlg, CFormView)
	ON_BN_CLICKED(IDC_BTN_OPEN_DEVICE, &CControlDlg::OnBnClickedBtnOpenDevice)
	ON_BN_CLICKED(IDC_BTN_START_SNAP, &CControlDlg::OnBnClickedBtnStartSnap)
	ON_BN_CLICKED(IDC_BTN_STOP_SNAP, &CControlDlg::OnBnClickedBtnStopSnap)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DEVICE, &CControlDlg::OnBnClickedBtnCloseDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnSelchangeComboDeviceList)
	ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CControlDlg::OnCbnDropdownComboDeviceList)
	ON_BN_CLICKED(IDC_CHECK_SHOW_DEVICE_FPS, &CControlDlg::OnBnClickedCheckShowDeviceFps)
    ON_WM_TIMER()
	ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_CAMERA_PARAM, &CControlDlg::OnBnClickedButtonCameraParam)
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
		//��ʼ���豸��
		IGXFactory::GetInstance().Init();

		//ö���豸
		IGXFactory::GetInstance().UpdateDeviceList(1000,m_vectorDeviceInfo);

		//δ��⵽�豸
		if (m_vectorDeviceInfo.size() <= 0)
		{
			MessageBox("δ��⵽�豸,��ȷ���豸�������Ӻ���������!");
            GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(false);
			GetDlgItem(IDC_BTN_OPEN_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_CLOSE_DEVICE)->EnableWindow(false);
			GetDlgItem(IDC_BTN_START_SNAP)->EnableWindow(false);
            GetDlgItem(IDC_BTN_STOP_SNAP)->EnableWindow(false);
			GetDlgItem(IDC_BTN_CAMERA_PARAM)->EnableWindow(false);
			GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(false);
			CMainFrame* pMainFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
			pMainFrm->m_objControlDlg = this;
			return;
		}

		//���豸������ʾ���豸�б���
		for (uint32_t i =0; i< m_vectorDeviceInfo.size(); i++)
		{
			//����豸������4��ֻ����4���豸��Ϣ
			if (i >= DEVICE_CONTS)
			{
				break;
			}

			gxstring strDeviceInformation = "";
			strDeviceInformation =m_vectorDeviceInfo[i].GetDisplayName();
			m_comboxDevice.SetItemData(m_comboxDevice.AddString(strDeviceInformation.c_str()),i);
			m_mapDeviceInformation.insert(map<int,CDeviceProcess*>::value_type(i,m_pDeviceProcess[i]));

			//���½���
			__UpdateUI(m_pDeviceProcess[i]);

		}

		m_comboxDevice.SetCurSel(0);

		//��ȡ��ǰ��ʾ���豸���������
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
		//��ȡ��ʾ�����Ӵ��ھ��
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

		//��ȡ�豸���
		int nDeviceIndex = m_nDeviceListCurrent+1;

		//���豸
		m_pDeviceProcessCurrent->OpenDevice(m_vectorDeviceInfo[m_nDeviceListCurrent].GetSN(),pWnd,nDeviceIndex);

		//��ʼ������
		__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

		//���½���
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
\breaf  ��ʼ������

\param  objFeatureControlPtr[in]      ���Կ�����

\return void
**/
//----------------------------------------------------------------------
void CControlDlg::__InitUI(CGXFeatureControlPointer objFeatureControlPtr)
{
 
	if (objFeatureControlPtr.IsNull())
	{
		return;
	}

	//����ǰ�豸�Ƿ���ʾ֡�ʱ�־ˢ������
	m_bCheckShowDevice = m_pDeviceProcessCurrent->GetShowFPS();

	UpdateData(FALSE);

}

//----------------------------------------------------------------------
/**
\breaf  ���½���

\param  pDeviceProcess[in]      �豸���������

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
    GetDlgItem(IDC_BTN_CAMERA_PARAM)->EnableWindow(IsOpen);
	GetDlgItem(IDC_CHECK_SHOW_DEVICE_FPS)->EnableWindow(IsOpen);
}

void CControlDlg::OnBnClickedBtnStartSnap()
{
	// TODO: Add your control notification handler code here
	try
	{
		//��ʼ�ɼ�
		m_pDeviceProcessCurrent->StartSnap();

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);

		//�����豸�ͺż�֡����Ϣ
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
		//ֹͣ�ɼ�
		m_pDeviceProcessCurrent->StopSnap();

		//���½���
		__UpdateUI(m_pDeviceProcessCurrent);

		//�رն�ʱ��1
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
		//�ر��豸
		m_pDeviceProcessCurrent->CloseDevice();
		

		//���½���
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

		//�жϵ�ǰ�豸�Ƿ��Ѵ�
		bool bIsOpen = m_pDeviceProcessCurrent->IsOpen();
		if (!bIsOpen)
		{
			__UpdateUI(m_pDeviceProcessCurrent);
		}

		else
		{
			//ˢ�½���
			__InitUI(m_pDeviceProcessCurrent->m_objFeatureControlPtr);

			//���½���
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
	int   nCtrlID = 0;             //< �����ȡ�Ŀؼ�ID

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
    case 1:
        //�����Ѵ��豸������ˢ��FPS����
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
		//����map���������Ѵ��豸ȫ���ر�
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
		//�ͷ��豸����Դ
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

void CControlDlg::OnBnClickedButtonCameraParam()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CCamerParamDlg dlg(m_pDeviceProcessCurrent);
    dlg.DoModal();
}
