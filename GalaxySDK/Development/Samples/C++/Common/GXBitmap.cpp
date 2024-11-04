//------------------------------------------------------------------------
/**
\file		GXBitmap.cpp
\brief		此类主要用于图像的显示和存储，图像显示和存储可以自适应黑白彩色相机，
图像存储可以存储为Bmp、Raw，对图像显示和存储进行了实现

\Date       2024-03-07
\Version    1.1.2403.9071
*/
//------------------------------------------------------------------------
#include "stdafx.h"
#include "GXBitmap.h"


//---------------------------------------------------------------------------------
//**
//\brief   构造函数
//\param   objCGXDevicePointer 图像设备指针
//\param   pWnd 窗体指针
//\return  无
//*/
//----------------------------------------------------------------------------------
CGXBitmap::CGXBitmap(CGXDevicePointer& objCGXDevicePointer,CWnd* pWnd)
:m_pWnd(pWnd)
,m_hDC(NULL)
,m_bIsColor(false)
,m_i64ImageHeight(0)
,m_i64ImageWidth(0)
,m_pBmpInfo(NULL)
,m_pImageBuffer(NULL)
,m_i64PixelFormat(0)
,m_i64RawSize(0)
,m_i64ConvertSize(0)
,m_objCGXDevicePointer(objCGXDevicePointer)
{

	if ((objCGXDevicePointer.IsNull())||(NULL == pWnd))
	{
		throw std::runtime_error("Argument is error");
	}

	HWND hWnd = pWnd->m_hWnd;
	if (!::IsWindow(hWnd))
	{
		throw std::runtime_error("The HWND must be form");
	}

	m_hDC  = ::GetDC(m_pWnd->m_hWnd);
	memset(m_chBmpBuf,0,sizeof(m_chBmpBuf));

	//初始化图像转换对象
	m_pConvert = IGXFactory::GetInstance().CreateImageFormatConvert();

	//初始化图像质量提升对象
	m_pProcess = IGXFactory::GetInstance().CreateImageProcess();

}


//---------------------------------------------------------------------------------
/**
\brief   析构函数

\return  无
*/
//----------------------------------------------------------------------------------
CGXBitmap::~CGXBitmap(void)
{
	m_objLock.Lock();
	//释放pDC
	::ReleaseDC(m_pWnd->m_hWnd, m_hDC);

	if (m_pImageBuffer != NULL)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief     通过GX_PIXEL_FORMAT_ENTRY获取最优Bit位
\param     emPixelFormatEntry 图像数据格式
\return    最优Bit位
*/
//----------------------------------------------------------------------------------
GX_VALID_BIT_LIST CGXBitmap::GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry) const
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	switch (emPixelFormatEntry)
	{
		case GX_PIXEL_FORMAT_R8:
		case GX_PIXEL_FORMAT_G8:
		case GX_PIXEL_FORMAT_B8:
		case GX_PIXEL_FORMAT_MONO8:
		case GX_PIXEL_FORMAT_BAYER_GR8:
		case GX_PIXEL_FORMAT_BAYER_RG8:
		case GX_PIXEL_FORMAT_BAYER_GB8:
		case GX_PIXEL_FORMAT_BAYER_BG8:
			{
				emValidBits = GX_BIT_0_7;
				break;
			}
		case GX_PIXEL_FORMAT_MONO10:
		case GX_PIXEL_FORMAT_MONO10_P:
		case GX_PIXEL_FORMAT_MONO10_PACKED:
		case GX_PIXEL_FORMAT_BAYER_GR10:
		case GX_PIXEL_FORMAT_BAYER_RG10:
		case GX_PIXEL_FORMAT_BAYER_GB10:
		case GX_PIXEL_FORMAT_BAYER_BG10:
			{
				emValidBits = GX_BIT_2_9;
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
				emValidBits = GX_BIT_4_11;
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
			//返回默认值GX_BIT_0_7
			break;
	}
	return emValidBits;
}

//---------------------------------------------------------------------------------
/**
\brief   为彩色相机图像显示准备资源

\return  无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ColorPrepareForShowImg()
{
	const int32_t BIT_COUNT = 24;
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_i64ImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount        = BIT_COUNT;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	//为经过翻转后的图像数据分配空间
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight * PIXEL)];
	if (NULL == m_pImageBuffer)
	{
		throw std::runtime_error("Fail to allocate memory");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   为黑白相机图像显示准备资源

\return  无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__MonoPrepareForShowImg()
{
	const int32_t BIT_COUNT = 8;
	//----------------------初始化bitmap头---------------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_i64ImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= BIT_COUNT; // 黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	// 黑白图像需要初始化调色板
	const int32_t PALETTE = 256;
	for(int32_t i32Index=0; i32Index<PALETTE; ++i32Index)
	{
		m_pBmpInfo->bmiColors[i32Index].rgbBlue	     = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbGreen	 = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbRed		 = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbReserved =0;
	}

	//为经过翻转后的图像数据分配空间
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pImageBuffer)
	{
		throw std::runtime_error("Fail to allocate memory");
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
	if (NULL == m_pImageBuffer)
	{
		m_objLock.Unlock();
		return;
	}

	// 为画图做准备
	RECT objRect;
	m_pWnd->GetClientRect(&objRect);
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
		(int32_t)m_i64ImageWidth,
		(int32_t)m_i64ImageHeight,
		m_pImageBuffer,
		m_pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY
		);
	m_objLock.Unlock();
	if (NULL != strDeviceSNFPS)
	{
		TextOut(objMemDC, 0, 0, strDeviceSNFPS, (int32_t)strlen(strDeviceSNFPS));
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
\brief     用于显示图像
\param     objCImageDataPointer  图像数据对象
\param     strDeviceSNFPS        图像帧率序列号
\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::Show(CImageDataPointer& objCImageDataPointer,char* pDeviceSNFPS)
{
	if (objCImageDataPointer.IsNull())
	{
		throw std::runtime_error("NULL pointer dereferenced");
	}

	GX_STATUS     emStatus = GX_STATUS_ERROR;
	if (__NeedFilp(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat)))
	{
		__FilpImage(objCImageDataPointer->GetBuffer());
	}
	else
	{
		m_objLock.Lock();
		if (NULL == m_pImageBuffer)
		{
			m_objLock.Unlock();
			return;
		}
		m_pConvert->Convert(objCImageDataPointer, m_pImageBuffer, static_cast<size_t>(m_i64ConvertSize), true);
		m_objLock.Unlock();
	}

	// 在屏幕上绘图
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     用于图像处理后并显示图像
\param     objCfg  图像处理调节参数对象
\param     objCImageDataPointer  图像数据对象
\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::ShowImageProcess(CImageProcessConfigPointer& objCfg, CImageDataPointer& objCImageDataPointer)
{
	if ((objCfg.IsNull())||(objCImageDataPointer.IsNull()))
	{
		throw std::runtime_error("NULL pointer dereferenced");
	}

	m_objLock.Lock();
	if (NULL == m_pImageBuffer)
	{
		m_objLock.Unlock();
		return;
	}
	objCfg->EnableConvertFlip(true);
	m_pProcess->ImageImprovment(objCImageDataPointer, m_pImageBuffer, objCfg);
	m_objLock.Unlock();

	__DrawImg();

}

//----------------------------------------------------------------------------------
/**
\brief     存储Bmp图像
\param     objCImageDataPointer  图像数据对象
\param     strFilePath  显示图像文件名
\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveBmp(const std::string& strFilePath) const
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
	HANDLE hFile = ::CreateFile(strFilePath.c_str(),
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
\param     objCImageDataPointer  图像数据对象
\param     strFilePath  显示图像文件名
\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveRaw(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath) const
{
	if ((objCImageDataPointer.IsNull())||(strFilePath == ""))
	{
		throw std::runtime_error("Argument is error");
	}

	DWORD   dwImageSize = (DWORD)objCImageDataPointer->GetPayloadSize();  // 写入文件的长度
	DWORD   dwBytesRead = 0;                // 文件读取的长度

	BYTE* pbuffer = (BYTE*)objCImageDataPointer->GetBuffer();
	// 创建文件
	HANDLE hFile = ::CreateFile(strFilePath.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,														
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)   // 创建失败则返回
	{
		throw std::runtime_error("Handle is invalid");
	}
	else                                 // 保存Raw图像          
	{ 
		::WriteFile(hFile, pbuffer, dwImageSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     当前像素是否为彩色
\param     objCGXDevicePointer  [in]    设备句柄
\param     bIsColorFilter       [out]   是否支持彩色

\return
*/
//----------------------------------------------------------------------------------
void CGXBitmap::IsColor(CGXDevicePointer& objCGXDevicePointer, bool &bIsColorFilter) const
{
	const std::string  strPixelFormat = objCGXDevicePointer->GetRemoteFeatureControl()
		->GetEnumFeature("PixelFormat")->GetValue().c_str();

	GX_PIXEL_FORMAT_ENTRY i32Pixel = static_cast<GX_PIXEL_FORMAT_ENTRY>(__ConvertPixelFormatToInt(strPixelFormat));

	//将图像格式和下述宏定义做按位与（&）运算，可判断像素格式是mono还是RGB
	const int32_t i32PixelMono = 0x01000000;               //判断是否为MONO格式的掩码
	const int32_t i32PixelRgb = 0x20000000;                //判断是否为RGB格式的掩码 
	const int32_t i32PixelMonoRgbCustom = 0x80000000;      //判断是否为MONO格式的掩码 
	const int32_t i32PixelColorMask = 0xFF000000;          //判断是否为彩色格式的掩码

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

	bIsColorFilter = !(bIsMono && (!bIsBayer) && (!bIsRgb));  // 用于判断是否为黑白相机
}

//----------------------------------------------------------------------------------
/**
\brief     字符型像素格式转换成整型

\return    整型像素格式
*/
//----------------------------------------------------------------------------------
int64_t CGXBitmap::__ConvertPixelFormatToInt(std::string PixelFormat) const
{
	if("Mono8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO8;
	}
	else if("BayerRG8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_RG8;
	}
	else if("BayerGB8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GB8;
	}
	else if("BayerGR8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GR8;
	}
	else if("BayerBG8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_BG8;
	}
	else if("RGB8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_RGB8;
	}
	else if("BGR8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BGR8;
	}
	else if("Mono10" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO10;
	}
	else if("BayerRG10" == PixelFormat)
	{
		return  GX_PIXEL_FORMAT_BAYER_RG10;
	}
	else if("BayerGB10" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GB10;
	}
	else if("BayerGR10" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GR10;
	}
	else if("BayerBG10" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_BG10;
	}
	else if("Mono10Packed" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO10_PACKED;
	}
	else if("Mono12" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO12;
	}
	else if("BayerRG12" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_RG12;
	}
	else if("BayerGB12" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GB12;
	}
	else if("BayerGR12" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GR12;
	}
	else if("BayerBG12" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_BG12;
	}
	else if("Mono12Packed" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO12_PACKED;
	}
	else if("Mono14" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO14;
	}
	else if("BayerRG14" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_RG14;
	}
	else if("BayerGB14" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GB14;
	}
	else if("BayerGR14" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GR14;
	}
	else if("BayerBG14" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_BG14;
	}
	else if("Mono16" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_MONO16;
	}
	else if("BayerRG16" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_RG16;
	}
	else if("BayerGB16" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GB16;
	}
	else if("BayerGR16" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_GR16;
	}
	else if("BayerBG16" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_BAYER_BG16;
	}
	else if("R8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_R8;
	}
	else if("B8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_B8;
	}
	else if("G8" == PixelFormat)
	{
		return GX_PIXEL_FORMAT_G8;
	}else
	{
		throw std::runtime_error("Format Undefined");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   为相机图像显示准备资源

\return  无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::PrepareForShowImg()
{
	// 释放旧内存, 原因是停止采集操作和回调函数是异步的
	// 若在停止采集后直接释放内存则可能因为回调函数还在运行导致程序崩溃
	// 所以改为在开始采集时释放旧内存
	UnPrepareForShowImg();

	// 更新图像的宽、高、像素格式、转换后的像素格式等
	__GetBasicAttribute();

	// 设置图像转换句柄
	__SetConvertHandle();

	if (m_bIsColor)
	{
		__ColorPrepareForShowImg();
	} 
	else
	{
		__MonoPrepareForShowImg();
	}

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
	if(NULL != m_pImageBuffer)
	{
		delete[] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief     得到整型的当前像素格式

\return    整型的当前像素格式
*/
//----------------------------------------------------------------------------------
int64_t CGXBitmap::GetCurrentPixelFormat()
{
	//获取当前像素格式
	if (0 == m_i64PixelFormat)
	{
		bool bIsLocal = m_objCGXDevicePointer->GetFeatureControl()->IsImplemented("OutPixelFormat");
		if (bIsLocal)
		{
			std::string  strPixelFormat
				= m_objCGXDevicePointer->GetFeatureControl()->GetEnumFeature("OutPixelFormat")->GetValue().c_str();
			m_i64PixelFormat = __ConvertPixelFormatToInt(strPixelFormat);
		}
		else
		{
			std::string  strPixelFormat 
				= m_objCGXDevicePointer->GetRemoteFeatureControl()->GetEnumFeature("PixelFormat")->GetValue().c_str();
			m_i64PixelFormat = __ConvertPixelFormatToInt(strPixelFormat);
		}
	}
	return m_i64PixelFormat;
}

//----------------------------------------------------------------------------------
/**
\brief     得到相机采图的宽高像素格式等属性

\return    无
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__GetBasicAttribute()
{
	//获得图像宽度、高度等
	m_i64ImageWidth = (int64_t)m_objCGXDevicePointer->GetRemoteFeatureControl()->GetIntFeature("Width")->GetValue();
	m_i64ImageHeight = (int64_t)m_objCGXDevicePointer->GetRemoteFeatureControl()->GetIntFeature("Height")->GetValue();

	//获取当前像素格式
	bool bIsLocal = m_objCGXDevicePointer->GetFeatureControl()->IsImplemented("OutPixelFormat");
	if(bIsLocal)
	{
		std::string  strPixelFormat = 
			m_objCGXDevicePointer->GetFeatureControl()->GetEnumFeature("OutPixelFormat")->GetValue().c_str();
		m_i64PixelFormat = __ConvertPixelFormatToInt(strPixelFormat);
	}else
	{
		std::string  strPixelFormat = 
			m_objCGXDevicePointer->GetRemoteFeatureControl()->GetEnumFeature("PixelFormat")->GetValue().c_str();
		m_i64PixelFormat = __ConvertPixelFormatToInt(strPixelFormat);
	}
	
	//获取当前像素格式是否为彩色
	IsColor(m_objCGXDevicePointer,m_bIsColor);

}

//----------------------------------------------------------------------------------
/**
\brief     设置图像转换句柄
\param     hConvertHandle 图像转换句柄
\param     emInputPixelForma 输入图像的像素格式

\return    true为8为数据，false为非8位数据
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__SetConvertHandle()
{
	// 设置插值方式
	m_pConvert->SetInterpolationType(GX_RAW2RGB_NEIGHBOUR);

	// 获取有效位数
	GX_VALID_BIT_LIST emValidBits = GetBestValudBit(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	// 设置有效位数
	m_pConvert->SetValidBits(emValidBits);

	if (m_bIsColor)
	{
		// 设置图像格式转换句柄，转换为BGR8格式
		m_pConvert->SetDstFormat(GX_PIXEL_FORMAT_BGR8);
		m_i64ConvertSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight, GX_PIXEL_FORMAT_BGR8);
	}
	else
	{
		// 设置图像格式转换句柄，转换为RGB8格式
		m_pConvert->SetDstFormat(GX_PIXEL_FORMAT_MONO8);
		m_i64ConvertSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight, GX_PIXEL_FORMAT_MONO8);
	}

	//计算转换前后图像的大小
	m_i64RawSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight
		, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));
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
		|| (NULL == pRawImageBuffer))
	{
		m_objLock.Unlock();
		return;
	}
	BYTE* pRawBuffer = reinterpret_cast<BYTE*>(pRawImageBuffer);
	if (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat)
	{
		// RGB格式需要翻转数据后显示
		for (int32_t i32Index = 0; i32Index < m_i64ImageHeight; ++i32Index)
		{
			memcpy(m_pImageBuffer + i32Index * m_i64ImageWidth * PIXEL
				, pRawBuffer + (m_i64ImageHeight - i32Index - 1) * m_i64ImageWidth * PIXEL, (size_t)m_i64ImageWidth * PIXEL);
		}
	}
	else if ((GX_PIXEL_FORMAT_R8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_G8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_B8 == m_i64PixelFormat)
		|| (GX_PIXEL_FORMAT_MONO8 == m_i64PixelFormat))
	{
		// 黑白相机需要翻转数据后显示
		for (int32_t i32Index = 0; i32Index < m_i64ImageHeight; ++i32Index)
		{
			memcpy(m_pImageBuffer + i32Index * m_i64ImageWidth
				, pRawBuffer + (m_i64ImageHeight - i32Index - 1) * m_i64ImageWidth, (size_t)m_i64ImageWidth);
		}
	}
	else 
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
bool CGXBitmap::__NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat)const
{
	if ((GX_PIXEL_FORMAT_BGR8 == emPixelFormat)
		||(GX_PIXEL_FORMAT_R8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_G8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_B8 == emPixelFormat)
		|| (GX_PIXEL_FORMAT_MONO8 == emPixelFormat))
	{
		return true;
	}
	return false;
}
