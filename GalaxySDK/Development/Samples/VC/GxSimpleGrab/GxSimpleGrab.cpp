#include "StdAfx.h"
#include "GxIAPI.h"
#include <iostream>
#include <string>

using namespace std;

void GetErrorString(GX_STATUS emErrorStatus);

//��ӡ������Ϣ�����ر��豸�Ϳ�
#define GX_VERIFY_EXIT(emStatus, hDevice) \
    if (emStatus != GX_STATUS_SUCCESS)     \
    {                                      \
        GetErrorString(emStatus);          \
        GXCloseDevice(hDevice);          \
        hDevice = NULL;                  \
        GXCloseLib();                      \
        printf("<App Exit!>\n");           \
        return emStatus;                   \
    }

struct THREAD_PARAM
{
	bool bRun;
	GX_DEV_HANDLE hDevice;
};

// �̺߳���
DWORD WINAPI GrabImgThread(LPVOID lpParam) 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	THREAD_PARAM* pThreadParam = (THREAD_PARAM*)lpParam;
	if (NULL == pThreadParam)
	{
		return 0;
	}

	while (pThreadParam->bRun)
	{
		//�㿽���ɼ�һ֡ͼ��
		PGX_FRAME_BUFFER pFrameBuffer = NULL;
		emStatus = GXDQBuf(pThreadParam->hDevice, &pFrameBuffer, 1000);
		if (GX_STATUS_SUCCESS == emStatus)
		{
			if (GX_FRAME_STATUS_SUCCESS == pFrameBuffer->nStatus)
			{
				cout << "<Successful acquisition: Width: " << pFrameBuffer->nWidth <<
					" Height: " << pFrameBuffer->nHeight << " FrameID: "<< pFrameBuffer->nFrameID <<
					">" << endl;
			}
			else
			{
				cout << "<Abnormal Acquisition: Exception code: " << pFrameBuffer->nStatus << ">" << endl;
			}
		}
		else
		{
			GetErrorString(emStatus);
		}
		//���ɼ�ͼ��buffer���ص��ɼ�ϵͳ
		emStatus = GXQBuf(pThreadParam->hDevice, pFrameBuffer);
		if (GX_STATUS_SUCCESS != emStatus)
		{
			GetErrorString(emStatus);
		}
	}

	cout << "<Acquisition thread Exit!>" << endl;
	
	return 0;
}

int main(int argc, char* argv[])
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//��ʼ���豸��
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		GetErrorString(emStatus);
		return 0;
	}

	//ö������豸
	uint32_t ui32DeviceNum = 0;
	emStatus = GXUpdateAllDeviceList(&ui32DeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
	    GetErrorString(emStatus);
		GXCloseLib();
		return 0;
	}

	//�жϵ�ǰ�豸���Ӹ���
	if (ui32DeviceNum <= 0)
	{
		cout << "No device!" << endl;
		GXCloseLib();
		return 0;
	}

	//ͨ��index������豸
	GX_DEV_HANDLE hDevice = NULL;
	emStatus = GXOpenDeviceByIndex(1, &hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
	    GetErrorString(emStatus);
		GXCloseLib();
		return 0;
	}
		
	//ѡ��Ĭ�ϲ�����
	emStatus = GXSetEnumValueByString(hDevice, "UserSetSelector", "default");
	GX_VERIFY_EXIT(emStatus, hDevice);
	//���ز�����
	emStatus = GXSetCommandValue(hDevice, "UserSetLoad");
	GX_VERIFY_EXIT(emStatus, hDevice);

	cout << "***********************************************" << endl;
	GX_DEVICE_INFO stDeviceInfo;
	string strVendorName = "NULL";
	string strModelName = "NULL";
	string strSN = "NULL";
	emStatus = GXGetDeviceInfo(1, &stDeviceInfo);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		GetErrorString(emStatus);
	}

	switch(stDeviceInfo.emDevType)
	{
	case GX_DEVICE_CLASS_USB2:
		strVendorName = (char *)&(stDeviceInfo.DevInfo.stUSBDevInfo.chVendorName[0]);
		strModelName = (char *)&(stDeviceInfo.DevInfo.stUSBDevInfo.chModelName[0]);
		strSN = (char *)&(stDeviceInfo.DevInfo.stUSBDevInfo.chSerialNumber[0]);
		break;
	case GX_DEVICE_CLASS_GEV:
		strVendorName = (char *)&(stDeviceInfo.DevInfo.stGEVDevInfo.chVendorName[0]);
		strModelName = (char *)&(stDeviceInfo.DevInfo.stGEVDevInfo.chModelName[0]);
		strSN = (char *)&(stDeviceInfo.DevInfo.stGEVDevInfo.chSerialNumber[0]);
		break;
	case GX_DEVICE_CLASS_U3V:
		strVendorName = (char *)&(stDeviceInfo.DevInfo.stU3VDevInfo.chVendorName[0]);
		strModelName = (char *)&(stDeviceInfo.DevInfo.stU3VDevInfo.chModelName[0]);
		strSN = (char *)&(stDeviceInfo.DevInfo.stU3VDevInfo.chSerialNumber[0]);
		break;
	case GX_DEVICE_CLASS_CXP:
		strVendorName = (char *)&(stDeviceInfo.DevInfo.stCXPDevInfo.chVendorName[0]);
		strModelName = (char *)&(stDeviceInfo.DevInfo.stCXPDevInfo.chModelName[0]);
		strSN = (char *)&(stDeviceInfo.DevInfo.stCXPDevInfo.chSerialNumber[0]);
		break;
	default:
		cout << "Not support device info!";
		break;
	}
	cout << "<Vendor Name:   " << strVendorName << ">" << endl;
	cout << "<Model Name:    " << strModelName << ">" << endl;
	cout << "<Serial Number: " << strSN << ">" << endl;
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

	//�����ɼ�
	emStatus = GXSetCommandValue(hDevice, "AcquisitionStart");
	GX_VERIFY_EXIT(emStatus, hDevice);;

	THREAD_PARAM threadParam;
	threadParam.bRun = true;
	threadParam.hDevice = hDevice;

	// �����ɼ��߳�
	HANDLE hThread = CreateThread(NULL, 0, GrabImgThread, &threadParam, 0, NULL);
	if (hThread == NULL) 
	{
		cerr << "Failed to create thread." << std::endl;
		GXCloseDevice(hDevice);
		GXCloseLib();
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
		//ֹͣ�ɼ�
		GXSetCommandValue(hDevice, "AcquisitionStop");
	}

	//�ر�����豸
	GXCloseDevice(hDevice);
	//�ر��豸��
	GXCloseLib();

	cout << "<App exit!>" << endl;

	return 0;
}

void GetErrorString(GX_STATUS emErrorStatus)
{
	char *error_info = NULL;
	size_t size = 0;
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ��ȡ������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, NULL, &size);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		printf("<Error when calling GXGetLastError>\n");
		return;
	}

	// ���������Ϣbuf
	error_info = new char[size];
	if (error_info == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return;
	}

	// ��ȡ������Ϣ
	emStatus = GXGetLastError(&emErrorStatus, error_info, &size);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		printf("<Error when calling GXGetLastError>\n");
	}
	else
	{
		printf("%s\n", error_info);
	}

	// �ͷŴ���buf��Դ
	if (error_info != NULL)
	{
		delete[]error_info;
		error_info = NULL;
	}
}