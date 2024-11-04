//------------------------------------------------------------------------
/**
\file		GXBitmap.h
\brief		此类主要用于图像的显示和存储，图像显示和存储可以自适应黑白彩色相机，
图像存储可以存储为Bmp、Raw，对图像显示和存储进行了声明
\Date       2024-03-07
\Version    1.1.2403.9071
*/
//------------------------------------------------------------------------
#pragma once

#include "GalaxyIncludes.h"
#include "GXIAPIBase.h"
#include "IGXImageProcess.h"

#define PIXEL 3

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


class CGXBitmap
{
public:
	///构造函数
	CGXBitmap(CGXDevicePointer& objCGXDevicePointer,CWnd* pWnd);

	///析构函数
	~CGXBitmap(void);

	 ///显示图像及帧率
	 void Show(CImageDataPointer& objCImageDataPointer,char* pDeviceSNFPS = NULL);

	 ///图像处理后并显示图像
	 void ShowImageProcess(CImageProcessConfigPointer& objCfg, CImageDataPointer& objCImageDataPointer);

	 /// 存储Bmp图像
	 void SaveBmp(const std::string& strFilePath) const;

	 /// 存储Raw图像
	 void SaveRaw(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath) const;

	 ///通过GX_PIXEL_FORMAT_ENTRY获取最优Bit位
	 GX_VALID_BIT_LIST GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry) const;

	 /// 当前像素是否为彩色
	 void IsColor(CGXDevicePointer& objCGXDevicePointer, bool &bIsColorFilter) const;

	 ///为相机图像显示准备资源
	 void PrepareForShowImg();

	 /// 释放为图像显示准备的资源
	 void UnPrepareForShowImg();

	 /// 得到当前的像素格式
	 int64_t GetCurrentPixelFormat();


private:

	///得到相机的基础属性
	void __GetBasicAttribute();

	///设置转换句柄
	void __SetConvertHandle();

	///为彩色相机图像显示准备资源
	void __ColorPrepareForShowImg();

	///为黑白相机图像显示准备资源
	void __MonoPrepareForShowImg();

	///将m_pBufferRGB中图像和帧率显示到界面
	void __DrawImg(char* strDeviceSNFPS = NULL);

	///计算宽度所占的字节数
	int64_t __GetStride(int64_t i64Width, bool bIsColor) const;

	/// 转换字符型像素格式为整形
	int64_t __ConvertPixelFormatToInt(std::string PixelFormat) const;

	///翻转图像buffer
	void __FilpImage(void* pRawImageBuffer);
	/// 判断图像显示前是否需要翻转
	bool __NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat)const;

private:

	CWnd*                         m_pWnd;                           ///<显示图像窗口(控件)指针
	bool                          m_bIsColor ;                      ///<是否是彩色像素
	int64_t                       m_i64ImageHeight;                 ///<原始图像高
	int64_t                       m_i64ImageWidth;                  ///<原始图像宽
	int64_t                       m_i64PixelFormat;                 ///< 当前的像素格式
	int64_t                       m_i64RawSize;                     ///< 设备输出原始图像大小
	int64_t                       m_i64ConvertSize;                 ///< 转换后图像大小
	BITMAPINFO                   *m_pBmpInfo;	                    ///<BITMAPINFO 结构指针，显示图像时使用
	char                          m_chBmpBuf[2048];	                ///<BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	HDC                           m_hDC;                            ///<绘制图像DC句柄
	BYTE                         *m_pImageBuffer;                   ///<保存翻转后的图像用于显示
	CGXDevicePointer              m_objCGXDevicePointer;            ///< 设备指针

	CGXImageFormatConvertPointer  m_pConvert;                       ///< 像素格式转换指针
	CGXImageProcessPointer        m_pProcess;                       ///< 图像质量提升指针
	CLock                         m_objLock;                        ///< 锁，防止回调仍在进行时析构资源造
private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

