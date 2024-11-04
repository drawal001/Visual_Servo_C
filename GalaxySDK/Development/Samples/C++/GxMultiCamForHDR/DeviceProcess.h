//------------------------------------------------------------
/**
\file    DeviceProcess.h 
\brief   Device Process 
*/
//------------------------------------------------------------
#pragma once
#include "GalaxyIncludes.h"
#include "GXBitmap.h"
#include "Fps.h"

#define  DEVICE_CONTS   (4)     ///< �������ͬʱ����4̨�豸

//---------------------------------------------------------------------------------
/**
\brief   �豸������
*/
//----------------------------------------------------------------------------------
class CDeviceProcess
{
public:

	CDeviceProcess(void);
	~CDeviceProcess(void);

	//---------------------------------------------------------------------------------
	/**
	\brief   �û��̳вɼ��¼�������
	*/
	//----------------------------------------------------------------------------------
	class CSampleCaptureEventHandler :public ICaptureEventHandler
	{
		//---------------------------------------------------------------------------------
		/**
		\brief   �ɼ��ص�����
		\param   objImageDataPointer      ͼ�������
		\param   pFrame                   �û�����

		\return  ��
		*/
		//----------------------------------------------------------------------------------
		void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
		{
			try
			{
				CDeviceProcess* pDeviceProcess = (CDeviceProcess*)pUserParam;

				//��ʾͼ��
				pDeviceProcess->__ShowPicture(objImageDataPointer);
			}
			catch (CGalaxyException)
			{
				//do nothing

			}
			catch (std::exception)
			{
				//do nothing
			}
		}
	};

public:
	/// ���豸
	void OpenDevice(gxstring strDeviceSN, CWnd* m_pWnd,int nDeviceIndex);

	/// �ر��豸
	void CloseDevice();

	/// ��ʼ�ɼ�
	void StartSnap();

	/// ֹͣ�ɼ�
	void StopSnap();

	/// ��ȡ�豸�򿪱�ʶ
	bool IsOpen() const;

	/// ��ȡ�豸�ɼ���ʶ
	bool IsSnap() const;

	/// �����Ƿ���ʾ֡�ʱ�־
	void SetShowFPS(bool bIsShowFps);

	/// ��ȡ�Ƿ���ʾ֡�ʱ�־
	bool GetShowFPS();

	/// ˢ���豸�ͺż�֡����Ϣ
	void RefreshDeviceSNFPS();

    /// ��ȡ�豸�ͺ���Ϣ
    void GetDevDisplayName(CString &strDisplayName);

    ///�����������
    void UpdataImgInfo(void);

	CGXFeatureControlPointer    m_objFeatureControlPtr;           ///< ���Կ�����
	CGXFeatureControlPointer    m_objStreamFeatureControlPtr;     ///< �������������

private:

	/// ��ʾͼ��
	void __ShowPicture(CImageDataPointer& objImageDataPointer);

    /// ��ȡ�ؼ�ʹ��״̬
    void __GetControlStatus(const gxstring &strName, bool &bIsEnable);

private:
	CGXDevicePointer            m_objDevicePtr;                  ///< �豸���
	CGXStreamPointer            m_objStreamPtr;                  ///< �豸������
	CGXBitmap*                  m_pBitmap;                       ///< ��ͼָ��
	CSampleCaptureEventHandler* m_pSampleCaptureEventHandle;     ///< �ص�ָ��
	bool                        m_bIsOpen;                       ///< �豸�Ƿ�򿪱�־
	bool                        m_bIsSnap;                       ///< �豸�Ƿ񿪲ɱ�־
	bool                        m_bShowDeviceFPS;                ///< ��ʾ�豸֡�ʱ�־
	char                        *m_pRecordDeviceSNFPS;           ///< �豸�ͺż�֡����Ϣ
	gxstring                    m_strDeviceSN;                   ///< �豸���к�
	CFps                        m_objfps;                        ///< ����֡��
	int							m_nDeviceIndex;                  ///< �豸���
    CWnd*                       m_pWnd;                          ///< �Ӵ��ھ��
};
