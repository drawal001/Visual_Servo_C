using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxMultiCam
{
    public class CCamerInfo
    {
        public bool                  m_bIsColorFilter             = false;                                          ///<判断是否为彩色相机
        public bool                  m_bIsOpen                    = false;	                                        ///<相机已打开标志
        public bool                  m_bIsSnap                    = false;		                                    ///<相机正在采集标志
        public bool                  m_bWhiteAuto                 = false;                                          ///<标识是否支持白平衡
        public bool                  m_bAcqSpeedLevel             = false;                                          ///<采集速度级别是否支持
        public bool                  m_bWhiteAutoSelectedIndex    = true;                                           ///<白平衡列表框转换标志
        public double                m_dFps                       = 0.0;			                                ///<帧率
        public IGXDevice             m_objIGXDevice               = null;                                           ///<设备对像
        public IGXStream             m_objIGXStream               = null;                                           ///<流对像
        public IGXFeatureControl     m_objIGXFeatureControl       = null;                                           ///<远端设备属性控制器对像
        public string                m_strBalanceWhiteAutoValue   = "Off";                                          ///<自动白平衡当前的值
        public string                m_strDisplayName             = "";                                             ///<设备显示名称
        public string                m_strSN                      = "";                                             ///<序列号
        public GX_DEVICE_CLASS_LIST  m_emDeviceType               = GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_UNKNOWN;   ///<设备类型   
        public ImageShowFrom         m_objImageShowFrom           = null;                                           ///<用于图像的显示
    }
}
