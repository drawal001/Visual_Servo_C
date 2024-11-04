using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using GxIAPINET;

namespace GxGigeRecovery
{
   public  class CGigeRecovery
    {

        GX_DEVICE_OFFLINE_CALLBACK_HANDLE   m_hCB                     = null;                           ///<掉线回调句柄
        string                              m_strPath                 = "";                             ///<文件的当前路径
        string                              m_strSavePath             = "";                             ///<相机配置参数文件保存路径
        string                              m_strMACAddress           = "";                             ///<相机MAC地址
        bool                                m_bIsOffLine              = false;                          ///<相机掉线标识
        bool                                m_bIsOpen                 = false;                          ///<相机打开标识 
        bool                                m_bIsSanp                 = false;                          ///<相机采集标志
        IGXFactory                          m_objIGXFactory           = null;                           ///<Factory对像
        IGXDevice                           m_objIGXDevice            = null;                           ///<设备对像
        IGXStream                           m_objIGXStream            = null;                           ///<流对像
        IGXFeatureControl                   m_objIGXFeatureControl    = null;                           ///<远端设备属性控制器对像
        IGXFeatureControl                   m_objIGXStreamFeatureControl = null;                        ///<流层属性控制器对象



        /// <summary>
        /// 构造函数
        /// </summary>
        public CGigeRecovery()
        {
            try
            {
                // 初始化设备库
                m_objIGXFactory = IGXFactory.GetInstance();
                m_objIGXFactory.Init();
                m_strPath = System.Environment.CurrentDirectory + "\\";
                m_strSavePath = m_strPath + "ConfigFile.ini";
            }
            catch (CGalaxyException ex)
            {
                string strErrorInfo = "错误码为：" + ex.GetErrorCode().ToString() + "错误描述信息为：" + ex.Message;
                Console.WriteLine(strErrorInfo);
                System.Environment.Exit(System.Environment.ExitCode);
            }
        }

        /// <summary>
        /// GigeRecovery掉线重连对外接口函数
        /// </summary>
        public void GigeRecovery()
        {
            bool bReturn = false;
            try
            {
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
            catch (CGalaxyException ex)
            {
                string strErrorInfo = "错误码为：" + ex.GetErrorCode().ToString() + "错误描述信息为：" + ex.Message;
                Console.WriteLine(strErrorInfo);
            }
        }

        /// <summary>
        /// 打开设备
        /// </summary>
        /// <returns></returns>
        private bool __OnOpenDevice()
        {
            bool bReturn    = false;    // 设置相机参数返回值
            bool bOpenState = false;    //打开设备接口的返回状态
            try
            {
                Console.Write("====================CGigeRecovery::__OnOpenDevice()====================\n");
                List<IGXDeviceInfo> listGXDeviceInfo = new List<IGXDeviceInfo>();
                m_objIGXFactory.UpdateAllDeviceList(1000, listGXDeviceInfo);

                // 判断当前连接设备个数
                if (listGXDeviceInfo.Count <= 0)
                {
                    Console.Write("<NO device>\n");
                    return false;
                }

                //获取第一台设备信息
                IGXDeviceInfo objIGXDeviceInfo = listGXDeviceInfo[0];

                //判断枚举到的设备是否为Gige
                if (objIGXDeviceInfo.GetDeviceClass() != GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_GEV)
                {
                    Console.Write("<The device is not GEV>");
                    return false;

                }
                m_strMACAddress = objIGXDeviceInfo.GetMAC();

                Console.Write("<Open device by MAC:{0}\n", m_strMACAddress);
                m_objIGXDevice = m_objIGXFactory.OpenDeviceByMAC (m_strMACAddress, GX_ACCESS_MODE.GX_ACCESS_EXCLUSIVE);
                m_objIGXFeatureControl = m_objIGXDevice.GetRemoteFeatureControl();


                //打开流
                if (null != m_objIGXDevice)
                {
                    m_objIGXStream = m_objIGXDevice.OpenStream(0);
                    m_objIGXStreamFeatureControl = m_objIGXStream.GetFeatureControl();
                }

                // 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
                // 以提高网络相机的采集性能,设置方法参考以下代码。
                GX_DEVICE_CLASS_LIST objDeviceClass = m_objIGXDevice.GetDeviceInfo().GetDeviceClass();
                if (GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_GEV == objDeviceClass)
                {
                    // 判断设备是否支持流通道数据包功能
                    if (true == m_objIGXFeatureControl.IsImplemented("GevSCPSPacketSize"))
                    {
                        // 获取当前网络环境的最优包长值
                        uint nPacketSize = m_objIGXStream.GetOptimalPacketSize();
                        // 将最优包长值设置为当前设备的流通道包长值
                        m_objIGXFeatureControl.GetIntFeature("GevSCPSPacketSize").SetValue(nPacketSize);
                    }
                }

                // 初始化参数
                Console.Write("<Initialize the device parameters>\n");
                bReturn = __InitParam();
                if (!bReturn)
                {
                    return false;
                }

                // 导出参数配置文件
                Console.Write("<Export config file>\n");
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.ExportConfigFile(m_strSavePath);
                }

                // 注册掉线回调函数
                Console.Write("<Register device Offline callback>\n");
                if (null != m_objIGXDevice)
                {
                    //RegisterDeviceOfflineCallback第一个参数属于用户自定参数(类型必须为引用
                    //类型)，若用户想用这个参数可以在委托函数中进行使用
                    m_hCB = m_objIGXDevice.RegisterDeviceOfflineCallback(null, __OnDeviceOfflineCallbackFun);
                }

                bOpenState = true;

                // 更新设备打开标识
                m_bIsOpen = true;


            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                bOpenState = false;
            }
            return bOpenState;
        }

        
        /// <summary>
        /// 初始化相机参数
        /// </summary>
        /// <returns>true：返回成功，false：返回失败</returns>
        private bool __InitParam()
        {
            bool bInitParamState = false;
            try
            {
                if (null != m_objIGXFeatureControl)
                {
                    //设置采集模式连续采集
                    m_objIGXFeatureControl.GetEnumFeature("AcquisitionMode").SetValue("Continuous");

                    //设置触发模式为关
                    m_objIGXFeatureControl.GetEnumFeature("TriggerMode").SetValue("Off");

                    //设置心跳超时时间为1s
                    //针对千兆网相机，程序在Debug模式下调试运行时，相机的心跳超时时间自动设置为5min，
                    //这样做是为了不让相机的心跳超时影响程序的调试和单步执行，同时这也意味着相机在这5min内无法断开，除非使相机断电再上电
                    //为了解决掉线重连问题，将相机的心跳超时时间设置为1s，方便程序掉线后可以重新连接
                    m_objIGXFeatureControl.GetIntFeature("GevHeartbeatTimeout").SetValue(1000);

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

                        bInitParamState = true;
                    }
                    else
                    {
                        bInitParamState = false;
                    }
                }
               
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                bInitParamState = false;
            }
            return bInitParamState;
        }

        /// <summary>
        /// 开始采集
        /// </summary>
        /// <returns>true：返回成功，false：返回失败</returns>
        private bool __OnStartSnap()
        {
            bool bStartSnapState = false;
            try
            {
                Console.WriteLine("====================CGigeRecovery::__OnStartSnap()====================");

                //发采集命令
                Console.WriteLine("<Send start snap command to device>");
                
                //开启采集流通道
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StartGrab();
                }

                //执行发送开采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStart").Execute();
                }

                m_bIsSanp = true;
                bStartSnapState = true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                bStartSnapState = false;
            }
            return bStartSnapState;
        }

        /// <summary>
        /// 停止采集
        /// </summary>
        /// <returns>true：返回成功，false：返回失败</returns>
        private bool __OnStopSnap()
        {
            bool bStopSnapState = false;
            try
            {
                Console.WriteLine("====================CGigeRecovery::__OnStopSnap()====================");

                //发送停止集命令
                Console.WriteLine("<Send stop snap command to device>");

                //发送停采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStop").Execute();
                }

                //关闭采集流通道
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StopGrab();
                }

                m_bIsSanp = false;
                bStopSnapState = true;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                bStopSnapState = false;
            }
            return bStopSnapState;
        }

        /// <summary>
        /// 关闭设备
        /// </summary>
        /// <returns></returns>
        private bool __OnCloseDevice()
        {
            try
            {
                Console.WriteLine("====================CGigeRecovery::__OnCloseDevice()====================");
               
                 //关闭流
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.Close();
                    m_objIGXStream = null;
                    m_objIGXStreamFeatureControl = null;
                }

                // 注销设备掉线事件回调
                Console.WriteLine("<Unregister device Offline callback>");
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.UnregisterDeviceOfflineCallback(m_hCB);
                }

                //关闭设备
                Console.WriteLine("<Close device>");
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.Close();
                    m_objIGXDevice = null;
                }

                m_bIsOpen = false;

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            } 
            return false;
        }

        /// <summary>
        /// 连续采集
        /// </summary>
        private void __Acquisition()
        {
            IImageData objIImageData = null; //GetImage采集对象
            Console.WriteLine("====================CGigeRecovery::__Acquisition()====================");
            Console.WriteLine("<Press any key to stop acquisition>");

            // 无按键按下
            while (!System.Console.KeyAvailable)
            {
                try
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
                        if (null != m_objIGXStream)
                        {
                            objIImageData = m_objIGXStream.GetImage(500);
                            Console.WriteLine("Successfully get Image");
                            //释放资源
                            if (null != objIImageData)
                            {
                                objIImageData.Destroy();
                                objIImageData = null; 
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                } 
            }

            Console.ReadKey();
        }

        /// <summary>
        /// 相机掉线处理
        /// </summary>
        private void __ProcessOffline()
        {

            Console.Write("**********************Process Offline**********************\r");
            try
            {
                // 如果开采，则停止采集
                if (m_bIsSanp)
                {
                    //发送停采命令
                    Console.Write("\n<Send stop snap command to device>\n");

                    //发送停采命令
                    if (null != m_objIGXFeatureControl)
                    {
                        m_objIGXFeatureControl.GetCommandFeature("AcquisitionStop").Execute();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                m_objIGXFeatureControl = null;
            }

            try
            {

                // 如果开采，则停止采集
                if (m_bIsSanp)
                {

                    //关闭采集流通道
                    if (null != m_objIGXStream)
                    {
                        m_objIGXStream.StopGrab();
                        m_objIGXStream.Close();
                        m_objIGXStream = null;
                        m_objIGXStreamFeatureControl = null;
                    }
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                m_bIsSanp = false;
            }

            try
            {
                // 如果打开，则关闭
                if (m_bIsOpen)
                {
                    // 注销设备掉线事件回调
                    Console.WriteLine("<Unregister device Offline callback>");
                    if (null != m_objIGXDevice)
                    {
                        m_objIGXDevice.UnregisterDeviceOfflineCallback(m_hCB);
                    }

                    //关闭设备
                    Console.WriteLine("<Close device>");
                    if (null != m_objIGXDevice)
                    {
                        m_objIGXDevice.Close();
                        m_objIGXDevice = null;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                m_bIsOpen = false;
            }
        }

        /// <summary>
        /// 设备重连
        /// </summary>
        private void __Recovery()
        {
            try
            {
                Console.Write("**********************Recovery**********************\r");
                List<IGXDeviceInfo> listGXDeviceInfo = new List<IGXDeviceInfo>();
                m_objIGXFactory.UpdateAllDeviceList(1000, listGXDeviceInfo);

                // 判断当前连接设备个数
                if (listGXDeviceInfo.Count <= 0)
                {
                    return ;
                }

                // 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.Close();
                    m_objIGXDevice = null;
                }

                Console.WriteLine("<Open device by MAC:{0}", m_strMACAddress);
                m_objIGXDevice = m_objIGXFactory.OpenDeviceByMAC(m_strMACAddress, GX_ACCESS_MODE.GX_ACCESS_EXCLUSIVE);
                m_objIGXFeatureControl = m_objIGXDevice.GetRemoteFeatureControl();


                //打开流
                if (null != m_objIGXDevice)
                {
                    m_objIGXStream = m_objIGXDevice.OpenStream(0);
                    m_objIGXStreamFeatureControl = m_objIGXStream.GetFeatureControl();
                }

                // 导入配置文件
                Console.WriteLine("<Import config file>");
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.ImportConfigFile(m_strSavePath);
                }

                // 注册掉线回调函数
                Console.WriteLine("<Register device Offline callback>");
                if (null != m_objIGXDevice)
                {
                    //RegisterDeviceOfflineCallback第一个参数属于用户自定参数(类型必须为引用
                    //类型)，若用户想用这个参数可以在委托函数中进行使用
                    m_hCB = m_objIGXDevice.RegisterDeviceOfflineCallback(null, __OnDeviceOfflineCallbackFun);
                }
     
                // 更新设备打开标识
                m_bIsOpen = true;

                // 发送开采命令
                Console.WriteLine("<Send start snap command to device>");

                //开启采集流通道
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StartGrab();
                }

                //执行发送开采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStart").Execute();
                }

                m_bIsSanp = true;
                m_bIsOffLine = false;

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
        }

        /// <summary>
        /// 掉线回调函数
        /// </summary>
        /// <param name="pUserParam">用户私有参数</param>
        private void __OnDeviceOfflineCallbackFun(object pUserParam)
        {
            try
            {
                m_bIsOffLine = true;
                Console.Write("**********************Device offline**********************\n");
            }
            catch (Exception)
            {

            }
        }
    }
}
