/* FileVersion.h --  version 0.1.0, 2005-2-19

  Copyright (C) 2010

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.  
*/

#ifndef _FILE_VERSION_H_
#define _FILE_VERSION_H_

#pragma comment(lib, "Version.lib")

class CFileVersion
{
public:
	explicit CFileVersion(CString strPath);
	~CFileVersion();

    /// 获取所有的文件版本信息，各信息间以\n分隔
	CString GetFullVersion();

	/// 获取备注
	CString GetComments();

	/// 获取获内部名称
	CString GetInternalName();

	/// 获取产品名称
	CString GetProductName();

	/// 获取公司名称
	CString GetCompanyName();

	/// 获取版权
	CString GetLegalCopyright();

	/// 获取产品版本
	CString GetProductVersion();

	/// 获取文件描述
	CString GetFileDescription();

	/// 获取合法商标
	CString GetLegalTrademarks();

	/// 获取个人用内部版本说明
	CString GetPrivateBuild();

	/// 获取文件版本
	CString GetFileVersion();

	/// 获取源文件名
	CString GetOriginalFilename();

	/// 取得特殊内部版本说明
	CString GetSpecialBuild();

private:

	/// 获取版本信息函数
	CString GetVer(CString chSubBlock);

	CString m_strPath;
};


#endif

