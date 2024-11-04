#include "stdafx.h"
#include "FileVersion.h"

CFileVersion::CFileVersion(CString strPath)
{
	m_strPath = strPath;
}

CFileVersion::~CFileVersion()
{
}

//--------------------------------------------------------------
/**
\brief   获取所有的文件版本信息，各信息间以\n分隔

\return  strRet   所有的版本信息
*/
//--------------------------------------------------------------
CString CFileVersion::GetFullVersion()
{
	CString strRet;
	strRet = "文件版本: " + GetFileVersion() + "\n"; 
	strRet += "说明: " + GetFileDescription() +"\n";
	strRet += "版权: " + GetLegalCopyright() +"\n";
	strRet += "备注: " + GetComments() + "\n";
	strRet += "产品版本: " + GetProductVersion() +"\n";
	strRet += "产品名称: " + GetProductName() +"\n";
	strRet += "个人用内部版本说明: " + GetPrivateBuild() +"\n";
	strRet += "公司名称: " + GetCompanyName() +"\n";
	strRet += "合法商标: " + GetLegalTrademarks() +"\n";
	strRet += "内部名称: " + GetInternalName() +"\n";
	strRet += "特殊内部版本说明: " + GetSpecialBuild() +"\n";
	strRet += "源文件名: " + GetOriginalFilename() +"\n";	
	return strRet;
}

//--------------------------------------------------------------
/**
\brief    获取备注

\return   备注
*/
//--------------------------------------------------------------
CString CFileVersion::GetComments()
{
	return GetVer("Comments");
}

//--------------------------------------------------------------
/**
\brief    获得内部名称

\return   内部名称
*/
//--------------------------------------------------------------
CString CFileVersion::GetInternalName()
{
	return GetVer("InternalName");
}

//--------------------------------------------------------------
/**
\brief    获取产品名称

\return   产品名称
*/
//--------------------------------------------------------------
CString CFileVersion::GetProductName()
{
	return GetVer("ProductName");
}

//--------------------------------------------------------------
/**
\brief    获取公司名称

\return   公司名称
*/
//--------------------------------------------------------------
CString CFileVersion::GetCompanyName()
{
	return GetVer("CompanyName");
}

//--------------------------------------------------------------
/**
\brief    获取版权

\return   版权
*/
//--------------------------------------------------------------
CString CFileVersion::GetLegalCopyright()
{
	return GetVer("LegalCopyright");
}

//--------------------------------------------------------------
/**
\brief    获取产品版本

\return   产品版本
*/
//--------------------------------------------------------------
CString CFileVersion::GetProductVersion()
{
	return GetVer("ProductVersion");
}

//--------------------------------------------------------------
/**
\brief    获取文件描述

\return   文件描述
*/
//--------------------------------------------------------------
CString CFileVersion::GetFileDescription()
{
	return GetVer("FileDescription");
}

//--------------------------------------------------------------
/**
\brief    获取合法商标

\return   合法商标
*/
//--------------------------------------------------------------
CString CFileVersion::GetLegalTrademarks()
{
	return GetVer("LegalTrademarks");
}

//--------------------------------------------------------------
/**
\brief    获取个人内部版本说明

\return   个人内部版本说明
*/
//--------------------------------------------------------------
CString CFileVersion::GetPrivateBuild()
{
	return GetVer("PrivateBuild");
}

//--------------------------------------------------------------
/**
\brief    获取文件版本

\return   文件版本
*/
//--------------------------------------------------------------
CString CFileVersion::GetFileVersion()
{
	return GetVer("FileVersion");
}

//--------------------------------------------------------------
/**
\brief    获取源文件名

\return   内部名称
*/
//--------------------------------------------------------------
CString CFileVersion::GetOriginalFilename()
{
	return GetVer("OriginalFilename");
}

//--------------------------------------------------------------
/**
\brief    获取特殊内部版本说明

\return   特殊内部版本说明
*/
//--------------------------------------------------------------
CString CFileVersion::GetSpecialBuild()
{
	return GetVer("SpecialBuild");
}

//--------------------------------------------------------------
/**
\brief    获取版本信息函数
\param    strSubBlock    获取版本项 

\return   版本信息
*/
//--------------------------------------------------------------
CString CFileVersion::GetVer(CString strSubBlock)
{
	struct LANGANDCODEPAGE 
	{
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;
	
	int     i             = 0;
	CString strRet        = "";
	DWORD   dwHandle      = 0;
	DWORD   dwInfoSize    = 0;
	UINT    cbTranslate   = 0;
	UINT    dwBytes       = 0;
	char    chBlock[256]  = {0};
	char*   pBuffer       = NULL;
	char*   pData         = NULL;

	// 获取文件版本信息大小
	dwInfoSize = GetFileVersionInfoSize((char*)(LPCTSTR)m_strPath, &dwHandle);

	if(dwInfoSize != 0)
	{
		// 获取版本信息
		pData = new char[dwInfoSize];
		if( GetFileVersionInfo((char*)(LPCTSTR)m_strPath, 0, dwInfoSize, pData))
		{
			
			VerQueryValue(pData, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);

			for(i = 0; i <(int)(cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++)
			{
				wsprintf(chBlock, 
					     TEXT("\\StringFileInfo\\%04x%04x\\%s"),
					     lpTranslate[i].wLanguage,
					     lpTranslate[i].wCodePage,
					     strSubBlock);		
				
				VerQueryValue(pData, 
					         chBlock, 
					         (LPVOID*)&pBuffer, 
				          	 &dwBytes); 
				
				strRet.Format("%s", pBuffer);

				break; //only retrieve the first language and code page
			}
		}
		if (pData != NULL)
		{
			delete []pData;
			pData = NULL;
		}
	}

	return strRet;
}