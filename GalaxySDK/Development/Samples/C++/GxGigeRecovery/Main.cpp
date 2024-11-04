//----------------------------------------------------------------------------------
/**
\file    Main.cpp 
\brief   Gige recovery function
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "GxGigeRecovery.h"
int _tmain(int argc, _TCHAR* argv[])
{
	//调用CGigeRecovery类实现设备掉线重连功能
	CGigeRecovery objCtrDevice;
	objCtrDevice.GigeRecovery();

	//按下任意键关闭控制台
	std::cout<<"\n<press any key to exit>"<<std::endl;
	while (!_kbhit());

	return 0;
}