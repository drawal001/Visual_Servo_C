#pragma once

#include "Def.h"
#include "GXBase.h"
#include "DxEnumerator.h"
#include "SampleGrabberCB.h"

#include <core/core.hpp>
#include <highgui/highgui.hpp>

//--------------------------------------------
/**
\brief Device operation class
*/
//---------------------------------------------
class CDevice
{
public:
    ///Constructor
    CDevice(void);
    ///Destructor
    virtual ~CDevice(void);

    ///Open the selected device
    DEVICE_STATUS OpenDevice(int nDevID);
    ///Close selected device
    DEVICE_STATUS CloseDevice();

    ///Send a start acquisition command to the current device
    DEVICE_STATUS StartAcquisition(CWnd *pWnd);
    ///Send a stop acquisition command to the current device
    DEVICE_STATUS StopAcquisition();

    ///Get exposure time from device
    DEVICE_STATUS GetExposureTimeFromDevice(long &lExposureTime);
    ///Get gain from device
    DEVICE_STATUS GetGainFromDevice(long &lGain);
    ///Get white balance from the device
    DEVICE_STATUS GetWhiteBalanceFromDevice(long &lWhiteBalance);
    ///Get the color camera flag from the device
    DEVICE_STATUS GetColorFromDevice(bool &bColorFlag);

    ///Get the exposure time range value from the device
    DEVICE_STATUS GetExposureTimeRangeFromDevice(DEVICE_EXPOSURE_TIME_RANGE &stExposureTimeRange);
    ///Get the gain range value from the device
    DEVICE_STATUS GetGainRangeFromDevice(DEVICE_GAIN_RANGE &stGainRange);
    ///Get the white balance range value from the device
    DEVICE_STATUS GetWhiteBalanceRangeFromDevice(DEVICE_WHITE_BALANCE_RANGE &stWhiteBalanceRange);

    ///Get the Filter property page
    DEVICE_STATUS GetFilterProperty(const HWND hWnd);
    ///Get the Pin property page
    DEVICE_STATUS GetPinProperty(const HWND hWnd);

    ///Set the exposure time to the device
    DEVICE_STATUS SetExposureTimeToDevice(long lExposureTime);
    ///Set the gain to the device
    DEVICE_STATUS SetGainToDevice(long lGain);
    ///Set white balance to the device
    DEVICE_STATUS SetWhiteBalanceToDevice(long lWhiteBalance);

    ///Get open device flag
    const bool GetOpenFlag() const;
    ///Get the start collection flag
    const bool GetAcquisitionFlag() const;
    ///Get color camera flag
    bool GetColorFlag();
    ///Get the exposure time of the device
    const long GetExposureTime() const;
    ///Get the gain of the device
    const long GetGain() const;
    ///Get the white balance of the device
    const long GetWhiteBalance() const;

private:
    ///Release resources
    void __ReleaseSource();
    ///Bind the device to the Filter
    DEVICE_STATUS __BindFilter(const int &nDevID);

    ///Set the exposure time execution function
    DEVICE_STATUS __SetExposureTime(const long lExposureTime, const long lControlFlags);
    ///Set the gain execution function
    DEVICE_STATUS __SetGain(const long lGain, const long lControlFlags);
    ///Set the white balance execution function
    DEVICE_STATUS __SetWhiteBalance(const long lWhiteBalance, const long lControlFlags);

private:
    int                     m_nDevID;                   ///<Record current device ID
    int                     m_nWidth;                   ///<Record the width of the image captured by the current device
    int                     m_nHeight;                  ///<Record the high image captured by the current device

    long                    m_lExposureTime;            ///<Record the exposure time of the current device
    long                    m_lGain;                    ///<Record the gain of the current device
    long                    m_lWhiteBalance;            ///<Record the white balance of the current device

    bool                    m_bOpenFlag;                ///<Whether the current device opens the flag
    bool                    m_bAcquisitionFlag;         ///<Whether the current device is collecting the flag bit
    bool                    m_bColorFlag;               ///<Current device color flag
    CSampleGrabberCB        m_objSampleGrabberCB;       ///<CSampleGrabberCB handle

    CComPtr<IGraphBuilder>  m_pGraph;                   ///<FilterGraph handle
    CComPtr<IBaseFilter>    m_pSampleGrabberFilter;     ///<SampleGrabber handle
    CComPtr<IBaseFilter>    m_pNullFilter;              ///<Filter handle
    CComPtr<IMediaControl>  m_pMediaControl;            ///<MediaControl handle
    CComPtr<ISampleGrabber> m_pSampleGrabber;           ///<SampleGrabber handle
    CComPtr<IBaseFilter>    m_pDeviceFilter;            ///<Binding device filter
    CComPtr<IPin>           m_pGrabberInput;            ///<Grabber Input pin
    CComPtr<IPin>           m_pGrabberOutput;           ///<Grabber output pin
    CComPtr<IPin>           m_pCameraOutput;            ///<CameraOutput output pin
    CComPtr<IPin>           m_pNullInputPin;            ///<Filter input pin
};

