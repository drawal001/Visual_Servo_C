//------------------------------------------------------------------------
/**
\file		StoreAVI.h
\brief		此类主要用于AVI视频的存储

<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>

\Date       2024-03-11
\Version    1.1.2403.9111
*/
//------------------------------------------------------------------------

#pragma once
#pragma warning (disable: 4996)
#include "GxIAPI.h"
#include "DxImageProc.h"
#include <string>
#include <iostream>

//保存AVI视频依赖的头文件
#include <wtypes.h>
#include <atlbase.h>
#include <shlobj.h>

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

#define  AVI_MAX_SIZE   0x80000000  ///< AVI文件最大值:2GB

///< AVI视频每帧图像数据相对于该帧头的偏移
#define AVI_FRAME_OFFSET  8

using namespace std;

//--------------------------------------------
/**
\brief 锁类
*/
//---------------------------------------------
class CLock
{
public:
	/// 构造
	CLock() { InitializeCriticalSection(&m_cs); }
	/// 析构
	virtual ~CLock() { DeleteCriticalSection(&m_cs); }
public:
	/// 进入临界区
	void Lock() { EnterCriticalSection(&m_cs); }
	/// 离开临界区
	void Unlock() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs; ///< 临界段变量	
};

//--------------------------------------------
/**
\brief 视频存储类
*/
//---------------------------------------------
class CStoreAVI
{
public:
	CStoreAVI(BITMAPINFO* stInfo, int64_t i64ImageWidth, int64_t i64ImageHeight, char* strStorePath = "."
		, uint64_t ui64PlayFPS = 15);
	~CStoreAVI();

	/// 选择视频编码器
	bool           SelectCompressor(HWND* pParent);

	/// 保存AVI视频实现函数:该函数带有检测磁盘空间,限制AVI文件大小等扩展功能
	void           ExSaveAVI(BYTE * pImageBuf);

	/// 保存AVI视频实现函数:该函数只实现写AVI文件,不带有其他扩展功能
	void           SaveAVI(BYTE *pImageBuf);

	/// 为保存avi视频文件准备资源
	bool           PrepareForSaveAVI(bool bPixelFormat);

	/// 释放为保存AVI视频使用的资源
	void           UnPrepareForSaveAVI();

private:

	/// 停止当前录制重新创建AVI视频文件
	bool           __ReCreatVedioFile();

	/// 获取存储目录所在磁盘的剩余空间
	int64_t        __GetDiskFreeSpace() const;

	/// 释放视频编码器相关资源
	void           __ResetCompressor();

	bool	       m_bCompressedAVI;              ///< 是否压缩存储
	int64_t        m_nImageWidth;                 ///< 图像宽
	int64_t        m_nImageHeight;                ///< 图像高
	bool		   m_bIsColor;					  ///< 当前是否是彩色像素
	int64_t        m_nFrameSize;                  ///< 计算AVI视频保存时每帧的大小
	int64_t        m_nWrittenDataSize;            ///< 写到AVI文件的图像数据大小（不包含AVI文件头大小）
	uint64_t       m_ui64PlayFPS;                 ///< AVI视频保存帧率

	CString        m_strStorePath;                ///< AVI文件存储路径

	BITMAPINFO     *m_pBmpInfo;	                  ///< BITMAPINFO 结构指针，显示图像时使用

	HIC            m_hic;                         ///< 标记使用的视频压缩器句柄
	LONG           m_nTimeFrame;                  ///< 写入avi文件需要的帧ID号
	PAVIFILE       m_pAVIFile;                    ///< AVI文件的句柄
	COMPVARS       m_Com;                         ///< 选择编码器使用参数
	PAVISTREAM     m_ps;                          ///< 通过AVI文件创建的视频流
	PAVISTREAM	   m_psCompressed;                ///< 通过视频流和视频压缩器创建的压缩视频流
	CLock          m_objLock;                     ///< 互斥锁
};


inline bool  MsgComposer(char* pPath,const char* pFormat, ...)
{
	char chBuffer[256] = { 0 };
	va_list vap;
	va_start(vap, pFormat);

	#if _MSC_VER >= 1400 //如果是VS2005或者以上版本
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
	#else if _MSC_VER == 1200 //如果是VC6
		_vsnprintf(chBuffer, 256, pFormat, vap );
	#endif
	va_end(vap);
	memcpy(pPath, chBuffer, 256);
	return true;
};
