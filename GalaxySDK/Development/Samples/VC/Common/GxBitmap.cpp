//------------------------------------------------------------------------
/**
\file		GXBitmap.cpp
\brief		������Ҫ����ͼ�����ʾ�ʹ洢��ͼ����ʾ�ʹ洢��������Ӧ�ڰײ�ɫ�����
ͼ��洢���Դ洢ΪBmp��Raw����ͼ����ʾ�ʹ洢������ʵ��

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

///< �жϷ���ֵ�궨��
#define BITMAP_VERIFY_STATUS(emStatus) \
 if (GX_STATUS_SUCCESS != emStatus) \
                                   {\
	                                break;\
                                   }\
//---------------------------------------------------------------------------------
/**
\brief   ���캯��
\param   hDevice ͼ���豸ָ��
\param   pWnd ����ָ��
\return  ��
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

	// �����ͼ���
	m_hDC = ::GetDC(m_pWnd->m_hWnd);

}


//---------------------------------------------------------------------------------
/**
\brief   ���캯��
\param   hDevice ͼ���豸ָ��
\param   pWnd ����ָ��
\return  ��
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

	// ����ͼ���ʽת�����
	emStatus = DxImageFormatConvertCreate(&m_hConvert);
	if (GX_STATUS_SUCCESS != emStatus)
	{
		throw std::invalid_argument("Initialization Failed!");
	}

	// �����ͼ���
	m_hDC = ::GetDC(*m_pHwnd);
}

//---------------------------------------------------------------------------------
/**
\brief   ��������

\return  ��
*/
//----------------------------------------------------------------------------------
CGXBitmap::~CGXBitmap(void)
{
	GX_STATUS     emStatus = GX_STATUS_ERROR;

	m_objLock.Lock();
	 //���Ϊͼ����ʾ׼������Դ
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

	// �ر�ͼ���ʽת�����
	if (NULL != m_hConvert)
	{
		DxImageFormatConvertDestroy(m_hConvert);
		m_hConvert = NULL;
	}


	// �رջ�ͼ���
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
\brief   Ϊ���ͼ����ʾ׼����Դ

\return  ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::PrepareForShowImg()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// �ͷž��ڴ�, ԭ����ֹͣ�ɼ������ͻص��������첽��
	// ����ֹͣ�ɼ���ֱ���ͷ��ڴ��������Ϊ�ص������������е��³������
	// ���Ը�Ϊ�ڿ�ʼ�ɼ�ʱ�ͷž��ڴ�
	UnPrepareForShowImg();

	// ����ͼ��Ŀ��ߡ����ظ�ʽ��ת��������ظ�ʽ��
	__GetBasicAttribute();

	// ����ͼ��ת�����
	__SetConvertHandle(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	if (m_bIsColor)
	{
		// Ϊ��ɫͼ����ʾ׼����Դ,����Buffer
		emStatus = __ColorPrepareForShowImg();
		VERIFY_STATUS_RET(emStatus);
	}
	else
	{
		// Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer
		emStatus = __MonoPrepareForShowImg();
		VERIFY_STATUS_RET(emStatus);

	}

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief     �ͷ�Ϊͼ����ʾ׼������Դ

\return    ��
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
\brief     ��ʾͼ��
\param     pCallbackFrame  �ص��ɼ���������
\param     pDeviceSNFPS    ֡��

\return    ��
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
\brief     ��ʾͼ��
\param     pFrame          ��֡�ɼ���������
\param     pDeviceSNFPS    ֡��

\return    ��
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
\brief     ����ͼ���ۺϴ�����������ͼ�����ݺ���ʾ
\param     pCallbackFrame  �ص��ɼ���������
\param     pCallbackFrame  ͼ���������ýṹ���ָ��
\param     pDeviceSNFPS    ֡��

\return    ��
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
\brief     ����ͼ���ۺϴ�����������ͼ�����ݺ���ʾ
\param     pFrame          ��֡�ɼ���������
\param     pCallbackFrame  ͼ���������ýṹ���ָ��
\param     pDeviceSNFPS    ֡��

\return    ��
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
\brief   ��m_pBufferRGB��ͼ����ʾ������
\param   strDeviceSNFPS  �豸֡�����к�

\return  ��
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

	//// Ϊ��ͼ��׼��
	RECT objRect;
	(NULL != m_pWnd) ? m_pWnd->GetClientRect(&objRect) : GetClientRect(*m_pHwnd, &objRect);
	i32WndWidth  = objRect.right - objRect.left;
	i32WndHeight = objRect.bottom - objRect.top;

	HDC      objMemDC = ::CreateCompatibleDC(m_hDC);
	HBITMAP  objMemBmp= CreateCompatibleBitmap(m_hDC, i32WndWidth, i32WndHeight);
	::SelectObject(objMemDC,objMemBmp);

	// ������ø���䣬����ͼ�����ˮ��
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
\brief     ��תͼ��buffer

\return    ��
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
		// BGR��ʽ��Ҫ��ת���ݺ���ʾ
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
		// �ڰ������Ҫ��ת���ݺ���ʾ
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
\brief     �ж�ͼ���Ƿ���Ҫ��ת

\return    true��Ҫ��ת��false����Ҫ��ת
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
\brief  Ϊ��ɫͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__ColorPrepareForShowImg()
{
	const int32_t BITCOUNT = 24;
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_i64ImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = BITCOUNT; // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	//----------------------------Ϊͼ�����ݷ���Buffer---------------------

	// Ϊ����RGBת�����ͼ�����Buffer
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
\brief Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS  CGXBitmap::__MonoPrepareForShowImg()
{
	const int32_t BITCOUNT = 8;
//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_i64ImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = BITCOUNT;  // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColor)
	{
		const int32_t PALETTE_SIZE = 256;
		// �ڰ������Ҫ���г�ʼ����ɫ�����
		for (int32_t i = 0; i < PALETTE_SIZE; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//----------------------------Ϊͼ�����ݷ���Buffer---------------------
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
\brief     ����ͼ������������������ͼ������
\param     pCallbackFrame      �ص��ɼ���������
\param     i64ColorCorrection  ��ɫУ��ֵ
\param     pContrastLut        �ԱȶȲ��ұ�
\param     pGammaLut           Gamma���ұ�
\param     emChannelOrder      ���ͼ��� RGB ͨ��˳�� 
\param     pDeviceSNFPS        ֡��

\return    ��
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
\brief     ����ͼ������������������ͼ������
\param     pFrame              ��֡�ɼ���������
\param     i64ColorCorrection  ��ɫУ��ֵ
\param     pContrastLut        �ԱȶȲ��ұ�
\param     pGammaLut           Gamma���ұ�
\param     pDeviceSNFPS        ֡��

\return    ��
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
\brief     �õ������ͼ�Ŀ�����ظ�ʽ������

\return    ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__GetBasicAttribute()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	GX_INT_VALUE stIntValue;

	// ��ȡ���
	emStatus = GXGetIntValue(m_hDevice, "Width", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageWidth = stIntValue.nCurValue;

	// ��ȡ�߶�
	emStatus = GXGetIntValue(m_hDevice, "Height", &stIntValue);
	VERIFY_STATUS_RET(emStatus);
	m_i64ImageHeight = stIntValue.nCurValue;

	// ��ȡ���ؾ��
	GX_LOCAL_DEV_HANDLE hLocalDev = NULL;
	emStatus = GXGetLocalDeviceHandleFromDev(m_hDevice, &hLocalDev);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ������ظ�ʽ
	GX_ENUM_VALUE stEnumValue;
	emStatus = GXGetEnumValue(hLocalDev, "OutPixelFormat", &stEnumValue);
	if (GX_STATUS_SUCCESS == emStatus) 
	{
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else if (GX_STATUS_NOT_IMPLEMENTED == emStatus) 
	{
		// ��ȡ��ǰ���ظ�ʽ
		emStatus = GXGetEnumValue(m_hDevice, "PixelFormat", &stEnumValue);
		VERIFY_STATUS_RET(emStatus);
		m_i64PixelFormat = stEnumValue.stCurValue.nCurValue;
	}
	else 
	{
		return GX_STATUS_ERROR;
	}

	m_bIsColor = IsColor(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	//����ԭʼͼ��buffer�Ĵ�С
	emStatus = DxImageFormatConvertGetBufferSizeForConversion(m_hConvert
		, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat), static_cast<VxUint32>(m_i64ImageWidth)
		, static_cast<VxUint32>(m_i64ImageHeight), reinterpret_cast<int*>(&m_i32RawSize) );
	VERIFY_STATUS_RET(emStatus);

	//����ת����ͼ��buffer�Ĵ�С
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
\brief     ������ʾͼ��
\param     pRawImgBuff   ͼ��buffer
\param     pDeviceSNFPS  ֡�ʣ�Ĭ��Ϊ�գ�������ʾ

\return    ��
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

	// ����Ļ�ϻ�ͼ
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     ����ͼ���ۺϴ������ʾͼ��
\param     pRawImgBuff         ԭʼͼ������
\param     pImageProcess       ͼ���������ýṹ���ָ��
\param     pDeviceSNFPS        ֡�ʣ�Ĭ��Ϊ�գ�������ʾ

\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ShowAfterProcess(void * pRawImgBuff,  void * pImageProcess, char * pDeviceSNFPS)
{
	// ͼ����
	__ImageProcess(m_bIsColor, pRawImgBuff, pImageProcess);

	// ����Ļ�ϻ�ͼ
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     ����ͼ���ۺϴ������ʾͼ��
\param     pRawImgBuff         ͼ������
\param     i64ColorCorrection  ��ɫУ��ֵ
\param     pContrastLut        �ԱȶȲ��ұ�
\param     pGammaLut           Gamma���ұ�
\param     pDeviceSNFPS        ֡�ʣ�Ĭ��Ϊ�գ�������ʾ

\return    ��
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

		//����ͼ������
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
		return; //�����쳣���򲻻�ͼ��ֱ�ӷ���
	}

	// ����Ļ�ϻ�ͼ
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     ����ͼ���ۺϴ���������ͼ������
\param     bIsColor            ��ǰ�����Ƿ�Ϊ��ɫ��true��ɫ��false �ڰ�
\param     pImageBuffer        ͼ��buffer
\param     pImageProcess       ͼ���������ýṹ���ָ��

\return    ��
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

		//����ͼ��ת�����
		emDxStatus = (DX_STATUS)DxImageFormatConvertCreate(&pImgFormatConvert);
		DX_VERIFY(emDxStatus);

		//Ϊת�����ͼ��Buffer�����ڴ�
		pImageRGBBuf = new(std::nothrow) BYTE[static_cast<uint32_t>(m_i64ImageWidth * m_i64ImageHeight)];
		if (NULL == pImageRGBBuf)
		{
			break;
		}

		// �ж��Ƿ�Ϊ8λͼ�����ǰ�λͼ��תΪ8λͼ
		bool bIsRaw8 = __IsPixelFormat8(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));
		if (!bIsRaw8)
		{
			// ���ΪMONO10_PACKED��MONO12_PACKED���ظ�ʽ���Ƚ���ת��ΪMono8
			if ((GX_PIXEL_FORMAT_MONO10_PACKED == static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat))
				|| (GX_PIXEL_FORMAT_MONO12_PACKED == static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat)))
			{
				// ������Чλ
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetValidBits(pImgFormatConvert, GetBestValudBit((GX_PIXEL_FORMAT_ENTRY)m_i64PixelFormat));
				DX_VERIFY(emDxStatus);

				// ����Ŀ�����ظ�ʽΪMono8
				emDxStatus = (DX_STATUS)DxImageFormatConvertSetOutputPixelFormat(pImgFormatConvert, GX_PIXEL_FORMAT_MONO8);
				DX_VERIFY(emDxStatus);

				// ��ȡԴBuffer��С
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pImgFormatConvert, (GX_PIXEL_FORMAT_ENTRY)m_i64PixelFormat
					, (VxUint32)m_i64ImageWidth, (VxUint32)m_i64ImageHeight, &nSrcBufferSize);
				DX_VERIFY(emDxStatus);

				// ��ȡĿ��Buffer��С
				emDxStatus = (DX_STATUS)DxImageFormatConvertGetBufferSizeForConversion(pImgFormatConvert, GX_PIXEL_FORMAT_MONO8
					, (VxUint32)m_i64ImageWidth, (VxUint32)m_i64ImageHeight, &nDstBufferSize);
				DX_VERIFY(emDxStatus);

				// ���ظ�ʽת��
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

		if (!bIsColor)//�ڰ����
		{
			MONO_IMG_PROCESS* pMono = reinterpret_cast<MONO_IMG_PROCESS*>(pImageProcess);
			emStatus = DxMono8ImgProcess(m_pBufferRaw16To8, m_pBufferMono, static_cast<VxUint32>(m_i64ImageWidth)
				, static_cast<VxUint32>(m_i64ImageHeight), pMono);
			DX_VERIFY(emDxStatus);

			// �ڰ������Ҫ��ת���ݺ���ʾ
			for (int32_t i = 0; i < m_i64ImageHeight; ++i)
			{
				memcpy(m_pImageBuffer + i * m_i64ImageWidth, m_pBufferMono + (m_i64ImageHeight - i - 1) * m_i64ImageWidth
					, (size_t)m_i64ImageWidth);
			}
		}
		else//��ɫ���
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

	// ����ͼ��ת�����
	DxImageFormatConvertDestroy(pImgFormatConvert);
}

//----------------------------------------------------------------------------------
/**
\brief     ��������ռ���ֽ���
\param     nWidth  ͼ����
\param     bIsColor  �Ƿ��ǲ�ɫ���
\return    ͼ��һ����ռ���ֽ���
*/
//----------------------------------------------------------------------------------
int64_t CGXBitmap::__GetStride(int64_t i64Width, bool bIsColor) const
{
	return bIsColor ? (i64Width * PIXEL) : i64Width;
}
//----------------------------------------------------------------------------------
/**
\brief     �洢Bmpͼ��
\param     pFilePath  ͼ��洢��·��

\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveBmp(const char* pFilePath)
{
	const uint32_t FILE_TYPE = 8;

	DWORD		         dwImageSize = static_cast<DWORD>(__GetStride(m_i64ImageWidth, m_bIsColor) * m_i64ImageHeight);
	BITMAPFILEHEADER     stBfh	     = {0};
	DWORD		         dwBytesRead = 0;

	stBfh.bfType	= (WORD)'M' << FILE_TYPE | 'B';			 //�����ļ�����
	stBfh.bfOffBits = m_bIsColor ?sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		:sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * 4);	//�����ļ�ͷ��СtrueΪ��ɫ,falseΪ�ڰ�
	stBfh.bfSize	= stBfh.bfOffBits + dwImageSize; //�ļ���С

	DWORD dwBitmapInfoHeader = m_bIsColor ?sizeof(BITMAPINFOHEADER)
		:sizeof(BITMAPINFOHEADER) + (256 * 4);	//����BitmapInfoHeader��СtrueΪ��ɫ,falseΪ�ڰ�

	//�����ļ�
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
	::WriteFile(hFile, m_pBmpInfo, dwBitmapInfoHeader, &dwBytesRead, NULL); //�ڰ׺Ͳ�ɫ����Ӧ
	::WriteFile(hFile, m_pImageBuffer, dwImageSize, &dwBytesRead, NULL);

	CloseHandle(hFile);
}

//----------------------------------------------------------------------------------
/**
\brief     �洢Rawͼ��
\param     pFilePath  �洢��·��+��ʾͼ���ļ���

\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveRaw(void* pRawBuffer,const char* pFilePath)
{
	DWORD   bytes_read = 0;                // �ļ���ȡ�ĳ���

	// �����ļ�
	HANDLE hFile = ::CreateFile(pFilePath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)   // ����ʧ���򷵻�
	{
		throw std::invalid_argument("Invalid Device Handle!");
	}
	else                                 // ����Rawͼ��          
	{
		::WriteFile(hFile, pRawBuffer, static_cast<DWORD>(strlen((char*)pRawBuffer)), &static_cast<DWORD>(bytes_read), NULL);
		CloseHandle(hFile);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     ��ʼ���洢AVI����Ҫ����Դ������
\param    m_pBmpInfo     
\param    m_i64ImageWidth   �洢��Ƶ��֡��
\param    m_i64ImageHeight   ͼ��Ŀ�
\param    strStorePath       �洢��·��
\param    ui64PlayFPS        �洢��Ƶ��֡��

\return    ��
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
\brief     ����ʼ���洢AVI����Ҫ����Դ������

\return    ��
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
\brief     �洢AVI�ļ�
\param    pBuffer  ���洢��ͼ��buffer

\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveAVI(BYTE *pBuffer)
{

	// ����AVI��Ƶ
	if (NULL == pBuffer) 
	{
		m_pSaveAVI->SaveAVI(m_pImageBuffer);
	}
	else 
	{
		m_pSaveAVI->SaveAVI(pBuffer);
	}
	
	//������Ҫ�����̿ռ���AVI�ļ��Ĵ洢��С��������,�ɵ���ExSaveAVIʵ����Ƶ����
	//m_objSaveAVI->ExSaveAVI(m_pImageBuffer); 
}

//----------------------------------------------------------------------------------
/**
\brief     �ж����������Ƿ�Ϊ��ɫ���ظ�ʽ
\param    bIsColorFilter  ���жϵ����ظ�ʽ

\return    true ��ǰΪ��ɫ���أ�false���ǲ�ɫ����
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::IsColor(GX_PIXEL_FORMAT_ENTRY i32Pixel)
{
	//��ͼ���ʽ�������궨������λ�루&�����㣬���ж����ظ�ʽ��mono����RGB
	int32_t i32PixelMono = 0x01000000;               //�ж��Ƿ�ΪMONO��ʽ������
	int32_t i32PixelRgb = 0x20000000;                //�ж��Ƿ�ΪRGB��ʽ������ 
	int32_t i32PixelMonoRgbCustom = 0x80000000;      //�ж��Ƿ�ΪMONO��ʽ������ 
	int32_t i32PixelColorMask = 0xFF000000;          //�ж��Ƿ�ΪMONO��ʽ������

	//��ͼ���ʽ�������궨������λ�루&�����㣬�ɵõ����ظ�ʽ��ID
	int32_t i32PixelIdMask = 0x0000FFFF;

	bool bIsMono = ((i32PixelColorMask & i32Pixel) == i32PixelMono); // �Ƿ�Ϊmono��ʽ

	bool bIsRgb = ((i32PixelColorMask & i32Pixel) == i32PixelRgb);  // �Ƿ�ΪRGB��ʽ           
	bool bIsBayer = (((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR8) <= (i32PixelIdMask & i32Pixel)
		&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG12))
		|| ((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR14) <= (i32PixelIdMask & i32Pixel)
			&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG14))
		|| ((i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_GR16) <= (i32PixelIdMask & i32Pixel)
			&& (i32PixelIdMask & i32Pixel) <= (i32PixelIdMask & GX_PIXEL_FORMAT_BAYER_BG16)));   // �Ƿ�ΪBayer��ʽ

	bool bIsColor = !(bIsMono && (!bIsBayer) && (!bIsRgb));  // �����ж��Ƿ�Ϊ�ڰ����

	return bIsColor;
}

//----------------------------------------------------------------------------------
/**
\brief     ѡ����Ƶ������

\return    ��
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
\brief     ͨ��GX_PIXEL_FORMAT_ENTRY��ȡ����Bitλ
\param     emPixelFormatEntry ͼ�����ݸ�ʽ
\return    ����Bitλ
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
		//��ʱû�����������ݸ�ʽ������
		break;
	}
	case GX_PIXEL_FORMAT_MONO16:
	case GX_PIXEL_FORMAT_BAYER_GR16:
	case GX_PIXEL_FORMAT_BAYER_RG16:
	case GX_PIXEL_FORMAT_BAYER_GB16:
	case GX_PIXEL_FORMAT_BAYER_BG16:
	{
		//��ʱû�����������ݸ�ʽ������
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
\brief     ����ͼ��ת�����
\param     hConvertHandle ͼ��ת�����
\param     emInputPixelForma ����ͼ������ظ�ʽ

\return    DX_OK �ɹ�������ʧ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CGXBitmap::__SetConvertHandle(GX_PIXEL_FORMAT_ENTRY emInputPixelFormat)
{
	VxInt32 emStatus = DX_OK;

	//��ɫתBGR8,�ڰ�תMONO8
	if (m_bIsColor) 
	{
		// ����ͼ���ʽת�������ת��ΪBGR8��ʽ
		emStatus = DxImageFormatConvertSetOutputPixelFormat(m_hConvert, GX_PIXEL_FORMAT_BGR8);
		VERIFY_STATUS_RET(emStatus);
	}
	else 
	{
		// ����ͼ���ʽת�������ת��ΪMono8��ʽ
		emStatus = DxImageFormatConvertSetOutputPixelFormat(m_hConvert, GX_PIXEL_FORMAT_MONO8);
		VERIFY_STATUS_RET(emStatus)
	}

	// ���ò�ֵ��ʽ
	emStatus = DxImageFormatConvertSetInterpolationType(m_hConvert, m_emConvertType);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ��Чλ��
	DX_VALID_BIT emValidBits = GetBestValudBit(emInputPixelFormat);

	// ������Чλ��
	emStatus = DxImageFormatConvertSetValidBits(m_hConvert, emValidBits);
	VERIFY_STATUS_RET(emStatus);

	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief     �ж�PixelFormat�Ƿ�Ϊ8λ
\param     emPixelFormatEntry ͼ�����ݸ�ʽ
\return    trueΪ8Ϊ���ݣ�falseΪ��8λ����
*/
//----------------------------------------------------------------------------------
bool CGXBitmap::__IsPixelFormat8(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	bool bIsPixelFormat8 = false;
	const unsigned  PIXEL_FORMATE_BIT = 0x00FF0000;  ///<�����뵱ǰ�����ݸ�ʽ����������õ���ǰ������λ��
	unsigned uiPixelFormatEntry = (unsigned)emPixelFormatEntry;
	if ((uiPixelFormatEntry & PIXEL_FORMATE_BIT) == GX_PIXEL_8BIT)
	{
		bIsPixelFormat8 = true;
	}
	return bIsPixelFormat8;
}

//----------------------------------------------------------------------------------
/**
\brief     ���ò�ֵ����
\param     emConvertType ��ֵ����

\return    DX_OK �ɹ�������ʧ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SetInterpolationAlgorithm(DX_BAYER_CONVERT_TYPE emConvertType)
{
	// ���ò�ֵ��ʽ
	m_emConvertType = emConvertType;
	DxImageFormatConvertSetInterpolationType(m_hConvert, emConvertType);
}
