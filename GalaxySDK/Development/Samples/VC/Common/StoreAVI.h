//------------------------------------------------------------------------
/**
\file		StoreAVI.h
\brief		������Ҫ����AVI��Ƶ�Ĵ洢

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

//����AVI��Ƶ������ͷ�ļ�
#include <wtypes.h>
#include <atlbase.h>
#include <shlobj.h>

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

#define  AVI_MAX_SIZE   0x80000000  ///< AVI�ļ����ֵ:2GB

///< AVI��Ƶÿ֡ͼ����������ڸ�֡ͷ��ƫ��
#define AVI_FRAME_OFFSET  8

using namespace std;

//--------------------------------------------
/**
\brief ����
*/
//---------------------------------------------
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

//--------------------------------------------
/**
\brief ��Ƶ�洢��
*/
//---------------------------------------------
class CStoreAVI
{
public:
	CStoreAVI(BITMAPINFO* stInfo, int64_t i64ImageWidth, int64_t i64ImageHeight, char* strStorePath = "."
		, uint64_t ui64PlayFPS = 15);
	~CStoreAVI();

	/// ѡ����Ƶ������
	bool           SelectCompressor(HWND* pParent);

	/// ����AVI��Ƶʵ�ֺ���:�ú������м����̿ռ�,����AVI�ļ���С����չ����
	void           ExSaveAVI(BYTE * pImageBuf);

	/// ����AVI��Ƶʵ�ֺ���:�ú���ֻʵ��дAVI�ļ�,������������չ����
	void           SaveAVI(BYTE *pImageBuf);

	/// Ϊ����avi��Ƶ�ļ�׼����Դ
	bool           PrepareForSaveAVI(bool bPixelFormat);

	/// �ͷ�Ϊ����AVI��Ƶʹ�õ���Դ
	void           UnPrepareForSaveAVI();

private:

	/// ֹͣ��ǰ¼�����´���AVI��Ƶ�ļ�
	bool           __ReCreatVedioFile();

	/// ��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�
	int64_t        __GetDiskFreeSpace() const;

	/// �ͷ���Ƶ�����������Դ
	void           __ResetCompressor();

	bool	       m_bCompressedAVI;              ///< �Ƿ�ѹ���洢
	int64_t        m_nImageWidth;                 ///< ͼ���
	int64_t        m_nImageHeight;                ///< ͼ���
	bool		   m_bIsColor;					  ///< ��ǰ�Ƿ��ǲ�ɫ����
	int64_t        m_nFrameSize;                  ///< ����AVI��Ƶ����ʱÿ֡�Ĵ�С
	int64_t        m_nWrittenDataSize;            ///< д��AVI�ļ���ͼ�����ݴ�С��������AVI�ļ�ͷ��С��
	uint64_t       m_ui64PlayFPS;                 ///< AVI��Ƶ����֡��

	CString        m_strStorePath;                ///< AVI�ļ��洢·��

	BITMAPINFO     *m_pBmpInfo;	                  ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��

	HIC            m_hic;                         ///< ���ʹ�õ���Ƶѹ�������
	LONG           m_nTimeFrame;                  ///< д��avi�ļ���Ҫ��֡ID��
	PAVIFILE       m_pAVIFile;                    ///< AVI�ļ��ľ��
	COMPVARS       m_Com;                         ///< ѡ�������ʹ�ò���
	PAVISTREAM     m_ps;                          ///< ͨ��AVI�ļ���������Ƶ��
	PAVISTREAM	   m_psCompressed;                ///< ͨ����Ƶ������Ƶѹ����������ѹ����Ƶ��
	CLock          m_objLock;                     ///< ������
};


inline bool  MsgComposer(char* pPath,const char* pFormat, ...)
{
	char chBuffer[256] = { 0 };
	va_list vap;
	va_start(vap, pFormat);

	#if _MSC_VER >= 1400 //�����VS2005�������ϰ汾
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
	#else if _MSC_VER == 1200 //�����VC6
		_vsnprintf(chBuffer, 256, pFormat, vap );
	#endif
	va_end(vap);
	memcpy(pPath, chBuffer, 256);
	return true;
};
