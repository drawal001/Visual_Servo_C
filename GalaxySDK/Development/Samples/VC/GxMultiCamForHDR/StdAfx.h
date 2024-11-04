// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7788D43B_D613_4B67_8A98_E4B7B63444D3__INCLUDED_)
#define AFX_STDAFX_H__7788D43B_D613_4B67_8A98_E4B7B63444D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0501 

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define   MAX_DEVICE_MUN  4  ///< ���֧��4̨���
#include "GxIAPI.h"


//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in] ������

\return ��
*/
//----------------------------------------------------------------------------------
inline void ShowErrorString(GX_STATUS emErrorStatus)
{
    char*     pchErrorInfo = NULL;
    size_t    nSize        = 0;
    GX_STATUS emStatus     = GX_STATUS_ERROR;

    // ��ȡ������Ϣ���ȣ��������ڴ�ռ�
    emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        AfxMessageBox("GXGetLastError��ȡ������Ϣ����ʧ�ܣ�");
    }

    pchErrorInfo = new char[nSize];
    if (NULL == pchErrorInfo)
    {
        return;
    }

    // ��ȡ������Ϣ������ʾ
    emStatus = GXGetLastError (&emErrorStatus, pchErrorInfo, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        AfxMessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
    }
    else
    {
        AfxMessageBox((LPCTSTR)pchErrorInfo);
    }

    // �ͷ�������ڴ�ռ�
    if (NULL != pchErrorInfo)
    {
        delete[] pchErrorInfo;
        pchErrorInfo = NULL;
    }
}


///< �жϷ���ֵ�궨��
#define VERIFY_RET(emStatus) \
if (emStatus != GX_STATUS_SUCCESS) \
                            {\
                            return; \
                            }\

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7788D43B_D613_4B67_8A98_E4B7B63444D3__INCLUDED_)
