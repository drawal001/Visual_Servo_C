//------------------------------------------------------------------------
/**
\file		IGXInterfaceInfo.h
\brief		The Interface enumeration process creates a list of CGXInterfaceInfo objects 
            (GxIAPICPP::gxinterfaceinfo_vector). Each CGXInterfaceInfo object stores the information of a Interface.
            The information is retrieved during the Interface enumeration process (IGXFactory::UpdateDeviceList)
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

//--------------------------------------------
/**
\brief  Interface info abstract class
*/
//---------------------------------------------
class GXIAPICPP_API IGXInterfaceInfo
{
public:
    //---------------------------------------------------------
	/**
	\brief Destructor
	*/
	//---------------------------------------------------------
	virtual ~IGXInterfaceInfo(){};
	
	//---------------------------------------------------------
	/**
	\brief Get interface type
	\return   Interface type
	*/
	//---------------------------------------------------------
	virtual GX_TL_TYPE_LIST GetType() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface vendor name
	\return   Interface vendor name
	*/
	//---------------------------------------------------------
	virtual GxIAPICPP::gxstring GetVendorName() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface model name
	\return   Interface model name
	*/
	//---------------------------------------------------------
	virtual GxIAPICPP::gxstring GetModelName() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface serial number
	\return   Interface serial number
	*/
	//---------------------------------------------------------
	virtual GxIAPICPP::gxstring GetSerialNumber() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface display name
	\return   Interface display name
	*/
	//---------------------------------------------------------
	virtual GxIAPICPP::gxstring GetDisplayName() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface ID (CXP)
	\return   Interface ID
	*/
	//---------------------------------------------------------
	virtual GxIAPICPP::gxstring GetInterfaceID() const = 0;
	
	//---------------------------------------------------------
	/**
	\brief Get interface init flag (CXP)
	\return   Interface init flag
	*/
	//---------------------------------------------------------
	virtual uint32_t GetInitFlag() const = 0;
};

template class GXIAPICPP_API                    GXSmartPtr<IGXInterfaceInfo>;
typedef GXSmartPtr<IGXInterfaceInfo>            IGXInterfaceInfoPointer;
