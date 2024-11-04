//------------------------------------------------------------------------
/**
\file		StoreAVI.cpp
\brief		此类用于AVI视频的存储

<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>

\Date       2024-03-11
\Version    1.1.2403.9111
*/
//------------------------------------------------------------------------

#include "stdafx.h"
#include "StoreAVI.h"

//---------------------------------------------------------------------------------
/**
\brief   构造函数
\param   stInfo         BITMAPINFO 结构指针，显示、存储图像时使用
\param   i64ImageWidth  图像宽度
\param   i64ImageHeight 图像高度
\param   strStorePath   AVI文件保存路径
\param   ui64PlayFPS    保存的帧率

\return  无
*/
//----------------------------------------------------------------------------------
CStoreAVI::CStoreAVI(BITMAPINFO* stInfo, int64_t i64ImageWidth, int64_t i64ImageHeight, char* strStorePath
					 , uint64_t ui64PlayFPS)
:   m_pBmpInfo(stInfo)
, m_pAVIFile(NULL)
, m_nTimeFrame(0)
, m_psCompressed(NULL)
, m_ps(NULL)
, m_hic(NULL)
, m_nWrittenDataSize(0)
, m_bCompressedAVI(FALSE)
, m_nImageWidth(i64ImageWidth)
, m_nImageHeight(i64ImageHeight)
, m_strStorePath(strStorePath)
, m_ui64PlayFPS(ui64PlayFPS)
, m_objLock()
, m_bIsColor(false)
{
	const int32_t COLOR_PIXEL = 3;
	// 计算每帧图像数据大小
	m_nFrameSize = m_nImageWidth * m_nImageHeight * COLOR_PIXEL;
	memset(&m_Com, 0, sizeof(m_Com));
}

//---------------------------------------------------------------------------------
/**
\brief   析构函数

\return  无
*/
//----------------------------------------------------------------------------------
CStoreAVI::~CStoreAVI()
{
	// 清理资源
	UnPrepareForSaveAVI();
}

//---------------------------------------------------------------------------------
/**
\brief   选择视频编码器
\param   pParent 窗口句柄

\return  true 选择成功，false选择失败
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::SelectCompressor(HWND* pParent)
{
	LPBITMAPINFOHEADER lpbi = &m_pBmpInfo->bmiHeader;
	DWORD  fccType = ICTYPE_VIDEO;

	m_Com.cbSize = sizeof(m_Com);

	//代码指定视频编码器类型,用户通过修改mmioFOURCC的宏参数可以指定编码器,如mmioFOURCC('x','v','i','d')
	//HIC hic = ICOpen(ICTYPE_VIDEO,mmioFOURCC('d','i','v','x'),ICMODE_COMPRESS);

	//若用户不清楚PC上已有编码器的的类型和名称,可通过下面的代码选择编码器
	BOOL bRet = ICCompressorChoose(*pParent, ICMF_CHOOSE_ALLCOMPRESSORS, NULL, NULL, &m_Com, "选择压缩类型");


	if (!bRet)
	{
		// bRet为FALSE标识用户未选择视频编码器
		return false;
	}
	HIC hic = ICOpen(m_Com.fccType, m_Com.fccHandler, ICMODE_QUERY);

	// 已选择视频编码器:判断选择的编码器是否可用
	if (hic != NULL)
	{
		if ((fccType == ICTYPE_VIDEO) && (ICCompressQuery(hic, lpbi, NULL) != ICERR_OK))
		{
			__ResetCompressor();
			MessageBox(NULL, TEXT("选择的压缩视频器不可用,请重新选择!"), TEXT("错误"), MB_OK);
			return false;
		}

		m_hic = hic;
		m_bCompressedAVI = true;
	}

	return true;
}

//----------------------------------------------------------------------------------
/**
\brief  为保存avi视频文件准备资源
\param  bPixelFormat 是否为彩色

\return true:准备成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::PrepareForSaveAVI(bool bPixelFormat)
{
	bool    bRet = FALSE;
	LONG    cbFormat = 0;    //文件格式
	m_bIsColor = bPixelFormat;
	if (bPixelFormat)
	{
		// 经过RGB转换的24图像数据格式
		cbFormat = sizeof(BITMAPINFOHEADER);
	}
	else
	{
		//  8位灰度图像文件格式
		cbFormat = sizeof(BITMAPINFOHEADER) + (256 * 4);
	}

	AVISTREAMINFO StreamInfo;
	HRESULT hr;

	SYSTEMTIME system;
	GetLocalTime(&system);
	char chAVIFileName[256] = { 0 };
	MsgComposer(chAVIFileName,"%s\\%d_%d_%d_%d_%d_%d_%d.avi",
		m_strStorePath,
		system.wYear,
		system.wMonth,
		system.wDay,
		system.wHour,
		system.wMinute,
		system.wSecond,
		system.wMilliseconds);

	::DeleteFile(chAVIFileName);

	AVIFileInit();
	//创建一个AVI文件	
	hr = AVIFileOpen(&m_pAVIFile, chAVIFileName, OF_WRITE | OF_CREATE, NULL);
	if (AVIERR_OK !=hr)
	{
		return FALSE;
	}

	const int32_t COLOR_PIXEL = 3;
	// 创建一个视频流
	::ZeroMemory(&StreamInfo, sizeof(StreamInfo));
	StreamInfo.fccType = streamtypeVIDEO;
	StreamInfo.fccHandler = 0;
	StreamInfo.dwScale = 1;
	StreamInfo.dwRate = static_cast<DWORD>(m_ui64PlayFPS);  //AVI视频流的播放帧率
	StreamInfo.dwSuggestedBufferSize = DWORD(m_nImageWidth * m_nImageHeight * COLOR_PIXEL);
	SetRect(&StreamInfo.rcFrame, 0, 0, (int)m_nImageWidth, (int)m_nImageHeight);
	hr = AVIFileCreateStream(m_pAVIFile, &m_ps, &StreamInfo);
	if (AVIERR_OK != hr)
	{
		return FALSE;
	}

	//初始化压缩视频流需要的结构体
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = { &opts };

	if (m_bCompressedAVI)
	{
		if (NULL == m_hic)
		{
			return FALSE;
		}

		// 压缩存储
		DWORD dwSize = ICGetStateSize(m_hic);
		LPVOID lpvod = (LPVOID)GlobalAlloc(NULL, dwSize);

		ICINFO icinfo;
		ICGetInfo(m_hic, &icinfo, sizeof(icinfo));
		opts.fccType = icinfo.fccType;
		opts.fccHandler = icinfo.fccHandler;
		opts.dwKeyFrameEvery = 0;
		opts.dwQuality = 5000;
		opts.dwBytesPerSecond = 0;
		opts.dwFlags = 8;
		opts.lpFormat = 0;
		opts.cbFormat = 0;
		opts.lpParms = lpvod;
		opts.cbParms = dwSize;
		opts.dwInterleaveEvery = 0;
	}
	else
	{
		// 非压缩存储
		opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 
	}

	//创建压缩视频流
	hr = AVIMakeCompressedStream(&m_psCompressed, m_ps, &opts, NULL);
	if (AVIERR_OK != hr)
	{
		return FALSE;
	}

	//设置视频流的输入文件格式。
	hr = AVIStreamSetFormat(m_psCompressed, 0, &m_pBmpInfo->bmiHeader, cbFormat);
	if (AVIERR_OK != hr)
	{
		return FALSE;
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  清理为保存avi视频文件准备资源

\return 无
*/
//----------------------------------------------------------------------------------
void CStoreAVI::UnPrepareForSaveAVI()
{
	m_objLock.Lock();

	if (m_psCompressed != NULL)
	{
		AVIStreamClose(m_psCompressed);
		m_psCompressed = NULL;
	}
	if (m_ps != NULL)
	{
		AVIStreamClose(m_ps);
		m_ps = NULL;
	}
	if (m_pAVIFile != NULL)
	{
		AVIFileEndRecord(m_pAVIFile);
		AVIFileClose(m_pAVIFile);
		m_pAVIFile = NULL;
	}
	__ResetCompressor();
	AVIFileExit();
	m_nTimeFrame = 0;

	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief  保存AVI文件
\param  pImageBuf  待保存的图像buffer

\return 无
*/
//----------------------------------------------------------------------------------
void CStoreAVI::ExSaveAVI(BYTE * pImageBuf)
{

	m_objLock.Lock();
	// AVI视频保存实现
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		NULL);

	// 判断是否写成功
	if (AVIERR_OK ==hr)
	{
		++m_nTimeFrame;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief  保存avi视频:该函数带有检测磁盘空间,限制AVI文件大小等扩展功能
\param  pImageBuf  [in]  指向图像缓冲区的指针

\return 无
*/
//----------------------------------------------------------------------------------
void CStoreAVI::SaveAVI(BYTE * pImageBuf)
{
	if(!m_bIsColor)
	{
		m_nFrameSize = m_nImageWidth * m_nImageHeight;
	}

	// ---------------------------------------------
	// 检测当前目录磁盘空间,若剩余空间不允许继续存储时则停止保存
	if (__GetDiskFreeSpace() < m_nFrameSize + AVI_FRAME_OFFSET)
	{
		UnPrepareForSaveAVI();
		return;
	}

	LONG nBufferSize = 0; // 实际写入到AVI文件的图像数据大小,若是压缩则为压缩后的大小
	BOOL bRet = TRUE;

	m_objLock.Lock();

	// 判断继续存储时AVI文件大小是否会达到允许的最大值
	// 若达到则创建新的AVI文件继续录制
	if ((m_nFrameSize + m_nWrittenDataSize + AVI_FRAME_OFFSET) > AVI_MAX_SIZE - AVI_HEADERSIZE)
	{
		bRet = __ReCreatVedioFile();
		if (!bRet)
		{
			m_objLock.Unlock();
			return;
		}

		m_nWrittenDataSize = 0;
	}

	// AVI视频保存实现
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		&nBufferSize);

	// 判断是否写成功
	if (AVIERR_OK == hr)
	{
		++m_nTimeFrame;

		// 统计AVI文件数据大小:每帧图像占用空间为数据大小加上偏移值8
		m_nWrittenDataSize = m_nWrittenDataSize + nBufferSize + AVI_FRAME_OFFSET;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief  释放视频编码器相关资源

\return 无
*/
//----------------------------------------------------------------------------------
void CStoreAVI::__ResetCompressor()
{
	if (m_hic != NULL)
	{
		ICClose(m_hic);
		m_hic = NULL;
	}

	ICCompressorFree(&m_Com);
	memset(&m_Com, 0, sizeof(m_Com));
}

//----------------------------------------------------------------------------------
/**
\brief  停止当前录制,重新创建AVI视频文件,然后继续录制

\return TRUE:重新创建成功 FALSE:创建失败
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::__ReCreatVedioFile()
{
	bool bRet = TRUE;

	// 先停止正在存储的文件
	UnPrepareForSaveAVI();

	// 重新创建
	bRet = PrepareForSaveAVI(m_bIsColor);

	return bRet;
}


//----------------------------------------------------------------------------------
/**
\brief  获取存储目录所在磁盘的剩余空间

\return nFreeBytes   剩余空间,单位为Byte
*/
//----------------------------------------------------------------------------------
int64_t CStoreAVI::__GetDiskFreeSpace() const
{
	int64_t nFreeBytes = 0;
	BOOL    bResult;

	//使用GetDiskFreeSpaceEx获取存储目录所在磁盘的剩余空间      
	bResult = GetDiskFreeSpaceEx(m_strStorePath, 0, 0,
		(PULARGE_INTEGER)&nFreeBytes);

	return nFreeBytes;
}
