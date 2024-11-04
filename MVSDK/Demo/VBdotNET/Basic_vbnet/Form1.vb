Imports System.IO

'BIG5 TRANS ALLOWED

Public Class Form1

    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        Dim iCameraNum As Long
        Dim CameraList() As tSdkCameraDevInfo
        Dim iRet As Integer

        '//ö���豸������豸�б�
        iRet = CameraEnumerateDevice(CameraList)
        iCameraNum = CameraList.GetLength(0)

        If iCameraNum < 1 Then
            m_hCamera = 0
            MsgBox("û��ɨ�赽����������������ٴ����б�����")
            Me.Close()
            Exit Sub
        Else

        End If

        iRet = InitCamera(CameraList(0), PictureBox1.Handle, PictureBox1.Width, PictureBox1.Height)

        If iRet <> 0 Then
            m_hCamera = 0
            MsgBox("Camera init error")
            Me.Close()
            Exit Sub
        Else
            Timer1.Enabled = True
            Timer1.Start()

            '��������������ʱ������ץͼ
            Timer2.Enabled = True
            Timer2.Start()

            '������������ץͼ�ص���ʹ�ûص��ķ�ʽץͼ
            'm_CapCallback = AddressOf CaptureImageCallbackFunc
            'CameraSetCallbackFunction(m_hCamera, m_CapCallback, 0, 0)

        End If
    End Sub

    Private Sub Button1_Click(sender As System.Object, e As System.EventArgs) Handles Button1.Click
        '��ʾ���ô���
        Dim iRet As Long
        iRet = CameraShowSettingPage(m_hCamera, 1)
    End Sub

    Private Sub Button2_Click(sender As System.Object, e As System.EventArgs) Handles Button2.Click
        '����
        CameraPlay(m_hCamera)
    End Sub

    Private Sub Button3_Click(sender As System.Object, e As System.EventArgs) Handles Button3.Click
        '��ͣ
        CameraPause(m_hCamera)
    End Sub

    Private Sub Button4_Click(sender As System.Object, e As System.EventArgs) Handles Button4.Click
        '���գ�����ͼƬ��ָ��·��
        Dim iRet As Long
        iRet = Snapshot()
    End Sub

    Private Sub Timer1_Tick(sender As System.Object, e As System.EventArgs) Handles Timer1.Tick
        Dim sFrameInfo As String
        sFrameInfo = "| Resolution " + Format$(m_iLastImageWidth, "General Number") + " X " + Format$(m_iLastImageHeight, "General Number") + " | Display frames " + Format$(m_iDisplayCounts, "General Number") + "|"
        Label1.Text = sFrameInfo
    End Sub

    Private Sub Timer2_Tick(sender As System.Object, e As System.EventArgs) Handles Timer2.Tick
        Dim iRet As Long
        iRet = CaptureImage()
    End Sub

    Private Sub Form1_FormClosing(sender As System.Object, e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        If m_hCamera <> 0 Then
            Timer1.Enabled = False
            Timer2.Enabled = False
            CameraUnInit(m_hCamera)         '����ʼ�����
            CameraAlignFree(m_pFrameBuffer)   '�ͷ�������ڴ��

        End If
    End Sub

    Private Sub SnapshotToBitmap_Click(sender As System.Object, e As System.EventArgs) Handles SnapshotToBitmap.Click
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


            '//����һ��buffer����������õ�ԭʼ����ת��ΪRGB���ݣ���ͬʱ���ͼ����Ч��
            pRgbBuffer = CameraAlignMalloc(FrameInfo.iWidth * FrameInfo.iHeight * 5, 16)


            '//CameraImageProcess�õ�RGB��ʽ������,�����Ҫ����RAW���ݣ���ֱ�ӵ���CameraSaveImage���ļ�����ѡ��RAW��ʽ����
            iRet = CameraImageProcess(m_hCamera, pRawBuffer, pRgbBuffer, FrameInfo)

            iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer)

            Form2.UpdateImage(pRgbBuffer, FrameInfo)

            CameraAlignFree(pRgbBuffer)   '�ͷ�������ڴ��

        End If


    End Sub

    Private Sub RWtest_Click(sender As System.Object, e As System.EventArgs) Handles RWtest.Click
        Form3.Show()

    End Sub
End Class
