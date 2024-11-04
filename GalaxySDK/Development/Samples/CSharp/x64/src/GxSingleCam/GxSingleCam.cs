using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using GxIAPINET;
using GxIAPINET.Sample.Common;


namespace GxSingleCam
{
    public partial class GxSingleCam : Form
    {
        bool                 m_bIsOpen                    = false;                           ///<设备打开状态
        bool                 m_bIsSnap                    = false;                           ///<发送开采命令标识
        bool                 m_bTriggerMode               = false;                           ///<是否支持触发模式
        bool                 m_bTriggerActive             = false;                           ///<是否支持触发极性
        bool                 m_bTriggerSource             = false;                           ///<是否支持触发源 
        bool                 m_bWhiteAuto                 = false;                           ///<标识是否支持白平衡
        bool                 m_bBalanceRatioSelector      = false;                           ///<标识是否支持白平衡通道
        bool                 m_bWhiteAutoSelectedIndex    = true;                            ///<白平衡列表框转换标志
        IGXFactory           m_objIGXFactory              = null;                            ///<Factory对像
        IGXDevice            m_objIGXDevice               = null;                            ///<设备对像
        IGXStream            m_objIGXStream               = null;                            ///<流对像
        IGXFeatureControl    m_objIGXFeatureControl       = null;                            ///<远端设备属性控制器对像
        IGXFeatureControl    m_objIGXStreamFeatureControl = null;                            ///<流层属性控制器对象
        string               m_strBalanceWhiteAutoValue   = "Off";                           ///<自动白平衡当前的值
        GxBitmap             m_objGxBitmap                = null;                            ///<图像显示类对象
        string               m_strFilePath                = "";                              ///<应用程序当前路径
       
        

        public GxSingleCam()
        {
            // 获取应用程序的当前执行路径
            m_strFilePath = Directory.GetCurrentDirectory().ToString();
            m_strFilePath = m_strFilePath + "\\GxSingleCamImages";

            //查看是否存在此文件
            if (!Directory.Exists(m_strFilePath))
            {
                Directory.CreateDirectory(m_strFilePath);
            }

            InitializeComponent();
        }

        /// <summary>
        /// 设备打开后初始化界面
        /// </summary>
        private void __InitUI()
        {
            __InitEnumComBoxUI(m_cb_TriggerMode, "TriggerMode", m_objIGXFeatureControl, ref m_bTriggerMode);                      //触发模式初始化
            __InitEnumComBoxUI(m_cb_TriggerSource, "TriggerSource", m_objIGXFeatureControl, ref m_bTriggerSource);                //触发源初始化
            __InitEnumComBoxUI(m_cb_TriggerActivation, "TriggerActivation", m_objIGXFeatureControl, ref m_bTriggerActive);        //触发极性初始化
            __InitShutterUI();                                                                                                    //曝光初始化
            __InitGainUI();                                                                                                       //增益的初始化
            __CleanBalanceUI();                                                                                                   //清除白平衡相关控件
            __InitWhiteRatioUI();                                                                                                 //初始化白平衡系数相关控件
            __InitEnumComBoxUI(m_cb_AutoWhite, "BalanceWhiteAuto", m_objIGXFeatureControl, ref m_bWhiteAuto);                     //自动白平衡的初始化
            __InitEnumComBoxUI(m_cb_RatioSelector, "BalanceRatioSelector", m_objIGXFeatureControl, ref m_bBalanceRatioSelector);  //白平衡通道选择


            //获取白平衡当前的值
            bool bIsImplemented = false;             //是否支持
            bool bIsReadable = false;                //是否可读
            // 获取是否支持
            if (null != m_objIGXFeatureControl)
            {
                bIsImplemented = m_objIGXFeatureControl.IsImplemented("BalanceWhiteAuto");
                bIsReadable = m_objIGXFeatureControl.IsReadable("BalanceWhiteAuto");
                if (bIsImplemented)
                {
                    if (bIsReadable)
                    {
                        //获取当前功能值
                        m_strBalanceWhiteAutoValue = m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetValue();
                    }
                }
            }
        }


        /// <summary>
        /// 对枚举型变量按照功能名称设置值
        /// </summary>
        /// <param name="strFeatureName">枚举功能名称</param>
        /// <param name="strValue">功能的值</param>
        /// <param name="objIGXFeatureControl">属性控制器对像</param>
        private void __SetEnumValue(string strFeatureName, string strValue, IGXFeatureControl objIGXFeatureControl)
        {
            if (null != objIGXFeatureControl)
            {
                //设置当前功能值
                objIGXFeatureControl.GetEnumFeature(strFeatureName).SetValue(strValue);
            }
        }

        /// <summary>
        /// 枚举型功能ComBox界面初始化
        /// </summary>
        /// <param name="cbEnum">ComboBox控件名称</param>
        /// <param name="strFeatureName">枚举型功能名称</param>
        /// <param name="objIGXFeatureControl">属性控制器对像</param>
        /// <param name="bIsImplemented">是否支持</param>
        private void __InitEnumComBoxUI(ComboBox cbEnum, string strFeatureName, IGXFeatureControl objIGXFeatureControl, ref bool bIsImplemented)
        {
            string strTriggerValue = "";                   //当前选择项
            List<string> list = new List<string>();   //Combox将要填入的列表
            bool bIsReadable = false;                //是否可读
            bool bIsWrite = false;
            // 获取是否支持
            if (null != objIGXFeatureControl)
            {

                bIsImplemented = objIGXFeatureControl.IsImplemented(strFeatureName);    
                // 如果不支持则直接返回
                if (!bIsImplemented)
                {
                    return;
                }

                bIsReadable = objIGXFeatureControl.IsReadable(strFeatureName);

                if (bIsReadable)
                {
                    list.AddRange(objIGXFeatureControl.GetEnumFeature(strFeatureName).GetEnumEntryList());
                    //获取当前功能值
                    strTriggerValue = objIGXFeatureControl.GetEnumFeature(strFeatureName).GetValue();
                }

                bIsWrite = objIGXFeatureControl.IsWritable(strFeatureName);
                // 如果不可写则直接返回
                if (!bIsWrite)
                {
                    bIsImplemented = false;
                    return;
                }
            }

            //清空组合框并更新数据到窗体
            cbEnum.Items.Clear();
            foreach (string str in list)
            {
                cbEnum.Items.Add(str);
            }

            //获得相机值和枚举到值进行比较，刷新对话框
            for (int i = 0; i < cbEnum.Items.Count; i++)
            {
                string strTemp = cbEnum.Items[i].ToString();
                if (strTemp == strTriggerValue)
                {
                    cbEnum.SelectedIndex = i;
                    break;
                }
            }
        }

        /// <summary>
        /// 清空白平衡相关控件
        /// </summary>
        private void __CleanBalanceUI()
        {
            //清空自动白平衡

            m_cb_AutoWhite.Items.Clear();
            m_cb_AutoWhite.Text = "";

            //清空自动白平衡通道选择
            m_cb_RatioSelector.Items.Clear();
            m_cb_RatioSelector.Text = "";

            //当前的白平衡文本
            m_lbl_WhiteRatio.Text = "";

            //当前的白平衡系数的编辑框
            m_txt_BalanceRatio.Text = "";
        }

        /// <summary>
        /// 初始化白平衡系数相关控件
        /// </summary>
        private void __InitWhiteRatioUI()
        {
            double dWhiteRatio = 0.0;                       //当前曝光值
            double dMin = 0.0;                       //最小值
            double dMax = 0.0;                       //最大值
            string strUnit = "";                        //单位
            string strText = "";                        //显示内容
            bool bIsBalanceRatio = false;                   //是否白平衡是否支持
            //获取当前相机的白平衡系数、最小值、最大值和单位
            if (null != m_objIGXFeatureControl)
            {
                bIsBalanceRatio = m_objIGXFeatureControl.IsImplemented("BalanceRatio");
                bool bBalanceRatioReadable = m_objIGXFeatureControl.IsReadable("BalanceRatio");
                if (!bIsBalanceRatio || !bBalanceRatioReadable)
                {
                    m_txt_BalanceRatio.Enabled = false; ;
                    return;
                }

                dWhiteRatio = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetValue();
                dMin = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetMin();
                dMax = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetMax();
                strUnit = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetUnit();
            }

            //刷新获取白平衡系数范围及单位到界面上
            strText = string.Format("白平衡系数({0}~{1}){2}", dMin.ToString("0.00"), dMax.ToString("0.00"), strUnit);
            m_lbl_WhiteRatio.Text = strText;

            //当前的白平衡系数的编辑框
            m_txt_BalanceRatio.Text = dWhiteRatio.ToString("0.00");
        }


        /// <summary>
        /// 曝光控制界面初始化
        /// </summary>
        private void __InitShutterUI()
        {
            double dCurShuter = 0.0;                       //当前曝光值
            double dMin = 0.0;                       //最小值
            double dMax = 0.0;                       //最大值
            string strUnit = "";                        //单位
            string strText = "";                        //显示内容

            //获取当前相机的曝光值、最小值、最大值和单位
            if (null != m_objIGXFeatureControl)
            {
                dCurShuter = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetValue();
                dMin = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMin();
                dMax = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMax();
                strUnit = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetUnit();
            }

            //刷新曝光范围及单位到界面上
            strText = string.Format("曝光时间({0}~{1}){2}", dMin.ToString("0.00"), dMax.ToString("0.00"), strUnit);
            m_lbl_Shutter.Text = strText;

            //当前的曝光值刷新到曝光的编辑框
            m_txt_Shutter.Text = dCurShuter.ToString("0.00");
        }

        /// <summary>
        /// 增益控制界面初始化
        /// </summary>
        private void __InitGainUI()
        {
            double dCurGain = 0;             //当前增益值
            double dMin = 0.0;           //最小值
            double dMax = 0.0;           //最大值
            string strUnit = "";            //单位
            string strText = "";            //显示内容

            //获取当前相机的增益值、最小值、最大值和单位
            if (null != m_objIGXFeatureControl)
            {
                dCurGain = m_objIGXFeatureControl.GetFloatFeature("Gain").GetValue();
                dMin = m_objIGXFeatureControl.GetFloatFeature("Gain").GetMin();
                dMax = m_objIGXFeatureControl.GetFloatFeature("Gain").GetMax();
                strUnit = m_objIGXFeatureControl.GetFloatFeature("Gain").GetUnit();
            }

            //更新增益值范围到界面
            strText = string.Format("增益({0}~{1}){2}", dMin.ToString("0.00"), dMax.ToString("0.00"), strUnit);
            m_lbl_Gain.Text = strText;

            //当前的增益值刷新到增益的编辑框
            string strCurGain = dCurGain.ToString("0.00");
            m_txt_Gain.Text = strCurGain;
        }

        /// <summary>
        /// 更新界面
        /// </summary>
        void __UpdateUI()
        {
            //相机控制相关使能操作
            m_btn_OpenDevice.Enabled = !m_bIsOpen;
            m_btn_CloseDevice.Enabled = m_bIsOpen;
            m_btn_StartDevice.Enabled = m_bIsOpen && !m_bIsSnap;
            m_btn_StopDevice.Enabled = m_bIsSnap;

            //相机参数相关的使能操作
            m_cb_TriggerMode.Enabled = m_bIsOpen && m_bTriggerMode;
            m_cb_TriggerSource.Enabled = m_bIsOpen && m_bTriggerSource;
            m_cb_TriggerActivation.Enabled = m_bIsOpen && m_bTriggerActive;
            m_cb_RatioSelector.Enabled = m_bIsOpen && m_bBalanceRatioSelector;


            m_txt_Shutter.Enabled = m_bIsOpen;
            m_txt_Gain.Enabled = m_bIsOpen;
            m_txt_BalanceRatio.Enabled = m_bIsOpen
                                         && m_bBalanceRatioSelector
                                         && (m_strBalanceWhiteAutoValue == "Off");


            m_cb_AutoWhite.Enabled = m_bIsOpen && m_bWhiteAuto;
            m_btn_SoftTriggerCommand.Enabled = m_bIsOpen;
        }

        /// <summary>
        /// 相机初始化
        /// </summary>
        void __InitDevice()
        {
            if (null != m_objIGXFeatureControl)
            {
                //设置采集模式连续采集
                m_objIGXFeatureControl.GetEnumFeature("AcquisitionMode").SetValue("Continuous");
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
        /// 加载窗体执行初始化UI和库
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxSingleCam_Load(object sender, EventArgs e)
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

                // 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
                if (null != m_objIGXDevice)
                {
                    m_objIGXDevice.Close();
                    m_objIGXDevice = null;
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

                //初始化相机参数
                __InitDevice();

                // 获取相机参数,初始化界面控件
                __InitUI();

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

                m_bIsOpen = false;

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
                    //RegisterCaptureCallback第一个参数属于用户自定参数(类型必须为引用
                    //类型)，若用户想用这个参数可以在委托函数中进行使用
                    m_objIGXStream.RegisterCaptureCallback(this, __CaptureCallbackPro);
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
                    //注销采集回调函数
                    m_objIGXStream.UnregisterCaptureCallback();
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
        /// 图像的显示和存储
        /// </summary>
        /// <param name="objIFrameData">图像信息对象</param>
        void ImageShowAndSave(IFrameData objIFrameData)
        {
            try
            {
                m_objGxBitmap.Show(objIFrameData);
            }
            catch (Exception)
            {
            }

            // 是否需要进行图像保存
            if (m_bSaveBmpImg.Checked)
            {
                DateTime dtNow = System.DateTime.Now;  // 获取系统当前时间
                string strDateTime = dtNow.Year.ToString() + "_"
                                   + dtNow.Month.ToString() + "_"
                                   + dtNow.Day.ToString() + "_"
                                   + dtNow.Hour.ToString() + "_"
                                   + dtNow.Minute.ToString() + "_"
                                   + dtNow.Second.ToString() + "_"
                                   + dtNow.Millisecond.ToString();

                string stfFileName = m_strFilePath + "\\" + strDateTime + ".bmp";  // 默认的图像保存名称
                m_objGxBitmap.SaveBmp(objIFrameData, stfFileName);
            }
        }

        /// <summary>
        /// 回调函数,用于获取图像信息和显示图像
        /// </summary>
        /// <param name="obj">用户自定义传入参数</param>
        /// <param name="objIFrameData">图像信息对象</param>
        private void __CaptureCallbackPro(object objUserParam, IFrameData objIFrameData)
        {
            try
            {
                GxSingleCam objGxSingleCam = objUserParam as GxSingleCam;
                objGxSingleCam.ImageShowAndSave(objIFrameData);
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 切换"触发模式"combox框响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_TriggerMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string strValue = m_cb_TriggerMode.Text;
                __SetEnumValue("TriggerMode", strValue, m_objIGXFeatureControl);

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 切换"触发源"Combox消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_TriggerSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string strValue = m_cb_TriggerSource.Text;
                __SetEnumValue("TriggerSource", strValue, m_objIGXFeatureControl);

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 切换"触发极性"Combox消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_TriggerActivation_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string strValue = m_cb_TriggerActivation.Text;
                __SetEnumValue("TriggerActivation", strValue, m_objIGXFeatureControl);

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
                //发送软触发命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("TriggerSoftware").Execute();
                }

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }


        /// <summary>
        /*-----------------------------------------------------------------------
         * 当切换自动白平衡模式为Once时,设备内部在设置完Once模式后会自动更新为off,
         * 为了与设备状态保持一致,程序以代码模拟该过程：判断当前设置模式为Once后,
         * 将界面随即更新为off(由UpdateWhiteAutoUI()函数实现),但此过程会导致函数
         * m_cb_AutoWhite_SelectedIndexChanged()执行两次,第二次执行时自动白平衡
         * 选项已经更新为off,若重新执行可能会打断Once的设置过程,引起白平衡不起作用,
         * 参数m_bWhiteAutoSelectedIndex即是为了解决函数重入问题而引入的变量
         ------------------------------------------------------------------------*/
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_AutoWhite_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                if (!m_bWhiteAutoSelectedIndex)
                {
                    return;
                }
                string strValue = m_cb_AutoWhite.Text;
                __SetEnumValue("BalanceWhiteAuto", strValue, m_objIGXFeatureControl);
                m_strBalanceWhiteAutoValue = strValue;
                // 获取白平衡系数更新界面
                __InitWhiteRatioUI();
                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 切换"白平衡通道"选项响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_RatioSelector_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string strValue = m_cb_RatioSelector.Text;
                __SetEnumValue("BalanceRatioSelector", strValue, m_objIGXFeatureControl);
                // 获取白平衡系数更新界面
                __InitWhiteRatioUI();
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
        private void GxSingleCam_FormClosed(object sender, FormClosedEventArgs e)
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

            try
            {
                //停止流通道、注销采集回调和关闭流
                if (null != m_objIGXStream)
                {
                    m_objIGXStream.StopGrab();
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
           
            try
            {
                //反初始化
                if (null != m_objIGXFactory)
                {
                    m_objIGXFactory.Uninit();
                }
            }
            catch (Exception)
            {

            }
        }

        /// <summary>
        /// 检查是否输入的是小数或整数，小数要求小数点最多8位
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">按键响应事件</param>
        private void __CheckKeyPress(object sender, KeyPressEventArgs e)
        {
            if (!(((e.KeyChar >= '0') && (e.KeyChar <= '9')) || e.KeyChar <= 31))
            {
                if (e.KeyChar == '.')
                {
                    if (((TextBox)sender).Text.Trim().IndexOf('.') > -1)
                        e.Handled = true;
                }
                else
                    e.Handled = true;
            }
            else
            {
                if (e.KeyChar <= 31)
                {
                    e.Handled = false;
                }
                else if (((TextBox)sender).Text.Trim().IndexOf('.') > -1)
                {
                    if (((TextBox)sender).Text.Trim().Substring(((TextBox)sender).Text.Trim().IndexOf('.') + 1).Length >= 8)
                        e.Handled = true;
                }
            }
        }

        /// <summary>
        /// 判断曝光结束事件是否输入的是小数或整数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_Shutter_KeyPress(object sender, KeyPressEventArgs e)
        {
            __CheckKeyPress(sender, e);
        }

        /// <summary>
        /// 判断增益是否输入的是小数或整数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_Gain_KeyPress(object sender, KeyPressEventArgs e)
        {
            __CheckKeyPress(sender, e);
        }

        /// <summary>
        /// 判断白平衡系数是否输入的是小数或整数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_BalanceRatio_KeyPress(object sender, KeyPressEventArgs e)
        {
            __CheckKeyPress(sender, e);
        }

        /// <summary>
        /// 判断是否有回车按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxSingleCam_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
            {
                this.m_pic_ShowImage.Focus();
            }
        }

        /// <summary>
        /// 控制曝光时间的Edit框失去焦点的响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_Shutter_Leave(object sender, EventArgs e)
        {
            double dShutterValue = 0.0;              //曝光值
            double dMin = 0.0;                       //最小值
            double dMax = 0.0;                       //最大值

            try
            {

                if (!m_bIsOpen)
                {
                    return;
                }

                try
                {
                    dShutterValue = Convert.ToDouble(m_txt_Shutter.Text);
                }
                catch (Exception)
                {
                    __InitShutterUI();
                    MessageBox.Show("请输入正确的曝光时间");
                    return;
                }

                //获取当前相机的曝光值、最小值、最大值和单位
                if (null != m_objIGXFeatureControl)
                {
                    dMin = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMin();
                    dMax = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMax();
                    //判断输入值是否在曝光时间的范围内
                    //若大于最大值则将曝光值设为最大值
                    if (dShutterValue > dMax)
                    {
                        dShutterValue = dMax;
                    }
                    //若小于最小值将曝光值设为最小值
                    if (dShutterValue < dMin)
                    {
                        dShutterValue = dMin;
                    }

                    m_txt_Shutter.Text = dShutterValue.ToString("F2");
                    m_objIGXFeatureControl.GetFloatFeature("ExposureTime").SetValue(dShutterValue);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 控制增益值的Edit框失去焦点的响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_Gain_Leave(object sender, EventArgs e)
        {
            double dGain = 0;            //增益值
            double dMin = 0.0;           //最小值
            double dMax = 0.0;           //最大值
            try
            {
                if (!m_bIsOpen)
                {
                    return;
                }

                try
                {
                    dGain = Convert.ToDouble(m_txt_Gain.Text);
                }
                catch (Exception)
                {
                    __InitGainUI();
                    MessageBox.Show("请输入正确的增益值");
                    return;
                }

                //当前相机的增益值、最小值、最大值
                if (null != m_objIGXFeatureControl)
                {
                    dMin = m_objIGXFeatureControl.GetFloatFeature("Gain").GetMin();
                    dMax = m_objIGXFeatureControl.GetFloatFeature("Gain").GetMax();

                    //判断输入值是否在增益值的范围内
                    //若输入的值大于最大值则将增益值设置成最大值
                    if (dGain > dMax)
                    {
                        dGain = dMax;
                    }

                    //若输入的值小于最小值则将增益的值设置成最小值
                    if (dGain < dMin)
                    {
                        dGain = dMin;
                    }

                    m_txt_Gain.Text = dGain.ToString("F2");
                    m_objIGXFeatureControl.GetFloatFeature("Gain").SetValue(dGain);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// "白平衡系数"Edit框失去焦点响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_BalanceRatio_Leave(object sender, EventArgs e)
        {
            double dBalanceRatio = 0;    //白平衡系数值
            double dMin = 0.0;           //最小值
            double dMax = 0.0;           //最大值
            try
            {
                if (!m_bIsOpen)
                {
                    return;
                }

                try
                {
                    dBalanceRatio = Convert.ToDouble(m_txt_BalanceRatio.Text);
                }
                catch (Exception)
                {
                    __InitWhiteRatioUI();
                    MessageBox.Show("请输入正确的白平衡系数");
                    return;
                }

                //当前相机的白平衡系数值、最小值、最大值
                if (null != m_objIGXFeatureControl)
                {
                    dMin = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetMin();
                    dMax = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetMax();

                    //判断输入值是否在白平衡系数的范围内
                    //若大于最大值则将白平衡系数设为最大值
                    if (dBalanceRatio > dMax)
                    {
                        dBalanceRatio = dMax;
                    }
                    //若小于最小值将白平衡系数设为最小值
                    if (dBalanceRatio < dMin)
                    {
                        dBalanceRatio = dMin;
                    }

                    m_txt_BalanceRatio.Text = dBalanceRatio.ToString("F2");
                    m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").SetValue(dBalanceRatio);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 更新自动白平衡从Once到off
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_timer_UpdateAutoWhite_Tick(object sender, EventArgs e)
        {
            try
            {
                string strValue = ""; //自动白平衡值
                Int32 i = 0;          //循环变量

                // 如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
                // 为与设备状态保持一致程序以代码模拟该过程：设置为Once后随即将界面更新为off
                if (m_strBalanceWhiteAutoValue == "Once")
                {
                    try
                    {
                        //获取自动白平衡枚举值
                        if (null != m_objIGXFeatureControl)
                        {
                            strValue = m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetValue();
                        }
                    }
                    catch (Exception)
                    {
                    }
                    m_strBalanceWhiteAutoValue = strValue;

                    if (m_strBalanceWhiteAutoValue == "Off")
                    {
                        for (i = 0; i < m_cb_AutoWhite.Items.Count; i++)
                        {
                            if (m_cb_AutoWhite.Items[i].ToString() == "Off")
                            {
                                m_cb_AutoWhite.SelectedIndex = i;
                                break;
                            }
                        }
                        __UpdateUI();
                    }
                }
            }
            catch (Exception)
            {
            }
        }

    }
}
