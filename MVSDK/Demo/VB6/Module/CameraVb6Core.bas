Attribute VB_Name = "CameraVbCore"
'///BIG5 TRANS ALLOWED


Public m_hCamera As Long        '������
Public m_hCamera2 As Long        '������
Public m_pFrameBuffer As Long   '��ʾ�������׵�ַ
Public m_pFrameBuffer2 As Long   '��ʾ�������׵�ַ
Public m_iLastImageWidth As Long
Public m_iLastImageHeight As Long
Public m_iDisplayCounts As Long '��¼��ǰ��ʾ����֡��
'ʵ������malloc�Ĳ��������ڶ�̬�����ڴ�
Public Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, ByVal dwBytes As Long) As Long
Public Declare Function GlobalLock Lib "kernel32" (ByVal hMem As Long) As Long
Public Declare Function GlobalUnlock Lib "kernel32" (ByVal hMem As Long) As Long
Public Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Long) As Long


'����һ֡ͼ����ʾ
Public Function CaptureImage() As Long
 
Dim sFrameInfo As tSdkFrameHead
Dim pRawBuffer As Long          'ԭʼ���ݵĻ�������ַ����SDK�л��
Dim iRet As Long
Dim Image As Long
Dim Pic As IPictureDisp
            
     '������ʾ���1ͼ��
    iRet = CameraGetImageBuffer(m_hCamera, sFrameInfo, pRawBuffer, 200) 'ץȡһ֡ͼ�񣬳�ʱʱ��Ϊ200����
    
    
    If iRet = CAMERA_STATUS_SUCCESS Then
       
        
        '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
        iRet = CameraImageProcess(m_hCamera, pRawBuffer, m_pFrameBuffer, sFrameInfo)
        
         If (m_iLastImageWidth <> sFrameInfo.iWidth) Or (m_iLastImageHeight <> sFrameInfo.iHeight) Then
            m_iLastImageWidth = sFrameInfo.iWidth
            m_iLastImageHeight = sFrameInfo.iHeight
            Form1.ImageDisplayer.Refresh
        End If
        
        If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
            iRet = CameraDisplayRGB24(m_hCamera, m_pFrameBuffer, sFrameInfo)
        End If
        
        iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer
        
        
    End If
    
    
     '������ʾ���2ͼ��
    iRet = CameraGetImageBuffer(m_hCamera2, sFrameInfo, pRawBuffer, 200) 'ץȡһ֡ͼ�񣬳�ʱʱ��Ϊ200����
    
    
    If iRet = CAMERA_STATUS_SUCCESS Then
       
        
        '��ԭʼ����ת��ΪRGB��ʽ��ͬʱ����ͼ�����Ч��
        iRet = CameraImageProcess(m_hCamera2, pRawBuffer, m_pFrameBuffer2, sFrameInfo)
        
        
        If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
            iRet = CameraDisplayRGB24(m_hCamera2, m_pFrameBuffer2, sFrameInfo)
        End If
        
        iRet = CameraReleaseImageBuffer(m_hCamera2, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer
        
        
    End If
    
    m_iDisplayCounts = m_iDisplayCounts + 1
    CaptureImage = CAMERA_STATUS_SUCCESS
End Function
'��ץ��ģʽ����һ֡ͼ�񲢱��浽�ļ���
Public Function Snapshot() As Long
    Dim FrameInfo As tSdkFrameHead
    Dim pRawBuffer As Long
    Dim pRgbBuffer As Long
    Dim pBufferHandle As Long
    Dim sFileName As String
    Dim sImageSize As tSdkImageResolution
    Dim iRet As Long
    Dim msg As String
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
        MsgBox ("Snapshot failed,is camera in pause mode?")
    Else
        '//�ɹ�ץ�ĺ󣬱��浽�ļ�
        
        sFileName = App.Path + "\" + Format$(Now, "yyyy-mm-dd-h-mm-ss") + ".BMP" '//��ʼ�������ļ����ļ���
 
        '//����һ��buffer����������õ�ԭʼ����ת��ΪRGB���ݣ���ͬʱ���ͼ����Ч��
        pRgbBuffer = CameraAlignMalloc(FrameInfo.iWidth * FrameInfo.iHeight * 4, 16)
       
        
        '//CameraImageProcess�õ�RGB��ʽ������,�����Ҫ����RAW���ݣ���ֱ�ӵ���CameraSaveImage���ļ�����ѡ��RAW��ʽ����
        iRet = CameraImageProcess(m_hCamera, pRawBuffer, pRgbBuffer, FrameInfo)
        
        
        '��VB��string����ת��ΪC���ַ������������ַ���ĩβ��ӽ�����
        sTmp = StrConv(sFileName, vbFromUnicode)
        ReDim Preserve sTmp(UBound(sTmp) + 1)
        sTmp(UBound(sTmp)) = 0
        
        iRet = CameraSaveImage(m_hCamera, sTmp(0), pRgbBuffer, FrameInfo, 2, 100) '1:JPG 2:BMP 4:RAW 8:PNG
        
        If iRet = CAMERA_STATUS_SUCCESS Then
            MsgBox ("Save image OK! The file is:[" + sFileName + "]OK!")
        Else
            MsgBox ("Save image failed")
        End If
        
        iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer)
        
        CameraAlignFree (pRgbBuffer)
        
        GlobalUnlock (pBufferHandle)   '�ͷ�������ڴ��
        GlobalFree (pBufferHandle)
        
    End If
End Function



'������ô��ڵ���Ϣ�ص���������ͨ�����ô����л�������ʱ���ᴥ����Ӧ����Ϣ����ͨ���˻ص��˺�����������Ϣ��������������ص������У���������ͬ����
'hCamera: ��ǰ����ľ��
'MSG:��Ϣ���ͣ�
   ' SHEET_MSG_LOAD_PARAM_DEFAULT    = 0,//����Ĭ�ϲ����İ�ť�����������Ĭ�ϲ�����ɺ󴥷�����Ϣ,
   ' SHEET_MSG_LOAD_PARAM_GROUP      = 1,//�л���������ɺ󴥷�����Ϣ,
   ' SHEET_MSG_LOAD_PARAM_FROMFILE   = 2,//���ز�����ť��������Ѵ��ļ��м�����������󴥷�����Ϣ
   ' SHEET_MSG_SAVE_PARAM_GROUP      = 3//���������ť���������������󴥷�����Ϣ
   ' ����μ�CameraDefine.bas��emSdkPropSheetMsg����

'uParam:��Ϣ�����Ĳ�������ͬ����Ϣ���������岻ͬ��
   ' �� MSG Ϊ SHEET_MSG_LOAD_PARAM_DEFAULTʱ��uParam��ʾ�����س�Ĭ�ϲ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
   ' �� MSG Ϊ SHEET_MSG_LOAD_PARAM_GROUPʱ��uParam��ʾ�л���Ĳ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
   ' �� MSG Ϊ SHEET_MSG_LOAD_PARAM_FROMFILEʱ��uParam��ʾ���ļ��в������ǵĲ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
   ' �� MSG Ϊ SHEET_MSG_SAVE_PARAM_GROUPʱ��uParam��ʾ��ǰ����Ĳ�����������ţ���0��ʼ���ֱ��ӦA,B,C,D����
Public Sub CameraConfigWinMsgCallBack(ByVal hCamera As Long, ByVal msg As Long, ByVal uParam As Long, ByVal pContext As Long)
    
   
End Sub

'��ʼ�����
Public Function InitCamera(ByRef pCameraInfo As tSdkCameraDevInfo, ByVal hDisplayWnd As Long, ByVal iDisWidth As Long, ByVal iDisHeight As Long) As Integer
Dim iRet As Long
Dim sCameraInfo As tSdkCameraCapbility

m_iDisplayCounts = 0

iRet = CameraInit(pCameraInfo, -1, -1, m_hCamera)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera = iRet
    Exit Function
End If

'��ø��������������,����һ���ڴ���������ͼ������ʾ
   
iRet = CameraGetCapability(m_hCamera, sCameraInfo)

m_pFrameBuffer = CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iWidthMax * 4, 16)

If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
    iRet = CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8)
End If

'��ʼ����ʾģ��
iRet = CameraDisplayInit(m_hCamera, hDisplayWnd)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera = iRet
    Exit Function
End If

iRet = CameraSetDisplaySize(m_hCamera, iDisWidth, iDisHeight)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera = iRet
    Exit Function
End If


'�����豸���ô���
iRet = CameraCreateSettingPage(m_hCamera, Form1.hWnd, pCameraInfo.acFriendlyName(0), AddressOf CameraConfigWinMsgCallBack, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera = iRet
    Exit Function
End If


'��ʼ����
iRet = CameraPlay(m_hCamera)
If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera = iRet
    Exit Function
End If




InitCamera = iRet
End Function


'��ʼ�����
Public Function InitCameraForEvision(ByRef pCameraInfo As tSdkCameraDevInfo) As Integer
Dim iRet As Long
Dim sCameraInfo As tSdkCameraCapbility

m_iDisplayCounts = 0

iRet = CameraInit(pCameraInfo, -1, -1, m_hCamera)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCameraForEvision = iRet
    Exit Function
End If

'��ø��������������,����һ���ڴ���������ͼ������ʾ
   
iRet = CameraGetCapability(m_hCamera, sCameraInfo)

m_pFrameBuffer = CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iWidthMax * 4, 16)




'�����豸���ô���
iRet = CameraCreateSettingPage(m_hCamera, Form1.hWnd, pCameraInfo.acFriendlyName(0), AddressOf CameraConfigWinMsgCallBack, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCameraForEvision = iRet
    Exit Function
End If

'���ô�ֱ��ת��evision����ʾ�ؼ������Ƿ���
iRet = CameraSetMirror(m_hCamera, 1, 1)

'��ʼ����
iRet = CameraPlay(m_hCamera)
If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCameraForEvision = iRet
    Exit Function
End If




InitCameraForEvision = iRet
End Function


'����������֣���ʼ�����1
Public Function InitCamera1ByNames(ByRef name As Byte, ByVal hDisplayWnd As Long, ByVal iDisWidth As Long, ByVal iDisHeight As Long) As Integer
Dim iRet As Long
Dim sCameraInfo As tSdkCameraCapbility

m_iDisplayCounts = 0

iRet = CameraInitEx2(name, m_hCamera)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera1ByNames = iRet
    Exit Function
End If

'��ø��������������,����һ���ڴ���������ͼ������ʾ
   
iRet = CameraGetCapability(m_hCamera, sCameraInfo)

m_pFrameBuffer = CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iWidthMax * 4, 16)

If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
    iRet = CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8)
End If

'��ʼ����ʾģ��
iRet = CameraDisplayInit(m_hCamera, hDisplayWnd)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera1ByNames = iRet
    Exit Function
End If

iRet = CameraSetDisplaySize(m_hCamera, iDisWidth, iDisHeight)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera1ByNames = iRet
    Exit Function
End If


'�����豸���ô���
iRet = CameraCreateSettingPage(m_hCamera, Form1.hWnd, name, AddressOf CameraConfigWinMsgCallBack, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera1ByNames = iRet
    Exit Function
End If


'��ʼ����
iRet = CameraPlay(m_hCamera)
If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera1ByNames = iRet
    Exit Function
End If




InitCamera1ByNames = iRet
End Function

'����������֣���ʼ�����2
Public Function InitCamera2ByNames(ByRef name As Byte, ByVal hDisplayWnd As Long, ByVal iDisWidth As Long, ByVal iDisHeight As Long) As Integer
Dim iRet As Long
Dim sCameraInfo As tSdkCameraCapbility

m_iDisplayCounts = 0

iRet = CameraInitEx2(name, m_hCamera2)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera2ByNames = iRet
    Exit Function
End If

'��ø��������������,����һ���ڴ���������ͼ������ʾ
   
iRet = CameraGetCapability(m_hCamera2, sCameraInfo)

m_pFrameBuffer2 = CameraAlignMalloc(sCameraInfo.sResolutionRange.iWidthMax * sCameraInfo.sResolutionRange.iWidthMax * 4, 16)

If sCameraInfo.sIspCapacity.bMonoSensor <> 0 Then
    iRet = CameraSetIspOutFormat(m_hCamera2, CAMERA_MEDIA_TYPE_MONO8)
End If

'��ʼ����ʾģ��
iRet = CameraDisplayInit(m_hCamera2, hDisplayWnd)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera2ByNames = iRet
    Exit Function
End If

iRet = CameraSetDisplaySize(m_hCamera2, iDisWidth, iDisHeight)

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera2ByNames = iRet
    Exit Function
End If


'�����豸���ô���
iRet = CameraCreateSettingPage(m_hCamera2, Form1.hWnd, name, AddressOf CameraConfigWinMsgCallBack, &H20121111, 0) '&H20121111��CameraConfigWinMsgCallBack�ص�ʱ�����أ���ʾ��δ����Զ������

If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera2ByNames = iRet
    Exit Function
End If


'��ʼ����
iRet = CameraPlay(m_hCamera2)
If iRet <> CAMERA_STATUS_SUCCESS Then
    InitCamera2ByNames = iRet
    Exit Function
End If




InitCamera2ByNames = iRet
End Function
