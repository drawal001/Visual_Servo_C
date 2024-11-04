//---------------------------------------------------------------
/**
\file         IGXImageProcess.h
\brief        Image Process class
\Date       2024-03-07
\Version    1.1.2403.9071

<p>Copyright (c) 2011-2021 China Daheng Group, Inc. Beijing Image
Vision Technology Branch and all right reserved.</p>
*/
//---------------------------------------------------------------

#pragma once
#include "GXIAPIBase.h"
#include "GXSmartPtr.h"
#include "IImageProcessConfig.h"
#include "IImageData.h"
#include "IGXImageSpecialFunction.h"

//--------------------------------------------
/**
\brief    image process interface class
*/
//---------------------------------------------
class GXIAPICPP_API IGXImageProcess :public IGXImageSpecialFunction
{

public:

	//---------------------------------------------------------
	/**
	\brief Destructor
	*/
	//---------------------------------------------------------
	virtual ~IGXImageProcess() {};

	//-------------------------------------------------------------
	/**
	\brief  Image defect pixel correction  
	\param  pInputBuffer		[in]  Image in
	\param  pOutputBuffer		[out] Image out
	\param  stDefectCorrection  [in]  Image parameter used to do defect correction
	\param  pDefectPosBuffer	[in]  Defect Pixel position file buffer
	\param  nDefectPosBufferSize[in]  Defect Pixel position file buffer size

	\return  DX_STATUS 
	*/
	//-------------------------------------------------------------
	virtual void  StaticDefectCorrection(void *pInputBuffer, void *pOutputBuffer, const GX_STATIC_DEFECT_CORRECTION stDefectCorrection, void* pDefectPosBuffer, int32_t nDefectPosBufferSize) = 0;


	//----------------------------------------------------------------------------------
	/**
	\brief    Image quality improvement
	\param [in]stImgInfo   Image Attribute Structure
	\param [in]pOutputBuf  Output Image Buffer
	\param [in]objCfg      Image quality improvement configuration parameter class

	\return void
	*/
	//----------------------------------------------------------------------------------
	virtual void ImageImprovment(GX_IMAGE_INFO& stImgInfo, void *pOutputBuf, CImageProcessConfigPointer& objCfg) = 0;


	//----------------------------------------------------------------------------------
	/**
	\brief    Image quality improvement
	\param [in]objFrameData   Source Image Pointer
	\param [in]pOutputBuf     Output Image Buffer
	\param [in]objCfg         Image quality improvement configuration parameter class

	\return void
	*/
	//----------------------------------------------------------------------------------
	virtual void ImageImprovment(CImageDataPointer objFrameData, void *pOutputBuf, CImageProcessConfigPointer& objCfg) = 0;

};

template class GXIAPICPP_API  GXSmartPtr<IGXImageProcess>;
typedef GXSmartPtr<IGXImageProcess>            CGXImageProcessPointer;
