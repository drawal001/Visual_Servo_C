//------------------------------------------------------------------------
/**
\file		IGXFactory.h
\brief		Definition of IGXFactory class. Initialize library, enumerates and open devices.
\Date       2024-05-21
\Version    1.1.2405.9211
*/
//------------------------------------------------------------------------
#pragma once
#include "GXIAPIBase.h"
#include "GXSmartPtr.h"
#include "IGXDevice.h"
#include "IGXDeviceInfo.h"
#include "IGXInterface.h"
#include "IGXInterfaceInfo.h"
#include "GXStringCPP.h"
#include "GXDeviceInfoVector.h"
#include "IGXImageFormatConvert.h"
#include "IGXImageSpecialFunction.h"
#include "GXInterfaceInfoVector.h"
#include "IGXImageProcess.h"

class GXIAPICPP_API IGXFactory
{
protected:

	///Constructor
	IGXFactory(){}

	///Destructor
	virtual ~IGXFactory(){}

public:
	//----------------------------------------------------------------------------------
	/**
	\brief     Get the static instance object of IGXFactory
	\return    The static instance object of IGXFactory
	*/
	//----------------------------------------------------------------------------------
	static IGXFactory& GetInstance();

	//----------------------------------------------------------------------------------
	/**
	\brief    Initializes the GxIAPICPP runtime system
	You must call Init before calling any other functions.
	When finished you must call Uninit to free up all resources used by GxIAPICPP.
	\return   void
	*/
	//----------------------------------------------------------------------------------
	virtual void Init() = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Frees resources allocated by the GxIAPICPP runtime system.
	\return   void
	*/
	//----------------------------------------------------------------------------------
	virtual void Uninit() = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Retrieve a list of available devices. If GigE cameras are connected, this can enumerate all GigE camera devices in the same subnet. 
	\param    nTimeout[in]        Timeout for the waits in ms.
	\param    vectorDeviceInfo[int,out] A list of available devices
	\return   void
	*/
	//----------------------------------------------------------------------------------
	virtual void UpdateDeviceList(uint32_t nTimeout, GxIAPICPP::gxdeviceinfo_vector& vectorDeviceInfo) = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Retrieve a list of available devices. If GigE cameras are connected, this can enumerate all GigE camera devices in all subnets. 
	\param    nTimeout[in]        Timeout for the waits in ms.
	\param    vectorDeviceInfo[int,out] A list of available devices
	\return   void
	*/
	//----------------------------------------------------------------------------------
	virtual void UpdateAllDeviceList(uint32_t nTimeout, GxIAPICPP::gxdeviceinfo_vector& vectorDeviceInfo) = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Open device by IP address, if present
	\param    strIP[in]        IP address
	\param    emAccessMode[in] The access mode
	\return   A smart pointer object of IGXDevice
	*/
	//----------------------------------------------------------------------------------
	virtual CGXDevicePointer OpenDeviceByIP(const GxIAPICPP::gxstring& strIP, GX_ACCESS_MODE emAccessMode) = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Open device by MAC address, if present
	\param    strIP[in]        MAC address
	\param    emAccessMode[in] The access mode
	\return   A smart pointer object of IGXDevice
	*/
	//----------------------------------------------------------------------------------
	virtual CGXDevicePointer OpenDeviceByMAC(const GxIAPICPP::gxstring& strMAC, GX_ACCESS_MODE emAccessMode) = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Open device by serials number
	\param    strIP[in]        Serials number
	\param    emAccessMode[in] The access mode
	\return   A smart pointer object of IGXDevice
	*/
	//----------------------------------------------------------------------------------
	virtual CGXDevicePointer OpenDeviceBySN(const GxIAPICPP::gxstring& strSN, GX_ACCESS_MODE emAccessMode) = 0;

	//----------------------------------------------------------------------------------
	/**
	\brief    Open device by user defined name, if present
	\param    strIP[in]        The user defined name
	\param    emAccessMode[in] The access mode
	\return   A smart pointer object of IGXDevice
	*/
	//----------------------------------------------------------------------------------
	virtual CGXDevicePointer OpenDeviceByUserID(const GxIAPICPP::gxstring& strUserID, GX_ACCESS_MODE emAccessMode) = 0;

	// ---------------------------------------------------------------------------
		/**
		\brief       Set Device's Static IP Address
		\param[in]   strDevcieMacAddress   Device's MAC Address
		\param[in]   emIpConfigMode        IP Configure Mode(Static IP/DHCP/LLA)
		\param[in]   strIPAddress          IP Address
		\param[in]   strSubnetMask         Subnet Mask
		\param[in]   strDefaultGateway     Default Gateway
		\param[in]   strUserID             User's Name

		\retrun     void
		*/
       // ---------------------------------------------------------------------------
		virtual void GigEIpConfiguration(const GxIAPICPP::gxstring& strDeviceMacAddress, 
			GX_IP_CONFIGURE_MODE emIpConfigMode,
			const GxIAPICPP::gxstring& strIpAddress,
			const GxIAPICPP::gxstring& strSubnetMask, 
			const GxIAPICPP::gxstring& strDefaultGateway,
			const GxIAPICPP::gxstring& strUserID) = 0;

		// ---------------------------------------------------------------------------
		/**
		\brief       ForceIP
		\param[in]   strDevcieMacAddress  Device's MAC Address
		\param[in]   strIPAddress         Ip Address
		\param[in]   strSubnetMask        Subnet Mask
		\param[in]   strDefaultGateway    Default Gateway

		\retrun      void
		*/
        // ---------------------------------------------------------------------------
		virtual void GigEForceIp(const GxIAPICPP::gxstring& pszDeviceMacAddress, 
			const GxIAPICPP::gxstring& strIpAddress,
			const GxIAPICPP::gxstring& strSubnetMask, 
			const GxIAPICPP::gxstring& strDefaultGateway) = 0;

		// ---------------------------------------------------------------------------
		/**
		\brief       reset/reconnect Device
		\param[in]   pszDevcieMacAddress  Device's MAC Address
		\param[in]   ui32FeatureInfo      reset Mode

		\retrun      void
		*/
		// ---------------------------------------------------------------------------
		virtual void GigEResetDevice(const GxIAPICPP::gxstring& strDeviceMacAddress, GX_RESET_DEVICE_MODE ui32FeatureInfo) = 0;
		
		// ---------------------------------------------------------------------------
		/**
		\brief       create new convert pointer
		\retrun      A smart pointer object of IGXImageFormatConvert
		*/
		// ---------------------------------------------------------------------------
		virtual CGXImageFormatConvertPointer CreateImageFormatConvert() = 0;
		
		// ---------------------------------------------------------------------------
		/**
		\brief       create new specail function pointer
		\retrun      A smart pointer object of IGXImageSpecialFunction
		*/
		// ---------------------------------------------------------------------------
		virtual CGXImageSpecialFunctionPointer CreateImageSpecialFunction() = 0;
		
		//----------------------------------------------------------------------------------
	    /**
	    \brief    Retrieve a list of available devices. If GigE cameras are connected, this can enumerate all GigE camera devices in all subnets. 
	    \param    u32DeviceType[in]   update device type
	    \param    nTimeout[in]        Timeout for the waits in ms.
	    \param    vectorDeviceInfo[int,out] A list of available devices
	    \return   void
	    */
	    //----------------------------------------------------------------------------------
	    virtual void UpdateAllDeviceListEx( uint32_t nTLType, uint32_t nTimeout, GxIAPICPP::gxdeviceinfo_vector& vectorDeviceInfo) = 0;

        //----------------------------------------------------------------------------------
	    /**
	    \brief    Get interface info,call enumeration interface first
	    \param    vectorInterfaceInfo[in]   interface list
	    \return   void
	    */
	    //----------------------------------------------------------------------------------
        virtual void GetAllInterfaceInfo(GxIAPICPP::gxinterfaceinfo_vector& vectorInterfaceInfo) = 0;
        
        //----------------------------------------------------------------------------------
	    /**
	    \brief    Get interface by index,call enumeration interface first
	    \param    nIndex[in]   interface index
	    \return   A smart pointer object of IGXInterface
	    */
	    //----------------------------------------------------------------------------------
        virtual CGXInterfacePointer GetInterface(const uint32_t nIndex) = 0;

		//----------------------------------------------------------------------------------
		/**
		\brief    Create new IGXImageProcess pointer
		\return   IGXImageProcess pointer
		*/
		//----------------------------------------------------------------------------------
		virtual CGXImageProcessPointer CreateImageProcess() = 0;

        //----------------------------------------------------------------------------------
        /**
        \brief      Set whether logs of the specified type can be sent
        \param      [in]ui32LogType   log type: GX_LOG_TYPE_FATAL|GX_LOG_TYPE_ERROR|GX_LOG_TYPE_INFO
                                                GX_LOG_TYPE_OFF: all cannot be sent
        \return
        */
        //----------------------------------------------------------------------------------
        virtual void SetLogType(const uint32_t ui32LogType) = 0;

        //----------------------------------------------------------------------------------
        /**
        \brief      Gets whether logs of a specified type can be sent
        \param      [out]ui32Value  log type value: FATAL|ERROR|WARN|INFO|DEBUG|TRACE
        \return
        */
        //----------------------------------------------------------------------------------
        virtual void GetLogType(uint32_t& ui32Value) = 0;

private:
	IGXFactory& operator=(const IGXFactory&);
	IGXFactory(const IGXFactory&);
};

