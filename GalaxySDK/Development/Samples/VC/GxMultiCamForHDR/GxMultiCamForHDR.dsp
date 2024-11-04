# Microsoft Developer Studio Project File - Name="GxMultiCamForHDR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GxMultiCamForHDR - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GxMultiCamForHDR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GxMultiCamForHDR.mak" CFG="GxMultiCamForHDR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GxMultiCamForHDR - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GxMultiCamForHDR - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GxMultiCamForHDR - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../bin/x86/"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../bin/x86/"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../inc" /I "../Common" /I "$(GALAXY_SDK_DEVELOPMENT)\VC SDK\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 GxIAPI.lib DxImageProc.lib /nologo /subsystem:windows /machine:I386 /out:"../../bin/x86/GxMultiCamForHDR.exe" /libpath:"../../lib/x86" /libpath:"..\..\lib\x86" /libpath:"$(GALAXY_SDK_DEVELOPMENT)\VC SDK\lib\x86"

!ELSEIF  "$(CFG)" == "GxMultiCamForHDR - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../bin/x86/"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../inc" /I "../Common" /I "$(GALAXY_SDK_DEVELOPMENT)\VC SDK\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GxIAPI.lib DxImageProc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/x86/GxMultiCamForHDR.exe" /pdbtype:sept /libpath:"../../lib/x86" /libpath:"..\..\lib\x86" /libpath:"$(GALAXY_SDK_DEVELOPMENT)\VC SDK\lib\x86"

!ENDIF 

# Begin Target

# Name "GxMultiCamForHDR - Win32 Release"
# Name "GxMultiCamForHDR - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CamerParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Fps.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GxBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDR.cpp
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDR.rc
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDRDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDRView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SltCamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Common\StoreAVI.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CamerParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\Fps.h
# End Source File
# Begin Source File

SOURCE=..\Common\GxBitmap.h
# End Source File
# Begin Source File

SOURCE=.\GxEx.h
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDR.h
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDRDoc.h
# End Source File
# Begin Source File

SOURCE=.\GxMultiCamForHDRView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SltCamDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Common\StoreAVI.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GxMultiCamForHDR.ico
# End Source File
# Begin Source File

SOURCE=.\res\GxMultiCamForHDR.rc2
# End Source File
# Begin Source File

SOURCE=.\res\GxMultiCamForHDRDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\GxMultiCamForHDR.manifest
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
