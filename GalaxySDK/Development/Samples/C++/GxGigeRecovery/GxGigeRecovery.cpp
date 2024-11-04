//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.cpp 
\brief   CGxGigeRecovery class declaration implementation
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "stdio.h"
#include "GxGigeRecovery.h"

using namespace std;

//---------------------------------------------------------------------------------
/**
\brief   构造函数

\return   无
*/
//----------------------------------------------------------------------------------
CGigeRecovery::CGigeRecovery()
{
	try
	{
		m_hCB              = NULL;
		m_strSavePath      = "";
		m_strMACAddress    = "";
		m_bIsOffline       = false;
		m_bIsOpen          = false;
		m_bIsSnap          = false;
		m_pDeviceOfflineEventHandle = new CSampleDeviceOfflineEventHandle();

		//初始化设备库
		IGXFactory::GetInstance().Init();

		//获取当前路径，用于保存设备配置参数
		char chModulePath[512] = {0};
		string strPath       = "";
		::GetModuleFileName(NULL,(LPTSTR)chModulePath,512);
		strPath = chModulePath;
		strPath = strPath.substr(0,strPath.rfind('\\')+1);
        strPath = strPath + "ConfigFile.ini";
		m_strSavePath  = strPath.c_str();
	}
	catch (CGalaxyException &e)
	{
		if (m_pDeviceOfflineEventHandle != NULL)
		{
			delete m_pDeviceOfflineEventHandle;
		}
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;		
	}
	catch (std::exception &e)
	{
		if (m_pDeviceOfflineEventHandle != NULL)
		{
			delete m_pDeviceOfflineEventHandle;
		}
		cout<<"<"<<e.what()<<">"<<endl;		
	}

}

//---------------------------------------------------------------------------------
/**
\brief   析构函数

\return   无
*/
//----------------------------------------------------------------------------------
CGigeRecovery::~CGigeRecovery()
{
	try
	{
		//关闭设备库
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;			
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;			
	}

	if (m_pDeviceOfflineEventHandle != NULL)
	{
		delete m_pDeviceOfflineEventHandle;
		m_pDeviceOfflineEventHandle = NULL;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   打开设备函数

\return   true：打开设备成功，false：打开设备失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnOpenDevice() throw()
{
	bool bIsDeviceOpen = false;              ///< 设备是否已打开标识
	bool bIsStreamOpen = false;              ///< 设备流是否已打开标识

	try
	{
		cout<<"====================CGigeRecovery::__OnOpenDevice()===================="<<endl;

		//子网枚举设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);

		//判断当前设备连接个数
		if (vectorDeviceInfo.size() <= 0)
		{
			cout<<"No Device!"<<endl;
			return m_bIsOpen;
		}

		//判断枚举到的设备是否为Gige
		if (vectorDeviceInfo[0].GetDeviceClass() != GX_DEVICE_CLASS_GEV)
		{
			cout<<"The device is not GEV"<<endl;
			return m_bIsOpen;
		}        	
        
		//通过MAC地址打开设备
		m_strMACAddress = vectorDeviceInfo[0].GetMAC();	
		cout<<"<Open device by MAC:"<<m_strMACAddress<<endl;
		m_objDevicePtr          = IGXFactory::GetInstance().OpenDeviceByMAC(m_strMACAddress,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr  = m_objDevicePtr->GetRemoteFeatureControl();
		int32_t nStreamCount    = m_objDevicePtr->GetStreamCount();

		//判断流个数
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			m_objStreamFeatureControlPtr = m_objStreamPtr->GetFeatureControl();
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("Not Find Device Stream");
		}

        // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，        // 以提高网络相机的采集性能,设置方法参考以下代码。
        GX_DEVICE_CLASS_LIST objDeviceClass = m_objDevicePtr->GetDeviceInfo().GetDeviceClass();
        if(GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            // 判断设备是否支持流通道数据包功能
            if(true == m_objFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                // 获取当前网络环境的最优包长值
                int nPacketSize = m_objStreamPtr->GetOptimalPacketSize();
                // 将最优包长值设置为当前设备的流通道包长值
                m_objFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            } 
        }

		//初始化设备参数
		cout<<"<Initialize the device parameters>"<<endl;
		__InitParam();

		//导出配置参数到文件
		cout<<"<Export config file>"<<endl;
		m_objDevicePtr->ExportConfigFile(m_strSavePath);

		//注册掉线回调函数
		cout<<"<Register device Offline callback>"<<endl;
		m_hCB = m_objDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);
		m_bIsOpen = true;
		return m_bIsOpen;

	}
	catch (CGalaxyException &e)
	{
		//判断设备流是否已关闭
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已关闭
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
	catch (std::exception &e)
	{
		//判断设备流是否已关闭
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已关闭
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   初始化参数函数

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__InitParam()
{
	//设置采集模式为连续
	m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");

	//设置触发模式为关
	m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");

	//设置心跳超时时间为1s
	//针对千兆网相机，程序在Debug模式下调试运行时，相机的心跳超时时间自动设置为5min，
	//这样做是为了不让相机的心跳超时影响程序的调试和单步执行，同时这也意味着相机在这5min内无法断开，除非使相机断电再上电
	//为了解决掉线重连问题，将相机的心跳超时时间设置为1s，方便程序掉线后可以重新连接
	m_objFeatureControlPtr->GetIntFeature("GevHeartbeatTimeout")->SetValue(1000);

	//设置Buffer处理模式

    try
    {
        m_objStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
    }
    catch (...)
    {
    }

}

//---------------------------------------------------------------------------------
/**
\brief   开始采集

\return   true：采集成功，false：采集失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStartSnap() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnStartSnap()===================="<<endl;

		//开启流层通道
		m_objStreamPtr->StartGrab();

		//发送开采命令
		cout<<"<Send start snap command to device>"<<endl;
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap = true;
		return m_bIsSnap;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}

}

//---------------------------------------------------------------------------------
/**
\brief   停止采集

\return   true：停止采集成功，false：停止采集失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStopSnap() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnStopSnap()===================="<<endl;

		//发送停采命令
		cout<<"<Send stop snap command to device>"<<endl;
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层通道
		m_objStreamPtr->StopGrab();
		m_bIsSnap = false;
		return m_bIsSnap;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsSnap;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   关闭设备

\return   true：关闭设备成功，false：关闭设备失败
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnCloseDevice() throw()
{
	try
	{
		cout<<"====================CGigeRecovery::__OnCloseDevice()===================="<<endl;

		//注销回调函数
		cout<<"<Unregister device Offline callback>"<<endl;
		m_objDevicePtr->UnregisterDeviceOfflineCallback(m_hCB);
		m_objStreamPtr->Close();

		//关闭设备
		cout<<"<Close Device>"<<endl;
		m_objDevicePtr->Close();
		m_bIsOpen = false;
		return m_bIsOpen;
	}
	catch (CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
		return m_bIsOpen;		
	}
}

//---------------------------------------------------------------------------------
/**
\brief   连续采集

\return   无
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Acquisition() throw()
{
	cout<<"====================CGigeRecovery::__Acquisition()===================="<<endl;;
	cout<<"<Press any key to stop acquisition>"<<endl;

	//无按键按下
	while(!_kbhit())
	{
		try
		{
			//判断设备是否掉线
			if (m_bIsOffline)
			{
				//掉线处理和重连
				__ProcessOffline();
				__Recovery();
			}
			else
			{
				m_objImageDataPtr = m_objStreamPtr->GetImage(500);
				printf("Successfully get Image\n");
			}
		}
		catch(CGalaxyException &e)
		{
			cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
		}
		catch(std::exception &e)
		{
			cout<<"<"<<e.what()<<">"<<endl;
		}

	}
	_getch();
}

//---------------------------------------------------------------------------------
/**
\brief   设备掉线处理

\return   无
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__ProcessOffline() throw()
{
	try
	{
		printf("**********************Process Offline**********************\r");
		//判断设备是否停止采集
		if (m_bIsSnap)
		{
			cout<<"\n<Send stop snap command to device>"<<endl;
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
		}
	}
	catch(CGalaxyException &e)
	{
		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
	}
	catch(std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
	}

	try
	{
		//判断设备是否停止采集
		if (m_bIsSnap)
		{
			m_objStreamPtr->StopGrab();
			m_bIsSnap = false;
		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
	catch(std::exception)
	{
		//do noting
	}

	try
	{
		//判断设备是否打开
		if (m_bIsOpen)
		{
			//注销掉线回调函数
			cout<<"<Unregister device Offline callback>"<<endl;
			m_objDevicePtr->UnregisterDeviceOfflineCallback(m_hCB);

			//关闭流和设备
			cout<<"<Close Device>"<<endl;
			m_objStreamPtr->Close();
			m_objDevicePtr->Close();
			m_bIsOpen = false;

		}
	}
	catch(CGalaxyException)
	{
		//do noting
	}
	catch(std::exception)
	{
		//do noting
	}

}

//---------------------------------------------------------------------------------
/**
\brief   设备重连

\return   无
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Recovery() throw()
{
	bool bIsDeviceOpen = false;              ///< 设备是否已打开标识
	bool bIsStreamOpen = false;              ///< 设备流是否已打开标识
	try
	{
        printf("**********************Recovery**********************\r");
		//子网枚举设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000,vectorDeviceInfo);
		//判断当前设备连接个数
		if (vectorDeviceInfo.size() <= 0)
		{
			return;
		}

		//打开设备
		cout<<"\n<Open device by MAC:"<<m_strMACAddress<<endl;
		m_objDevicePtr          = IGXFactory::GetInstance().OpenDeviceByMAC(m_strMACAddress,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;
		m_objFeatureControlPtr  = m_objDevicePtr->GetRemoteFeatureControl();

		//判断流个数
		int32_t nStreamCount    = m_objDevicePtr->GetStreamCount();
		if (nStreamCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			bIsStreamOpen = true;
		}
		else
		{
			throw exception("未发现设备流!");
		}

		//导入配置文件
		cout<<"<Import config file>"<<endl;
		m_objDevicePtr->ImportConfigFile(m_strSavePath);

		//注册掉线回调函数
		cout<<"<Register device Offline callback>"<<endl;
		m_hCB = m_objDevicePtr->RegisterDeviceOfflineCallback(m_pDeviceOfflineEventHandle,this);
		m_bIsOpen = true;

		//发送开采命令
		cout<<"<Send start snap command to device>"<<endl;
		m_objStreamPtr->StartGrab();
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
		m_bIsSnap    = true;
		m_bIsOffline = false;
	}
	catch(CGalaxyException &e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		cout<<"<"<<e.GetErrorCode()<<">"<<"<"<<e.what()<<">"<<endl;
	}
	catch(std::exception &e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		cout<<"<"<<e.what()<<">"<<endl;
	}
}

//---------------------------------------------------------------------------------
/**
\brief   设备掉线重连对外接口函数

\return   无
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::GigeRecovery()
{
	bool bReturn = false;
	try
	{
		//打开设备
		bReturn = __OnOpenDevice();
		if (!bReturn)
		{
			return;
		}

		//开始采集
		bReturn = __OnStartSnap();
		if (!bReturn)
		{
			//关闭设备
			__OnCloseDevice();
			return;
		}

		//循环采集
		__Acquisition();

		//停止采集
		__OnStopSnap();

		//关闭设备
		__OnCloseDevice();

	}
	catch (std::exception &e)
	{
		cout<<"<"<<e.what()<<">"<<endl;
	}

}

