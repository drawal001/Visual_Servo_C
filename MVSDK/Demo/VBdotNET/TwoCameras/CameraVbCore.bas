'BIG5 TRANS ALLOWED


Module CameraVbCore
    '���1����
    Public m_hCamera1 As Integer        '������
    Public m_hBufferHandle1 As Integer  '��ʾ���������
    Public m_pFrameBuffer1 As Integer   '��ʾ�������׵�ַ
    Public m_iLastImageWidth1 As Integer
    Public m_iLastImageHeight1 As Integer
    Public m_iDisplayCounts1 As Integer '��¼��ǰ��ʾ����֡��

    '���2����
    Public m_hCamera2 As Integer        '������
    Public m_hBufferHandle2 As Integer  '��ʾ���������
    Public m_pFrameBuffer2 As Integer   '��ʾ�������׵�ַ
    Public m_iLastImageWidth2 As Integer
    Public m_iLastImageHeight2 As Integer
    Public m_iDisplayCounts2 As Integer '��¼��ǰ��ʾ����֡��

    'ʵ������malloc�Ĳ��������ڶ�̬�����ڴ�
    Public Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Integer, ByVal dwBytes As Integer) As Integer
    Public Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Integer) As Integer
    Public Declare Function GlobalUnlock Lib "kernel32" (ByVal hMem As Integer) As Integer
    Public Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Integer) As Integer

    '���1����һ֡ͼ����ʾ
    Public Function CaptureImage1() As Integer

        Dim sFrameInfo As tSdkFrameHead
        Dim pRawBuffer As Integer          'ԭʼ���ݵĻ�������ַ����SDK�л��
        Dim iRet As Integer

        iRet = CameraGetImageBuffer(m_hCamera1, sFrameInfo, pRawBuffer, 200) 'ץȡһ֡ͼ�񣬳�ʱʱ��Ϊ200����


        If iRet = CAMERA_STATUS_SUCCESS Then
            If (m_iLastImageWidth1 <> sFrameInfo.iWidth) Or (m_iLastImageHeight1 <> sFrameInfo.iHeight) Then
                m_iLastImageWidth1 = sFrameInfo.iWidth
                m_iLastImageHeight1 = sFrameInfo.iHeight
                Form1.PictureBox1.Refresh()
            End If

            '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
            iRet = CameraImageProcess(m_hCamera1, pRawBuffer, m_pFrameBuffer1, sFrameInfo)

            If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
                iRet = CameraDisplayRGB24(m_hCamera1, m_pFrameBuffer1, sFrameInfo)
            End If

            iRet = CameraReleaseImageBuffer(m_hCamera1, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer

            m_iDisplayCounts1 = m_iDisplayCounts1 + 1
        End If

        CaptureImage1 = CAMERA_STATUS_SUCCESS
    End Function

    '���1����һ֡ͼ����ʾ
    Public Function CaptureImage2() As Integer

        Dim sFrameInfo As tSdkFrameHead
        Dim pRawBuffer As Integer          'ԭʼ���ݵĻ�������ַ����SDK�л��
        Dim iRet As Integer

        iRet = CameraGetImageBuffer(m_hCamera2, sFrameInfo, pRawBuffer, 200) 'ץȡһ֡ͼ�񣬳�ʱʱ��Ϊ200����


        If iRet = CAMERA_STATUS_SUCCESS Then
            If (m_iLastImageWidth2 <> sFrameInfo.iWidth) Or (m_iLastImageHeight2 <> sFrameInfo.iHeight) Then
                m_iLastImageWidth2 = sFrameInfo.iWidth
                m_iLastImageHeight2 = sFrameInfo.iHeight
                Form1.PictureBox1.Refresh()
            End If

            '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
            iRet = CameraImageProcess(m_hCamera2, pRawBuffer, m_pFrameBuffer2, sFrameInfo)

            If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
                iRet = CameraDisplayRGB24(m_hCamera2, m_pFrameBuffer2, sFrameInfo)
            End If

            iRet = CameraReleaseImageBuffer(m_hCamera2, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer

            m_iDisplayCounts2 = m_iDisplayCounts2 + 1
        End If

        CaptureImage2 = CAMERA_STATUS_SUCCESS
    End Function

        '��ʼ�����1
    Public Function InitCamera1(ByVal hDisplayWnd As Integer, ByVal iDisWidth As Integer, ByVal iDisHeight As Integer) As Integer
        Dim iRet As Integer
        Dim sCameraInfo As tSdkCameraCapbility

        m_iDisplayCounts1 = 0

        'iRet = CameraInitEx2("camera1", m_hCamera1) '�����ִ����
        iRet = CameraInitEx(0, -1, -1, m_hCamera1) '����Ŵ�

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ø��������������,����һ���ڴ���������ͼ������ʾ

        iRet = CameraGetCapability(m_hCamera1, sCameraInfo)

        m_hBufferHandle1 = GlobalAlloc(2, sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iHeightMax * 3)
        m_pFrameBuffer1 = GlobalLock(m_hBufferHandle1)

        If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
            CameraSetIspOutFormat(m_hCamera1, CAMERA_MEDIA_TYPE_MONO8)
        End If

        '��ʼ����ʾģ��
        iRet = CameraDisplayInit(m_hCamera1, hDisplayWnd)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        iRet = CameraSetDisplaySize(m_hCamera1, iDisWidth, iDisHeight)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If


        '�����豸���ô���
        iRet = CameraCreateSettingPage(m_hCamera1, Form1.Handle, StringToBytes("Camera1")(0), 0, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ʼ����
        iRet = CameraPlay(m_hCamera1)

        Return iRet

    End Function
    '��ʼ�����2
    Public Function InitCamera2(ByVal hDisplayWnd As Integer, ByVal iDisWidth As Integer, ByVal iDisHeight As Integer) As Integer
        Dim iRet As Integer
        Dim sCameraInfo As tSdkCameraCapbility

        m_iDisplayCounts2 = 0


        'iRet = CameraInitEx2("camera2", m_hCamera2) '�����ִ����
        iRet = CameraInitEx(1, -1, -1, m_hCamera2) '����Ŵ�

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ø��������������,����һ���ڴ���������ͼ������ʾ

        iRet = CameraGetCapability(m_hCamera2, sCameraInfo)

        m_hBufferHandle2 = GlobalAlloc(2, sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iHeightMax * 3)
        m_pFrameBuffer2 = GlobalLock(m_hBufferHandle2)

        If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
            CameraSetIspOutFormat(m_hCamera2, CAMERA_MEDIA_TYPE_MONO8)
        End If

        '��ʼ����ʾģ��
        iRet = CameraDisplayInit(m_hCamera2, hDisplayWnd)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        iRet = CameraSetDisplaySize(m_hCamera2, iDisWidth, iDisHeight)

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If


        '�����豸���ô���
        iRet = CameraCreateSettingPage(m_hCamera2, Form1.Handle, StringToBytes("Camera2")(0), 0, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

        If iRet <> CAMERA_STATUS_SUCCESS Then
            Return iRet
        End If

        '��ʼ����
        iRet = CameraPlay(m_hCamera2)

        Return iRet

    End Function

End Module
