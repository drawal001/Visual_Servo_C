#include "StdAfx.h"
#include "Device.h"

//---------------------------------------------------
/**
\brief Constructor
*/
//----------------------------------------------------
CDevice::CDevice(void) 
    : m_nDevID(DEVICE_ID_INVAILD)
    , m_lGain(0)
    , m_nWidth(0)
    , m_nHeight(0)
    , m_lExposureTime(0)
    , m_lWhiteBalance(0)
    , m_bOpenFlag(false)
    , m_bAcquisitionFlag(false)
    , m_bColorFlag(false)
{
    __ReleaseSource();
}

//---------------------------------------------------
/**
\brief Destructor
*/
//----------------------------------------------------
CDevice::~CDevice(void)
{
    CloseDevice();
}

//---------------------------------------------------
/**
\brief Open the selected device
\param  nDevID          Enter the device ID to open
\return DEVICE_STATUS   Return error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::OpenDevice(int nDevID)
{
    DEVICE_STATUS       emStatus    = DEVICE_OPEN_FAIL;
    HRESULT             hResult     = S_OK;
    CComPtr<IEnumPins>  pEnum       = NULL;
    
    do 
    {
        if(m_bOpenFlag == true)
        {
            emStatus = DEVICE_SUCCESS;
            break;
        }

        //Create a GraphManager, SampleGrabber, Filter component
        hResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&m_pGraph);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&m_pSampleGrabberFilter);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC , IID_IBaseFilter, (LPVOID *)&m_pNullFilter);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pGraph->AddFilter(m_pNullFilter, L"NullRenderer");
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (void **)&m_pSampleGrabber);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pGraph->AddFilter(m_pSampleGrabberFilter, L"Grabber");
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Binding device and Filter
        if(DEVICE_SUCCESS != __BindFilter(nDevID))
        {
            break;
        }

        //Bind device Filter and GraphManager
        hResult = m_pGraph->AddFilter(m_pDeviceFilter, NULL);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Prepare to set the device Filter pin to point to the Camera's Pin
        hResult = m_pDeviceFilter->EnumPins(&pEnum);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Reset this pin to the first pin
        hResult = pEnum->Reset();
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Set up a device Filter to connect to the Camera output pin
        hResult = pEnum->Next(1, &m_pCameraOutput, NULL); 
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Prepare to set the pins of SampleGrabber
        pEnum = NULL; 
        hResult = m_pSampleGrabberFilter->EnumPins(&pEnum);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Reset pin order to the first pin
        hResult = pEnum->Reset();
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Set the input pin of SampleGrabber
        hResult = pEnum->Next(1, &m_pGrabberInput, NULL); 
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        pEnum = NULL;
        hResult = m_pSampleGrabberFilter->EnumPins(&pEnum);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Reset pin order to the first pin
        hResult = pEnum->Reset();
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Skip the first input pin
        hResult = pEnum->Skip(1);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Set the output pin of SampleGrabber
        hResult = pEnum->Next(1, &m_pGrabberOutput, NULL); 
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Prepare to set up the pin of the empty renderer
        pEnum = NULL;
        hResult = m_pNullFilter->EnumPins(&pEnum);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Reset pin order to the first pin
        hResult = pEnum->Reset();
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Set the input pin of NullRenderer
        hResult = pEnum->Next(1, &m_pNullInputPin, NULL);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_nDevID = nDevID;
        m_bOpenFlag = true;
        emStatus = DEVICE_SUCCESS;
    } while (0);

    pEnum = NULL;

    //Failed to open device, release resources
    if(emStatus != DEVICE_SUCCESS)
    {
        m_bOpenFlag = false;
        __ReleaseSource();
    }

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Close selected device
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::CloseDevice()
{
    //Current device is not turned off
    if(m_bOpenFlag == true)
    {
		m_objSampleGrabberCB.SetImageSaveFlag(false);
        StopAcquisition();
        __ReleaseSource();

        m_nDevID = DEVICE_ID_INVAILD;
        m_bOpenFlag = false;
        m_bColorFlag = false;
    }

    return DEVICE_SUCCESS;
}

//---------------------------------------------------
/**
\brief  Send a start acquisition command to the current device
\param  pWnd            Enter the handle of the captured image display interface
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::StartAcquisition(CWnd *pWnd)
{
    DEVICE_STATUS emStatus = DEVICE_START_ACQUISITION_FAIL;
	HRESULT       hResult  = S_OK;

    do 
    {
        if(m_bAcquisitionFlag == true)
        {
            emStatus = DEVICE_SUCCESS;
            break;
        }

        //Connect Pin
        hResult = m_pGraph->Connect(m_pCameraOutput, m_pGrabberInput);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pGraph->Connect(m_pGrabberOutput, m_pNullInputPin);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Set the format of the captured image
        AM_MEDIA_TYPE stMediaType;
        ZeroMemory(&stMediaType, sizeof(AM_MEDIA_TYPE));
        hResult = m_pSampleGrabber->GetConnectedMediaType(&stMediaType);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        VIDEOINFOHEADER *stVideoHeader;
        stVideoHeader = reinterpret_cast<VIDEOINFOHEADER*>(stMediaType.pbFormat);
        m_nWidth = stVideoHeader->bmiHeader.biWidth;
        m_nHeight = stVideoHeader->bmiHeader.biHeight;

        m_objSampleGrabberCB.SetWidth(m_nWidth);
        m_objSampleGrabberCB.SetHeight(m_nHeight);
        m_objSampleGrabberCB.SetWindowWnd(pWnd);
        m_objSampleGrabberCB.SetDevID(m_nDevID);
        GetColorFromDevice(m_bColorFlag);
        m_objSampleGrabberCB.SetColorFlag(m_bColorFlag);

        hResult = m_pSampleGrabber->SetBufferSamples(FALSE);  
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pSampleGrabber->SetOneShot(FALSE);  
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        //Registration callback function
        hResult = m_pSampleGrabber->SetCallback(&m_objSampleGrabberCB, 1);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_pMediaControl->Run(); 

        m_bAcquisitionFlag = true;
        emStatus = DEVICE_SUCCESS;
    } while (0);

    if (emStatus != DEVICE_SUCCESS)
    {
        m_bAcquisitionFlag = false;
    }

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Send a stop acquisition command to the current device
\return DEVICE_STATUS Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::StopAcquisition()
{
    DEVICE_STATUS emStatus = DEVICE_SUCCESS;
	HRESULT       hResult  = S_OK;

    if(m_bAcquisitionFlag == false)
    {
        return emStatus;
    }

	do 
	{		
		if(m_pMediaControl->Stop() != S_OK)
		{
			emStatus = DEVICE_STOP_ACQUISITION_FAIL;
			break;
		}

		//Disconnect Pin
		hResult = m_pGraph->Disconnect(m_pCameraOutput);
		VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

		hResult = m_pGraph->Disconnect(m_pGrabberInput);
		VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

		hResult = m_pGraph->Disconnect(m_pGrabberOutput);
		VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

		hResult = m_pGraph->Disconnect(m_pNullInputPin);
		VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

	} while (0);

    m_nWidth = 0;
    m_nHeight = 0;
    m_bAcquisitionFlag = false;	
	m_objSampleGrabberCB.ReleaseImage();

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get exposure time from device
\param  lExposureTime   Output the acquired exposure time
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetExposureTimeFromDevice(long &lExposureTime)
{
    long                        lControlFlags   = 0;
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_EXPOSURE_TIME_FAIL;
    CComPtr<IAMCameraControl>   pCameraControl  = NULL;

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void **)&pCameraControl);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pCameraControl->Get(CameraControl_Exposure, &lExposureTime, &lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_lExposureTime = lExposureTime;
        emStatus = DEVICE_SUCCESS;
    } while (0);

    pCameraControl = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get gain from device
\param  lGain           Output gain
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetGainFromDevice(long &lGain)
{
    long                        lControlFlags   = 0;
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_GAIN_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;
    
    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->Get(VideoProcAmp_Gain, &lGain, &lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_lGain = lGain;
        emStatus = DEVICE_SUCCESS;
    } while (0);
    
    pVideoProcAmp = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get white balance from the device
\param  lWhiteBalance   Output the obtained white balance
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetWhiteBalanceFromDevice(long &lWhiteBalance)
{
    long                        lControlFlags   = 0;
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_WHITE_BALANCE_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->Get(VideoProcAmp_WhiteBalance, &lWhiteBalance, &lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_lWhiteBalance = lWhiteBalance;
        emStatus = DEVICE_SUCCESS;
    } while (0);

    pVideoProcAmp = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the color camera flag from the device
\param  bColorFlag      Output the obtained color flag
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetColorFromDevice(bool &bColorFlag)
{
    HRESULT                     hResult     = S_OK;
    DEVICE_STATUS               emStatus    = DEVICE_GET_COLOR_FAIL;
    CComPtr<IDHCamFilter>       pCamFilter  = NULL;

    do
    {
        if (m_bOpenFlag == false)
        {
            break;
        }

        hResult = m_pDeviceFilter->QueryInterface(IID_IDHCamFilter, (void **)&pCamFilter);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pCamFilter->IsColor(bColorFlag);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        m_bColorFlag = bColorFlag;
        emStatus = DEVICE_SUCCESS;
    } while (0);

    pCamFilter = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the exposure time range value from the device
\param  stExposureTimeRange Output exposure time range value
\return DEVICE_STATUS       Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetExposureTimeRangeFromDevice(DEVICE_EXPOSURE_TIME_RANGE &stExposureTimeRange)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_EXPOSURE_TIME_RANGE_FAIL;
    CComPtr<IAMCameraControl>   pCameraControl  = NULL;
    
    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void **)&pCameraControl);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pCameraControl->GetRange(CameraControl_Exposure, 
                                            &stExposureTimeRange.lExposureTimeMin, 
                                            &stExposureTimeRange.lExposureTimeMax, 
                                            &stExposureTimeRange.lExposureTimeStep, 
                                            &stExposureTimeRange.lExposureTimeDefault, 
                                            &stExposureTimeRange.lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pCameraControl = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the gain range value from the device
\param  stGainRange     Output gain range value
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetGainRangeFromDevice(DEVICE_GAIN_RANGE &stGainRange)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_GAIN_RANGE_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;
    
    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->GetRange(VideoProcAmp_Gain,
                                            &stGainRange.lGainMin, 
                                            &stGainRange.lGainMax, 
                                            &stGainRange.lGainStep, 
                                            &stGainRange.lGainDefault, 
                                            &stGainRange.lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pVideoProcAmp = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the white balance range value from the device
\param  stWhiteBalanceRange Output white balance range value
\return DEVICE_STATUS       Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetWhiteBalanceRangeFromDevice(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_GET_WHITE_BALANCE_REANGE_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;
    

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->GetRange(VideoProcAmp_WhiteBalance, 
                                            &stWhiteBalanceRange.lWhiteBalanceMin, 
                                            &stWhiteBalanceRange.lWhiteBalanceMax, 
                                            &stWhiteBalanceRange.lWhiteBalanceStep, 
                                            &stWhiteBalanceRange.lWhiteBalanceDefault, 
                                            &stWhiteBalanceRange.lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pVideoProcAmp = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the Filter property page
\param  hWnd            Enter the interface handle to display the Filter property page
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetFilterProperty(const HWND hWnd)
{
    FILTER_INFO                     stFilterInfo;
    CAUUID                          caGUID;
    DEVICE_STATUS                   emStatus        = DEVICE_GET_FILTER_PROPERTY_FAIL;
    HRESULT                         hResult         = S_OK;
    IUnknown                        *pFilterUnk     = NULL;
    CComPtr<ISpecifyPropertyPages>  pProp           = NULL;

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pDeviceFilter->QueryFilterInfo(&stFilterInfo);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pDeviceFilter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pProp->GetPages(&caGUID);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = OleCreatePropertyFrame(hWnd, 0, 0, L"Filter", 1, &pFilterUnk, caGUID.cElems, caGUID.pElems, 0, 0, NULL);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    if(pFilterUnk != NULL)
    {
        pFilterUnk->Release();
        pFilterUnk = NULL;
    }

    if(stFilterInfo.pGraph != NULL)
    {
        stFilterInfo.pGraph->Release();
        stFilterInfo.pGraph = NULL;
    }

    pProp = NULL;
    CoTaskMemFree(caGUID.pElems);

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Get the Pin property page
\param  hWnd            Enter the interface handle to display the Pin property page
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::GetPinProperty(const HWND hWnd)
{
    CAUUID                          caGUID;
    PIN_INFO                        stPinInfo;
    HRESULT                         hResult     = S_OK;
    DEVICE_STATUS                   emStatus    = DEVICE_GET_PIN_PROPERTY_FAIL;
    CComPtr<ISpecifyPropertyPages>  pProp       = NULL;

    do 
    {
        hResult = m_pCameraOutput->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pProp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = m_pCameraOutput->QueryPinInfo(&stPinInfo);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pProp->GetPages(&caGUID);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = OleCreatePropertyFrame(hWnd, 0, 0, L"Pin", 1, (IUnknown **)&(m_pCameraOutput.p), 
                               caGUID.cElems, caGUID.pElems, 0, 0, NULL);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);
    
    if(stPinInfo.pFilter != NULL)
    {
        stPinInfo.pFilter->Release();
        stPinInfo.pFilter = NULL;
    }
    pProp = NULL;
    CoTaskMemFree(caGUID.pElems);

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Set the exposure time to the device
\param  lExposureTime   Enter the exposure time you want to set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::SetExposureTimeToDevice(long lExposureTime)
{
    DEVICE_STATUS               emStatus = DEVICE_SET_EXPOSURE_TIME_FAIL;
    DEVICE_EXPOSURE_TIME_RANGE  stExposureTimeRange;

    emStatus = GetExposureTimeRangeFromDevice(stExposureTimeRange);
    if(emStatus != DEVICE_SUCCESS)
    {
        return emStatus;
    }

    VERIFY_PARAM_CONFORM_RANGE(lExposureTime, stExposureTimeRange.lExposureTimeMax,
                                               stExposureTimeRange.lExposureTimeMin,
                                               stExposureTimeRange.lExposureTimeStep);
    
    stExposureTimeRange.lControlFlags = CameraControl_Flags_Manual;
    return __SetExposureTime(lExposureTime, stExposureTimeRange.lControlFlags);
}

//---------------------------------------------------
/**
\brief  Set the gain to the device
\param  lGain           Enter the gain to set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::SetGainToDevice(long lGain)
{
    DEVICE_STATUS emStatus = DEVICE_SET_GAIN_FAIL;
    DEVICE_GAIN_RANGE stGainRange;

    emStatus = GetGainRangeFromDevice(stGainRange);
    if(emStatus != DEVICE_SUCCESS)
    {
        return emStatus;
    }

    VERIFY_PARAM_CONFORM_RANGE(lGain, stGainRange.lGainMax,
                                      stGainRange.lGainMin,
                                      stGainRange.lGainStep);

    stGainRange.lControlFlags = CameraControl_Flags_Manual;
    return __SetGain(lGain, stGainRange.lControlFlags);
}

//---------------------------------------------------
/**
\brief  Set white balance to the device
\param  lGain           Enter the white balance to be set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::SetWhiteBalanceToDevice(long lWhiteBalance)
{
    DEVICE_STATUS emStatus = DEVICE_SET_WHITE_BALANCE_FAIL;
    DEVICE_WHITE_BALANCE_RANGE stWhiteBalanceRange;

    emStatus = GetWhiteBalanceRangeFromDevice(stWhiteBalanceRange);
    if(emStatus != DEVICE_SUCCESS)
    {
        return emStatus;
    }

    VERIFY_PARAM_CONFORM_RANGE(lWhiteBalance, stWhiteBalanceRange.lWhiteBalanceMax,
                                              stWhiteBalanceRange.lWhiteBalanceMin,
                                              stWhiteBalanceRange.lWhiteBalanceStep);

	switch(lWhiteBalance)
	{
	case 0:
		stWhiteBalanceRange.lControlFlags = CameraControl_Flags_Manual;
		break;
	case 1:
		stWhiteBalanceRange.lControlFlags = CameraControl_Flags_Auto;
		break;
	case 2:
		stWhiteBalanceRange.lControlFlags = CameraControl_Flags_Manual;
		break;
	default:
		return emStatus;
	}

    return __SetWhiteBalance(lWhiteBalance, stWhiteBalanceRange.lControlFlags);
}

//---------------------------------------------------
/**
\brief  Save image 
\param  bImgSaveFlg     Enter the image save flag 
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::SaveImage(const bool bImgSaveFlg)
{
	DEVICE_STATUS emStatus = DEVICE_SET_SAVE_IMAGE_FAIL;
	if (false == m_bOpenFlag)
	{
		return emStatus;
	}

	m_objSampleGrabberCB.SetImageSaveFlag(bImgSaveFlg);

	emStatus = DEVICE_SUCCESS;
	return emStatus;
}

//---------------------------------------------------
/**
\brief  Get open device flag
\return bool
*/
//----------------------------------------------------
const bool CDevice::GetOpenFlag() const
{
    return m_bOpenFlag;
}

//---------------------------------------------------
/**
\brief  Get the start collection flag
\return bool
*/
//----------------------------------------------------
const bool CDevice::GetAcquisitionFlag() const
{
    return m_bAcquisitionFlag;
}

//---------------------------------------------------
/**
\brief  Get the exposure time of the device
\return long
*/
//----------------------------------------------------
const long CDevice::GetExposureTime() const
{
    return m_lExposureTime;
}

//---------------------------------------------------
/**
\brief  Get color camera flag
\return long
*/
//----------------------------------------------------
bool CDevice::GetColorFlag()
{
    return m_bColorFlag;
}

//---------------------------------------------------
/**
\brief  Get the gain of the device
\return long
*/
//----------------------------------------------------
const long CDevice::GetGain() const
{
    return m_lGain;
}

//---------------------------------------------------
/**
\brief  Get the white balance of the device
\return long
*/
//----------------------------------------------------
const long CDevice::GetWhiteBalance() const
{
    return m_lWhiteBalance;
}

//---------------------------------------------------
/**
\brief Release resources
*/
//----------------------------------------------------
void CDevice::__ReleaseSource()
{
    m_pGrabberInput         = NULL;
    m_pGrabberOutput        = NULL;
    m_pNullInputPin         = NULL;
    m_pCameraOutput         = NULL;
    m_pMediaControl         = NULL;
    m_pDeviceFilter         = NULL;
    m_pSampleGrabber        = NULL;
    m_pNullFilter           = NULL;
    m_pSampleGrabberFilter  = NULL;
    m_pGraph                = NULL;
}

//---------------------------------------------------
/**
\brief  Bind the device to the Filter
\param  nDevID          Enter the device to bind
\param  pDeviceFilter   Output bound Filter
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::__BindFilter(const int &nDevID)
{
    int                     nIndex          = 0;
    ULONG                   lFetched        = 0;
    bool                    bRet            = false;
    HRESULT                 hResult         = S_OK;
    IMoniker                *pMoniker       = NULL;
    CComPtr<ICreateDevEnum> pCreateDevEnum  = NULL;
    CComPtr<IEnumMoniker>   pEnumMoniker    = NULL;
    CComPtr<IPropertyBag>   pBag            = NULL;
    DEVICE_STATUS           emStatus        = DEVICE_BIND_FILTER_FAIL;
    CDxEnumerator           objDxEnumerator;

    if(false == objDxEnumerator.CreateEnumerator(pCreateDevEnum, pEnumMoniker))
    {
        pCreateDevEnum = NULL;
        pEnumMoniker   = NULL;
        return emStatus;
    }

    do 
    {
        hResult = pEnumMoniker->Next(1, &pMoniker, &lFetched);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        if (nIndex == nDevID)
        {
            //Generate a DirectShow Filter
            hResult  = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pDeviceFilter);
			if (hResult != S_OK)
			{
				emStatus = DEVICE_BIND_FILTER_FAIL;
				break;
			}
			emStatus = DEVICE_SUCCESS;
        }

        nIndex++;      
    } while (hResult == S_OK);

    if(pMoniker != NULL)
    {
        pMoniker->Release();
        pMoniker = NULL;
    }

    pCreateDevEnum = NULL;
    pEnumMoniker   = NULL;
    pBag           = NULL;

    return emStatus;
}

//---------------------------------------------------
/**
\brief  Set the exposure time execution function
\param  lExposureTime   Enter the exposure time you want to set
\param  lControlFlags   Enter the exposure time control standard bit to be set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::__SetExposureTime(const long lExposureTime, const long lControlFlags)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_SET_EXPOSURE_TIME_FAIL;
    CComPtr<IAMCameraControl>   pCameraControl  = NULL;
    
    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void **)&pCameraControl);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pCameraControl->Set(CameraControl_Exposure, lExposureTime, lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pCameraControl = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Set the gain execution function
\param  lGain           Enter the gain to set
\param  lControlFlags   Enter the gain control standard bit to be set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::__SetGain(const long lGain, const long lControlFlags)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_SET_GAIN_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->Set(VideoProcAmp_Gain, lGain, lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pVideoProcAmp = NULL;
    return emStatus;
}

//---------------------------------------------------
/**
\brief  Set the white balance execution function
\param  lWhiteBalance   Enter the white balance to be set
\param  lControlFlags   Enter the white balance control standard bit to be set
\return DEVICE_STATUS   Output error code
*/
//----------------------------------------------------
DEVICE_STATUS CDevice::__SetWhiteBalance(const long lWhiteBalance, const long lControlFlags)
{
    HRESULT                     hResult         = S_OK;
    DEVICE_STATUS               emStatus        = DEVICE_SET_WHITE_BALANCE_FAIL;
    CComPtr<IAMVideoProcAmp>    pVideoProcAmp   = NULL;

    do 
    {
        hResult = m_pDeviceFilter->QueryInterface(IID_IAMVideoProcAmp, (void **)&pVideoProcAmp);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        hResult = pVideoProcAmp->Set(VideoProcAmp_WhiteBalance, lWhiteBalance, lControlFlags);
        VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult);

        emStatus = DEVICE_SUCCESS;
    } while (0);

    pVideoProcAmp = NULL;
    return emStatus;
}

