
// GigeConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GigeConfig.h"
#include "GigeConfigDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGigeConfigDlg �Ի���




CGigeConfigDlg::CGigeConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGigeConfigDlg::IDD, pParent)
	, m_strCamIp(_T(""))
	, m_strCamMask(_T(""))
	, m_strCamGateway(_T(""))
	, m_strCamMac(_T(""))
	, m_strEtIp(_T(""))
	, m_strEtMask(_T(""))
	, m_strEtGateway(_T(""))
	, m_strEtMac(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGigeConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAMERA, m_ListBoxCamera);
	DDX_Text(pDX, IDC_EDIT_CAM_IP, m_strCamIp);
	DDX_Text(pDX, IDC_EDIT_CAM_MASK, m_strCamMask);
	DDX_Text(pDX, IDC_EDIT_CAM_GATEWAY, m_strCamGateway);
	DDX_Text(pDX, IDC_EDIT_CAM_MAC, m_strCamMac);
	DDX_Text(pDX, IDC_EDIT_ET_IP, m_strEtIp);
	DDX_Text(pDX, IDC_EDIT_ET_MASK, m_strEtMask);
	DDX_Text(pDX, IDC_EDIT_ET_GATEWAY, m_strEtGateway);
	DDX_Text(pDX, IDC_EDIT_ET_MAC, m_strEtMac);
}

BEGIN_MESSAGE_MAP(CGigeConfigDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CGigeConfigDlg::OnBnClickedButtonUpdate)
	ON_LBN_SELCHANGE(IDC_LIST_CAMERA, &CGigeConfigDlg::OnLbnSelchangeListCamera)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CGigeConfigDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CGigeConfigDlg ��Ϣ�������

BOOL CGigeConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OnBnClickedButtonUpdate();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGigeConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGigeConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGigeConfigDlg::OnBnClickedButtonUpdate()
{
	m_ListBoxCamera.ResetContent();
	m_CamerasInfo.RemoveAll();

	tSdkCameraDevInfo DevInfo[64];
	INT nCount = ARRAYSIZE(DevInfo);
	if (CameraEnumerateDevice(DevInfo, &nCount) == CAMERA_STATUS_SUCCESS)
	{
		for (int i = 0; i < nCount; ++i)
		{
			if (memcmp(DevInfo[i].acPortType, "NET", 3) != 0)
				continue;

			m_ListBoxCamera.AddString(DevInfo[i].acFriendlyName);
			m_CamerasInfo.Add(DevInfo[i]);
		}
	}
}

void CGigeConfigDlg::OnLbnSelchangeListCamera()
{
	m_strCamIp.Empty();
	m_strCamMask.Empty();
	m_strCamGateway.Empty();
	m_strEtIp.Empty();
	m_strEtMask.Empty();
	m_strEtGateway.Empty();

	int Index = m_ListBoxCamera.GetCurSel();
	if (Index >= 0)
	{
		CameraGigeGetIp(&m_CamerasInfo[Index], 
			m_strCamIp.GetBufferSetLength(16),
			m_strCamMask.GetBufferSetLength(16),
			m_strCamGateway.GetBufferSetLength(16),
			m_strEtIp.GetBufferSetLength(16),
			m_strEtMask.GetBufferSetLength(16),
			m_strEtGateway.GetBufferSetLength(16));
		m_strCamIp.ReleaseBuffer();
		m_strCamMask.ReleaseBuffer();
		m_strCamGateway.ReleaseBuffer();
		m_strEtIp.ReleaseBuffer();
		m_strEtMask.ReleaseBuffer();
		m_strEtGateway.ReleaseBuffer();

		CameraGigeGetMac(&m_CamerasInfo[Index], 
			m_strCamMac.GetBufferSetLength(18),
			m_strEtMac.GetBufferSetLength(18));
		m_strCamMac.ReleaseBuffer();
		m_strEtMac.ReleaseBuffer();
	}
	
	UpdateData(FALSE);
}

void CGigeConfigDlg::OnBnClickedButtonApply()
{
	UpdateData(TRUE);
	int Index = m_ListBoxCamera.GetCurSel();
	if (Index >= 0)
	{
		if (CameraGigeSetIp(&m_CamerasInfo[Index], m_strCamIp, m_strCamMask, m_strCamGateway, TRUE) == CAMERA_STATUS_SUCCESS)
			MessageBox("���óɹ�");
		else
			MessageBox("����ʧ��");
	}
}
