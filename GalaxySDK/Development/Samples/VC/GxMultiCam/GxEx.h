#ifndef GX_EX_H
#define GX_EX_H
#include "GxIAPI.h"
#include "GxBitmap.h"


///��������ṹ��
typedef struct CAMER_INFO
{
	BOOL						  bIsOpen;		  ///< ����Ѵ򿪱�־
	BOOL					   	  bIsSnap;		  ///< ������ڲɼ���־
	float						  fFps;			  ///< ֡��
	CGXBitmap                    *m_pBitmap;      ///< ��ʾ����ͼ����
}CAMER_INFO;
	
#endif