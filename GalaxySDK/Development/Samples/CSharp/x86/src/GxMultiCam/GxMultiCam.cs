using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxMultiCam
{
    public partial class GxMultiCam : Form
    {
       
        List<CCamerInfo>             m_listCCamerInfo        = new List<CCamerInfo>();           ///<相机参数状态列表
        IGXFactory                   m_objIGXFactory         = null;                             ///<Factory对像
        List<IGXDeviceInfo>          m_listIGXDeviceInfo     = new List<IGXDeviceInfo>();        ///<设备信息列表
        int                          m_nCamNum	             = 0;								 ///<初始化相机数目
        int                          m_nOperateID            = -1;                               ///<操作相机编号        

        

        /// <summary>
        /// 构造函数
        /// </summary>
        public GxMultiCam()
        {
            InitializeComponent();    
        }

        /// <summary>
        ///加载窗体时初始化界面
        /// </summary>
        private void __LoadInitUI()
        {
            //相机控制相关使能操作
            m_btn_OpenDevice.Enabled = false;
            m_btn_CloseDevice.Enabled = false;
            m_btn_StartDevice.Enabled = false;
            m_btn_StopDevice.Enabled =  false;

            m_txt_Shutter.Enabled = false;
            m_txt_Gain.Enabled = false;
            m_txt_AcqSpeedLevel.Enabled = false;
            m_cb_AutoWhite.Enabled = false;
        }

        /// <summary>
        /// 设备打开后初始化界面
        /// </summary>
        private void __InitUI()
        {
            __InitShutterUI();                                                                                                   //曝光初始化
            __InitGainUI();                                                                                                      //增益的初始化
            __InitAcquisitionSpeedLevelUI();                                                                                     //采集速度级别的初始化
            __InitEnumComBoxUI(m_cb_AutoWhite, "BalanceWhiteAuto"
                            , m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl
                            , ref m_listCCamerInfo[m_nOperateID].m_bWhiteAuto);                                                 //自动白平衡的初始化
            //获取白平衡当前的值
            bool bIsImplemented = false;             //是否支持
            bool bIsReadable = false;                //是否可读
            // 获取是否支持
            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                bIsImplemented = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsImplemented("BalanceWhiteAuto");
                bIsReadable = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsReadable("BalanceWhiteAuto");
                if (bIsImplemented)
                {
                    if (bIsReadable)
                    {
                        //获取当前功能值
                        m_listCCamerInfo[m_nOperateID].m_strBalanceWhiteAutoValue = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetValue();
                    }
                }
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
            List<string> list = new List<string>();        //Combox将要填入的列表
            bool bIsReadable = false;                      //是否可读
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
        /// 当枚举的相机个数超过4个，按4个显示，小于等于4个按实际值显示
        /// 当枚举的相机个数小于0按0返回
        /// </summary>
        /// <param name="nDeviceCount">实际枚举到的个数</param>
        /// <returns>显示的最小个数</returns>
        private int __GetMin(int nDeviceCount)
        {
            if (nDeviceCount < 0)
            {
                return 0;
            }

            if (nDeviceCount > 4)
            {
                return 4;
            }

            return nDeviceCount;
        }

        /// <summary>
        /// 加载窗体执行初始化UI和库
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxMultiCam_Load(object sender, EventArgs e)
        {
            try
            {
                __LoadInitUI();
                m_objIGXFactory = IGXFactory.GetInstance();
                m_objIGXFactory.Init();
                __EnumDevice();     
            }
            catch (Exception)
            {
            }
        }

        /// <summary>
        /// 更新界面
        /// </summary>
        private void __UpdateUI()
        {
            bool bIsDevOpened = m_listCCamerInfo[m_nOperateID].m_bIsOpen;
            bool bIsSnap = m_listCCamerInfo[m_nOperateID].m_bIsSnap;
            bool bWhiteAuto = m_listCCamerInfo[m_nOperateID].m_bWhiteAuto;
            bool bAcqSpeedLevel = m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel;

            //相机控制相关使能操作
            m_btn_OpenDevice.Enabled = !bIsDevOpened;
            m_btn_CloseDevice.Enabled = bIsDevOpened;
            m_btn_StartDevice.Enabled = bIsDevOpened && !bIsSnap;
            m_btn_StopDevice.Enabled  = bIsSnap;

            //相机参数相关的使能操作
            m_txt_Shutter.Enabled = bIsDevOpened;
            m_txt_Gain.Enabled = bIsDevOpened;
            m_txt_AcqSpeedLevel.Enabled = bIsDevOpened && (!bIsSnap) && bAcqSpeedLevel;
            m_cb_AutoWhite.Enabled = bIsDevOpened && bWhiteAuto;       
        }

        /// <summary>
        /// 枚举设备
        /// </summary>
        private void __EnumDevice()
        {
            m_listIGXDeviceInfo.Clear();
            if (null != m_objIGXFactory)
            {
                m_objIGXFactory.UpdateAllDeviceList(200, m_listIGXDeviceInfo);
            }

            // 判断当前连接设备个数
            if (m_listIGXDeviceInfo.Count <= 0)
            {
                MessageBox.Show("未检测到设备,请确保设备正常连接然后重启程序!");
                return;
            }

            m_nCamNum = __GetMin(m_listIGXDeviceInfo.Count);
            m_cb_EnumDevice.Items.Clear();
            for (int i = 0; i < m_nCamNum;i++ )
            {
                m_cb_EnumDevice.Items.Add(m_listIGXDeviceInfo[i].GetDisplayName());
                CCamerInfo objCCamerInfo = new CCamerInfo();
                objCCamerInfo.m_strDisplayName = m_listIGXDeviceInfo[i].GetDisplayName();
                objCCamerInfo.m_strSN = m_listIGXDeviceInfo[i].GetSN();
                objCCamerInfo.m_emDeviceType = m_listIGXDeviceInfo[i].GetDeviceClass();
                m_listCCamerInfo.Add(objCCamerInfo);
            }

            if (m_cb_EnumDevice.Items.Count > 0)
            {
                m_cb_EnumDevice.SelectedIndex = 0;
            }

            for (int i = 0; i < m_nCamNum; i++)
            {
                if (m_cb_EnumDevice.Text == m_listCCamerInfo[i].m_strDisplayName)
                {
                    m_nOperateID = i;
                }
            }
           
            __UpdateUI();
        }

        /// <summary>
        /// 关闭所有的设备和流
        /// </summary>
        private void __CloseAll()
        {
            for (int i = 0; i < m_listCCamerInfo.Count; i++)
            {
                if (null != m_listCCamerInfo[m_nOperateID].m_objImageShowFrom)
                {
                    m_listCCamerInfo[m_nOperateID].m_objImageShowFrom.CloseDevice();
                    m_listCCamerInfo[i].m_bIsSnap = false;
                    m_listCCamerInfo[i].m_bIsOpen = false;
                }
            }
            m_listCCamerInfo.Clear();
        }

        /// <summary>
        /// 初始化参数
        /// </summary>
        private void __InitParam()
        {
            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                //设置采集模式连续采集
                m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("AcquisitionMode").SetValue("Continuous");

                //设置触发模式为关
                m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("TriggerMode").SetValue("Off");
            }
        }

        /// <summary>
        /// 曝光控制界面初始化
        /// </summary>
        private void __InitShutterUI()
        {
            double dCurShuter = 0.0;                 //当前曝光值
            double dMin = 0.0;                       //最小值
            double dMax = 0.0;                       //最大值
            string strUnit = "";                     //单位
            string strText = "";                     //显示内容

            //获取当前相机的曝光值、最小值、最大值和单位
            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                dCurShuter = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetValue();
                dMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMin();
                dMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMax();
                strUnit = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetUnit();
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
            double dMin = 0.0;               //最小值
            double dMax = 0.0;               //最大值
            string strUnit = "";             //单位
            string strText = "";             //显示内容

            //获取当前相机的增益值、最小值、最大值和单位
            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                dCurGain = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetValue();
                dMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetMin();
                dMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetMax();
                strUnit = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetUnit();
            }

            //更新增益值范围到界面
            strText = string.Format("增益({0}~{1}){2}", dMin.ToString("0.00"), dMax.ToString("0.00"), strUnit);
            m_lbl_Gain.Text = strText;

            //当前的增益值刷新到增益的编辑框
            string strCurGain = dCurGain.ToString("0.00");
            m_txt_Gain.Text = strCurGain;
        }

        /// <summary>
        /// 采集速度级别界面初始化
        /// </summary>
        private void __InitAcquisitionSpeedLevelUI()
        {
            Int64 nCurAcquisitionSpeedLevel = 0;             //当前采集速度级别
            Int64 nMin = 0;           //最小值
            Int64 nMax = 0;           //最大值
            string strText = "";      //显示内容

            //获取当前相机的采集速度级别、最小值、最大值
            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                if (!(m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsImplemented("AcquisitionSpeedLevel")))
                {
                    m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = false;

                    //更新采集速度级别值范围到界面
                    m_lbl_AcqSpeedLevel.Text = "采集速度级别";
                    m_txt_AcqSpeedLevel.Text = "";
                    return;
                }

                m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = true;
                nCurAcquisitionSpeedLevel = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetValue();
                nMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetMin();
                nMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetMax();
            }

            //更新采集速度级别值范围到界面
            strText = string.Format("采集速度级别({0}~{1})", nMin.ToString(), nMax.ToString());
            m_lbl_AcqSpeedLevel.Text = strText;

            //当前的采集速度级别值刷新到增益的编辑框
            string strCurAcquisitionSpeedLevel = nCurAcquisitionSpeedLevel.ToString();
            m_txt_AcqSpeedLevel.Text = strCurAcquisitionSpeedLevel;
        }

        /// <summary>
        /// 点击"打开设备"按钮消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_OpenDevice_Click(object sender, EventArgs e)
        {
            string strSN = ""; //打开设备用的序列号
            try
            {
                // 判断当前连接设备个数
                if (m_listIGXDeviceInfo.Count <= 0)
                {
                    MessageBox.Show("未发现设备!");
                    return;
                }

                try
                {
                    //停止流通道、注销采集回调和关闭流
                    if (null != m_listCCamerInfo[m_nOperateID].m_objIGXStream)
                    {
                        m_listCCamerInfo[m_nOperateID].m_objIGXStream.Close();
                        m_listCCamerInfo[m_nOperateID].m_objIGXStream = null;
                    }
                }
                catch (Exception)
                {
                }


                // 如果设备已经打开则关闭，保证相机在初始化出错情况下能再次打开
                if (null != m_listCCamerInfo[m_nOperateID].m_objIGXDevice)
                {
                    m_listCCamerInfo[m_nOperateID].m_objIGXDevice.Close();
                    m_listCCamerInfo[m_nOperateID].m_objIGXDevice = null;
                }

                strSN = m_listIGXDeviceInfo[m_nOperateID].GetSN();
                IGXDevice objIGXDevice= m_objIGXFactory.OpenDeviceBySN(strSN, GX_ACCESS_MODE.GX_ACCESS_EXCLUSIVE);
                //打开列表选中的设备
                m_listCCamerInfo[m_nOperateID].m_objIGXDevice = objIGXDevice;
                m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl = m_listCCamerInfo[m_nOperateID].m_objIGXDevice.GetRemoteFeatureControl();

                __InitParam();

                // 获取相机参数,初始化界面控件
                __InitUI();

                ImageShowFrom objImageShowFrom = new ImageShowFrom(objIGXDevice, m_nOperateID, m_objIGXFactory);
                objImageShowFrom.OpenDevice();
                m_listCCamerInfo[m_nOperateID].m_objImageShowFrom = objImageShowFrom;
                __SelectDeviceAndShow(m_nOperateID);

                // 更新设备打开标识
                m_listCCamerInfo[m_nOperateID].m_bIsOpen = true;

                //刷新界面
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 根据对应的设备对象绑定显示控件
        /// </summary>
        /// <param name="nCamID">相机ID</param>
        private void __SelectDeviceAndShow(int nCamID)
        {
            switch (nCamID)
            {
                case 0:
                    {
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.MdiParent = this;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.StartPosition = FormStartPosition.Manual;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Location = new Point(0, 0);
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Show();
                    }
                    break;
                case 1:
                    {
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.MdiParent = this;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.StartPosition = FormStartPosition.Manual;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Location = new Point(390, 0);
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Show();
                    }
                    break;
                case 2:
                    {
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.MdiParent = this;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.StartPosition = FormStartPosition.Manual;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Location = new Point(0, 330);
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Show();
                    }
                    break;
                case 3:
                    {
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.MdiParent = this;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.StartPosition = FormStartPosition.Manual;
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Location = new Point(390,330);
                        m_listCCamerInfo[nCamID].m_objImageShowFrom.Show();
                    }
                    break;
                default:
                    break;
            }

        }

        /// <summary>
        /// 选择对应的设备
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_EnumDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                this.m_cb_EnumDevice.Focus();
                for (int i = 0; i < m_nCamNum; i++)
                {
                    if (m_cb_EnumDevice.Text == m_listCCamerInfo[i].m_strDisplayName)
                    {
                        m_nOperateID = i;
                        if (m_listCCamerInfo[m_nOperateID].m_bIsOpen)
                        {
                            __InitUI();
                        }
                        break;
                    }                    
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
        /// 点击"关闭设备"按钮消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_CloseDevice_Click(object sender, EventArgs e)
        {
            try
            {
                if (null != m_listCCamerInfo[m_nOperateID].m_objImageShowFrom)
                {
                    m_listCCamerInfo[m_nOperateID].m_objImageShowFrom.CloseDevice();
                    m_listCCamerInfo[m_nOperateID].m_bIsSnap = false;
                    m_listCCamerInfo[m_nOperateID].m_bIsOpen = false;
                }
                //刷新界面
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 点击"开始采集"按钮消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_StartDevice_Click(object sender, EventArgs e)
        {
            try
            {
                m_listCCamerInfo[m_nOperateID].m_objImageShowFrom.StartDevice();
                m_listCCamerInfo[m_nOperateID].m_bIsSnap = true;
                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 点击"停止采集"按钮消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_StopDevice_Click(object sender, EventArgs e)
        {
            try
            {
                m_listCCamerInfo[m_nOperateID].m_objImageShowFrom.StopDevice();
                m_listCCamerInfo[m_nOperateID].m_bIsSnap = false;
                // 更新界面UI
                __UpdateUI();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// 窗体关闭的时候释放资源
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxMultiCam_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                this.m_cb_EnumDevice.Focus();
                __CloseAll();
                m_objIGXFactory.Uninit();

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
                if (!m_listCCamerInfo[m_nOperateID].m_bWhiteAutoSelectedIndex)
                {
                    return;
                }

                string strValue = m_cb_AutoWhite.Text;
                __SetEnumValue("BalanceWhiteAuto", strValue, m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl);
               
                m_listCCamerInfo[m_nOperateID].m_strBalanceWhiteAutoValue = strValue;

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
        /// 限定采集速度级别只能输入整数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_txt_AcqSpeedLevel_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsNumber(e.KeyChar) && e.KeyChar != 8)
            {
                e.Handled = true;
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
        /// 判断是否有回车按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxMultiCam_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
            {
                this.m_cb_EnumDevice.Focus();
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

        private void m_txt_AcqSpeedLevel_Leave(object sender, EventArgs e)
        {
            Int64 nCurAcquisitionSpeedLevel = 0;   //采集速度级别
            Int64 nMin = 0;                        //最小值
            Int64 nMax = 0;                        //最大值
            string strValue = "";                  //用于存放设置前的值
            try
            {
                if (!m_listCCamerInfo[m_nOperateID].m_bIsOpen)
                {
                    return;
                }

                strValue = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetValue().ToString();

                try
                {
                    nCurAcquisitionSpeedLevel = Convert.ToInt64(m_txt_AcqSpeedLevel.Text);     //当前采集速度级别
                }
                catch (Exception)
                {
                    MessageBox.Show("请输入正确的采集速度级别");
                    m_txt_AcqSpeedLevel.Text = strValue;
                    return;
                }

                if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
                {
                    if (!(m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsImplemented("AcquisitionSpeedLevel")))
                    {
                        m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = false;
                        return;
                    }

                    nMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetMax();
                    nMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetMin();

                    if (nCurAcquisitionSpeedLevel > nMax)
                    {
                        nCurAcquisitionSpeedLevel = nMax;
                    }
                    //若小于最小值将曝光值设为最小值
                    if (nCurAcquisitionSpeedLevel < nMin)
                    {
                        nCurAcquisitionSpeedLevel = nMin;
                    }

                    m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = true;
                    m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").SetValue(nCurAcquisitionSpeedLevel);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
                {
                    if (!(m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsImplemented("AcquisitionSpeedLevel")))
                    {
                        m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = false;
                    }
                    else
                    {
                        m_listCCamerInfo[m_nOperateID].m_bAcqSpeedLevel = true;
                        m_txt_AcqSpeedLevel.Text = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetIntFeature("AcquisitionSpeedLevel").GetValue().ToString();
                    }
                }
                GxMultiCam.ActiveForm.Focus();
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
            string strValue ="";                     //曝光改变前的值
            try
            {
                if (!m_listCCamerInfo[m_nOperateID].m_bIsOpen)
                {
                    return;
                }

                strValue = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetValue().ToString();

                try
                {
                    dShutterValue = Convert.ToDouble(m_txt_Shutter.Text);
                }
                catch (Exception)
                {
                    MessageBox.Show("请输入正确的曝光值");
                    m_txt_Shutter.Text = strValue;
                    return;
                }

                //获取当前相机的曝光值、最小值、最大值和单位
                if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
                {
                    dMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMin();
                    dMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").GetMax();
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
                    m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("ExposureTime").SetValue(dShutterValue);
                }
            }
            catch (CGalaxyException ex)
            {
                string strErrorInfo = "错误码为：" + ex.GetErrorCode().ToString() + "错误描述信息为：" + ex.Message;
                MessageBox.Show(strErrorInfo);
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
            string strValue = "";                     //改变前的增益值
            try
            {
                if (!m_listCCamerInfo[m_nOperateID].m_bIsOpen)
                {
                    return;
                }
                strValue = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetValue().ToString();
                try
                {
                    dGain = Convert.ToDouble(m_txt_Gain.Text);
                }
                catch (Exception)
                {
                    MessageBox.Show("请输入正确的增益值");
                    m_txt_Gain.Text = strValue; 
                    return;
                }

                //当前相机的增益值、最小值、最大值
                if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
                {
                    dMin = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetMin();
                    dMax = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").GetMax();

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
                    m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetFloatFeature("Gain").SetValue(dGain);
                }
            }
            catch (CGalaxyException ex)
            {
                string strErrorInfo = "错误码为：" + ex.GetErrorCode().ToString() + "错误描述信息为：" + ex.Message;
                MessageBox.Show(strErrorInfo);
            }
        }

        /// <summary>
        /// 主要让设备列表combox能够显示全
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_cb_EnumDevice_DropDown(object sender, EventArgs e)
        {
            Graphics objGraphics = null;
            Font objFont = null;

            try
            {
                int nWidth = m_cb_EnumDevice.Width;
                objGraphics = m_cb_EnumDevice.CreateGraphics();
                objFont = m_cb_EnumDevice.Font;
                int nVertScrollBarWidth = (m_cb_EnumDevice.Items.Count > m_cb_EnumDevice.MaxDropDownItems) ? SystemInformation.VerticalScrollBarWidth : 0;
                int nNewWidth;
                for (int i = 0; i < m_cb_EnumDevice.Items.Count; i++)
                {
                    nNewWidth = (int)objGraphics.MeasureString(m_cb_EnumDevice.GetItemText(m_cb_EnumDevice.Items[i]), objFont).Width + nVertScrollBarWidth;
                    if (nWidth < nNewWidth)
                    {
                        nWidth = nNewWidth;
                    }
                }
                m_cb_EnumDevice.DropDownWidth = nWidth;
            }
            catch
            {

            }
            finally
            {
                if (null != objGraphics)
                {
                    objGraphics.Dispose();
                }
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
                if (m_listCCamerInfo[m_nOperateID].m_strBalanceWhiteAutoValue == "Once")
                {
                    try
                    {
                        //获取自动白平衡枚举值
                        if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
                        {
                            strValue = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("BalanceWhiteAuto").GetValue();
                        }
                    }
                    catch (Exception)
                    {
                    }
                    m_listCCamerInfo[m_nOperateID].m_strBalanceWhiteAutoValue = strValue;

                    if (m_listCCamerInfo[m_nOperateID].m_strBalanceWhiteAutoValue == "Off")
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