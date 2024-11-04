//----------------------------------------------------------------------------------
/**
\brief  CGxCameraEventsDlg class declaration
*/
//----------------------------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "StopWatch.h"

// CGxCameraEventsDlg 对话框
class CGxCameraEventsDlg : public CDialog
{
public:

	/// 标准构造函数
	CGxCameraEventsDlg(CWnd* pParent = NULL);	

	/// 对话框数据
	enum { IDD = IDD_GXCAMERAEVENTS_DIALOG };

protected:

	/// DDX/DDV 支持
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	HICON m_hIcon;

	/// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOpenDevice();
	afx_msg void OnBnClickedStartSnap();
	afx_msg void OnBnClickedStopSnap();
	afx_msg void OnBnClickedCloseDevice();
	afx_msg void OnBnClickedSendSoftCommand();
	afx_msg void OnClose();

public:
	/// 打印时间信息到编辑窗口
	void PrintInfotoWnd(std::string &strInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStopWatch                      m_objImageTime;               ///< 收到图像的时间
	CStopWatch                      m_objShutterTime;             ///< 曝光完成事件时间
	bool                            m_bIsTrigValid;               ///< 触发是否有效标志:当一次触发正在执行时，将该标志置为false

	//---------------------------------------------------------------------------------
	/**
	\brief    用户处理采集回调类
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandle :public ICaptureEventHandler
	{
	public:

		//---------------------------------------------------------------------------------
		/**
		\brief    采集回调函数
		\param    objImageDataPointer		采集信息对象
		\param	  pUserParam                用户私有参数

		\return   void
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			CGxCameraEventsDlg* pDlg      = (CGxCameraEventsDlg*)pUserParam;

			try
			{
				double              dSnaptime = 0;
				char                chTemp[8] = "ms";

				//获取采集图像时间,由于计时器精度问题，测量帧周期过小的相机的时候会出现误差
				dSnaptime = pDlg->m_objImageTime.Stop();
				CString strInformation = "";
				strInformation.Format("开始发送软触发命令到成功接收图像历时: %0.2f%s",dSnaptime,chTemp);
				std::string strPrintInformation = strInformation.GetBuffer(0);
				pDlg->PrintInfotoWnd(strPrintInformation);		
			}
			catch (CGalaxyException)
			{
				//do nothing
			}
			catch (std::exception)
			{
				//do nothing
			}

			pDlg->m_bIsTrigValid = true;
		}
	};

	//---------------------------------------------------------------------------------
	/**
	\brief    用户处理曝光结束事件回调类
	*/
	//----------------------------------------------------------------------------------
	class CSampleFeatureEventHandle :public IFeatureEventHandler
	{
	public:

		//---------------------------------------------------------------------------------
		/**
		\brief    曝光事件回调函数
		\param    strFeatureName		事件名称
		\param	  pUserParam            用户私有参数

		\return   void
		*/
		//----------------------------------------------------------------------------------
		void DoOnFeatureEvent(const GxIAPICPP::gxstring& strFeatureName, void* pUserParam)
		{
			try
			{
				CGxCameraEventsDlg*  pDlg             = (CGxCameraEventsDlg*)pUserParam;
				double              dElapsedtime      = 0;
				char                chTemp[8]         = "ms";
				if (strFeatureName == "EventExposureEnd")
				{
					//获取曝光结束事件时间,由于计时器精度问题，测量帧周期过小的相机的时候会出现误差
					dElapsedtime = pDlg->m_objShutterTime.Stop();

					//记录曝光结束时间
					CString strInformation = "";
					strInformation.Format("开始发送软触发命令到曝光结束时间历时: %0.2f%s",dElapsedtime,chTemp);
					std::string strPrintInformation = strInformation.GetBuffer(0);
					pDlg->PrintInfotoWnd(strPrintInformation);
				}
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

private:

	/// 判断相机是否支持曝光结束事件
	bool __IsSupportExposureEnd();

	/// 初始化相机参数
	void __InitParam();

	/// 更新界面
	void __UpdateUI();

	CGXDevicePointer                m_objDevicePtr;               ///< 设备句柄
	CGXStreamPointer                m_objStreamPtr;               ///< 流对象
	CGXFeatureControlPointer        m_objFeatureControlPtr;       ///< 设备控制器对象
	CGXFeatureControlPointer        m_objStreamFeatureControlPtr; ///< 流层控制器对象
	GX_FEATURE_CALLBACK_HANDLE      m_hCB;                        ///< 曝光结束事件回调句柄
	void*                           m_pLogWnd;                    ///< 指向时间记录窗口
	CSampleCaptureEventHandle*      m_pCaptureEventHandle;        ///< 采集回调指针
	CSampleFeatureEventHandle*      m_pFeatureEventHandle;        ///< 事件回调指针
	bool                            m_bIsOpen;                    ///< 设备是否打开标识
	bool                            m_bIsSnap;                    ///< 是否采集标识
};