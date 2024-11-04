#pragma once

#include "Def.h"
#include "CvvImage.h"
#include "string"

using namespace std;

//--------------------------------------------
/**
\brief Image acquisition class
*/
//---------------------------------------------
class CSampleGrabberCB : public ISampleGrabberCB
{
public:  
    ///Constructor
    CSampleGrabberCB();
    ///Destructor
    virtual ~CSampleGrabberCB(); 

    ///Set the width of the captured image
    void SetWidth(const long lWidth);
    ///Set the height of the captured image
    void SetHeight(const long lHeight);

    ///Set display interface handle
    void SetWindowWnd(CWnd *pWnd);
    ///Set the ID of the current collection device
    void SetDevID(const int nDevID);
    ///Set the current device color flag
    void SetColorFlag(const bool bColorFlag);
	///Set the current device Image Save Flag
	void SetImageSaveFlag(const bool bSaveFlag);
	///Free image memory space
	void ReleaseImage();

    ///Rewrite interface
    STDMETHODIMP_(ULONG) AddRef();
    ///Rewrite interface
    STDMETHODIMP_(ULONG) Release(); 
    ///Rewrite interface
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
    ///Rewrite callback collection function interface
    STDMETHODIMP SampleCB(double dSampleTime, IMediaSample *pSample );
    ///Rewrite callback collection function interface
    STDMETHODIMP BufferCB(double dSampleTime, BYTE *pBuffer, long lBufferSize);

private:
    ///Create image Buffer
    void __CreateImage();

	///WORD type to string type
	string __WORDToString(WORD wChar);

private:
    long        m_lWidth;           ///<The width of the current captured image
    long        m_lHeight;          ///<The High of the current captured image

    int         m_nDevID;           ///<ID of the current collection device
    bool        m_bColorFlag;       ///<Color flag

    CWnd        *m_pWnd;            ///<Display interface handle
    IplImage    *m_pImage;          ///<Image buffer

	bool        m_bSavePictureFlg;  ///<Image save flag
};

