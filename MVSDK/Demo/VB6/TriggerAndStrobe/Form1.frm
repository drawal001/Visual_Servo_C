VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Camera VB Demo[Basic]"
   ClientHeight    =   8880
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   11235
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   592
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   749
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "SW trigger"
      Height          =   255
      Left            =   4200
      TabIndex        =   3
      Top             =   600
      Width           =   1695
   End
   Begin VB.Timer InfoUpdateTimer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   10560
      Top             =   600
   End
   Begin VB.Timer GrabTimer 
      Enabled         =   0   'False
      Interval        =   25
      Left            =   10080
      Top             =   600
   End
   Begin VB.PictureBox ImageDisplayer 
      Height          =   5655
      Left            =   240
      ScaleHeight     =   373
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   717
      TabIndex        =   1
      Top             =   2400
      Width           =   10815
   End
   Begin VB.CommandButton CommandSetCamera 
      Caption         =   "Settings"
      Height          =   375
      Index           =   1
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Frame Frame1 
      Caption         =   "Trigger Mode"
      Height          =   1215
      Left            =   1560
      TabIndex        =   4
      Top             =   0
      Width           =   4575
      Begin VB.OptionButton Option1 
         Caption         =   "Continuous mode"
         Height          =   180
         Index           =   0
         Left            =   120
         TabIndex        =   10
         Top             =   360
         Value           =   -1  'True
         Width           =   1695
      End
      Begin VB.OptionButton Option1 
         Caption         =   "Software trigger Mode"
         Height          =   180
         Index           =   1
         Left            =   120
         TabIndex        =   9
         Top             =   600
         Width           =   2415
      End
      Begin VB.OptionButton Option1 
         Caption         =   "External trigger Mode"
         Height          =   180
         Index           =   2
         Left            =   120
         TabIndex        =   8
         Top             =   840
         Width           =   2415
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Strobe signal mode"
      Height          =   1215
      Left            =   6480
      TabIndex        =   5
      Top             =   0
      Width           =   4575
      Begin VB.OptionButton Option2 
         Caption         =   "Auto Strobe Mode"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   16
         Top             =   480
         Value           =   -1  'True
         Width           =   2175
      End
      Begin VB.OptionButton Option2 
         Caption         =   "Manual Mode"
         Height          =   255
         Index           =   1
         Left            =   240
         TabIndex        =   15
         Top             =   720
         Width           =   1335
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "External trigger signal type"
      Height          =   975
      Left            =   1560
      TabIndex        =   6
      Top             =   1320
      Width           =   4575
      Begin VB.OptionButton Option4 
         Caption         =   "Trailing eage"
         Height          =   255
         Index           =   1
         Left            =   1920
         TabIndex        =   14
         Top             =   240
         Width           =   1575
      End
      Begin VB.OptionButton Option4 
         Caption         =   "High level"
         Height          =   255
         Index           =   2
         Left            =   240
         TabIndex        =   13
         Top             =   480
         Width           =   1575
      End
      Begin VB.OptionButton Option4 
         Caption         =   "Low level"
         Height          =   255
         Index           =   3
         Left            =   1920
         TabIndex        =   12
         Top             =   480
         Width           =   1575
      End
      Begin VB.OptionButton Option4 
         Caption         =   "Leading eage"
         Height          =   255
         Index           =   0
         Left            =   240
         TabIndex        =   11
         Top             =   240
         Value           =   -1  'True
         Width           =   1575
      End
   End
   Begin VB.Frame Frame4 
      Caption         =   "Strobe manual mode palority"
      Height          =   975
      Left            =   6480
      TabIndex        =   7
      Top             =   1320
      Width           =   3015
      Begin VB.OptionButton Option3 
         Caption         =   "Active Low"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   18
         Top             =   480
         Width           =   1935
      End
      Begin VB.OptionButton Option3 
         Caption         =   "Active High"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   17
         Top             =   240
         Value           =   -1  'True
         Width           =   1935
      End
   End
   Begin VB.Label StatusLabel 
      Caption         =   "Video Information"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   8280
      Width           =   10215
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

Private Sub Command1_Click() '����һ��������Ϣ��ִ��һ������
Dim iRet As Long
iRet = CameraSoftTrigger(m_hCamera)
End Sub

Private Sub CommandSetCamera_Click(index As Integer) '��ʾ������ô���
Dim iRet As Long
iRet = CameraShowSettingPage(m_hCamera, 1)

End Sub

Private Sub CommandSnapShot_Click(index As Integer)
Dim iRet As Long
    iRet = Snapshot()
End Sub

Private Sub Form_Load()
Dim iCameraNum As Long
Dim CameraList(10) As tSdkCameraDevInfo
Dim iRet As Integer
Dim iMode As Long


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

    ImageDisplayer.ScaleMode = 3 '������ؿ��
    
    iRet = InitCamera(CameraList(0), ImageDisplayer.hWnd, ImageDisplayer.Width, ImageDisplayer.Height)
    
    
   
If iRet <> 0 Then
            m_hCamera = 0
            MsgBox ("�����ʼ��ʧ��")
            Unload Me
            Exit Sub
Else
    GrabTimer.Enabled = True
    InfoUpdateTimer.Enabled = True
    '��ȡ�����ǰ����������Ʋ��������½��浥ѡ��ť
    iRet = CameraGetTriggerMode(m_hCamera, iMode)
    Option1.Item(iMode).Value = True
     iRet = CameraGetExtTrigSignalType(m_hCamera, iMode)
    Option4.Item(iMode).Value = True
    iRet = CameraGetStrobeMode(m_hCamera, iMode)
    Option2.Item(iMode).Value = True
    iRet = CameraGetStrobePolarity(m_hCamera, iMode)
    Option3.Item(iMode).Value = True
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

'����VB6���ܺܺõ�֧�ֶ��̱߳�̣�������ʹ���˶�ʱ���ķ�ʽ��������ץȡͼ�������ʹ�õ���VB.NET�������ʹ�ûص��������߽����ɼ��߳�����ץȡ�ķ�ʽ��
Private Sub GrabTimer_Timer()
Dim iRet As Long
    iRet = CaptureImage()
End Sub
'һ��ˢ��һ��ͼ����Ϣ
Private Sub InfoUpdateTimer_Timer()
Dim sFrameInfo As String
    sFrameInfo = "| Resolution " + Format$(m_iLastImageWidth, "General Number") + " X " + Format$(m_iLastImageHeight, "General Number") + " | Display frames " + Format$(m_iDisplayCounts, "General Number") + "|"
    StatusLabel.Caption = sFrameInfo
    
End Sub

'///BIG5 TRANS ALLOWED

Private Sub Option1_Click(index As Integer) '�����������ģʽ��0Ϊ����Ԥ��ģʽ��1Ϊ����ģʽ��2ΪӲ������ģʽ
Dim iRet As Long
iRet = CameraSetTriggerMode(m_hCamera, index)

End Sub

Private Sub Option2_Click(index As Integer) '�����������������ģʽ��0Ϊ�Զ�ģʽ��1Ϊ�ֶ�ģʽ���Զ�ģʽΪ����Զ����������ͬ���źţ��ֶ�ģʽ������ֶ�����������źŵ���ʱ�������ȡ�
Dim iRet As Long
iRet = CameraSetStrobeMode(m_hCamera, index)
End Sub

Private Sub Option3_Click(index As Integer) '�����ֶ������ģʽ�£�������źŵ���Ч���ԡ�
Dim iRet As Long
iRet = CameraSetStrobePolarity(m_hCamera, index)
End Sub

Private Sub Option4_Click(index As Integer) '����������ⴥ�������źŵ����࣬0Ϊ�����ش�����1Ϊ�½��ش�����2Ϊ�ߵ�ƽ������3Ϊ�͵�ƽ��������ƽ����ģʽ���������ܣ�ͨ�����CameraSetExtTrigJitterTime��������ȥ��ʱ��
Dim iRet As Long
iRet = CameraSetExtTrigSignalType(m_hCamera, index)
End Sub
