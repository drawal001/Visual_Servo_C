//------------------------------------------------------------------------
/**
\file		StoreAVI.cpp
\brief		��������AVI��Ƶ�Ĵ洢

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
\brief   ���캯��
\param   stInfo         BITMAPINFO �ṹָ�룬��ʾ���洢ͼ��ʱʹ��
\param   i64ImageWidth  ͼ����
\param   i64ImageHeight ͼ��߶�
\param   strStorePath   AVI�ļ�����·��
\param   ui64PlayFPS    �����֡��

\return  ��
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
	// ����ÿ֡ͼ�����ݴ�С
	m_nFrameSize = m_nImageWidth * m_nImageHeight * COLOR_PIXEL;
	memset(&m_Com, 0, sizeof(m_Com));
}

//---------------------------------------------------------------------------------
/**
\brief   ��������

\return  ��
*/
//----------------------------------------------------------------------------------
CStoreAVI::~CStoreAVI()
{
	// ������Դ
	UnPrepareForSaveAVI();
}

//---------------------------------------------------------------------------------
/**
\brief   ѡ����Ƶ������
\param   pParent ���ھ��

\return  true ѡ��ɹ���falseѡ��ʧ��
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::SelectCompressor(HWND* pParent)
{
	LPBITMAPINFOHEADER lpbi = &m_pBmpInfo->bmiHeader;
	DWORD  fccType = ICTYPE_VIDEO;

	m_Com.cbSize = sizeof(m_Com);

	//����ָ����Ƶ����������,�û�ͨ���޸�mmioFOURCC�ĺ��������ָ��������,��mmioFOURCC('x','v','i','d')
	//HIC hic = ICOpen(ICTYPE_VIDEO,mmioFOURCC('d','i','v','x'),ICMODE_COMPRESS);

	//���û������PC�����б������ĵ����ͺ�����,��ͨ������Ĵ���ѡ�������
	BOOL bRet = ICCompressorChoose(*pParent, ICMF_CHOOSE_ALLCOMPRESSORS, NULL, NULL, &m_Com, "ѡ��ѹ������");


	if (!bRet)
	{
		// bRetΪFALSE��ʶ�û�δѡ����Ƶ������
		return false;
	}
	HIC hic = ICOpen(m_Com.fccType, m_Com.fccHandler, ICMODE_QUERY);

	// ��ѡ����Ƶ������:�ж�ѡ��ı������Ƿ����
	if (hic != NULL)
	{
		if ((fccType == ICTYPE_VIDEO) && (ICCompressQuery(hic, lpbi, NULL) != ICERR_OK))
		{
			__ResetCompressor();
			MessageBox(NULL, TEXT("ѡ���ѹ����Ƶ��������,������ѡ��!"), TEXT("����"), MB_OK);
			return false;
		}

		m_hic = hic;
		m_bCompressedAVI = true;
	}

	return true;
}

//----------------------------------------------------------------------------------
/**
\brief  Ϊ����avi��Ƶ�ļ�׼����Դ
\param  bPixelFormat �Ƿ�Ϊ��ɫ

\return true:׼���ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::PrepareForSaveAVI(bool bPixelFormat)
{
	bool    bRet = FALSE;
	LONG    cbFormat = 0;    //�ļ���ʽ
	m_bIsColor = bPixelFormat;
	if (bPixelFormat)
	{
		// ����RGBת����24ͼ�����ݸ�ʽ
		cbFormat = sizeof(BITMAPINFOHEADER);
	}
	else
	{
		//  8λ�Ҷ�ͼ���ļ���ʽ
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
	//����һ��AVI�ļ�	
	hr = AVIFileOpen(&m_pAVIFile, chAVIFileName, OF_WRITE | OF_CREATE, NULL);
	if (AVIERR_OK !=hr)
	{
		return FALSE;
	}

	const int32_t COLOR_PIXEL = 3;
	// ����һ����Ƶ��
	::ZeroMemory(&StreamInfo, sizeof(StreamInfo));
	StreamInfo.fccType = streamtypeVIDEO;
	StreamInfo.fccHandler = 0;
	StreamInfo.dwScale = 1;
	StreamInfo.dwRate = static_cast<DWORD>(m_ui64PlayFPS);  //AVI��Ƶ���Ĳ���֡��
	StreamInfo.dwSuggestedBufferSize = DWORD(m_nImageWidth * m_nImageHeight * COLOR_PIXEL);
	SetRect(&StreamInfo.rcFrame, 0, 0, (int)m_nImageWidth, (int)m_nImageHeight);
	hr = AVIFileCreateStream(m_pAVIFile, &m_ps, &StreamInfo);
	if (AVIERR_OK != hr)
	{
		return FALSE;
	}

	//��ʼ��ѹ����Ƶ����Ҫ�Ľṹ��
	AVICOMPRESSOPTIONS opts;
	AVICOMPRESSOPTIONS FAR * aopts[1] = { &opts };

	if (m_bCompressedAVI)
	{
		if (NULL == m_hic)
		{
			return FALSE;
		}

		// ѹ���洢
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
		// ��ѹ���洢
		opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 
	}

	//����ѹ����Ƶ��
	hr = AVIMakeCompressedStream(&m_psCompressed, m_ps, &opts, NULL);
	if (AVIERR_OK != hr)
	{
		return FALSE;
	}

	//������Ƶ���������ļ���ʽ��
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
\brief  ����Ϊ����avi��Ƶ�ļ�׼����Դ

\return ��
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
\brief  ����AVI�ļ�
\param  pImageBuf  �������ͼ��buffer

\return ��
*/
//----------------------------------------------------------------------------------
void CStoreAVI::ExSaveAVI(BYTE * pImageBuf)
{

	m_objLock.Lock();
	// AVI��Ƶ����ʵ��
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		NULL);

	// �ж��Ƿ�д�ɹ�
	if (AVIERR_OK ==hr)
	{
		++m_nTimeFrame;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief  ����avi��Ƶ:�ú������м����̿ռ�,����AVI�ļ���С����չ����
\param  pImageBuf  [in]  ָ��ͼ�񻺳�����ָ��

\return ��
*/
//----------------------------------------------------------------------------------
void CStoreAVI::SaveAVI(BYTE * pImageBuf)
{
	if(!m_bIsColor)
	{
		m_nFrameSize = m_nImageWidth * m_nImageHeight;
	}

	// ---------------------------------------------
	// ��⵱ǰĿ¼���̿ռ�,��ʣ��ռ䲻��������洢ʱ��ֹͣ����
	if (__GetDiskFreeSpace() < m_nFrameSize + AVI_FRAME_OFFSET)
	{
		UnPrepareForSaveAVI();
		return;
	}

	LONG nBufferSize = 0; // ʵ��д�뵽AVI�ļ���ͼ�����ݴ�С,����ѹ����Ϊѹ����Ĵ�С
	BOOL bRet = TRUE;

	m_objLock.Lock();

	// �жϼ����洢ʱAVI�ļ���С�Ƿ��ﵽ��������ֵ
	// ���ﵽ�򴴽��µ�AVI�ļ�����¼��
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

	// AVI��Ƶ����ʵ��
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		&nBufferSize);

	// �ж��Ƿ�д�ɹ�
	if (AVIERR_OK == hr)
	{
		++m_nTimeFrame;

		// ͳ��AVI�ļ����ݴ�С:ÿ֡ͼ��ռ�ÿռ�Ϊ���ݴ�С����ƫ��ֵ8
		m_nWrittenDataSize = m_nWrittenDataSize + nBufferSize + AVI_FRAME_OFFSET;
	}
	m_objLock.Unlock();
}

//----------------------------------------------------------------------------------
/**
\brief  �ͷ���Ƶ�����������Դ

\return ��
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
\brief  ֹͣ��ǰ¼��,���´���AVI��Ƶ�ļ�,Ȼ�����¼��

\return TRUE:���´����ɹ� FALSE:����ʧ��
*/
//----------------------------------------------------------------------------------
bool CStoreAVI::__ReCreatVedioFile()
{
	bool bRet = TRUE;

	// ��ֹͣ���ڴ洢���ļ�
	UnPrepareForSaveAVI();

	// ���´���
	bRet = PrepareForSaveAVI(m_bIsColor);

	return bRet;
}


//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�

\return nFreeBytes   ʣ��ռ�,��λΪByte
*/
//----------------------------------------------------------------------------------
int64_t CStoreAVI::__GetDiskFreeSpace() const
{
	int64_t nFreeBytes = 0;
	BOOL    bResult;

	//ʹ��GetDiskFreeSpaceEx��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�      
	bResult = GetDiskFreeSpaceEx(m_strStorePath, 0, 0,
		(PULARGE_INTEGER)&nFreeBytes);

	return nFreeBytes;
}
