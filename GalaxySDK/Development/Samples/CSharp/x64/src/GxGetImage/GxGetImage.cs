using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxGetImage
{
    public partial class GxGetImage : Form
    {
        bool                  m_bIsOpen                   = false;                  ///<设备打开状态
        bool                  m_bIsSnap                   = false;                  ///<发送开采命令标识
        IGXFactory            m_objIGXFactory             = null;                   ///<Factory对像
        IGXDevice             m_objIGXDevice              = null;                   ///<设备对像
        IGXStream             m_objIGXStream              = null;                   ///<流对像
        IGXFeatureControl     m_objIGXFeatureControl      = null;                   ///<远端设备属性控制器对像
        IGXFeatureControl     m_objIGXStreamFeatureControl = null;                  ///<流层属性控制器对象
        CStatistics           m_objStatistic              = new CStatistics();      ///<数据统计类对象用于处理统计时间
        CStopWatch            m_objStopTime               = new CStopWatch();       ///<定义时间差类对象
        GxBitmap              m_objGxBitmap               = null;                   ///<图像显示类对象

        public GxGetImage()
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
            m_txt_TimeOut.Enabled = m_bIsOpen && m_bIsSnap;
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
            }
        }

        /// <summary>
        /// 更新界面的统计数据
        /// </summary>
        /// <param name="dData">时间间隔</param>
        private void __UpdateStatisticalData(double dData)
        {
            m_objStatistic.AddStatisticalData(dData);

            // 获取平均值并显示
            m_txt_AveTime.Text = m_objStatistic.GetAverage().ToString("F3");

            // 获取最大值并显示
            m_txt_MaxTime.Text = m_objStatistic.GetMax().ToString("F3");

            // 获取最小值并显示
            m_txt_MinTime.Text = m_objStatistic.GetMin().ToString("F3");
        }

        /// <summary>
        /// 加载窗体执行初始化UI和库
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxGetImage_Load(object sender, EventArgs e)
        {
            try
            {
                //刷新界面
                __UpdateUI();

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
                        m_objIGXFeatureControl = null;
                    }
                }
            }
            catch (Exception)
            {
            }
            m_bIsSnap = false;
            try
            {
                //停止流通道和关闭流
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StopGrab();
                    m_objIGXStream.Close();
                    m_objIGXStream = null;
                    m_objIGXStreamFeatureControl = null;
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

                __InitDevice();

                if (null != m_objGxBitmap)
                {
                    m_objGxBitmap.ReleaseBuffer();
                }
                m_objGxBitmap = new GxBitmap(m_objIGXDevice, m_pic_ShowImage, m_objIGXStream, m_objIGXFactory);

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
        /// 关闭设备关闭流
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_CloseDevice_Click(object sender, EventArgs e)
        {
            try
            {
                m_objStatistic.Reset();
                // 停止采集关闭设备、关闭流
                __CloseAll();

                //刷新界面
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            if (null != m_objGxBitmap)
            {
                m_objGxBitmap.ReleaseBuffer();
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
                    m_objIGXStream.StartGrab();
                }

                //发送开采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStart").Execute();
                }
                m_bIsSnap = true;

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
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
        /// 发送软触发命令采单帧
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_SoftTriggerCommand_Click(object sender, EventArgs e)
        {
            try
            {
                IImageData objIImageData = null;
                double dElapsedtime = 0;
                uint nTimeout = 500;

                if (!string.IsNullOrEmpty(m_txt_TimeOut.Text))
                {
                    try
                    {
                        nTimeout = Convert.ToUInt32(m_txt_TimeOut.Text);
                    }
                    catch (Exception)
                    {
                        m_txt_TimeOut.Text = (500).ToString();
                        MessageBox.Show("请输入正确的有效数字！");
                        return;
                    }
                }

                m_txt_TimeOut.Text = nTimeout.ToString();

                //每次发送触发命令之前清空采集输出队列
                //防止库内部缓存帧，造成本次GXGetImage得到的图像是上次发送触发得到的图
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.FlushQueue();
                }

                //发送软触发命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("TriggerSoftware").Execute();
                }

                //获取图像
                if (null != m_objIGXStream)
                {
                    //计时开始
                    m_objStopTime.Start();

                    objIImageData = m_objIGXStream.GetImage(nTimeout);

                    //结束计时
                    dElapsedtime = m_objStopTime.Stop();
                }

                m_objGxBitmap.Show(objIImageData);

                if (null != objIImageData)
                {
                    //用完之后释放资源
                    objIImageData.Destroy();
                }
                
                //更新界面的时间统计数据
                __UpdateStatisticalData(dElapsedtime);
               
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 限定超时时间只能输入数字
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_TimeOut_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsNumber(e.KeyChar) && e.KeyChar != 8)
            {
                e.Handled = true;
            }
        }

        /// <summary>
        /// 窗体关闭、关闭流、关闭设备、反初始化
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxGetImage_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                //关闭流、关闭设备
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
    }
}
