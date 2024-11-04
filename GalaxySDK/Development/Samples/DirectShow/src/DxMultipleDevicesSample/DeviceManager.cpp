#include "StdAfx.h"
#include "DeviceManager.h"

//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CDeviceManager::CDeviceManager(void)
{
    
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CDeviceManager::~CDeviceManager(void)
{
}

//---------------------------------------------------
/**
\brief  Enumerate registered devices
\return int Output the number of devices enumerated
*/
//----------------------------------------------------
int CDeviceManager::EnumDevice(void)
{
    int                         nCount          = 0;
    ULONG                       lFetched        = 0;
    HRESULT                     hResult         = S_OK;
    IMoniker                    *pMoniker       = NULL;
    CComPtr<ICreateDevEnum>     pCreateDevEnum  = NULL;
    CComPtr<IEnumMoniker>       pEnumMoniker    = NULL;
    CDxEnumerator               objDxEnumerator;

    if(false == objDxEnumerator.CreateEnumerator(pCreateDevEnum, pEnumMoniker))
    {
        pCreateDevEnum = NULL;
        pEnumMoniker   = NULL;
        return nCount;
    }

    do 
    {
        hResult = pEnumMoniker->Next(1, &pMoniker, &lFetched);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);
        nCount++;
    } while (true);

    pCreateDevEnum = NULL;
    pEnumMoniker = NULL;
    if(pMoniker != NULL)
    {
        pMoniker->Release();
        pMoniker = NULL;
    }

    return nCount;
}

//---------------------------------------------------
/**
\brief  Obtain device information based on device ID
\param  nDevID          Input device ID
\param  strDevName      Output device information
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDeviceManager::GetDeviceInfo(const int &nDevID, std::string &strDevName)
{
    int                         nCount              = 0;
    ULONG                       lFetched            = 0;
    HRESULT                     hResult             = S_OK;
    IMoniker                    *pMoniker           = NULL;
    CComPtr<IPropertyBag>       pBag                = NULL;
    CComPtr<ICreateDevEnum>     pCreateDevEnum      = NULL;
    CComPtr<IEnumMoniker>       pEnumMoniker        = NULL;
    char                        szCamName[1024];
    VARIANT                     var;
    CDxEnumerator               objDxEnumerator;
    
    if(false == objDxEnumerator.CreateEnumerator(pCreateDevEnum, pEnumMoniker))
    {
        pCreateDevEnum = NULL;
        pEnumMoniker   = NULL;
        return DEVICE_GET_INFO_FAIL;
    }

    while(pEnumMoniker->Next(1, &pMoniker, &lFetched) == S_OK)
    {
        if(nCount != nDevID)
        {
            nCount++;
            continue;
        }

        hResult = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(hResult == S_OK)
        {
            //Get device name
            var.vt = VT_BSTR;
            hResult = pBag->Read(L"FriendlyName", &var, NULL);
            if(hResult == S_OK)
            {
                WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, szCamName, 1024 , "", NULL);
                SysFreeString(var.bstrVal);	
                strDevName = szCamName;
            }
            pBag = NULL;
        }
        pMoniker->Release();
        pMoniker = NULL;
        nCount++;
    }
    pCreateDevEnum = NULL;
    pEnumMoniker = NULL;

    return __IsDaHengDevice(strDevName);
}

DEVICE_STATUS CDeviceManager::__IsDaHengDevice(const std::string &strDeviceName)
{
    const std::string strDaHeng = "Daheng Imaging Device";

    if(strncmp(strDaHeng.c_str(), strDeviceName.c_str(), strDaHeng.length()) == 0)
    {
        return DEVICE_SUCCESS;
    }

    return DEVICE_GET_INFO_FAIL;
}