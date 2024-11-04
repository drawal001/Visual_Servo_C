//------------------------------------------------------------------------
/**
\file		GXBitmap.cpp
\brief		������Ҫ����ͼ�����ʾ�ʹ洢��ͼ����ʾ�ʹ洢��������Ӧ�ڰײ�ɫ�����
ͼ��洢���Դ洢ΪBmp��Raw����ͼ����ʾ�ʹ洢������ʵ��

\Date       2024-03-07
\Version    1.1.2403.9071
*/
//------------------------------------------------------------------------
#include "stdafx.h"
#include "GXBitmap.h"


//---------------------------------------------------------------------------------
//**
//\brief   ���캯��
//\param   objCGXDevicePointer ͼ���豸ָ��
//\param   pWnd ����ָ��
//\return  ��
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

	//��ʼ��ͼ��ת������
	m_pConvert = IGXFactory::GetInstance().CreateImageFormatConvert();

	//��ʼ��ͼ��������������
	m_pProcess = IGXFactory::GetInstance().CreateImageProcess();

}


//---------------------------------------------------------------------------------
/**
\brief   ��������

\return  ��
*/
//----------------------------------------------------------------------------------
CGXBitmap::~CGXBitmap(void)
{
	m_objLock.Lock();
	//�ͷ�pDC
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
\brief     ͨ��GX_PIXEL_FORMAT_ENTRY��ȡ����Bitλ
\param     emPixelFormatEntry ͼ�����ݸ�ʽ
\return    ����Bitλ
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
			//����Ĭ��ֵGX_BIT_0_7
			break;
	}
	return emValidBits;
}

//---------------------------------------------------------------------------------
/**
\brief   Ϊ��ɫ���ͼ����ʾ׼����Դ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__ColorPrepareForShowImg()
{
	const int32_t BIT_COUNT = 24;
	//---------------------------��ʼ��bitmapͷ---------------------------
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

	//Ϊ������ת���ͼ�����ݷ���ռ�
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight * PIXEL)];
	if (NULL == m_pImageBuffer)
	{
		throw std::runtime_error("Fail to allocate memory");
	}
}

//---------------------------------------------------------------------------------
/**
\brief   Ϊ�ڰ����ͼ����ʾ׼����Դ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__MonoPrepareForShowImg()
{
	const int32_t BIT_COUNT = 8;
	//----------------------��ʼ��bitmapͷ---------------------------------
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= (LONG)m_i64ImageWidth;
	m_pBmpInfo->bmiHeader.biHeight			= (LONG)m_i64ImageHeight;	

	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= BIT_COUNT; // �ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	// �ڰ�ͼ����Ҫ��ʼ����ɫ��
	const int32_t PALETTE = 256;
	for(int32_t i32Index=0; i32Index<PALETTE; ++i32Index)
	{
		m_pBmpInfo->bmiColors[i32Index].rgbBlue	     = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbGreen	 = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbRed		 = i32Index;
		m_pBmpInfo->bmiColors[i32Index].rgbReserved =0;
	}

	//Ϊ������ת���ͼ�����ݷ���ռ�
	m_pImageBuffer = new(std::nothrow) BYTE[(size_t)(m_i64ImageWidth * m_i64ImageHeight)];
	if (NULL == m_pImageBuffer)
	{
		throw std::runtime_error("Fail to allocate memory");
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
	if (NULL == m_pImageBuffer)
	{
		m_objLock.Unlock();
		return;
	}

	// Ϊ��ͼ��׼��
	RECT objRect;
	m_pWnd->GetClientRect(&objRect);
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
\brief     ������ʾͼ��
\param     objCImageDataPointer  ͼ�����ݶ���
\param     strDeviceSNFPS        ͼ��֡�����к�
\return    ��
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

	// ����Ļ�ϻ�ͼ
	__DrawImg(pDeviceSNFPS);
}

//----------------------------------------------------------------------------------
/**
\brief     ����ͼ�������ʾͼ��
\param     objCfg  ͼ������ڲ�������
\param     objCImageDataPointer  ͼ�����ݶ���
\return    ��
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
\brief     �洢Bmpͼ��
\param     objCImageDataPointer  ͼ�����ݶ���
\param     strFilePath  ��ʾͼ���ļ���
\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveBmp(const std::string& strFilePath) const
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
	::WriteFile(hFile, m_pBmpInfo, dwBitmapInfoHeader, &dwBytesRead, NULL); //�ڰ׺Ͳ�ɫ����Ӧ
	::WriteFile(hFile, m_pImageBuffer, dwImageSize, &dwBytesRead, NULL);

	CloseHandle(hFile);
}

//----------------------------------------------------------------------------------
/**
\brief     �洢Rawͼ��
\param     objCImageDataPointer  ͼ�����ݶ���
\param     strFilePath  ��ʾͼ���ļ���
\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::SaveRaw(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath) const
{
	if ((objCImageDataPointer.IsNull())||(strFilePath == ""))
	{
		throw std::runtime_error("Argument is error");
	}

	DWORD   dwImageSize = (DWORD)objCImageDataPointer->GetPayloadSize();  // д���ļ��ĳ���
	DWORD   dwBytesRead = 0;                // �ļ���ȡ�ĳ���

	BYTE* pbuffer = (BYTE*)objCImageDataPointer->GetBuffer();
	// �����ļ�
	HANDLE hFile = ::CreateFile(strFilePath.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,														
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)   // ����ʧ���򷵻�
	{
		throw std::runtime_error("Handle is invalid");
	}
	else                                 // ����Rawͼ��          
	{ 
		::WriteFile(hFile, pbuffer, dwImageSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
	}
}

//----------------------------------------------------------------------------------
/**
\brief     ��ǰ�����Ƿ�Ϊ��ɫ
\param     objCGXDevicePointer  [in]    �豸���
\param     bIsColorFilter       [out]   �Ƿ�֧�ֲ�ɫ

\return
*/
//----------------------------------------------------------------------------------
void CGXBitmap::IsColor(CGXDevicePointer& objCGXDevicePointer, bool &bIsColorFilter) const
{
	const std::string  strPixelFormat = objCGXDevicePointer->GetRemoteFeatureControl()
		->GetEnumFeature("PixelFormat")->GetValue().c_str();

	GX_PIXEL_FORMAT_ENTRY i32Pixel = static_cast<GX_PIXEL_FORMAT_ENTRY>(__ConvertPixelFormatToInt(strPixelFormat));

	//��ͼ���ʽ�������궨������λ�루&�����㣬���ж����ظ�ʽ��mono����RGB
	const int32_t i32PixelMono = 0x01000000;               //�ж��Ƿ�ΪMONO��ʽ������
	const int32_t i32PixelRgb = 0x20000000;                //�ж��Ƿ�ΪRGB��ʽ������ 
	const int32_t i32PixelMonoRgbCustom = 0x80000000;      //�ж��Ƿ�ΪMONO��ʽ������ 
	const int32_t i32PixelColorMask = 0xFF000000;          //�ж��Ƿ�Ϊ��ɫ��ʽ������

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

	bIsColorFilter = !(bIsMono && (!bIsBayer) && (!bIsRgb));  // �����ж��Ƿ�Ϊ�ڰ����
}

//----------------------------------------------------------------------------------
/**
\brief     �ַ������ظ�ʽת��������

\return    �������ظ�ʽ
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
\brief   Ϊ���ͼ����ʾ׼����Դ

\return  ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::PrepareForShowImg()
{
	// �ͷž��ڴ�, ԭ����ֹͣ�ɼ������ͻص��������첽��
	// ����ֹͣ�ɼ���ֱ���ͷ��ڴ��������Ϊ�ص������������е��³������
	// ���Ը�Ϊ�ڿ�ʼ�ɼ�ʱ�ͷž��ڴ�
	UnPrepareForShowImg();

	// ����ͼ��Ŀ��ߡ����ظ�ʽ��ת��������ظ�ʽ��
	__GetBasicAttribute();

	// ����ͼ��ת�����
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
\brief     �ͷ�Ϊͼ����ʾ׼������Դ

\return    ��
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
\brief     �õ����͵ĵ�ǰ���ظ�ʽ

\return    ���͵ĵ�ǰ���ظ�ʽ
*/
//----------------------------------------------------------------------------------
int64_t CGXBitmap::GetCurrentPixelFormat()
{
	//��ȡ��ǰ���ظ�ʽ
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
\brief     �õ������ͼ�Ŀ�����ظ�ʽ������

\return    ��
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__GetBasicAttribute()
{
	//���ͼ���ȡ��߶ȵ�
	m_i64ImageWidth = (int64_t)m_objCGXDevicePointer->GetRemoteFeatureControl()->GetIntFeature("Width")->GetValue();
	m_i64ImageHeight = (int64_t)m_objCGXDevicePointer->GetRemoteFeatureControl()->GetIntFeature("Height")->GetValue();

	//��ȡ��ǰ���ظ�ʽ
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
	
	//��ȡ��ǰ���ظ�ʽ�Ƿ�Ϊ��ɫ
	IsColor(m_objCGXDevicePointer,m_bIsColor);

}

//----------------------------------------------------------------------------------
/**
\brief     ����ͼ��ת�����
\param     hConvertHandle ͼ��ת�����
\param     emInputPixelForma ����ͼ������ظ�ʽ

\return    trueΪ8Ϊ���ݣ�falseΪ��8λ����
*/
//----------------------------------------------------------------------------------
void CGXBitmap::__SetConvertHandle()
{
	// ���ò�ֵ��ʽ
	m_pConvert->SetInterpolationType(GX_RAW2RGB_NEIGHBOUR);

	// ��ȡ��Чλ��
	GX_VALID_BIT_LIST emValidBits = GetBestValudBit(static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));

	// ������Чλ��
	m_pConvert->SetValidBits(emValidBits);

	if (m_bIsColor)
	{
		// ����ͼ���ʽת�������ת��ΪBGR8��ʽ
		m_pConvert->SetDstFormat(GX_PIXEL_FORMAT_BGR8);
		m_i64ConvertSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight, GX_PIXEL_FORMAT_BGR8);
	}
	else
	{
		// ����ͼ���ʽת�������ת��ΪRGB8��ʽ
		m_pConvert->SetDstFormat(GX_PIXEL_FORMAT_MONO8);
		m_i64ConvertSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight, GX_PIXEL_FORMAT_MONO8);
	}

	//����ת��ǰ��ͼ��Ĵ�С
	m_i64RawSize = m_pConvert->GetBufferSizeForConversion(m_i64ImageWidth, m_i64ImageHeight
		, static_cast<GX_PIXEL_FORMAT_ENTRY>(m_i64PixelFormat));
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
		|| (NULL == pRawImageBuffer))
	{
		m_objLock.Unlock();
		return;
	}
	BYTE* pRawBuffer = reinterpret_cast<BYTE*>(pRawImageBuffer);
	if (GX_PIXEL_FORMAT_BGR8 == m_i64PixelFormat)
	{
		// RGB��ʽ��Ҫ��ת���ݺ���ʾ
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
		// �ڰ������Ҫ��ת���ݺ���ʾ
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
\brief     �ж�ͼ���Ƿ���Ҫ��ת

\return    true��Ҫ��ת��false����Ҫ��ת
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
