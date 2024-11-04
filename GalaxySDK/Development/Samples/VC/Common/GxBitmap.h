//------------------------------------------------------------------------
/**
\file		GXBitmap.h
\brief		������Ҫ����ͼ�����ʾ�ʹ洢��ͼ����ʾ�ʹ洢��������Ӧ�ڰײ�ɫ�����
ͼ��洢���Դ洢ΪBmp��Raw����ͼ����ʾ�ʹ洢����������

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

///< �жϷ���ֵ�궨��
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
\brief ͼ����ʾ���洢��
*/
//---------------------------------------------
class CGXBitmap
{
public:
	///���캯����ʹ��MFC���ھ��
	CGXBitmap(GX_DEV_HANDLE& hDevice, CWnd* pWnd);
	///���캯����ʹ��QT���ھ��
	CGXBitmap(GX_DEV_HANDLE& hDevice, HWND* pHwnd);
	///��������
	~CGXBitmap(void);

	///Ϊ���ͼ����ʾ׼����Դ
	GX_STATUS PrepareForShowImg();
	/// �ͷ�Ϊͼ����ʾ׼������Դ
	void UnPrepareForShowImg();
	
	///��ʾͼ��
	void Show(GX_FRAME_CALLBACK_PARAM* pCallbackFrame, char* pDeviceSNFPS = NULL);
	void Show(GX_FRAME_DATA* pFrame, char* pDeviceSNFPS = NULL);

	/// ͼ���ۺϴ������ʾͼ��
	void ShowAfterProcess(GX_FRAME_CALLBACK_PARAM* pCallbackFrame ,void * pImageProcess,char* pDeviceSNFPS = NULL);
	void ShowAfterProcess(GX_FRAME_DATA* pFrame,void * pImageProcess, char* pDeviceSNFPS = NULL);

	///ͼ��������������ʾͼ��
	void ShowAfterImprovment(GX_FRAME_CALLBACK_PARAM* pCallbackFrame,int64_t i64ColorCorrection, BYTE *pContrastLut
		, BYTE *pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder = DX_ORDER_RGB, char* pDeviceSNFPS = NULL);
	void ShowAfterImprovment(GX_FRAME_DATA* pFrame, int64_t i64ColorCorrection
		, BYTE *pContrastLut, BYTE *pGammaLut, DX_RGB_CHANNEL_ORDER emChannelOrder = DX_ORDER_RGB, char* pDeviceSNFPS = NULL);

	 /// �洢Bmpͼ��
	 void SaveBmp(const char* pFilePath);
	 /// �洢Rawͼ��
	 void SaveRaw(void* pRawBuffer, const char* pFilePath);

	 /// ��ʼ���洢AVI����Ҫ����Դ������
	 bool InitAVI(char* strStorePath = ".", uint64_t ui64PlayFPS = 15);
	 /// ����ʼ���洢AVI����Ҫ����Դ������
	 void UnInitAVI();
	 /// ѡ����Ƶ������
	 bool SelectCompressor();
	 /// �洢AVI
	 void SaveAVI(BYTE *pBuffer = NULL);

	 /// �Ƿ�Ϊ��ɫ���ظ�ʽ
	 bool IsColor(GX_PIXEL_FORMAT_ENTRY i32Pixel);

	 ///ͨ��GX_PIXEL_FORMAT_ENTRY��ȡ����Bitλ
	 DX_VALID_BIT GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);

	 /// ���ò�ֵ����
	 void SetInterpolationAlgorithm(DX_BAYER_CONVERT_TYPE emConvertType);


private:
	///�õ�����Ļ�������
	GX_STATUS __GetBasicAttribute();

	///��ʾͼ��
	void __Show(void* pRawImgBuff, char* pDeviceSNFPS = NULL);

	/// ͼ���ۺϴ������ʾ
	void __ShowAfterProcess(void* pRawImgBuff, void * pImageProcess, char* pDeviceSNFPS = NULL);

	/// ͼ��������������ʾ
	void __ShowAfterImprovment(void* pRawImgBuff, int64_t i64ColorCorrection, BYTE *pContrastLut, BYTE *pGammaLut
		, DX_RGB_CHANNEL_ORDER emChannelOrder, char* pDeviceSNFPS = NULL);

	/// ʹ��ͼ���ۺϴ���������ͼ������
	void __ImageProcess(bool bIsColor, void* pImageBuffer, void * pImageProcess);

	///����ת�����
	GX_STATUS __SetConvertHandle(GX_PIXEL_FORMAT_ENTRY emInputPixelFormat);	

	/// �ж�PixelFormat�Ƿ�Ϊ8λ
	bool __IsPixelFormat8(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);

	///��m_pBufferRGB��ͼ���֡����ʾ������
	void __DrawImg(char* strDeviceSNFPS = NULL);

	///��תͼ��buffer
	void __FilpImage(void* pRawImageBuffer);
	/// �ж�ͼ����ʾǰ�Ƿ���Ҫ��ת
	bool __NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat) const;

	///Ϊ��ɫ���ͼ����ʾ׼����Դ
	GX_STATUS __ColorPrepareForShowImg();
	///Ϊ�ڰ����ͼ����ʾ׼����Դ
	GX_STATUS __MonoPrepareForShowImg();

	///��������ռ���ֽ���
	int64_t __GetStride(int64_t i64Width, bool bIsColor) const;

	char                            m_chBmpBuf[2048];	             ///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
	bool							m_bIsColor;						 ///< ��ǰ�Ƿ��ǲ�ɫ����
	BYTE                           *m_pImageBuffer;                  ///< ���洦����ͼ��buffer������ʾ
	BYTE                           *m_pBufferMono;                   ///< Monoͼ�����ݣ����ںڰ�ͼ������ɺ�ת
	BYTE                           *m_pBufferRaw16To8;               ///< ��8λͼת��8λ���buffer
	int64_t                         m_i64ImageHeight;                ///< ԭʼͼ���
	int64_t                         m_i64ImageWidth;                 ///< ԭʼͼ���
	int64_t                         m_i64PixelFormat;                ///< ��ǰ�����ظ�ʽ
	int32_t                         m_i32RawSize;                    ///< �豸���ԭʼͼ���С
	int32_t                         m_i32ConvertSize;                ///< ת����ͼ���С
	BITMAPINFO                     *m_pBmpInfo;	                     ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��

	HDC                             m_hDC;                           ///< ����ͼ��DC���
	CWnd                           *m_pWnd;                          ///< ��ʾͼ�񴰿�(�ؼ�)ָ��
	HWND                           *m_pHwnd;                         ///< ��ʾͼ��Ĵ���(�ؼ�)ָ��
	DX_IMAGE_FORMAT_CONVERT_HANDLE  m_hConvert;                      ///< ���ظ�ʽת�����
	GX_DEV_HANDLE                   m_hDevice;                       ///< �豸���

	CStoreAVI                      *m_pSaveAVI;                      ///< �洢��Ƶ��
	CLock                           m_objLock;                       ///< ������ֹ��Դ�ͷ�ʱ�ص����ڽ���
	DX_BAYER_CONVERT_TYPE           m_emConvertType;                 ///< ��ֵ�㷨

private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

