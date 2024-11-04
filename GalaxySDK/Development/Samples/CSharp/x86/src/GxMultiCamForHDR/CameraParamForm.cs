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

namespace GxMultiCamForHDR
{
    public partial class CameraParamForm : Form
    {
        /// 视频采集模式
        public struct GRAB_MODE
        {
            public string strImgSize;       ///< 显示格式：1080P/720P/480P
            public long lImgWidth;          ///< 图像宽 
            public long lImgHeight;         ///< 图像高
            public long lImgOffsetX;        ///< X偏移
            public long lImgOffsetY;        ///< Y偏移
        }

        const string        IMG_SIZE_OFF                        = "Off";                    ///< 显示格式：默认
        const string        IMG_SIZE_1080P                      = "1080P";                  ///< 显示格式：1080P
        const string        IMG_SIZE_720P                       = "720P";                   ///< 显示格式：720P
        const string        IMG_SIZE_480P                       = "480P";                   ///< 显示格式：480P

        const long          IMG_WIDTH_1920                      = 1920;                     ///< 1080P图像宽
        const long          IMG_WIDTH_1280                      = 1280;                     ///< 720P图像宽
        const long          IMG_WIDTH_640                       = 640;                      ///< 480P图像宽

        const long          IMG_HEIGHT_1080                     = 1080;                     ///< 1080P图像高
        const long          IMG_HEIGHT_1024                     = 1024;                     ///< 720P图像高
        const long          IMG_HEIGHT_480                      = 480;                      ///< 480P图像高

        const long          IMG_OFFSET_X_0                      = 0;                        ///< 1080P水平偏移
        const long          IMG_OFFSET_X_320                    = 320;                      ///< 720P水平偏移
        const long          IMG_OFFSET_X_640                    = 640;                      ///< 480P水平偏移

        const long          IMG_OFFSET_Y_0                      = 0;                        ///< 1080P垂直偏移
        const long          IMG_OFFSET_Y_28                     = 28;                       ///< 720P垂直偏移
        const long          IMG_OFFSET_Y_300                    = 300;                      ///< 480P垂直偏移

        const int           FLOAT_PRECISION                     = 100;                      ///< 浮点精度，该值不能超过10^4，否则可能会因超过
                                                                                            ///< MFC滑动条范围类型而设置范围失败
                                              
        bool                m_bIsGrabModeEnable                 = false;                    ///< 采集模式使能状态
        bool                m_bIsPixelFormatEnable              = false;                    ///< 像素格式使能状态
        bool                m_bIsReverseXEnable                 = false;                    ///< 水平翻转使能状态
        bool                m_bIsReverseYEnable                 = false;                    ///< 垂直翻转使能状态
        bool                m_bIsExposureTimeEnable             = false;                    ///< 曝光时间使能状态
        bool                m_bIsExposureAutoEnable             = false;                    ///< 自动曝光使能状态
        bool                m_bIsExpectedGrayValueEnable        = false;                    ///< 期望灰度值使能状态
        bool                m_bIsGainAutoEnable                 = false;                    ///< 自动增益使能状态
        bool                m_bIsGammaEnableEnable              = false;                    ///< Gamma使能使能状态
        bool                m_bIsGammaModeEnable                = false;                    ///< Gamma模式使能状态
        bool                m_bIsGammaEnable                    = false;                    ///< Gamma值使能状态
        bool                m_bIsAWBModeEnable                  = false;                    ///< 自动白平衡模式使能状态
        bool                m_bIsAWBSelectorEnable              = false;                    ///< 白平衡通道选择使能状态
        bool                m_bIsBalanceRatioEnable             = false;                    ///< 白平衡系数使能状态
        bool                m_bIsSaturationModeEnable           = false;                    ///< 饱和度模式使能状态
        bool                m_bIsSaturationEnable               = false;                    ///< 饱和度值使能状态
        bool                m_bIsSharpnessModeEnable            = false;                    ///< 锐化模式使能状态
        bool                m_bIsSharpnessEnable                = false;                    ///< 锐化值使能状态
        bool                m_bIs2DNoiseReductionModeEnable     = false;                    ///< 2D降噪模式使能状态
        bool                m_bIs3DNoiseReductionModeEnable     = false;                    ///< 3D降噪模式使能状态
        bool                m_bIsHDRModeEnable                  = false;                    ///< HDR模式使能状态
        bool                m_bIsHDRLongEnable                  = false;                    ///< HDR亮场期望值使能状态
        bool                m_bIsHDRShortEnable                 = false;                    ///< HDR暗场期望值使能状态
        bool                m_bIsHDRMainEnable                  = false;                    ///< HDR融合期望值使能状态
        bool                m_bIsBalanceRatioReadable           = false;                    ///< 白平衡系数只读状态
        bool                m_bIsExposureTimeReadable           = false;                    ///< 曝光时间只读状态
        IGXDevice           m_objIGXDevice                      = null;                     ///< 设备对像
        IGXFeatureControl   m_objIGXFeatureControl              = null;                     ///< 远端设备属性控制器对像
        int                 m_nOperateID                        = 0;                        ///< 选择的相机号
        string              m_strAWBValue                       = "";                       ///< 自动白平衡下拉框值
        string              m_strExposureAutoValue              = "";                       ///< 自动曝光下拉框值
        string              m_strGainAutoValue                  = "";                       ///< 自动白平衡下拉框值
        GxMultiCamForHDR    m_frmGxMultiCamForHDR               = null;                     ///< 父窗口
        List<GRAB_MODE>     m_lstGrabMode                       = new List<GRAB_MODE>();    ///< 视频采集模式

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="objIGXDevice">设备对像</param>
        /// <param name="nOperateID">选择的相机号</param>
        public CameraParamForm(IGXDevice objIGXDevice, int nOperateID)
        {
            InitializeComponent();

            m_objIGXDevice = objIGXDevice;
            m_nOperateID = nOperateID + 1;
        }

        /// <summary>
        /// 加载窗体执行初始化UI
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void CameraParamForm_Load(object sender, EventArgs e)
        {
            m_frmGxMultiCamForHDR  = (GxMultiCamForHDR)this.Owner;
            m_objIGXFeatureControl = m_objIGXDevice.GetRemoteFeatureControl();

            // 初始化UI
            __InitUI();

            // 设置对话框标题
            string strSN = m_objIGXDevice.GetDeviceInfo().GetSN();

            string strTmp = string.Format("序号:{0} SN:{1}", m_nOperateID.ToString(), strSN);
            this.Text = strTmp;
        }

        /// <summary>
        /// 初始化UI
        /// </summary>
        private void __InitUI()
        {
            try
            {
                // 初始化图像格式控制相关控件
                __InitImageFormatUI();

                // 初始化模拟控制相关控件
                __InitAnalogUI();

                // 初始化颜色转换控制相关控件
                __InitColorTransfUI();

                // 初始化采集控制相关控件
                __InitAcqUI();

                // 初始化图像质量控制相关控件
                __InitImgQualityUI();

                // 初始化HDR控制相关控件
                __InitHDRUI();

                // 刷新界面控件使能状态
                __UpDateUIEnable();

                m_timer_UpdateAWB.Start();
                m_timer_UpdateExposureAuto.Start();
                m_timer_UpdateGainAuto.Start();                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 初始化图像格式控制相关控件
        /// </summary>
        private void __InitImageFormatUI()
        {
            // 初始化采集视频模式下拉框
            __InitComboGrabMode(m_cb_GrabMode, ref m_bIsGrabModeEnable);

            // 初始化像素格式下拉框
            __UpDateEnumComboUI("PixelFormat", m_cb_PixelFormat, ref m_bIsPixelFormatEnable);

            // 初始化水平翻转下拉框
            __UpDateBoolComboUI("ReverseX", m_cb_ReverseX, ref m_bIsReverseXEnable);

            // 初始化垂直翻转下拉框
            __UpDateBoolComboUI("ReverseY", m_cb_ReverseY, ref m_bIsReverseYEnable);

        }

        /// <summary>
        /// 初始化模拟控制相关控件
        /// </summary>
        private void __InitAnalogUI()
        {
            bool bIsReadable = false;

            // 初始化自动增益下拉框
            __UpDateEnumComboUI("GainAuto", m_cb_GainAuto, ref m_bIsGainAutoEnable);
            m_strGainAutoValue = m_cb_GainAuto.Text;

            // 初始化Gamma使能下拉框
            __UpDateBoolComboUI("GammaEnable", m_cb_GammaEnable, ref m_bIsGammaEnableEnable);

            // 初始化Gamma模式下拉框
            __UpDateEnumComboUI("GammaMode", m_cb_GammaMode, ref m_bIsGammaModeEnable);

            // 初始化Gamma值
            __UpDateFloatEditUI("Gamma", m_lbl_GammaRange, m_txt_Gamma, m_scrollBar_Gamma, ref bIsReadable, ref m_bIsGammaEnable);

        }

        /// <summary>
        /// 初始化颜色转换控制相关控件
        /// </summary>
        private void __InitColorTransfUI()
        {
            // 初始化自动白平衡模式下拉框
            __UpDateEnumComboUI("BalanceWhiteAuto", m_cb_AWBMode, ref m_bIsAWBModeEnable);

            // 初始化自动白平衡通道选择下拉框
            __UpDateEnumComboUI("BalanceRatioSelector", m_cb_WBSelector, ref m_bIsAWBSelectorEnable);
            m_strAWBValue = m_cb_AWBMode.Text;

            // 初始化白平衡系数
            __UpDateFloatEditUI("BalanceRatio", m_lbl_BalanceRatioRange, m_txt_BalanceRatio, m_scrollBar_BalanceRatio, ref m_bIsBalanceRatioReadable, ref m_bIsBalanceRatioEnable);

            // 初始化饱和度模式下拉框
            __UpDateEnumComboUI("SaturationMode", m_cb_SaturationMode, ref m_bIsSaturationModeEnable);

            // 初始化饱和度值
            __UpDateIntEditUI("Saturation", m_lbl_SaturationRange, m_txt_Saturation, m_scrollBar_Saturation, ref m_bIsSaturationEnable);

        }

        /// <summary>
        /// 初始化采集控制相关控件
        /// </summary>
        private void __InitAcqUI()
        {
            // 初始化曝光时间
            __UpDateFloatEditUI("ExposureTime", m_lbl_ExposureTimeRange, m_txt_ExposureTime, m_scrollBar_ExposureTime, ref m_bIsExposureTimeReadable, ref m_bIsExposureTimeEnable);

            // 初始化自动曝光
            __UpDateEnumComboUI("ExposureAuto", m_cb_ExposureAuto, ref m_bIsExposureAutoEnable);
            m_strExposureAutoValue = m_cb_ExposureAuto.Text;

            // 初始化期望灰度值
            __UpDateIntEditUI("ExpectedGrayValue", m_lbl_ExpectedGrayValueRange, m_txt_ExpectedGrayValue, m_scrollBar_ExpectedGrayValue, ref m_bIsExpectedGrayValueEnable);

        }

        /// <summary>
        /// 初始化图像质量控制相关控件
        /// </summary>
        private void __InitImgQualityUI()
        {
            bool bIsReadable = false;

            // 初始化锐化模式下拉框
            __UpDateEnumComboUI("SharpnessMode", m_cb_SharpnessMode, ref m_bIsSharpnessModeEnable);

            // 初始化锐度值
            __UpDateFloatEditUI("Sharpness", m_lbl_SharpnessRange, m_txt_Sharpness, m_scrollBar_Sharpness, ref bIsReadable, ref m_bIsSharpnessEnable);

            // 初始化2D降噪模式
            __UpDateEnumComboUI("NoiseReductionMode2D", m_cb_2DNoiseReductionMode, ref m_bIs2DNoiseReductionModeEnable);

            // 初始化3D降噪模式
            __UpDateEnumComboUI("NoiseReductionMode3D", m_cb_3DNoiseReductionMode, ref m_bIs3DNoiseReductionModeEnable);

        }

        /// <summary>
        /// 初始化HDR控制相关控件
        /// </summary>
        private void __InitHDRUI()
        {
            // 初始化HDR模式下拉框
            __UpDateEnumComboUI("HDRMode", m_cb_HDRMode, ref m_bIsHDRModeEnable);

            // 初始化HDR亮场期望值
            __UpDateIntEditUI("HDRTargetLongValue", m_lbl_HDRLongValue, m_txt_HDRLongValue, m_scrollBar_HDRLongValue, ref m_bIsHDRLongEnable);

            // 初始化HDR暗场期望值
            __UpDateIntEditUI("HDRTargetShortValue", m_lbl_HDRShortValue, m_txt_HDRShortValue, m_scrollBar_HDRShortValue, ref m_bIsHDRShortEnable);

            // 初始化DHR融合期望值
            __UpDateIntEditUI("HDRTargetMainValue", m_lbl_HDRMainValue, m_txt_HDRMainValue, m_scrollBar_HDRMainValue, ref m_bIsHDRMainEnable);


        }

        /// <summary>
        /// 获取节点是否可读，是否使能
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="bIsReadable">是否可读</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __GetControlStatus(string strFeatureName, ref bool bIsReadable, ref bool bIsEnable)
        {
            bool bIsImplement   = false;
            bool bIsWritable    = false;

            // 是否支持该节点
            bIsImplement = m_objIGXFeatureControl.IsImplemented(strFeatureName);
            if (!bIsImplement)
            {
                bIsEnable = false;
                return;
            }
            else
            {
                // 支持该节点
                // 判断该节点是否可读
                bIsReadable = m_objIGXFeatureControl.IsReadable(strFeatureName);
                if (!bIsReadable)
                {
                    bIsEnable = false;
                    return;
                }

                // 判断该节点是否可写
                bIsWritable = m_objIGXFeatureControl.IsWritable(strFeatureName);

                // 支持且可写
                if (bIsWritable)
                {
                    bIsEnable = true;
                }
                else
                {
                    bIsEnable = false;
                }
            }
        }


        /// <summary>
        /// 枚举型功能ComBox界面初始化
        /// </summary>
        /// <param name="strFeatureName">枚举型功能名称</param>
        /// <param name="cbEnum">ComboBox控件名称</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __UpDateEnumComboUI(string strFeatureName, ComboBox cbEnum, ref bool bIsEnable)
        {
            string          strCurEnum      = "";                   // 当前选择项
            bool            bIsReadable     = false;                // 是否可读
            List<string>    lstEnumEntry    = new List<string>();   // Combox将要填入的列表

            // 判断控件是否使能
            __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
            if (!bIsReadable)
            {
                bIsEnable = false;
                // 重置该控件
                cbEnum.Items.Clear();
                return;
            }

            // 获取功能的枚举项
            lstEnumEntry.AddRange(m_objIGXFeatureControl.GetEnumFeature(strFeatureName).GetEnumEntryList());
            strCurEnum = m_objIGXFeatureControl.GetEnumFeature(strFeatureName).GetValue();

            // 初始化当前控件的可选项
            cbEnum.Items.Clear();
            foreach (string str in lstEnumEntry)
            {
                cbEnum.Items.Add(str);
            }

            //获得相机值和枚举到值进行比较，刷新对话框
            for (int i = 0; i < cbEnum.Items.Count; ++i)
            {
                string strTemp = cbEnum.Items[i].ToString();
                if (0 == string.Compare(strTemp, strCurEnum))
                {
                    cbEnum.SelectedIndex = i;
                    break;
                }
            }
        }

        /// <summary>
        /// Bool型功能ComBox界面初始化
        /// </summary>
        /// <param name="strFeatureName">枚举型功能名称</param>
        /// <param name="cbEnum">ComboBox控件名称</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __UpDateBoolComboUI(string strFeatureName, ComboBox cbEnum, ref bool bIsEnable)
        {
            bool bIsReadable    = false;    // 是否可读
            bool bValue         = false;    // 当前项

            // 判断控件是否使能
            __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
            if (!bIsReadable)
            {
                bIsEnable = false;
                // 重置该控件
                cbEnum.Items.Clear();
                return;
            }

            //清空组合框并更新数据到窗体
            cbEnum.Items.Clear();
            cbEnum.Items.Add("true");
            cbEnum.Items.Add("false");

            // 获取当前值并显示到界面
            bValue = m_objIGXFeatureControl.GetBoolFeature(strFeatureName).GetValue();
            if (bValue)
            {
                cbEnum.SelectedIndex = 0;
            }
            else
            {
                cbEnum.SelectedIndex = 1;
            }
        }

        /// <summary>
        /// 更新Float编辑框UI
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="lblRange">取值范围</param>
        /// <param name="txtValue">当前值</param>
        /// <param name="scrollBar">滑动条控件</param>
        /// <param name="bIsReadable">是否可读</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __UpDateFloatEditUI(string strFeatureName, Label lblRange, TextBox txtValue, HScrollBar scrollBar, ref bool bIsReadable, ref bool bIsEnable)
        {
            string strRange     = "";
            string strUnit      = "";
            double dCurValue    = 0.0;
            double dMaxValue    = 0.0;
            double dMinValue    = 0.0;

            // 判断控件是否使能
            __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
            if (!bIsReadable)
            {
                bIsEnable = false;
                // 重置该控件
                lblRange.Text = "";
                txtValue.Text = "";
                scrollBar.Value = scrollBar.Minimum;
                return;
            }

            dMaxValue = m_objIGXFeatureControl.GetFloatFeature(strFeatureName).GetMax();
            dMinValue = m_objIGXFeatureControl.GetFloatFeature(strFeatureName).GetMin();
            strUnit = m_objIGXFeatureControl.GetFloatFeature(strFeatureName).GetUnit();

            // 初始化Label范围
            strRange = string.Format("({0}~{1}){2}", dMinValue.ToString("f4"), dMaxValue.ToString("f4"), strUnit);
            lblRange.Text = strRange;

            // 获取当前值并将当前值更新到界面
            dCurValue = m_objIGXFeatureControl.GetFloatFeature(strFeatureName).GetValue();
            txtValue.Text = dCurValue.ToString("f4");

            // 初始化滑动条
            scrollBar.Maximum = (int)(dMaxValue * FLOAT_PRECISION) - 1 + scrollBar.LargeChange;
            scrollBar.Minimum = (int)(dMinValue * FLOAT_PRECISION);
            scrollBar.Value   = (int)(dCurValue * FLOAT_PRECISION);
        }

        /// <summary>
        /// 更新Int编辑框UI
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="lblRange">取值范围</param>
        /// <param name="txtValue">当前值</param>
        /// <param name="scrollBar">滑动条控件</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __UpDateIntEditUI(string strFeatureName, Label lblRange, TextBox txtValue, HScrollBar scrollBar, ref bool bIsEnable)
        {
            bool    bIsReadable = false;
            string  strRange    = "";
            int     nCurValue   = 0;
            int     nMaxValue   = 0;
            int     nMinValue   = 0;

            // 判断控件是否使能
            __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
            if (!bIsReadable)
            {
                bIsEnable = false;
                // 重置该控件
                lblRange.Text = "";
                txtValue.Text = "";
                scrollBar.Value = scrollBar.Minimum;
                return;
            }

            nMaxValue = (int)m_objIGXFeatureControl.GetIntFeature(strFeatureName).GetMax();
            nMinValue = (int)m_objIGXFeatureControl.GetIntFeature(strFeatureName).GetMin();

            // 初始化Label范围
            strRange = string.Format("({0}~{1})", nMinValue.ToString(), nMaxValue.ToString());
            lblRange.Text = strRange;

            // 获取当前值并将当前值更新到界面
            nCurValue = (int)m_objIGXFeatureControl.GetIntFeature(strFeatureName).GetValue();
            txtValue.Text = nCurValue.ToString();

            // 初始化滑动条
            scrollBar.Maximum = nMaxValue - 1 + scrollBar.LargeChange;
            scrollBar.Minimum = nMinValue;
            scrollBar.Value   = nCurValue;
        }

        /// <summary>
        /// 初始化图像采集模式
        /// </summary>
        private void __InitGrabMode()
        {
            long lWidthMax  = 0;
            long lHeightMax = 0;

            List<string> lstImgSize  = new List<string>();
            List<long> lstImgWidth   = new List<long>();
            List<long> lstImgHeight  = new List<long>();
            List<long> lstImgOffsetX = new List<long>();
            List<long> lstImgOffsetY = new List<long>();

            GRAB_MODE stGrabMode;

            lWidthMax = m_objIGXFeatureControl.GetIntFeature("WidthMax").GetValue();
            lHeightMax = m_objIGXFeatureControl.GetIntFeature("HeightMax").GetValue();

            lstImgSize.Clear();
            lstImgWidth.Clear();
            lstImgHeight.Clear();
            lstImgOffsetX.Clear();
            lstImgOffsetY.Clear();

            lstImgSize.Add(IMG_SIZE_OFF);
            lstImgSize.Add(IMG_SIZE_1080P);
            lstImgSize.Add(IMG_SIZE_720P);
            lstImgSize.Add(IMG_SIZE_480P);

            lstImgWidth.Add(lWidthMax);
            lstImgWidth.Add(IMG_WIDTH_1920);
            lstImgWidth.Add(IMG_WIDTH_1280);
            lstImgWidth.Add(IMG_WIDTH_640);

            lstImgHeight.Add(lHeightMax);
            lstImgHeight.Add(IMG_HEIGHT_1080);
            lstImgHeight.Add(IMG_HEIGHT_1024);
            lstImgHeight.Add(IMG_HEIGHT_480);

            lstImgOffsetX.Add(0);
            lstImgOffsetX.Add(IMG_OFFSET_X_0);
            lstImgOffsetX.Add(IMG_OFFSET_X_320);
            lstImgOffsetX.Add(IMG_OFFSET_X_640);

            lstImgOffsetY.Add(0);
            lstImgOffsetY.Add(IMG_OFFSET_Y_0);
            lstImgOffsetY.Add(IMG_OFFSET_Y_28);
            lstImgOffsetY.Add(IMG_OFFSET_Y_300);

            for (int i = 0; i < 4; ++i)
            {
                stGrabMode.strImgSize  = lstImgSize[i];
                stGrabMode.lImgWidth   = lstImgWidth[i];
                stGrabMode.lImgHeight  = lstImgHeight[i];
                stGrabMode.lImgOffsetX = lstImgOffsetX[i];
                stGrabMode.lImgOffsetY = lstImgOffsetY[i];

                m_lstGrabMode.Add(stGrabMode);
            }
        }

        /// <summary>
        /// 初始化采集视频模式下拉框
        /// </summary>
        /// <param name="cbEnum">下拉框控件</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __InitComboGrabMode(ComboBox cbEnum, ref bool bIsEnable)
        {
            string strCurEnum = "";     // 当前选择项

            __InitGrabMode();

            // 判断设备是否支持更改采集视频模式
            __IsSupportImgSize(ref bIsEnable);

            // 判断控件是否使能
            if (!bIsEnable)
            {
                cbEnum.Items.Clear();
                return;
            }

            // 获取功能的枚举项
            __GetGrabModeValue(ref strCurEnum);

            cbEnum.Items.Clear();

            for (int i = 0; i < m_lstGrabMode.Count; ++i)
            {
                cbEnum.Items.Add(m_lstGrabMode[i].strImgSize);
            }

            //获得相机值和枚举到值进行比较，刷新对话框
            for (int i = 0; i < m_lstGrabMode.Count; ++i)
            {
                if (0 == string.Compare(m_lstGrabMode[i].strImgSize, strCurEnum))
                {
                    cbEnum.SelectedIndex = i;
                    break;
                }
            }
        }

        /// <summary>
        /// 获取当前采集模式
        /// </summary>
        /// <param name="strCurValue">视频采集模式的显示格式</param>
        private void __GetGrabModeValue(ref string strCurValue)
        {
            long lImgWidth = 0;
            long lImgHeight = 0;
            long lImgOffsetX = 0;
            long lImgOffsetY = 0;

            lImgWidth = m_objIGXFeatureControl.GetIntFeature("Width").GetValue();
            lImgHeight = m_objIGXFeatureControl.GetIntFeature("Height").GetValue();
            lImgOffsetX = m_objIGXFeatureControl.GetIntFeature("OffsetX").GetValue();
            lImgOffsetY = m_objIGXFeatureControl.GetIntFeature("OffsetY").GetValue();

            for (int i = 0; i < m_lstGrabMode.Count; ++i)
            {
                if((lImgWidth == m_lstGrabMode[i].lImgWidth)
                 &&(lImgHeight == m_lstGrabMode[i].lImgHeight)
                 &&(lImgOffsetX == m_lstGrabMode[i].lImgOffsetX)
                 &&(lImgOffsetY == m_lstGrabMode[i].lImgOffsetY))
                {
                    // 获取当前GrabMode
                    strCurValue = m_lstGrabMode[i].strImgSize;
                    return;
                }
            }

            // 若均不匹配则为"Off" 
            strCurValue = m_lstGrabMode[0].strImgSize;
        }

        /// <summary>
        /// 判断设备支持该分辨率
        /// </summary>
        /// <param name="bIsEnable">是否使能</param>
        private void __IsSupportImgSize(ref bool bIsEnable)
        {
            // 若设备最大宽高大于1080P则支持，否则不支持
            if ((m_lstGrabMode[0].lImgWidth >= m_lstGrabMode[1].lImgWidth) && (m_lstGrabMode[0].lImgHeight >= m_lstGrabMode[1].lImgHeight))
            {
                bIsEnable = true;
            }
            else
            {
                bIsEnable = false;
            }
        }

        /// <summary>
        /// 编辑框失去焦点的响应函数
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_txt_Leave(object sender, EventArgs e)
        {
            TextBox txtBox = (TextBox)sender;

            switch (txtBox.Name)
            {
                case "m_txt_Saturation":
                    __SetIntEdit("Saturation", m_txt_Saturation, m_scrollBar_Saturation);
                    break;
                case "m_txt_ExpectedGrayValue":
                    __SetIntEdit("ExpectedGrayValue", m_txt_ExpectedGrayValue, m_scrollBar_ExpectedGrayValue);
                    break;
                case "m_txt_HDRLongValue":
                    __SetIntEdit("HDRTargetLongValue", m_txt_HDRLongValue, m_scrollBar_HDRLongValue);
                    break;
                case "m_txt_HDRShortValue":
                    __SetIntEdit("HDRTargetShortValue", m_txt_HDRShortValue, m_scrollBar_HDRShortValue);
                    break;
                case "m_txt_HDRMainValue":
                    __SetIntEdit("HDRTargetMainValue", m_txt_HDRMainValue, m_scrollBar_HDRMainValue);
                    break;
                case "m_txt_Gamma":
                    __SetFloatEdit("Gamma", m_txt_Gamma, m_scrollBar_Gamma);
                    break;
                case "m_txt_BalanceRatio":
                    __SetFloatEdit("BalanceRatio", m_txt_BalanceRatio, m_scrollBar_BalanceRatio);
                    break;
                case "m_txt_ExposureTime":
                    __SetFloatEdit("ExposureTime", m_txt_ExposureTime, m_scrollBar_ExposureTime);
                    break;
                case "m_txt_Sharpness":
                    __SetFloatEdit("Sharpness", m_txt_Sharpness, m_scrollBar_Sharpness);
                    break;
                default:
                    break;

            }

        }

        /// <summary>
        /// 设置FloatEdit
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="txtValue">当前值编辑框</param>
        /// <param name="scrollBar">滑动条控件</param>
        private void __SetFloatEdit(string strFeatureName, TextBox txtValue, HScrollBar scrollBar)
        {
            double  dCurValue     = 0.0;
            bool    bIsReadable   = false;
            bool    bIsEnable     = false;

            try
            {
                // 获取当前项对应的值
                dCurValue = Convert.ToDouble(txtValue.Text);
                scrollBar.Value = (int)(dCurValue * FLOAT_PRECISION);

                // 判断控件是否使能
                __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
                if(bIsEnable)
                {
                    m_objIGXFeatureControl.GetFloatFeature(strFeatureName).SetValue(dCurValue);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 设置IntEdit
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="txtValue">当前值编辑框</param>
        /// <param name="scrollBar">滑动条控件</param>
        private void __SetIntEdit(string strFeatureName, TextBox txtValue, HScrollBar scrollBar)
        {
            int  nCurValue      = 0;
            bool bIsReadable    = false;
            bool bIsEnable      = false;

            try
            {
                // 获取当前项对应的值
                nCurValue = Convert.ToInt32(txtValue.Text);
                scrollBar.Value = nCurValue;

                // 判断控件是否使能
                __GetControlStatus(strFeatureName, ref bIsReadable, ref bIsEnable);
                if (bIsEnable)
                {
                    m_objIGXFeatureControl.GetIntFeature(strFeatureName).SetValue(nCurValue);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 检查是否输入的是小数或整数，小数要求小数点最多8位
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">按键响应事件</param>
        private void m_txt_KeyPress(object sender, KeyPressEventArgs e)
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
        /// 更新UI值
        /// </summary>
        /// <param name="strCbName">下拉框ID</param>
        private void __UpDateUI(string strCbName)
        {
            bool bIsReadable = false;

            try
            {
                switch (strCbName)
                {
                    case "m_cb_PixelFormat":
                        __UpDateEnumComboUI("SaturationMode", m_cb_SaturationMode, ref m_bIsSaturationModeEnable);
                        __UpDateIntEditUI("Saturation", m_lbl_SaturationRange, m_txt_Saturation, m_scrollBar_Saturation, ref m_bIsSaturationEnable);
                        __UpDateEnumComboUI("BalanceWhiteAuto", m_cb_AWBMode, ref m_bIsAWBModeEnable);
                        __UpDateEnumComboUI("BalanceRatioSelector", m_cb_WBSelector, ref m_bIsAWBSelectorEnable);
                        __UpDateFloatEditUI("BalanceRatio", m_lbl_BalanceRatioRange, m_txt_BalanceRatio, m_scrollBar_BalanceRatio, ref m_bIsBalanceRatioReadable, ref m_bIsBalanceRatioEnable);
                        __UpDateIntEditUI("ExpectedGrayValue", m_lbl_ExpectedGrayValueRange, m_txt_ExpectedGrayValue, m_scrollBar_ExpectedGrayValue, ref m_bIsExpectedGrayValueEnable);
                        break;
                    case "m_cb_ExposureAuto":
                        __UpDateFloatEditUI("ExposureTime", m_lbl_ExposureTimeRange, m_txt_ExposureTime, m_scrollBar_ExposureTime, ref m_bIsExposureTimeReadable, ref m_bIsExposureTimeEnable);
                        break;
                    case "m_cb_GammaMode":
                        __UpDateFloatEditUI("Gamma", m_lbl_GammaRange, m_txt_Gamma, m_scrollBar_Gamma, ref bIsReadable, ref m_bIsGammaEnable);
                        break;
                    case "m_cb_AWBMode":
                        __UpDateFloatEditUI("BalanceRatio", m_lbl_BalanceRatioRange, m_txt_BalanceRatio, m_scrollBar_BalanceRatio, ref m_bIsBalanceRatioReadable, ref m_bIsBalanceRatioEnable);
                        break;
                    case "m_cb_WBSelector":
                        __UpDateFloatEditUI("BalanceRatio", m_lbl_BalanceRatioRange, m_txt_BalanceRatio, m_scrollBar_BalanceRatio, ref m_bIsBalanceRatioReadable, ref m_bIsBalanceRatioEnable);
                        break;
                    case "m_cb_HDRMode":
                        __UpDateEnumComboUI("SaturationMode", m_cb_SaturationMode, ref m_bIsSaturationModeEnable);
                        __UpDateIntEditUI("Saturation", m_lbl_SaturationRange, m_txt_Saturation, m_scrollBar_Saturation, ref m_bIsSaturationEnable);
                        __UpDateEnumComboUI("GammaMode", m_cb_GammaMode, ref m_bIsGammaModeEnable);
                        __UpDateFloatEditUI("Gamma", m_lbl_GammaRange, m_txt_Gamma, m_scrollBar_Gamma, ref bIsReadable, ref m_bIsGammaEnable);
                        __UpDateEnumComboUI("GainAuto", m_cb_GainAuto, ref m_bIsGainAutoEnable);
                        __UpDateIntEditUI("ExpectedGrayValue", m_lbl_ExpectedGrayValueRange, m_txt_ExpectedGrayValue, m_scrollBar_ExpectedGrayValue, ref m_bIsExpectedGrayValueEnable);
                        __UpDateEnumComboUI("ExposureAuto", m_cb_ExposureAuto, ref m_bIsExposureAutoEnable);
                        __UpDateFloatEditUI("ExposureTime", m_lbl_ExposureTimeRange, m_txt_ExposureTime, m_scrollBar_ExposureTime, ref m_bIsExposureTimeReadable, ref m_bIsExposureTimeEnable);
                        break;
                    default:
                        break;
                }

                __UpDateUIEnable();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 刷新界面控件使能状态
        /// </summary>
        private void __UpDateUIEnable()
        {
            bool bIsOpen = m_frmGxMultiCamForHDR.IsOpen();
            bool bIsSnap = m_frmGxMultiCamForHDR.IsSnap();

            m_cb_GrabMode.Enabled = (bIsOpen && (!bIsSnap) && m_bIsGrabModeEnable);
            m_cb_PixelFormat.Enabled = (bIsOpen && (!bIsSnap) && m_bIsPixelFormatEnable);
            m_cb_ReverseX.Enabled = (bIsOpen && m_bIsReverseXEnable);
            m_cb_ReverseY.Enabled = (bIsOpen && m_bIsReverseYEnable);

            m_cb_GainAuto.Enabled = (bIsOpen && m_bIsGainAutoEnable);
            m_cb_GammaEnable.Enabled = (bIsOpen && m_bIsGammaEnableEnable);
            m_cb_GammaMode.Enabled = (bIsOpen && m_bIsGammaModeEnable);
            m_txt_Gamma.Enabled = (bIsOpen && m_bIsGammaEnable);
            m_scrollBar_Gamma.Enabled = (bIsOpen && m_bIsGammaEnable);

            m_cb_AWBMode.Enabled = (bIsOpen && m_bIsAWBModeEnable);
            m_cb_WBSelector.Enabled = (bIsOpen && m_bIsAWBSelectorEnable);
            m_txt_BalanceRatio.Enabled = (bIsOpen && m_bIsBalanceRatioEnable);
            m_scrollBar_BalanceRatio.Enabled = (bIsOpen && m_bIsBalanceRatioEnable);
            m_cb_SaturationMode.Enabled = (bIsOpen && m_bIsSaturationModeEnable);
            m_txt_Saturation.Enabled = (bIsOpen && m_bIsSaturationEnable);
            m_scrollBar_Saturation.Enabled = (bIsOpen && m_bIsSaturationEnable);

            m_txt_ExposureTime.Enabled = (bIsOpen && m_bIsExposureTimeEnable);
            m_scrollBar_ExposureTime.Enabled = (bIsOpen && m_bIsExposureTimeEnable);
            m_cb_ExposureAuto.Enabled = (bIsOpen && m_bIsExposureAutoEnable);
            m_txt_ExpectedGrayValue.Enabled = (bIsOpen && m_bIsExpectedGrayValueEnable);
            m_scrollBar_ExpectedGrayValue.Enabled = (bIsOpen && m_bIsExpectedGrayValueEnable);

            m_cb_SharpnessMode.Enabled = (bIsOpen && m_bIsSharpnessModeEnable);
            m_txt_Sharpness.Enabled = (bIsOpen && m_bIsSharpnessEnable);
            m_scrollBar_Sharpness.Enabled = (bIsOpen && m_bIsSharpnessEnable);
            m_cb_2DNoiseReductionMode.Enabled = (bIsOpen && m_bIs2DNoiseReductionModeEnable);
            m_cb_3DNoiseReductionMode.Enabled = (bIsOpen && m_bIs3DNoiseReductionModeEnable);

            m_cb_HDRMode.Enabled = (bIsOpen && m_bIsHDRModeEnable);
            m_txt_HDRLongValue.Enabled = (bIsOpen && m_bIsHDRLongEnable);
            m_scrollBar_HDRLongValue.Enabled = (bIsOpen && m_bIsHDRLongEnable);
            m_txt_HDRShortValue.Enabled = (bIsOpen && m_bIsHDRShortEnable);
            m_scrollBar_HDRShortValue.Enabled = (bIsOpen && m_bIsHDRShortEnable);
            m_txt_HDRMainValue.Enabled = (bIsOpen && m_bIsHDRMainEnable);
            m_scrollBar_HDRMainValue.Enabled = (bIsOpen && m_bIsHDRMainEnable);

            m_txt_ExposureTime.ReadOnly = (bIsOpen && m_bIsExposureTimeReadable && !m_bIsExposureTimeEnable);
            m_txt_BalanceRatio.ReadOnly = (bIsOpen && m_bIsBalanceRatioReadable && !m_bIsBalanceRatioEnable);

        }

        /// <summary>
        /// 设置图像采集模式
        /// </summary>
        private void __SetGrabMode()
        {
            // 先将图像大小设为最大
            m_objIGXFeatureControl.GetIntFeature("OffsetX").SetValue(m_lstGrabMode[0].lImgOffsetX);
            m_objIGXFeatureControl.GetIntFeature("OffsetY").SetValue(m_lstGrabMode[0].lImgOffsetY);
            m_objIGXFeatureControl.GetIntFeature("Width").SetValue(m_lstGrabMode[0].lImgWidth);
            m_objIGXFeatureControl.GetIntFeature("Height").SetValue(m_lstGrabMode[0].lImgHeight);
            
            for (int i = 0; i < m_lstGrabMode.Count; ++i)
            {
                if(0 == string.Compare(m_cb_GrabMode.Text, m_lstGrabMode[i].strImgSize))
                {
                    m_objIGXFeatureControl.GetIntFeature("Width").SetValue(m_lstGrabMode[i].lImgWidth);
                    m_objIGXFeatureControl.GetIntFeature("Height").SetValue(m_lstGrabMode[i].lImgHeight);
                    m_objIGXFeatureControl.GetIntFeature("OffsetX").SetValue(m_lstGrabMode[i].lImgOffsetX);
                    m_objIGXFeatureControl.GetIntFeature("OffsetY").SetValue(m_lstGrabMode[i].lImgOffsetY);
                    break;
                }
            }
        }

        /// <summary>
        /// 设置枚举项到设备中
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="cbEnum">下拉框控件</param>
        private void __SetEnum(string strFeatureName, ComboBox cbEnum)
        {
            try
            {
                m_objIGXFeatureControl.GetEnumFeature(strFeatureName).SetValue(cbEnum.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 
        }

        /// <summary>
        /// 将选定的下拉框值设置到设备中
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="cbEnum">下拉框控件</param>
        private void __SetBool(string strFeatureName, ComboBox cbEnum)
        {
            bool bCurValue = false;

            try
            {
                if (0 == string.Compare(cbEnum.Text, "true"))
                {
                    bCurValue = true;
                }
                else
                {
                    bCurValue = false;
                }

                m_objIGXFeatureControl.GetBoolFeature(strFeatureName).SetValue(bCurValue);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        /// <summary>
        /// 切换combox命令相应函数
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_cb_SelectionChangeCommitted(object sender, EventArgs e)
        {
            ComboBox comboBox = (ComboBox)sender;

            switch (comboBox.Name)
            {
                case "m_cb_GrabMode":
                    __SetGrabMode();
                    m_frmGxMultiCamForHDR.UpDateImgInfo();
                    break;
                case "m_cb_PixelFormat":
                    __SetEnum("PixelFormat", m_cb_PixelFormat);
                    break;
                case "m_cb_ReverseX":
                    __SetBool("ReverseX", m_cb_ReverseX);
                    break;
                case "m_cb_ReverseY":
                    __SetBool("ReverseY", m_cb_ReverseY);
                    break;
                case "m_cb_GainAuto":
                    __SetEnum("GainAuto", m_cb_GainAuto);
                    m_strGainAutoValue = m_cb_GainAuto.Text;
                    break;
                case "m_cb_GammaEnable":
                    __SetBool("GammaEnable", m_cb_GammaEnable);
                    break;
                case "m_cb_GammaMode":
                    __SetEnum("GammaMode", m_cb_GammaMode);
                    break;
                case "m_cb_AWBMode":
                    __SetEnum("BalanceWhiteAuto", m_cb_AWBMode);
                    m_strAWBValue = m_cb_AWBMode.Text;
                    break;
                case "m_cb_WBSelector":
                    __SetEnum("BalanceRatioSelector", m_cb_WBSelector);
                    break;
                case "m_cb_SaturationMode":
                    __SetEnum("SaturationMode", m_cb_SaturationMode);
                    break;
                case "m_cb_ExposureAuto":
                    __SetEnum("ExposureAuto", m_cb_ExposureAuto);
                    m_strExposureAutoValue = m_cb_ExposureAuto.Text;
                    break;
                case "m_cb_SharpnessMode":
                    __SetEnum("SharpnessMode", m_cb_SharpnessMode);
                    break;
                case "m_cb_2DNoiseReductionMode":
                    __SetEnum("NoiseReductionMode2D", m_cb_2DNoiseReductionMode);
                    break;
                case "m_cb_3DNoiseReductionMode":
                    __SetEnum("NoiseReductionMode3D", m_cb_3DNoiseReductionMode);
                    break;
                case "m_cb_HDRMode":
                    __SetEnum("HDRMode", m_cb_HDRMode);
                    break;
                default:
                    break;
            }
            __UpDateUI(comboBox.Name);
        }

        /// <summary>
        /// 键盘按下消息响应函数
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void CameraParamForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
            {
                this.label1.Focus();
            }
        }

        /// <summary>
        /// 设置Float滑动条
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="nCurValue">当前值</param>
        /// <param name="txtValue">当前值编辑框</param>
        private void __SetFloatScroll(string strFeatureName, int nCurValue, TextBox txtValue)
        {
            string strCurValue  = "";
            double dCurValue    = 0.0;

            dCurValue = (double)nCurValue / FLOAT_PRECISION;
            strCurValue = string.Format("{0}", dCurValue.ToString("f4"));
            txtValue.Text = strCurValue;

            m_objIGXFeatureControl.GetFloatFeature(strFeatureName).SetValue((double)nCurValue / FLOAT_PRECISION);
        }

        /// <summary>
        /// 设置Int滑动条
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="nCurValue">当前值</param>
        /// <param name="txtValue">当前值编辑框</param>
        private void __SetIntScroll(string strFeatureName, int nCurValue, TextBox txtValue)
        {
            txtValue.Text = nCurValue.ToString();
            m_objIGXFeatureControl.GetIntFeature(strFeatureName).SetValue(nCurValue);
        }

        /// <summary>
        /// 滑动条滑动消息响应函数
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_scrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            int nCurValue = 0;
            try
            {
                ScrollBar scrollBar = (ScrollBar)sender;
                nCurValue = scrollBar.Value;

                switch (scrollBar.Name)
                {
                    case "m_scrollBar_Gamma":
                        __SetFloatScroll("Gamma", nCurValue, m_txt_Gamma);
                        break;
                    case "m_scrollBar_BalanceRatio":
                        __SetFloatScroll("BalanceRatio", nCurValue, m_txt_BalanceRatio);
                        break;
                    case "m_scrollBar_Saturation":
                        __SetIntScroll("Saturation", nCurValue, m_txt_Saturation);
                        break;
                    case "m_scrollBar_ExposureTime":
                        __SetFloatScroll("ExposureTime", nCurValue, m_txt_ExposureTime);
                        break;
                    case "m_scrollBar_ExpectedGrayValue":
                        __SetIntScroll("ExpectedGrayValue", nCurValue, m_txt_ExpectedGrayValue);
                        break;
                    case "m_scrollBar_Sharpness":
                        __SetFloatScroll("Sharpness", nCurValue, m_txt_Sharpness);
                        break;
                    case "m_scrollBar_HDRLongValue":
                        __SetIntScroll("HDRTargetLongValue", nCurValue, m_txt_HDRLongValue);
                        break;
                    case "m_scrollBar_HDRShortValue":
                        __SetIntScroll("HDRTargetShortValue", nCurValue, m_txt_HDRShortValue);
                        break;
                    case "m_scrollBar_HDRMainValue":
                        __SetIntScroll("HDRTargetMainValue", nCurValue, m_txt_HDRMainValue);
                        break;
                    default:
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            } 

        }

        /// <summary>
        /// 更新自动白平衡模式定时器
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_timer_UpdateAWB_Tick(object sender, EventArgs e)
        {
            bool            bIsReadable     = false;
            bool            bIsEnable       = false;
            string          strEnumCurVal   = "";
            List<string>    lstEnumEntry    = new List<string>();     // Combox将要填入的列表

            try
            {
                //如果自动白平衡方式为Once,设置成功后实际的白平衡方式会自动变为off
                //程序通过定时读取设备内部白平衡的当前值实现UI界面的更新
                if (0 == string.Compare(m_strAWBValue, "Once"))
                {
                    __GetControlStatus("BalanceWhiteAuto", ref bIsReadable, ref bIsEnable);
                    if (bIsReadable)
                    {
                        //获取自动白平衡枚举值
                        strEnumCurVal = m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetValue();
                    }

                    lstEnumEntry.AddRange(m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetEnumEntryList());

                    //判断设备自动白平衡是否变为OFF
                    if (0 == string.Compare(strEnumCurVal, "Off"))
                    {
                        for (int i = 0; i < lstEnumEntry.Count; i++)
                        {
                            if (0 == string.Compare(lstEnumEntry[i], "Off"))
                            {
                                m_cb_AWBMode.SelectedIndex = i;
                                m_strAWBValue = m_cb_AWBMode.Text;
                                __GetControlStatus("BalanceRatio", ref m_bIsBalanceRatioReadable, ref m_bIsBalanceRatioEnable);
                                __UpDateUIEnable();
                                break;
                            }
                        }
                    }
                    __RefreshWhiteRatio();
                }
                else if (0 == string.Compare(m_strAWBValue, "Continuous"))
                {
                    //获取当前白平衡系数，更新界面
                    __RefreshWhiteRatio();
                }
                else
                {
                }
            }
            catch (Exception)
            {
                return;
            } 
        }

        /// <summary>
        /// 更新白平衡系数
        /// </summary>
        private void __RefreshWhiteRatio()
        {
            double  dBalanceRatio   = 0.0;
            bool    bIsReadable     = false;
            bool    bIsEnable       = false;

            //获取当前白平衡系数，更新界面
            __GetControlStatus("BalanceRatio", ref bIsReadable, ref bIsEnable);
            if (bIsReadable)
            {
                dBalanceRatio = m_objIGXFeatureControl.GetFloatFeature("BalanceRatio").GetValue();

                m_txt_BalanceRatio.Text = dBalanceRatio.ToString("f4");
            }
        }

        /// <summary>
        /// 更新自动曝光定时器
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_timer_UpdateExposureAuto_Tick(object sender, EventArgs e)
        {
            bool            bIsReadable     = false;
            bool            bIsEnable       = false;
            string          strEnumCurVal   = "";
            List<string>    lstEnumEntry    = new List<string>();     // Combox将要填入的列表

            try
            {
                //如果自动曝光方式为Once,设置成功后实际的曝光方式会自动变为off
                //程序通过定时读取设备内部曝光的当前值实现UI界面的更新
                if (0 == string.Compare(m_strExposureAutoValue, "Once"))
                {
                    __GetControlStatus("ExposureAuto", ref bIsReadable, ref bIsEnable);
                    if (bIsReadable)
                    {
                        //获取自动曝光枚举值
                        strEnumCurVal = m_objIGXFeatureControl.GetEnumFeature("ExposureAuto").GetValue();
                    }

                    lstEnumEntry.AddRange(m_objIGXFeatureControl.GetEnumFeature("ExposureAuto").GetEnumEntryList());

                    //判断设备自动曝光是否变为OFF
                    if (0 == string.Compare(strEnumCurVal, "Off"))
                    {
                        for (int i = 0; i < lstEnumEntry.Count; i++)
                        {
                            if (0 == string.Compare(lstEnumEntry[i], "Off"))
                            {
                                m_cb_ExposureAuto.SelectedIndex = i;
                                m_strExposureAutoValue = m_cb_ExposureAuto.Text;
                                __GetControlStatus("ExposureTime", ref m_bIsExposureTimeReadable, ref m_bIsExposureTimeEnable);
                                __UpDateUIEnable();
                                break;
                            }
                        }
                    }
                    __RefreshExposureTime();
                }
                else if (0 == string.Compare(m_strExposureAutoValue, "Continuous"))
                {
                    __RefreshExposureTime();
                }
                else
                {
                }
            }
            catch (Exception)
            {
                return;
            } 
        }

        /// <summary>
        /// 更新曝光时间
        /// </summary>
        private void __RefreshExposureTime()
        {
            double  dExposureTime   = 0.0;
            bool    bIsReadable     = false;
            bool    bIsEnable       = false;

            //获取当前曝光值，更新界面
            __GetControlStatus("ExposureTime", ref bIsReadable, ref bIsEnable);
            if (bIsReadable)
            {
                dExposureTime = m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetValue();
                m_txt_ExposureTime.Text = dExposureTime.ToString("f4");
            }
        }

        /// <summary>
        /// 关闭窗口
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void CameraParamForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            m_timer_UpdateAWB.Stop();
            m_timer_UpdateExposureAuto.Stop();
            m_timer_UpdateGainAuto.Stop();
        }

        /// <summary>
        /// 更新自动增益定时器
        /// </summary>
        /// <param name="sender">控件对象</param>
        /// <param name="e">事件</param>
        private void m_timer_UpdateGainAuto_Tick(object sender, EventArgs e)
        {
            bool            bIsReadable     = false;
            bool            bIsEnable       = false;
            string          strEnumCurVal   = "";
            List<string>    lstEnumEntry    = new List<string>();     // Combox将要填入的列表

            try
            {
                //如果自动曝光方式为Once,设置成功后实际的曝光方式会自动变为off
                //程序通过定时读取设备内部曝光的当前值实现UI界面的更新
                if (0 == string.Compare(m_strGainAutoValue, "Once"))
                {
                    __GetControlStatus("GainAuto", ref bIsReadable, ref bIsEnable);
                    if (bIsReadable)
                    {
                        //获取自动曝光枚举值
                        strEnumCurVal = m_objIGXFeatureControl.GetEnumFeature("GainAuto").GetValue();
                    }

                    lstEnumEntry.AddRange(m_objIGXFeatureControl.GetEnumFeature("GainAuto").GetEnumEntryList());

                    //判断设备自动曝光是否变为OFF
                    if (0 == string.Compare(strEnumCurVal, "Off"))
                    {
                        for (int i = 0; i < lstEnumEntry.Count; i++)
                        {
                            if (0 == string.Compare(lstEnumEntry[i], "Off"))
                            {
                                m_cb_GainAuto.SelectedIndex = i;
                                m_strGainAutoValue = m_cb_GainAuto.Text;
                                break;
                            }
                        }
                    }
                }
            }
            catch (Exception)
            {
                return;
            } 
        }
    }
}
