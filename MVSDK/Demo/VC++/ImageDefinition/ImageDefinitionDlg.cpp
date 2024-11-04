
// ImageDefinitionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageDefinition.h"
#include "ImageDefinitionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageDefinitionDlg �Ի���
enum
{
	WM_SET_DEF_VAL = WM_USER + 1000,
};




CImageDefinitionDlg::CImageDefinitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageDefinitionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCamera = -1;
	m_iDispFrameNum = 0;
	m_bExit = FALSE;
	m_hDispThread = NULL;
	m_pFrameBuffer = NULL;
}

void CImageDefinitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageDefinitionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CImageDefinitionDlg::OnBnClickedButtonSettings)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_ALG, &CImageDefinitionDlg::OnCbnSelchangeComboAlg)
	ON_MESSAGE(WM_SET_DEF_VAL, &CImageDefinitionDlg::OnSetDefValue)
END_MESSAGE_MAP()


// CImageDefinitionDlg ��Ϣ�������

BOOL CImageDefinitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pDisp = GetDlgItem(IDC_STATIC_IMAGE);

	m_iAlg = 0;
	((CComboBox*)GetDlgItem(IDC_COMBO_ALG))->SetCurSel(m_iAlg);

	CameraSdkInit(1);
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

void CImageDefinitionDlg::OnPaint()
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
HCURSOR CImageDefinitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CImageDefinitionDlg::InitCamera()
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
		MessageBox(msg);
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

void CImageDefinitionDlg::OnBnClickedButtonSettings()
{
	CameraShowSettingPage(m_hCamera, TRUE);
}

void CImageDefinitionDlg::OnTimer(UINT_PTR nIDEvent)
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

void CImageDefinitionDlg::OnDestroy()
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

UINT WINAPI CImageDefinitionDlg::uiDisplayThread(LPVOID lpParam)
{
	tSdkFrameHead 	sFrameInfo;
	CImageDefinitionDlg* 		pThis = (CImageDefinitionDlg*)lpParam;
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
				double def_val;
				if (CameraEvaluateImageDefinition(pThis->m_hCamera, pThis->m_iAlg, pThis->m_pFrameBuffer, &sFrameInfo, &def_val) == CAMERA_STATUS_SUCCESS)
				{
					pThis->PostMessage(WM_SET_DEF_VAL, *(DWORD*)&def_val, *((DWORD*)&def_val + 1));
				}

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

void CImageDefinitionDlg::OnCbnSelchangeComboAlg()
{
	m_iAlg = ((CComboBox*)GetDlgItem(IDC_COMBO_ALG))->GetCurSel();
}

LRESULT CImageDefinitionDlg::OnSetDefValue(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	double val;
	*(DWORD*)&val = (DWORD)wParam;
	*((DWORD*)&val + 1) = (DWORD)lParam;
	strText.Format("%.2lf", val);
	SetDlgItemText(IDC_EDIT_DEFINITION_VALUE, strText);
	return 0;
}
