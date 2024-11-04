//------------------------------------------------------------------------
/**
\file       GXIAPIBase.h
\brief      Platform-dependent type definitions
\Date       2024-05-27
\Version    1.1.2405.9271
*/
//------------------------------------------------------------------------

#ifndef GXIAPI_BASE_H
#define GXIAPI_BASE_H

#if defined(_WIN32)
#ifndef _STDINT_H 
#ifdef _MSC_VER // Microsoft compiler
#if _MSC_VER < 1600
typedef __int8            int8_t;   ///< makes int 8 portable across different platforms
typedef __int16           int16_t;  ///< makes short portable across different platforms
typedef __int32           int32_t;  ///< makes __int32 portable across different platforms
typedef __int64           int64_t;  ///< makes int64 portable across different platforms
typedef unsigned __int8   uint8_t;  ///< makes unsigned char portable across different platforms
typedef unsigned __int16  uint16_t; ///< makes unsigned short portable across different platforms
typedef unsigned __int32  uint32_t; ///< makes unsigned int32 portable across different platforms
typedef unsigned __int64  uint64_t; ///< makes unsigned int64 portable across different platforms
#else
// stdint.h is already included in Visual Studio 2010 
#include <stdint.h>
#endif
#else
// Not a Microsoft compiler
#include <stdint.h>
#endif
#endif 
#else
// Linux
#include <stdint.h>
#endif

#ifdef GXIAPICPP_EXPORTS
#define GXIAPICPP_API __declspec(dllexport)
#else
#define GXIAPICPP_API __declspec(dllimport)
#endif

//------------------------------------------------------------------------------
//  The error codes list
//------------------------------------------------------------------------------
typedef enum GX_STATUS_LIST
{
    GX_STATUS_SUCCESS                =  0,           ///< Operation was successful; no error occurred.
    GX_STATUS_ERROR                  = -1,           ///< Unspecified runtime error.
    GX_STATUS_NOT_FOUND_TL           = -2,           ///< Special error code for initialize, when IGXFactory::Init was called.
    GX_STATUS_NOT_FOUND_DEVICE       = -3,           ///< Special error code for opening device, when IGXFactory::OpenDeviceByxxx was called.
    GX_STATUS_OFFLINE                = -4,           ///< Communication error has occurred; e.g. a read or write operation failed.
    GX_STATUS_INVALID_PARAMETER      = -5,           ///< One of the parameters given was not valid or out of range.
    GX_STATUS_INVALID_HANDLE         = -6,           ///< Given handle does not support the operation; e.g. function call on wrong handle or NULL pointer.
    GX_STATUS_INVALID_CALL           = -7,           ///< Requested operation is not allowed, special for interface; e.g. callback capture and the getimage can not be used at the same time.
    GX_STATUS_INVALID_ACCESS         = -8,           ///< Requested operation is not allowed, special for camera; e.g. a remote device has be opened by another client.
    GX_STATUS_NEED_MORE_BUFFER       = -9,           ///< A provided buffer is too small to receive the expected amount of data.
    GX_STATUS_ERROR_TYPE             = -10,          ///< Special error code for IGXFeatureControl::GetxxxFeature(); e.g. code like this "GetIntFeature("Gain")" is wrong, because "Gain" is an fload feature.
    GX_STATUS_OUT_OF_RANGE           = -11,          ///< A given value is out of range; e.g. IIntFeautre::SetValue() had been set an value bigger than the Max Value.
    GX_STATUS_NOT_IMPLEMENTED        = -12,          ///< Requested operation is not implemented; e.g. no color feature on Mono Camera.
    GX_STATUS_NOT_INIT_API           = -13,          ///< A module or resource is not initialized; e.g. IGXFactory::Init was not called.
    GX_STATUS_TIMEOUT                = -14,          ///< An operation's timeout time expired before it could be completed.     
}GX_STATUS_LIST;
typedef int32_t GX_STATUS;

//------------------------------------------------------------------------------
//  Used for open device interface; IGXFactory::OpenDeviceByxxx
//------------------------------------------------------------------------------
typedef enum GX_ACCESS_MODE
{
    GX_ACCESS_READONLY      =2,        ///< Open a device read only. All Port functions can only read from the device.
    GX_ACCESS_CONTROL       =3,        ///< Open a device in a way that other hosts/processes can have read only access to the device. Device's access level is read/write for this process.
    GX_ACCESS_EXCLUSIVE     =4,        ///< Open a device in a way that only this host/process can have access to the device. Device's access level is read/write for this process.
}GX_ACCESS_MODE;
typedef int32_t GX_ACCESS_MODE_CMD;

//------------------------------------------------------------------------------
//  Used for CGXDeviceInfo
//------------------------------------------------------------------------------
typedef enum  GX_DEVICE_CLASS_LIST
{
    GX_DEVICE_CLASS_UNKNOWN = 0,     ///< Not used in a command. Can be used to initialize a variable to query that information
    GX_DEVICE_CLASS_USB2    = 1,     ///< USB2.0 Cameras
    GX_DEVICE_CLASS_GEV     = 2,     ///< GigE Vision Cameras
    GX_DEVICE_CLASS_U3V     = 3,     ///< USB3.0 Cameras
    GX_DEVICE_CLASS_SMART   = 4,     ///< Smart Cameras
    GX_DEVICE_CLASS_CXP     = 5,     ///< CXP Cameras
}GX_DEVICE_CLASS_LIST;
typedef  int32_t GX_DEVICE_CLASS;

//------------------------------------------------------------------------------
//  Used for CGXInterfaceInfo
//------------------------------------------------------------------------------
typedef enum GX_TL_TYPE_LIST
{
    GX_TL_TYPE_UNKNOWN  = 0,     ///< Not used in a command. Can be used to initialize a variable to query that information
    GX_TL_TYPE_USB      = 1,     ///< USB2.0
    GX_TL_TYPE_GEV      = 2,     ///< GEV
    GX_TL_TYPE_U3V      = 4,     ///< U3V
    GX_TL_TYPE_CXP      = 8,     ///< CXP
}GX_TL_TYPE_LIST;
typedef int32_t GX_TL_TYPE;

//------------------------------------------------------------------------------
//  Used for CGXDeviceInfo
//------------------------------------------------------------------------------
typedef enum  GX_ACCESS_STATUS
{
    GX_ACCESS_STATUS_UNKNOWN    = 0,   ///< Not used in a command. Can be used to initialize a variable to query that information
    GX_ACCESS_STATUS_READWRITE  = 1,   ///< Device can be read and write
    GX_ACCESS_STATUS_READONLY   = 2,   ///< Device can only be read
    GX_ACCESS_STATUS_NOACCESS   = 3,   ///< Device no access
}GX_ACCESS_STATUS;
typedef int32_t GX_ACCESS_STATUS_CMD;

//------------------------------------------------------------------------------
//  Used for IImageData::GetStatus 
//------------------------------------------------------------------------------
typedef enum GX_FRAME_STATUS_LIST
{
    GX_FRAME_STATUS_SUCCESS          = 0,     ///< Flag to indicate that a buffer was filled completed.
    GX_FRAME_STATUS_INCOMPLETE       = -1,    ///< Flag to indicate that a buffer was filled but an error occurred during that process.
}GX_FRAME_STATUS_LIST;
typedef  int32_t  GX_FRAME_STATUS;

//------------------------------------------------------------------------------
//  IP Confingure Mode
//------------------------------------------------------------------------------
enum GX_IP_CONFIGURE_MODE_LIST
{
    GX_IP_CONFIGURE_DHCP       = 6,   ///< Enable DHCP
    GX_IP_CONFIGURE_LLA        = 4,   ///< LLA
    GX_IP_CONFIGURE_STATIC_IP  = 5,   ///< Static IP
    GX_IP_CONFIGURE_DEFAULT    = 7,   ///< Default
};
typedef int32_t GX_IP_CONFIGURE_MODE;

//------------------------------------------------------------------------------
//  Used for IGXFeatureControl::GetFeatureType 
//------------------------------------------------------------------------------
typedef enum GX_FEATURE_TYPE
{
    GX_FEATURE_INT                 =0x10000000,  ///< Int feature
    GX_FEATURE_FLOAT               =0X20000000,  ///< Float feature
    GX_FEATURE_ENUM                =0x30000000,  ///< Enum feature
    GX_FEATURE_BOOL                =0x40000000,  ///< Bool feature
    GX_FEATURE_STRING              =0x50000000,  ///< String feature
    GX_FEATURE_BUFFER              =0x60000000,  ///< Register feature
    GX_FEATURE_COMMAND             =0x70000000,  ///< Command feature
}GX_FEATURE_TYPE;

#define GX_PIXEL_MONO                  ( 0x01000000 )
#define GX_PIXEL_COLOR                 ( 0x02000000 )

#define GX_PIXEL_8BIT                  ( 0x00080000 )
#define GX_PIXEL_10BIT                 ( 0x000A0000 )
#define GX_PIXEL_12BIT                 ( 0x000C0000 )
#define GX_PIXEL_14BIT                 ( 0x000E0000 )
#define GX_PIXEL_16BIT                 ( 0x00100000 )
#define GX_PIXEL_24BIT                 ( 0x00180000 )
#define GX_PIXEL_30BIT                 ( 0x001E0000 )
#define GX_PIXEL_32BIT                 ( 0x00200000 )
#define GX_PIXEL_36BIT                 ( 0x00240000 )
#define GX_PIXEL_48BIT                 ( 0x00300000 )
#define GX_PIXEL_64BIT                 ( 0x00400000 )
#define GX_PIXEL_96BIT                 ( 0x00600000 )

//------------------------------------------------------------------------------
//  Used for IImageData::GetPixelFormat 
//------------------------------------------------------------------------------
#ifndef GX_PIXEL_FORMAT_ENTRY_DEFINED
#define GX_PIXEL_FORMAT_ENTRY_DEFINED
typedef enum GX_PIXEL_FORMAT_ENTRY
{
 GX_PIXEL_FORMAT_UNDEFINED          = (0),
    GX_PIXEL_FORMAT_MONO8              = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0001),//0x1080001,
    GX_PIXEL_FORMAT_MONO8_SIGNED       = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0002),//0x1080002,
    GX_PIXEL_FORMAT_MONO10             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0003),//0x1100003,
    GX_PIXEL_FORMAT_MONO10_P           = (GX_PIXEL_MONO  | GX_PIXEL_10BIT | 0x0046),//0x010A0046,
    GX_PIXEL_FORMAT_MONO12             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0005),//0x1100005,
    GX_PIXEL_FORMAT_MONO12_P           = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0047),//0x010C0047,
    GX_PIXEL_FORMAT_MONO14             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0025),//0x1100025,
    GX_PIXEL_FORMAT_MONO14_P           = (GX_PIXEL_MONO  | GX_PIXEL_14BIT | 0x0104),//0x010E0104,
    GX_PIXEL_FORMAT_MONO16             = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0007),//0x1100007,

    GX_PIXEL_FORMAT_BAYER_GR8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0008),//0x1080008,               
    GX_PIXEL_FORMAT_BAYER_RG8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x0009),//0x1080009,                
    GX_PIXEL_FORMAT_BAYER_GB8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x000A),//0x108000A,
    GX_PIXEL_FORMAT_BAYER_BG8          = (GX_PIXEL_MONO  | GX_PIXEL_8BIT  | 0x000B),//0x108000B,

    GX_PIXEL_FORMAT_BAYER_GR10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000C),//0x110000C, 
    GX_PIXEL_FORMAT_BAYER_GR10_P       = (GX_PIXEL_MONO  | GX_PIXEL_10BIT | 0x0056),//0x010A0056,
    GX_PIXEL_FORMAT_BAYER_RG10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000D),//0x110000D,
    GX_PIXEL_FORMAT_BAYER_RG10_P       = (GX_PIXEL_MONO  | GX_PIXEL_10BIT | 0x0058),//0x010A0058,
    GX_PIXEL_FORMAT_BAYER_GB10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000E),//0x110000E,
    GX_PIXEL_FORMAT_BAYER_GB10_P       = (GX_PIXEL_MONO  | GX_PIXEL_10BIT | 0x0054),//0x010A0054,
    GX_PIXEL_FORMAT_BAYER_BG10         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x000F),//0x110000F,
    GX_PIXEL_FORMAT_BAYER_BG10_P       = (GX_PIXEL_MONO  | GX_PIXEL_10BIT | 0x0052),//0x010A0052,  

    GX_PIXEL_FORMAT_BAYER_GR12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0010),//0x1100010,  
    GX_PIXEL_FORMAT_BAYER_GR12_P       = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0057),//0x010C0057,
    GX_PIXEL_FORMAT_BAYER_RG12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0011),//0x1100011,
    GX_PIXEL_FORMAT_BAYER_RG12_P       = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0059),//0x010C0059,
    GX_PIXEL_FORMAT_BAYER_GB12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0012),//0x1100012,
    GX_PIXEL_FORMAT_BAYER_GB12_P       = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0055),//0x010C0055,
    GX_PIXEL_FORMAT_BAYER_BG12         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0013),//0x1100013,
    GX_PIXEL_FORMAT_BAYER_BG12_P       = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0053),//0x010C0053,   

    GX_PIXEL_FORMAT_BAYER_GR14         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0109),//0x01100109, 
    GX_PIXEL_FORMAT_BAYER_GR14_P       = (GX_PIXEL_MONO  | GX_PIXEL_14BIT | 0x0105),//0x010E0105,
    GX_PIXEL_FORMAT_BAYER_RG14         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x010A),//0x0110010A, 
    GX_PIXEL_FORMAT_BAYER_RG14_P       = (GX_PIXEL_MONO  | GX_PIXEL_14BIT | 0x0106),//0x010E0106,
    GX_PIXEL_FORMAT_BAYER_GB14         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x010B),//0x0110010B,  
    GX_PIXEL_FORMAT_BAYER_GB14_P       = (GX_PIXEL_MONO  | GX_PIXEL_14BIT | 0x0107),//0x010E0107,
    GX_PIXEL_FORMAT_BAYER_BG14         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x010C),//0x0110010C,
    GX_PIXEL_FORMAT_BAYER_BG14_P       = (GX_PIXEL_MONO  | GX_PIXEL_14BIT | 0x0108),//0x010E0108, 

    GX_PIXEL_FORMAT_BAYER_GR16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x002E),//0x110002E,                
    GX_PIXEL_FORMAT_BAYER_RG16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x002F),//0x110002F,
    GX_PIXEL_FORMAT_BAYER_GB16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0030),//0x1100030,
    GX_PIXEL_FORMAT_BAYER_BG16         = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x0031),//0x1100031,

    GX_PIXEL_FORMAT_RGB8_PLANAR        = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0021),//0x2180021,
    GX_PIXEL_FORMAT_RGB10_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0022),//0x2300022,
    GX_PIXEL_FORMAT_RGB12_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0023),//0x2300023,
    GX_PIXEL_FORMAT_RGB16_PLANAR       = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0024),//0x2300024,

    GX_PIXEL_FORMAT_RGB8               = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0014),//0x2180014,
    GX_PIXEL_FORMAT_RGB10              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0018),//0x2300018,
    GX_PIXEL_FORMAT_RGB12              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x001A),//0x230001A,
    GX_PIXEL_FORMAT_RGB14              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x005E),//0x230005E,
    GX_PIXEL_FORMAT_RGB16              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0033),//0x2300033,

    GX_PIXEL_FORMAT_BGR8               = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0015),//0x2180015,
    GX_PIXEL_FORMAT_BGR10              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x0019),//0x2300019,
    GX_PIXEL_FORMAT_BGR12              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x001B),//0x230001B,
    GX_PIXEL_FORMAT_BGR14              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x004A),//0x230004A,
    GX_PIXEL_FORMAT_BGR16              = (GX_PIXEL_COLOR | GX_PIXEL_48BIT | 0x004B),//0x230004B,

    GX_PIXEL_FORMAT_RGBA8              = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0016),//0x2200016,
    GX_PIXEL_FORMAT_BGRA8              = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0017),//0x2200017,
    GX_PIXEL_FORMAT_ARGB8              = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0018),//0x2200018,    The standard agreement is undefined   
    GX_PIXEL_FORMAT_ABGR8              = (GX_PIXEL_COLOR | GX_PIXEL_32BIT | 0x0019),//0x2200019,    The standard agreement is undefined  

    GX_PIXEL_FORMAT_R8                 = (GX_PIXEL_MONO | GX_PIXEL_8BIT | 0x00C9),//0x010800C9,
    GX_PIXEL_FORMAT_G8                 = (GX_PIXEL_MONO | GX_PIXEL_8BIT | 0x00CD),//0x010800CD,
    GX_PIXEL_FORMAT_B8                 = (GX_PIXEL_MONO | GX_PIXEL_8BIT | 0x00D1),//0x010800D1,

    GX_PIXEL_FORMAT_COORD3D_ABC32F           = (GX_PIXEL_COLOR | GX_PIXEL_96BIT | 0X00C0),//0x021800C0,     3D coordinate A-B-C 32-bit floating point 
    GX_PIXEL_FORMAT_COORD3D_ABC32F_PLANAR    = (GX_PIXEL_COLOR | GX_PIXEL_96BIT | 0X00C1),//0x021800C1,     3D coordinate A-B-C 32-bit floating point planar
    GX_PIXEL_FORMAT_COORD3D_C16              = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0x00B8),//0x011000B8,     3D coordinate C 16-bit
    GX_PIXEL_FORMAT_COORD3D_C16_I16          = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0xFF02),//0x0110FF02,     Vendor-defined format
    GX_PIXEL_FORMAT_COORD3D_C16_S16          = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0xFF03),//0x0110FF03,     Vendor-defined format
    GX_PIXEL_FORMAT_COORD3D_C16_I16_S16      = (GX_PIXEL_MONO  | GX_PIXEL_16BIT | 0xFF04),//0x0110FF04,     Vendor-defined format
    
    GX_PIXEL_FORMAT_YUV444_8           = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0020),//0x2180020,
    GX_PIXEL_FORMAT_YUV422_8           = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x0032),//0x2100032,
    GX_PIXEL_FORMAT_YUV411_8           = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x001E),//0x20C001E,
    GX_PIXEL_FORMAT_YUV420_8_PLANAR    = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x0040),//0x20C0040,    The standard agreement is undefined 

    GX_PIXEL_FORMAT_YCBCR444_8         = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x005B),//0x218005B,
    GX_PIXEL_FORMAT_YCBCR422_8         = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x003B),//0x210003B,
    GX_PIXEL_FORMAT_YCBCR411_8         = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x005A),//0x20C005A,

    GX_PIXEL_FORMAT_YCBCR601_444_8     = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x003D),//0x218003D,
    GX_PIXEL_FORMAT_YCBCR601_422_8     = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x003E),//0x210003E,
    GX_PIXEL_FORMAT_YCBCR601_411_8     = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x003F),//0x20C003F,

    GX_PIXEL_FORMAT_YCBCR709_444_8     = (GX_PIXEL_COLOR | GX_PIXEL_24BIT | 0x0040),//0x2180040,
    GX_PIXEL_FORMAT_YCBCR709_422_8     = (GX_PIXEL_COLOR | GX_PIXEL_16BIT | 0x0041),//0x2100041,
    GX_PIXEL_FORMAT_YCBCR709_411_8     = (GX_PIXEL_COLOR | GX_PIXEL_12BIT | 0x0042),//0x20C0042,

    GX_PIXEL_FORMAT_MONO10_PACKED      = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0004),//0x010C0004,   GigE Vision specific format
    GX_PIXEL_FORMAT_MONO12_PACKED      = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0006),//0x010C0006,   GigE Vision specific format

    GX_PIXEL_FORMAT_BAYER_BG10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0029),//0x010C0029,   GigE Vision specific format
    GX_PIXEL_FORMAT_BAYER_BG12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002D),//0x010C002D,   GigE Vision specific format

    GX_PIXEL_FORMAT_BAYER_GB10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0028),//0x010C0028,   GigE Vision specific format
    GX_PIXEL_FORMAT_BAYER_GB12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002C),//0x010C002C,   GigE Vision specific format

    GX_PIXEL_FORMAT_BAYER_GR10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0026),//0x010C0026,   GigE Vision specific format
    GX_PIXEL_FORMAT_BAYER_GR12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002A),//0x010C002A,   GigE Vision specific format

    GX_PIXEL_FORMAT_BAYER_RG10_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x0027),//0x010C0027,   GigE Vision specific format
    GX_PIXEL_FORMAT_BAYER_RG12_PACKED  = (GX_PIXEL_MONO  | GX_PIXEL_12BIT | 0x002B),//0x010C002B,   GigE Vision specific format
}GX_PIXEL_FORMAT_ENTRY;
#endif

//------------------------------------------------------------------------------
//  Image interpolation method; Used for IImageProcessConfig::SetConvertType 
//------------------------------------------------------------------------------
typedef enum  GX_BAYER_CONVERT_TYPE_LIST
{
    GX_RAW2RGB_NEIGHBOUR  = 0,
    GX_RAW2RGB_ADAPTIVE   = 1,
    GX_RAW2RGB_NEIGHBOUR3 = 2
}GX_BAYER_CONVERT_TYPE_LIST;

//------------------------------------------------------------------------------
//  Image valid bit; Used for IImageData::ConvertToRaw8/IImageData::ConvertToRGB24/IImageProcessConfig.SetValidBit 
//------------------------------------------------------------------------------
typedef enum  GX_VALID_BIT_LIST
{
    GX_BIT_0_7      = 0,    ///< bit 0~7
    GX_BIT_1_8      = 1,    ///< bit 1~8
    GX_BIT_2_9      = 2,    ///< bit 2~9
    GX_BIT_3_10     = 3,    ///< bit 3~10
    GX_BIT_4_11     = 4,    ///< bit 4~11
    GX_BIT_5_12     = 5,    ///< bit 5~12
    GX_BIT_6_13     = 6,    ///< bit 6~13
    GX_BIT_7_14     = 7,    ///< bit 7~14
    GX_BIT_8_15     = 8     ///< bit 8~15
}GX_VALID_BIT_LIST;


//------------------------------------------------------------------------------
//  Reset Device Mode
//------------------------------------------------------------------------------
typedef enum GX_RESET_DEVICE_MODE
{
    GX_MANUFACTURER_SPECIFIC_RECONNECT   = 0x1,    ///< reconnect Device
    GX_MANUFACTURER_SPECIFIC_RESET     = 0x2     ///< reset Device 
}GX_RESET_DEVICE_MODE;

//------------------------------------------------------------------------------
//  color transform factor
//  Valid range: -4.0 ~ 4.0
//------------------------------------------------------------------------------
#ifndef COLOR_TRANSFORM_FACTOR_DEFINED
#define COLOR_TRANSFORM_FACTOR_DEFINED
typedef  struct  COLOR_TRANSFORM_FACTOR
{
    float fGain00;          ///< red    contribution to the red   pixel   (multiplicative factor)
    float fGain01;          ///< green  contribution to the red   pixel   (multiplicative factor)
    float fGain02;          ///< blue   contribution to the red   pixel   (multiplicative factor)

    float fGain10;          ///< red   contribution to the green pixel   (multiplicative factor)
    float fGain11;          ///< green contribution to the green pixel   (multiplicative factor)
    float fGain12;          ///< blue  contribution to the green pixel   (multiplicative factor)

    float fGain20;          ///< red   contribution to the blue  pixel   (multiplicative factor)
    float fGain21;          ///< green contribution to the blue  pixel   (multiplicative factor)
    float fGain22;          ///< blue  contribution to the blue  pixel   (multiplicative factor)
} COLOR_TRANSFORM_FACTOR;
#endif

typedef enum GX_LOG_TYPE_LIST
{
    GX_LOG_TYPE_OFF   = 0x00000000,     ///< All types are not sent
    GX_LOG_TYPE_FATAL = 0x00000001,     ///< log type: FATAL
    GX_LOG_TYPE_ERROR = 0x00000010,     ///< log type: ERROR
    GX_LOG_TYPE_WARN  = 0x00000100,     ///< log type: WARN
    GX_LOG_TYPE_INFO  = 0x00001000,     ///< log type: INFO
    GX_LOG_TYPE_DEBUG = 0x00010000,     ///< log type: DEBUG
    GX_LOG_TYPE_TRACE = 0x00100000,     ///< log type: TRACE
}GX_LOG_TYPE_LIST;
typedef uint32_t GX_LOG_TYPE;

//------------------------------------------------------------------------------
//  Call back handles
//------------------------------------------------------------------------------
typedef void* GX_DEVICE_OFFLINE_CALLBACK_HANDLE;    ///< Retrieve from IGXDevice::RegisterDeviceOfflineCallback 
typedef void* GX_FEATURE_CALLBACK_HANDLE;           ///< Retrieve from IGXFeatureControl::RegisterFeatureCallback 

//------------------------------------------------------------------------------
//  Batch operation register structure
//------------------------------------------------------------------------------
typedef struct GX_REGISTER_STACK_ENTRY
{
    uint64_t   nAddress;        ///> Address of the register
    void*      pBuffer;         ///> Pointer to the buffer containing the data
    size_t     nSize;           ///> Number of bytes to read
} GX_REGISTER_STACK_ENTRY;


//------------------------------------------------------------------------------
//  Basic image data structure
//------------------------------------------------------------------------------
typedef struct GX_IMAGE_INFO
{
    void    *pBuffer;                  ///< Pointer to the buffer containing the data
    uint16_t nWidth;                   ///< Image width
    uint16_t nHeight;                  ///< Image height
    GX_PIXEL_FORMAT_ENTRY emSrcFormat; ///< Pixel format
}GX_IMAGE_INFO;

/// Bayer layout
typedef enum  GX_PIXEL_COLOR_FILTER
{
    GX_NONE    = 0,   
    GX_BAYERRG = 1,   
    GX_BAYERGB = 2,   
    GX_BAYERGR = 3,   
    GX_BAYERBG = 4    
} GX_PIXEL_COLOR_FILTER;

/// image actual bits
typedef enum GX_ACTUAL_BITS
{
    GX_ACTUAL_BITS_8  = 8,     ///< 8bit
    GX_ACTUAL_BITS_10 = 10,    ///< 10bit
    GX_ACTUAL_BITS_12 = 12,    ///< 12bit
    GX_ACTUAL_BITS_14 = 14,    ///< 14bit
    GX_ACTUAL_BITS_16 = 16     ///< 16bit
} GX_ACTUAL_BITS;


/// static defect pixel correction struct 
typedef  struct  GX_STATIC_DEFECT_CORRECTION
{
    uint32_t                ui32ImgWidth;   
    uint32_t                ui32ImgHeight;               
    uint32_t                ui32ImgOffsetX;               
    uint32_t                ui32ImgOffsetY;
    uint32_t                ui32ImgWidthMax;
    GX_PIXEL_COLOR_FILTER   emBayerType;
    GX_ACTUAL_BITS          emActualBits;

} GX_STATIC_DEFECT_CORRECTION;

#endif //GXIAPI_BASE_H