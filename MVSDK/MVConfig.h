#include <iostream>
#include <windows.h>
#include "Demo/VC++/Include/CameraApi.h"

class MVCamera {
public:
    CameraHandle pCameraHandle;
    enum CameraProperties
    {
        CAMERA_GAIN,
        CAMERA_GAMMA,
        CAMERA_CONTRAST,
        CAMERA_SATURATION,
        CAMERA_SHARPNESS
    }eProperties;
    enum ExposureMode
    {
        MANUA,
        AUTO
    };

    int CamInit();
    int SetTriggerMode();
    int SetExposureMode(bool bMode);
    int SetExposureTime(double fExposureTime);
    int CameraConfig(CameraProperties eProperties, double fValue);
};