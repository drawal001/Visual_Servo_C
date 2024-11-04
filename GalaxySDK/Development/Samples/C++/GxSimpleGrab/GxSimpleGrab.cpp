#include <iostream>
#include "stdafx.h"
#include "GalaxyIncludes.h"

using namespace std;

struct THREAD_PARAM
{
	bool bRun;
	CGXStreamPointer pStream;
};

// 线程函数
DWORD WINAPI GrabImgThread(LPVOID lpParam) 
{
	THREAD_PARAM* pThreadParam = (THREAD_PARAM*)lpParam;
	if (NULL == pThreadParam)
	{
		cout << "lpParam is NULL!" << endl;
		return 0;
	}

	while (pThreadParam->bRun)
	{
		try
		{
			//零拷贝采集一帧图像
			CImageDataPointer pImgData = pThreadParam->pStream->DQBuf(1000);
			if (GX_FRAME_STATUS_SUCCESS == pImgData->GetStatus())
			{
				cout << "<Successful acquisition: Width: " << pImgData->GetWidth() <<
					" Height: " << pImgData->GetHeight() << " FrameID: "<< pImgData->GetFrameID() <<
					">" << endl;
			}
			else
			{
				cout << "<Abnormal Acquisition: Exception code: " << pImgData->GetStatus() << ">" << endl;
			}
			//将采集图像buffer还回到采集系统
			pThreadParam->pStream->QBuf(pImgData);
		}
		catch (CGalaxyException &e)
		{
			cout << "<" << e.GetErrorCode() << ">" << "<" << e.what() << ">" << endl;
		}
		catch (std::exception &e)
		{
			cout << "<" << e.what() << ">" << endl;
		}
		
	}

	cout << "<Acquisition thread Exit!>" << endl;
	
	return 0;
}

int main(int argc, char* argv[])
{
	try
	{
		//初始化设备库
		IGXFactory::GetInstance().Init();

		//枚举相机设备
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

		//判断当前设备连接个数
		if (vectorDeviceInfo.size() <= 0)
		{
			cout << "No device!" << endl;
			return 0;
		}

		//通过SN打开相机设备
		CGXDevicePointer pDevice = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
		//获取相机属性控制对象
		CGXFeatureControlPointer pRemoteControl = pDevice->GetRemoteFeatureControl();
		//流层对象
		CGXStreamPointer pStream;
		if (pDevice->GetStreamCount() > 0)
		{
			pStream = pDevice->OpenStream(0);
		}
		else
		{
			cout << "Not find stream!";
			return 0;
		}
		
		//选择默认参数组
		pRemoteControl->GetEnumFeature("UserSetSelector")->SetValue("Default");
		//加载参数组
		pRemoteControl->GetCommandFeature("UserSetLoad")->Execute();

		cout << "***********************************************" << endl;
		cout << "<Vendor Name:   " << pDevice->GetDeviceInfo().GetVendorName() << ">" << endl;
		cout << "<Model Name:    " << pDevice->GetDeviceInfo().GetModelName() << ">" << endl;
		cout << "<Serial Number: " << pDevice->GetDeviceInfo().GetSN() << ">" << endl;
		cout << "***********************************************" << endl << endl;

		cout << "Press [a] or [A] and then press [Enter] to start acquisition" << endl;
		cout << "Press [x] or [X] and then press [Enter] to Exit the Program" << endl << endl;

		int nStartKey = 0;
		bool bWaitStart = true;
		bool bGrab = false;
		while (bWaitStart)
		{
			nStartKey = getchar();
			switch (nStartKey)
			{
				//press 'a' and [Enter] to start acquisition
				//press 'x' and [Enter] to exit
			case 'a':
			case 'A':
				//Start to acquisition
				bWaitStart = false;
				bGrab = true;
				break;
			case 'x':
			case 'X':
				//App exit
				cout << "<App exit!>" << endl;
				return 0;
			default:
				break;
			}
		}

		//开启流层采集
		pStream->StartGrab();
		//开启相机采集
		pRemoteControl->GetCommandFeature("AcquisitionStart")->Execute();

		THREAD_PARAM threadParam;
		threadParam.bRun = true;
		threadParam.pStream = pStream;

		// 创建采集线程
		HANDLE hThread = CreateThread(NULL, 0, GrabImgThread, &threadParam, 0, NULL);
		if (hThread == NULL) 
		{
			cerr << "Failed to create thread." << std::endl;
			return 0;
		}

		//判断退出
		bWaitStart = true;
		while (bWaitStart)
		{
			int nKey = getchar();
			//press 'x' and [Enter] for exit
			switch (nKey)
			{
			case 'X':
			case 'x':
				bWaitStart = false;
				break;
			default:
				break;
			}
		}

		//等待采集线程退出
		threadParam.bRun = false;
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);

		if (bGrab)
		{
			//相机停止采集
			pRemoteControl->GetCommandFeature("AcquisitionStart")->Execute();
			//流层停止采集
			pStream->StopGrab();
		}

		//关闭流
		pStream->Close();
		//关闭相机设备
		pDevice->Close();
		//关闭设备库
		IGXFactory::GetInstance().Uninit();
	}
	catch (CGalaxyException &e)
	{
		cout << "<" << e.GetErrorCode() << ">" << "<" << e.what() << ">" << endl;
	}
	catch (std::exception &e)
	{
		cout << "<" << e.what() << ">" << endl;
	}

	cout << "<App exit!>" << endl;

	return 0;
}