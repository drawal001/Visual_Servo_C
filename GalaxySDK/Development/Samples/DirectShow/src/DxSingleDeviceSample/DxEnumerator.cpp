#include "StdAfx.h"
#include "DxEnumerator.h"

//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CDxEnumerator::CDxEnumerator(void)
{
    CoInitialize(NULL);
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CDxEnumerator::~CDxEnumerator(void)
{
    CoUninitialize();
}

//---------------------------------------------------
/**
\brief  Create an enumerator component
\param  pCreateDevEnum  Input CreateDevEnum
\param  pEnumMoniker    Input EnumMoniker
\return bool
*/
//----------------------------------------------------
bool CDxEnumerator::CreateEnumerator(CComPtr<ICreateDevEnum> &pCreateDevEnum, CComPtr<IEnumMoniker> &pEnumMoniker)
{
    bool    bRet    = false;
    HRESULT hResult = S_OK;

    do 
    {
        hResult = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&pCreateDevEnum);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pEnumMoniker->Reset();
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        bRet = true;
    } while (0);

    return bRet;
}
