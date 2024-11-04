#pragma once

#include "Def.h"

#include <iostream>
#include <atlbase.h>

#include "DxEnumerator.h"

//--------------------------------------------
/**
\brief Device management class
*/
//---------------------------------------------
class CDeviceManager
{
public:
    ///Constructor
    CDeviceManager(void);
    ///Destructor
    virtual ~CDeviceManager(void);

    ///Enumerate registered devices
    int EnumDevice(void);
    ///Obtain device information based on device ID
    DEVICE_STATUS GetDeviceInfo(const int &nDevID, std::string &strDevName);

private:
    DEVICE_STATUS __IsDaHengDevice(const std::string &strDeviceName);
};

