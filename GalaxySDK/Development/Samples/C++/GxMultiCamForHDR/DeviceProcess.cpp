//------------------------------------------------------------
/**
\file    DeviceProcess.cpp 
\brief   Device Process 
*/
//------------------------------------------------------------

#include "StdAfx.h"
#include "DeviceProcess.h"

CDeviceProcess::CDeviceProcess(void)
{
	m_pBitmap                   = NULL;
	m_pSampleCaptureEventHandle = NULL;
	m_bIsOpen                   = false;
	m_bIsSnap                   = false;
	m_bShowDeviceFPS            = true;
	m_nDeviceIndex              = 0;
	m_strDeviceSN               = "";
	m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();
	m_pRecordDeviceSNFPS        = new char[1024];
    m_pWnd                      = NULL;
}

CDeviceProcess::~CDeviceProcess(void)
{
	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}

	if (m_pRecordDeviceSNFPS != NULL)
	{
		delete[] m_pRecordDeviceSNFPS;
		m_pRecordDeviceSNFPS = NULL;
	}
}

//------------------------------------------------------------
/**
\brief   Open Device 
\param   strDeviceSN   [in]         设备序列号
\param   m_pWnd        [in]         窗口句柄
\param   nDeviceIndex  [in]         设备序号
\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::OpenDevice(gxstring strDeviceSN, CWnd* pWnd,int nDeviceIndex)
{
	bool bIsDeviceOpen   = false;      ///< 判断设备是否已打开标志
	bool bIsStreamOpen   = false;      ///< 判断设备流是否已打开标志
    bool bIsAcqModeEnable      = false;
    bool bIsTriggerModeEnable  = false;

	try
	{
		if (m_bIsOpen || pWnd == NULL || strDeviceSN == "")
		{
			throw std::exception("无效参数!");
		}

        m_pWnd          = pWnd;
		m_strDeviceSN   = strDeviceSN;
		m_nDeviceIndex  = nDeviceIndex;

		//打开设备
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strDeviceSN,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//获取属性控制器
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

        //设置采集模式连续采集
        __GetControlStatus("AcquisitionMode", bIsAcqModeEnable);
        if(bIsAcqModeEnable)
        {
            m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
        }

		//设置触发模式为关
        __GetControlStatus("TriggerMode", bIsTriggerModeEnable);
        if(bIsTriggerModeEnable)
        {
		    m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
        }

		//获取设备流个数
		int nCount = m_objDevicePtr->GetStreamCount();

		if (nCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			m_objStreamFeatureControlPtr = m_objStreamPtr->GetFeatureControl();
			bIsStreamOpen = true;
		}
		else
		{
			throw std::exception("未发现设备流!");
		}

        // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
        // 以提高网络相机的采集性能,设置方法参考以下代码。
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

		m_bIsOpen = true;

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CGXBitmap(m_objDevicePtr,pWnd);
	}
	catch (CGalaxyException& e)
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

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;

	}
	catch (std::exception& e)
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

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;
	}

}
//------------------------------------------------------------
/**
\brief   Close Device 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::CloseDevice()
{
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
		//判断是否停止采集
		if (m_bIsSnap)
		{

			//发送停采命令
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			m_objStreamPtr->StopGrab();

			//注销回调函数
			m_objStreamPtr->UnregisterCaptureCallback();

		}
	}
	catch (CGalaxyException)
	{

	}

	try
	{
		//关闭流对象
		m_objStreamPtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//关闭设备
		m_objDevicePtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	m_bIsSnap = false;
	m_bIsOpen = false;

	// 释放为图像显示准备的资源

	if (m_pBitmap != NULL)
	{
		m_pBitmap->UnPrepareForShowImg();
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}
//------------------------------------------------------------
/**
\brief   Start Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StartSnap()
{
	//判断设备是否已打开
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
        try
        {
            //设置Buffer处理模式
            m_objStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
        }
        catch (...)
        {
        }
		
		//注册回调函数
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);
	}
	catch (CGalaxyException& e)
	{
		throw e;
	}

	try
	{

		//开启流层采集
		m_objStreamPtr->StartGrab();
	}
	catch (CGalaxyException& e)
	{
		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}

	try
	{
		// 为图像显示准备资源
		m_pBitmap->PrepareForShowImg();
		//发送开采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		m_bIsSnap = true;
	}
	catch (CGalaxyException& e)
	{
		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();

		// 释放为图像显示准备的资源
		m_pBitmap->UnPrepareForShowImg();
		throw e;
	}
	catch (std::exception& e)
	{
		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();

		// 释放为图像显示准备的资源
		m_pBitmap->UnPrepareForShowImg();
		throw e;
	}

}

//------------------------------------------------------------
/**
\brief   Stop Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StopSnap()
{
	//判断设备是否已打开
	if (!m_bIsOpen || !m_bIsSnap)
	{
		return;
	}

	try
	{
		//发送停采命令
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//关闭流层采集
		m_objStreamPtr->StopGrab();

		//注销回调函数
		m_objStreamPtr->UnregisterCaptureCallback();

		//帧率置0
		m_objfps.Reset();

		m_bIsSnap = false;
	}
	catch (CGalaxyException& e)
	{
		throw e;

	}
	catch (std::exception& e)
	{
		throw e;

	}
}

//------------------------------------------------------------
/**
\brief   Get Device Open Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsOpen() const
{
	return m_bIsOpen;
}

//------------------------------------------------------------
/**
\brief   Get Device Snap Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsSnap() const
{
	return m_bIsSnap;
}

//------------------------------------------------------------
/**
\brief   Set Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::SetShowFPS(bool bIsShowFps)
{
	m_bShowDeviceFPS = bIsShowFps;
}

//------------------------------------------------------------
/**
\brief   Get Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::GetShowFPS()
{
	return m_bShowDeviceFPS;
}


//------------------------------------------------------------
/**
\brief   Refresh Device SN and FPS

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::RefreshDeviceSNFPS()
{
	double dFps = 0;        ///< 帧率

	//更新帧率
	m_objfps.UpdateFps();

	//获取最近一次的帧率
	dFps = m_objfps.GetFps();

	sprintf_s(m_pRecordDeviceSNFPS, 1024,
		"序号:%d SN:%s FPS:%.2f",
		m_nDeviceIndex,
		m_strDeviceSN.c_str(),
		dFps);
}

//------------------------------------------------------------
/**
\brief   Show Image
\param   objImageDataPointer [in]       图像信息

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::__ShowPicture(CImageDataPointer& objImageDataPointer)
{
	if (m_bShowDeviceFPS)
	{
		//增加帧数
		m_objfps.IncreaseFrameNum();

		//显示图像及帧率
		m_pBitmap->Show(objImageDataPointer,m_pRecordDeviceSNFPS);
	}
	else
	{
		//显示图像
		m_pBitmap->Show(objImageDataPointer);
	}
}

//------------------------------------------------------------
/**
\brief   更新图像信息
\param   pWnd   [in]       当前子窗口句柄

\return
*/
//------------------------------------------------------------
void CDeviceProcess::UpdataImgInfo(void)
{
    try
    {
        if (m_pBitmap != NULL)
        {
            delete m_pBitmap;
            m_pBitmap = NULL;
        }
        m_pBitmap = new CGXBitmap(m_objDevicePtr, m_pWnd);
    }
    catch (CGalaxyException& e)
    {
        throw e;
    }
    catch (std::exception& e)
    {
        throw e;
    }
}

//------------------------------------------------------------
/**
\brief   获取属性节点是否支持且可写
\param   strName    [in]        属性名
\param   bIsEnable  [out]       是否使能

\return
*/
//------------------------------------------------------------
void CDeviceProcess::__GetControlStatus(const gxstring &strName, bool &bIsEnable)
{
    bool    bIsImplement    = false;
    bool    bIsWritable     = false;
    bool    bIsReadable     = false;

    // 是否支持该节点
    bIsImplement = m_objFeatureControlPtr->IsImplemented(strName);
    if(!bIsImplement)
    {
        bIsEnable = false;
        return;
    }
    else
    {
        // 判断该节点是否可读
        bIsReadable = m_objFeatureControlPtr->IsReadable(strName);
        if(!bIsReadable)
        {
            bIsEnable = true;
            return;
        }

        // 判断该节点是否可写
        bIsWritable = m_objFeatureControlPtr->IsWritable(strName);

        // 支持且可写
        if(bIsWritable)
        {
            bIsEnable = true;
        }
        else
        {
            bIsEnable = false;
        }
    }
}

//------------------------------------------------------------
/**
\brief   获取设备型号信息
\param   strDisplayName [out]       设备DisplayName

\return
*/
//------------------------------------------------------------
void CDeviceProcess::GetDevDisplayName(CString &strDisplayName)
{
    gxstring strDeviceModelName = m_objFeatureControlPtr->GetStringFeature("DeviceModelName")->GetValue();
    strDisplayName.Format("序号:%d %s(%s)", m_nDeviceIndex, strDeviceModelName.c_str(), m_strDeviceSN.c_str());
}
