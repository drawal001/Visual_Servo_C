//------------------------------------------------------------
/**
\file    DeviceProcess.h 
\brief   Device Process 
*/
//------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "Fps.h"

//---------------------------------------------------------------------------------
/**
\brief   设备处理类
*/
//----------------------------------------------------------------------------------
class CDeviceProcess
{
public:

	CDeviceProcess(void);
	~CDeviceProcess(void);

	//---------------------------------------------------------------------------------
	/**
	\brief   用户继承采集事件处理类
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler :public ICaptureEventHandler
	{
		//---------------------------------------------------------------------------------
		/**
		\brief   采集回调函数
		\param   objImageDataPointer      图像处理参数
		\param   pFrame                   用户参数

		\return  无
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CDeviceProcess* pDeviceProcess = (CDeviceProcess*)pUserParam;

				//显示图像
				pDeviceProcess->__ShowPicture(objImageDataPointer);
			}
			catch (CGalaxyException)
			{
				//do nothing

			}
			catch (std::exception)
			{
				//do nothing
			}
		}
	};

public:
	/// 打开设备
	void OpenDevice(gxstring strDeviceSN, CWnd* m_pWnd,int nDeviceIndex);

	/// 关闭设备
	void CloseDevice();

	/// 开始采集
	void StartSnap();

	/// 停止采集
	void StopSnap();

	/// 获取设备打开标识
	bool IsOpen() const;

	/// 获取设备采集标识
	bool IsSnap() const;

	/// 设置是否显示帧率标志
	void SetShowFPS(bool bIsShowFps);

	/// 获取是否显示帧率标志
	bool GetShowFPS();

	/// 刷新设备型号及帧率信息
	void RefreshDeviceSNFPS();

	/// 获取是否为彩色
	bool GetIsColor();

	CGXFeatureControlPointer    m_objFeatureControlPtr;           ///< 属性控制器
	CGXFeatureControlPointer    m_objStreamFeatureControlPtr;     ///< 流层控制器对象

private:

	/// 显示图像
	void __ShowPicture(CImageDataPointer& objImageDataPointer);

	CGXDevicePointer            m_objDevicePtr;                  ///< 设备句柄
	CGXStreamPointer            m_objStreamPtr;                  ///< 设备流对象
	CGXBitmap*                  m_pBitmap;                       ///< 画图指针
	CSampleCaptureEventHandler* m_pSampleCaptureEventHandle;     ///< 回调指针
	bool                        m_bIsOpen;                       ///< 设备是否打开标志
	bool                        m_bIsSnap;                       ///< 设备是否开采标志
	bool                        m_bShowDeviceFPS;                ///< 显示设备帧率标志
	char                        *m_pRecordDeviceSNFPS;           ///< 设备型号及帧率信息
	gxstring                    m_strDeviceSN;                   ///< 设备序列号
	CFps                        m_objfps;                        ///< 计算帧率
	int							m_nDeviceIndex;                  ///< 设备序号
};
