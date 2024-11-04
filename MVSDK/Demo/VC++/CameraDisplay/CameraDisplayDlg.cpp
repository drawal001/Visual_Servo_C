
// CameraDisplayDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CameraDisplay.h"
#include "CameraDisplayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern int gLanguage;

// CCameraDisplayDlg �Ի���



CCameraDisplayDlg::CCameraDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraDisplayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bPause = TRUE;
	m_hCamera = -1;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pFrameBuffer = NULL;
}

void CCameraDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraDisplayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CCameraDisplayDlg::OnBnClickedButtonSettings)
	ON_BN_CLICKED(IDC_RADIO_SCALE, &CCameraDisplayDlg::OnBnClickedRadioScale)
	ON_BN_CLICKED(IDC_RADIO_DIRECT, &CCameraDisplayDlg::OnBnClickedRadioDirect)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CCameraDisplayDlg ��Ϣ�������

BOOL CCameraDisplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_pDisp = GetDlgItem(IDC_STATIC_DISP);
	((CButton*)GetDlgItem(IDC_RADIO_SCALE))->SetCheck(BST_CHECKED);
	OnBnClickedRadioScale();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CameraSdkInit(gLanguage);
	if (!InitCamera())
	{
		EndDialog(0);
		return TRUE;
	}

	SetTimer(100, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCameraDisplayDlg::OnPaint()
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
HCURSOR CCameraDisplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CCameraDisplayDlg::InitCamera()
{
	tSdkCameraDevInfo sCameraList[10];
	INT iCameraNums;
	CameraSdkStatus status;
	CRect rect;
	tSdkCameraCapbility sCameraInfo;

	//ö���豸������豸�б�
	//Enumerate camera
	iCameraNums = 10;//����CameraEnumerateDeviceǰ��������iCameraNums = 10����ʾ���ֻ��ȡ10���豸�������Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ

	if (CameraEnumerateDevice(sCameraList,&iCameraNums) != CAMERA_STATUS_SUCCESS || iCameraNums == 0)
	{
		MessageBox("No camera was found!");
		return FALSE;
	}

	//��ʾ���У�����ֻ����������һ���������ˣ�ֻ��ʼ����һ�������(-1,-1)��ʾ�����ϴ��˳�ǰ����Ĳ���������ǵ�һ��ʹ�ø�����������Ĭ�ϲ���.
	//In this demo ,we just init the first camera.
	if ((status = CameraInit(&sCameraList[0],-1,-1,&m_hCamera)) != CAMERA_STATUS_SUCCESS)
	{
		CString msg;
		msg.Format("Failed to init the camera! Error code is %d",status);
		MessageBox(msg +"��ԭ����" + CameraGetErrorString(status));
		return FALSE;
	}


	//Get properties description for this camera.
	CameraGetCapability(m_hCamera,&sCameraInfo);

	m_pFrameBuffer = (BYTE *)CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax*sCameraInfo.sResolutionRange.iHeightMax*4,16);	

	ASSERT(m_pFrameBuffer);

	if (sCameraInfo.sIspCapacity.bMonoSensor)
	{
		// �ڰ�������8λ�Ҷ�
		// Black and white camera output 8-bit grayscale
		CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8);
	}

	//ʹ��SDK��װ�õ���ʾ�ӿ�
	//Use  SDK to display camera images.
	CameraDisplayInit(m_hCamera, m_pDisp->m_hWnd);
	m_pDisp->GetClientRect(&rect);
	//Set display window size
	CameraSetDisplaySize(m_hCamera,rect.right - rect.left,rect.bottom - rect.top);

	//֪ͨSDK�ڲ��������������ҳ�档��������Ϊ����������ơ���Ҳ���Ը���SDK�Ľӿ����Լ�ʵ���������������ҳ�棬
	//�������ǽ�����ʹ��SDK�ڲ��Զ������ķ�ʽ����ʡȥ���ڽ��濪���ϵĴ���ʱ�䡣
	//Create the settings window for the camera
	CameraCreateSettingPage(m_hCamera,GetSafeHwnd(), sCameraList[0].acFriendlyName,NULL,0,0);

#ifdef USE_CALLBACK_GRAB_IMAGE //���Ҫʹ�ûص�������ʽ������USE_CALLBACK_GRAB_IMAGE�����
	//Set the callback for image capture
	SDK_TRACE(CameraSetCallbackFunction(m_hCamera,GrabImageCallback,(PVOID)this,NULL),gLanguage?"����ͼ��ץȡ�Ļص�����":"Set image grab call back function");
#else
	m_hDispThread = (HANDLE)_beginthreadex(NULL, 0, &uiDisplayThread, this, 0,  &m_threadID);
	ASSERT (m_hDispThread); 
	SetThreadPriority(m_hDispThread,THREAD_PRIORITY_HIGHEST);
#endif
	//Tell the camera begin to sendding image
	CameraPlay(m_hCamera);
	m_bPause = FALSE;
	return TRUE;
}

BOOL CCameraDisplayDlg::ResetScrollBar(int iFrameW, int iFrameH)
{
	CRect rc;
	m_pDisp->GetClientRect(rc);

	int MaxHPos = iFrameW - rc.Width();
	int MaxVPos = iFrameH - rc.Height();
	if (MaxHPos > 0)
	{
		SetScrollRange(SB_HORZ, 0, MaxHPos);
		EnableScrollBar(SB_HORZ, ESB_ENABLE_BOTH);
	}
	else
	{
		EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);
	}
	if (MaxVPos > 0)
	{
		SetScrollRange(SB_VERT, 0, MaxVPos);
		EnableScrollBar(SB_VERT, ESB_ENABLE_BOTH);
	}
	else
	{
		EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	}

	SetScrollPos(SB_HORZ, 0);
	SetScrollPos(SB_VERT, 0);
	CameraSetDisplayOffset(m_hCamera, 0, 0);
	return TRUE;
}

void CCameraDisplayDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString strStatusText;
	int iTimeCurrnet = 0;
	static int iDispNum = 0;

	//��SDK�ڲ��������Ĳɼ���֡������֡���ȵȡ�
	CameraGetFrameStatistic(m_hCamera, &m_sFrameCount);
	iTimeCurrnet = GetTickCount();

	if (0 != iTimeCurrnet-m_iTimeLast)
	{
		m_fCapFps = (float)((m_sFrameCount.iCapture - m_sFrameLast.iCapture)*1000.0)/(float)(iTimeCurrnet-m_iTimeLast);
		m_fDispFps = (float)((m_iDispFrameNum - iDispNum)*1000.0)/(float)(iTimeCurrnet-m_iTimeLast);
	}
	else
	{
		return;
	}        

	m_iTimeLast = iTimeCurrnet;

	//Update frame information
	if (gLanguage != 0)//chinese
	{
		strStatusText.Format("| ͼ��ֱ���:%d*%d | ��ʾ֡��:%4.2f FPS | ����֡��:%4.2f FPS |",
			m_sFrInfo.iWidth, m_sFrInfo.iHeight,
			m_fDispFps, m_fCapFps);
	}
	else//english
	{
		strStatusText.Format("| Resolution:%d*%d | Display rate:%4.2f FPS | Capture rate:%4.2f FPS |",
			m_sFrInfo.iWidth, m_sFrInfo.iHeight,
			m_fDispFps, m_fCapFps);
	}
	GetDlgItem(IDC_STATIC_INFORMATION)->SetWindowText(strStatusText);

	m_sFrameLast.iCapture = m_sFrameCount.iCapture;
	m_sFrameLast.iLost = m_sFrameCount.iLost;
	m_sFrameLast.iTotal = m_sFrameCount.iTotal;
	iDispNum = m_iDispFrameNum;
}

void CCameraDisplayDlg::OnDestroy()
{
	//����ʼ�����
	if (m_hCamera > 0)
	{
		if (NULL != m_hDispThread)
		{
			//�ȴ��ɼ��߳̽���
			m_bExit = TRUE;
			::WaitForSingleObject(m_hDispThread, INFINITE);
			CloseHandle(m_hDispThread);
			m_hDispThread = NULL;
		}

		//����ʼ�������
		CameraUnInit(m_hCamera);
		m_hCamera = 0;
	}

	if (m_pFrameBuffer)
	{
		CameraAlignFree(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
	}

	CDialogEx::OnDestroy();
}

UINT WINAPI CCameraDisplayDlg::uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CCameraDisplayDlg* 		pThis = (CCameraDisplayDlg*)lpParam;
	BYTE*			pbyBuffer;
	CameraSdkStatus status;


	while (!pThis->m_bExit)
	{   

		if(CameraGetImageBuffer(pThis->m_hCamera,&sFrameInfo,&pbyBuffer,1000) == CAMERA_STATUS_SUCCESS)
		{	
			//����õ�ԭʼ����ת����RGB��ʽ�����ݣ�ͬʱ����ISPģ�飬��ͼ����н��룬������������ɫУ���ȴ���
			//�ҹ�˾�󲿷��ͺŵ������ԭʼ���ݶ���Bayer��ʽ��
			status = CameraImageProcess(pThis->m_hCamera, pbyBuffer, pThis->m_pFrameBuffer,&sFrameInfo);//����ģʽ

			//�ֱ��ʸı��ˣ���ˢ�±���
			if (pThis->m_sFrInfo.iWidth != sFrameInfo.iWidth || pThis->m_sFrInfo.iHeight != sFrameInfo.iHeight)
			{
				pThis->m_sFrInfo.iWidth = sFrameInfo.iWidth;
				pThis->m_sFrInfo.iHeight = sFrameInfo.iHeight;
				pThis->Invalidate();

				if (!pThis->m_bScaleDisp)
				{
					pThis->ResetScrollBar(sFrameInfo.iWidth, sFrameInfo.iHeight);
				}
			}

			if(status == CAMERA_STATUS_SUCCESS)
			{
				//����SDK��װ�õ���ʾ�ӿ�����ʾͼ��,��Ҳ���Խ�m_pFrameBuffer�е�RGB����ͨ��������ʽ��ʾ������directX,OpengGL,�ȷ�ʽ��
				CameraImageOverlay(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
				CameraDisplayRGB24(pThis->m_hCamera, pThis->m_pFrameBuffer, &sFrameInfo);
				pThis->m_iDispFrameNum++;
			}    

			//�ڳɹ�����CameraGetImageBuffer�󣬱������CameraReleaseImageBuffer���ͷŻ�õ�buffer��
			//�����ٴε���CameraGetImageBufferʱ�����򽫱�����֪�������߳��е���CameraReleaseImageBuffer���ͷ���buffer
			CameraReleaseImageBuffer(pThis->m_hCamera,pbyBuffer);

			memcpy(&pThis->m_sFrInfo,&sFrameInfo,sizeof(tSdkFrameHead));
		}

	}

	return 0;
}

void CCameraDisplayDlg::OnBnClickedButtonSettings()
{
	CameraShowSettingPage(m_hCamera, TRUE);
}

void CCameraDisplayDlg::OnBnClickedRadioScale()
{
	ShowScrollBar(SB_BOTH, FALSE);
	CameraSetDisplayMode(m_hCamera, DISPLAYMODE_SCALE);
	m_pDisp->Invalidate();
	m_bScaleDisp = TRUE;
}

void CCameraDisplayDlg::OnBnClickedRadioDirect()
{
	ShowScrollBar(SB_BOTH, TRUE);
	ResetScrollBar(m_sFrInfo.iWidth, m_sFrInfo.iHeight);
	CameraSetDisplayMode(m_hCamera, DISPLAYMODE_REAL);
	m_pDisp->Invalidate();
	m_bScaleDisp = FALSE;
}

void CCameraDisplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos = 0;
	int maxpos = 0;
	GetScrollRange(SB_HORZ, &minpos, &maxpos);

	// Get the current position of scroll box.  
	int curpos = GetScrollPos(SB_HORZ);  

	// Determine the new position of scroll box.  
	switch (nSBCode)  
	{  
	case SB_LEFT:      // Scroll to far left.  
		curpos = minpos;  
		break;  

	case SB_RIGHT:      // Scroll to far right.  
		curpos = maxpos;  
		break;  

	case SB_ENDSCROLL:   // End scroll.  
		break;  

	case SB_LINELEFT:      // Scroll left.  
		curpos = max(minpos, curpos - 10);  
		break;  

	case SB_LINERIGHT:   // Scroll right.  
		curpos = min(maxpos, curpos + 10);
		break;

	case SB_PAGELEFT:    // Scroll one page left.  
		if (curpos > minpos)  
			curpos = max(minpos, curpos - 100);  
		break;  

	case SB_PAGERIGHT:      // Scroll one page right.  
		if (curpos < maxpos)  
			curpos = min(maxpos, curpos + 100);
		break;  

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position  
		curpos = nPos;      // of the scroll box at the end of the drag operation.  
		break;  

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the  
		curpos = nPos;     // position that the scroll box has been dragged to.  
		break;  
	}

	SetScrollPos(SB_HORZ, curpos);
	CameraSetDisplayOffset(m_hCamera, curpos, -1);

	CDialogEx::OnHScroll(nSBCode, curpos, pScrollBar);
}

void CCameraDisplayDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos = 0;
	int maxpos = 0;
	GetScrollRange(SB_VERT, &minpos, &maxpos);

	// Get the current position of scroll box.  
	int curpos = GetScrollPos(SB_VERT);  

	// Determine the new position of scroll box.  
	switch (nSBCode)  
	{  
	case SB_LEFT:      // Scroll to far left.  
		curpos = minpos;  
		break;  

	case SB_RIGHT:      // Scroll to far right.  
		curpos = maxpos;  
		break;  

	case SB_ENDSCROLL:   // End scroll.  
		break;  

	case SB_LINELEFT:      // Scroll left.  
		curpos = max(minpos, curpos - 10);  
		break;  

	case SB_LINERIGHT:   // Scroll right.  
		curpos = min(maxpos, curpos + 10);
		break;

	case SB_PAGELEFT:    // Scroll one page left.  
		if (curpos > minpos)  
			curpos = max(minpos, curpos - 100);  
		break;  

	case SB_PAGERIGHT:      // Scroll one page right.  
		if (curpos < maxpos)  
			curpos = min(maxpos, curpos + 100);
		break;  

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position  
		curpos = nPos;      // of the scroll box at the end of the drag operation.  
		break;  

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the  
		curpos = nPos;     // position that the scroll box has been dragged to.  
		break;  
	}

	SetScrollPos(SB_VERT, curpos);
	CameraSetDisplayOffset(m_hCamera, -1, curpos);

	CDialogEx::OnVScroll(nSBCode, curpos, pScrollBar);
}
