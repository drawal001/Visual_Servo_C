//------------------------------------------------------------------------
/**
\file		GXBitmap.cpp
\brief		此类主要用于图像的显示和存储，图像显示和存储可以自适应黑白彩色相机，
图像存储可以存储为Bmp、Raw，对图像显示和存储进行了实现

<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>

\Date       2024-05-27
\Version    1.1.2405.9271
*/
//------------------------------------------------------------------------
#include "stdafx.h"
#include "GXBitmap.h"
#include <string.h>
#include <stdio.h>

///< 判断返回值宏定义
#define BITMAP_VERIFY_STATUS(emStatus) \
 if (GX_STATUS_SUCCESS != emStatus) \
                                   {\
	                                break;\
                                   }\
//---------------------------------------------------------------------------------
/**
\brief   构造函数
\param   hDevice 图像设备指针
\param   pWnd 窗体指针
\return  无
*/
//----------------------------------------------------------------------------------
CGXBitmap::CGXBitmap(GX_DEV_HANDLE& hDevice, CWnd* pWnd)
: m_pImageBuffer(NULL)
, m_i64ImageHeight(0)
, m_i64ImageWidth(0)
, m_i32RawSize(0)
, m_i32ConvertSize(0)
, m_pBmpInfo(NULL)
, m_pWnd(pWnd)
, m_pHwnd(NULL)
, m_hDC(NULL)
, m_hConvert(NULL)
, m_pSaveAVI(NULL)
, m_bIsColor(false)
, m_hDevice(hDevice)
, m_pBufferMono(NULL)
, m_i64PixelFormat(0)
, m_pBufferRaw16To8(NULL)
, m_emConvertType(RAW2RGB_NEIGHBOUR)
{
	memset(m_chBmpBuf,0, sizeof(m_chBmpBuf));
	if (NULL == pWnd)
	{
		throw std::invalid_argument("Null Pointer!");
	}
	if (NULL == hDevice)
	{
		throw std::invalid_argument("Invalid Device Handle!");
	}

	GX_STATUS     emStatus     = GX_STATUS_ERROR;
	emStatus = DxImageFormatConvertCreate(&m_hConvert);
	if (GX_STATUS_SUCCESS != emStatus)
	{
		throw std::invalid_argument("Initialization Failed!");
	}

	// 申请绘图句柄
	m_hDC = ::GetDC(m_pWnd->m_hWnd);

}


//---------------------------------------------------------------------------------
/**
\brief   构造函数
\param   hDevice 图像设备指针
\param   pWnd 窗体指针
\return  无
*/
//----------------------------------------------------------------------------------
CGXBitmap::CGXBitmap(GX_DEV_HANDLE& hDevice, HWND* pHwnd)
	:  m_pImageBuffer(NULL)
	, m_i64ImageHeight(0)
	, m_i64ImageWidth(0)
	, m_i32RawSize(0)
	, m_i32ConvertSize(0)
	, m_pBmpInfo(NULL)
	, m_pWnd(NULL)
	, m_pHwnd(pHwnd)
	, m_hDC(NULL)
	, m_hConvert(NULL)
	, m_pSaveAVI(NULL)
	, m_hDevice(hDevice)
	, m_pBufferMono(NULL)
	, m_i64PixelFormat(0)
	, m_pBufferRaw16To8(NULL)
	, m_emConvertType(RAW2RGB_NEIGHBOUR)
{
	memset(m_chBmpBuf,0, sizeof(m_chBmpBuf));
	if (NULL == pHwnd)
	{
		throw std::invalid_argument("Null Pointer!");
	}
	if (NULL == hDevice)
	{
		throw std::invalid_argument("Invalid Device Handle!");
	}

	GX_STATUS     emStatus = GX_STATUS_ERROR;

	// 申请图像格式转换句柄
	emStatus = DxImageFormatConvertCreate(&m_hConvert);
	if (GX_STATUS_SUCCESS != emStatus)
	{
		throw std::invalid_argument("Initialization Failed!");
	}

	// 申请绘图句柄
	m_hDC = ::GetDC(*m_pHwnd);
}

//---------------------------------------------------------------------------------
/**
\brief   析构函数

\return  无
*/
//----------------------------------------------------------------------------------
CGXBitmap::~CGXBitmap(void)
{
	GX_STATUS     emStatus = GX_STATUS_ERROR;

	m_objLock.Lock();
	 //清除为图像显示准备的资源
	if (NULL != m_pImageBuffer)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	if (NULL != m_pBufferMono)
	{
		delete[] m_pBufferMono;
		m_pBufferMono = NULL;
	}
	if (NULL != m_pBufferRaw16To8) 
	{
		delete[] m_pBufferRaw16To8;
		m_pBufferRaw16To8 = NULL;
	}

	// 关闭图像格式转换句柄
	if (NULL != m_hConvert)
	{
		DxImageFormatConvertDestroy(m_hConvert);
		m_hConvert = NULL;
	}


	// 关闭绘图句柄
	if (NULL != m_hDC)
	{
		if (NULL != m_pWnd) 
		{
			::ReleaseDC(m_pWnd->m_hWnd, m_hDC);
			m_hDC = NULL;
		}
		else if (NULL != m_pHwnd) 
		{
			::ReleaseDC(*m_pHwnd, m_hDC);
			m_hDC = NULL;
		}else
		{
		
		}
	}
	m_objLock.Unlock();
}

//---------------------------------------------------------------------------------
/**
\brief   为相机图像显示准备资源

\return  无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::PrepareForShowImg()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 释放旧内存, 原因是停止采集操作和回调函数是异步的
	// 若在停止采集后直接释放内存则可能因为回调函数还在运行导致程序崩溃
	// 所以改为在开始采集时释放旧内存
	UnPrepareForShowImg();

	// 更新图像的宽、高、像素格式、转换后的像素格式等
	__GetBasicAttribute();

	// 设置图像转换句柄
	__SetConvertHandle(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	if (m_bIsColor)
	{
		// 为彩色图像显示准备资源,分配Buffer
		emStatus = __ColorPrepareForShowImg();
		VERIFY_STATUS_RET(emStatus);
	}
	else
	{
		// 为黑白图像显示准备资源,分配Buffer
		emStatus = __MonoPrepareForShowImg();
		VERIFY_STATUS_RET(emStatus);

	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief     释放为图像显示准备的资源

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::UnPrepareForShowImg()
{
	m_objLock.Lock();
	if (NULL != m_pImageBuffer)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	if (NULL != m_pBufferMono)
	{
		delete[] m_pBufferMono;
		m_pBufferMono = NULL;
	}
	if (NULL != m_pBufferRaw16To8) 
	{
		delete[] m_pBufferRaw16To8;
		m_pBufferRaw16To8 = NULL;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief     显示图像
\param     pCallbackFrame  回调采集数据类型
\param     pDeviceSNFPS    帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::Show(GX_FRAME_CALLBACK_PARAM * pCallbackFrame, char * pDeviceSNFPS)
{
	void* pRawImgBuff = const_cast<void*>(pCallbackFrame->pImgBuf);
	if (0 == pCallbackFrame->status)
	{
		__Show(pRawImgBuff, pDeviceSNFPS);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     显示图像
\param     pFrame          单帧采集数据类型
\param     pDeviceSNFPS    帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::Show(GX_FRAME_DATA * pFrame, char * pDeviceSNFPS)
{
	void* pRawImgBuff = const_cast<void*>(pFrame->pImgBuf);
	if (0 == pFrame->nStatus)
	{
		__Show(pRawImgBuff, pDeviceSNFPS);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     调用图像综合处理函数处理完图像数据后显示
\param     pCallbackFrame  回调采集数据类型
\param     pCallbackFrame  图像处理功能设置结构体的指针
\param     pDeviceSNFPS    帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::ShowAfterProcess(GX_FRAME_CALLBACK_PARAM * pCallbackFrame, void * pImageProcess, char * pDeviceSNFPS)
{
	void* pImgBuff = const_cast<void*>(pCallbackFrame->pImgBuf);
	if (0 == pCallbackFrame->status)
	{
		__ShowAfterProcess(pImgBuff, pImageProcess, pDeviceSNFPS);
	}

}

//----------------------------------------------------------------------------------
/**
\brief     调用图像综合处理函数处理完图像数据后显示
\param     pFrame          单帧采集数据类型
\param     pCallbackFrame  图像处理功能设置结构体的指针
\param     pDeviceSNFPS    帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::ShowAfterProcess(GX_FRAME_DATA * pFrame, void * pImageProcess, char * pDeviceSNFPS)
{
	void* pImgBuff = const_cast<void*>(pFrame->pImgBuf);
	if (0 == pFrame->nStatus)
	{
		__ShowAfterProcess(pImgBuff, pImageProcess, pDeviceSNFPS);
	}
}

//---------------------------------------------------------------------------------
/**
\brief   将m_pBufferRGB中图像显示到界面
\param   strDeviceSNFPS  设备帧率序列号

\return  无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__DrawImg(char* strDeviceSNFPS)
{
	int32_t i32WndWidth  = 0;
	int32_t i32WndHeight = 0;

	m_objLock.Lock();
	if ((NULL == m_hDC)
		|| (NULL == m_pImageBuffer))
	{
		m_objLock.Unlock();
		return;
	}

	//// 为画图做准备
	RECT objRect;
	(NULL != m_pWnd) ? m_pWnd->GetClientRect(&objRect) : GetClientRect(*m_pHwnd, &objRect);
	i32WndWidth  = objRect.right - objRect.left;
	i32WndHeight = objRect.bottom - objRect.top;

	HDC      objMemDC = ::CreateCompatibleDC(m_hDC);
	HBITMAP  objMemBmp= CreateCompatibleBitmap(m_hDC, i32WndWidth, i32WndHeight);
	::SelectObject(objMemDC,objMemBmp);

	// 必须调用该语句，否则图像出现水纹
	::SetStretchBltMode(objMemDC, COLORONCOLOR);
	::StretchDIBits(objMemDC,
		0,
		0,
		i32WndWidth,
		i32WndHeight,
		0,
		0,
		(int)m_i64ImageWidth,
		(int)m_i64ImageHeight,
		m_pImageBuffer,
		m_pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	if (NULL != strDeviceSNFPS)
	{
		TextOut(objMemDC, 0, 0, strDeviceSNFPS, (int)strlen(strDeviceSNFPS));
	}
		StretchBlt(m_hDC,
			0,
			0,
			i32WndWidth,
			i32WndHeight,
			objMemDC,
			0,
			0,
			i32WndWidth,
			i32WndHeight,
			SRCCOPY);

	::DeleteDC(objMemDC);
	DeleteObject(objMemBmp);

	m_objLock.Unlock();
}


//----------------------------------------------------------------------------------
/**
\brief     翻转图像buffer

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__FilpImage(void* pRawImageBuffer)
{
	m_objLock.Lock();
	if ((NULL == m_pImageBuffer)
		||(NULL == pRawImageBuffer))
	{
		m_objLock.Unlock();
		return;
	}
	BYTE* pRawBuffer = reinterpret_cast<BYTE*>(pRawImageBuffer);
	if (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat)
	{
		// BGR格式需要翻转数据后显示
		for (int32_t i = 0; i < m_i64ImageHeight; ++i)
		{
			memcpy(m_pImageBuffer + i * m_i64ImageWidth * PIXEL, pRawBuffer + (m_i64ImageHeight - i - 1) * m_i64ImageWidth * PIXEL
				, (size_t)m_i64ImageWidth * PIXEL);
		}
	}
	else if ((GX_PIXEL_FORMAT_R8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_G8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_B8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_MONO8 == m_i64PixelFormat))
	{
		// 黑白相机需要翻转数据后显示
		for (int32_t i = 0; i < m_i64ImageHeight; ++i)
		{
			memcpy(m_pImageBuffer + i * m_i64ImageWidth, pRawBuffer + (m_i64ImageHeight - i - 1) * m_i64ImageWidth
				, (size_t)m_i64ImageWidth);
		}
	}else
	{
	
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief     判断图像是否需要反转

\return    true需要反转，false不需要反转
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::__NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat) const
{
	if ((GX_PIXEL_FORMAT_BGR8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_R8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_G8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_B8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_MONO8 == emPixelFormat))
	{
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------
/**
\brief  为彩色图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__ColorPrepareForShowImg()
{
	const int32_t BITCOUNT = 24;
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_i64ImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = BITCOUNT; // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	//----------------------------为图像数据分配Buffer---------------------

	// 为经过RGB转换后的图像分配Buffer
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight * PIXEL)];
	if (NULL == m_pImageBuffer)
	{
		return GX_STATUS_ERROR;
	}

	m_pBufferRaw16To8 = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pBufferRaw16To8)
	{
		UnPrepareForShowImg();
		return GX_STATUS_ERROR;
	}
	return GX_STATUS_SUCCESS;
}

//----------------------------------------------------------------------------------
/**
\brief 为黑白图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
GX_STATUS  CGXBitmap::__MonoPrepareForShowImg()
{
	const int32_t BITCOUNT = 8;
//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_i64ImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = BITCOUNT;  // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColor)
	{
		const int32_t PALETTE_SIZE = 256;
		// 黑白相机需要进行初始化调色板操作
		for (int32_t i = 0; i < PALETTE_SIZE; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//----------------------------为图像数据分配Buffer---------------------
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pImageBuffer)
	{
		return GX_STATUS_ERROR;
	}

	m_pBufferMono = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pBufferMono)
	{
		UnPrepareForShowImg();
		return GX_STATUS_ERROR;
	}

	m_pBufferRaw16To8 = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pBufferRaw16To8)
	{
		UnPrepareForShowImg();
		return GX_STATUS_ERROR;
	}

	return GX_STATUS_SUCCESS;
}



//----------------------------------------------------------------------------------
/**
\brief     调用图像质量提升函数提升图像质量
\param     pCallbackFrame      回调采集数据类型
\param     i64ColorCorrection  颜色校正值
\param     pContrastLut        对比度查找表
\param     pGammaLut           Gamma查找表
\param     emChannelOrder      输出图像的 RGB 通道顺序 
\param     pDeviceSNFPS        帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::ShowAfterImprovment(GX_FRAME_CALLBACK_PARAM * pCallbackFrame, int64_t i64ColorCorrection, BYTE * pContrastLut
									, BYTE * pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder, char * pDeviceSNFPS)
{
	void* pImgBuff = const_cast<void*>(pCallbackFrame->pImgBuf);
	if (0 == pCallbackFrame->status)
	{
		__ShowAfterImprovment(pImgBuff, i64ColorCorrection, pContrastLut, pGammaLut, emChannelOrder, pDeviceSNFPS);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     调用图像质量提升函数提升图像质量
\param     pFrame              单帧采集数据类型
\param     i64ColorCorrection  颜色校正值
\param     pContrastLut        对比度查找表
\param     pGammaLut           Gamma查找表
\param     pDeviceSNFPS        帧率

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::ShowAfterImprovment(GX_FRAME_DATA * pFrame, int64_t i64ColorCorrection, BYTE * pContrastLut
									, BYTE * pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder, char * pDeviceSNFPS)
{
	void* pImgBuff = const_cast<void*>(pFrame->pImgBuf);
	if (0 == pFrame->nStatus)
	{
		__ShowAfterImprovment(pImgBuff, i64ColorCorrection, pContrastLut, pGammaLut, emChannelOrder, pDeviceSNFPS);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     得到相机采图的宽高像素格式等属性

\return    无
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__GetBasicAttribute()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_INT_VALUE stIntValue;

	// 获取宽度
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageWidth = stIntValue.nCurValue;

	// 获取高度
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageHeight = stIntValue.nCurValue;

	// 获取本地句柄
	GX_LOCAL_DEV_HANDLE hLocalDev = NULL;
	emStatus = GXGetLocalDeviceHandleFromDev(m_hDevice, &hLocalDev);
	VERIFY_STATUS_RET(emStatus);

	// 获取输出像素格式
	GX_ENUM_VALUE stEnumValue;
	emStatus = GXGetEnumValue(hLocalDev, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus) 
	{
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus) 
	{
		// 获取当前像素格式
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else 
	{
		return GX_STATUS_ERROR;
	}

	m_bIsColor = IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	//计算原始图像buffer的大小
	emStatus = DxImageFormatConvertGetBufferSizeForConversion(m_hConvert
		, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat), static_cast<VxUint32>(m_i64ImageWidth)
		, static_cast<VxUint32>(m_i64ImageHeight), reinterpret_cast<int*>(&m_i32RawSize) );
	VERIFY_STATUS_RET(emStatus);

	//计算转换后图像buffer的大小
	if (m_bIsColor)
	{
		emStatus = DxImageFormatConvertGetBufferSizeForConversion(m_hConvert, GX_PIXEL_FORMAT_BGR8
			, static_cast<VxUint32>(m_i64ImageWidth), static_cast<VxUint32>(m_i64ImageHeight)
			, reinterpret_cast<int*>(&m_i32ConvertSize));
		VERIFY_STATUS_RET(emStatus);
	}
	else
	{
		emStatus = DxImageFormatConvertGetBufferSizeForConversion(m_hConvert, GX_PIXEL_FORMAT_MONO8
			, static_cast<VxUint32>(m_i64ImageWidth), static_cast<VxUint32>(m_i64ImageHeight)
			, reinterpret_cast<int*>(&m_i32ConvertSize));
		VERIFY_STATUS_RET(emStatus);
	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief     用于显示图像
\param     pRawImgBuff   图像buffer
\param     pDeviceSNFPS  帧率，默认为空，即不显示

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__Show(void* pRawImgBuff, char* pDeviceSNFPS)
{
	GX_STATUS     emStatus = GX_STATUS_ERROR;
	if (__NeedFilp(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat)))
	{
		__FilpImage(pRawImgBuff);
	}
	else
	{
		m_objLock.Lock();
		emStatus = DxImageFormatConvert(m_hConvert, pRawImgBuff, m_i32RawSize, m_pImageBuffer, m_i32ConvertSize
			, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat), static_cast<VxUint32>(m_i64ImageWidth)
			, static_cast<VxUint32>(m_i64ImageHeight), true);
		m_objLock.Unlock();
		GX_VERIFY(emStatus);
	}

	// 在屏幕上绘图
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     用于图像综合处理后显示图像
\param     pRawImgBuff         原始图像数据
\param     pImageProcess       图像处理功能设置结构体的指针
\param     pDeviceSNFPS        帧率，默认为空，即不显示

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ShowAfterProcess(void * pRawImgBuff,  void * pImageProcess, char * pDeviceSNFPS)
{
	// 图像处理
	__ImageProcess(m_bIsColor, pRawImgBuff, pImageProcess);

	// 在屏幕上绘图
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     用于图像综合处理后显示图像
\param     pRawImgBuff         图像数据
\param     i64ColorCorrection  颜色校正值
\param     pContrastLut        对比度查找表
\param     pGammaLut           Gamma查找表
\param     pDeviceSNFPS        帧率，默认为空，即不显示

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ShowAfterImprovment(void * pRawImgBuff,int64_t i64ColorCorrection, BYTE * pContrastLut, BYTE * pGammaLut
									  , DX_RGB_CHANNEL_ORDER emChannelOrder, char * pDeviceSNFPS)
{
	GX_STATUS     emStatus = GX_STATUS_ERROR;

	m_objLock.Lock();
	if ((NULL == pRawImgBuff)
		|| (NULL == m_pImageBuffer)) 
	{
		m_objLock.Unlock();
		return;
	}

	do 
	{
		if (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat)
		{
			BYTE* pRawBuffer = reinterpret_cast<BYTE*>(pRawImgBuff);
			for (int32_t i = 0; i < m_i64ImageHeight; ++i)
			{
				memcpy(m_pImageBuffer + i * m_i64ImageWidth * PIXEL, pRawBuffer + (m_i64ImageHeight - i - 1) * m_i64ImageWidth * PIXEL
					, (size_t)m_i64ImageWidth * PIXEL);
			}
		}
		else
		{
			emStatus = DxImageFormatConvert(m_hConvert, pRawImgBuff, m_i32RawSize, m_pImageBuffer, m_i32ConvertSize
				, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat), static_cast<VxUint32>(m_i64ImageWidth)
				, static_cast<VxUint32>(m_i64ImageHeight), true);
			DX_VERIFY(emStatus);
		}

		//提升图像质量
		emStatus = DxImageImprovmentEx(m_pImageBuffer
			, m_pImageBuffer
			, (VxUint32)m_i64ImageWidth
			, (VxUint32)m_i64ImageHeight
			, i64ColorCorrection
			, pContrastLut
			, pGammaLut
			, emChannelOrder);
		DX_VERIFY(emStatus);
	} while (0);
	m_objLock.Unlock();

	if (GX_STATUS_SUCCESS != emStatus)
	{
		return; //出现异常，则不绘图，直接返回
	}

	// 在屏幕上绘图
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     用于图像综合处理函数处理图像数据
\param     bIsColor            当前像素是否为彩色，true彩色，false 黑白
\param     pImageBuffer        图像buffer
\param     pImageProcess       图像处理功能设置结构体的指针

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ImageProcess(bool bIsColor, void * pImageBuffer, void * pImageProcess)
{
	m_objLock.Lock();
	if ((NULL == pImageBuffer)
		|| (NULL == m_pBufferRaw16To8)) 
	{
		m_objLock.Unlock();
		return;
	}
	GX_STATUS   emStatus       = GX_STATUS_ERROR;
	DX_STATUS   emDxStatus     = DX_OK;
	BYTE *      pImageRGBBuf   = NULL;
	VxInt32     nSrcBufferSize = 0;              
	VxInt32     nDstBufferSize = 0;

	DX_IMAGE_FORMAT_CONVERT_HANDLE pImgFormatConvert = NULL;
	do {

		//创建图像转换句柄
		emDxStatus = (DX_STATUS)DxImageFormatConvertCreate(&pImgFormatConvert);
		DX_VERIFY(emDxStatus);

		//为转换后的图像Buffer申请内存
		pImageRGBBuf = new(std::nothrow) BYTE[static_cast<uint32_t>(m_i64ImageWidth * m_i64ImageHeight)];
		if (NULL == pImageRGBBuf)
		{
			break;
		}

		// 判断是否为8位图，不是八位图则转为8位图
		bool bIsRaw8 = __IsPixelFormat8(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));
		if (!bIsRaw8)
		{
			// 如果为MONO10_PACKED或MONO12_PACKED像素格式则先将其转换为Mono8
			if ((GX_PIXEL_FORMAT_MONO10_PACKED == static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat))
				|| (GX_PIXEL_FORMAT_MONO12_PACKED == static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat)))
			{
				// 设置有效位
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetValidBits(pImgFormatConvert, GetBestValudBit((GX_PIXEL_FORMAT_ENTRY)m_i64PixelFormat));
				DX_VERIFY(emDxStatus);

				// 设置目标像素格式为Mono8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pImgFormatConvert, GX_PIXEL_FORMAT_MONO8);
				DX_VERIFY(emDxStatus);

				// 获取源Buffer大小
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pImgFormatConvert, (GX_PIXEL_FORMAT_ENTRY)m_i64PixelFormat
					, (VxUint32)m_i64ImageWidth, (VxUint32)m_i64ImageHeight, &nSrcBufferSize);
				DX_VERIFY(emDxStatus);

				// 获取目标Buffer大小
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pImgFormatConvert, GX_PIXEL_FORMAT_MONO8
					, (VxUint32)m_i64ImageWidth, (VxUint32)m_i64ImageHeight, &nDstBufferSize);
				DX_VERIFY(emDxStatus);

				// 像素格式转换
				emDxStatus = (DX_STATUS)DxImageFormatConvert(pImgFormatConvert, (void*)pImageBuffer, nSrcBufferSize, pImageRGBBuf, nDstBufferSize
					, (GX_PIXEL_FORMAT_ENTRY)m_i64PixelFormat, (VxUint32)m_i64ImageWidth, (VxUint32)m_i64ImageHeight, false);
				DX_VERIFY(emDxStatus);

				memcpy(m_pBufferRaw16To8, pImageRGBBuf,static_cast<size_t>(m_i64ImageWidth * m_i64ImageHeight));

			}
			else
			{
				emStatus = DxRaw16toRaw8(pImageBuffer, m_pBufferRaw16To8, static_cast<VxUint32>(m_i64ImageWidth)
					, static_cast<VxUint32>(m_i64ImageHeight), GetBestValudBit(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat)));
				DX_VERIFY(emDxStatus);
			}
		}
		else
		{
			memcpy(m_pBufferRaw16To8, pImageBuffer, static_cast<size_t>(m_i64ImageWidth * m_i64ImageHeight));
		}

		if (!bIsColor)//黑白相机
		{
			MONO_IMG_PROCESS* pMono = reinterpret_cast<MONO_IMG_PROCESS*>(pImageProcess);
			emStatus = DxMono8ImgProcess(m_pBufferRaw16To8, m_pBufferMono, static_cast<VxUint32>(m_i64ImageWidth)
				, static_cast<VxUint32>(m_i64ImageHeight), pMono);
			DX_VERIFY(emDxStatus);

			// 黑白相机需要翻转数据后显示
			for (int32_t i = 0; i < m_i64ImageHeight; ++i)
			{
				memcpy(m_pImageBuffer + i * m_i64ImageWidth, m_pBufferMono + (m_i64ImageHeight - i - 1) * m_i64ImageWidth
					, (size_t)m_i64ImageWidth);
			}
		}
		else//彩色相机
		{
			COLOR_IMG_PROCESS* pColor = reinterpret_cast<COLOR_IMG_PROCESS*>(pImageProcess);
			emStatus = DxRaw8ImgProcess(m_pBufferRaw16To8, m_pImageBuffer, static_cast<VxUint32>(m_i64ImageWidth)
				, static_cast<VxUint32>(m_i64ImageHeight), pColor);
			DX_VERIFY(emDxStatus);
		}
	} while (0);
	m_objLock.Unlock();

	if (NULL != pImageRGBBuf)
	{
		delete[] pImageRGBBuf;
		pImageRGBBuf = NULL;
	}

	// 销毁图像转换句柄
	DxImageFormatConvertDestroy(pImgFormatConvert);
}

//----------------------------------------------------------------------------------
/**
\brief     计算宽度所占的字节数
\param     nWidth  图像宽度
\param     bIsColor  是否是彩色相机
\return    图像一行所占的字节数
*/
//----------------------------------------------------------------------------------
int64_t CGXBitmap::__GetStride(int64_t i64Width, bool bIsColor) const
{
	return bIsColor ? (i64Width * PIXEL) : i64Width;
}
//----------------------------------------------------------------------------------
/**
\brief     存储Bmp图像
\param     pFilePath  图像存储的路径

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveBmp(const char* pFilePath)
{
	const uint32_t FILE_TYPE = 8;

	DWORD		         dwImageSize = static_cast<DWORD>(__GetStride(m_i64ImageWidth, m_bIsColor) * m_i64ImageHeight);
	BITMAPFILEHEADER     stBfh	     = {0};
	DWORD		         dwBytesRead = 0;

	stBfh.bfType	= (WORD)'M' << FILE_TYPE | 'B';			 //定义文件类型
	stBfh.bfOffBits = m_bIsColor ?sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		:sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * 4);	//定义文件头大小true为彩色,false为黑白
	stBfh.bfSize	= stBfh.bfOffBits + dwImageSize; //文件大小

	DWORD dwBitmapInfoHeader = m_bIsColor ?sizeof(BITMAPINFOHEADER)
		:sizeof(BITMAPINFOHEADER) + (256 * 4);	//定义BitmapInfoHeader大小true为彩色,false为黑白

	//创建文件
	HANDLE hFile = ::CreateFile(pFilePath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,														
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		throw std::runtime_error("Handle is invalid");
	}

	::WriteFile(hFile, &stBfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
	::WriteFile(hFile, m_pBmpInfo, dwBitmapInfoHeader, &dwBytesRead, NULL); //黑白和彩色自适应
	::WriteFile(hFile, m_pImageBuffer, dwImageSize, &dwBytesRead, NULL);

	CloseHandle(hFile);
}

//----------------------------------------------------------------------------------
/**
\brief     存储Raw图像
\param     pFilePath  存储的路径+显示图像文件名

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveRaw(void* pRawBuffer,const char* pFilePath)
{
	DWORD   bytes_read = 0;                // 文件读取的长度

	// 创建文件
	HANDLE hFile = ::CreateFile(pFilePath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)   // 创建失败则返回
	{
		throw std::invalid_argument("Invalid Device Handle!");
	}
	else                                 // 保存Raw图像          
	{
		::WriteFile(hFile, pRawBuffer, static_cast<DWORD>(strlen((char*)pRawBuffer)), &static_cast<DWORD>(bytes_read), NULL);
		CloseHandle(hFile);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     初始化存储AVI所需要的资源和配置
\param    m_pBmpInfo     
\param    m_i64ImageWidth   存储视频的帧率
\param    m_i64ImageHeight   图像的宽
\param    strStorePath       存储的路径
\param    ui64PlayFPS        存储视频的帧率

\return    无
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::InitAVI(char* strStorePath, uint64_t ui64PlayFPS)
{
	m_pSaveAVI = new(std::nothrow) CStoreAVI(m_pBmpInfo, m_i64ImageWidth, m_i64ImageHeight,strStorePath, ui64PlayFPS);
	if (NULL == m_pSaveAVI) 
	{
		return false;
	}
	bool bFlag = m_pSaveAVI->PrepareForSaveAVI(m_bIsColor);
	return bFlag;
}

//----------------------------------------------------------------------------------
/**
\brief     反初始化存储AVI所需要的资源和配置

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::UnInitAVI()
{
	m_pSaveAVI->UnPrepareForSaveAVI();
	if (NULL != m_pSaveAVI)
	{
		delete m_pSaveAVI;
		m_pSaveAVI = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief     存储AVI文件
\param    pBuffer  待存储的图像buffer

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveAVI(BYTE *pBuffer)
{

	// 保存AVI视频
	if (NULL == pBuffer) 
	{
		m_pSaveAVI->SaveAVI(m_pImageBuffer);
	}
	else 
	{
		m_pSaveAVI->SaveAVI(pBuffer);
	}
	
	//若不需要检测磁盘空间或对AVI文件的存储大小进行限制,可调用ExSaveAVI实现视频保存
	//m_objSaveAVI->ExSaveAVI(m_pImageBuffer); 
}

//----------------------------------------------------------------------------------
/**
\brief     判断输入像素是否为彩色像素格式
\param    bIsColorFilter  待判断的像素格式

\return    true 当前为彩色像素，false不是彩色像素
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::IsColor(GX_PIXEL_FORMAT_ENTRY i32Pixel)
{
	//将图像格式和下述宏定义做按位与（&）运算，可判断像素格式是mono还是RGB
	int32_t i32PixelMono = 0x01000000;               //判断是否为MONO格式的掩码
	int32_t i32PixelRgb = 0x20000000;                //判断是否为RGB格式的掩码 
	int32_t i32PixelMonoRgbCustom = 0x80000000;      //判断是否为MONO格式的掩码 
	int32_t i32PixelColorMask = 0xFF000000;          //判断是否为MONO格式的掩码

	//将图像格式与下述宏定义做按位与（&）运算，可得到像素格式的ID
	int32_t i32PixelIdMask = 0x0000FFFF;

	bool bIsMono = ((i32PixelColorMask & i32Pixel) == i32PixelMono); // 是否为mono格式

	bool bIsRgb = ((i32PixelColorMask & i32Pixel) == i32PixelRgb);  // 是否为RGB格式           
	bool bIsBayer = (((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR8) <= (i32PixelIdMask & i32Pixel)
		&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG12))
		|| ((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR14) <= (i32PixelIdMask & i32Pixel)
			&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG14))
		|| ((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR16) <= (i32PixelIdMask & i32Pixel)
			&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG16)));   // 是否为Bayer格式

	bool bIsColor = !(bIsMono && (!bIsBayer) && (!bIsRgb));  // 用于判断是否为黑白相机

	return bIsColor;
}

//----------------------------------------------------------------------------------
/**
\brief     选择视频编码器

\return    无
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::SelectCompressor()
{
	if (NULL == m_pWnd) 
	{
		return m_pSaveAVI->SelectCompressor(m_pHwnd);
	}
	else 
	{
		return m_pSaveAVI->SelectCompressor(&m_pWnd->m_hWnd);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     通过GX_PIXEL_FORMAT_ENTRY获取最优Bit位
\param     emPixelFormatEntry 图像数据格式
\return    最优Bit位
*/
//----------------------------------------------------------------------------------
DX_VALID_BIT CGXBitmap::GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	DX_VALID_BIT emValidBits = DX_BIT_0_7;
	switch (emPixelFormatEntry)
	{
	case GX_PIXEL_FORMAT_MONO8:
	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
	case GX_PIXEL_FORMAT_BGR8:
	case GX_PIXEL_FORMAT_RGB8:
	{
		emValidBits = DX_BIT_0_7;
		break;
	}
	case GX_PIXEL_FORMAT_MONO10:
	case GX_PIXEL_FORMAT_MONO10_P:
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
	case GX_PIXEL_FORMAT_MONO10_PACKED:
	{
		emValidBits = DX_BIT_2_9;
		break;
	}
	case GX_PIXEL_FORMAT_MONO12:
	case GX_PIXEL_FORMAT_MONO12_P:
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
	case GX_PIXEL_FORMAT_MONO12_PACKED:
	{
		emValidBits = DX_BIT_4_11;
		break;
	}
	case GX_PIXEL_FORMAT_MONO14:
	{
		//暂时没有这样的数据格式待升级
		break;
	}
	case GX_PIXEL_FORMAT_MONO16:
	case GX_PIXEL_FORMAT_BAYER_GR16:
	case GX_PIXEL_FORMAT_BAYER_RG16:
	case GX_PIXEL_FORMAT_BAYER_GB16:
	case GX_PIXEL_FORMAT_BAYER_BG16:
	{
		//暂时没有这样的数据格式待升级
		break;
	}
	default:
	{	
		break;
	}
	}
	return emValidBits;
}

//----------------------------------------------------------------------------------
/**
\brief     设置图像转换句柄
\param     hConvertHandle 图像转换句柄
\param     emInputPixelForma 输入图像的像素格式

\return    DX_OK 成功，否则失败
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__SetConvertHandle(GX_PIXEL_FORMAT_ENTRY emInputPixelFormat)
{
	VxInt32 emStatus = DX_OK;

	//彩色转BGR8,黑白转MONO8
	if (m_bIsColor) 
	{
		// 设置图像格式转换句柄，转换为BGR8格式
		emStatus = DxImageFormatConvertSetOutputPixelFormat(m_hConvert, GX_PIXEL_FORMAT_BGR8);
		VERIFY_STATUS_RET(emStatus);
	}
	else 
	{
		// 设置图像格式转换句柄，转换为Mono8格式
		emStatus = DxImageFormatConvertSetOutputPixelFormat(m_hConvert, GX_PIXEL_FORMAT_MONO8);
		VERIFY_STATUS_RET(emStatus)
	}

	// 设置插值方式
	emStatus = DxImageFormatConvertSetInterpolationType(m_hConvert, m_emConvertType);
	VERIFY_STATUS_RET(emStatus);

	// 获取有效位数
	DX_VALID_BIT emValidBits = GetBestValudBit(emInputPixelFormat);

	// 设置有效位数
	emStatus = DxImageFormatConvertSetValidBits(m_hConvert, emValidBits);
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief     判断PixelFormat是否为8位
\param     emPixelFormatEntry 图像数据格式
\return    true为8为数据，false为非8位数据
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::__IsPixelFormat8(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	bool bIsPixelFormat8 = false;
	const unsigned  PIXEL_FORMATE_BIT = 0x00FF0000;  ///<用于与当前的数据格式进行与运算得到当前的数据位数
	unsigned uiPixelFormatEntry = (unsigned)emPixelFormatEntry;
	if ((uiPixelFormatEntry & PIXEL_FORMATE_BIT) == GX_PIXEL_8BIT)
	{
		bIsPixelFormat8 = true;
	}
	return bIsPixelFormat8;
}

//----------------------------------------------------------------------------------
/**
\brief     设置插值方法
\param     emConvertType 插值方法

\return    DX_OK 成功，否则失败
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SetInterpolationAlgorithm(DX_BAYER_CONVERT_TYPE emConvertType)
{
	// 设置插值方式
	m_emConvertType = emConvertType;
	DxImageFormatConvertSetInterpolationType(m_hConvert, emConvertType);
}
