VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Camera VB Demo[Basic]"
   ClientHeight    =   8880
   ClientLeft      =   225
   ClientTop       =   555
   ClientWidth     =   20250
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   592
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   1350
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton CommandSetCamera2 
      Caption         =   "Settings"
      Height          =   375
      Index           =   0
      Left            =   9120
      TabIndex        =   7
      Top             =   240
      Width           =   1215
   End
   Begin VB.PictureBox ImageDisplayer2 
      Height          =   7215
      Left            =   9000
      ScaleHeight     =   477
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   565
      TabIndex        =   6
      Top             =   840
      Width           =   8535
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
      TabIndex        =   5
      Top             =   240
      Width           =   1215
   End
   Begin VB.PictureBox ImageDisplayer 
      Height          =   7215
      Left            =   240
      ScaleHeight     =   477
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   565
      TabIndex        =   3
      Top             =   840
      Width           =   8535
   End
   Begin VB.CommandButton CommandSnapShot 
      Caption         =   "Snapshot"
      Height          =   375
      Index           =   2
      Left            =   4200
      TabIndex        =   2
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
      TabIndex        =   4
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

Private Sub CommandSetCamera2_Click(index As Integer)
Dim iRet As Long
iRet = CameraShowSettingPage(m_hCamera2, 1)
End Sub

Private Sub CommandSnapShot_Click(index As Integer)
Dim iRet As Long
    iRet = Snapshot()
End Sub

Private Sub Form_Load()
Dim iCameraNum As Long
Dim CameraList(10) As tSdkCameraDevInfo
Dim iRet As Integer
Dim sTmp1() As Byte
Dim sTmp2() As Byte

sTmp1 = StrConv("111", vbFromUnicode)  '��111����������1�����֣�������ֿ��������ǵ���ʾ����Լ����޸ĺá�
ReDim Preserve sTmp1(UBound(sTmp1) + 1)
sTmp1(UBound(sTmp1)) = 0

sTmp2 = StrConv("222", vbFromUnicode)  '��111����������2�����֣�������ֿ��������ǵ���ʾ����Լ����޸ĺá�
ReDim Preserve sTmp2(UBound(sTmp2) + 1)
sTmp2(UBound(sTmp2)) = 0




'//ö���豸������豸�б�
iCameraNum = CameraEnumerateDeviceEx()

If iCameraNum < 1 Then
            m_hCamera = 0
            MsgBox ("û��ɨ�赽����������������ٴ����б�����")
            Unload Me
            Exit Sub

    
End If

    ImageDisplayer.ScaleMode = 3 '������ؿ��
    ImageDisplayer2.ScaleMode = 3 '������ؿ��
    
    iRet = InitCamera1ByNames(sTmp1(0), ImageDisplayer.hWnd, ImageDisplayer.Width, ImageDisplayer.Height)

If iRet <> 0 Then
    m_hCamera = 0
    MsgBox ("���1��ʼ��ʧ��")
    Unload Me
    Exit Sub
End If


   iRet = InitCamera2ByNames(sTmp2(0), ImageDisplayer2.hWnd, ImageDisplayer2.Width, ImageDisplayer2.Height)

If iRet <> 0 Then
    m_hCamera2 = 0
    MsgBox ("���2��ʼ��ʧ��")
    Unload Me
    Exit Sub
End If

    GrabTimer.Enabled = True
    InfoUpdateTimer.Enabled = True


End Sub

Private Sub Form_Unload(Cancel As Integer)

If m_hCamera <> 0 Then
           GrabTimer.Enabled = False
           InfoUpdateTimer.Enabled = False
           CameraUnInit (m_hCamera)         '����ʼ�����
           CameraAlignFree (m_pFrameBuffer)
End If

If m_hCamera2 <> 0 Then
           GrabTimer.Enabled = False
           InfoUpdateTimer.Enabled = False
           CameraUnInit (m_hCamera2)         '����ʼ�����
           CameraAlignFree (m_pFrameBuffer2)
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

