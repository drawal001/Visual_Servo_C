#include <iostream>
#include "stdafx.h"
#include "GalaxyIncludes.h"

using namespace std;

struct THREAD_PARAM
{
	bool bRun;
	CGXStreamPointer pStream;
};

// �̺߳���
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
			//�㿽���ɼ�һ֡ͼ��
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
			//���ɼ�ͼ��buffer���ص��ɼ�ϵͳ
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
		//��ʼ���豸��
		IGXFactory::GetInstance().Init();

		//ö������豸
		GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
		IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

		//�жϵ�ǰ�豸���Ӹ���
		if (vectorDeviceInfo.size() <= 0)
		{
			cout << "No device!" << endl;
			return 0;
		}

		//ͨ��SN������豸
		CGXDevicePointer pDevice = IGXFactory::GetInstance().OpenDeviceBySN(vectorDeviceInfo[0].GetSN(), GX_ACCESS_EXCLUSIVE);
		//��ȡ������Կ��ƶ���
		CGXFeatureControlPointer pRemoteControl = pDevice->GetRemoteFeatureControl();
		//�������
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
		
		//ѡ��Ĭ�ϲ�����
		pRemoteControl->GetEnumFeature("UserSetSelector")->SetValue("Default");
		//���ز�����
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

		//��������ɼ�
		pStream->StartGrab();
		//��������ɼ�
		pRemoteControl->GetCommandFeature("AcquisitionStart")->Execute();

		THREAD_PARAM threadParam;
		threadParam.bRun = true;
		threadParam.pStream = pStream;

		// �����ɼ��߳�
		HANDLE hThread = CreateThread(NULL, 0, GrabImgThread, &threadParam, 0, NULL);
		if (hThread == NULL) 
		{
			cerr << "Failed to create thread." << std::endl;
			return 0;
		}

		//�ж��˳�
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

		//�ȴ��ɼ��߳��˳�
		threadParam.bRun = false;
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);

		if (bGrab)
		{
			//���ֹͣ�ɼ�
			pRemoteControl->GetCommandFeature("AcquisitionStart")->Execute();
			//����ֹͣ�ɼ�
			pStream->StopGrab();
		}

		//�ر���
		pStream->Close();
		//�ر�����豸
		pDevice->Close();
		//�ر��豸��
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