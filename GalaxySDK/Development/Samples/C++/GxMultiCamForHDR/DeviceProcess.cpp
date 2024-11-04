//------------------------------------------------------------
/**
\file    DeviceProcess.cpp 
\brief   Device Process 
*/
//------------------------------------------------------------

#include "StdAfx.h"
#include "DeviceProcess.h"

CDeviceProcess::CDeviceProcess(void)
{
	m_pBitmap                   = NULL;
	m_pSampleCaptureEventHandle = NULL;
	m_bIsOpen                   = false;
	m_bIsSnap                   = false;
	m_bShowDeviceFPS            = true;
	m_nDeviceIndex              = 0;
	m_strDeviceSN               = "";
	m_pSampleCaptureEventHandle = new CSampleCaptureEventHandler();
	m_pRecordDeviceSNFPS        = new char[1024];
    m_pWnd                      = NULL;
}

CDeviceProcess::~CDeviceProcess(void)
{
	if (m_pSampleCaptureEventHandle != NULL)
	{
		delete m_pSampleCaptureEventHandle;
		m_pSampleCaptureEventHandle = NULL;
	}

	if (m_pRecordDeviceSNFPS != NULL)
	{
		delete[] m_pRecordDeviceSNFPS;
		m_pRecordDeviceSNFPS = NULL;
	}
}

//------------------------------------------------------------
/**
\brief   Open Device 
\param   strDeviceSN   [in]         �豸���к�
\param   m_pWnd        [in]         ���ھ��
\param   nDeviceIndex  [in]         �豸���
\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::OpenDevice(gxstring strDeviceSN, CWnd* pWnd,int nDeviceIndex)
{
	bool bIsDeviceOpen   = false;      ///< �ж��豸�Ƿ��Ѵ򿪱�־
	bool bIsStreamOpen   = false;      ///< �ж��豸���Ƿ��Ѵ򿪱�־
    bool bIsAcqModeEnable      = false;
    bool bIsTriggerModeEnable  = false;

	try
	{
		if (m_bIsOpen || pWnd == NULL || strDeviceSN == "")
		{
			throw std::exception("��Ч����!");
		}

        m_pWnd          = pWnd;
		m_strDeviceSN   = strDeviceSN;
		m_nDeviceIndex  = nDeviceIndex;

		//���豸
		m_objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strDeviceSN,GX_ACCESS_EXCLUSIVE);
		bIsDeviceOpen = true;

		//��ȡ���Կ�����
		m_objFeatureControlPtr = m_objDevicePtr->GetRemoteFeatureControl();

        //���òɼ�ģʽ�����ɼ�
        __GetControlStatus("AcquisitionMode", bIsAcqModeEnable);
        if(bIsAcqModeEnable)
        {
            m_objFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
        }

		//���ô���ģʽΪ��
        __GetControlStatus("TriggerMode", bIsTriggerModeEnable);
        if(bIsTriggerModeEnable)
        {
		    m_objFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
        }

		//��ȡ�豸������
		int nCount = m_objDevicePtr->GetStreamCount();

		if (nCount > 0)
		{
			m_objStreamPtr = m_objDevicePtr->OpenStream(0);
			m_objStreamFeatureControlPtr = m_objStreamPtr->GetFeatureControl();
			bIsStreamOpen = true;
		}
		else
		{
			throw std::exception("δ�����豸��!");
		}

        // �����û��ڴ��������֮�󣬸��ݵ�ǰ���绷�������������ͨ������ֵ��
        // �������������Ĳɼ�����,���÷����ο����´��롣
        GX_DEVICE_CLASS_LIST objDeviceClass = m_objDevicePtr->GetDeviceInfo().GetDeviceClass();
        if(GX_DEVICE_CLASS_GEV == objDeviceClass)
        {
            // �ж��豸�Ƿ�֧����ͨ�����ݰ�����
            if(true == m_objFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
            {
                // ��ȡ��ǰ���绷�������Ű���ֵ
                int nPacketSize = m_objStreamPtr->GetOptimalPacketSize();
                // �����Ű���ֵ����Ϊ��ǰ�豸����ͨ������ֵ
                m_objFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
            } 
        }

		m_bIsOpen = true;

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}
		m_pBitmap = new CGXBitmap(m_objDevicePtr,pWnd);
	}
	catch (CGalaxyException& e)
	{

		//�ж��豸���Ƿ��Ѵ�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//�ж��豸�Ƿ��Ѵ�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;

	}
	catch (std::exception& e)
	{
		//�ж��豸���Ƿ��Ѵ�
		if (bIsStreamOpen)
		{
			m_objStreamPtr->Close();
		} 

		//�ж��豸�Ƿ��Ѵ�
		if (bIsDeviceOpen)
		{
			m_objDevicePtr->Close();
		}

		if (m_pBitmap != NULL)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
		}

		m_bIsOpen = false;

		throw e;
	}

}
//------------------------------------------------------------
/**
\brief   Close Device 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::CloseDevice()
{
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
		//�ж��Ƿ�ֹͣ�ɼ�
		if (m_bIsSnap)
		{

			//����ͣ������
			m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//�ر�����ɼ�
			m_objStreamPtr->StopGrab();

			//ע���ص�����
			m_objStreamPtr->UnregisterCaptureCallback();

		}
	}
	catch (CGalaxyException)
	{

	}

	try
	{
		//�ر�������
		m_objStreamPtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	try
	{
		//�ر��豸
		m_objDevicePtr->Close();
	}
	catch (CGalaxyException)
	{
		//do noting
	}

	m_bIsSnap = false;
	m_bIsOpen = false;

	// �ͷ�Ϊͼ����ʾ׼������Դ

	if (m_pBitmap != NULL)
	{
		m_pBitmap->UnPrepareForShowImg();
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

}
//------------------------------------------------------------
/**
\brief   Start Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StartSnap()
{
	//�ж��豸�Ƿ��Ѵ�
	if (!m_bIsOpen)
	{
		return;
	}

	try
	{
        try
        {
            //����Buffer����ģʽ
            m_objStreamFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
        }
        catch (...)
        {
        }
		
		//ע��ص�����
		m_objStreamPtr->RegisterCaptureCallback(m_pSampleCaptureEventHandle,this);
	}
	catch (CGalaxyException& e)
	{
		throw e;
	}

	try
	{

		//��������ɼ�
		m_objStreamPtr->StartGrab();
	}
	catch (CGalaxyException& e)
	{
		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();
		throw e;
	}

	try
	{
		// Ϊͼ����ʾ׼����Դ
		m_pBitmap->PrepareForShowImg();
		//���Ϳ�������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		m_bIsSnap = true;
	}
	catch (CGalaxyException& e)
	{
		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();

		// �ͷ�Ϊͼ����ʾ׼������Դ
		m_pBitmap->UnPrepareForShowImg();
		throw e;
	}
	catch (std::exception& e)
	{
		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();

		// �ͷ�Ϊͼ����ʾ׼������Դ
		m_pBitmap->UnPrepareForShowImg();
		throw e;
	}

}

//------------------------------------------------------------
/**
\brief   Stop Snap 

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::StopSnap()
{
	//�ж��豸�Ƿ��Ѵ�
	if (!m_bIsOpen || !m_bIsSnap)
	{
		return;
	}

	try
	{
		//����ͣ������
		m_objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

		//�ر�����ɼ�
		m_objStreamPtr->StopGrab();

		//ע���ص�����
		m_objStreamPtr->UnregisterCaptureCallback();

		//֡����0
		m_objfps.Reset();

		m_bIsSnap = false;
	}
	catch (CGalaxyException& e)
	{
		throw e;

	}
	catch (std::exception& e)
	{
		throw e;

	}
}

//------------------------------------------------------------
/**
\brief   Get Device Open Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsOpen() const
{
	return m_bIsOpen;
}

//------------------------------------------------------------
/**
\brief   Get Device Snap Flag 

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::IsSnap() const
{
	return m_bIsSnap;
}

//------------------------------------------------------------
/**
\brief   Set Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::SetShowFPS(bool bIsShowFps)
{
	m_bShowDeviceFPS = bIsShowFps;
}

//------------------------------------------------------------
/**
\brief   Get Show frame Frequency Flag

\return  void
*/
//------------------------------------------------------------
bool CDeviceProcess::GetShowFPS()
{
	return m_bShowDeviceFPS;
}


//------------------------------------------------------------
/**
\brief   Refresh Device SN and FPS

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::RefreshDeviceSNFPS()
{
	double dFps = 0;        ///< ֡��

	//����֡��
	m_objfps.UpdateFps();

	//��ȡ���һ�ε�֡��
	dFps = m_objfps.GetFps();

	sprintf_s(m_pRecordDeviceSNFPS, 1024,
		"���:%d SN:%s FPS:%.2f",
		m_nDeviceIndex,
		m_strDeviceSN.c_str(),
		dFps);
}

//------------------------------------------------------------
/**
\brief   Show Image
\param   objImageDataPointer [in]       ͼ����Ϣ

\return  void
*/
//------------------------------------------------------------
void CDeviceProcess::__ShowPicture(CImageDataPointer& objImageDataPointer)
{
	if (m_bShowDeviceFPS)
	{
		//����֡��
		m_objfps.IncreaseFrameNum();

		//��ʾͼ��֡��
		m_pBitmap->Show(objImageDataPointer,m_pRecordDeviceSNFPS);
	}
	else
	{
		//��ʾͼ��
		m_pBitmap->Show(objImageDataPointer);
	}
}

//------------------------------------------------------------
/**
\brief   ����ͼ����Ϣ
\param   pWnd   [in]       ��ǰ�Ӵ��ھ��

\return
*/
//------------------------------------------------------------
void CDeviceProcess::UpdataImgInfo(void)
{
    try
    {
        if (m_pBitmap != NULL)
        {
            delete m_pBitmap;
            m_pBitmap = NULL;
        }
        m_pBitmap = new CGXBitmap(m_objDevicePtr, m_pWnd);
    }
    catch (CGalaxyException& e)
    {
        throw e;
    }
    catch (std::exception& e)
    {
        throw e;
    }
}

//------------------------------------------------------------
/**
\brief   ��ȡ���Խڵ��Ƿ�֧���ҿ�д
\param   strName    [in]        ������
\param   bIsEnable  [out]       �Ƿ�ʹ��

\return
*/
//------------------------------------------------------------
void CDeviceProcess::__GetControlStatus(const gxstring &strName, bool &bIsEnable)
{
    bool    bIsImplement    = false;
    bool    bIsWritable     = false;
    bool    bIsReadable     = false;

    // �Ƿ�֧�ָýڵ�
    bIsImplement = m_objFeatureControlPtr->IsImplemented(strName);
    if(!bIsImplement)
    {
        bIsEnable = false;
        return;
    }
    else
    {
        // �жϸýڵ��Ƿ�ɶ�
        bIsReadable = m_objFeatureControlPtr->IsReadable(strName);
        if(!bIsReadable)
        {
            bIsEnable = true;
            return;
        }

        // �жϸýڵ��Ƿ��д
        bIsWritable = m_objFeatureControlPtr->IsWritable(strName);

        // ֧���ҿ�д
        if(bIsWritable)
        {
            bIsEnable = true;
        }
        else
        {
            bIsEnable = false;
        }
    }
}

//------------------------------------------------------------
/**
\brief   ��ȡ�豸�ͺ���Ϣ
\param   strDisplayName [out]       �豸DisplayName

\return
*/
//------------------------------------------------------------
void CDeviceProcess::GetDevDisplayName(CString &strDisplayName)
{
    gxstring strDeviceModelName = m_objFeatureControlPtr->GetStringFeature("DeviceModelName")->GetValue();
    strDisplayName.Format("���:%d %s(%s)", m_nDeviceIndex, strDeviceModelName.c_str(), m_strDeviceSN.c_str());
}
