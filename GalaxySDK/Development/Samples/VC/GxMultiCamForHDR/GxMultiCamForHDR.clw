; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCamerParamDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GxMultiCamForHDR.h"
LastPage=0

ClassCount=11
Class1=CGxMultiCamForHDRApp
Class2=CGxMultiCamForHDRDoc
Class3=CGxMultiCamForHDRView
Class4=CMainFrame

ResourceCount=5
Resource1=IDR_MENU_CAM
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CSltCam
Class7=CSltCamDlg
Resource4=IDD_DLG_CAMSLT
Class8=CSpeedDlg
Class9=CGainShutterDlg
Class10=CAoiDlg
Class11=CCamerParamDlg
Resource5=IDD_DLG_CAMERA_PARAM

[CLS:CGxMultiCamForHDRApp]
Type=0
HeaderFile=GxMultiCamForHDR.h
ImplementationFile=GxMultiCamForHDR.cpp
Filter=N

[CLS:CGxMultiCamForHDRDoc]
Type=0
HeaderFile=GxMultiCamForHDRDoc.h
ImplementationFile=GxMultiCamForHDRDoc.cpp
Filter=N
LastObject=CGxMultiCamForHDRDoc

[CLS:CGxMultiCamForHDRView]
Type=0
HeaderFile=GxMultiCamForHDRView.h
ImplementationFile=GxMultiCamForHDRView.cpp
Filter=C
LastObject=CGxMultiCamForHDRView


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
HeaderFile=GxMultiCamForHDR.cpp
ImplementationFile=GxMultiCamForHDR.cpp
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
ControlCount=72
Control1=IDC_COMBO_GRAB_MODE,combobox,1344339970
Control2=IDC_COMBO_PIXEL_FORMAT,combobox,1344339970
Control3=IDC_COMBO_REVERSE_X,combobox,1344339970
Control4=IDC_COMBO_REVERSE_Y,combobox,1344339970
Control5=IDC_COMBO_GAIN_AUTO,combobox,1344339970
Control6=IDC_COMBO_GAMMA_ENABLE,combobox,1344339970
Control7=IDC_COMBO_GAMMA_MODE,combobox,1344339970
Control8=IDC_EDIT_GAMMA,edit,1350631552
Control9=IDC_SLIDER_GAMMA,msctls_trackbar32,1342242840
Control10=IDC_COMBO_AWB_MODE,combobox,1344339970
Control11=IDC_COMBO_WB_SELECTOR,combobox,1344339970
Control12=IDC_EDIT_BALANCE_RATIO,edit,1350631552
Control13=IDC_SLIDER_BALANCE_RATIO,msctls_trackbar32,1342242840
Control14=IDC_COMBO_SATURATION_MODE,combobox,1344339970
Control15=IDC_EDIT_SATURATION,edit,1350631552
Control16=IDC_SLIDER_SATURATION,msctls_trackbar32,1342242840
Control17=IDC_EDIT_EXPOSURE_TIME,edit,1350631552
Control18=IDC_SLIDER_EXPOSURE_TIME,msctls_trackbar32,1342242840
Control19=IDC_COMBO_EXPOSURE_AUTO,combobox,1344339970
Control20=IDC_EDIT_EXPECTED_GRAY_VALUE,edit,1350631552
Control21=IDC_SLIDER_EXPECTED_GRAY_VALUE,msctls_trackbar32,1342242840
Control22=IDC_COMBO_SHARPNESS_MODE,combobox,1344339970
Control23=IDC_EDIT_SHARPNESS,edit,1350631552
Control24=IDC_SLIDER_SHARPNESS,msctls_trackbar32,1342242840
Control25=IDC_COMBO_2D_NOISE_REDUCTION_MODE,combobox,1344339970
Control26=IDC_COMBO_3D_NOISE_REDUCTION_MODE,combobox,1344339970
Control27=IDC_COMBO_HDR_MODE,combobox,1344339970
Control28=IDC_EDIT_HDR_LONG_VALUE,edit,1350631552
Control29=IDC_SLIDER_HDR_LONG_VALUE,msctls_trackbar32,1342242840
Control30=IDC_EDIT_HDR_SHORT_VALUE,edit,1350631552
Control31=IDC_SLIDER_HDR_SHORT_VALUE,msctls_trackbar32,1342242840
Control32=IDC_EDIT_HDR_MAIN_VALUE,edit,1350631552
Control33=IDC_SLIDER_HDR_MAIN_VALUE,msctls_trackbar32,1342242840
Control34=IDC_STATIC,button,1342177287
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,static,1342308352
Control37=IDC_STATIC,static,1342308352
Control38=IDC_STATIC,static,1342308352
Control39=IDC_STATIC,button,1342177287
Control40=IDC_STATIC_EXPOSURE_TIME,static,1342308352
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC_EXPECTED_GRAY_VALUE,static,1342308352
Control43=IDC_STATIC,button,1342177287
Control44=IDC_STATIC,static,1342308352
Control45=IDC_STATIC,static,1342308352
Control46=IDC_STATIC,static,1342308352
Control47=IDC_STATIC_GAMMA,static,1342308352
Control48=IDC_STATIC,static,1342308352
Control49=IDC_STATIC,static,1342308352
Control50=IDC_STATIC_BALANCE_RATIO,static,1342308352
Control51=IDC_STATIC,static,1342308352
Control52=IDC_STATIC_SATURATION,static,1342308352
Control53=IDC_STATIC,button,1342177287
Control54=IDC_STATIC,button,1342177287
Control55=IDC_STATIC,static,1342308352
Control56=IDC_STATIC_SHARPNESS,static,1342308352
Control57=IDC_STATIC,static,1342308352
Control58=IDC_STATIC,static,1342308352
Control59=IDC_STATIC,static,1342308352
Control60=IDC_STATIC_HDR_LONG_VALUE,static,1342308352
Control61=IDC_STATIC_HDR_SHORT_VALUE,static,1342308352
Control62=IDC_STATIC_HDR_MAIN_VALUE,static,1342308352
Control63=IDC_STATIC,button,1342177287
Control64=IDC_STATIC_GAMMA_RANGE,static,1342308352
Control65=IDC_STATIC_BALANCE_RATIO_RANGE,static,1342308352
Control66=IDC_STATIC_SATURATION_RANGE,static,1342308352
Control67=IDC_STATIC_EXPOSURE_TIME_RANGE,static,1342308352
Control68=IDC_STATIC_EXPECTED_GRAY_VALUE_RANGE,static,1342308352
Control69=IDC_STATIC_SHARPNESS_RANGE,static,1342308352
Control70=IDC_STATIC_HDR_LONG_VALUE_RANGE,static,1342308352
Control71=IDC_STATIC_HDR_SHORT_VALUE_RANGE,static,1342308352
Control72=IDC_STATIC_HDR_MAIN_VALUE_RANGE,static,1342308352

[CLS:CCamerParamDlg]
Type=0
HeaderFile=CamerParamDlg.h
ImplementationFile=CamerParamDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCamerParamDlg

