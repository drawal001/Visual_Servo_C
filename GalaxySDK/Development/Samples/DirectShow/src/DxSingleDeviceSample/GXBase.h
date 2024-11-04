//------------------------------------------------------------------------
/**
\file		GXBase.h
\brief		Platform-dependent type definitions
\Date       2019-12-27
\Version    1.1.1912.9271
*/
//------------------------------------------------------------------------

#ifndef GXIAPI_BASE_H
#define GXIAPI_BASE_H

#include <map>
#include <windows.h>
#include <stdio.h>
#include <initguid.h>

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

#define GX_INFO_LENGTH_64_BYTE  (64)        ///< 64 Byte

GUID DECLSPEC_SELECTANY CLSID_VirtualCam = { 0x12b42c2, 0x30b4, 0x4745, { 0x92, 0x5e, 0xce, 0x80, 0xc6, 0xed, 0x66, 0xac } };

GUID DECLSPEC_SELECTANY CLSID_Filter_FirstProp = { 0x891fcd76, 0xd295, 0x4624, { 0xb5, 0x99, 0x2e, 0xae, 0x4b, 0x2, 0x5d, 0xe8} };

GUID DECLSPEC_SELECTANY CLSID_Filter_SecondProp = { 0x2a7b5c89, 0xf96c, 0x4ad7, { 0x92, 0x9f, 0x40, 0x2d, 0xf2, 0x7c, 0xeb, 0x28} };

GUID DECLSPEC_SELECTANY CLSID_PIN_FirstProp = { 0x765e5cf9, 0xdf3f, 0x4230, { 0xb7, 0x50, 0xe1, 0x6a, 0x3e, 0x6, 0x12, 0xfa} };

typedef struct GX_ENUM_DESCRIPTION
{
	int64_t nValue;                               ///< Enum Index
	char    szSymbolic[GX_INFO_LENGTH_64_BYTE];   ///< Device Information
}GX_ENUM_DESCRIPTION;

typedef struct GX_FEATURE_ENUM_ENTRY_INFO
{
	int64_t     nIndex;                           ///< Enum Index
	char    szSymbolic[GX_INFO_LENGTH_64_BYTE];   ///< Enum Information
}GX_FEATURE_ENUM_ENTRY_INFO;

typedef struct GX_INT_FEATURE
{
	uint32_t nMaxValue;
	uint32_t nMinValue;
	uint32_t nCurrentValue;
	uint32_t nIncValue;
	char  strUnit[16];

} GX_INT_FEATURE;

typedef struct GX_FLOAT_FEATURE
{
	double  dMaxValue;
	double  dMinValue;
	double  dCurrentValue;
	double  dIncValue;
	bool    bHasInc;
	bool    bIsSupport;
	char  strUnit[16];
}GX_FLOAT_FEATURE;

typedef enum tagVideoProcAmpProperty_EX
{
	VideoProcAmp_Wb_OFFSETX      = VideoProcAmp_Brightness + 1000,         ///< OffsetX
	VideoProcAmp_Wb_OFFSETY      = VideoProcAmp_Wb_OFFSETX + 1,            ///< OffsetY
	VideoProcAmp_Wb_Width        = VideoProcAmp_Wb_OFFSETY + 1,            ///< Width
	VideoProcAmp_Wb_Height       = VideoProcAmp_Wb_Width + 1,              ///< Height
	VideoProcAmp_Wb_AcqFrameRate = VideoProcAmp_Wb_Height + 1,             ///< AcquisitionFrameRate
	VideoProcAmp_Wb_ExposureTime = VideoProcAmp_Wb_AcqFrameRate + 1,       ///< ExposureTime
	VideoProcAmp_Wb_PacketSize   = VideoProcAmp_Wb_ExposureTime + 1,       ///< GevSCPSPacketSize
	VideoProcAmp_Wb_GevSCPD      = VideoProcAmp_Wb_PacketSize + 1,         ///< GevSCPD
	VideoProcAmp_Wb_AcqFrameRateMode = VideoProcAmp_Wb_GevSCPD + 1,
} 	VideoProcAmpProperty_EX;


typedef enum GX_PIXEL_SIZE_ENTRY
{
	GX_PIXEL_SIZE_BPP8  = 8,
	GX_PIXEL_SIZE_BPP10 = 10,
	GX_PIXEL_SIZE_BPP12 = 12,
	GX_PIXEL_SIZE_BPP16 = 16,
	GX_PIXEL_SIZE_BPP24 = 24,
	GX_PIXEL_SIZE_BPP30 = 30,
	GX_PIXEL_SIZE_BPP32 = 32,
	GX_PIXEL_SIZE_BPP36 = 36,
	GX_PIXEL_SIZE_BPP48 = 48,
	GX_PIXEL_SIZE_BPP64 = 64,
}GX_PIXEL_SIZE_ENTRY;

// {3777CEAF-52EE-40de-A1AF-1CC492F3680E}
DEFINE_GUID(IID_IDHCamFilter,                  
			0x3777ceaf, 0x52ee, 0x40de, 0xa1, 0xaf, 0x1c, 0xc4, 0x92, 0xf3, 0x68, 0xe);


// {4916005E-906E-4790-88EA-6FA495857780}
DEFINE_GUID(IID_IDHCamPin,     
			0x4916005e, 0x906e, 0x4790, 0x88, 0xea, 0x6f, 0xa4, 0x95, 0x85, 0x77, 0x80);

interface IDHCamFilter : public IUnknown
{
	STDMETHOD(IsColor)(bool& bIsColor) = 0;
	STDMETHOD(EnableColorCorrect)(bool bIsEnable) = 0;
	STDMETHOD(GetColorCorrectStatus)(bool& bIsEnable) = 0;
	STDMETHOD(SetSharpen)(bool bIsEnable, double dValue) = 0;
	STDMETHOD(SetLightness)(bool bIsEnable, long nValue) = 0;
	STDMETHOD(SetContrast)(bool bIsEnable, long nValue) = 0;
	STDMETHOD(SetSaturation)(bool bIsEnable, long nValue) = 0;
	STDMETHOD(SetGamma)(bool bIsEnable, double dValue) = 0;
	STDMETHOD(GetSharpen)(bool& bIsEnable, double& dValue) = 0;
	STDMETHOD(GetLightness)(bool& bIsEnable, long& nValue) = 0;
	STDMETHOD(GetContrast)(bool& bIsEnable, long& nValue) = 0;
	STDMETHOD(GetSaturation)(bool& bIsEnable, long& nValue) = 0;
	STDMETHOD(GetGamma)(bool& bIsEnable, double& dValue) = 0;
	STDMETHOD(GetPixelSize)(GX_PIXEL_SIZE_ENTRY& emPixelSize) = 0;
	STDMETHOD(GetDevicePointer)(void** pDevice) = 0;
	STDMETHOD(IsConnect)(bool& bIsConnected) = 0;
	STDMETHOD(GetBoolFeatureValue)(const char* strFeatureName, bool& bValue) = 0;
	STDMETHOD(SetBoolFeatureValue)(const char* strFeatureName, bool bValue) = 0;
	STDMETHOD(GetIntFeatureValue)(const char* strFeatureName, int64_t& nValue) = 0;
	STDMETHOD(SetIntFeatureValue)(const char* strFeatureName, int64_t nValue) = 0;
	STDMETHOD(GetIntFeatureRang)(const char* strFeatureName, GX_INT_FEATURE& stFeatureRange) = 0;
	STDMETHOD(GetFloatFeatureValue)(const char* strFeatureName, float& dValue) = 0;
	STDMETHOD(SetFloatFeatureValue)(const char* strFeatureName, float dValue) = 0;
	STDMETHOD(GetFloatFeatureRang)(const char* strFeatureName, GX_FLOAT_FEATURE& stFeatureRange) = 0;
	STDMETHOD(GetEnumFeatureValue)(const char* strFeatureName, char* strValue, int64_t* pStrLen) = 0;
	STDMETHOD(SetEnumFeatureValue)(const char* strFeatureName, const char* strValue) = 0;
	STDMETHOD(GetEnumFeatureList)(const char* strFeatureName, GX_FEATURE_ENUM_ENTRY_INFO* pEnumEntry, int64_t* pEnumCount) = 0;
	STDMETHOD(GetStringFeatureValue)(const char* strFeatureName, char* strValue, int64_t* pStrLen) = 0;
	STDMETHOD(SetStringFeatureValue)(const char* strFeatureName, const char* strValue) = 0;
	STDMETHOD(GetStringMaxLength)(const char* strFeatureName, int64_t& nMaxLength) = 0;
	STDMETHOD(GetRegisterFeature)(const char* strFeatureName, uint8_t* pBuffer, int64_t nLength) = 0;
	STDMETHOD(SetRegisterFeature)(const char* strFeatureName, uint8_t* pBuffer, int64_t nLength) = 0;
	STDMETHOD(GetRegisterLength)(const char* strFeatureName, int64_t& nLength) = 0;

};

interface IDHCamPin : public IUnknown
{ 
	STDMETHOD(GetCurrentDeviceIndex)(long& plDevice) = 0;
	STDMETHOD(SetCurrentDeviceIndex)(long lDevice) = 0;
	STDMETHOD(GetDeviceList)(GX_ENUM_DESCRIPTION* pEnumDescription, long* pBufferSizet) = 0;

};
#endif //GXIAPI_BASE_H