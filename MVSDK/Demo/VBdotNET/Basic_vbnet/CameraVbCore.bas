Imports System.IO
'BIG5 TRANS ALLOWED
Public Module CameraVbCore

    Public m_hCamera As Integer        '������
    Public m_hBufferHandle As Integer  '��ʾ���������
    Public m_pFrameBuffer As Integer   '��ʾ�������׵�ַ
    Public m_iLastImageWidth As Integer
    Public m_iLastImageHeight As Integer
    Public m_iDisplayCounts As Integer '��¼��ǰ��ʾ����֡��
    Public m_CapCallback As CAMERA_SNAP_PROC
    'ʵ������malloc�Ĳ��������ڶ�̬�����ڴ�
    Public Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Integer, ByVal dwBytes As Integer) As Integer
    Public Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Integer) As Integer
    Public Declare Function GlobalUnlock Lib "kernel32" (ByVal hMem As Integer) As Integer
    Public Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Integer) As Integer

    '����һ֡ͼ����ʾ
    Public Function CaptureImage() As Integer

        Dim sFrameInfo As tSdkFrameHead
        Dim pRawBuffer As Integer          'ԭʼ���ݵĻ�������ַ����SDK�л��
        Dim iRet As Integer

        iRet = CameraGetImageBuffer(m_hCamera, sFrameInfo, pRawBuffer, 200) 'ץȡһ֡ͼ�񣬳�ʱʱ��Ϊ200����


        If iRet = CAMERA_STATUS_SUCCESS Then
            If (m_iLastImageWidth <> sFrameInfo.iWidth) Or (m_iLastImageHeight <> sFrameInfo.iHeight) Then
                m_iLastImageWidth = sFrameInfo.iWidth
                m_iLastImageHeight = sFrameInfo.iHeight
                Form1.PictureBox1.Refresh()
            End If

            '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
            iRet = CameraImageProcess(m_hCamera, pRawBuffer, m_pFrameBuffer, sFrameInfo)

            If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
                iRet = CameraDisplayRGB24(m_hCamera, m_pFrameBuffer, sFrameInfo)
            End If

            iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer

            m_iDisplayCounts = m_iDisplayCounts + 1
        End If

        CaptureImage = CAMERA_STATUS_SUCCESS
    End Function

    Public Sub CaptureImageCallbackFunc(ByVal hCamera As Integer, _
            ByVal pRawBuffer As Integer, _
            ByRef sFrameInfo As tSdkFrameHead, _
            ByVal Context As Integer)

        Dim iRet As Integer

        If (m_iLastImageWidth <> sFrameInfo.iWidth) Or (m_iLastImageHeight <> sFrameInfo.iHeight) Then
            m_iLastImageWidth = sFrameInfo.iWidth
            m_iLastImageHeight = sFrameInfo.iHeight
            Form1.PictureBox1.Refresh()
        End If

        '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
        iRet = CameraImageProcess(m_hCamera, pRawBuffer, m_pFrameBuffer, sFrameInfo)

        If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
            iRet = CameraDisplayRGB24(m_hCamera, m_pFrameBuffer, sFrameInfo)
        End If

        m_iDisplayCounts = m_iDisplayCounts + 1

    End Sub

    '��ץ��ģʽ����һ֡ͼ�񲢱��浽�ļ���,Ҳ����ֱ�ӽ�CameraGetImageBuffer�õ�ͼ����д�����߱��档
    Public Function Snapshot() As Integer
        Dim FrameInfo As tSdkFrameHead
        Dim pRawBuffer As Integer
        Dim pRgbBuffer As Integer
        Dim pBufferHandle As Integer
        Dim sFileName As String
        Dim sImageSize As tSdkImageResolution
        Dim iRet As Integer
        Dim sTmp() As Byte
  

        sImageSize.iWidth = 0
        sImageSize.iHeight = 0
        sImageSize.iIndex = &HFF

        '//CameraSetResolutionForSnap����ץ��ʱ�ķֱ��ʣ����Ժ�Ԥ��ʱ��ͬ��Ҳ���Բ�ͬ��
        '//sImageSize.iIndex = 0xff; sImageSize.iWidth �� sImageSize.iHeight ��0����ʾץ��ʱ�ֱ��ʺ�Ԥ��ʱ��ͬ��
        '//�����ϣ��ץ��ʱΪ�����ķֱ��ʣ���ο����ǵ�Snapshot���̡����߲���SDK�ֲ����й�CameraSetResolutionForSnap�ӿڵ���ϸ˵��
        iRet = CameraSetResolutionForSnap(m_hCamera, sImageSize)

        ' //  CameraSnapToBufferץ��һ֡ͼ�����ݵ��������У��û�������SDK�ڲ�����,�ɹ����ú���Ҫ
		'// !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
        iRet = CameraSnapToBuffer(m_hCamera, FrameInfo, pRawBuffer, 1000)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            MsgBox("Snapshot failed,is camera in pause mode?")
        Else
            '//�ɹ�ץ�ĺ󣬱��浽�ļ�

            sFileName = Application.StartupPath() + "\" + Format$(Now, "yyyy-mm-dd-h-mm-ss") + ".BMP" '//��ʼ�������ļ����ļ���

            '//����һ��buffer����������õ�ԭʼ����ת��ΪRGB���ݣ���ͬʱ���ͼ����Ч��
            pRgbBuffer = CameraAlignMalloc(FrameInfo.iWidth * FrameInfo.iHeight * 4, 16)

            '//CameraImageProcess�õ�RGB��ʽ������,�����Ҫ����RAW���ݣ���ֱ�ӵ���CameraSaveImage���ļ�����ѡ��RAW��ʽ����
            iRet = CameraImageProcess(m_hCamera, pRawBuffer, pRgbBuffer, FrameInfo)


            '��VB��string����ת��ΪC���ַ������������ַ���ĩβ��ӽ�����
            sTmp = System.Text.Encoding.Default.GetBytes(sFileName)
            sTmp(UBound(sTmp)) = 0

            iRet = CameraSaveImage(m_hCamera, sTmp(0), pRgbBuffer, FrameInfo, 2, 100) '1:JPG 2:BMP 4:RAW 8:PNG

            If iRet = CAMERA_STATUS_SUCCESS Then
                MsgBox("Save image OK! The file is:[" + sFileName + "]OK!")
            Else
                MsgBox("Save image failed")
            End If

            iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer)

            CameraAlignFree(pRgbBuffer)

        End If

        Return iRet
    End Function


    '��ʼ�����
    Public Function InitCamera(ByRef pCameraInfo As tSdkCameraDevInfo, ByVal hDisplayWnd As Integer, ByVal iDisWidth As Integer, ByVal iDisHeight As Integer) As Integer
        Dim iRet As Integer
        Dim sCameraInfo As tSdkCameraCapbility

        m_iDisplayCounts = 0

        iRet = CameraInit(pCameraInfo, -1, -1, m_hCamera)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ø��������������,����һ���ڴ���������ͼ������ʾ

        iRet = CameraGetCapability(m_hCamera, sCameraInfo)

        m_pFrameBuffer = CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iHeightMax * 4, 16)

        If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
            CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8)
        End If

        '��ʼ����ʾģ��
        iRet = CameraDisplayInit(m_hCamera, hDisplayWnd)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        iRet = CameraSetDisplaySize(m_hCamera, iDisWidth, iDisHeight)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If


        '�����豸���ô���
        iRet = CameraCreateSettingPage(m_hCamera, Form1.Handle, pCameraInfo.acFriendlyName(0), 0, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ʼ����
        iRet = CameraPlay(m_hCamera)

        Return iRet

    End Function

End Module
