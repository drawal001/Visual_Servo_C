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

namespace GxMultiCamForHDR
{
    public partial class GxMultiCamForHDR : Form
    {
       
        List<CCamerInfo>             m_listCCamerInfo        = new List<CCamerInfo>();           ///<相机参数状态列表
        IGXFactory                   m_objIGXFactory         = null;                             ///<Factory对像
        List<IGXDeviceInfo>          m_listIGXDeviceInfo     = new List<IGXDeviceInfo>();        ///<设备信息列表
        int                          m_nCamNum	             = 0;								 ///<初始化相机数目
        int                          m_nOperateID            = -1;                               ///<操作相机编号        
        CameraParamForm              m_objCameraParamForm    = null;                             ///<设备参数调节                                                                                                        
              
        
        /// <summary>
        /// 构造函数
        /// </summary>
        public GxMultiCamForHDR()
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
            m_btn_StopDevice.Enabled = false;
            m_btn_CameraParam.Enabled = false;

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
        private void GxMultiCamForHDR_Load(object sender, EventArgs e)
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
            m_btn_StopDevice.Enabled = bIsSnap;
            m_btn_CameraParam.Enabled = bIsDevOpened;

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
            bool bIsAcqModeEnable      = false;
            bool bIsTriggerModeEnable  = false;

            if (null != m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl)
            {
                //设置采集模式连续采集
                __GetControlStatus("AcquisitionMode", ref bIsAcqModeEnable);
                if (bIsAcqModeEnable)
                {
                    m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("AcquisitionMode").SetValue("Continuous");
                }

                //设置触发模式为关
                __GetControlStatus("TriggerMode", ref bIsTriggerModeEnable);
                if (bIsTriggerModeEnable)
                {
                    m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.GetEnumFeature("TriggerMode").SetValue("Off");
                }
            }
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
                // 开采之前更新信息
                UpDateImgInfo();

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
        private void GxMultiCamForHDR_FormClosed(object sender, FormClosedEventArgs e)
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
        /// 判断是否有回车按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GxMultiCamForHDR_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return)
            {
                this.m_cb_EnumDevice.Focus();
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
        /// 点击"参数设置"按钮消息响应函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void m_btn_CameraParam_Click(object sender, EventArgs e)
        {
            m_objCameraParamForm = new CameraParamForm(m_listCCamerInfo[m_nOperateID].m_objIGXDevice, m_nOperateID);
            m_objCameraParamForm.Owner = this;
            m_objCameraParamForm.ShowDialog();

        }

        /// <summary>
        /// 相机是否已打开
        /// </summary>
        public bool IsOpen()
        {
            return m_listCCamerInfo[m_nOperateID].m_bIsOpen;
        }

        /// <summary>
        /// 相机是否正在采集
        /// </summary>
        public bool IsSnap()
        {
            return m_listCCamerInfo[m_nOperateID].m_bIsSnap;
        }

        /// <summary>
        /// 更新相机信息
        /// </summary>
        public void UpDateImgInfo()
        {
            m_listCCamerInfo[m_nOperateID].m_objImageShowFrom.UpDateImgInfo();
        }

        /// <summary>
        /// 获取节点是否可读，是否使能
        /// </summary>
        /// <param name="strFeatureName">功能名</param>
        /// <param name="bIsEnable">是否使能</param>
        private void __GetControlStatus(string strFeatureName, ref bool bIsEnable)
        {
            bool bIsImplement = false;
            bool bIsWritable = false;
            bool bIsReadable = false;

            // 是否支持该节点
            bIsImplement = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsImplemented(strFeatureName);
            if (!bIsImplement)
            {
                bIsEnable = false;
                return;
            }
            else
            {
                // 支持该节点
                // 判断该节点是否可读
                bIsReadable = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsReadable(strFeatureName);
                if (!bIsReadable)
                {
                    bIsEnable = false;
                    return;
                }

                // 判断该节点是否可写
                bIsWritable = m_listCCamerInfo[m_nOperateID].m_objIGXFeatureControl.IsWritable(strFeatureName);

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

    }
}