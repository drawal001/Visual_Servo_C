
// DrawTextDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawText.h"
#include "DrawTextDlg.h"
#include "afxdialogex.h"
#include "TextInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawTextDlg �Ի���




CDrawTextDlg::CDrawTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDrawTextDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera = -1;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pFrameBuffer = NULL;
	m_pTextInfoDlg = NULL;
}

void CDrawTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawTextDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDrawTextDlg ��Ϣ�������

BOOL CDrawTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pDisp = GetDlgItem(IDC_STATIC_DISP);

	CameraSdkInit(1);
	if (!InitCamera())
	{
		EndDialog(0);
		return TRUE;
	}

	m_pTextInfoDlg = new CTextInfoDlg();
	m_pTextInfoDlg->Create(CTextInfoDlg::IDD);
	m_pTextInfoDlg->ShowWindow(SW_SHOW);

	SetTimer(100, 1000, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDrawTextDlg::OnPaint()
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
HCURSOR CDrawTextDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDrawTextDlg::OnTimer(UINT_PTR nIDEvent)
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
	strStatusText.Format("| ͼ��ֱ���:%d*%d | ��ʾ֡��:%4.2f FPS | ����֡��:%4.2f FPS |",
		m_sFrInfo.iWidth, m_sFrInfo.iHeight,
		m_fDispFps, m_fCapFps);	
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strStatusText);

	m_sFrameLast.iCapture = m_sFrameCount.iCapture;
	m_sFrameLast.iLost = m_sFrameCount.iLost;
	m_sFrameLast.iTotal = m_sFrameCount.iTotal;
	iDispNum = m_iDispFrameNum;
}

void CDrawTextDlg::OnDestroy()
{
	if (m_pTextInfoDlg != NULL)
	{
		m_pTextInfoDlg->DestroyWindow();
		delete m_pTextInfoDlg;
		m_pTextInfoDlg = NULL;
	}

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

BOOL CDrawTextDlg::InitCamera()
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

	//ʹ��SDK��װ�õ���ʾ�ӿ�
	//Use SDK to display camera images.
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
	return TRUE;
}

UINT WINAPI CDrawTextDlg::uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CDrawTextDlg* 		pThis = (CDrawTextDlg*)lpParam;
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
			}

			if(status == CAMERA_STATUS_SUCCESS)
			{
				pThis->m_csTextItemArray.Lock();
				for (int i = 0; i < pThis->m_TextItemArray.GetSize(); ++i)
				{
					TextItem* pItem = &pThis->m_TextItemArray[i];
					CameraDrawText(pThis->m_pFrameBuffer, &sFrameInfo, pItem->strFontFileName,
						pItem->uFontW, pItem->uFontH, pItem->strText, pItem->rcOut.left, pItem->rcOut.top,
						pItem->rcOut.Width(), pItem->rcOut.Height(), pItem->uColor, pItem->uFlags);
				}
				pThis->m_csTextItemArray.Unlock();

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
