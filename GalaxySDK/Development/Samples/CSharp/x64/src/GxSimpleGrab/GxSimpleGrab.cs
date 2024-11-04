using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using GxIAPINET;

namespace GxSimpleGrab
{
    class Sample
    {
        static public volatile bool threadState = false;
        static public IGXFeatureControl m_remoteFeatureControl = null;

        static void Main(string[] args)
        {
            try
            {
                // initialization
                IGXFactory.GetInstance().Init();

                // Enumerating cameras
                List<IGXDeviceInfo> listGxDeviceInfo = new List<IGXDeviceInfo>();
                IGXFactory.GetInstance().UpdateAllDeviceList(300, listGxDeviceInfo);
                if (listGxDeviceInfo.Count < 1)
                {
                    Console.WriteLine("Device not found");
                    IGXFactory.GetInstance().Uninit();
                    Console.WriteLine("Press any key to exit...");
                    Console.ReadKey();
                    return;
                }

                Int32 nDeviceCount = listGxDeviceInfo.Count;
                Console.WriteLine("Found Device: " + nDeviceCount);

                Int32 index = 0;
                foreach (IGXDeviceInfo deviceInfo in listGxDeviceInfo)
                {
                    Console.WriteLine($"\tID: {index}, SN: {deviceInfo.GetSN()}");
                    Console.WriteLine();
                    ++index;
                }

                index = 0;
                string deviceSN = listGxDeviceInfo[index].GetSN();
                IGXDevice objDevice = IGXFactory.GetInstance().OpenDeviceBySN(deviceSN, GX_ACCESS_MODE.GX_ACCESS_CONTROL);
                m_remoteFeatureControl = objDevice.GetRemoteFeatureControl();

                // Restore default parameter group
                m_remoteFeatureControl.GetEnumFeature("UserSetSelector").SetValue("Default");
                m_remoteFeatureControl.GetCommandFeature("UserSetLoad").Execute();

                Console.WriteLine("***********************************************");
                Console.WriteLine($"<Vendor Name:   {listGxDeviceInfo[index].GetVendorName()}>");
                Console.WriteLine($"<Model Name:    {listGxDeviceInfo[index].GetModelName()}>");
                Console.WriteLine("***********************************************");
                Console.WriteLine("Press [a] or [A] and then press [Enter] to start acquisition");
                Console.WriteLine("Press [x] or [X] and then press [Enter] to Exit the Program");

                bool bWaitStart = true;
                while (bWaitStart)
                {
                    ConsoleKeyInfo key = Console.ReadKey();
                    switch (key.Key)
                    {
                        case ConsoleKey.A:
                            bWaitStart = false;
                            break;
                        case ConsoleKey.X:
                            Console.WriteLine("<App exit!>");
                            objDevice.Close();
                            IGXFactory.GetInstance().Uninit();
                            return;
                        default:
                            break;
                    }
                }

                // Open the specified flow channel
                IGXStream objStream = objDevice.OpenStream(0);
                
                threadState = true;
                Thread thread = new Thread(new ParameterizedThreadStart(CaptureThread));
                thread.Start(objStream);

                bWaitStart = true;
                while (bWaitStart)
                {
                    ConsoleKeyInfo key = Console.ReadKey();
                    switch (key.Key)
                    {
                        case ConsoleKey.X:
                            bWaitStart = false;
                            break;
                        default:
                            break;
                    }
                }

                threadState = false;
                thread.Join();
                objStream.Close();
                objDevice.Close();
                IGXFactory.GetInstance().Uninit();
            }
            catch (CGalaxyException ex)
            {
                Console.WriteLine("GalaxyException: " + ex.Message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exception: " + ex.Message);
            }

            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }

        public static void CaptureThread(Object objStream)
        {
            IGXStream stream = (IGXStream)objStream;
            stream.StartGrab();
            m_remoteFeatureControl.GetCommandFeature("AcquisitionStart").Execute();

            while (threadState)
            {
                try
                {
                    IFrameData frameData = stream.DQBuf(100);
                    if (frameData.GetStatus() == GX_FRAME_STATUS_LIST.GX_FRAME_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"<Successful acquisition: Width: {frameData.GetWidth()}, " +
                            $"Height: {frameData.GetHeight()}, FrameID: {frameData.GetFrameID()}>");
                    }
                    else
                    {
                        Console.WriteLine($"<Abnormal Acquisition>");
                    }
                    stream.QBuf(frameData);
                }
                catch (CGalaxyException ex)
                {
                    Console.WriteLine("GalaxyException: " + ex.Message);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Exception: " + ex.Message);
                }
            }

            m_remoteFeatureControl.GetCommandFeature("AcquisitionStop").Execute();
            stream.StopGrab();
            Console.WriteLine("Acquisition thread Exit!");
        }
    }
}