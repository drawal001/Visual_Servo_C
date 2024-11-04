
// FirstStepMultiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FirstStepMulti.h"
#include "FirstStepMultiDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFirstStepMultiDlg �Ի���




CFirstStepMultiDlg::CFirstStepMultiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFirstStepMultiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ZeroMemory(m_Grabber, sizeof(m_Grabber));
	ZeroMemory(m_hCamera, sizeof(m_hCamera));
	ZeroMemory(m_DevInfo, sizeof(m_DevInfo));
}

void CFirstStepMultiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFirstStepMultiDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_PAUSE_1, &CFirstStepMultiDlg::OnBnClickedCheckPause1)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_1, &CFirstStepMultiDlg::OnBnClickedButtonCmSet1)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_1, &CFirstStepMultiDlg::OnBnClickedButtonSnapshot1)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_2, &CFirstStepMultiDlg::OnBnClickedCheckPause2)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_2, &CFirstStepMultiDlg::OnBnClickedButtonCmSet2)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_2, &CFirstStepMultiDlg::OnBnClickedButtonSnapshot2)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_3, &CFirstStepMultiDlg::OnBnClickedCheckPause3)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_3, &CFirstStepMultiDlg::OnBnClickedButtonCmSet3)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_3, &CFirstStepMultiDlg::OnBnClickedButtonSnapshot3)
	ON_BN_CLICKED(IDC_CHECK_PAUSE_4, &CFirstStepMultiDlg::OnBnClickedCheckPause4)
	ON_BN_CLICKED(IDC_BUTTON_CM_SET_4, &CFirstStepMultiDlg::OnBnClickedButtonCmSet4)
	ON_BN_CLICKED(IDC_BUTTON_SNAPSHOT_4, &CFirstStepMultiDlg::OnBnClickedButtonSnapshot4)
END_MESSAGE_MAP()


// CFirstStepMultiDlg ��Ϣ�������

BOOL CFirstStepMultiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CameraSdkInit(1);

	UINT CMS_ID[] = { IDC_STATIC_CM1, IDC_STATIC_CM2, IDC_STATIC_CM3, IDC_STATIC_CM4 };
	UINT DISPS_ID[] = { IDC_CM_VIEW_1, IDC_CM_VIEW_2, IDC_CM_VIEW_3, IDC_CM_VIEW_4 };

	int nDev = 4;
	CameraEnumerateDevice(m_DevInfo, &nDev);
	for (int i = 0; i < nDev; ++i)
	{
		if (CameraGrabber_Create(&m_Grabber[i], &m_DevInfo[i]) != CAMERA_STATUS_SUCCESS)
			continue;

		SetDlgItemText(CMS_ID[i], m_DevInfo[i].acFriendlyName);

		CameraGrabber_GetCameraHandle(m_Grabber[i], &m_hCamera[i]);
		CameraCreateSettingPage(m_hCamera[i], m_hWnd, m_DevInfo[i].acFriendlyName, NULL, NULL, 0);

		CameraGrabber_SetRGBCallback(m_Grabber[i], FrameRGBCallbackEntry, this);
		CameraGrabber_SetSaveImageCompleteCallback(m_Grabber[i], SaveImageCompleteEntry, this);

		// �ڰ��������ISP����Ҷ�ͼ��
		// ��ɫ���ISPĬ�ϻ����BGR24ͼ��
		tSdkCameraCapbility cap;
		CameraGetCapability(m_hCamera[i], &cap);
		if (cap.sIspCapacity.bMonoSensor)
			CameraSetIspOutFormat(m_hCamera[i], CAMERA_MEDIA_TYPE_MONO8);

		CameraGrabber_SetHWnd(m_Grabber[i], GetDlgItem(DISPS_ID[i])->m_hWnd);
		CameraGrabber_StartLive(m_Grabber[i]);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFirstStepMultiDlg::OnPaint()
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
HCURSOR CFirstStepMultiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFirstStepMultiDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	for (int i = 0; i < ARRAYSIZE(m_Grabber); ++i)
	{
		CameraGrabber_Destroy(m_Grabber[i]);
	}
}

void CFirstStepMultiDlg::OnBnClickedCheckPause1()
{
	if (m_Grabber[0] != NULL)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_PAUSE_1))->GetCheck() == BST_CHECKED)
			CameraGrabber_StopLive(m_Grabber[0]);
		else
			CameraGrabber_StartLive(m_Grabber[0]);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonCmSet1()
{
	if (m_Grabber[0] != NULL)
	{
		CameraShowSettingPage(m_hCamera[0], TRUE);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonSnapshot1()
{
	if (m_Grabber[0] != NULL)
	{
		CameraGrabber_SaveImageAsync(m_Grabber[0]);
	}
}

void CFirstStepMultiDlg::OnBnClickedCheckPause2()
{
	if (m_Grabber[1] != NULL)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_PAUSE_2))->GetCheck() == BST_CHECKED)
			CameraGrabber_StopLive(m_Grabber[1]);
		else
			CameraGrabber_StartLive(m_Grabber[1]);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonCmSet2()
{
	if (m_Grabber[1] != NULL)
	{
		CameraShowSettingPage(m_hCamera[1], TRUE);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonSnapshot2()
{
	if (m_Grabber[1] != NULL)
	{
		CameraGrabber_SaveImageAsync(m_Grabber[1]);
	}
}

void CFirstStepMultiDlg::OnBnClickedCheckPause3()
{
	if (m_Grabber[2] != NULL)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_PAUSE_3))->GetCheck() == BST_CHECKED)
			CameraGrabber_StopLive(m_Grabber[2]);
		else
			CameraGrabber_StartLive(m_Grabber[2]);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonCmSet3()
{
	if (m_Grabber[2] != NULL)
	{
		CameraShowSettingPage(m_hCamera[2], TRUE);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonSnapshot3()
{
	if (m_Grabber[2] != NULL)
	{
		CameraGrabber_SaveImageAsync(m_Grabber[2]);
	}
}

void CFirstStepMultiDlg::OnBnClickedCheckPause4()
{
	if (m_Grabber[3] != NULL)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_PAUSE_4))->GetCheck() == BST_CHECKED)
			CameraGrabber_StopLive(m_Grabber[3]);
		else
			CameraGrabber_StartLive(m_Grabber[3]);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonCmSet4()
{
	if (m_Grabber[3] != NULL)
	{
		CameraShowSettingPage(m_hCamera[3], TRUE);
	}
}

void CFirstStepMultiDlg::OnBnClickedButtonSnapshot4()
{
	if (m_Grabber[3] != NULL)
	{
		CameraGrabber_SaveImageAsync(m_Grabber[3]);
	}
}

void CFirstStepMultiDlg::FrameRGBCallback(void* Grabber, BYTE *pFrameBuffer, tSdkFrameHead* pFrameHead)
{
	// ���ݴ���ص�

	// ���ںڰ����������򿪺�������ISP����Ҷ�ͼ��
	// ��˴˴�pFrameBuffer=8λ�Ҷ�����
	// �����Ͳ�ɫ���һ�����BGR24����

	// ��ɫ���ISPĬ�ϻ����BGR24ͼ��
	// pFrameBuffer=BGR24����
}

void CFirstStepMultiDlg::SaveImageComplete(void* Grabber, void* Image, CameraSdkStatus Status)
{
	if (Image != NULL)
	{
		// ��ʼ�������ļ����ļ���
		CString FileName;
		CString strTime = CTime::GetCurrentTime().Format(_T("%Y%m%d%H%M%S"));
		GetCurrentDirectory(MAX_PATH, FileName.GetBufferSetLength(MAX_PATH));
		FileName.ReleaseBuffer();
		FileName.AppendFormat("\\Snapshot%s.bmp", strTime);

		if (CameraImage_SaveAsBmp(Image, FileName) == CAMERA_STATUS_SUCCESS)
			MessageBox(FileName);
		else
			MessageBox("����ʧ��");

		CameraImage_Destroy(Image);
	}
}
