using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GxIAPINET;

namespace GxGigeRecovery
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // 调用CGigeRecovery类实现掉线重连功能
                CGigeRecovery objCtrDevice = new CGigeRecovery();
                objCtrDevice.GigeRecovery();

                // 按下任意键关闭控制台
                Console.Write("\n<press any key to exit>\n");
                while (!System.Console.KeyAvailable) ;
                IGXFactory.GetInstance().Uninit();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
