using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxImageProcess
{
    public partial class GxImageProcess : Form
    {
        bool                                    m_bIsOpen                   = false;                          ///< 相机打开标识 
        bool                                    m_bIsSnap                   = false;                          ///< 相机开始采集标识
        bool                                    m_bColorDevice              = false;                          ///< 标识是否为彩色相机
        bool                                    m_bAwbLampHouse             = false;                          ///< 标示是否支持光源选择
        bool                                    m_bWhiteAutoSelectedIndex   = true;                           ///<白平衡列表框转换标志
        IGXFactory                              m_objIGXFactory             = null;                           ///<Factory对像
        IGXDevice                               m_objIGXDevice              = null;                           ///<设备对像
        IGXStream                               m_objIGXStream              = null;                           ///<流对像
        IGXFeatureControl                       m_objIGXFeatureControl      = null;                           ///<远端设备属性控制器对像
        IGXFeatureControl                       m_objIGXStreamFeatureControl = null;                          ///<流层属性控制器对象
        IImageProcessConfig                     m_objCfg                    = null;                           ///<图像配置参数对象
        GxBitmap                                m_objGxBitmap               = null;                           ///<图像显示类对象
        string                                  m_strBalanceWhiteAutoValue  = "Off";                          ///<自动白平衡当前的值
        bool                                    m_bEnableColorCorrect       = false;                          ///<颜色校正使能标志位
        bool                                    m_bEnableGamma              = false;                          ///<Gamma使能标志位
        bool                                    m_bEnableSharpness          = false;                          ///<锐化使能标志位 
        bool                                    m_bEnableAutoWhite          = false;                          ///<自动白平衡使能标志位
        bool                                    m_bEnableDenoise            = false;                          ///<图像降噪使能标志位
        bool                                    m_bEnableSaturation         = false;                          ///<饱和度使能标志位
        bool                                    m_bEnumDevices              = false;                          ///<是否枚举到设备标志位
        List<IGXDeviceInfo>                     m_listGXDeviceInfo;                                           ///<存放枚举到的设备的容器
        
        public GxImageProcess()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 清空白平衡相关控件
        /// </summary>
        private void __CleanBalanceUI()
        {
            //清空自动白平衡
            m_cb_AutoWhite.Items.Clear();
            m_cb_AutoWhite.Text = "";

            //清空白平衡光源
            m_cb_AwbLight.Items.Clear();
            m_cb_AwbLight.Text = "";
        }

        /// <summary>
        /// 设备打开后初始化界面
        /// </summary>
        private void __InitUI()
        {

            __CleanBalanceUI();
            if (null != m_objCfg)
            {
                if (m_bColorDevice)
                {
                    // 打开的是彩色相机
                    __InitEnumComBoxUI(m_cb_AutoWhite, "BalanceWhiteAuto", m_objIGXFeatureControl, ref m_bEnableAutoWhite);                 //自动白平衡的初始化
                    __InitEnumComBoxUI(m_cb_AwbLight, "AWBLampHouse", m_objIGXFeatureControl, ref m_bAwbLampHouse);                         //白平衡通道选择

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

                    m_ScrollBar_Saturation.Value = m_objCfg.GetSaturationParam();
                    m_txt_Saturation.Text = m_objCfg.GetSaturationParam().ToString();
                    m_objCfg.EnableColorCorrection(false);
                    m_checkBox_OpenColorCorrect.Checked = (m_objCfg.IsColorCorrection() && !m_bEnableColorCorrect);
                    m_checkBox_OpenDenoise.Checked = m_objCfg.IsDenoise();

                    m_bEnableAutoWhite = false;
                    m_bEnableColorCorrect = false;
                    m_bEnableDenoise = false;
                    m_bEnableSaturation = false;

                }
                else
                {
                    // 打开的是黑白相机
                    m_bEnableAutoWhite = true;
                    m_bEnableColorCorrect = true;
                    m_bEnableDenoise = true;
                    m_bEnableSaturation = true;

                }


                // 初始化设滑动条的位置
                m_ScrollBar_Sharpen.Value = Convert.ToInt32(m_objCfg.GetSharpenParam() * 10);
                m_ScrollBar_Gamma.Value = Convert.ToInt32(m_objCfg.GetGammaParam() * 10);
                m_ScrollBar_Contrast.Value = m_objCfg.GetContrastParam();
                m_ScrollBar_Lightness.Value = m_objCfg.GetLightnessParam();

                // 初始化文本框值
                m_txt_Sharpen.Text = m_objCfg.GetSharpenParam().ToString();
                m_txt_Gamma.Text = m_objCfg.GetGammaParam().ToString();
                m_txt_Contrast.Text = m_objCfg.GetContrastParam().ToString();
                m_txt_Lightness.Text = m_objCfg.GetLightnessParam().ToString();

                // 初始化新check框
                m_CheckBox__BadPixelCorrect.Checked = m_objCfg.IsDefectivePixelCorrect();
                m_checkBox_OpenSharpen.Checked = m_objCfg.IsSharpen();
                m_checkBox_Accelerate.Checked = m_objCfg.IsAccelerate();
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
            string       strValue = "";                   //当前选择项
            List<string> list            = new List<string>();   //Combox将要填入的列表
            bool         bIsReadable     = false;                //是否可读
            bool         bIsWrite        = false;
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
                    strValue = objIGXFeatureControl.GetEnumFeature(strFeatureName).GetValue();
                }
                else if (strFeatureName.Equals("AWBLampHouse"))
                {
                    m_bAwbLampHouse = false;
                    return;
                }

                bIsWrite = m_objIGXFeatureControl.IsWritable(strFeatureName);
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
                if (strTemp == strValue)
                {
                    cbEnum.SelectedIndex = i;
                    break;
                }
            }
        }


        /// <summary>
        /// 更新界面
        /// </summary>
        private void __UpdateUI()
        {
            m_btn_OpenDevice.Enabled = m_bEnumDevices;
            m_btn_CloseDevice.Enabled = m_bIsOpen;
            m_btn_StartDevice.Enabled = m_bIsOpen && !m_bIsSnap;
            m_btn_StopDevice.Enabled = m_bIsSnap;

            m_cb_AutoWhite.Enabled = (m_bIsOpen && !m_bEnableAutoWhite);
            m_cb_AwbLight.Enabled = (m_bIsOpen && !m_bAwbLampHouse);
            m_cb_ChooseDevice.Enabled = m_bEnumDevices;

            m_ScrollBar_Sharpen.Enabled = (m_bIsOpen && !m_bEnableSharpness && m_checkBox_OpenSharpen.Checked);
            m_ScrollBar_Gamma.Enabled = (m_bIsOpen && !m_bEnableGamma);
            m_ScrollBar_Contrast.Enabled = m_bIsOpen;
            m_ScrollBar_Lightness.Enabled = m_bIsOpen;
            m_ScrollBar_Saturation.Enabled = (m_bIsOpen && !m_bEnableSaturation);

            // 初始化文本框值
            m_txt_Sharpen.Enabled = (m_bIsOpen && !m_bEnableSharpness && m_checkBox_OpenSharpen.Checked);
            m_txt_Gamma.Enabled = (m_bIsOpen && !m_bEnableGamma);
            m_txt_Contrast.Enabled = m_bIsOpen;
            m_txt_Lightness.Enabled = m_bIsOpen;
            m_txt_Saturation.Enabled = (m_bIsOpen && !m_bEnableSaturation);

            // 初始化新check框
            m_CheckBox__BadPixelCorrect.Enabled = m_bIsOpen;
            m_checkBox_OpenColorCorrect.Enabled = (m_bIsOpen && !m_bEnableColorCorrect);
            m_checkBox_OpenDenoise.Enabled = (m_bIsOpen && !m_bEnableDenoise);
            m_checkBox_OpenSharpen.Enabled = (m_bIsOpen && !m_bEnableSharpness);
            m_checkBox_Accelerate.Enabled = m_bIsOpen;

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

                //设置触发模式为关
                m_objIGXFeatureControl.GetEnumFeature("TriggerMode").SetValue("Off");

                // 判断相机是否彩色
                m_objGxBitmap.IsSupportColor(ref m_bColorDevice);

                // 判断相机是否支持白平衡光源选择
                m_bAwbLampHouse = m_objIGXFeatureControl.IsImplemented("AWBLampHouse");

                __IsEnableDeviceColorCorrect();
                __IsEnableDeviceGamma();
                __IsEnableDeviceSharpness();
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
                //如果未停采则先停止采集
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

            //关闭设备
            __CloseDevice();
            m_bIsOpen = false;
        }


        /// <summary>
        /// 加载窗体执行初始化UI和库
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxImageProcess_Load(object sender, EventArgs e)
        {
            try
            {
                //刷新界面
                __UpdateUI();
                m_objIGXFactory = IGXFactory.GetInstance();
                m_objIGXFactory.Init();
                __InitDeviceCombox();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 初始化选择设备下拉框
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void __InitDeviceCombox()
        {
            try
            {
                m_listGXDeviceInfo = new List<IGXDeviceInfo>();
                m_objIGXFactory.UpdateAllDeviceList(200, m_listGXDeviceInfo);

                // 判断当前连接设备个数
                if (m_listGXDeviceInfo.Count <= 0)
                {
                    MessageBox.Show("没有枚举到设备，请插入相机并重启程序！");
                    m_bEnumDevices = false;
                    __UpdateUI();
                    return;
                }
                else
                {
                    m_bEnumDevices = true;
                    __UpdateUI();

                    foreach(IGXDeviceInfo objDeviceInfo in m_listGXDeviceInfo)
                    {
                        m_cb_ChooseDevice.Items.Add(objDeviceInfo.GetDisplayName());
                    }

                    // 默认下拉列表选择第一台相机
                    m_cb_ChooseDevice.SelectedIndex = 0;
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 打开设备响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_OpenDevice_Click(object sender, EventArgs e)
        {
            try
            {
                //关闭流
                __CloseStream();
                // 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
                __CloseDevice();

                int nSel = m_cb_ChooseDevice.SelectedIndex;
                if (nSel < 0)
                {
                    MessageBox.Show("请插入相机！");
                    return;
                }

                //打开相机
                m_objIGXDevice = m_objIGXFactory.OpenDeviceBySN(m_listGXDeviceInfo[nSel].GetSN(), GX_ACCESS_MODE.GX_ACCESS_EXCLUSIVE);
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

                m_objCfg = m_objIGXDevice.CreateImageProcessConfig();

                if (null != m_objGxBitmap)
                {
                    m_objGxBitmap.ReleaseBuffer();
                }
                m_objGxBitmap = new GxBitmap(m_objIGXDevice, m_pic_ShowImage, m_objIGXStream, m_objIGXFactory);

                //相机初始化
                __InitDevice();

                // 初始化UI界面控件
                __InitUI();

                // 更新设备打开标识
                m_bIsOpen = true;
                m_bEnumDevices = false;

                //刷新界面
                __UpdateUI();

                //打开设备前其它UI的Enable都为false，这样会导致按Tap键获取不到焦点
                //，在打开设备后重新把焦点定到关闭设备按钮上
                m_btn_CloseDevice.Focus();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 开始采集响应函数
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
                    m_objIGXStream.RegisterCaptureCallback(this, __OnFrameCallbackFun);
                    m_objIGXStream.StartGrab();
                }

                //发送开采命令
                if (null != m_objIGXFeatureControl)
                {
                    m_objIGXFeatureControl.GetCommandFeature("AcquisitionStart").Execute();
                }
                m_bIsSnap = true;
                m_bEnumDevices = false;

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 停止采集响应函数
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
        /// 关闭设备响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_CloseDevice_Click(object sender, EventArgs e)
        {
            try
            {
                // 停止采集关闭设备、关闭流
                __CloseAll();
                
                m_lbl_IsAccelerate.Text = "";

                m_bEnumDevices = true;

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
        /// 窗体关闭之后
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxImageProcess_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                // 停止采集关闭设备、关闭流
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
        ///  用于图像处理后并显示图像
        /// </summary>
        /// <param name="objIBaseData">图像信息对象</param>
        public void ShowImageProcess(IBaseData objIBaseData)
        {
            GX_VALID_BIT_LIST emValidBits = GX_VALID_BIT_LIST.GX_BIT_0_7;
            try
            {
                if (null != m_objCfg)
                {
                    emValidBits = m_objGxBitmap.GetBestValudBit(objIBaseData.GetPixelFormat());
                    m_objCfg.SetValidBit(emValidBits);
                }
            }
            catch (Exception)
            {
              
            }

            if (null != m_objCfg)
            {
                m_objGxBitmap.ShowImageProcess(m_objCfg, objIBaseData);
            }
        }

        /// <summary>
        /// 采集事件的委托函数
        /// </summary>
        /// <param name="objUserParam">用户私有参数</param>
        /// <param name="objIFrameData">图像信息对象</param>
        private void __OnFrameCallbackFun(object objUserParam, IFrameData objIFrameData)
        {
            try
            {
                GxImageProcess objGxImageProcess = objUserParam as GxImageProcess;
                objGxImageProcess.ShowImageProcess(objIFrameData);
            }
            catch (Exception)
            {

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

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
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
        /// 切换"自动白平衡光源"选项响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_AwbLight_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                string strValue = m_cb_AwbLight.Text;
                __SetEnumValue("AWBLampHouse", strValue, m_objIGXFeatureControl);

                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        /// <summary>
        /// 是否使能坏点校正
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_CheckBox__BadPixelCorrect_CheckedChanged(object sender, EventArgs e)
        {
            try
            {  
                m_objCfg.EnableDefectivePixelCorrect(m_CheckBox__BadPixelCorrect.Checked);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 是否使能降噪
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_checkBox_OpenDenoise_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                //降噪只能是彩色相机支持
                if (m_bColorDevice)
                {
                    m_objCfg.EnableDenoise(m_checkBox_OpenDenoise.Checked);
                }   
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 是否使能颜色校正
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_checkBox_OpenColorCorrect_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                //颜色校正只能是彩色相机支持
                if (m_bColorDevice)
                {
                    m_objCfg.EnableColorCorrection(m_checkBox_OpenColorCorrect.Checked);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 是否开启锐化
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_checkBox_OpenSharpen_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                m_objCfg.EnableSharpen(m_checkBox_OpenSharpen.Checked);

                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }


        /// <summary>
        /// 设置并显示锐化值
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_ScrollBar_Sharpen_ValueChanged(object sender, EventArgs e)
        {

            try
            {
                double dSharpen = 0.0;

                dSharpen = (double)m_ScrollBar_Sharpen.Value;
                dSharpen = dSharpen / 10.0;

                m_objCfg.SetSharpenParam(dSharpen);
                m_txt_Sharpen.Text = m_objCfg.GetSharpenParam().ToString();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 设置并显示Gamma值
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_ScrollBar_Gamma_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                double dGamma = 0.0;
                dGamma = (double)m_ScrollBar_Gamma.Value;
                dGamma = dGamma / 10.0;

                m_objCfg.SetGammaParam(dGamma);
                m_txt_Gamma.Text = m_objCfg.GetGammaParam().ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 设置并显示对比度值
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_ScrollBar_Contrast_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                m_objCfg.SetContrastParam(m_ScrollBar_Contrast.Value);
                m_txt_Contrast.Text = m_objCfg.GetContrastParam().ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 设置并显示亮度值
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_ScrollBar_Lightness_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                m_objCfg.SetLightnessParam(m_ScrollBar_Lightness.Value);
                m_txt_Lightness.Text = m_objCfg.GetLightnessParam().ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 设置并显示饱和度
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_ScrollBar_Saturation_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                //饱和度只能是彩色相机支持
                if (m_bColorDevice)
                {
                    m_objCfg.SetSaturationParam(m_ScrollBar_Saturation.Value);
                    m_txt_Saturation.Text = m_objCfg.GetSaturationParam().ToString();
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

        /// <summary>
        /// 使能算法加速
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_checkBox_Accelerate_CheckedChanged(object sender, EventArgs e)
        {
            try
            {
                int nHeight = (int)m_objIGXFeatureControl.GetIntFeature("Height").GetValue();
                bool bFlag = ((nHeight % 4) != 0) && m_checkBox_Accelerate.Checked; //判断当前是否4的倍数以及是否使能
                if (bFlag)
                {
                    MessageBox.Show("使用CPU加速，高必须是4的整数倍！");
                    m_checkBox_Accelerate.Checked = false;
                    return;
                }
                m_objCfg.EnableAccelerate(m_checkBox_Accelerate.Checked);
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message); 
            }
           
        }

        /// <summary>
        /// 获取设备是否打开颜色校正
        /// </summary>
        private void __IsEnableDeviceColorCorrect()
        {
            m_bEnableColorCorrect = false;
            bool bIsImplement = m_objIGXFeatureControl.IsImplemented("ColorTransformationEnable");
            if (false == bIsImplement)
            {
                return;
            }

            bool bIsReadable = m_objIGXFeatureControl.IsReadable("ColorTransformationEnable");
            if (!bIsReadable)
            {
                return;
            }

            bool bIsWrite = m_objIGXFeatureControl.IsWritable("ColorTransformationEnable");
            if (!bIsWrite)
            {
                return;
            }

            IBoolFeature pBoolFeature = m_objIGXFeatureControl.GetBoolFeature("ColorTransformationEnable");
            m_bEnableColorCorrect = pBoolFeature.GetValue();
        }

        /// <summary>
        /// 获取设备是否打开Gamma使能
        /// </summary>
        private void __IsEnableDeviceGamma()
        {
            m_bEnableGamma = false;
            bool bIsImplement = m_objIGXFeatureControl.IsImplemented("GammaEnable");
            if(false == bIsImplement)
            {
                return;
            }

            IBoolFeature pBoolFeature = m_objIGXFeatureControl.GetBoolFeature("GammaEnable");

            m_bEnableGamma = pBoolFeature.GetValue();
        }

        /// <summary>
        /// 获取设备是否打开锐化使能
        /// </summary>
        private void __IsEnableDeviceSharpness()
        {
            m_bEnableSharpness = false;
            bool bIsImplement = m_objIGXFeatureControl.IsImplemented("SharpnessMode");
            if (false == bIsImplement)
            {
                return;
            }

            IEnumFeature pEnumFeature = m_objIGXFeatureControl.GetEnumFeature("SharpnessMode");
            string strValue = pEnumFeature.GetValue();

            if (true == strValue.Equals("Off"))
            {
                m_bEnableSharpness = false;
            }
            else if (true == strValue.Equals("On"))
            {
                m_bEnableSharpness = true;
            }
        }
    }
}
