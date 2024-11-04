
// Denoise3DDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Denoise3D.h"
#include "Denoise3DDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDenoise3DDlg �Ի���




CDenoise3DDlg::CDenoise3DDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDenoise3DDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDenoise3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDenoise3DDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_IMG_CAP1, &CDenoise3DDlg::OnBnClickedButtonImgCap1)
	ON_BN_CLICKED(IDC_BUTTON_IMG_CAP2, &CDenoise3DDlg::OnBnClickedButtonImgCap2)
	ON_BN_CLICKED(IDC_BUTTON_IMG_CAP3, &CDenoise3DDlg::OnBnClickedButtonImgCap3)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE1, &CDenoise3DDlg::OnBnClickedButtonImgSave1)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE2, &CDenoise3DDlg::OnBnClickedButtonImgSave2)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE3, &CDenoise3DDlg::OnBnClickedButtonImgSave3)
	ON_BN_CLICKED(IDC_BUTTON_IMG_SAVE4, &CDenoise3DDlg::OnBnClickedButtonImgSave4)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CDenoise3DDlg::OnBnClickedButtonSettings)
END_MESSAGE_MAP()


// CDenoise3DDlg ��Ϣ�������

BOOL CDenoise3DDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CheckDlgButton(IDC_RADIO_AVG, BST_CHECKED);

	CameraSdkInit(1);

	m_Grabber = NULL;
	if (CameraGrabber_CreateFromDevicePage(&m_Grabber) == CAMERA_STATUS_SUCCESS)
	{
		SetTimer(1, 1000, NULL);

		CameraGrabber_GetCameraHandle(m_Grabber, &m_hCamera);
		CameraGrabber_GetCameraDevInfo(m_Grabber, &m_DevInfo);

		CameraCreateSettingPage(m_hCamera, m_hWnd, m_DevInfo.acFriendlyName, NULL, NULL, 0);

		CameraGrabber_SetHWnd(m_Grabber, GetDlgItem(IDC_STATIC_DISP)->m_hWnd);
		CameraGrabber_StartLive(m_Grabber);
	}
	else
	{
		EndDialog(0);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDenoise3DDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	CameraGrabber_Destroy(m_Grabber);
	m_Grabber = NULL;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDenoise3DDlg::OnPaint()
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
HCURSOR CDenoise3DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDenoise3DDlg::OnTimer(UINT_PTR nIDEvent)
{
	tSdkGrabberStat Stat;
	CameraGrabber_GetStat(m_Grabber, &Stat);

	CString strText;
	strText.Format("��:%u ��:%u ��ʾ֡��:%.02f ����֡��:%.02f", Stat.Width, Stat.Height, Stat.DispFps, Stat.CapFps);
	SetDlgItemText(IDC_STATIC_STAT, strText);

	CDialogEx::OnTimer(nIDEvent);
}

void CDenoise3DDlg::DispImg(int index)
{
	void *RGBImage = NULL;
	if (CameraImage_Create(&RGBImage, m_Imgs[index].RgbData, &m_Imgs[index].RgbHead, FALSE) == CAMERA_STATUS_SUCCESS)
	{
		CameraImage_Draw(RGBImage, GetDlgItem(IDC_STATIC_IMG1 + index)->GetSafeHwnd(), 1);
	}
	CameraImage_Destroy(RGBImage);
}

void CDenoise3DDlg::DoImgCap(int index)
{
	tSdkFrameHead FrameInfo;
	BYTE *pRawBuffer;
	CameraSdkStatus status;

	// �ֱ�����Ԥ��ģʽ��ͬ
	tSdkImageResolution sImageSize;
	memset(&sImageSize,0,sizeof(tSdkImageResolution));
	sImageSize.iIndex = 0xff;
	CameraSetResolutionForSnap(m_hCamera, &sImageSize);

	//	CameraSnapToBufferץ��һ֡ͼ�����ݵ��������У��û�������SDK�ڲ�����,�ɹ����ú���Ҫ
	// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
	if((status = CameraSnapToBuffer(m_hCamera, &FrameInfo, &pRawBuffer, 1000)) != CAMERA_STATUS_SUCCESS)
	{
		MessageBox("Snapshot failed,is camera in pause mode?");
		return;
	}
	else
	{
		m_Imgs[index].RawHead = FrameInfo;
		CameraAlignFree(m_Imgs[index].RawData);
		m_Imgs[index].RawData = CameraAlignMalloc(FrameInfo.uBytes, 16);
		memcpy(m_Imgs[index].RawData, pRawBuffer, FrameInfo.uBytes);

		CameraAlignFree(m_Imgs[index].RgbData);
		m_Imgs[index].RgbData = CameraAlignMalloc(FrameInfo.iWidth * FrameInfo.iHeight * 3, 16);
		CameraImageProcess(m_hCamera, pRawBuffer, m_Imgs[index].RgbData, &FrameInfo);
		m_Imgs[index].RgbHead = FrameInfo;

		DispImg(index);

		if (DoCompose() )
		{
			DispImg(3);
		}

		CameraReleaseImageBuffer(m_hCamera, pRawBuffer);
	}
}

BOOL CDenoise3DDlg::DoCompose()
{
	CameraSdkStatus status;
	CString msg;
	tSdkFrameHead ImgHeads[4];
	BYTE* ImgDatas[4];

	for (int i = 0; i < 3; ++i)
	{
		if (m_Imgs[i].RawData == NULL)
			return FALSE;
		ImgHeads[i] = m_Imgs[i].RawHead;
		ImgDatas[i] = m_Imgs[i].RawData;
	}

	m_Imgs[3].RawHead = m_Imgs[2].RawHead;
	CameraAlignFree(m_Imgs[3].RawData);
	m_Imgs[3].RawData = CameraAlignMalloc(m_Imgs[3].RawHead.uBytes, 16);

	float Weights[3];
	if (GetCheckedRadioButton(IDC_RADIO_AVG, IDC_RADIO_SUM) == IDC_RADIO_AVG)
	{
		Weights[0] = Weights[1] = Weights[2] = 1.0f / ARRAYSIZE(Weights);
	}
	else
	{
		Weights[0] = Weights[1] = Weights[2] = 1.0f;
	}

	status = CameraManualDenoise3D(ImgHeads, ImgDatas, 3, Weights, &m_Imgs[3].RawHead, m_Imgs[3].RawData);
	if (status == CAMERA_STATUS_SUCCESS)
	{
		m_Imgs[3].RgbHead = m_Imgs[3].RawHead;
		CameraAlignFree(m_Imgs[3].RgbData);
		m_Imgs[3].RgbData = CameraAlignMalloc(m_Imgs[3].RgbHead.iWidth * m_Imgs[3].RgbHead.iHeight * 3, 16);
		CameraImageProcess(m_hCamera, m_Imgs[3].RawData, m_Imgs[3].RgbData, &m_Imgs[3].RgbHead);
		return TRUE;
	}
	else
	{
		msg.Format("�ϳ�ʧ��, err:%d", status);
		MessageBox(msg);
		return FALSE;
	}
}

void CDenoise3DDlg::OnBnClickedButtonImgCap1()
{
	DoImgCap(0);
}

void CDenoise3DDlg::OnBnClickedButtonImgCap2()
{
	DoImgCap(1);
}

void CDenoise3DDlg::OnBnClickedButtonImgCap3()
{
	DoImgCap(2);
}

void CDenoise3DDlg::SaveImg(int index)
{
	CString msg;
	CameraSdkStatus status;

	if (m_Imgs[index].RgbData == NULL)
	{
		msg.Format("����ͼƬ%dδ����", index + 1);
		MessageBox(msg);
		return;
	}

	CFileDialog Dlg(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"ͼƬ (*.bmp)|*.bmp||");
	if (Dlg.DoModal() == IDOK)
	{
		tSdkFrameHead FrameInfo = m_Imgs[index].RgbHead;
		BYTE* pRgbBuffer = m_Imgs[index].RgbData;

		void *RGBImage = NULL;
		if (CameraImage_Create(&RGBImage, pRgbBuffer, &FrameInfo, FALSE) == CAMERA_STATUS_SUCCESS)
		{
			if ((status = CameraImage_SaveAsBmp(RGBImage, Dlg.GetPathName())) == CAMERA_STATUS_SUCCESS)
			{
				msg.Format("'%s'�ѱ���", (LPCSTR)Dlg.GetPathName());
			}
			else
			{
				msg.Format("'%s'����ʧ�ܣ�err: %d", (LPCSTR)Dlg.GetPathName(), status);
			}
			MessageBox(msg);
		}
		CameraImage_Destroy(RGBImage);
	}
}

void CDenoise3DDlg::OnBnClickedButtonImgSave1()
{
	SaveImg(0);
}

void CDenoise3DDlg::OnBnClickedButtonImgSave2()
{
	SaveImg(1);
}

void CDenoise3DDlg::OnBnClickedButtonImgSave3()
{
	SaveImg(2);
}

void CDenoise3DDlg::OnBnClickedButtonImgSave4()
{
	SaveImg(3);
}

void CDenoise3DDlg::OnBnClickedButtonSettings()
{
	CameraShowSettingPage(m_hCamera, TRUE);
}
