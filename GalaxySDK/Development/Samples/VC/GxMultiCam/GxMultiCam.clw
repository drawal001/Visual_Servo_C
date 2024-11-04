; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCamerParamDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GxMultiCam.h"
LastPage=0

ClassCount=11
Class1=CGxMultiCamApp
Class2=CGxMultiCamDoc
Class3=CGxMultiCamView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_DLG_CAMSLT
Resource2=IDD_ABOUTBOX
Resource3=IDR_MAINFRAME
Class5=CAboutDlg
Class6=CSltCam
Class7=CSltCamDlg
Resource4=IDR_MENU_CAM
Class8=CSpeedDlg
Class9=CGainShutterDlg
Class10=CAoiDlg
Class11=CCamerParamDlg
Resource5=IDD_DLG_CAMERA_PARAM

[CLS:CGxMultiCamApp]
Type=0
HeaderFile=GxMultiCam.h
ImplementationFile=GxMultiCam.cpp
Filter=N

[CLS:CGxMultiCamDoc]
Type=0
HeaderFile=GxMultiCamDoc.h
ImplementationFile=GxMultiCamDoc.cpp
Filter=N
LastObject=CGxMultiCamDoc

[CLS:CGxMultiCamView]
Type=0
HeaderFile=GxMultiCamView.h
ImplementationFile=GxMultiCamView.cpp
Filter=C
LastObject=CGxMultiCamView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_MENU_OPEN




[CLS:CAboutDlg]
Type=0
HeaderFile=GxMultiCam.cpp
ImplementationFile=GxMultiCam.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MENU_CAM]
Type=1
Class=CMainFrame
Command1=ID_MENU_SLTCAM
Command2=ID_MENU_OPEN
Command3=ID_MENU_START
Command4=ID_MENU_STOP
Command5=ID_MENU_CLOSE
Command6=ID_MENU_CAMMER_PARAM
Command7=ID_MENU_CAMID
CommandCount=7

[CLS:CSltCam]
Type=0
HeaderFile=SltCam.h
ImplementationFile=SltCam.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST1
VirtualFilter=dWC

[DLG:IDD_DLG_CAMSLT]
Type=1
Class=CSltCamDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CMB_CAM,combobox,1344340226
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BTN_REENUM,button,1342242816

[CLS:CSltCamDlg]
Type=0
HeaderFile=SltCamDlg.h
ImplementationFile=SltCamDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CMB_CAM
VirtualFilter=dWC

[CLS:CSpeedDlg]
Type=0
HeaderFile=SpeedDlg.h
ImplementationFile=SpeedDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSpeedDlg
VirtualFilter=dWC

[CLS:CGainShutterDlg]
Type=0
HeaderFile=GainShutterDlg.h
ImplementationFile=GainShutterDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SLR_SHUTTER

[CLS:CAoiDlg]
Type=0
HeaderFile=AoiDlg.h
ImplementationFile=AoiDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAoiDlg
VirtualFilter=dWC

[DLG:IDD_DLG_CAMERA_PARAM]
Type=1
Class=CCamerParamDlg
ControlCount=9
Control1=IDC_STATIC,static,1342308352
Control2=IDC_COMBO_AWB,combobox,1344339971
Control3=IDC_STATIC_ACQ_SPEEDLEVEL,static,1342308352
Control4=IDC_SLIDER_ACQ_SPEEDLEVEL,msctls_trackbar32,1342242840
Control5=IDC_EDIT_ACQ_SPEEDLEVEL,edit,1350633600
Control6=IDC_STC_SHUTTER_SHOW,static,1342308352
Control7=IDC_EDIT_SHUTTER,edit,1350631552
Control8=IDC_STC_GAIN,static,1342308352
Control9=IDC_EDIT_GAIN,edit,1350631552

[CLS:CCamerParamDlg]
Type=0
HeaderFile=CamerParamDlg.h
ImplementationFile=CamerParamDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCamerParamDlg

