//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.cpp 
\brief   Gige recovery function
\version v1.0.2404.9281
\date    2024-4-28
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "GxIAPI.h"
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;
#include <TCHAR.H>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();
//----------------------------------------------------------------------------------
/** 
\brief   设备控制类
*/
//----------------------------------------------------------------------------------
class CGigeRecovery
{
public:
	CGigeRecovery();
    virtual ~CGigeRecovery();

	/// GigeRecovery掉线重连对外接口函数
	void GigeRecovery();

private:

	/// 打开设备
	bool __OnOpenDevice();

	/// 初始化相机参数
	bool __InitParam();
	
	/// 开始采集
	bool __OnStartSnap();
	
	/// 停止采集
	bool __OnStopSnap();

	/// 关闭设备
	bool __OnCloseDevice();
	
	/// 连续采集
	void __Acquisition();

	/// 设备掉线处理
	void __ProcessOffline();

	/// 设备重连
	void __Recovery();

	/// 打印错误信息描述
	void __GetErrorString(GX_STATUS emErrorStatus);

	/// 掉线回调函数
	static void __stdcall __OnDeviceOfflineCallbackFun(void* pUserParam);

private:
	GX_DEV_HANDLE             m_hDevice;                              ///< 设备句柄
	GX_DS_HANDLE			  m_hStream;				              ///< 设备流句柄
	GX_EVENT_CALLBACK_HANDLE  m_hCB;                                  ///< 掉线回调句柄
	string                    m_strPath;                              ///< 文件的当前路径
	string                    m_strSavePath;                          ///< 相机配置参数文件保存路径
	char                      m_chModulePath[100];                    ///< 获取的当前工作路径 
	char                      m_chMACAddress[GX_INFO_LENGTH_32_BYTE]; ///< 相机MAC地址
	GX_FRAME_DATA             m_stFrameData;                          ///< GetImage获取的图像
	bool                      m_bIsOffLine;                           ///< 相机掉线标识
	bool                      m_bIsOpen;                              ///< 相机打开标识 
	bool                      m_bIsSanp;                              ///< 相机采集标志
};

//----------------------------------------------------------------------------------
/**
\brief 主函数main

\return int 
*/
//----------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// 调用CGigeRecovery类实现掉线重连功能
	CGigeRecovery objCtrDevice;
	objCtrDevice.GigeRecovery();

	// 按下任意键关闭控制台
	printf("\n<press any key to exit>\n");
	while (!_kbhit());

	return 0;
}

//---------------------------------------------------------------------------------
/**
\brief   构造函数

 \return   无
*/
//----------------------------------------------------------------------------------
CGigeRecovery::CGigeRecovery()
{ 
	m_hDevice             = NULL;              // 设备句柄
	m_hStream			  = NULL;			   // 设备流句柄
	m_hCB                 = NULL;              // 掉线回调句柄
	m_stFrameData.pImgBuf = NULL;              // GetImage获取的图像buffer
	m_strPath             = "";                // 获取的当前工作路径
	m_strSavePath         = "";                // 文件的当前路径
	m_bIsOffLine          = false;             // 相机掉线标识
	m_bIsOpen             = false;             // 相机打开标识
	m_bIsSanp             = false;             // 相机采集标识

	// 初始化设备库
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		exit(0);
	}

	// 获取当前路径，用于保存相机参数配置文件
	GetModuleFileName(NULL, m_chModulePath, sizeof(m_chModulePath));
	(_tcsrchr(m_chModulePath, _T('\\')))[1] = 0;
	m_strPath = m_chModulePath;
	m_strSavePath = m_strPath + "ConfigFile.ini";	
}

//---------------------------------------------------------------------------------
/**
\brief    析构函数

\return   无
*/
//----------------------------------------------------------------------------------
CGigeRecovery::~CGigeRecovery()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 关闭设备库
	emStatus = GXCloseLib();
}

//---------------------------------------------------------------------------------
/**
\brief    打开设备

\return   true：返回成功，false：返回失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS         emStatus   = GX_STATUS_SUCCESS;   
	uint32_t          nDeviceNum = 0;        // 设备个数 
	bool              bReturn    = false;    // 设置相机参数返回值
    GX_DEVICE_IP_INFO stDeviceIPInfo;        // 相机IP信息
	GX_DEVICE_BASE_INFO *pDeviceBaseInfo;    // 相机基本信息
	size_t nDeviceBaseInfoSize = 0;          // GX_DEVICE_BASE_INFO结构体长度
	GX_OPEN_PARAM     stOpenParam;           // 打开相机参数
	uint32_t	  nDSNum	  = 0;

	// 枚举设备个数
	printf("====================CGigeRecovery::__OnOpenDevice()====================\n");
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
		
	// 判断当前连接设备个数
	if (nDeviceNum <= 0)
	{
		printf("<NO device>\n");
 		return false;
	}

	pDeviceBaseInfo = new(std::nothrow) GX_DEVICE_BASE_INFO[nDeviceNum];
	if (NULL == pDeviceBaseInfo) 
	{
		return false;
	}

	//获取设备基础信息
	nDeviceBaseInfoSize = sizeof(GX_DEVICE_BASE_INFO) * nDeviceNum;
	emStatus = GXGetAllDeviceBaseInfo(pDeviceBaseInfo, &nDeviceBaseInfoSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		delete[] pDeviceBaseInfo;
		pDeviceBaseInfo = NULL;
		__GetErrorString(emStatus);
		return false;
	}

	//判断枚举到的设备是否为Gige
	if (pDeviceBaseInfo->deviceClass != GX_DEVICE_CLASS_GEV)
	{
		delete[] pDeviceBaseInfo;
		pDeviceBaseInfo = NULL;
		printf("<The device is not GEV>");
		return false;
	}
	delete[] pDeviceBaseInfo;
	pDeviceBaseInfo = NULL;

	//获取第一台设备的网络信息
	emStatus = GXGetDeviceIPInfo(1, &stDeviceIPInfo);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	memcpy(m_chMACAddress, stDeviceIPInfo.szMAC, GX_INFO_LENGTH_32_BYTE);
	
	// 通过MAC地址打开设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_MAC;
	stOpenParam.pszContent = stDeviceIPInfo.szMAC;
	printf("<Open device by MAC: %s>\n", stDeviceIPInfo.szMAC);
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// 获取流通道数
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	if(0 < nDSNum)
	{
		// 获取流句柄
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
			return false;
		}
	}


	// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
	// 以提高网络相机的采集性能,设置方法参考以下代码。
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// 判断设备是否支持流通道数据包功能
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
			return false;
		}

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
			&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
			&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
		{
			bImplementPacketSize = true;
		}


		if (bImplementPacketSize)
		{
			// 获取当前网络环境的最优包长值
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				__GetErrorString(emStatus);
				return false;
			}

			// 将最优包长值设置为当前设备的流通道包长值
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				__GetErrorString(emStatus);
				return false;
			}
		}
	}
	
    // 初始化参数
	printf("<Initialize the device parameters>\n");
	bReturn = __InitParam();
	if (!bReturn)
	{
		return false;
	}

    // 导出参数配置文件
	printf("<Export config file>\n");
	emStatus = GXExportConfigFile(m_hDevice, m_strSavePath.c_str());
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// 注册掉线回调函数
	printf("<Register device Offline callback>\n");
	emStatus = GXRegisterDeviceOfflineCallback(m_hDevice, this, __OnDeviceOfflineCallbackFun, &m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	m_bIsOpen = true;	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数

\return   true：返回成功，false：返回失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__InitParam()
{
	GX_STATUS emStatus     = GX_STATUS_SUCCESS;  
	uint32_t   ui32PayLoadSize = 0;     // 原始Raw图像大小
	
	// 设置采集模式连续采集
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// 关闭触发模式
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "Off");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	//设置流层Buffer处理模式
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	//设置心跳超时时间为1s
	//针对千兆网相机，程序在Debug模式下调试运行时，相机的心跳超时时间自动设置为5min，
	//这样做是为了不让相机的心跳超时影响程序的调试和单步执行，同时这也意味着相机在这5min内无法断开，除非使相机断电再上电
	//为了解决掉线重连问题，将相机的心跳超时时间设置为1s，方便程序掉线后可以重新连接
	emStatus = GXSetIntValue(m_hDevice, "GevHeartbeatTimeout", 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	
	// 获取图像大小
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_stFrameData.pImgBuf = new(std::nothrow) BYTE[(size_t)ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   掉线回调函数
\param   pUserParam   用户参数

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGigeRecovery::__OnDeviceOfflineCallbackFun(void* pUserParam)
{
	CGigeRecovery *pCtrDevice = (CGigeRecovery *)pUserParam;

	// 相机掉线
	pCtrDevice->m_bIsOffLine = true;
	printf("**********************Device offline**********************\n");
}

//---------------------------------------------------------------------------------
/**
\brief    开始采集

\return   true：返回成功，false：返回失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStartSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;  

	printf("====================CGigeRecovery::__OnStartSnap()====================\n");

	// 发采集命令
	printf("<Send start snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_bIsSanp = true;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    停止采集

\return   true：返回成功，false：返回失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStopSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;  	
	printf("====================CGigeRecovery::__OnStopSnap()====================\n");
	
	// 发送停止采集命令
	printf("<Send stop snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_bIsSanp = false;
	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    关闭设备

\return   true：返回成功，false：返回失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	printf("====================CGigeRecovery::__OnCloseDevice()====================\n");

	// 注销设备掉线事件回调
	printf("<Unregister device Offline callback>\n");
	emStatus = GXUnregisterDeviceOfflineCallback(m_hDevice, m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
	}
	
	// 关闭相机
	printf("<Close device>\n");
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
	}
	m_hDevice = NULL;

	// 释放资源
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete[]m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}

	m_bIsOpen = false;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    连续采集

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Acquisition() 
{ 
	GX_STATUS emStatus = GX_STATUS_SUCCESS; 
	printf("====================CGigeRecovery::__Acquisition()====================\n");
	printf("<Press any key to stop acquisition>\n");
	
	// 无按键按下
	while(!_kbhit())
	{
		if (m_bIsOffLine)         // 掉线，进行掉线处理和重连
		{
	       // 掉线处理
		   __ProcessOffline();
		   
		   // 重连
		   __Recovery();
		}
		else                    // 未掉线，则获取图像
		{
			emStatus = GXGetImage(m_hDevice, &m_stFrameData, 500);
			if (emStatus == GX_STATUS_SUCCESS)
			{
				printf("Successfully get Image\n");
			}
			else
			{
				__GetErrorString(emStatus);
			}
		}
	}

	_getch();
}

//---------------------------------------------------------------------------------
/**
\brief    相机掉线处理

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__ProcessOffline()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	printf("**********************Process Offline**********************\r");

	// 如果开采，则停止采集
	if (m_bIsSanp)
	{
		// 发送停止采集命令
		printf("\n<Send stop snap command to device>\n");
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
		}
		m_bIsSanp = false;
	}

	// 如果打开，则关闭
	if (m_bIsOpen)
	{
		// 注销设备掉线事件回调
	    printf("<Unregister device Offline callback>\n");
  	    emStatus = GXUnregisterDeviceOfflineCallback(m_hDevice, m_hCB);
	    if (emStatus != GX_STATUS_SUCCESS)
	    {
	       __GetErrorString(emStatus);
	    }
		
		// 关闭相机
		printf("<Close device>\n");
		emStatus = GXCloseDevice(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
		}
		m_hDevice = NULL;
		
		// 释放资源
		if (m_stFrameData.pImgBuf != NULL)
		{
			delete[] m_stFrameData.pImgBuf;
			m_stFrameData.pImgBuf = NULL;
		}
		m_bIsOpen = false;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    重连相机

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Recovery()
{
	GX_STATUS         emStatus     = GX_STATUS_SUCCESS;   
	uint32_t          nDeviceNum   = 0;     // 设备个数   
	uint32_t          ui32PayLoadSize = 0;     // 原始Raw图像大小
	GX_OPEN_PARAM     stOpenParam;          // 打开相机参数

	printf("**********************Recovery**********************\r");
	
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		return;
	}

	// 判断当前连接设备个数
	if (nDeviceNum <= 0)
	{
		return;
	}

	// 在打开设备之前对句柄进行判断，可以保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_hDevice = NULL;
	}
	
	// 通过已知的MAC地址打开设备
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_MAC;
	stOpenParam.pszContent = m_chMACAddress;
	printf("\n<Open Device by MAC %s>\n", m_chMACAddress);
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}

	// 导入配置文件
	printf("<Import config file>\n");
	emStatus = GXImportConfigFile(m_hDevice, m_strSavePath.c_str());
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	
	// 给图像分配buffer
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete[] m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}

	GX_DS_HANDLE hStream = NULL;
	emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1 ,&hStream);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	emStatus = GXGetPayLoadSize(hStream,&ui32PayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}

	m_stFrameData.pImgBuf = new(std::nothrow) BYTE[(size_t)ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return;
	}

	// 注册掉线回调函数
	printf("<Register device Offline callback>\n");
	emStatus = GXRegisterDeviceOfflineCallback(m_hDevice, this, __OnDeviceOfflineCallbackFun, &m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	 m_bIsOpen = true;

	 // 发送开采命令
	printf("<Send start snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	m_bIsSanp = true;
	m_bIsOffLine = false;
}

//----------------------------------------------------------------------------------
/**
\brief  获取错误信息描述
\param  emErrorStatus  错误码

\return void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__GetErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// 获取错误描述信息长度
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (pchErrorInfo == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return ;
	}
	
	// 获取错误信息描述
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		printf("<GXGetLastError接口调用失败!>\n");
	}

	printf("%s\n", (LPCTSTR)pchErrorInfo);

	// 释放资源
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  掉线重连对外接口函数

\return void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::GigeRecovery()
{
	bool bReturn = false;

	// 打开设备
	bReturn = __OnOpenDevice();
	if (!bReturn)
	{
		return;
	}

	// 开始采集
	bReturn = __OnStartSnap();
	if (!bReturn)
	{
		__OnCloseDevice();
		return;
	}

	// 循环采集
	__Acquisition();	

	// 停止采集
	__OnStopSnap();
		
	// 关闭设备
	__OnCloseDevice();
}





