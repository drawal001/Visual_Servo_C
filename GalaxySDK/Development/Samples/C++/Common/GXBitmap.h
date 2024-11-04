//------------------------------------------------------------------------
/**
\file		GXBitmap.h
\brief		������Ҫ����ͼ�����ʾ�ʹ洢��ͼ����ʾ�ʹ洢��������Ӧ�ڰײ�ɫ�����
ͼ��洢���Դ洢ΪBmp��Raw����ͼ����ʾ�ʹ洢����������
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
	/// ����
	CLock() { InitializeCriticalSection(&m_cs); }
	/// ����
	virtual ~CLock() { DeleteCriticalSection(&m_cs); }
public:
	/// �����ٽ���
	void Lock() { EnterCriticalSection(&m_cs); }
	/// �뿪�ٽ���
	void Unlock() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs; ///< �ٽ�α���	
};


class CGXBitmap
{
public:
	///���캯��
	CGXBitmap(CGXDevicePointer& objCGXDevicePointer,CWnd* pWnd);

	///��������
	~CGXBitmap(void);

	 ///��ʾͼ��֡��
	 void Show(CImageDataPointer& objCImageDataPointer,char* pDeviceSNFPS = NULL);

	 ///ͼ�������ʾͼ��
	 void ShowImageProcess(CImageProcessConfigPointer& objCfg, CImageDataPointer& objCImageDataPointer);

	 /// �洢Bmpͼ��
	 void SaveBmp(const std::string& strFilePath) const;

	 /// �洢Rawͼ��
	 void SaveRaw(CImageDataPointer& objCImageDataPointer,const std::string& strFilePath) const;

	 ///ͨ��GX_PIXEL_FORMAT_ENTRY��ȡ����Bitλ
	 GX_VALID_BIT_LIST GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry) const;

	 /// ��ǰ�����Ƿ�Ϊ��ɫ
	 void IsColor(CGXDevicePointer& objCGXDevicePointer, bool &bIsColorFilter) const;

	 ///Ϊ���ͼ����ʾ׼����Դ
	 void PrepareForShowImg();

	 /// �ͷ�Ϊͼ����ʾ׼������Դ
	 void UnPrepareForShowImg();

	 /// �õ���ǰ�����ظ�ʽ
	 int64_t GetCurrentPixelFormat();


private:

	///�õ�����Ļ�������
	void __GetBasicAttribute();

	///����ת�����
	void __SetConvertHandle();

	///Ϊ��ɫ���ͼ����ʾ׼����Դ
	void __ColorPrepareForShowImg();

	///Ϊ�ڰ����ͼ����ʾ׼����Դ
	void __MonoPrepareForShowImg();

	///��m_pBufferRGB��ͼ���֡����ʾ������
	void __DrawImg(char* strDeviceSNFPS = NULL);

	///��������ռ���ֽ���
	int64_t __GetStride(int64_t i64Width, bool bIsColor) const;

	/// ת���ַ������ظ�ʽΪ����
	int64_t __ConvertPixelFormatToInt(std::string PixelFormat) const;

	///��תͼ��buffer
	void __FilpImage(void* pRawImageBuffer);
	/// �ж�ͼ����ʾǰ�Ƿ���Ҫ��ת
	bool __NeedFilp(GX_PIXEL_FORMAT_ENTRY emPixelFormat)const;

private:

	CWnd*                         m_pWnd;                           ///<��ʾͼ�񴰿�(�ؼ�)ָ��
	bool                          m_bIsColor ;                      ///<�Ƿ��ǲ�ɫ����
	int64_t                       m_i64ImageHeight;                 ///<ԭʼͼ���
	int64_t                       m_i64ImageWidth;                  ///<ԭʼͼ���
	int64_t                       m_i64PixelFormat;                 ///< ��ǰ�����ظ�ʽ
	int64_t                       m_i64RawSize;                     ///< �豸���ԭʼͼ���С
	int64_t                       m_i64ConvertSize;                 ///< ת����ͼ���С
	BITMAPINFO                   *m_pBmpInfo;	                    ///<BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	char                          m_chBmpBuf[2048];	                ///<BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
	HDC                           m_hDC;                            ///<����ͼ��DC���
	BYTE                         *m_pImageBuffer;                   ///<���淭ת���ͼ��������ʾ
	CGXDevicePointer              m_objCGXDevicePointer;            ///< �豸ָ��

	CGXImageFormatConvertPointer  m_pConvert;                       ///< ���ظ�ʽת��ָ��
	CGXImageProcessPointer        m_pProcess;                       ///< ͼ����������ָ��
	CLock                         m_objLock;                        ///< ������ֹ�ص����ڽ���ʱ������Դ��
private:
	CGXBitmap& operator=(const CGXBitmap&);
	CGXBitmap(const CGXBitmap&);
};

