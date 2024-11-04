#ifndef GX_EX_H
#define GX_EX_H
#include "GxIAPI.h"
#include "GxBitmap.h"


///相机参数结构体
typedef struct CAMER_INFO
{
	BOOL						  bIsOpen;		  ///< 相机已打开标志
	BOOL					   	  bIsSnap;		  ///< 相机正在采集标志
	float						  fFps;			  ///< 帧率
	CGXBitmap                    *m_pBitmap;      ///< 显示及画图对象
}CAMER_INFO;
	
#endif