
// stdafx.h : ��Н���f�����`��ǻ�����`����
// ����ݶ��p�[���鶼����ǻ
// �������X��ǻ�����`��

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ֲ Windows �Y�������ߌ�p�[ǻ�Ị
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ���D CString �ʊ����fε��}�dǻ

// ��� MFC �����D������ݶ������z粽�ǻ�����m��ǻ���
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���z�V�����Н�V��
#include <afxext.h>         // MFC ��H


#include <afxdisp.h>        // MFC �`���ԞF



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 ���ͷ���ǻ�E��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows ���ͷ���ǻ�E��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // ����?�����ǻ MFC �E��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


