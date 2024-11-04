//----------------------------------------------------------------------------------
/**
\file    GxGigeRecovery.cpp 
\brief   Gige recovery function
\version v1.0.2404.9281
\date    2024-4-28
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "GxIAPI.h"
#include <conio.h>
#include <iostream>
#include <string>
using namespace std;
#include <TCHAR.H>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();
//----------------------------------------------------------------------------------
/** 
\brief   �豸������
*/
//----------------------------------------------------------------------------------
class CGigeRecovery
{
public:
	CGigeRecovery();
    virtual ~CGigeRecovery();

	/// GigeRecovery������������ӿں���
	void GigeRecovery();

private:

	/// ���豸
	bool __OnOpenDevice();

	/// ��ʼ���������
	bool __InitParam();
	
	/// ��ʼ�ɼ�
	bool __OnStartSnap();
	
	/// ֹͣ�ɼ�
	bool __OnStopSnap();

	/// �ر��豸
	bool __OnCloseDevice();
	
	/// �����ɼ�
	void __Acquisition();

	/// �豸���ߴ���
	void __ProcessOffline();

	/// �豸����
	void __Recovery();

	/// ��ӡ������Ϣ����
	void __GetErrorString(GX_STATUS emErrorStatus);

	/// ���߻ص�����
	static void __stdcall __OnDeviceOfflineCallbackFun(void* pUserParam);

private:
	GX_DEV_HANDLE             m_hDevice;                              ///< �豸���
	GX_DS_HANDLE			  m_hStream;				              ///< �豸�����
	GX_EVENT_CALLBACK_HANDLE  m_hCB;                                  ///< ���߻ص����
	string                    m_strPath;                              ///< �ļ��ĵ�ǰ·��
	string                    m_strSavePath;                          ///< ������ò����ļ�����·��
	char                      m_chModulePath[100];                    ///< ��ȡ�ĵ�ǰ����·�� 
	char                      m_chMACAddress[GX_INFO_LENGTH_32_BYTE]; ///< ���MAC��ַ
	GX_FRAME_DATA             m_stFrameData;                          ///< GetImage��ȡ��ͼ��
	bool                      m_bIsOffLine;                           ///< ������߱�ʶ
	bool                      m_bIsOpen;                              ///< ����򿪱�ʶ 
	bool                      m_bIsSanp;                              ///< ����ɼ���־
};

//----------------------------------------------------------------------------------
/**
\brief ������main

\return int 
*/
//----------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// ����CGigeRecovery��ʵ�ֵ�����������
	CGigeRecovery objCtrDevice;
	objCtrDevice.GigeRecovery();

	// ����������رտ���̨
	printf("\n<press any key to exit>\n");
	while (!_kbhit());

	return 0;
}

//---------------------------------------------------------------------------------
/**
\brief   ���캯��

 \return   ��
*/
//----------------------------------------------------------------------------------
CGigeRecovery::CGigeRecovery()
{ 
	m_hDevice             = NULL;              // �豸���
	m_hStream			  = NULL;			   // �豸�����
	m_hCB                 = NULL;              // ���߻ص����
	m_stFrameData.pImgBuf = NULL;              // GetImage��ȡ��ͼ��buffer
	m_strPath             = "";                // ��ȡ�ĵ�ǰ����·��
	m_strSavePath         = "";                // �ļ��ĵ�ǰ·��
	m_bIsOffLine          = false;             // ������߱�ʶ
	m_bIsOpen             = false;             // ����򿪱�ʶ
	m_bIsSanp             = false;             // ����ɼ���ʶ

	// ��ʼ���豸��
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		exit(0);
	}

	// ��ȡ��ǰ·�������ڱ���������������ļ�
	GetModuleFileName(NULL, m_chModulePath, sizeof(m_chModulePath));
	(_tcsrchr(m_chModulePath, _T('\\')))[1] = 0;
	m_strPath = m_chModulePath;
	m_strSavePath = m_strPath + "ConfigFile.ini";	
}

//---------------------------------------------------------------------------------
/**
\brief    ��������

\return   ��
*/
//----------------------------------------------------------------------------------
CGigeRecovery::~CGigeRecovery()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ر��豸��
	emStatus = GXCloseLib();
}

//---------------------------------------------------------------------------------
/**
\brief    ���豸

\return   true�����سɹ���false������ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnOpenDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS         emStatus   = GX_STATUS_SUCCESS;   
	uint32_t          nDeviceNum = 0;        // �豸���� 
	bool              bReturn    = false;    // ���������������ֵ
    GX_DEVICE_IP_INFO stDeviceIPInfo;        // ���IP��Ϣ
	GX_DEVICE_BASE_INFO *pDeviceBaseInfo;    // ���������Ϣ
	size_t nDeviceBaseInfoSize = 0;          // GX_DEVICE_BASE_INFO�ṹ�峤��
	GX_OPEN_PARAM     stOpenParam;           // ���������
	uint32_t	  nDSNum	  = 0;

	// ö���豸����
	printf("====================CGigeRecovery::__OnOpenDevice()====================\n");
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
		
	// �жϵ�ǰ�����豸����
	if (nDeviceNum <= 0)
	{
		printf("<NO device>\n");
 		return false;
	}

	pDeviceBaseInfo = new(std::nothrow) GX_DEVICE_BASE_INFO[nDeviceNum];
	if (NULL == pDeviceBaseInfo) 
	{
		return false;
	}

	//��ȡ�豸������Ϣ
	nDeviceBaseInfoSize = sizeof(GX_DEVICE_BASE_INFO) * nDeviceNum;
	emStatus = GXGetAllDeviceBaseInfo(pDeviceBaseInfo, &nDeviceBaseInfoSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		delete[] pDeviceBaseInfo;
		pDeviceBaseInfo = NULL;
		__GetErrorString(emStatus);
		return false;
	}

	//�ж�ö�ٵ����豸�Ƿ�ΪGige
	if (pDeviceBaseInfo->deviceClass != GX_DEVICE_CLASS_GEV)
	{
		delete[] pDeviceBaseInfo;
		pDeviceBaseInfo = NULL;
		printf("<The device is not GEV>");
		return false;
	}
	delete[] pDeviceBaseInfo;
	pDeviceBaseInfo = NULL;

	//��ȡ��һ̨�豸��������Ϣ
	emStatus = GXGetDeviceIPInfo(1, &stDeviceIPInfo);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	memcpy(m_chMACAddress, stDeviceIPInfo.szMAC, GX_INFO_LENGTH_32_BYTE);
	
	// ͨ��MAC��ַ���豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_MAC;
	stOpenParam.pszContent = stDeviceIPInfo.szMAC;
	printf("<Open device by MAC: %s>\n", stDeviceIPInfo.szMAC);
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// ��ȡ��ͨ����
	emStatus = GXGetDataStreamNumFromDev(m_hDevice, &nDSNum);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	if(0 < nDSNum)
	{
		// ��ȡ�����
		emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1, &m_hStream);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
			return false;
		}
	}


	// �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
	// �������������Ĳɼ�����,���÷����ο����´��롣
	{
		bool	 bImplementPacketSize = false;
		uint32_t unPacketSize		  = 0;

		// �ж��豸�Ƿ�֧����ͨ�����ݰ�����
		GX_NODE_ACCESS_MODE emAccessMode = GX_NODE_ACCESS_MODE_NI;
		emStatus = GXGetNodeAccessMode(m_hDevice, "GevSCPSPacketSize", &emAccessMode);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
			return false;
		}

		if((emAccessMode != GX_NODE_ACCESS_MODE_NI) 
			&& (emAccessMode != GX_NODE_ACCESS_MODE_UNDEF)
			&& (emAccessMode != GX_NODE_ACCESS_MODE_NA))
		{
			bImplementPacketSize = true;
		}


		if (bImplementPacketSize)
		{
			// ��ȡ��ǰ���绷�������Ű���ֵ
			emStatus = GXGetOptimalPacketSize(m_hDevice,&unPacketSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				__GetErrorString(emStatus);
				return false;
			}

			// �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
			emStatus = GXSetIntValue(m_hDevice, "GevSCPSPacketSize", unPacketSize);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				__GetErrorString(emStatus);
				return false;
			}
		}
	}
	
    // ��ʼ������
	printf("<Initialize the device parameters>\n");
	bReturn = __InitParam();
	if (!bReturn)
	{
		return false;
	}

    // �������������ļ�
	printf("<Export config file>\n");
	emStatus = GXExportConfigFile(m_hDevice, m_strSavePath.c_str());
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// ע����߻ص�����
	printf("<Register device Offline callback>\n");
	emStatus = GXRegisterDeviceOfflineCallback(m_hDevice, this, __OnDeviceOfflineCallbackFun, &m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	m_bIsOpen = true;	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   ��ʼ������

\return   true�����سɹ���false������ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__InitParam()
{
	GX_STATUS emStatus     = GX_STATUS_SUCCESS;  
	uint32_t   ui32PayLoadSize = 0;     // ԭʼRawͼ���С
	
	// ���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnumValueByString(m_hDevice, "AcquisitionMode", "Continuous");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	// �رմ���ģʽ
	emStatus = GXSetEnumValueByString(m_hDevice, "TriggerMode", "Off");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}

	//��������Buffer����ģʽ
	emStatus = GXSetEnumValueByString(m_hStream, "StreamBufferHandlingMode", "OldestFirst");

	//����������ʱʱ��Ϊ1s
	//���ǧ���������������Debugģʽ�µ�������ʱ�������������ʱʱ���Զ�����Ϊ5min��
	//��������Ϊ�˲��������������ʱӰ�����ĵ��Ժ͵���ִ�У�ͬʱ��Ҳ��ζ���������5min���޷��Ͽ�������ʹ����ϵ����ϵ�
	//Ϊ�˽�������������⣬�������������ʱʱ������Ϊ1s�����������ߺ������������
	emStatus = GXSetIntValue(m_hDevice, "GevHeartbeatTimeout", 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	
	// ��ȡͼ���С
	emStatus = GXGetPayLoadSize(m_hStream, &ui32PayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_stFrameData.pImgBuf = new(std::nothrow) BYTE[(size_t)ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief   ���߻ص�����
\param   pUserParam   �û�����

\return  void
*/
//----------------------------------------------------------------------------------
void __stdcall CGigeRecovery::__OnDeviceOfflineCallbackFun(void* pUserParam)
{
	CGigeRecovery *pCtrDevice = (CGigeRecovery *)pUserParam;

	// �������
	pCtrDevice->m_bIsOffLine = true;
	printf("**********************Device offline**********************\n");
}

//---------------------------------------------------------------------------------
/**
\brief    ��ʼ�ɼ�

\return   true�����سɹ���false������ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStartSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;  

	printf("====================CGigeRecovery::__OnStartSnap()====================\n");

	// ���ɼ�����
	printf("<Send start snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_bIsSanp = true;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    ֹͣ�ɼ�

\return   true�����سɹ���false������ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnStopSnap() 
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;  	
	printf("====================CGigeRecovery::__OnStopSnap()====================\n");
	
	// ����ֹͣ�ɼ�����
	printf("<Send stop snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return false;
	}
	m_bIsSanp = false;
	
	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    �ر��豸

\return   true�����سɹ���false������ʧ��
*/
//----------------------------------------------------------------------------------
bool CGigeRecovery::__OnCloseDevice() 
{
	// TODO: Add your control notification handler code here
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	printf("====================CGigeRecovery::__OnCloseDevice()====================\n");

	// ע���豸�����¼��ص�
	printf("<Unregister device Offline callback>\n");
	emStatus = GXUnregisterDeviceOfflineCallback(m_hDevice, m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
	}
	
	// �ر����
	printf("<Close device>\n");
	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
	}
	m_hDevice = NULL;

	// �ͷ���Դ
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete[]m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}

	m_bIsOpen = false;

	return true;
}

//---------------------------------------------------------------------------------
/**
\brief    �����ɼ�

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Acquisition() 
{ 
	GX_STATUS emStatus = GX_STATUS_SUCCESS; 
	printf("====================CGigeRecovery::__Acquisition()====================\n");
	printf("<Press any key to stop acquisition>\n");
	
	// �ް�������
	while(!_kbhit())
	{
		if (m_bIsOffLine)         // ���ߣ����е��ߴ��������
		{
	       // ���ߴ���
		   __ProcessOffline();
		   
		   // ����
		   __Recovery();
		}
		else                    // δ���ߣ����ȡͼ��
		{
			emStatus = GXGetImage(m_hDevice, &m_stFrameData, 500);
			if (emStatus == GX_STATUS_SUCCESS)
			{
				printf("Successfully get Image\n");
			}
			else
			{
				__GetErrorString(emStatus);
			}
		}
	}

	_getch();
}

//---------------------------------------------------------------------------------
/**
\brief    ������ߴ���

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__ProcessOffline()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	printf("**********************Process Offline**********************\r");

	// ������ɣ���ֹͣ�ɼ�
	if (m_bIsSanp)
	{
		// ����ֹͣ�ɼ�����
		printf("\n<Send stop snap command to device>\n");
		emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStop");
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
		}
		m_bIsSanp = false;
	}

	// ����򿪣���ر�
	if (m_bIsOpen)
	{
		// ע���豸�����¼��ص�
	    printf("<Unregister device Offline callback>\n");
  	    emStatus = GXUnregisterDeviceOfflineCallback(m_hDevice, m_hCB);
	    if (emStatus != GX_STATUS_SUCCESS)
	    {
	       __GetErrorString(emStatus);
	    }
		
		// �ر����
		printf("<Close device>\n");
		emStatus = GXCloseDevice(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__GetErrorString(emStatus);
		}
		m_hDevice = NULL;
		
		// �ͷ���Դ
		if (m_stFrameData.pImgBuf != NULL)
		{
			delete[] m_stFrameData.pImgBuf;
			m_stFrameData.pImgBuf = NULL;
		}
		m_bIsOpen = false;
	}
}

//---------------------------------------------------------------------------------
/**
\brief    �������

\return   void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__Recovery()
{
	GX_STATUS         emStatus     = GX_STATUS_SUCCESS;   
	uint32_t          nDeviceNum   = 0;     // �豸����   
	uint32_t          ui32PayLoadSize = 0;     // ԭʼRawͼ���С
	GX_OPEN_PARAM     stOpenParam;          // ���������

	printf("**********************Recovery**********************\r");
	
	emStatus = GXUpdateAllDeviceList(&nDeviceNum, 1000);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		return;
	}

	// �жϵ�ǰ�����豸����
	if (nDeviceNum <= 0)
	{
		return;
	}

	// �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		m_hDevice = NULL;
	}
	
	// ͨ����֪��MAC��ַ���豸
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode   = GX_OPEN_MAC;
	stOpenParam.pszContent = m_chMACAddress;
	printf("\n<Open Device by MAC %s>\n", m_chMACAddress);
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}

	// ���������ļ�
	printf("<Import config file>\n");
	emStatus = GXImportConfigFile(m_hDevice, m_strSavePath.c_str());
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	
	// ��ͼ�����buffer
	if (m_stFrameData.pImgBuf != NULL)
	{
		delete[] m_stFrameData.pImgBuf;
		m_stFrameData.pImgBuf = NULL;
	}

	GX_DS_HANDLE hStream = NULL;
	emStatus = GXGetDataStreamHandleFromDev(m_hDevice, 1 ,&hStream);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	emStatus = GXGetPayLoadSize(hStream,&ui32PayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}

	m_stFrameData.pImgBuf = new(std::nothrow) BYTE[(size_t)ui32PayLoadSize];
	if (m_stFrameData.pImgBuf == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return;
	}

	// ע����߻ص�����
	printf("<Register device Offline callback>\n");
	emStatus = GXRegisterDeviceOfflineCallback(m_hDevice, this, __OnDeviceOfflineCallbackFun, &m_hCB);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	 m_bIsOpen = true;

	 // ���Ϳ�������
	printf("<Send start snap command to device>\n");
	emStatus = GXSetCommandValue(m_hDevice, "AcquisitionStart");
	if (emStatus != GX_STATUS_SUCCESS)
	{
		__GetErrorString(emStatus);
		return;
	}
	m_bIsSanp = true;
	m_bIsOffLine = false;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������Ϣ����
\param  emErrorStatus  ������

\return void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::__GetErrorString(GX_STATUS emErrorStatus)
{
	char      *pchErrorInfo = NULL;
	size_t    nSize         = 0;
	GX_STATUS emStatus      = GX_STATUS_SUCCESS;
	
	// ��ȡ����������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new(std::nothrow) char[nSize];
	if (pchErrorInfo == NULL)
	{
		printf("<Failed to allocate memory>\n");
		return ;
	}
	
	// ��ȡ������Ϣ����
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		printf("<GXGetLastError�ӿڵ���ʧ��!>\n");
	}

	printf("%s\n", (LPCTSTR)pchErrorInfo);

	// �ͷ���Դ
	if (pchErrorInfo != NULL)
	{
		delete[]pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ������������ӿں���

\return void
*/
//----------------------------------------------------------------------------------
void CGigeRecovery::GigeRecovery()
{
	bool bReturn = false;

	// ���豸
	bReturn = __OnOpenDevice();
	if (!bReturn)
	{
		return;
	}

	// ��ʼ�ɼ�
	bReturn = __OnStartSnap();
	if (!bReturn)
	{
		__OnCloseDevice();
		return;
	}

	// ѭ���ɼ�
	__Acquisition();	

	// ֹͣ�ɼ�
	__OnStopSnap();
		
	// �ر��豸
	__OnCloseDevice();
}





