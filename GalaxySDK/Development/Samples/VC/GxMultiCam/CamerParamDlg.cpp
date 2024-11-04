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
 
	// ��ʼ������
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
	// ���öԻ������
	CString strTemp = _T("");
	strTemp.Format("%s�������", m_pMainFrame->m_pBaseinfo[m_pMainFrame->m_nOperateID].szDisplayName);
	SetWindowText(strTemp);

	//��ʼ��UI
	InitUI();

	//ˢ�½���ؼ�ʹ��״̬
	UpDataUI();
	
	//������ʱ��:���豸���Զ���ƽ��ֵΪOnceʱ�����ƽ���ͨ����ʱ��ȡ��ǰֵ����UI����
	SetTimer(0, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��UI�ؼ�����

\return  ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitUI()
{
	// ��ȡ��ǰ�������豸������ж�����Ч��
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}

	GX_STATUS emStatus = GX_STATUS_ERROR;
	int       nValue   = 0;
	
    // ��ʼ���Զ���ƽ��Combox�� 
 	InitAutoWhiteBalanceUI(hDevice);
  
	// ��ʼ���ع���ؿؼ�
	InitShutterUI(hDevice);

	// ��ʼ��������ؿؼ�
	InitGainUI(hDevice);

	// ��ʼ���ɼ��ٶȼ�����ؿؼ�
    InitSpeedLevel(hDevice);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ɼ��ٶȼ�����ؿؼ�
\param   hDevice  [in]   �豸���
 
\return  ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitSpeedLevel(GX_DEV_HANDLE hDevice)
{
	// ��ȡ�ؼ���ָ��
	CSliderCtrl *pSliderCtrl     = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_ACQ_SPEEDLEVEL);
	CStatic     *pStcSpeedLevel  = (CStatic *)GetDlgItem(IDC_STATIC_ACQ_SPEEDLEVEL);
	CEdit       *pEditSpeedLevel = (CEdit *)GetDlgItem(IDC_EDIT_ACQ_SPEEDLEVEL);

	// �жϻ�ȡ�ؼ�ָ�����Ч��
	if ((pSliderCtrl == NULL) || (pStcSpeedLevel == NULL) || (pEditSpeedLevel == NULL))
	{
		return;
	}
    
	GX_STATUS emStatus    = GX_STATUS_ERROR;
	int32_t   nStep       = 0;
	CString   strRange    = "";
	int64_t   nSpeedLevel = 0;
	GX_INT_VALUE stIntValue;

	// ��ѯ��ǰ����Ƿ�֧���ٶȼ���
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

	// ��ȡ�ɼ��ٶȼ���Χ
	emStatus = GXGetIntValue(hDevice, "AcquisitionSpeedLevel", &stIntValue);
	GX_VERIFY(emStatus);
	pSliderCtrl->SetRange((int)stIntValue.nMin, (int)stIntValue.nMax, true);

	// ��ʾ��Χ
    strRange.Format("�ɼ��ٶȼ���(%d~%d)", (int)stIntValue.nMin, (int)stIntValue.nMax);
	pStcSpeedLevel->SetWindowText(strRange);
	
	// ��ȡ��ǰֵ�����µ�����
	nSpeedLevel = stIntValue.nCurValue;
	pSliderCtrl->SetPos((int)nSpeedLevel);
	m_nSpeedLevel = (int)nSpeedLevel;
   
	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ��������ؿؼ�
\param   hDevice  [in]  �豸���
 
\return  ��
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

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(hDevice, "Gain", &m_stGainFloatRange);
	GX_VERIFY(emStatus);

	// ��ʾ��Χ
    strTemp.Format("����(%.4f~%.4f)%s",m_stGainFloatRange.dMin, m_stGainFloatRange.dMax, m_stGainFloatRange.szUnit);
	pStcGain->SetWindowText(strTemp);
	
	// ��ȡ��ǰֵ�����µ�����
	m_dGainValue = m_stGainFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ���ع���ؿؼ�
\param   hDevice  [in]   �豸���

\return  ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::InitShutterUI(GX_DEV_HANDLE hDevice)
{
	CStatic     *pStcShutterShow = (CStatic *)GetDlgItem(IDC_STC_SHUTTER_SHOW);
	CEdit       *pEditShutterVal = (CEdit *)GetDlgItem(IDC_EDIT_SHUTTER);

	// �жϿؼ�����Ч��
	if ((pEditShutterVal == NULL) || (pStcShutterShow == NULL))
	{
		return;
	}

	GX_STATUS      emStatus = GX_STATUS_ERROR;
	CString        strTemp  = "";
	double         dShutterValue   = 0.0;

	// ��ȡ�����ͷ�Χ,����ʼ��Edit�ؼ���static��Χ��ʾ��
	emStatus = GXGetFloatValue(hDevice, "ExposureTime", &m_stShutterFloatRange);
	GX_VERIFY(emStatus);


	strTemp.Format("�ع�(%.4f~%.4f)%s", m_stShutterFloatRange.dMin, m_stShutterFloatRange.dMax, m_stShutterFloatRange.szUnit);
	pStcShutterShow->SetWindowText(strTemp);

	// ��ȡ��ǰֵ������ǰֵ���µ�����
	m_dShutterValue = m_stShutterFloatRange.dCurValue;

	UpdateData(FALSE);
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ����Ͽ�UI����
\param   hDevice    [in]   ������

\return  ��
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

	//�Ƿ�֧���Զ���ƽ�⹦��
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
		// ��֧���Զ���ƽ����ֱ�ӷ���
		return;
	}

	// ��ȡ���ܵ�ö����
	emStatus = GXGetEnumValue(hDevice, "BalanceWhiteAuto", &stEnumValue);
    GX_VERIFY(emStatus);
	nEntryNum = stEnumValue.nSupportedNum;

	// ��ȡö���͵ĵ�ǰֵ,����Ϊ���浱ǰ��ʾֵ
	m_nAutoWhiteBalance = stEnumValue.stCurValue.nCurValue;

	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
	pComboBox->ResetContent();
	for (uint32_t i = 0; i < nEntryNum; i++)
	{
		pComboBox->SetItemData(pComboBox->AddString(stEnumValue.nArrySupportedValue[i].strCurSymbolic), (uint32_t)stEnumValue.nArrySupportedValue[i].nCurValue);
		if (stEnumValue.nArrySupportedValue[i].nCurValue == stEnumValue.stCurValue.nCurValue)
		{
			nCursel = i;
		}
	}

	// ���õ�ǰֵΪ�������ʾֵ
	pComboBox->SetCurSel(nCursel);
}

//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in] ������

\return ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize     = 0;
	GX_STATUS emStatus = GX_STATUS_ERROR;
	
	// ��ȡ������Ϣ���ȣ��������ڴ�ռ�
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		MessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
	}
	else
	{
		MessageBox((LPCTSTR)pchErrorInfo);
	}
	
	// �ͷ�������ڴ�ռ�
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

void CCamerParamDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	// �رն�ʱ��
	KillTimer(0);

	CDialog::OnClose();
}

//----------------------------------------------------------------------------------
/**
\brief  ���ڻ�������Ϣ��Ӧ����

  \return ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	// ��ȡ��ǰ�������豸������ж�����Ч��
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}
	
	GX_STATUS   emStatus = GX_STATUS_ERROR;
	int64_t     nValue = 0;
	double      dValue = 0;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	
	// ��ȡ��ǰ������λ��
	nValue = pSlider->GetPos();
	
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_ACQ_SPEEDLEVEL: // ���ڲɼ��ٶȼ���
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
\brief  �л��Զ���ƽ�����Combox�ؼ�

\return ��
*/
//----------------------------------------------------------------------------------
void CCamerParamDlg::OnSelchangeComboWriteBlance() 
{
	// TODO: Add your control notification handler code here
	// ��ȡ��ǰ�������豸������ж�����Ч��
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	if (hDevice == NULL)
	{
		return;
	}

	CComboBox    *pBox     = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	int64_t       nEnumVal = 0;
	int           nIndex   = 0;
	
	nIndex    = pBox->GetCurSel();                    // ��ȡ��ǰѡ��
	nEnumVal  = (int64_t)pBox->GetItemData(nIndex);   // ��ȡ��ǰѡ���Ӧ��ö����ֵ

	//���ð�ƽ��
	//����Զ���ƽ�ⷽʽΪOnce,�豸�����ƽ�����Զ����ĸ�ö��ֵΪoff,
	//����ĸ�����OnTimer������ʱ��ȡ�Զ���ƽ���ö��ֵ���
	emStatus = GXSetEnumValue(hDevice, "BalanceWhiteAuto", nEnumVal);
	GX_VERIFY(emStatus);

	m_nAutoWhiteBalance = nEnumVal;
}

//----------------------------------------------------------------------------------
/**
\brief  ˢ��UI����

\return ��
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
\brief    �����ع�ʱ���EDIT�ؼ�ʧȥ�������Ӧ����

\return   ��
*/
//--------------------------------------------------------------------
void CCamerParamDlg::OnKillfocusEditShutter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GX_STATUS status = GX_STATUS_SUCCESS;

	//��ö�Ӧ�����豸���
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];

	//�ж�����ֵ�Ƿ����ع�ʱ��ķ�Χ��
	//���������ֵ���ع�ֵ��Ϊ���ֵ
	if (m_dShutterValue > m_stShutterFloatRange.dMax)
	{
		m_dShutterValue = m_stShutterFloatRange.dMax;
	}
	//��С����Сֵ���ع�ֵ��Ϊ��Сֵ
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
\brief  ��������ֵ��Edit�ؼ�ʧȥ�������Ӧ����

\return ��
*/
//---------------------------------------------------------
void CCamerParamDlg::OnKillfocusEditGain() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	GX_STATUS status = GX_STATUS_SUCCESS;
	
	//��ö�Ӧ�����豸���
	GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];
	
	//�ж�����ֵ�Ƿ�������ֵ�ķ�Χ��
	//�������ֵ�������ֵ������ֵ���ó����ֵ
	if (m_dGainValue > m_stGainFloatRange.dMax)
	{
		m_dGainValue = m_stGainFloatRange.dMax;
	}

    //�������ֵС����Сֵ�������ֵ���ó���Сֵ
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
\brief  �ַ���Ϣ(��Ҫ����س���Ϣ)
\param  pMsg  ��Ϣ�ṹ��

\return �ɹ���TRUE  ʧ�ܣ�FALSE
*/
//----------------------------------------------------------------
BOOL CCamerParamDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;
	int nCtrlID = 0;           //<  �����ȡ�Ŀؼ�ID

	//�ж��Ƿ��Ǽ��̻س���Ϣ
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam)
	{
		//��ȡ��ǰӵ�����뽹��Ĵ���(�ؼ�)ָ��
		pWnd = GetFocus();

		//��ȡ�ؼ�ID
		nCtrlID = pWnd->GetDlgCtrlID();

		//�ж�ID����
		switch(nCtrlID)
		{
		case IDC_EDIT_SHUTTER:
		case IDC_EDIT_GAIN:

			//ʧȥ����
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
\brief  ��ʱ������:��ʱ��ȡ�Զ���ƽ���ö��ֵʵ������Once�ɹ�������Զ�����Ϊoff
\param  ��ʱ���¼�ID

\return ��
*/
//----------------------------------------------------------------
void CCamerParamDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	GX_STATUS status = GX_STATUS_SUCCESS;
	GX_ENUM_VALUE stEnumValue;

	//����Զ���ƽ�ⷽʽΪOnce,���óɹ���ʵ�ʵİ�ƽ�ⷽʽ���Զ���Ϊoff
	//����ͨ����ʱ��ȡ�豸�ڲ���ƽ��ĵ�ǰֵʵ��UI����ĸ���
	if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_ONCE)
	{
		int64_t    nWBEnumVal = 0;   //< �����ƽ���ö��ֵ
		CComboBox *pAutoWBBox = (CComboBox *)GetDlgItem(IDC_COMBO_AWB);
		GX_DEV_HANDLE hDevice = m_pMainFrame->m_hDevices[m_pMainFrame->m_nOperateID];

		//��ȡ�Զ���ƽ��ö��ֵ
		status = GXGetEnumValue(hDevice, "BalanceWhiteAuto", &stEnumValue);
		if (status != GX_STATUS_SUCCESS)
		{
			return;
		}
		nWBEnumVal = stEnumValue.stCurValue.nCurValue;
		m_nAutoWhiteBalance = nWBEnumVal;

		//�ж��豸�Զ���ƽ���Ƿ��ΪOFF
		if (m_nAutoWhiteBalance == GX_BALANCE_WHITE_AUTO_OFF)
		{
			for (int i = 0; i < WHITE_BALANCE_RNTRY; i++)
			{
				if ((int64_t)pAutoWBBox->GetItemData(i) == GX_BALANCE_WHITE_AUTO_OFF)
				{
					// ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
					pAutoWBBox->SetCurSel(i);
					break;
				}
			}	
		}
	}

	CDialog::OnTimer(nIDEvent);
}
