//------------------------------------------------------------------------
/**
\file		IGXInterface.h
\brief		Definition of the IGXInterface interface
\Date       2022-08-30
\Version    1.0.2208.9301

<p>Copyright (c) 2022-2022  China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>
*/
//------------------------------------------------------------------------
#pragma once
#include "GXIAPIBase.h"
#include "GalaxyException.h"
#include "GXSmartPtr.h"
#include "GXStringCPP.h"
#include "IGXDeviceInfo.h"
#include "IGXFeatureControl.h"
#include "IGXInterfaceInfo.h"
#include "GXDeviceInfoVector.h"

//--------------------------------------------
/**
\brief  Interface abstract class
*/
//---------------------------------------------
class GXIAPICPP_API IGXInterface
{

public:
    
    //---------------------------------------------------------
	/**
	\brief Destructor
	*/
	//---------------------------------------------------------
    virtual ~IGXInterface(){};
    
    //---------------------------------------------------------
    /**
    \brief Get interface info
    \return   interface info pointer
    */
    //---------------------------------------------------------
    virtual IGXInterfaceInfoPointer GetInterfaceInfo() = 0;
    
    //---------------------------------------------------------
    /**
    \brief Get all device information under the interface
    \param    vectorDeviceInfo[in|out]    device info list
    \return   void
    */
    //---------------------------------------------------------
    virtual void GetAllDeviceInfo( GxIAPICPP::gxdeviceinfo_vector& vectorDeviceInfo) = 0;
    
    //---------------------------------------------------------
    /**
    \brief Get interface feature control pointer
    \return   Feature control pointer
    */
    //---------------------------------------------------------
    virtual CGXFeatureControlPointer GetFeatureControl() = 0;
};

template class GXIAPICPP_API                GXSmartPtr<IGXInterface>;
typedef GXSmartPtr<IGXInterface>            CGXInterfacePointer;
