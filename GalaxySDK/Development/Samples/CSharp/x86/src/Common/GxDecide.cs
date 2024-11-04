using System;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.IO;
using GxIAPINET;

namespace GxIAPINET.Sample.Common
{
    public static class CDecide
    {
        public const uint GVSP_PIX_MONO       = 0x01000000;   ///<判断是否为MONO格式的掩码
        public const uint GVSP_PIX_RGB        = 0x02000000;   ///<判断是否为RGB格式的掩码
        public const uint GVSP_PIX_CUSTOM     = 0x80000000;   ///<判断是否为自定义格式的掩码
        public const uint GVSP_PIX_COLOR_MASK = 0xFF000000;   ///<判断是否为彩色格式的掩码
        public const uint GVSP_PIX_ID_MASK    = 0x0000FFFF;   ///<将图像格式与下述宏定义做按位与（&）运算，可得到像素格式的ID

        /// <summary>
        /// 是否支持黑白
        /// </summary>
        public static bool GetISMono(uint nPixelFormat)
        {
            bool bIsMono = ((GVSP_PIX_COLOR_MASK & nPixelFormat) == GVSP_PIX_MONO);
            return bIsMono;
        }

        /// <summary>
        /// 是否支持彩色
        /// </summary>
        public static bool GetIsBayer(uint nPixelFormat)
        {
            bool bIsBayer = (((GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR8) <= (GVSP_PIX_ID_MASK & nPixelFormat)
                               && (GVSP_PIX_ID_MASK & nPixelFormat) <= (GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12))
                               || ((GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR14) <= (GVSP_PIX_ID_MASK & nPixelFormat)
                               && (GVSP_PIX_ID_MASK & nPixelFormat) <= (GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG14))
                               || ((GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR16) <= (GVSP_PIX_ID_MASK & nPixelFormat)
                               && (GVSP_PIX_ID_MASK & nPixelFormat) <= (GVSP_PIX_ID_MASK & (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG16)));
            return bIsBayer;
        }

        /// <summary>
        /// 是否支持RGB
        /// </summary>
        public static bool GetIsRGB(uint nPixelFormat)
        {
            bool bIsBayer = ((GVSP_PIX_COLOR_MASK & nPixelFormat) == GVSP_PIX_RGB);
            return bIsBayer;
        }

        /// <summary>
        /// 是否支持RGB8
        /// </summary>
        public static bool GetIsRGB8(uint nPixelFormat)
        {
            bool bIsRGB8 = ((nPixelFormat == (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_RGB8)
                           || (nPixelFormat == (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BGR8));
            return bIsRGB8;
        }

        /// <summary>
        /// 是否为黑白
        /// </summary>
        public static bool GetIsGray(uint nPixelFormat)
        {

            bool bIsGray = (GetISMono(nPixelFormat)) && (!GetIsBayer(nPixelFormat)) && (!GetIsRGB(nPixelFormat));
            return bIsGray;
        }

        /// <summary>
        /// 根据像素格式获取对应ID
        /// </summary>
        public static void GetConvertPixelFormat(String strPixelFormat, ref uint nPixelFormatValue)
        {
            switch (strPixelFormat)
            {
                case "Mono8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO8;
                    break;

                case "BayerRG8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG8;
                    break;

                case "BayerGB8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB8;
                    break;

                case "BayerGR8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR8;
                    break;

                case "BayerBG8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG8;
                    break;

                case "RGB8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_RGB8;
                    break;

                case "BGR8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BGR8;
                    break;

                case "Mono10":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10;
                    break;

                case "Mono10_Packed":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10_PACKED;
                    break;

                case "Mono10_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO10_P;
                    break;

                case "BayerRG10":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10;
                    break;

                case "BayerRG10_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10_P;
                    break;

                case "BayerRG10_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG10_PACKED;
                    break;

                case "BayerGB10":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10;
                    break;

                case "BayerGB10_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10_P;
                    break;

                case "BayerGB10_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB10_PACKED;
                    break;

                case "BayerGR10":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10;
                    break;

                case "BayerGR10_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10_P;
                    break;

                case "BayerGR10_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR10_PACKED;
                    break;

                case "BayerBG10":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10;
                    break;

                case "BayerBG10_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10_P;
                    break;

                case "BayerBG10_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG10_PACKED;
                    break;

                case "Mono12":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12;
                    break;

                case "Mono12_Packed":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12_PACKED;
                    break;

                case "Mono12_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO12_P;
                    break;

                case "BayerRG12":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12;
                    break;

                case "BayerRG12_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12_P;
                    break;

                case "BayerRG12_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG12_PACKED;
                    break;

                case "BayerGB12":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12;
                    break;

                case "BayerGB12_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12_P;
                    break;

                case "BayerGB12_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB12_PACKED;
                    break;

                case "BayerGR12":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12;
                    break;

                case "BayerGR12_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12_P;
                    break;

                case "BayerGR12_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR12_PACKED;
                    break;

                case "BayerBG12":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12;
                    break;

                case "BayerBG12_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12_P;
                    break;

                case "BayerBG12_Packet":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG12_PACKED;
                    break;

                case "Mono14":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO14;
                    break;

                case "Mono14_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO14_P;
                    break;

                case "BayerRG14":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG14;
                    break;

                case "BayerRG14_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG14_P;
                    break;

                case "BayerGB14":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB14;
                    break;

                case "BayerGB14_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB14_P;
                    break;

                case "BayerGR14":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR14;
                    break;

                case "BayerGR14_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR14_P;
                    break;

                case "BayerBG14":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG14;
                    break;

                case "BayerBG14_P":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG14_P;
                    break;

                case "Mono16":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_MONO16;
                    break;

                case "BayerRG16":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_RG16;
                    break;

                case "BayerGB16":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GB16;
                    break;

                case "BayerGR16":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_GR16;
                    break;

                case "BayerBG16":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_BAYER_BG16;
                    break;

                case "R8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_R8;
                    break;

                case "B8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_B8;
                    break;

                case "G8":
                    nPixelFormatValue = (uint)GX_PIXEL_FORMAT_ENTRY.GX_PIXEL_FORMAT_G8;
                    break;

                default:
                    nPixelFormatValue = 0;
                    break;
            }
        }
    }
}