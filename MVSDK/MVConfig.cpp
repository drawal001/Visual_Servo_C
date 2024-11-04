#include  "MVConfig.h"

int MVCamera::CamInit() {
    int ret = 0;
    //初始化SDK
    ret += CameraSdkInit(1);
    if (!ret) {
        std::cerr << "error: fail to init SDK, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    //枚举相机
    tSdkCameraDevInfo pDSCameraList[4];
    int piNums = 4;
    ret += CameraEnumerateDevice(pDSCameraList, &piNums);
    if (!ret || piNums == 0) {
        std::cerr << "error: could not find any cameras, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    //初始化相机
    ret += CameraInit(&pDSCameraList[0], -1, -1, &pCameraHandle);
    if(!ret || pCameraHandle == NULL) {
        std::cerr << "error: fail to init camera, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    ret += CameraSetTriggerMode(pCameraHandle, 0);
    if(!ret) {
        std::cerr << "error: fail to set continuous acquisition, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    return ret;
}

int MVCamera::SetTriggerMode() {
    int ret = 0;
    ret += CameraSetTriggerMode(pCameraHandle, 0);
    if (!ret)
    {
        std::cerr << "error: fail to set continuous acquisition, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    return ret;
}

int MVCamera::SetExposureMode(bool bMode) {
    int ret = 0;
    ret += CameraSetAeState(pCameraHandle, bMode);
    if (!ret) {
        std::cerr << "error: fail to set ExposureMpde, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    return ret;
}

int MVCamera::SetExposureTime(double fExposureTime) {
    int ret = 0;
    ret += CameraSetExposureTime(pCameraHandle, fExposureTime);
    if (!ret)
    {
        std::cerr << "error: fail to set ExposureTime, return CameraStatus: " << ret << std::endl;
        return -1;
    }
    return ret;
}

int MVCamera::CameraConfig(MVCamera::CameraProperties eProperties, double fValue) {
    int ret = 0;
    switch (eProperties)
    {
    case CAMERA_GAIN:
    {
        ret += CameraSetAnalogGain(pCameraHandle, (int)fValue);
        if (!ret)
        {
            std::cerr << "error: fail to set Gain, return CameraStatus: " << ret << std::endl;
        }
        break;
    }
    case CAMERA_GAMMA:
    {
        ret += CameraSetGamma(pCameraHandle, (int)fValue);
        if (!ret)
        {
            std::cerr << "error: fail to set Gamma, return CameraStatus: " << ret << std::endl;
        }
        break;
    }
    case CAMERA_CONTRAST:
    {
        ret += CameraSetAnalogGain(pCameraHandle, (int)fValue);
        if (!ret)
        {
            std::cerr << "error: fail to set Contrast, return CameraStatus: " << ret << std::endl;
        }
        break;
    }
        // CAMERA_CONTRAST,
        // CAMERA_SATURATION,
        // CAMERA_SHARPNESS
        default:
        break;
    }
    return ret;
}