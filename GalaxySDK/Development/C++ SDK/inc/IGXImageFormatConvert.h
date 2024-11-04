//---------------------------------------------------------------
/**
\file           IGxImageFormatConvert.h
\brief        Definition of the IGxImageFormatConvert interface
\Date       2024-05-21
\Version    1.1.2405.9211
<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>
*/
//---------------------------------------------------------------

#pragma once
#include "GXIAPIBase.h"
#include "GXSmartPtr.h"
#include "IImageData.h"


//--------------------------------------------
/**
\brief    Image format conversion class
*/
//---------------------------------------------
class GXIAPICPP_API IGXImageFormatConvert
{

public:

    //---------------------------------------------------------
    /**
    \brief Destructor
    */
    //---------------------------------------------------------
    virtual ~IGXImageFormatConvert(){};

    //----------------------------------------------------------------------------------
	/**
	\brief       Sets the pixel format of the image you want to convert
	\param    emPixelFormat[in]       Image pixel format expected to be converted (Default : GX_PIXEL_FORMAT_UNDEFINED) 
	\return    void
	*/
	//----------------------------------------------------------------------------------
    virtual void SetDstFormat(GX_PIXEL_FORMAT_ENTRY emDstFormat) = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief      Get the pixel format of the image you want to convert
	\return   Image pixel format expected to be converted
	*/
	//----------------------------------------------------------------------------------
    virtual GX_PIXEL_FORMAT_ENTRY GetDstFormat() = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief        Set conversion algorithm
	\param     emConvertType[in]       Algorithm used in conversion (Default : GX_RAW2RGB_NEIGHBOUR) 
	\return     void
	*/
	//----------------------------------------------------------------------------------
    virtual void    SetInterpolationType(GX_BAYER_CONVERT_TYPE_LIST emInterpolationType) = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief        Get conversion algorithm
	\return      Algorithm used in conversion
	*/
	//----------------------------------------------------------------------------------
    virtual GX_BAYER_CONVERT_TYPE_LIST GetInterpolationType() = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief        Sets the alpha value of an image with an alpha channel
	\param     ui64AlphaValue[in]       Alpha channel value (Range : 0-255  Default : 255)  
	\return     void
	*/
	//----------------------------------------------------------------------------------
    virtual void SetAlphaValue(uint64_t ui64AlphaValue) = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief        Gets the alpha value of an image with an alpha channel
	\return     Alpha channel value
	*/
	//----------------------------------------------------------------------------------
    virtual uint64_t GetAlphaValue() = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief       Obtain the converted image buffer size according to the image data(Unit : byte)
	\param    pObjSrcImageData[in]       Source image pointer
	\return    Buffer length of converted image
	*/
	//----------------------------------------------------------------------------------
    virtual uint64_t GetBufferSizeForConversion(CImageDataPointer pObjSrcImageData) = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief       Obtain the converted image buffer size according to the image data(Unit : byte)
	\param    ui64SrcWidth[in]        Source image width
	\param    ui64SrcHeight[in]       Source image height
	\param    emSrcFormat[in]        Source image pixel format
	\return    Buffer length of converted image
	*/
	//----------------------------------------------------------------------------------
    virtual uint64_t GetBufferSizeForConversion(uint64_t ui64SrcWidth, uint64_t ui64SrcHeight, 
        GX_PIXEL_FORMAT_ENTRY emSrcFormat) = 0;

    //----------------------------------------------------------------------------------
	/**
	\brief        Perform conversion

    Supported image format conversion include: 

    1.Bayer conversion
         a. input image format  GX_PIXEL_FORMAT_BAYER_GR8 GX_PIXEL_FORMAT_BAYER_RG8 
								GX_PIXEL_FORMAT_BAYER_GB8 GX_PIXEL_FORMAT_BAYER_BG8
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_RGB8 
								GX_PIXEL_FORMAT_RGBA8 GX_PIXEL_FORMAT_BGRA8 
                                GX_PIXEL_FORMAT_ARGB8 GX_PIXEL_FORMAT_ABGR8 
								GX_PIXEL_FORMAT_RGB8_PLANAR

         b. input image format  GX_PIXEL_FORMAT_BAYER_GR10 GX_PIXEL_FORMAT_BAYER_RG10
								GX_PIXEL_FORMAT_BAYER_GB10 GX_PIXEL_FORMAT_BAYER_BG10 
                                GX_PIXEL_FORMAT_BAYER_GR12 GX_PIXEL_FORMAT_BAYER_RG12
								GX_PIXEL_FORMAT_BAYER_GB12 GX_PIXEL_FORMAT_BAYER_BG12 
                                GX_PIXEL_FORMAT_BAYER_GR14 GX_PIXEL_FORMAT_BAYER_RG14
								GX_PIXEL_FORMAT_BAYER_GB14 GX_PIXEL_FORMAT_BAYER_BG14
                                GX_PIXEL_FORMAT_BAYER_GR16 GX_PIXEL_FORMAT_BAYER_RG16
								GX_PIXEL_FORMAT_BAYER_GB16 GX_PIXEL_FORMAT_BAYER_BG16
            output image format GX_PIXEL_FORMAT_MONO16 GX_PIXEL_FORMAT_RGB16
								GX_PIXEL_FORMAT_BGR16 GX_PIXEL_FORMAT_RGB16_PLANAR 
			                    GX_PIXEL_FORMAT_RGB8 GX_PIXEL_FORMAT_BGR8

        2.RGB conversion
         a. input image format  GX_PIXEL_FORMAT_RGB8 GX_PIXEL_FORMAT_BGR8
            output image format GX_PIXEL_FORMAT_YUV444_8 GX_PIXEL_FORMAT_YUV422_8
								GX_PIXEL_FORMAT_YUV411_8 GX_PIXEL_FORMAT_YUV420_8_PLANAR 
                                GX_PIXEL_FORMAT_YCBCR444_8 GX_PIXEL_FORMAT_YCBCR422_8
								GX_PIXEL_FORMAT_YCBCR411_8 GX_PIXEL_FORMAT_MONO8
								GX_PIXEL_FORMAT_RGB8

         b. input image format  GX_PIXEL_FORMAT_RGB16 GX_PIXEL_FORMAT_BGR16
            output image format GX_PIXEL_FORMAT_MONO16 

        3.Packed conversion(GVSP)
         a. input image format  GX_PIXEL_FORMAT_MONO10_PACKED GX_PIXEL_FORMAT_MONO12_PACKED 
		                        GX_PIXEL_FORMAT_BAYER_RG10_PACKED GX_PIXEL_FORMAT_BAYER_GR10_PACKED
								GX_PIXEL_FORMAT_BAYER_BG10_PACKED GX_PIXEL_FORMAT_BAYER_GB10_PACKED
								GX_PIXEL_FORMAT_BAYER_RG12_PACKED GX_PIXEL_FORMAT_BAYER_GR12_PACKED
								GX_PIXEL_FORMAT_BAYER_BG12_PACKED GX_PIXEL_FORMAT_BAYER_GB12_PACKED
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_MONO10
								GX_PIXEL_FORMAT_MONO12 GX_PIXEL_FORMAT_RGB8
                                GX_PIXEL_FORMAT_BAYER_RG8 GX_PIXEL_FORMAT_BAYER_GR8
								GX_PIXEL_FORMAT_BAYER_BG8 GX_PIXEL_FORMAT_BAYER_GB8
                                GX_PIXEL_FORMAT_BAYER_RG10 GX_PIXEL_FORMAT_BAYER_GR10
								GX_PIXEL_FORMAT_BAYER_BG10 GX_PIXEL_FORMAT_BAYER_GB10
                                GX_PIXEL_FORMAT_BAYER_RG12 GX_PIXEL_FORMAT_BAYER_GR12
								GX_PIXEL_FORMAT_BAYER_BG12 GX_PIXEL_FORMAT_BAYER_GB12
								GX_PIXEL_FORMAT_RGB8

        4.Packed conversion(PFNC)
         a. input image format  GX_PIXEL_FORMAT_MONO10_P GX_PIXEL_FORMAT_MONO12_P
								GX_PIXEL_FORMAT_MONO14_P
		                        GX_PIXEL_FORMAT_BAYER_RG10_P GX_PIXEL_FORMAT_BAYER_GR10_P
								GX_PIXEL_FORMAT_BAYER_BG10_P GX_PIXEL_FORMAT_BAYER_GB10_P
								GX_PIXEL_FORMAT_BAYER_RG12_P GX_PIXEL_FORMAT_BAYER_GR12_P
								GX_PIXEL_FORMAT_BAYER_BG12_P GX_PIXEL_FORMAT_BAYER_GB12_P
   								GX_PIXEL_FORMAT_BAYER_RG14_P GX_PIXEL_FORMAT_BAYER_GR14_P
								GX_PIXEL_FORMAT_BAYER_BG14_P GX_PIXEL_FORMAT_BAYER_GB14_P                            
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_MONO10
								GX_PIXEL_FORMAT_MONO12 GX_PIXEL_FORMAT_MONO14
								GX_PIXEL_FORMAT_RGB8
                                GX_PIXEL_FORMAT_BAYER_RG8 GX_PIXEL_FORMAT_BAYER_GR8
								GX_PIXEL_FORMAT_BAYER_BG8 GX_PIXEL_FORMAT_BAYER_GB8
                                GX_PIXEL_FORMAT_BAYER_RG10 GX_PIXEL_FORMAT_BAYER_GR10
								GX_PIXEL_FORMAT_BAYER_BG10 GX_PIXEL_FORMAT_BAYER_GB10
                                GX_PIXEL_FORMAT_BAYER_RG12 GX_PIXEL_FORMAT_BAYER_GR12
								GX_PIXEL_FORMAT_BAYER_BG12 GX_PIXEL_FORMAT_BAYER_GB12
                                GX_PIXEL_FORMAT_BAYER_RG14 GX_PIXEL_FORMAT_BAYER_GR14
								GX_PIXEL_FORMAT_BAYER_BG14 GX_PIXEL_FORMAT_BAYER_GB14	
        5.Mono conversion
         a. input image format  GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_R8
								GX_PIXEL_FORMAT_G8 GX_PIXEL_FORMAT_B8
            output image format GX_PIXEL_FORMAT_RGB8

         b. input image format  GX_PIXEL_FORMAT_MONO10 GX_PIXEL_FORMAT_MONO12
								GX_PIXEL_FORMAT_MONO14 GX_PIXEL_FORMAT_MONO16
            output image format GX_PIXEL_FORMAT_RGB8 

    6.Coord3D conversion
        a. input image format  GX_PIXEL_FORMAT_COORD3D_C16
           output image format GX_PIXEL_FORMAT_COORD3D_ABC32F

        b. input image format  GX_PIXEL_FORMAT_COORD3D_C16
           output image format GX_PIXEL_FORMAT_COORD3D_ABC32F_PLANAR

	\param    pObjSrcImageData[in]             Source image pointer
	\param    pDstBuffer[in|out]               Output converted image buffer data
	\param    nDstBufferSize[in]               Input pDstBuffer size
	\param    bFlip[in]                        Is the image mirrored
	\return    void
	*/
	//----------------------------------------------------------------------------------
    virtual void Convert(CImageDataPointer pObjSrcImageData, void* pDstBuffer, size_t nDstBufferSize, bool bFlip) = 0;
    
    //----------------------------------------------------------------------------------
	/**
	\brief        Perform conversion

    Supported image format conversion include: 

     1.Bayer conversion
         a. input image format  GX_PIXEL_FORMAT_BAYER_GR8 GX_PIXEL_FORMAT_BAYER_RG8 
								GX_PIXEL_FORMAT_BAYER_GB8 GX_PIXEL_FORMAT_BAYER_BG8
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_RGB8 
								GX_PIXEL_FORMAT_RGBA8 GX_PIXEL_FORMAT_BGRA8 
                                GX_PIXEL_FORMAT_ARGB8 GX_PIXEL_FORMAT_ABGR8 
								GX_PIXEL_FORMAT_RGB8_PLANAR

         b. input image format  GX_PIXEL_FORMAT_BAYER_GR10 GX_PIXEL_FORMAT_BAYER_RG10
								GX_PIXEL_FORMAT_BAYER_GB10 GX_PIXEL_FORMAT_BAYER_BG10 
                                GX_PIXEL_FORMAT_BAYER_GR12 GX_PIXEL_FORMAT_BAYER_RG12
								GX_PIXEL_FORMAT_BAYER_GB12 GX_PIXEL_FORMAT_BAYER_BG12 
                                GX_PIXEL_FORMAT_BAYER_GR14 GX_PIXEL_FORMAT_BAYER_RG14
								GX_PIXEL_FORMAT_BAYER_GB14 GX_PIXEL_FORMAT_BAYER_BG14
                                GX_PIXEL_FORMAT_BAYER_GR16 GX_PIXEL_FORMAT_BAYER_RG16
								GX_PIXEL_FORMAT_BAYER_GB16 GX_PIXEL_FORMAT_BAYER_BG16
            output image format GX_PIXEL_FORMAT_MONO16 GX_PIXEL_FORMAT_RGB16
								GX_PIXEL_FORMAT_BGR16 GX_PIXEL_FORMAT_RGB16_PLANAR 
			                    GX_PIXEL_FORMAT_RGB8 GX_PIXEL_FORMAT_BGR8

        2.RGB conversion
         a. input image format  GX_PIXEL_FORMAT_RGB8 GX_PIXEL_FORMAT_BGR8
            output image format GX_PIXEL_FORMAT_YUV444_8 GX_PIXEL_FORMAT_YUV422_8
								GX_PIXEL_FORMAT_YUV411_8 GX_PIXEL_FORMAT_YUV420_8_PLANAR 
                                GX_PIXEL_FORMAT_YCBCR444_8 GX_PIXEL_FORMAT_YCBCR422_8
								GX_PIXEL_FORMAT_YCBCR411_8 GX_PIXEL_FORMAT_MONO8
								GX_PIXEL_FORMAT_RGB8

         b. input image format  GX_PIXEL_FORMAT_RGB16 GX_PIXEL_FORMAT_BGR16
            output image format GX_PIXEL_FORMAT_MONO16 

        3.Packed conversion(GVSP)
         a. input image format  GX_PIXEL_FORMAT_MONO10_PACKED GX_PIXEL_FORMAT_MONO12_PACKED 
		                        GX_PIXEL_FORMAT_BAYER_RG10_PACKED GX_PIXEL_FORMAT_BAYER_GR10_PACKED
								GX_PIXEL_FORMAT_BAYER_BG10_PACKED GX_PIXEL_FORMAT_BAYER_GB10_PACKED
								GX_PIXEL_FORMAT_BAYER_RG12_PACKED GX_PIXEL_FORMAT_BAYER_GR12_PACKED
								GX_PIXEL_FORMAT_BAYER_BG12_PACKED GX_PIXEL_FORMAT_BAYER_GB12_PACKED
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_MONO10
								GX_PIXEL_FORMAT_MONO12 GX_PIXEL_FORMAT_RGB8
                                GX_PIXEL_FORMAT_BAYER_RG8 GX_PIXEL_FORMAT_BAYER_GR8
								GX_PIXEL_FORMAT_BAYER_BG8 GX_PIXEL_FORMAT_BAYER_GB8
                                GX_PIXEL_FORMAT_BAYER_RG10 GX_PIXEL_FORMAT_BAYER_GR10
								GX_PIXEL_FORMAT_BAYER_BG10 GX_PIXEL_FORMAT_BAYER_GB10
                                GX_PIXEL_FORMAT_BAYER_RG12 GX_PIXEL_FORMAT_BAYER_GR12
								GX_PIXEL_FORMAT_BAYER_BG12 GX_PIXEL_FORMAT_BAYER_GB12
								GX_PIXEL_FORMAT_RGB8

        4.Packed conversion(PFNC)
         a. input image format  GX_PIXEL_FORMAT_MONO10_P GX_PIXEL_FORMAT_MONO12_P
								GX_PIXEL_FORMAT_MONO14_P
		                        GX_PIXEL_FORMAT_BAYER_RG10_P GX_PIXEL_FORMAT_BAYER_GR10_P
								GX_PIXEL_FORMAT_BAYER_BG10_P GX_PIXEL_FORMAT_BAYER_GB10_P
								GX_PIXEL_FORMAT_BAYER_RG12_P GX_PIXEL_FORMAT_BAYER_GR12_P
								GX_PIXEL_FORMAT_BAYER_BG12_P GX_PIXEL_FORMAT_BAYER_GB12_P
   								GX_PIXEL_FORMAT_BAYER_RG14_P GX_PIXEL_FORMAT_BAYER_GR14_P
								GX_PIXEL_FORMAT_BAYER_BG14_P GX_PIXEL_FORMAT_BAYER_GB14_P                            
            output image format GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_MONO10
								GX_PIXEL_FORMAT_MONO12 GX_PIXEL_FORMAT_MONO14
								GX_PIXEL_FORMAT_RGB8
                                GX_PIXEL_FORMAT_BAYER_RG8 GX_PIXEL_FORMAT_BAYER_GR8
								GX_PIXEL_FORMAT_BAYER_BG8 GX_PIXEL_FORMAT_BAYER_GB8
                                GX_PIXEL_FORMAT_BAYER_RG10 GX_PIXEL_FORMAT_BAYER_GR10
								GX_PIXEL_FORMAT_BAYER_BG10 GX_PIXEL_FORMAT_BAYER_GB10
                                GX_PIXEL_FORMAT_BAYER_RG12 GX_PIXEL_FORMAT_BAYER_GR12
								GX_PIXEL_FORMAT_BAYER_BG12 GX_PIXEL_FORMAT_BAYER_GB12
                                GX_PIXEL_FORMAT_BAYER_RG14 GX_PIXEL_FORMAT_BAYER_GR14
								GX_PIXEL_FORMAT_BAYER_BG14 GX_PIXEL_FORMAT_BAYER_GB14	
        5.Mono conversion
         a. input image format  GX_PIXEL_FORMAT_MONO8 GX_PIXEL_FORMAT_R8
								GX_PIXEL_FORMAT_G8 GX_PIXEL_FORMAT_B8
            output image format GX_PIXEL_FORMAT_RGB8

         b. input image format  GX_PIXEL_FORMAT_MONO10 GX_PIXEL_FORMAT_MONO12
								GX_PIXEL_FORMAT_MONO14 GX_PIXEL_FORMAT_MONO16
            output image format GX_PIXEL_FORMAT_RGB8 

    6.Coord3D conversion
        a. input image format  GX_PIXEL_FORMAT_COORD3D_C16
           output image format GX_PIXEL_FORMAT_COORD3D_ABC32F

        b. input image format  GX_PIXEL_FORMAT_COORD3D_C16
           output image format GX_PIXEL_FORMAT_COORD3D_ABC32F_PLANAR

    \param    pSrcBuffer[in]                 Source image Buffer
    \param    ui64SrcWidth[in]               Source image width
    \param    ui64SrcHeight[in]              Source image Height
    \param    emSrcFormat[in]                Source image pixel format
	\param    pDstBuffer[in|out]             Output converted image buffer data
	\param    nDstBufferSize[in]             Input pDstBuffer size
	\param    bFlip[in]                      Is the image mirrored
	\return    void
	*/
	//----------------------------------------------------------------------------------
    virtual void Convert(void* pSrcBuffer, uint64_t ui64SrcWidth, uint64_t ui64SrcHeight, 
        GX_PIXEL_FORMAT_ENTRY emSrcFormat, void* pDstBuffer, size_t nDstBufferSize, bool bFlip) = 0;
	
	//--------------------------------------------------
	/**
	\brief  Load 3D Calibration Parameters 
	\param  pCalibParamBuffer   [in]     Calibration Parameters
	\param  nBufferSize			[in]     Calibration Parameters size

	\return void
	*/
	//--------------------------------------------------
	virtual void Set3DCalibParam(void* pCalibParamBuffer, size_t nBufferSize) = 0;

	//--------------------------------------------------
	/**
	\brief  Get 3D Calibration Parameters 
	\param  pCalibParamBuffer   [in]     Calibration Parameters
	\param  nBufferSize			[in]     Calibration Parameters size

	\return void
	*/
	//--------------------------------------------------
	virtual void* Get3DCalibParam() = 0;

	//--------------------------------------------------
	/**
	\brief  Set Y-direction step 
	\param  dY					[in]     Y-direction step

	\return void
	*/
	//--------------------------------------------------
	virtual void SetYStep(double dY) = 0;

	//--------------------------------------------------
	/**
	\brief  Get Y-direction step 
	\param  dY					[in]     Y-direction step

	\return double
	*/
	//--------------------------------------------------
	virtual double GetYStep() = 0;

	//--------------------------------------------------
	/**
	\brief  Set Valid Bits
	\param  emValidBits     [in] Valid Bits value

	\return void
	*/
	//--------------------------------------------------
	virtual void SetValidBits(GX_VALID_BIT_LIST emValidBits) = 0;

	//--------------------------------------------------
	/**
	\brief  Get Valid Bits

	\return Valid Bit
	*/
	//--------------------------------------------------
	virtual GX_VALID_BIT_LIST  GetValidBits() = 0;

    //The extended adjustment parameters are added below and cannot be added to existing functions
    //...
};

template class GXIAPICPP_API  GXSmartPtr<IGXImageFormatConvert>;
typedef GXSmartPtr<IGXImageFormatConvert>            CGXImageFormatConvertPointer;

