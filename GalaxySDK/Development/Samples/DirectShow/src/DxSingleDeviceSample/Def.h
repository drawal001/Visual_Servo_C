#ifndef __DEF_H_
#define __DEF_H_

#define WIN32_LEAN_AND_MEAN

#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

#include "qedit.h"
#include "DShow.h"

#define DEVICE_ID_INVAILD       -1                          ///<Invalid Device ID
#define SAVE_IMAGE_PATH         "DxSingleCamMonoImages"     ///<The address of saved images

///Determine the return value of HRESULT
#define VERIFY_HRESULT_PRINT_RETURN_STATUS(hResult) \
{\
    if(S_OK != hResult)\
    {\
        break;\
    }\
}

///Determine whether the currently entered parameter meets the range value
#define VERIFY_PARAM_CONFORM_RANGE(lCurrentParam, lMaxParam, lMinParam, lStepParam) \
{\
    if(lStepParam != 0)\
    {\
        lCurrentParam = (lCurrentParam / lStepParam) * lStepParam;\
    }\
\
    if(lCurrentParam > lMaxParam)\
    {\
        lCurrentParam = lMaxParam;\
    }\
\
    if(lCurrentParam < lMinParam)\
    {\
        lCurrentParam = lMinParam;\
    }\
}\

///Operating device error code
typedef enum DEVICE_STATUS
{
    DEVICE_SUCCESS                          = 0,
    DEVICE_CREATE_ENUMERATOR_FAIL           = 1,
    DEVICE_ENUM_FAIL                        = 2,
    DEVICE_GET_INFO_FAIL                    = 3,
    DEVICE_BIND_FILTER_FAIL                 = 4,
    DEVICE_OPEN_FAIL                        = 5,
    DEVICE_CLOSE_FAIL                       = 6,
    DEVICE_START_ACQUISITION_FAIL           = 7,
    DEVICE_STOP_ACQUISITION_FAIL            = 8,
    DEVICE_GET_EXPOSURE_TIME_FAIL           = 9,
    DEVICE_GET_EXPOSURE_TIME_RANGE_FAIL     = 10,
    DEVICE_SET_EXPOSURE_TIME_FAIL           = 11,
    DEVICE_GET_GAIN_FAIL                    = 12,
    DEVICE_GET_GAIN_RANGE_FAIL              = 13,
    DEVICE_SET_GAIN_FAIL                    = 14,
    DEVICE_IS_BLACK_AND_WHITE_DEVICE        = 15,
    DEVICE_GET_WHITE_BALANCE_FAIL           = 16,
    DEVICE_GET_WHITE_BALANCE_REANGE_FAIL    = 17,
    DEVICE_SET_WHITE_BALANCE_FAIL           = 18,
    DEVICE_GET_FILTER_PROPERTY_FAIL         = 19,
    DEVICE_GET_PIN_PROPERTY_FAIL            = 20,
    DEVICE_GET_IMAGE_FAIL                   = 21,
    DEVICE_GET_PIXEL_SIZE_FAIL              = 22,
    DEVICE_GET_COLOR_FAIL                   = 23,
	DEVICE_SET_SAVE_IMAGE_FAIL              = 24,
}DEVICE_STATUS;

///Save exposure time range
typedef struct DEVICE_EXPOSURE_TIME_RANGE
{
    long lExposureTimeMax;
    long lExposureTimeMin;
    long lExposureTimeStep;
    long lExposureTimeDefault;
    long lControlFlags;
}DEVICE_EXPOSURE_TIME_RANGE;

///Save gain range
typedef struct DEVICE_GAIN_RANGE
{
    long lGainMax;
    long lGainMin;
    long lGainStep;
    long lGainDefault;
    long lControlFlags;
}DEVICE_GAIN_RANGE;

///Save white balance range
typedef struct DEVICE_WHITE_BALANCE_RANGE
{
    long lWhiteBalanceMax;
    long lWhiteBalanceMin;
    long lWhiteBalanceStep;
    long lWhiteBalanceDefault;
    long lControlFlags;
}DEVICE_WHITE_BALANCE_RANGE;

#endif

