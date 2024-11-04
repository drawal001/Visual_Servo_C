using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxCameraEvents
{
    public partial class GxCameraEvents : Form
    {
        bool                           m_bIsOpen                  = false;                ///< 相机打开标识 
        bool                           m_bIsSnap                  = false;                ///< 相机开始采集标识
        IGXFactory                     m_objIGXFactory            = null;                 ///<Factory对像
        IGXDevice                      m_objIGXDevice             = null;                 ///<设备对像
        IGXStream                      m_objIGXStream             = null;                 ///<流对像
        IGXFeatureControl              m_objIGXFeatureControl     = null;                 ///<远端设备属性控制器对像
        IGXFeatureControl              m_objIGXStreamFeatureControl = null;               ///<流层属性控制器对象
        GX_FEATURE_CALLBACK_HANDLE     m_hFeatureCallback         = null;                 ///<Feature事件的句柄
        CStopWatch                     m_objImageTime             = null;                 ///< 收到图像的时间
        CStopWatch                     m_objShutterTime           = null;                 ///< 曝光完成事件时间
        bool                           m_bIsTrigValid             = true;                 ///< 触发是否有效标志:当一次触发正在执行时，将该标志置为false                                                                                  ///
                                            
        public GxCameraEvents()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 更新界面
        /// </summary>
        private void __UpdateUI()
        {
            m_btn_OpenDevice.Enabled = !m_bIsOpen;
            m_btn_CloseDevice.Enabled = m_bIsOpen;
            m_btn_StartDevice.Enabled = m_bIsOpen && !m_bIsSnap;
            m_btn_StopDevice.Enabled = m_bIsSnap;
            m_btn_SoftTriggerCommand.Enabled = m_bIsOpen && m_bIsSnap;
        }

        /// <summary>
        /// 相机初始化
        /// </summary>
        private void __InitDevice()
        {
            if (null != m_objIGXFeatureControl)
            {
                //设置采集模式连续采集
                m_objIGXFeatureControl.GetEnumFeature("AcquisitionMode").SetValue("Continuous");

                //设置触发模式为开
                m_objIGXFeatureControl.GetEnumFeature("TriggerMode").SetValue("On");

                //选择触发源为软触发
                m_objIGXFeatureControl.GetEnumFeature("TriggerSource").SetValue("Software");

                //选择曝光完成事件
                m_objIGXFeatureControl.GetEnumFeature("EventSelector").SetValue("ExposureEnd");

                //曝光完成事件使能
                m_objIGXFeatureControl.GetEnumFeature("EventNotification").SetValue("On");

            }
        }


        /// <summary>
        /// 加载窗体执行初始化UI和库
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxCameraEvents_Load(object sender, EventArgs e)
        {
            try
            {
                //刷新界面
                __UpdateUI();

                m_objImageTime = new CStopWatch();
                m_objShutterTime = new CStopWatch();
                m_objIGXFactory = IGXFactory.GetInstance();
                m_objIGXFactory.Init();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 关闭流
        /// </summary>
        private void __CloseStream()
        {
            try
            {
                //关闭流
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.Close();
                    m_objIGXStream = null;
                    m_objIGXStreamFeatureControl = null;
                }
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 关闭设备
        /// </summary>
        private void __CloseDevice()
        {
            try
            {
                //关闭设备
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.Close();
                    m_objIGXDevice = null;
                }
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 停止采集关闭设备、关闭流
        /// </summary>
        private void __CloseAll()
        {
            try
            {
                // 如果未停采则先停止采集
                if (m_bIsSnap)
                {
                    if (null != m_objIGXFeatureControl)
                    {
                        m_objIGXFeatureControl.GetCommandFeature("AcquisitionStop").Execute();
                    }
                }
            }
            catch (Exception)
            {
            }

            m_bIsSnap = false;

            try
            {
                //停止流通道、注销采集回调和关闭流
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StopGrab();
                    //注销采集回调函数
                    m_objIGXStream.UnregisterCaptureCallback();
                    m_objIGXStream.Close();
                    m_objIGXStream = null;
                    m_objIGXStreamFeatureControl = null;
                }
            }
            catch (Exception)
            {
            }

            try
            {
                // 注销曝光结束事件回调函数
                if (null != m_objIGXFeatureControl)
                {
                    if (null != m_hFeatureCallback)
                    {
                        m_objIGXFeatureControl.UnregisterFeatureCallback(m_hFeatureCallback);
                        m_objIGXFeatureControl = null;
                    }
                }
            }
            catch (Exception)
            {
            }

            //关闭设备
            __CloseDevice();
            m_bIsOpen = false;
        }

    
        /// <summary>
        /// 打开设备打开流
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_OpenDevice_Click(object sender, EventArgs e)
        {
            try
            {
                List<IGXDeviceInfo> listGXDeviceInfo = new List<IGXDeviceInfo>();

                //关闭流
                __CloseStream();
                // 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
                __CloseDevice();

                m_objIGXFactory.UpdateAllDeviceList(200, listGXDeviceInfo);

                // 判断当前连接设备个数
                if (listGXDeviceInfo.Count <= 0)
                {
                    MessageBox.Show("未发现设备!");
                    return;
                }

                //打开列表第一个设备
                m_objIGXDevice = m_objIGXFactory.OpenDeviceBySN(listGXDeviceInfo[0].GetSN(), GX_ACCESS_MODE.GX_ACCESS_EXCLUSIVE);
                m_objIGXFeatureControl = m_objIGXDevice.GetRemoteFeatureControl();


                __InitDevice();

                //打开流
                if (null != m_objIGXDevice)
                {
                    m_objIGXStream = m_objIGXDevice.OpenStream(0);
                    m_objIGXStreamFeatureControl = m_objIGXStream.GetFeatureControl();
                }

                // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
                // 以提高网络相机的采集性能,设置方法参考以下代码。
                GX_DEVICE_CLASS_LIST objDeviceClass = m_objIGXDevice.GetDeviceInfo().GetDeviceClass();
                if(GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_GEV == objDeviceClass)
                {
                    // 判断设备是否支持流通道数据包功能
                    if(true == m_objIGXFeatureControl.IsImplemented("GevSCPSPacketSize"))
                    {
                        // 获取当前网络环境的最优包长值
                        uint nPacketSize = m_objIGXStream.GetOptimalPacketSize();
                        // 将最优包长值设置为当前设备的流通道包长值
                        m_objIGXFeatureControl.GetIntFeature("GevSCPSPacketSize").SetValue(nPacketSize);
                    }
                }

                // 更新设备打开标识
                m_bIsOpen = true;

                //刷新界面
                __UpdateUI();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }      
        }

        /// <summary>
        /// 开始采集
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_StartDevice_Click(object sender, EventArgs e)
        {
            try
            {
                // 注册曝光结束事件回调函数
                if (null != m_objIGXFeatureControl)
                {
                    //RegisterFeatureCallback第二个参数属于用户自定参数(类型必须为引用
                    //类型)，若用户想用这个参数可以在委托函数中进行使用
                    m_hFeatureCallback = m_objIGXFeatureControl.RegisterFeatureCallback("EventExposureEnd", null, __OnFeatureCallbackFun);
                }

                if (null != m_objIGXStreamFeatureControl)
                {
                    try
                    {
                        //设置流层Buffer处理模式为OldestFirst
                        m_objIGXStreamFeatureControl.GetEnumFeature("StreamBufferHandlingMode").SetValue("OldestFirst");
                    }
                    catch (Exception)
                    {
                    }
                }

                //开启采集流通道
                if (null != m_objIGXStream)
                {
                    //RegisterCaptureCallback第一个参数属于用户自定参数(类型必须为引用
                    //类型)，若用户想用这个参数可以在委托函数中进行使用
                    m_objIGXStream.RegisterCaptureCallback(null, __OnFrameCallbackFun);
                    m_objIGXStream.StartGrab();
                }

                //发送开采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStart").Execute();
                }
                m_bIsSnap = true;
                m_bIsTrigValid = true;

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 发送软触发命令
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_SoftTriggerCommand_Click(object sender, EventArgs e)
        {
            try
            {
                // 如果当触发回调正在执行的过程中，再次点击触发按键后，此次点击会被屏蔽掉
                if (!m_bIsTrigValid)
                {
                    return;
                }
                else
                {
                    m_bIsTrigValid = false;
                }

                // 发送软触发命令的时间
                string strTimeNow = "\r\n开始发送软触发命令的时间点:" + DateTime.Now.Hour.ToString("D2")
                                    + "h:" + DateTime.Now.Minute.ToString("D2")
                                    + "m:" + DateTime.Now.Second.ToString("D2")
                                    + "s:" + DateTime.Now.Millisecond.ToString("D3") + "ms" + "\r\n";
                m_txt_TimeLog.AppendText(strTimeNow);

                //发送软触发命令
                if (null != m_objIGXFeatureControl)
                {                
                    // 到收到图像历时时间：计时开始
                    m_objImageTime.Start();

                    // 到曝光结束事件历时时间：计时开始
                    m_objShutterTime.Start();

                    m_objIGXFeatureControl.GetCommandFeature("TriggerSoftware").Execute();
                }


            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                m_bIsTrigValid = true;
            }  
        }

        /// <summary>
        /// 停止采集
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_StopDevice_Click(object sender, EventArgs e)
        {
            try
            {
                //发送停采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStop").Execute();
                }

                //关闭采集流通道
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StopGrab();
                    //注销采集回调函数
                    m_objIGXStream.UnregisterCaptureCallback();
                }

                // 注销曝光结束事件回调函数
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.UnregisterFeatureCallback(m_hFeatureCallback);
                }

                m_bIsSnap = false;

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }  
        }

        /// <summary>
        /// 关闭设备
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_CloseDevice_Click(object sender, EventArgs e)
        {
            try
            {
                //关闭所有
                __CloseAll();

                //刷新界面
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 窗体关闭、关闭流、关闭设备、反初始化
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxCameraEvents_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                //关闭所有
                __CloseAll();

                //反初始化
                if (null != m_objIGXFactory)
                {
                    m_objIGXFactory.Uninit();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 采集事件的委托函数
        /// </summary>
        /// <param name="objUserParam">用户私有参数</param>
        /// <param name="objIFrameData">图像信息对象</param>
        private void __OnFrameCallbackFun(object objUserParam, IFrameData objIFrameData)
        {
            if (InvokeRequired)
            {
                this.BeginInvoke(new CaptureDelegate(__OnFrameCallbackFun), new object[] { objUserParam, objIFrameData });
                return;
            }
            double dElapsedtime = 0;

            try
            {
                // 结束计时
                dElapsedtime = m_objImageTime.Stop();
                string strImageTime = "开始发送软触发命令到成功收到图像历时：" + dElapsedtime.ToString("F2") + "ms" + "\r\n";
                __ShowTimeLog(strImageTime);
            }
            catch (Exception)
            {

            }

            m_bIsTrigValid = true;
        }


        /// <summary>
        /// 曝光结束事件的委托函数
        /// </summary>
        /// <param name="objUserParam">用户私有参数</param>
        /// <param name="strFeatureName">功能名称字符串</param>
        private void __OnFeatureCallbackFun(object objUserParam, string strFeatureName)
        {
           
            double dElapsedtime = 0;
            try
            {
                if (InvokeRequired)
                {
                    this.BeginInvoke(new FeatureDelegate(__OnFeatureCallbackFun), new object[] { objUserParam, strFeatureName });
                    return;
                }

                if ("EventExposureEnd" == strFeatureName)
                {
                    // 结束计时
                    dElapsedtime = m_objShutterTime.Stop();
                    string strShutterTime = "开始发送软触发命令到曝光结束事件历时：" + dElapsedtime.ToString("F2") + "ms" + "\r\n";
                    __ShowTimeLog(strShutterTime);
                }
            }
            catch (Exception)
            {

            }
        }

        /// <summary>
        /// 把时间更新到文本框
        /// </summary>
        /// <param name="strTimeLogInfo">显示类容</param>
        private void __ShowTimeLog(string strTimeLogInfo)
        {
            m_txt_TimeLog.AppendText(strTimeLogInfo); 
        }

    }
}
