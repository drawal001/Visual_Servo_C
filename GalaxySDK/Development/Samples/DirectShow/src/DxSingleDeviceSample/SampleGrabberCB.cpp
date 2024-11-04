#include "StdAfx.h"
#include "SampleGrabberCB.h"
#include "resource.h"

//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CSampleGrabberCB::CSampleGrabberCB(void)
    : m_lWidth(0)
    , m_lHeight(0)
    , m_bColorFlag(false)
    , m_pWnd(NULL)
    , m_pImage(NULL)
    , m_nDevID(DEVICE_ID_INVAILD)
	, m_bSavePictureFlg(false)
{
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CSampleGrabberCB::~CSampleGrabberCB(void)
{
    ReleaseImage();
}

//---------------------------------------------------
/**
\brief Set the width of the captured image
\param lWidth Enter the width of the set image
*/
//----------------------------------------------------
void CSampleGrabberCB::SetWidth(const long lWidth)
{
    m_lWidth = lWidth;
}

//---------------------------------------------------
/**
\brief Set the height of the captured image
\param lHeight Enter the height of the settings image
*/
//----------------------------------------------------
void CSampleGrabberCB::SetHeight(const long lHeight)
{
    m_lHeight = lHeight;
}

//---------------------------------------------------
/**
\brief Set display interface handle
\param hWnd Input interface handle
*/
//----------------------------------------------------
void CSampleGrabberCB::SetWindowWnd(CWnd *pWnd)
{
    m_pWnd = pWnd;
}

//---------------------------------------------------
/**
\brief Set the ID of the current collection device
\param nDevID Input device ID
*/
//----------------------------------------------------
void CSampleGrabberCB::SetDevID(const int nDevID)
{
    m_nDevID = nDevID;
}

//---------------------------------------------------
/**
\brief Set the current device color flag
\param bColorFlag Input device color flag
*/
//----------------------------------------------------
void CSampleGrabberCB::SetColorFlag(const bool bColorFlag)
{
    m_bColorFlag = bColorFlag;
}

//---------------------------------------------------
/**
\brief Set the current device image save flag
\param bSaveFlag Input device image save flag
*/
//----------------------------------------------------
void CSampleGrabberCB::SetImageSaveFlag(const bool bSaveFlag)
{
	m_bSavePictureFlg = bSaveFlag;
}

//---------------------------------------------------
/**
\brief Rewrite interface
*/
//----------------------------------------------------
STDMETHODIMP_(ULONG) CSampleGrabberCB::AddRef()
{ 
    return S_OK; 
}

//---------------------------------------------------
/**
\brief Rewrite interface
*/
//----------------------------------------------------
STDMETHODIMP_(ULONG) CSampleGrabberCB::Release()
{ 
    return S_OK; 
} 

//---------------------------------------------------
/**
\brief Rewrite interface
*/
//----------------------------------------------------
STDMETHODIMP CSampleGrabberCB::QueryInterface(REFIID riid, void ** ppv) 
{  
    if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown)
    {  
        *ppv =  static_cast<void *>( this );  
        return S_OK;  
    }   
    return E_NOINTERFACE;  
}

//---------------------------------------------------
/**
\brief Rewrite callback collection function interface
*/
//----------------------------------------------------
STDMETHODIMP CSampleGrabberCB::SampleCB(double dSampleTime, IMediaSample *pSample)  
{  
    return S_OK;  
} 

//---------------------------------------------------
/**
\brief Rewrite callback collection function interface
\param dSampleTime  Enter the captured image timestamp
\param pBuffer      Enter the captured image data
\param lBufferSize  Enter the size of the captured image    
*/
//----------------------------------------------------
STDMETHODIMP CSampleGrabberCB::BufferCB(double dSampleTime, BYTE *pBuffer, long lBufferSize) 
{  
    CDC         *pDC            = NULL;
    CRect       objRect;
    CvvImage    objCvvImage;
	SYSTEMTIME  stSystemTime;
	string      strFileName     = "";

    if (pBuffer == NULL) 
    {
        return E_POINTER;  
    }

    __CreateImage();

    memcpy(m_pImage->imageData, (char*)pBuffer, lBufferSize);
    m_pWnd->GetClientRect(&objRect);
    pDC = m_pWnd->GetDC();
    if(pDC == NULL)
    {
        return E_POINTER; 
    }

    m_pImage->origin = IPL_ORIGIN_BL;
	objCvvImage.CopyOf(m_pImage);  
	objCvvImage.DrawToHDC(pDC->GetSafeHdc(),&objRect);

	if (m_bSavePictureFlg == true)
	{
		GetLocalTime(&stSystemTime);
		strFileName = SAVE_IMAGE_PATH;
		strFileName = strFileName + "\\" 
			+ __WORDToString(stSystemTime.wYear) + "_" 
			+ __WORDToString(stSystemTime.wMonth) + "_" 
			+ __WORDToString(stSystemTime.wDay) + "_" 
			+ __WORDToString(stSystemTime.wHour) + "_" 
			+ __WORDToString(stSystemTime.wMinute) + "_" 
			+ __WORDToString(stSystemTime.wSecond) + "_" 
			+ __WORDToString(stSystemTime.wMilliseconds) + ".bmp";

		cvSaveImage(strFileName.c_str(), m_pImage);
	}	

    ReleaseDC(m_pWnd->GetSafeHwnd(), pDC->GetSafeHdc());

    return S_OK;  
} 

//---------------------------------------------------
/**
\brief Create image Buffer  
*/
//----------------------------------------------------
void CSampleGrabberCB::__CreateImage()
{
    if(m_pImage != NULL)
    {
        return;
    }

    m_pImage = cvCreateImage(cvSize(m_lWidth, m_lHeight), IPL_DEPTH_8U, 3);
}

//---------------------------------------------------
/**
\brief   WORD type to string type
\param	 wChar	Enter the WORD value to convert
\return  string  Converted string type
*/
//----------------------------------------------------
string CSampleGrabberCB::__WORDToString(WORD wChar)
{
	char chTmp[16];
	string strRes;

	sprintf(chTmp,"%d",wChar);
	strRes = chTmp;

	return strRes;
}

//---------------------------------------------------
/**
\brief   Free image memory space
*/
//----------------------------------------------------
void CSampleGrabberCB::ReleaseImage()
{
	if (m_pImage != NULL) 
	{  
		cvReleaseImage(&m_pImage); 
		m_pImage = NULL;  
	}
}