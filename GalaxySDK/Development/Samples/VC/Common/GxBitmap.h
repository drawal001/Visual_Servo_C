//------------------------------------------------------------------------
/**
\file		GXBitmap.h
\brief		此类主要用于图像的显示和存储，图像显示和存储可以自适应黑白彩色相机，
图像存储可以存储为Bmp、Raw，对图像显示和存储进行了声明

<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>

\Date       2024-05-27
\Version    1.1.2405.9271
*/
//------------------------------------------------------------------------
#pragma once 
#pragma warning (disable: 4996)
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "GxPixelFormat.h"
#include "StoreAVI.h"
#include<stdio.h>

#define PIXEL 3

///< 判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
 if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
	                                return emStatus;\
                                   }\

#define  GX_VERIFY(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS)\
                                     {\
	                                  return ;\
                                     } 
#define  GX_VERIFY_BREAK(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS)\
                                     {\
	                                  break ;\
                                     }

#define  DX_VERIFY(emStatus) \
	if (emStatus != DX_OK)\
                                     {\
	                                  break ;\
                                     } 
#define  DX_VERIFY_RETURN(emStatus) \
	if (emStatus != DX_OK)\
                                     {\
	                                 return ;\
                                     } 

//--------------------------------------------
/**
\brief 图像显示、存储类
*/
//---------------------------------------------
class CGXBitmap
{
public:
	///构造函数，使用MFC窗口句柄
	CGXBitmap(GX_DEV_HANDLE& hDevice, CWnd* pWnd);
	///构造函数，使用QT窗口句柄
	CGXBitmap(GX_DEV_HANDLE& hDevice, HWND* pHwnd);
	///析构函数
	~CGXBitmap(void);

	///为相机图像显示准备资源
	GX_STATUS PrepareForShowImg();
	/// 释放为图像显示准备的资源
	void UnPrepareForShowImg();
	
	///显示图像
	void Show(GX_FRAME_CALLBACK_PARAM* pCallbackFrame, char* pDeviceSNFPS = NULL);
	void Show(GX_FRAME_DATA* pFrame, char* pDeviceSNFPS = NULL);

	/// 图像综合处理后显示图像
	void ShowAfterProcess(GX_FRAME_CALLBACK_PARAM* pCallbackFrame ,void * pImageProcess,char* pDeviceSNFPS = NULL);
	void ShowAfterProcess(GX_FRAME_DATA* pFrame,void * pImageProcess, char* pDeviceSNFPS = NULL);

	///图像质量提升后显示图像
	void ShowAfterImprovment(GX_FRAME_CALLBACK_PARAM* pCallbackFrame,int64_t i64ColorCorrection, BYTE *pContrastLut
		, BYTE *pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder = DX_ORDER_RGB, char* pDeviceSNFPS = NULL);
	void ShowAfterImprovment(GX_FRAME_DATA* pFrame, int64_t i64ColorCorrection
		, BYTE *pContrastLut, BYTE *pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder = DX_ORDER_RGB, char* pDeviceSNFPS = NULL);

	 /// 存储Bmp图像
	 void SaveBmp(const char* pFilePath);
	 /// 存储Raw图像
	 void SaveRaw(void* pRawBuffer, const char* pFilePath);

	 /// 初始化存储AVI所需要的资源和配置
	 bool InitAVI(char* strStorePath = ".", uint64_t ui64PlayFPS = 15);
	 /// 反初始化存储AVI所需要的资源和配置
	 void UnInitAVI();
	 /// 选择视频编码器
	 bool SelectCompressor();
	 /// 存储AVI
	 void SaveAVI(BYTE *pBuffer = NULL);

	 /// 是否为彩色像素格式
	 bool IsColor(GX_PIXEL_FORMAT_ENTRY i32Pixel);

	 ///通过GX_PIXEL_FORMAT_ENTRY获取最优Bit位
	 DX_VALID_BIT GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);

	 /// 设置插值方法
	 void SetInterpolationAlgorithm(DX_BAYER_CONVERT_TYPE emConvertType);


private:
	///得到相机的基础属性
	GX_STATUS __GetBasicAttribute();

	///显示图像
	void __Show(void* pRawImgBuff, char* pDeviceSNFPS = NULL);

	/// 图像综合处理后显示
	void __ShowAfterProcess(void* pRawImgBuff, void * pImageProcess, char* pDeviceSNFPS = NULL);

	/// 图像质量提升后显示
	void __ShowAfterImprovment(void* pRawImgBuff, int64_t i64ColorCorrection, BYTE *pContrastLut, BYTE *pGammaLut
		, DX_RGB_CHANNEL_ORDER emChannelOrder, char* pDeviceSNFPS = NULL);

	/// 使用图像综合处理函数提升图像质量
	void __ImageProcess(bool bIsColor, void* pImageBuffer, void * pImageProcess);

	///设置转换句柄
	GX_STATUS __SetConvertHandle(GX_PIXEL_FORMAT_ENTRY emInputPixelFormat);	

	/// 判断PixelFormat是否为8位
	bool __IsPixelFormat8(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);

	///将m_pBufferRGB中图像和帧率显示到界面
	void __DrawImg(char* strDeviceSNFPS = NULL);

	///翻转图像buffer
	void __FilpImage(void* pRawImageBuffer);
	/// 判断图像显示前是否需要翻转
	bool __NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat) const;

	///为彩色相机图像显示准备资源
	GX_STATUS __ColorPrepareForShowImg();
	///为黑白相机图像显示准备资源
	GX_STATUS __MonoPrepareForShowImg();

	///计算宽度所占的字节数
	int64_t __GetStride(int64_t i64Width, bool bIsColor) const;

	char                            m_chBmpBuf[2048];	             ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	bool							m_bIsColor;						 ///< 当前是否是彩色像素
	BYTE                           *m_pImageBuffer;                  ///< 保存处理后的图像buffer用于显示
	BYTE                           *m_pBufferMono;                   ///< Mono图像数据，用于黑白图像处理完成后翻转
	BYTE                           *m_pBufferRaw16To8;               ///< 非8位图转成8位后的buffer
	int64_t                         m_i64ImageHeight;                ///< 原始图像高
	int64_t                         m_i64ImageWidth;                 ///< 原始图像宽
	int64_t                         m_i64PixelFormat;                ///< 当前的像素格式
	int32_t                         m_i32RawSize;                    ///< 设备输出原始图像大小
	int32_t                         m_i32ConvertSize;                ///< 转换后图像大小
	BITMAPINFO                     *m_pBmpInfo;	                     ///< BITMAPINFO 结构指针，显示图像时使用

	HDC                             m_hDC;                           ///< 绘制图像DC句柄
	CWnd                           *m_pWnd;                          ///< 显示图像窗口(控件)指针
	HWND                           *m_pHwnd;                         ///< 显示图像的窗口(控件)指针
	DX_IMAGE_FORMAT_CONVERT_HANDLE  m_hConvert;                      ///< 像素格式转换句柄
	GX_DEV_HANDLE                   m_hDevice;                       ///< 设备句柄

	CStoreAVI                      *m_pSaveAVI;                      ///< 存储视频类
	CLock                           m_objLock;                       ///< 锁，防止资源释放时回调仍在进行
	DX_BAYER_CONVERT_TYPE           m_emConvertType;                 ///< 插值算法

private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

