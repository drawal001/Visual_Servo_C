//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.h 
\brief   CGxGigeRecovery class declaration
*/
//----------------------------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "conio.h"
#include <iostream>


class CSampleDeviceOfflineEventHandle;              ///< 声明掉线事件处理类

//--------------------------------------------------
/**
\brief     设备控制类
*/
//--------------------------------------------------
class CGigeRecovery
{
public:

	/// 构造函数
	CGigeRecovery();
 
	/// 析构函数
	virtual ~CGigeRecovery();

	/// 掉线重连对外接口函数
	void GigeRecovery();

	bool m_bIsOffline;               ///< 设备掉线标识

private:

	/// 打开设备
	bool __OnOpenDevice() throw();

	/// 开始采集
	bool __OnStartSnap() throw();

	/// 停止采集
	bool __OnStopSnap() throw();

	/// 关闭设备
	bool __OnCloseDevice() throw();

	/// 初始化设备参数
	void __InitParam();

	/// 连续采集
	void __Acquisition() throw();

	/// 设备掉线处理
	void __ProcessOffline() throw();

	/// 设备重连
	void __Recovery() throw();

	CGXDevicePointer                   m_objDevicePtr;                       ///< 设备句柄
	CGXStreamPointer                   m_objStreamPtr;                       ///< 流对象
	CGXFeatureControlPointer           m_objFeatureControlPtr;               ///< 属性控制器对象
	CGXFeatureControlPointer           m_objStreamFeatureControlPtr;         ///< 流层控制器对象
	GX_DEVICE_OFFLINE_CALLBACK_HANDLE  m_hCB;                                ///< 掉线回调句柄
	CImageDataPointer                  m_objImageDataPtr;                    ///< GetImage获取的图像

	bool                               m_bIsOpen;                            ///< 设备打开标识
	bool                               m_bIsSnap;                            ///< 设备采集标识  
	GxIAPICPP::gxstring                m_strSavePath;                        ///< 设备配置参数文件保存路径
	CSampleDeviceOfflineEventHandle*   m_pDeviceOfflineEventHandle;          ///< 掉线事件指针
	gxstring                           m_strMACAddress;                      ///< 相机MAC地址
};

//--------------------------------------------------
/**
\brief     用户继承掉线处理类
*/
//--------------------------------------------------
class CSampleDeviceOfflineEventHandle :public IDeviceOfflineEventHandler
{
public:
	//--------------------------------------------------
	/**
	\brief     掉线回调函数
    \param     pUserParam      用户参数
    
	\return    void
	*/
	//--------------------------------------------------
	void DoOnDeviceOfflineEvent(void* pUserParam)
	{
		try
		{
			CGigeRecovery* pGigeRecovery = (CGigeRecovery*)pUserParam;
			pGigeRecovery->m_bIsOffline = true;
			std::cout<<"**********************Device offline**********************"<<std::endl;

		}
		catch (CGalaxyException)
		{
			//do nothing
		}
		catch (std::exception)
		{
			//do noting
		}
	}
};