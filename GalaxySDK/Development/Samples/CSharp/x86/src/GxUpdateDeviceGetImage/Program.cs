using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using GxIAPINET;
using GxIAPINET.Sample.Common;

namespace GxUpdateDeviceGetImage
{
    class Sample
    {
        static void Main(string[] args)
        {
            try
            {
                // 初始化
                IGXFactory.GetInstance().Init();

                // 枚举相机
                List<IGXDeviceInfo> listGxDeviceInfo = new List<IGXDeviceInfo>();
                UpdateDevice(ref listGxDeviceInfo);
                if (listGxDeviceInfo.Count < 1)
                {
                    Console.WriteLine("Not Find Device");
                    IGXFactory.GetInstance().Uninit();
                    Console.ReadKey();
                    return;
                }

                int nDeviceCount = listGxDeviceInfo.Count;
                Console.WriteLine("Found Device: " + nDeviceCount);

                int index = 0;
                foreach (IGXDeviceInfo deviceInfo in listGxDeviceInfo)
                {
                    Console.WriteLine("\tID: " + index + ", SN: " + deviceInfo.GetSN());

                    if (deviceInfo.GetDeviceClass() == GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_SMART ||
                        deviceInfo.GetDeviceClass() == GX_DEVICE_CLASS_LIST.GX_DEVICE_CLASS_GEV)
                    {
                        Console.WriteLine("\tID: " + index + ", IP: " + deviceInfo.GetIP());
                    }

                    Console.WriteLine();
                    ++index;
                }

                index = 0;
                Console.WriteLine("Default test first device...");

                string deviceSN = listGxDeviceInfo[index].GetSN();
                IGXDevice objDevice = IGXFactory.GetInstance().OpenDeviceBySN(deviceSN, GX_ACCESS_MODE.GX_ACCESS_CONTROL);
                IGXFeatureControl featureControl = objDevice.GetRemoteFeatureControl();

                Console.WriteLine("Restore default parameter group...");

                // 恢复默认参数组
                featureControl.GetEnumFeature("UserSetSelector").SetValue("Default");
                featureControl.GetCommandFeature("UserSetLoad").Execute();

                // 打开指定流通道
                IGXStream objStream = objDevice.OpenStream(0);

                // 单帧采集
                featureControl.GetEnumFeature("AcquisitionMode").SetValue("SingleFrame");

                UInt32 timeoutMS = 500;

                objStream.StartGrab();
                featureControl.GetCommandFeature("AcquisitionStart").Execute();

                IImageData imageData;
                try
                {
                    imageData = objStream.GetImage(timeoutMS);
                    Console.WriteLine("Successfully obtained an image");
                    Console.WriteLine("\tW * H = " + imageData.GetWidth() + " * " + imageData.GetHeight());

                    do
                    {
                        if (imageData.GetStatus() != GX_FRAME_STATUS_LIST.GX_FRAME_STATUS_SUCCESS)
                        {
                            Console.WriteLine("Error Frame");
                            break;
                        }

                        Console.WriteLine("Success Frame, Frame ID: " + imageData.GetFrameID() + ", TimeStamp: " + imageData.GetTimeStamp());

                        bool isColoreImage = false;
                        IsSupportColor(objDevice, ref isColoreImage);
                        Int32 outputImageSize = (Int32)(isColoreImage ? (imageData.GetPayloadSize() * 3) : (imageData.GetPayloadSize()));
                        IntPtr pOutputImageBuffer = Marshal.AllocHGlobal(outputImageSize);

                        IGXImageProcess imageProcess = IGXFactory.GetInstance().CreateImageProcess();
                        IImageProcessConfig imageProcessConfig = objDevice.CreateImageProcessConfig();
                        imageProcessConfig.SetValidBit(GetBestValudBit(imageData.GetPixelFormat()));
                        imageProcess.ImageImprovment(imageData, pOutputImageBuffer, imageProcessConfig);

                        Marshal.FreeHGlobal(pOutputImageBuffer);
                    } while (false);
                }
                catch (Exception e)
                {
                    Console.WriteLine("GetImage error: " + e.Message);
                }

                objDevice.GetRemoteFeatureControl().GetCommandFeature("AcquisitionStop").Execute();
                objStream.StopGrab();

                objDevice.Close();
            }
            catch (CGalaxyException ex)
            {
                Console.WriteLine("Main CGXException: " + ex.Message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Main Exception: " + ex.Message);
            }

            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
            IGXFactory.GetInstance().Uninit();
        }

        /// <summary>
        /// 是否支持彩色
        /// </summary>
        /// <param name="bIsColorFilter">是否支持彩色</param>
        static void IsSupportColor(IGXDevice objDevice, ref bool bIsColorFilter)
        {
            bool bIsImplemented = false;
            bool bIsMono = false;
            string strPixelFormat = "";
            uint nPixelFormatValue = 0;

            strPixelFormat = objDevice.GetRemoteFeatureControl().GetEnumFeature("PixelFormat").GetValue();
            if (0 == string.Compare(strPixelFormat, 0, "Mono", 0, 4))
            {
                bIsMono = true;
            }
            else
            {
                bIsMono = false;
            }


            // 通过当前像素格式判断是否为彩色相机
            CDecide.GetConvertPixelFormat(strPixelFormat, ref nPixelFormatValue);
            bIsImplemented = CDecide.GetIsGray(nPixelFormatValue);
            if ((!bIsMono) && (!bIsImplemented))
            {
                bIsColorFilter = true;
            }
            else
            {
                bIsColorFilter = false;
            }
        }

        /// <summary>
        /// 枚举相机
        /// </summary>
        /// <param name="listGxDeviceInfo">设备信息列表</param>
        static void UpdateDevice(ref List<IGXDeviceInfo> listGxDeviceInfo)
        {
            List<IGXDeviceInfo> tempDeviceInfoList = new List<IGXDeviceInfo>();
            IGXFactory.GetInstance().UpdateAllDeviceList(300, tempDeviceInfoList);

            listGxDeviceInfo = tempDeviceInfoList;
        }

        /// <summary>
        /// 通过GX_PIXEL_FORMAT_ENTRY获取最优Bit位
        /// </summary>
        /// <param name="em">图像数据格式</param>
        /// <returns>最优Bit位</returns>
        static GX_VALID_BIT_LIST GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
        {
            GX_VALID_BIT_LIST emValidBits = GX_VALID_BIT_LIST.GX_BIT_0_7;
            switch (emPixelFormatEntry)
            {
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_RGB8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BGR8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_R8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_G8:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_B8:
                    {
                        emValidBits = GX_VALID_BIT_LIST.GX_BIT_0_7;
                        break;
                    }
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10_PACKED:
                    {
                        emValidBits = GX_VALID_BIT_LIST.GX_BIT_2_9;
                        break;
                    }
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12_PACKED:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12_PACKED:
                    {
                        emValidBits = GX_VALID_BIT_LIST.GX_BIT_4_11;
                        break;
                    }
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO14:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR14:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG14:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB14:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG14:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG14_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB14_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR14_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG14_P:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO14_P:
                    {
                        emValidBits = GX_VALID_BIT_LIST.GX_BIT_6_13;
                        break;
                    }
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO16:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR16:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG16:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB16:
                case GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG16:
                    {
                        emValidBits = GX_VALID_BIT_LIST.GX_BIT_8_15;
                        break;
                    }
                default:
                    break;
            }
            return emValidBits;
        }
    }
}
