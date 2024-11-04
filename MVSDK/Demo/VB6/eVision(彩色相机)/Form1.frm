VERSION 5.00
Object = "{9AE86E03-97BC-11D1-A49B-000021633168}#6.7#0"; "eVision.ocx"
Begin VB.Form Form1 
   Caption         =   "Camera VB Demo[Basic]"
   ClientHeight    =   8880
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   12210
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   592
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   814
   StartUpPosition =   3  '����ȱʡ
   Begin eVision.EC24Image EC24Image1 
      Height          =   6975
      Left            =   360
      TabIndex        =   4
      Top             =   840
      Width           =   11415
      _Version        =   393223
      _ExtentX        =   20135
      _ExtentY        =   12303
      _StockProps     =   0
      BeginProperty TextFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "System"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
   End
   Begin VB.Timer InfoUpdateTimer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   6600
      Top             =   240
   End
   Begin VB.Timer GrabTimer 
      Enabled         =   0   'False
      Interval        =   25
      Left            =   6000
      Top             =   240
   End
   Begin VB.CommandButton ButtonPlay 
      Caption         =   "Play"
      Height          =   375
      Index           =   1
      Left            =   1560
      TabIndex        =   3
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton CommandSetCamera 
      Caption         =   "Settings"
      Height          =   375
      Index           =   1
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   1215
   End
   Begin VB.CommandButton ButtonPause 
      Caption         =   "Pause"
      Height          =   375
      Index           =   0
      Left            =   2880
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
   Begin VB.Label StatusLabel 
      Caption         =   "Video Information"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   8280
      Width           =   11655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub MvCameraApiLoad()

End Sub

Private Sub ButtonPause_Click(index As Integer)
    CameraPause (m_hCamera)
End Sub

Private Sub ButtonPlay_Click(index As Integer)
    CameraPlay (m_hCamera)
End Sub

Private Sub CommandSetCamera_Click(index As Integer) '��ʾ������ô���
Dim iRet As Long
iRet = CameraShowSettingPage(m_hCamera, 1)

End Sub



Private Sub Form_Load()
Dim iCameraNum As Long
Dim CameraList(10) As tSdkCameraDevInfo
Dim iRet As Integer



'//ö���豸������豸�б�
iCameraNum = 10 '����CameraEnumerateDeviceǰ��������iCameraNums = 10����ʾ���ֻ��ȡ10���豸�������Ҫö�ٸ�����豸�������sCameraList����Ĵ�С��iCameraNums��ֵ
iRet = CameraEnumerateDevice(CameraList(0), iCameraNum)

If iCameraNum < 1 Then
            m_hCamera = 0
            MsgBox ("û��ɨ�赽����������������ٴ����б�����")
            Unload Me
            Exit Sub
Else
    
End If
    iRet = InitCameraForEvision(CameraList(0)) '��ʼ����һ������������2��������� CameraList(1)

If iRet <> 0 Then
    m_hCamera = 0
    MsgBox ("�����ʼ��ʧ��")
    Unload Me
    Exit Sub
Else
    'iRet = CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_MONO8) '��ɫ�����Ҫִ����䣬Ĭ�������24λ�Ĳ�ɫ��ʽ�ġ��ڰ�����Ͱ����Ｄ�
    GrabTimer.Enabled = True
    InfoUpdateTimer.Enabled = True
End If

End Sub

Private Sub Form_Unload(Cancel As Integer)

If m_hCamera <> 0 Then
           GrabTimer.Enabled = False
           InfoUpdateTimer.Enabled = False
           CameraUnInit (m_hCamera)         '����ʼ�����
           CameraAlignFree (m_pFrameBuffer)
End If

End Sub


'����һ֡ͼ����ʾ
Public Function CaptureImageForEvision() As Long
 
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
        
     
        If iRet = CAMERA_STATUS_SUCCESS Then '����ɹ�������ʾ��ͼ������ʹ����SDK��װ�Ľӿڽ���ͼ����ʾ����Ҳ�����Լ�ʵ��ͼ�����ʾ������ֱ����������ñ���ͼ��Ľӿڽ��б���
           ' iRet = CameraDisplayRGB24(m_hCamera, m_pFrameBuffer, sFrameInfo)
           EC24Image1.SetImagePointer m_pFrameBuffer
           EC24Image1.SetSize CLng(sFrameInfo.iWidth), CLng(sFrameInfo.iHeight)
           EC24Image1.ZoomX = CSng(EC24Image1.Width / sFrameInfo.iWidth)
           EC24Image1.ZoomY = CSng(EC24Image1.Height / sFrameInfo.iHeight)
           
           
           
           EC24Image1.Refresh
           
           
        End If
        
        iRet = CameraReleaseImageBuffer(m_hCamera, pRawBuffer) '�ɹ�����CameraGetImageBuffer��������CameraReleaseImageBuffer�ͷ�buffer
        
        
    End If
    
   
    
    m_iDisplayCounts = m_iDisplayCounts + 1
    CaptureImageForEvision = CAMERA_STATUS_SUCCESS
End Function


'����VB6���ܺܺõ�֧�ֶ��̱߳�̣�������ʹ���˶�ʱ���ķ�ʽ��������ץȡͼ�������ʹ�õ���VB.NET�������ʹ�ûص��������߽����ɼ��߳�����ץȡ�ķ�ʽ��
Private Sub GrabTimer_Timer()
Dim iRet As Long
    iRet = CaptureImageForEvision()
End Sub
'һ��ˢ��һ��ͼ����Ϣ
Private Sub InfoUpdateTimer_Timer()
Dim sFrameInfo As String
    sFrameInfo = "| Resolution " + Format$(m_iLastImageWidth, "General Number") + " X " + Format$(m_iLastImageHeight, "General Number") + " | Display frames " + Format$(m_iDisplayCounts, "General Number") + "|"
    StatusLabel.Caption = sFrameInfo
    
End Sub

'///BIG5 TRANS ALLOWED

