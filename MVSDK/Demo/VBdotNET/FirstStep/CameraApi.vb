Imports System.Runtime.InteropServices

Public Delegate Sub CB_CameraGrabberFrameCallback( _
        ByVal Grabber As IntPtr, _
        ByVal pFrameBuffer As IntPtr, _
        ByRef pFrameHead As tSdkFrameHead, _
        ByVal Context As IntPtr)

Public Delegate Function CB_CameraGrabberFrameListener( _
        ByVal Grabber As IntPtr, _
        ByVal Phase As Integer, _
        ByVal pFrameBuffer As IntPtr, _
        ByRef pFrameHead As tSdkFrameHead, _
        ByVal Context As IntPtr) As Integer

Public Delegate Sub CB_CameraGrabberSaveImageComplete( _
            ByVal Grabber As IntPtr, _
            ByVal Image As IntPtr, _
            ByVal Status As Integer, _
            ByVal Context As IntPtr)

Public Module CameraApi
    'BIG5 TRANS ALLOWED

    Public Delegate Sub CAMERA_SNAP_PROC(ByVal hCamera As Integer, _
            ByVal pRawBuffer As IntPtr, _
            ByRef sFrameInfo As tSdkFrameHead, _
            ByVal Context As IntPtr)

    Public Function StringToBytes(ByVal Str As String) As Byte()
        Dim sTmp() As Byte
        sTmp = System.Text.Encoding.Default.GetBytes(Str)
        ReDim Preserve sTmp(UBound(sTmp) + 1)
        sTmp(UBound(sTmp)) = 0
        StringToBytes = sTmp
    End Function

    Public Function BytesToString(ByRef Data() As Byte) As String
        Dim zeroChar As Char = ChrW(0)
        Dim str As String = System.Text.Encoding.Default.GetString(Data)
        Dim index As Integer = str.IndexOf(zeroChar)
        If index > 0 Then
            str = str.Substring(0, index)
        End If
        BytesToString = str
    End Function


    '/******************************************************/
    '// ������      : CameraSdkInit
    '// ��������    : ���SDK��ʼ�����ڵ����κ�SDK�����ӿ�ǰ������
    '//                       �ȵ��øýӿڽ��г�ʼ�����ú�����������������
    '//                       �ڼ�ֻ��Ҫ����һ�Ρ�
    '// ����            : iLanguageSel ����ѡ��SDK�ڲ���ʾ��Ϣ�ͽ��������,
    '//                                                0:��ʾӢ��,1:��ʾ���ġ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSdkInit Lib "MVCAMSDK.dll" _
    ( _
        ByVal iLanguageSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraEnumerateDevice
    '// ��������    : ö���豸���������豸�б��ڵ���CameraInit
    '//                       ֮ǰ��������øú���������豸����Ϣ��
    '// ����       : pDSCameraList �豸�б�����ָ�롣
    '//              piNums        �豸�ĸ���ָ�룬����ʱ����pDSCameraList
    '//                            �����Ԫ�ظ�������������ʱ������ʵ���ҵ����豸������
    '//              ע�⣬piNumsָ���ֵ�����ʼ�����Ҳ�����pDSCameraList����Ԫ�ظ�����
    '//              �����п�������ڴ������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function _CameraEnumerateDevice Lib "MVCAMSDK.dll" Alias "CameraEnumerateDevice" _
    ( _
            ByVal pDSCameraList As IntPtr, _
            ByRef piNums As Integer _
    ) As Integer

    Public Function CameraEnumerateDevice(ByRef CameraList() As tSdkCameraDevInfo) As Integer
        Dim CameraCount As Integer = 256
        Dim status As Integer
        Dim ptr As IntPtr
        Dim SizeOfDevInfo As Integer = Marshal.SizeOf(New tSdkCameraDevInfo())

        ReDim CameraList(-1)
        ptr = Marshal.AllocHGlobal(SizeOfDevInfo * CameraCount)
        status = _CameraEnumerateDevice(ptr, CameraCount)
        If status = 0 And CameraCount > 0 Then
            ReDim CameraList(CameraCount - 1)
            For i As Integer = 0 To CameraCount - 1 Step 1
                CameraList(i) = Marshal.PtrToStructure(CType((CType(ptr, Int64) + i * SizeOfDevInfo), IntPtr), New tSdkCameraDevInfo().GetType)
            Next
        End If

        Marshal.FreeHGlobal(ptr)
        CameraEnumerateDevice = status
    End Function


    '/******************************************************/
    '// ������  : CameraInit
    '// ��������    : �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
    '//           �����صĲ����ӿڡ�
    '// ����        : pCameraInfo    ��������豸������Ϣ����CameraEnumerateDevice
    '//                          ������á�
    '//            iParamLoadMode  �����ʼ��ʱʹ�õĲ������ط�ʽ��-1��ʾʹ���ϴ��˳�ʱ�Ĳ������ط�ʽ��
    '//            emTeam         ��ʼ��ʱʹ�õĲ����顣-1��ʾ�����ϴ��˳�ʱ�Ĳ����顣
    '//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
    '//                          ���ظ��������Ч������ڵ����������
    '//                          ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
    '//                          ���ڶ����֮������֡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraInit Lib "MVCAMSDK.dll" _
    ( _
            ByRef pCameraInfo As tSdkCameraDevInfo, _
            ByVal iParamLoadMode As Integer, _
            ByVal emTeam As Integer, _
            ByRef pCameraHandle As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetCallbackFunction
    '// ��������    : ����ͼ�񲶻�Ļص��������������µ�ͼ������֡ʱ��
    '//                       pCallBack��ָ��Ļص������ͻᱻ���á�
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            pCallBack �ص�����ָ�롣
    '//            pContext  �ص������ĸ��Ӳ������ڻص�����������ʱ
    '//                      �ø��Ӳ����ᱻ���룬����ΪNULL��������
    '//                      ������ʱЯ��������Ϣ��
    '//            pCallbackOld  ���ڱ��浱ǰ�Ļص�����������ΪNULL��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetCallbackFunction Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal pCallBack As CAMERA_SNAP_PROC, _
            ByVal pContext As IntPtr, _
            ByVal OldCallBack As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraUnInit
    '// ��������    : �������ʼ�����ͷ���Դ��
    '// ����            : hCamera ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraUnInit Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetInformation
    '// ��������    : ��������������Ϣ
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            pbuffer ָ�����������Ϣָ���ָ�롣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetInformation Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef sDescribe As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImageProcess
    '// ��������    : ����õ����ԭʼ���ͼ�����ݽ��д������ӱ��Ͷȡ�
    '//                       ��ɫ�����У��������ȴ���Ч�������õ�RGB888
    '//                       ��ʽ��ͼ�����ݡ�
    '// ����            : hCamera  ����ľ������CameraInit������á�
    '//            pbyIn       ����ͼ�����ݵĻ�������ַ������ΪNULL��
    '//            pbyOut   �����ͼ������Ļ�������ַ������ΪNULL��
    '//            pFrInfo  ����ͼ���֡ͷ��Ϣ��������ɺ�֡ͷ��Ϣ
    '//                                        �е�ͼ���ʽuiMediaType����֮�ı䡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImageProcess Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal pbyIn As IntPtr, _
            ByVal pbyOut As IntPtr, _
            ByRef pFrInfo As tSdkFrameHead _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraDisplayInit
    '// ��������    : ��ʼ��SDK�ڲ�����ʾģ�顣�ڵ���CameraDisplayRGB24
    '//                       ǰ�����ȵ��øú�����ʼ����������ڶ��ο����У�
    '//                       ʹ���Լ��ķ�ʽ����ͼ����ʾ(������CameraDisplayRGB24)��
    '//                       ����Ҫ���ñ�������
    '// ����            : hCamera           ����ľ������CameraInit������á�
    '//            hWndDisplay      ��ʾ���ڵľ����һ��Ϊ���ڵ�m_hWnd��Ա��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraDisplayInit Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal hWndDisplay As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraDisplayRGB24
    '// ��������    : ��ʾͼ�񡣱�����ù�CameraDisplayInit����
    '//                       ��ʼ�����ܵ��ñ�������
    '// ����            : hCamera  ����ľ������CameraInit������á�
    '//            pbyRGB24 ͼ������ݻ�������RGB888��ʽ��
    '//            pFrInfo  ͼ���֡ͷ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraDisplayRGB24 Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal pbyRGB24 As IntPtr, _
            ByRef pFrInfo As tSdkFrameHead _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetDisplaySize
    '// ��������    : ������ʾ�ؼ��ĳߴ硣������ù�
    '//                       CameraDisplayInit���г�ʼ�����ܵ��ñ�������
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            iWidth    ���
    '//            iHeight   �߶�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetDisplaySize Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iWidth As Integer, _
            ByVal iHeight As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetImageBuffer
    '// ��������    : ���һ֡ͼ�����ݡ�Ϊ�����Ч�ʣ�SDK��ͼ��ץȡʱ�������㿽�����ƣ�
    '//                       CameraGetImageBufferʵ�ʻ�����ں��е�һ����������ַ��
    '//                       �ú����ɹ����ú󣬱������CameraReleaseImageBuffer�ͷ���
    '//                       CameraGetImageBuffer�õ��Ļ�����,�Ա����ں˼���ʹ��
    '//                       �û�������
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            pFrameInfo  ͼ���֡ͷ��Ϣָ�롣
    '//            pbyBuffer   ָ��ͼ������ݵĻ�����ָ�롣����
    '//                                               �������㿽�����������Ч�ʣ����
    '//                                               ����ʹ����һ��ָ��ָ���ָ�롣
    '//            ByVal wTimes ץȡͼ��ĳ�ʱʱ�䡣��λ���롣��
    '//                                               wTimesʱ���ڻ�δ���ͼ����ú���
    '//                                               �᷵�س�ʱ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetImageBuffer Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pFrameInfo As tSdkFrameHead, _
        ByRef pbyBuffer As IntPtr, _
        ByVal wTimes As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSnapToBuffer
    '// ��������    : ץ��һ��ͼ�񵽻������С���������ץ��ģʽ������
    '//                       �Զ��л���ץ��ģʽ�ķֱ��ʽ���ͼ�񲶻�Ȼ��
    '//                       ���񵽵����ݱ��浽�������С�
    '//                       �ú����ɹ����ú󣬱������CameraReleaseImageBuffer
    '//                       �ͷ���CameraSnapToBuffer�õ��Ļ�������������ο�
    '//                       CameraGetImageBuffer�����Ĺ����������֡�
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            pFrameInfo  ָ�룬����ͼ���֡ͷ��Ϣ��
    '//            pbyBuffer   ָ��ָ���ָ�룬��������ͼ�񻺳����ĵ�ַ��
    '//            uWaitTimeMs ��ʱʱ�䣬��λ���롣�ڸ�ʱ���ڣ������Ȼû��
    '//                                               �ɹ���������ݣ��򷵻س�ʱ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSnapToBuffer Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pFrameInfo As tSdkFrameHead, _
        ByRef pbyBuffer As IntPtr, _
        ByVal uWaitTimeMs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraReleaseImageBuffer
    '// ��������    : �ͷ���CameraGetImageBuffer��õĻ�������
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            pbyBuffer          ��CameraGetImageBuffer��õĻ�������ַ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraReleaseImageBuffer Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal pbyBuffer As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraPlay
    '// ��������    : ��SDK���빤��ģʽ����ʼ��������������͵�ͼ��
    '//                       ���ݡ������ǰ����Ǵ���ģʽ������Ҫ���յ�
    '//                       ����֡�Ժ�Ż����ͼ��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraPlay Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraPause
    '// ��������    : ��SDK������ͣģʽ�����������������ͼ�����ݣ�
    '//                       ͬʱҲ�ᷢ�������������ͣ������ͷŴ������
    '//                       ��ͣģʽ�£����Զ�����Ĳ����������ã���������Ч��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraPause Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraStop
    '// ��������    : ��SDK����ֹͣ״̬��һ���Ƿ���ʼ��ʱ���øú�����
    '//                       �ú��������ã������ٶ�����Ĳ����������á�
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraStop Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraInitRecord
    '// ��������    : ��ʼ��һ��¼��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            iFormat    ¼��ĸ�ʽ����ǰֻ֧�ֲ�ѹ����MSCV���ַ�ʽ��
    '//                                               0:��ѹ����1:MSCV��ʽѹ����
    '//            pcSavePath  ¼���ļ������·����
    '//            b2GLimit   ���ΪTRUE,���ļ�����2Gʱ�Զ��ָ
    '//            dwQuality   ¼����������ӣ�Խ��������Խ�á���Χ1��100.
    '//            iFrameRate  ¼���֡�ʡ������趨�ı�ʵ�ʲɼ�֡�ʴ�
    '//                                               �����Ͳ���©֡��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraInitRecord Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iFormat As Integer, _
        ByRef pcSavePath As Byte, _
        ByVal b2GLimit As Integer, _
        ByVal dwQuality As Integer, _
        ByVal iFrameRate As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraStopRecord
    '// ��������    : ��������¼�񡣵�CameraInitRecord�󣬿���ͨ���ú���
    '//                       ������һ��¼�񣬲�����ļ����������
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraStopRecord Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraPushFrame
    '// ��������    : ��һ֡���ݴ���¼�����С��������CameraInitRecord
    '//                       ���ܵ��øú�����CameraStopRecord���ú󣬲����ٵ���
    '//                       �ú������������ǵ�֡ͷ��Ϣ��Я����ͼ��ɼ���ʱ���
    '//                       ��Ϣ�����¼����Ծ�׼��ʱ��ͬ����������֡�ʲ��ȶ�
    '//                       ��Ӱ�졣
    '// ����            : hCamera                   ����ľ������CameraInit������á�
    '//            pbyImageBuffer    ͼ������ݻ�������������RGB��ʽ��
    '//            pFrInfo           ͼ���֡ͷ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraPushFrame Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal pbyImageBuffer As IntPtr, _
        ByRef pFrInfo As tSdkFrameHead _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSaveImage
    '// ��������    : ��ͼ�񻺳��������ݱ����ͼƬ�ļ���
    '// ����            : hCamera            ����ľ������CameraInit������á�
    '//            lpszFileName   ͼƬ�����ļ�����·����
    '//            pbyImageBuffer ͼ������ݻ�������
    '//            pFrInfo        ͼ���֡ͷ��Ϣ��
    '//            byFileType     ͼ�񱣴�ĸ�ʽ��ȡֵ��Χ�μ�CameraDefine.bas
    '//                                                      ��emSdkFileType�����Ͷ��塣Ŀǰ֧��
    '//                                                      BMP��JPG��PNG��RAW���ָ�ʽ������RAW��ʾ
    '//                                                      ��������ԭʼ���ݣ�����RAW��ʽ�ļ�Ҫ��
    '//                                                      pbyImageBuffer��pFrInfo����CameraGetImageBuffer
    '//                                                      ��õ����ݣ�����δ��CameraImageProcessת��
    '//                                                      ��BMP��ʽ����֮�����Ҫ�����BMP��JPG����
    '//                                                      PNG��ʽ����pbyImageBuffer��pFrInfo����
    '//                                                      CameraImageProcess������RGB��ʽ���ݡ�
    '//                                                  �����÷����Բο�Advanced�����̡�
    '//            byQuality      ͼ�񱣴���������ӣ���������ΪJPG��ʽ
    '//                                                  ʱ�ò�����Ч����Χ1��100�������ʽ
    '//                           ����д��0��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSaveImage Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef lpszFileName As Byte, _
        ByVal pbyImageBuffer As IntPtr, _
        ByRef pFrInfo As tSdkFrameHead, _
        ByVal byFileType As Byte, _
        ByVal byQuality As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetImageResolution
    '// ��������    : ��õ�ǰԤ���ķֱ��ʡ�
    '// ����            : hCamera            ����ľ������CameraInit������á�
    '//            psCurVideoSize �ṹ��ָ�룬���ڷ��ص�ǰ�ķֱ��ʡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetImageResolution Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef psCurVideoSize As tSdkImageResolution _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetImageResolution
    '// ��������    : ����Ԥ���ķֱ��ʡ�
    '// ����            : hCamera              ����ľ������CameraInit������á�
    '//            pImageResolution �ṹ��ָ�룬���ڷ��ص�ǰ�ķֱ��ʡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetImageResolution Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pImageResolution As tSdkImageResolution _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetMediaType
    '// ��������    : ��������ǰ���ԭʼ���ݵĸ�ʽ�����š�
    '// ����            : hCamera           ����ľ������CameraInit������á�
    '//            piMediaType   ָ�룬���ڷ��ص�ǰ��ʽ���͵������š�
    '//                                                     ��CameraGetCapability�����������ԣ�
    '//                                                     ��tSdkCameraCapbility�ṹ���е�pMediaTypeDesc
    '//                                                     ��Ա�У����������ʽ���������֧�ֵĸ�ʽ��
    '//                                                     piMediaType��ָ��������ţ����Ǹ�����������š�
    '//                                                     pMediaTypeDesc[*piMediaType].iMediaType���ʾ��ǰ��ʽ��
    '//                                                     ���롣�ñ�����μ�CameraDefine.bas��[ͼ���ʽ����]���֡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetMediaType Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piMediaType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetMediaType
    '// ��������    : ������������ԭʼ���ݸ�ʽ��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            iMediaType  ��CameraGetCapability�����������ԣ�
    '//                                               ��tSdkCameraCapbility�ṹ���е�pMediaTypeDesc
    '//                                               ��Ա�У����������ʽ���������֧�ֵĸ�ʽ��
    '//                                               iMediaType���Ǹ�����������š�
    '//                                               pMediaTypeDesc[iMediaType].iMediaType���ʾ��ǰ��ʽ��
    '//                                               ���롣�ñ�����μ�CameraDefine.bas��[ͼ���ʽ����]���֡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetMediaType Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iMediaType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAeState
    '// ��������    : ��������ع��ģʽ���Զ������ֶ���
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            bAeState    TRUE��ʹ���Զ��ع⣻FALSE��ֹͣ�Զ��ع⡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAeState Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bAeState As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAeState
    '// ��������    : ��������ǰ���ع�ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pAeState   ָ�룬���ڷ����Զ��ع��ʹ��״̬��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAeState Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pAeState As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetSharpness
    '// ��������    : ����ͼ��Ĵ�����񻯲�����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iSharpness �񻯲�������Χ��CameraGetCapability
    '//                                          ��ã�һ����[0,100]��0��ʾ�ر��񻯴���
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetSharpness Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iSharpness As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetSharpness
    '// ��������    : ��ȡ��ǰ���趨ֵ��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            piSharpness ָ�룬���ص�ǰ�趨���񻯵��趨ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetSharpness Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piSharpness As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetLutMode
    '// ��������    : ��������Ĳ��任ģʽLUTģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            emLutMode  LUTMODE_PARAM_GEN ��ʾ��٤��ͶԱȶȲ�����̬����LUT��
    '//                                              LUTMODE_PRESET    ��ʾʹ��Ԥ���LUT��
    '//                                              LUTMODE_USER_DEF  ��ʾʹ���û��Զ���LUT��
    '//                                              LUTMODE_PARAM_GEN�Ķ���ο�CameraDefine.bas��emSdkLutMode���͡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetLutMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal emLutMode As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetLutMode
    '// ��������    : �������Ĳ��任ģʽLUTģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pemLutMode ָ�룬���ص�ǰLUTģʽ��������CameraSetLutMode
    '//                                              ��emLutMode������ͬ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetLutMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pemLutMode As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSelectLutPreset
    '// ��������    : ѡ��Ԥ��LUTģʽ�µ�LUT��������ʹ��CameraSetLutMode
    '//                       ��LUTģʽ����ΪԤ��ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iSel              ��������š���ĸ�����CameraGetCapability
    '//                                              ��á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSelectLutPreset Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetLutPresetSel
    '// ��������    : ���Ԥ��LUTģʽ�µ�LUT�������š�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piSel      ָ�룬���ر�������š�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetLutPresetSel Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetCustomLut
    '// ��������    : �����Զ����LUT��������ʹ��CameraSetLutMode
    '//                       ��LUTģʽ����Ϊ�Զ���ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//             iChannel ָ��Ҫ�趨��LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
    '//                      ����ͨ����LUT����ͬʱ�滻��
    '//                      �ο�CameraDefine.h��emSdkLutChannel���塣
    '//            pLut              ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
    '//                                      4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetCustomLut Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iChannel As Integer, _
        ByVal pLut As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetCustomLut
    '// ��������    : ��õ�ǰʹ�õ��Զ���LUT��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//             iChannel ָ��Ҫ��õ�LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
    '//                      ���غ�ɫͨ����LUT��
    '//                      �ο�CameraDefine.h��emSdkLutChannel���塣
    '//            pLut       ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
    '//                                      4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetCustomLut Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iChannel As Integer, _
        ByRef pLut As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetCurrentLut
    '// ��������    : ��������ǰ��LUT�����κ�LUTģʽ�¶����Ե���,
    '//                       ����ֱ�۵Ĺ۲�LUT���ߵı仯��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//             iChannel ָ��Ҫ��õ�LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
    '//                      ���غ�ɫͨ����LUT��
    '//                      �ο�CameraDefine.h��emSdkLutChannel���塣
    '//            pLut       ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
    '//                                      4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetCurrentLut Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iChannel As Integer, _
        ByRef pLut As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetWbMode
    '// ��������    : ���������ƽ��ģʽ����Ϊ�ֶ����Զ����ַ�ʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            bAuto         TRUE�����ʾʹ���Զ�ģʽ��
    '//                                              FALSE�����ʾʹ���ֶ�ģʽ��ͨ������
    '//                                                        CameraSetOnceWB������һ�ΰ�ƽ�⡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetWbMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bAuto As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetWbMode
    '// ��������    : ��õ��ڵİ�ƽ��ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbAuto    ָ�룬����TRUE��ʾ�Զ�ģʽ��FALSE
    '//                                              Ϊ�ֶ�ģʽ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetWbMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbAuto As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetPresetClrTemp
    '// ��������    : ����ɫ��ģʽ
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//             iSel     �����š�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetPresetClrTemp Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetPresetClrTemp
    '// ��������    :
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            ByRef piSel
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetPresetClrTemp Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetOnceWB
    '// ��������    : ���ֶ���ƽ��ģʽ�£����øú��������һ�ΰ�ƽ�⡣
    '//                       ��Ч��ʱ��Ϊ���յ���һ֡ͼ������ʱ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetOnceWB Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetOnceBB
    '// ��������    : ִ��һ�κ�ƽ�������
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetOnceBB Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetAeTarget
    '// ��������    : �趨�Զ��ع������Ŀ��ֵ���趨��Χ��CameraGetCapability
    '//                       ������á�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iAeTarget  ����Ŀ��ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAeTarget Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iAeTarget As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAeTarget
    '// ��������    : ����Զ��ع������Ŀ��ֵ��
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            *piAeTarget ָ�룬����Ŀ��ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAeTarget Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piAeTarget As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetExposureTime
    '// ��������    : �����ع�ʱ�䡣��λΪ΢�롣����CMOS�����������ع�
    '//                       �ĵ�λ�ǰ�����������ģ���ˣ��ع�ʱ�䲢������΢��
    '//                       ���������ɵ������ǻᰴ��������ȡ�ᡣ�ڵ���
    '//                       �������趨�ع�ʱ��󣬽����ٵ���CameraGetExposureTime
    '//                       �����ʵ���趨��ֵ��
    '// ����            : hCamera        ����ľ������CameraInit������á�
    '//            fExposureTime �ع�ʱ�䣬��λ΢�롣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetExposureTime Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal fExposureTime As Double _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetExposureLineTime
    '// ��������    : ���һ�е��ع�ʱ�䡣����CMOS�����������ع�
    '//                       �ĵ�λ�ǰ�����������ģ���ˣ��ع�ʱ�䲢������΢��
    '//                       ���������ɵ������ǻᰴ��������ȡ�ᡣ���������
    '//                   ���þ��Ƿ���CMOS����ع�һ�ж�Ӧ��ʱ�䡣
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//                  pfLineTime
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExposureLineTime Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pfLineTime As Double _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetExposureTime
    '// ��������    : ���������ع�ʱ�䡣��μ�CameraSetExposureTime
    '//                       �Ĺ���������
    '// ����            : hCamera          ����ľ������CameraInit������á�
    '//            pfExposureTime  ָ�룬���ص�ǰ���ع�ʱ�䣬��λ΢�롣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExposureTime Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pfExposureTime As Double _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAnalogGain
    '// ��������    : ���������ͼ��ģ������ֵ����ֵ����CameraGetCapability���
    '//                       ��������Խṹ����sExposeDesc.fAnalogGainStep����
    '//                       �õ�ʵ�ʵ�ͼ���źŷŴ�����
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            iAnalogGain �趨��ģ������ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAnalogGain Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iAnalogGain As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAnalogGain
    '// ��������    : ���ͼ���źŵ�ģ������ֵ���μ�CameraSetAnalogGain
    '//                       ��ϸ˵����
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            piAnalogGain ָ�룬���ص�ǰ��ģ������ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAnalogGain Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piAnalogGain As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetGain
    '// ��������    : ����ͼ����������档�趨��Χ��CameraGetCapability
    '//                       ��õ�������Խṹ����sRgbGainRange��Ա������
    '//                       ʵ�ʵķŴ������趨ֵ/100��
    '// ����            : hCamera  ����ľ������CameraInit������á�
    '//            iRGain   ��ɫͨ��������ֵ��
    '//            iGGain   ��ɫͨ��������ֵ��
    '//            iBGain   ��ɫͨ��������ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetGain Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iRGain As Integer, _
        ByVal iGGain As Integer, _
        ByVal iBGain As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraGetGain
    '// ��������    : ���ͼ������������档������μ�CameraSetGain
    '//                       �Ĺ����������֡�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piRGain   ָ�룬���غ�ɫͨ������������ֵ��
    '//            piGGain    ָ�룬������ɫͨ������������ֵ��
    '//            piBGain    ָ�룬������ɫͨ������������ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetGain Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piRGain As Integer, _
        ByRef piGGain As Integer, _
        ByRef piBGain As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetGamma
    '// ��������    : �趨LUT��̬����ģʽ�µ�Gammaֵ���趨��ֵ��
    '//                       ���ϱ�����SDK�ڲ�������ֻ�е�������ڶ�̬
    '//                       �������ɵ�LUTģʽʱ���Ż���Ч����ο�CameraSetLutMode
    '//                       �ĺ���˵�����֡�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iGamma     Ҫ�趨��Gammaֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetGamma Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iGamma As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetGamma
    '// ��������    : ���LUT��̬����ģʽ�µ�Gammaֵ����ο�CameraSetGamma
    '//                       �����Ĺ���������
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piGamma    ָ�룬���ص�ǰ��Gammaֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetGamma Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piGamma As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetContrast
    '// ��������    : �趨LUT��̬����ģʽ�µĶԱȶ�ֵ���趨��ֵ��
    '//                       ���ϱ�����SDK�ڲ�������ֻ�е�������ڶ�̬
    '//                       �������ɵ�LUTģʽʱ���Ż���Ч����ο�CameraSetLutMode
    '//                       �ĺ���˵�����֡�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iContrast  �趨�ĶԱȶ�ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetContrast Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iContrast As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetContrast
    '// ��������    : ���LUT��̬����ģʽ�µĶԱȶ�ֵ����ο�
    '//                       CameraSetContrast�����Ĺ���������
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piContrast ָ�룬���ص�ǰ�ĶԱȶ�ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetContrast Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piContrast As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetSaturation
    '// ��������    : �趨ͼ����ı��Ͷȡ��Ժڰ������Ч��
    '//                       �趨��Χ��CameraGetCapability��á�100��ʾ
    '//                       ��ʾԭʼɫ�ȣ�����ǿ��
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            iSaturation  �趨�ı��Ͷ�ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetSaturation Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iSaturation As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetSaturation
    '// ��������    : ���ͼ����ı��Ͷȡ�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            piSaturation ָ�룬���ص�ǰͼ����ı��Ͷ�ֵ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetSaturation Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piSaturation As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetMonochrome
    '// ��������    : ���ò�ɫתΪ�ڰ׹��ܵ�ʹ�ܡ�
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            bEnable   TRUE����ʾ����ɫͼ��תΪ�ڰס�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetMonochrome Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetMonochrome
    '// ��������    : ��ò�ɫת���ڰ׹��ܵ�ʹ��״����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbEnable   ָ�롣����TRUE��ʾ�����˲�ɫͼ��
    '//                                              ת��Ϊ�ڰ�ͼ��Ĺ��ܡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetMonochrome Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetInverse
    '// ��������    : ���ò�ͼ����ɫ��ת���ܵ�ʹ�ܡ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            bEnable    TRUE����ʾ����ͼ����ɫ��ת���ܣ�
    '//                                              ���Ի�����ƽ����Ƭ��Ч����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetInverse Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetInverse
    '// ��������    : ���ͼ����ɫ��ת���ܵ�ʹ��״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbEnable   ָ�룬���ظù���ʹ��״̬��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetInverse Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAntiFlick
    '// ��������    : �����Զ��ع�ʱ��Ƶ�����ܵ�ʹ��״̬�������ֶ�
    '//                       �ع�ģʽ����Ч��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            bEnable    TRUE��������Ƶ������;FALSE���رոù��ܡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAntiFlick Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAntiFlick
    '// ��������    : ����Զ��ع�ʱ��Ƶ�����ܵ�ʹ��״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbEnable   ָ�룬���ظù��ܵ�ʹ��״̬��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAntiFlick Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetLightFrequency
    '// ��������    : ����Զ��ع�ʱ����Ƶ����Ƶ��ѡ��
    '// ����            : hCamera        ����ľ������CameraInit������á�
    '//            piFrequencySel ָ�룬����ѡ��������š�0:50HZ 1:60HZ
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetLightFrequency Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piFrequencySel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetLightFrequency
    '// ��������    : �����Զ��ع�ʱ��Ƶ����Ƶ�ʡ�
    '// ����            : hCamera       ����ľ������CameraInit������á�
    '//            iFrequencySel 0:50HZ , 1:60HZ
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetLightFrequency Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iFrequencySel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetFrameSpeed
    '// ��������    : �趨������ͼ���֡�ʡ�����ɹ�ѡ���֡��ģʽ��
    '//                       CameraGetCapability��õ���Ϣ�ṹ����iFrameSpeedDesc
    '//                       ��ʾ���֡��ѡ��ģʽ������
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            iFrameSpeed ѡ���֡��ģʽ�����ţ���Χ��0��
    '//                                               CameraGetCapability��õ���Ϣ�ṹ����iFrameSpeedDesc - 1
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetFrameSpeed Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iFrameSpeed As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetFrameSpeed
    '// ��������    : ���������ͼ���֡��ѡ�������š������÷��ο�
    '//                       CameraSetFrameSpeed�����Ĺ����������֡�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            piFrameSpeed ָ�룬����ѡ���֡��ģʽ�����š�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetFrameSpeed Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piFrameSpeed As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetParameterMode
    '// ��������    : �趨������ȡ��Ŀ�����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iMode     ������ȡ�Ķ��󡣲ο�CameraDefine.bas
    '//                                     ��emSdkParameterMode�����Ͷ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetParameterMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iTarget As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetParameterMode
    '// ��������    :
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            ByRef piMode
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetParameterMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piMode As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetParameterMask
    '// ��������    : ���ò�����ȡ�����롣�������غͱ���ʱ����ݸ�
    '//                       ��������������ģ��������Ƿ���ػ��߱��档
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            uMask     ���롣�ο�CameraDefine.bas��PROP_SHEET_INDEX
    '//                                             ���Ͷ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetParameterMask Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal uMask As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSaveParameter
    '// ��������    : ���浱ǰ���������ָ���Ĳ������С�����ṩ��A,B,C,D
    '//                       A,B,C,D����ռ������в����ı��档
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iTeam      PARAMETER_TEAM_A ���浽A����,
    '//                                              PARAMETER_TEAM_B ���浽B����,
    '//                                              PARAMETER_TEAM_C ���浽C����,
    '//                                              PARAMETER_TEAM_D ���浽D����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSaveParameter Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iTeam As Integer _
    ) As Integer

    '/******************************************************/
    '// ������   : CameraSaveParameterToFile
    '// �������� : ���浱ǰ���������ָ�����ļ��С����ļ����Ը��Ƶ�
    '//        ��ĵ����Ϲ�����������أ�Ҳ���������������á�
    '// ����     : hCamera  ����ľ������CameraInit������á�
    '//            sFileName  �����ļ�������·����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function _CameraSaveParameterToFile Lib "MVCAMSDK.dll" Alias "CameraSaveParameterToFile" _
    ( _
        ByVal hCamera As Integer, _
        ByRef sFileName As Byte _
    ) As Integer

    Public Function CameraSaveParameterToFile(ByVal hCamera As Integer, ByVal sFileName As String)
        CameraSaveParameterToFile = _CameraSaveParameterToFile(hCamera, StringToBytes(sFileName)(0))
    End Function

    '/******************************************************/
    '// ������      : CameraReadParameterFromFile
    '// ��������    : ��PC��ָ���Ĳ����ļ��м��ز������ҹ�˾�������
    '//                       ������PC��Ϊ.config��׺���ļ���λ�ڰ�װ�µ�
    '//                       Camera\Configs�ļ����С�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            *sFileName �����ļ�������·����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function _CameraReadParameterFromFile Lib "MVCAMSDK.dll" Alias "CameraReadParameterFromFile" _
    ( _
            ByVal hCamera As Integer, _
            ByRef sFileName As Byte _
    ) As Integer

    Public Function CameraReadParameterFromFile(ByVal hCamera As Integer, ByVal sFileName As String)
        CameraReadParameterFromFile = _CameraReadParameterFromFile(hCamera, StringToBytes(sFileName)(0))
    End Function

    '/******************************************************/
    '// ������      : CameraLoadParameter
    '// ��������    : ����ָ����Ĳ���������С�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iTeam             PARAMETER_TEAM_A ����A�����,
    '//                                              PARAMETER_TEAM_B ����B�����,
    '//                                              PARAMETER_TEAM_C ����C�����,
    '//                                              PARAMETER_TEAM_D ����D�����,
    '//                                              PARAMETER_TEAM_DEFAULT ����Ĭ�ϲ�����
    '//                                              ���Ͷ���ο�CameraDefine.bas��emSdkParameterTeam����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraLoadParameter Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iTeam As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetCurrentParameterGroup
    '// ��������    : ��õ�ǰѡ��Ĳ����顣
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piTeam     ָ�룬���ص�ǰѡ��Ĳ����顣����ֵ
    '//                                              �ο�CameraLoadParameter��iTeam������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetCurrentParameterGroup Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piTeam As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetTransPackLen
    '// ��������    : �����������ͼ�����ݵķְ���С��
    '//                       Ŀǰ��SDK�汾�У��ýӿڽ���GIGE�ӿ������Ч��
    '//                       �����������紫��ķְ���С������֧�־�֡��������
    '//                       ���ǽ���ѡ��8K�ķְ���С��������Ч�Ľ��ʹ���
    '//                       ��ռ�õ�CPU����ʱ�䡣
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iPackSel  �ְ�����ѡ��������š��ְ����ȿ���
    '//                                              ���������Խṹ����pPackLenDesc��Ա������
    '//                                              iPackLenDesc��Ա���ʾ����ѡ�ķְ�ģʽ������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetTransPackLen Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iPackSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetTransPackLen
    '// ��������    : ��������ǰ����ְ���С��ѡ�������š�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piPackSel  ָ�룬���ص�ǰѡ��ķְ���С�����š�
    '//                                              �μ�CameraSetTransPackLen��iPackSel��
    '//                                              ˵����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetTransPackLen Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piPackSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraIsAeWinVisible
    '// ��������    : ����Զ��ع�ο����ڵ���ʾ״̬��
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            pbIsVisible  ָ�룬����TRUE�����ʾ��ǰ���ڻ�
    '//                                                ��������ͼ�������ϡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraIsAeWinVisible Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbIsVisible As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAeWinVisible
    '// ��������    : �����Զ��ع�ο����ڵ���ʾ״̬�������ô���״̬
    '//                       Ϊ��ʾ������CameraImageOverlay���ܹ�������λ��
    '//                       �Ծ��εķ�ʽ������ͼ���ϡ�
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            bIsVisible  TRUE������Ϊ��ʾ��FALSE������ʾ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAeWinVisible Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bIsVisible As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAeWindow
    '// ��������    : ����Զ��ع�ο����ڵ�λ�á�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piHOff     ָ�룬���ش���λ�����ϽǺ�����ֵ��
    '//            piVOff     ָ�룬���ش���λ�����Ͻ�������ֵ��
    '//            piWidth    ָ�룬���ش��ڵĿ�ȡ�
    '//            piHeight   ָ�룬���ش��ڵĸ߶ȡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAeWindow Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piHOff As Integer, _
        ByRef piVOff As Integer, _
        ByRef piWidth As Integer, _
        ByRef piHeight As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAeWindow
    '// ��������    : �����Զ��ع�Ĳο����ڡ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iHOff             �������Ͻǵĺ�����
    '//            iVOff      �������Ͻǵ�������
    '//            iWidth     ���ڵĿ��
    '//            iHeight    ���ڵĸ߶�
    '//                       ���iHOff��iVOff��iWidth��iHeightȫ��Ϊ0����
    '//                       ��������Ϊÿ���ֱ����µľ���1/2��С����������
    '//                       �ֱ��ʵı仯������仯�����iHOff��iVOff��iWidth��iHeight
    '//                       �������Ĵ���λ�÷�Χ�����˵�ǰ�ֱ��ʷ�Χ�ڣ�
    '//                   ���Զ�ʹ�þ���1/2��С���ڡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAeWindow Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iHOff As Integer, _
        ByVal iVOff As Integer, _
        ByVal iWidth As Integer, _
        ByVal iHeight As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetMirror
    '// ��������    : ����ͼ������������������Ϊˮƽ�ʹ�ֱ��������
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iDir              ��ʾ����ķ���0����ʾˮƽ����1����ʾ��ֱ����
    '//            bEnable   TRUE��ʹ�ܾ���;FALSE����ֹ����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetMirror Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iDir As Integer, _
        ByVal bEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetMirror
    '// ��������    : ���ͼ��ľ���״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iDir              ��ʾҪ��õľ�����
    '//                                              0����ʾˮƽ����1����ʾ��ֱ����
    '//            pbEnable   ָ�룬����TRUE�����ʾiDir��ָ�ķ���
    '//                                              ����ʹ�ܡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetMirror Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iDir As Integer, _
        ByRef pbEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetWbWindow
    '// ��������    : ��ð�ƽ��ο����ڵ�λ�á�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            PiHOff    ָ�룬���زο����ڵ����ϽǺ�����       ��
    '//            PiVOff     ָ�룬���زο����ڵ����Ͻ�������      ��
    '//            PiWidth    ָ�룬���زο����ڵĿ�ȡ�
    '//            PiHeight   ָ�룬���زο����ڵĸ߶ȡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetWbWindow Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piHOff As Integer, _
        ByRef piVOff As Integer, _
        ByRef piWidth As Integer, _
        ByRef piHeight As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetWbWindow
    '// ��������    : ���ð�ƽ��ο����ڵ�λ�á�
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            iHOff            �ο����ڵ����ϽǺ����ꡣ
    '//            iVOff     �ο����ڵ����Ͻ������ꡣ
    '//            iWidth    �ο����ڵĿ�ȡ�
    '//            iHeight   �ο����ڵĸ߶ȡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetWbWindow Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iHOff As Integer, _
        ByVal iVOff As Integer, _
        ByVal iWidth As Integer, _
        ByVal iHeight As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraIsWbWinVisible
    '// ��������    : ��ð�ƽ�ⴰ�ڵ���ʾ״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbShow    ָ�룬����TRUE�����ʾ�����ǿɼ��ġ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraIsWbWinVisible Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbShow As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetWbWinVisible
    '// ��������    : ���ð�ƽ�ⴰ�ڵ���ʾ״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            bShow      TRUE�����ʾ����Ϊ�ɼ����ڵ���
    '//                                              CameraImageOverlay��ͼ�������Ͻ��Ծ���
    '//                                              �ķ�ʽ���Ӱ�ƽ��ο����ڵ�λ�á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetWbWinVisible Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bShow As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImageOverlay
    '// ��������    : �������ͼ�������ϵ���ʮ���ߡ���ƽ��ο����ڡ�
    '//                       �Զ��ع�ο����ڵ�ͼ�Ρ�ֻ������Ϊ�ɼ�״̬��
    '//                       ʮ���ߺͲο����ڲ��ܱ������ϡ�
    '//                       ע�⣬�ú���������ͼ�������RGB��ʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pRgbBuffer ͼ�����ݻ�������
    '//            pFrInfo    ͼ���֡ͷ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImageOverlay Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal pRgbBuffer As IntPtr, _
        ByRef pFrInfo As tSdkFrameHead _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetCrossLine
    '// ��������    : ����ָ��ʮ���ߵĲ�����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iLine             ��ʾҪ���õڼ���ʮ���ߵ�״̬����ΧΪ[0,8]����9����
    '//            x          ʮ��������λ�õĺ�����ֵ��
    '//            y                         ʮ��������λ�õ�������ֵ��
    '//            uColor     ʮ���ߵ���ɫ����ʽΪ(R|(G<<8)|(B<<16))
    '//            bVisible   ʮ���ߵ���ʾ״̬��TRUE����ʾ��ʾ��
    '//                                              ֻ������Ϊ��ʾ״̬��ʮ���ߣ��ڵ���
    '//                                              CameraImageOverlay��Żᱻ���ӵ�ͼ���ϡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetCrossLine Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iLine As Integer, _
        ByVal x As Integer, _
        ByVal y As Integer, _
        ByVal uColor As Integer, _
        ByVal bVisible As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetCrossLine
    '// ��������    : ���ָ��ʮ���ߵ�״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iLine             ��ʾҪ��ȡ�ĵڼ���ʮ���ߵ�״̬����ΧΪ[0,8]����9����
    '//            px                ָ�룬���ظ�ʮ��������λ�õĺ����ꡣ
    '//            py                ָ�룬���ظ�ʮ��������λ�õĺ����ꡣ
    '//            pcolor     ָ�룬���ظ�ʮ���ߵ���ɫ����ʽΪ(R|(G<<8)|(B<<16))��
    '//            pbVisible  ָ�룬����TRUE�����ʾ��ʮ���߿ɼ���
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetCrossLine Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iLine As Integer, _
        ByRef px As Integer, _
        ByRef py As Integer, _
        ByRef pcolor As Integer, _
        ByRef pbVisible As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetCapability
    '// ��������    : �����������������ṹ�塣�ýṹ���а��������
    '//                       �����õĸ��ֲ����ķ�Χ��Ϣ����������غ����Ĳ���
    '//                       ���أ�Ҳ�����ڶ�̬������������ý��档
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            pCameraInfo ָ�룬���ظ�������������Ľṹ�塣
    '//                           tSdkCameraCapbility��CameraDefine.bas�ж��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetCapability Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pCameraInfo As tSdkCameraCapbility _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraWriteSN
    '// ��������    : ������������кš��ҹ�˾������кŷ�Ϊ3����
    '//                       0�������ҹ�˾�Զ����������кţ�����ʱ�Ѿ�
    '//                       �趨�ã�1����2���������ο���ʹ�á�ÿ������
    '//                       �ų��ȶ���32���ֽڡ�
    '// ����            : hCamera  ����ľ������CameraInit������á�
    '//            pbySN       ���кŵĻ�������
    '//            iLevel   Ҫ�趨�����кż���ֻ����1����2��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraWriteSN Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pbySN As Byte, _
        ByVal iLevel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraReadSN
    '// ��������    : ��ȡ���ָ����������кš����кŵĶ�����ο�
    '//                   CameraWriteSN�����Ĺ����������֡�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pbySN             ���кŵĻ�������
    '//            iLevel     Ҫ��ȡ�����кż���ֻ����1��2��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraReadSN Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pbySN As Byte, _
        ByVal iLevel As Integer _
    ) As Integer
    '/******************************************************/
    '// ������      : CameraSetTriggerDelayTime
    '// ��������    : ����Ӳ������ģʽ�µĴ�����ʱʱ�䣬��λ΢�롣
    '//                       ��Ӳ�����ź����ٺ󣬾���ָ������ʱ���ٿ�ʼ�ɼ�
    '//                       ͼ�񡣽������ͺŵ����֧�ָù��ܡ�������鿴
    '//                       ��Ʒ˵���顣
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            uDelayTimeUs Ӳ������ʱ����λ΢�롣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetTriggerDelayTime Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal uDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetTriggerDelayTime
    '// ��������    : ��õ�ǰ�趨��Ӳ������ʱʱ�䡣
    '// ����            : hCamera       ����ľ������CameraInit������á�
    '//            puDelayTimeUs ָ�룬������ʱʱ�䣬��λ΢�롣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetTriggerDelayTime Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef puDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetTriggerCount
    '// ��������    : ���ô���ģʽ�µĴ���֡���������������Ӳ������
    '//                       ģʽ����Ч��Ĭ��Ϊ1֡����һ�δ����źŲɼ�һ֡ͼ��
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            iCount    һ�δ����ɼ���֡����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetTriggerCount Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iCount As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetTriggerCount
    '// ��������    : ���һ�δ�����֡����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            ByRef piCount
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetTriggerCount Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piCount As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSoftTrigger
    '// ��������    : ִ��һ��������ִ�к󣬻ᴥ����CameraSetTriggerCount
    '//                   ָ����֡����
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSoftTrigger Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetTriggerMode
    '// ��������    : ��������Ĵ���ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iModeSel   ģʽѡ�������š����趨��ģʽ��
    '//                                              CameraGetCapability������ȡ����ο�
    '//                                          CameraDefine.bas��tSdkCameraCapbility�Ķ��塣
    '//                                              һ�������0��ʾ�����ɼ�ģʽ��1��ʾ
    '//                                              �������ģʽ��2��ʾӲ������ģʽ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetTriggerMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iModeSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetTriggerMode
    '// ��������    : �������Ĵ���ģʽ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            piModeSel  ָ�룬���ص�ǰѡ����������ģʽ�������š�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetTriggerMode Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piModeSel As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraGetResolutionForSnap
    '// ��������    : ���ץ��ģʽ�µķֱ���ѡ�������š�
    '// ����            : hCamera          ����ľ������CameraInit������á�
    '//            pImageResolution ָ�룬����ץ��ģʽ�ķֱ��ʡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetResolutionForSnap Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pImageResolution As tSdkImageResolution _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetResolutionForSnap
    '// ��������    : ����ץ��ģʽ��������ͼ��ķֱ��ʡ�
    '// ����            : hCamera         ����ľ������CameraInit������á�
    '//            pImageResolution ���pImageResolution->iWidth
    '//                                                        �� pImageResolution->iHeight��Ϊ0��
    '//                                        ���ʾ�趨Ϊ���浱ǰԤ���ֱ��ʡ�ץ
    '//                                        �µ���ͼ��ķֱ��ʻ�͵�ǰ�趨��
    '//                                                        Ԥ���ֱ���һ����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetResolutionForSnap Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pImageResolution As tSdkImageResolution _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraCustomizeResolution
    '// ��������    : �򿪷ֱ����Զ�����壬��ͨ�����ӻ��ķ�ʽ
    '//                       ������һ���Զ���ֱ��ʡ�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            pImageCustom ָ�룬�����Զ���ķֱ��ʡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraCustomizeResolution Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pImageCustom As tSdkImageResolution _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraCustomizeReferWin
    '// ��������    : �򿪲ο������Զ�����塣��ͨ�����ӻ��ķ�ʽ��
    '//                       ���һ���Զ��崰�ڵ�λ�á�һ�������Զ����ƽ��
    '//                       ���Զ��ع�Ĳο����ڡ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            iWinType   Ҫ���ɵĲο����ڵ���;��0,�Զ��ع�ο����ڣ�
    '//                                              1,��ƽ��ο����ڡ�
    '//            hParent    ���øú����Ĵ��ڵľ��������ΪNULL��
    '//            piHOff     ָ�룬�����Զ��崰�ڵ����ϽǺ����ꡣ
    '//            piVOff     ָ�룬�����Զ��崰�ڵ����Ͻ������ꡣ
    '//            piWidth    ָ�룬�����Զ��崰�ڵĿ�ȡ�
    '//            piHeight   ָ�룬�����Զ��崰�ڵĸ߶ȡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraCustomizeReferWin Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iWinType As Integer, _
        ByVal hParent As IntPtr, _
        ByRef piHOff As Integer, _
        ByRef piVOff As Integer, _
        ByRef piWidth As Integer, _
        ByRef piHeight As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraCreateSettingPage
    '// ��������    : ������������������ô��ڡ����øú�����SDK�ڲ���
    '//                       ������������������ô��ڣ�ʡȥ�������¿������
    '//                       ���ý����ʱ�䡣ǿ�ҽ���ʹ����ʹ�øú�����
    '//                       SDKΪ�����������ô��ڡ�
    '// ����            : hCamera       ����ľ������CameraInit������á�
    '//            hParent       Ӧ�ó��������ڵľ��������ΪNULL��
    '//            pWinText      �ַ���ָ�룬������ʾ�ı�������
    '//            pCallbackFunc ������Ϣ�Ļص�����������Ӧ���¼�����ʱ��
    '//                                                     pCallbackFunc��ָ��ĺ����ᱻ���ã�
    '//                                                     �����л��˲���֮��Ĳ���ʱ��pCallbackFunc
    '//                                                     ���ص�ʱ������ڲ�����ָ������Ϣ���͡�
    '//                                                     �������Է������Լ������Ľ�����������ɵ�UI
    '//                                                     ֮�����ͬ�����ò�������ΪNULL��
    '//            pCallbackCtx  �ص������ĸ��Ӳ���������ΪNULL��pCallbackCtx
    '//                                                     ����pCallbackFunc���ص�ʱ����Ϊ����֮һ���롣
    '//                                                     ������ʹ�øò�������һЩ�����жϡ�
    '//            uReserved     Ԥ������������Ϊ0��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraCreateSettingPage Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal hParent As IntPtr, _
            ByRef pWinText As Byte, _
            ByVal pCallbackFunc As IntPtr, _
            ByVal pCallbackCtx As IntPtr, _
            ByVal uReserved As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraShowSettingPage
    '// ��������    : ��������������ô�����ʾ״̬�������ȵ���CameraCreateSettingPage
    '//                       �ɹ���������������ô��ں󣬲��ܵ��ñ���������
    '//                       ��ʾ��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            bShow             TRUE����ʾ;FALSE�����ء�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraShowSettingPage Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal bShow As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetActiveSettingSubPage
    '// ��������    : ����������ô��ڵļ���ҳ�档������ô����ж��
    '//                       ��ҳ�湹�ɣ��ú��������趨��ǰ��һ����ҳ��
    '//                       Ϊ����״̬����ʾ����ǰ�ˡ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            index      ��ҳ��������š��ο�CameraDefine.bas��
    '//                                              PROP_SHEET_INDEX�Ķ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetActiveSettingSubPage Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal index As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSpecialControl
    '// ��������    : ���һЩ�������������õĽӿڣ����ο���ʱһ�㲻��Ҫ
    '//                       ���á�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            dwCtrlCode �����롣
    '//            dwParam    �������룬��ͬ��dwCtrlCodeʱ�����岻ͬ��
    '//            lpData     ���Ӳ�������ͬ��dwCtrlCodeʱ�����岻ͬ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSpecialControl Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal dwCtrlCode As Integer, _
        ByVal dwParam As Integer, _
        ByVal lpData As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetFrameStatistic
    '// ��������    : ����������֡�ʵ�ͳ����Ϣ����������֡�Ͷ�֡�������
    '// ����            : hCamera          ����ľ������CameraInit������á�
    '//            psFrameStatistic ָ�룬����ͳ����Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetFrameStatistic Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef psFrameStatistic As tSdkFrameStatistic _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetNoiseFilter
    '// ��������    : ����ͼ����ģ���ʹ��״̬��
    '// ����            : hCamera   ����ľ������CameraInit������á�
    '//            bEnable   TRUE��ʹ�ܣ�FALSE����ֹ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetNoiseFilter Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal bEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetNoiseFilterState
    '// ��������    : ���ͼ����ģ���ʹ��״̬��
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            *pEnable   ָ�룬����״̬��TRUE��Ϊʹ�ܡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetNoiseFilterState Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef pEnable As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraRstTimeStamp
    '// ��������    : ��λͼ��ɼ���ʱ�������0��ʼ��
    '// ����            : ByVal hCamera As Integer, _
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraRstTimeStamp Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer


    '//******************************************************/
    '// ������   : CameraSaveUserData
    '// �������� : ���û��Զ�������ݱ��浽����ķ����Դ洢���С�
    '//              ÿ���ͺŵ��������֧�ֵ��û���������󳤶Ȳ�һ����
    '//              ���Դ��豸�����������л�ȡ�ó�����Ϣ��
    '// ����     : hCamera    ����ľ������CameraInit������á�
    '//            uStartAddr  ��ʼ��ַ����0��ʼ��
    '//            pbData      ���ݻ������׵�ַ
    '//            ilen        д�����ݵĳ��ȣ�ilen + uStartAddr����
    '//                        С���û�����󳤶�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '//******************************************************/
    Public Declare Function CameraSaveUserData Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uStartAddr As Integer, _
        ByRef pbData As Byte, _
        ByVal ilen As Integer _
    ) As Integer


    '//******************************************************/
    '// ������   : CameraLoadUserData
    '// �������� : ������ķ����Դ洢���ж�ȡ�û��Զ�������ݡ�
    '//              ÿ���ͺŵ��������֧�ֵ��û���������󳤶Ȳ�һ����
    '//              ���Դ��豸�����������л�ȡ�ó�����Ϣ��
    '// ����     : hCamera    ����ľ������CameraInit������á�
    '//            uStartAddr  ��ʼ��ַ����0��ʼ��
    '//            pbData      ���ݻ������׵�ַ�����ض��������ݡ�
    '//            ilen        ��ȡ���ݵĳ��ȣ�ilen + uStartAddr����
    '//                        С���û�����󳤶�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '//******************************************************/
    Public Declare Function CameraLoadUserData Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uStartAddr As Integer, _
        ByRef pbData As Byte, _
        ByVal ilen As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraGetFriendlyName
    '// ��������    : ��ȡ�û��Զ�����豸�ǳơ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pName      ָ�룬����ָ��0��β���ַ�����
    '//                                              �豸�ǳƲ�����32���ֽڣ���˸�ָ��
    '//                                              ָ��Ļ�����������ڵ���32���ֽڿռ䡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetFriendlyName Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pName As Byte _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetFriendlyName
    '// ��������    : �����û��Զ�����豸�ǳơ�
    '// ����            : hCamera    ����ľ������CameraInit������á�
    '//            pName      ָ�룬ָ��0��β���ַ�����
    '//                                              �豸�ǳƲ�����32���ֽڣ���˸�ָ��
    '//                                              ָ���ַ�������С�ڵ���32���ֽڿռ䡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetFriendlyName Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pName As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : __stdcall CameraSdkGetVersionString
    '// ��������    :
    '// ����            : pVersionString ָ�룬����SDK�汾�ַ�����
    '//                            ��ָ��ָ��Ļ�������С�������
    '//                            32���ֽ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSdkGetVersionString Lib "MVCAMSDK.dll" _
    ( _
            ByRef pVersionString As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraCheckFwUpdate
    '// ��������    : ���̼��汾���Ƿ���Ҫ������
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            pNeedUpdate ָ�룬���ع̼����״̬��TRUE��ʾ��Ҫ����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraCheckFwUpdate Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pNeedUpdate As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetFirmwareVision
    '// ��������    : ��ù̼��汾���ַ���
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            pVersion ����ָ��һ������32�ֽڵĻ�������
    '//                      ���ع̼��İ汾�ַ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetFirmwareVision Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pVersion As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetEnumInfo
    '// ��������    : ���ָ���豸��ö����Ϣ
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            pCameraInfo ָ�룬�����豸��ö����Ϣ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetEnumInfo Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pCameraInfo As tSdkCameraDevInfo _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetInerfaceVersion
    '// ��������    : ���ָ���豸�ӿڵİ汾
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            pVersion ָ��һ������32�ֽڵĻ����������ؽӿڰ汾�ַ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetInerfaceVersion Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef pVersion As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetIOState
    '// ��������    : ����ָ��IO�ĵ�ƽ״̬��IOΪ�����IO�����
    '//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
    '//              iOutputIoCounts������
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            iOutputIOIndex IO�������ţ���0��ʼ��
    '//            uState Ҫ�趨��״̬��1Ϊ�ߣ�0Ϊ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetIOState Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iOutputIOIndex As Integer, _
            ByVal uState As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetIOState
    '// ��������    : ����ָ��IO�ĵ�ƽ״̬��IOΪ������IO�����
    '//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
    '//              iInputIoCounts������
    '// ����            : hCamera ����ľ������CameraInit������á�
    '//            iInputIOIndex IO�������ţ���0��ʼ��
    '//            puState ָ�룬����IO״̬,1Ϊ�ߣ�0Ϊ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetIOState Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iInputIOIndex As Integer, _
            ByRef puState As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetAeAlgorithm
    '// ��������    : �����Զ��ع�ʱѡ����㷨����ͬ���㷨������
    '//                       ��ͬ�ĳ�����
    '// ����            : hCamera                   ����ľ������CameraInit������á�
    '//            iIspProcessor    ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
    '//                                                             emSdkIspProcessor�Ķ���
    '//            iAeAlgorithmSel   Ҫѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
    '//                                                             ��iAeAlmSwDesc��iAeAlmHdDesc������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetAeAlgorithm Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iIspProcessor As Integer, _
        ByVal iAeAlgorithmSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetAeAlgorithm
    '// ��������    : ��õ�ǰ�Զ��ع���ѡ����㷨
    '// ����            : hCamera                   ����ľ������CameraInit������á�
    '//            iIspProcessor    ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
    '//                                                             emSdkIspProcessor�Ķ���
    '//            piAeAlgorithmSel  ���ص�ǰѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
    '//                                                             ��iAeAlmSwDesc��iAeAlmHdDesc������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetAeAlgorithm Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iIspProcessor As Integer, _
        ByRef piAlgorithmSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetBayerDecAlgorithm
    '// ��������    : ����Bayer����ת��ɫ���㷨��
    '// ����            : hCamera                   ����ľ������CameraInit������á�
    '//            iIspProcessor    ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
    '//                                                             emSdkIspProcessor�Ķ���
    '//            iAlgorithmSel     Ҫѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
    '//                                                             ��iBayerDecAlmSwDesc��iBayerDecAlmHdDesc������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetBayerDecAlgorithm Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iIspProcessor As Integer, _
        ByVal iAlgorithmSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetBayerDecAlgorithm
    '// ��������    : ���Bayer����ת��ɫ��ѡ����㷨��
    '// ����            : hCamera                   ����ľ������CameraInit������á�
    '//            iIspProcessor    ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
    '//                                                             emSdkIspProcessor�Ķ���
    '//            piAlgorithmSel    ���ص�ǰѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
    '//                                                             ��iBayerDecAlmSwDesc��iBayerDecAlmHdDesc������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetBayerDecAlgorithm Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iIspProcessor As Integer, _
        ByRef piAlgorithmSel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetIspProcessor
    '// ��������    : ����ͼ����Ԫ���㷨ִ�ж�����PC�˻��������
    '//                       ��ִ���㷨�����������ִ��ʱ���ή��PC�˵�CPUռ���ʡ�
    '// ����            : hCamera           ����ľ������CameraInit������á�
    '//            iIspProcessor    �ο�CameraDefine.h��
    '//                                                     emSdkIspProcessor�Ķ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetIspProcessor Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByVal iIspProcessor As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetIspProcessor
    '// ��������    : ���ͼ����Ԫ���㷨ִ�ж���
    '// ����            : hCamera            ����ľ������CameraInit������á�
    '//            piIspProcessor ����ѡ��Ķ��󣬷���ֵ�ο�CameraDefine.h��
    '//                                                      emSdkIspProcessor�Ķ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetIspProcessor Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
        ByRef piIspProcessor As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetBlackLevel
    '// ��������    : ����ͼ��ĺڵ�ƽ��׼��Ĭ��ֵΪ0
    '// ����            : hCamera     ����ľ������CameraInit������á�
    '//            iBlackLevel Ҫ�趨�ĵ�ƽֵ����ΧΪ0��255��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetBlackLevel Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iBlackLevel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetBlackLevel
    '// ��������    : ���ͼ��ĺڵ�ƽ��׼��Ĭ��ֵΪ0
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            piBlackLevel ���ص�ǰ�ĺڵ�ƽֵ����ΧΪ0��255��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetBlackLevel Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef piBlackLevel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetWhiteLevel
    '// ��������    : ����ͼ��İ׵�ƽ��׼��Ĭ��ֵΪ255
    '// ����            : hCamera           ����ľ������CameraInit������á�
    '//            iWhiteLevel      Ҫ�趨�ĵ�ƽֵ����ΧΪ0��255��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetWhiteLevel Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByVal iWhiteLevel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetWhiteLevel
    '// ��������    : ���ͼ��İ׵�ƽ��׼��Ĭ��ֵΪ255
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//            piWhiteLevel ���ص�ǰ�İ׵�ƽֵ����ΧΪ0��255��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetWhiteLevel Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer, _
            ByRef piWhiteLevel As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraEnumerateDeviceEx
    '// ��������	: ö���豸���������豸�б��ڵ���CameraInitEx
    '//			  ֮ǰ��������øú���ö���豸��
    '// ����	     : 
    '// ����ֵ     : �����豸������0��ʾ�ޡ�
    '/******************************************************/
    Public Declare Function CameraEnumerateDeviceEx Lib "MVCAMSDK.dll" _
    ( _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraInitEx
    '// ��������	: �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
    '//			  �����صĲ����ӿڡ�		
    '// ����	    : iDeviceIndex    ����������ţ�CameraEnumerateDeviceEx�������������	
    '//            iParamLoadMode  �����ʼ��ʱʹ�õĲ������ط�ʽ��-1��ʾʹ���ϴ��˳�ʱ�Ĳ������ط�ʽ��
    '//            emTeam         ��ʼ��ʱʹ�õĲ����顣-1��ʾ�����ϴ��˳�ʱ�Ĳ����顣
    '//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
    '//							 ���ظ��������Ч������ڵ����������
    '//							 ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
    '//							 ���ڶ����֮������֡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraInitEx Lib "MVCAMSDK.dll" _
    ( _
        ByVal iDeviceIndex As Integer, _
        ByVal iParamLoadMode As Integer, _
        ByVal emTeam As Integer, _
        ByRef pCameraHandle As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraInitEx2
    '// ��������    : �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
    '//                       �����صĲ����ӿڡ�  ע����Ҫ�ȵ���CameraEnumerateDeviceExö�����
    '// ����            : CameraName    �������
    '//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
    '//                                                      ���ظ��������Ч������ڵ����������
    '//                                                      ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
    '//                                                      ���ڶ����֮������֡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function _CameraInitEx2 Lib "MVCAMSDK.dll" Alias "CameraInitEx2" _
    ( _
        ByRef CameraName As Byte, _
        ByRef pCameraHandle As Integer _
    ) As Integer

    Public Function CameraInitEx2(ByVal CameraName As String, ByRef pCameraHandle As Integer) As Integer
        CameraInitEx2 = _CameraInitEx2(StringToBytes(CameraName)(0), pCameraHandle)
    End Function

    '/******************************************************/
    '// ������ 	: CameraGetImageBufferEx
    '// ��������	: ���һ֡ͼ�����ݡ��ýӿڻ�õ�ͼ���Ǿ���������RGB��ʽ���ú������ú�
    '//			  ����Ҫ���� CameraReleaseImageBuffer �ͷţ�Ҳ��Ҫ����free֮��ĺ����ͷ�
    '//              ���ͷŸú������ص�ͼ�����ݻ�������
    '// ����	    : hCamera	  ����ľ������CameraInit������á�
    '//            piWidth    ����ָ�룬����ͼ��Ŀ��
    '//            piHeight   ����ָ�룬����ͼ��ĸ߶�
    '//            UINT wTimes ץȡͼ��ĳ�ʱʱ�䡣��λ���롣��
    '//						  wTimesʱ���ڻ�δ���ͼ����ú���
    '//						  �᷵�س�ʱ��Ϣ��
    '// ����ֵ   : �ɹ�ʱ������RGB���ݻ��������׵�ַ;
    '//            ���򷵻�0��
    '/******************************************************/
    Public Declare Function CameraGetImageBufferEx Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef piWidth As Integer, _
        ByRef piHeight As Integer, _
        ByVal wTimes As Integer _
    ) As IntPtr

    '/******************************************************/
    '// ������ 	: CameraImageProcessEx
    '// ��������	: ����õ����ԭʼ���ͼ�����ݽ��д������ӱ��Ͷȡ�
    '//			  ��ɫ�����У��������ȴ���Ч�������õ�RGB888
    '//			  ��ʽ��ͼ�����ݡ�	
    '// ����	    : hCamera      ����ľ������CameraInit������á�
    '//            pbyIn	     ����ͼ�����ݵĻ�������ַ������ΪNULL��	
    '//            pbyOut        �����ͼ������Ļ�������ַ������ΪNULL��
    '//            pFrInfo       ����ͼ���֡ͷ��Ϣ��������ɺ�֡ͷ��Ϣ
    '//            uOutFormat    �������ͼ��������ʽ������CAMERA_MEDIA_TYPE_MONO8 CAMERA_MEDIA_TYPE_RGB CAMERA_MEDIA_TYPE_RGBA8������һ�֡�
    '//                          pbyIn��Ӧ�Ļ�������С�������uOutFormatָ���ĸ�ʽ��ƥ�䡣
    '//            uReserved     Ԥ����������������Ϊ0     
    '//					   �е�ͼ���ʽuiMediaType����֮�ı䡣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImageProcessEx Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal pbyIn As IntPtr, _
        ByVal pbyOut As IntPtr, _
        ByRef pFrInfo As tSdkFrameHead, _
        ByVal uOutFormat As Integer, _
        ByVal uReserved As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetStrobeMode
    '// ��������	: ����IO���Ŷ����ϵ�STROBE�źš����źſ���������ƿ��ƣ�Ҳ�������ⲿ��е���ſ��ơ�
    '// ����	    : hCamera ����ľ������CameraInit������á�
    '//             iMode   ��ΪSTROBE_SYNC_WITH_TRIG_AUTO      �ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš�
    '//                                                         ��ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
    '//                     ��ΪSTROBE_SYNC_WITH_TRIG_MANUALʱ���ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)��
    '//                                                         �ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)��
    '//                                                         ��Ч���Կ�����(CameraSetStrobePolarity)��
    '//                     ��ΪSTROBE_ALWAYS_HIGHʱ��STROBE�źź�Ϊ��,������������
    '//                     ��ΪSTROBE_ALWAYS_LOWʱ��STROBE�źź�Ϊ��,������������
    '//
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetStrobeMode Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iMode As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetStrobeMode
    '// ��������	: ���ߵ�ǰSTROBE�ź����õ�ģʽ��
    '// ����	    : hCamera ����ľ������CameraInit������á�
    '//             piMode  ָ�룬����STROBE_SYNC_WITH_TRIG_AUTO,STROBE_SYNC_WITH_TRIG_MANUAL��STROBE_ALWAYS_HIGH����STROBE_ALWAYS_LOW��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetStrobeMode Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef piMode As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetStrobeDelayTime
    '// ��������	: ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Դ����ź���ʱʱ�䡣
    '// ����	    : hCamera       ����ľ������CameraInit������á�
    '//             uDelayTimeUs  ��Դ����źŵ���ʱʱ�䣬��λΪus������Ϊ0��������Ϊ������ 
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetStrobeDelayTime Lib "MVCAMSDK.dll" _
    ( _
    ByVal hCamera As Integer, _
    ByVal uDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetStrobeDelayTime
    '// ��������	: ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú����������Դ����ź���ʱʱ�䡣
    '// ����	    : hCamera           ����ľ������CameraInit������á�
    '//             upDelayTimeUs     ָ�룬������ʱʱ�䣬��λus��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetStrobeDelayTime Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef upDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetStrobePulseWidth
    '// ��������	: ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú��������������ȡ�
    '// ����	    : hCamera       ����ľ������CameraInit������á�
    '//             uTimeUs       ����Ŀ�ȣ���λΪʱ��us��  
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetStrobePulseWidth Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetStrobePulseWidth
    '// ��������	: ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�������������ȡ�
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             upTimeUs  ָ�룬���������ȡ���λΪʱ��us��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetStrobePulseWidth Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef upTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetStrobePolarity
    '// ��������	: ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Ч��ƽ�ļ��ԡ�Ĭ��Ϊ����Ч���������źŵ���ʱ��STROBE�źű����ߡ�
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             iPolarity STROBE�źŵļ��ԣ�0Ϊ�͵�ƽ��Ч��1Ϊ�ߵ�ƽ��Ч��Ĭ��Ϊ�ߵ�ƽ��Ч��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetStrobePolarity Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uPolarity As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetStrobePolarity
    '// ��������	: ��������ǰSTROBE�źŵ���Ч���ԡ�Ĭ��Ϊ�ߵ�ƽ��Ч��
    '// ����	    : hCamera       ����ľ������CameraInit������á�
    '//             ipPolarity    ָ�룬����STROBE�źŵ�ǰ����Ч���ԡ�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetStrobePolarity Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef upPolarity As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetExtTrigSignalType
    '// ��������	: ��������ⴥ���źŵ����ࡣ�ϱ��ء��±��ء����߸ߡ��͵�ƽ��ʽ��
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             iType     �ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
    '//                       emExtTrigSignal���Ͷ��塣

    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetExtTrigSignalType Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetExtTrigSignalType
    '// ��������	: ��������ǰ�ⴥ���źŵ����ࡣ
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             ipType    ָ�룬�����ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
    '//                       emExtTrigSignal���Ͷ��塣
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExtTrigSignalType Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef ipType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetExtTrigShutterType
    '// ��������	: �����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
    '//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             iType     �ⴥ�����ŷ�ʽ���ο�CameraDefine.h��emExtTrigShutterMode���͡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetExtTrigShutterType Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetExtTrigShutterType
    '// ��������	: ����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
    '//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//             ipType    ָ�룬���ص�ǰ�趨���ⴥ�����ŷ�ʽ������ֵ�ο�
    '//                       CameraDefine.h��emExtTrigShutterMode���͡�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExtTrigShutterType Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef ipType As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetExtTrigDelayTime
    '// ��������	: �����ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣 
    '//              �����õ�ֵuDelayTimeUs��Ϊ0ʱ��������յ��ⴥ���źź󣬽���ʱuDelayTimeUs��΢����ٽ���ͼ�񲶻�
    '// ����	    : hCamera       ����ľ������CameraInit������á�
    '//             uDelayTimeUs  ��ʱʱ�䣬��λΪ΢�룬Ĭ��Ϊ0.
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetExtTrigDelayTime Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetExtTrigDelayTime
    '// ��������	: ������õ��ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣 
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//            UINT* upDelayTimeUs
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExtTrigDelayTime Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef upDelayTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraSetExtTrigJitterTime
    '// ��������	: ��������ⴥ���źŵ�����ʱ�䡣Ĭ��Ϊ0����λΪ΢�롣
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//            UINT uTimeUs
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetExtTrigJitterTime Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetExtTrigJitterTime
    '// ��������	: ������õ�����ⴥ������ʱ�䣬Ĭ��Ϊ0.��λΪ΢��
    '// ����	    : hCamera   ����ľ������CameraInit������á�
    '//            UINT* upTimeUs
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExtTrigJitterTime Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef upTimeUs As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraGetExtTrigCapability
    '// ��������	: �������ⴥ������������
    '// ����	    : hCamera           ����ľ������CameraInit������á�
    '//             puCapabilityMask  ָ�룬���ظ�����ⴥ���������룬����ο�CameraDefine.h��
    '//                               EXT_TRIG_MASK_ ��ͷ�ĺ궨�塣   
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Integer
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetExtTrigCapability Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef puCapabilityMask As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraReConnect
    '// ��������	: ���������豸������USB�豸������ߺ�����
    '// ����	    : hCamera	   ����ľ������CameraInit������á� 
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraReConnect Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������ 	: CameraConnectTest
    '// ��������	: �������������״̬�����ڼ������Ƿ����
    '// ����	    : hCamera	   ����ľ������CameraInit������á� 
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraConnectTest Lib "MVCAMSDK.dll" _
    ( _
            ByVal hCamera As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraSetLedBrightness
    '// ��������    : ���������LED���ȣ�����LED���ͺţ��˺������ش�����룬��ʾ��֧�֡�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//             index      LED�Ƶ������ţ���0��ʼ�����ֻ��һ���ɿ������ȵ�LED����ò���Ϊ0 ��
    '//             uBrightness LED����ֵ����Χ0��255. 0��ʾ�رգ�255������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���򷵻� ��0ֵ���ο�CameraStatus.h�д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetLedBrightness Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal index As Integer, _
        ByVal uBrightness As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGetLedBrightness
    '// ��������    : ��������LED���ȣ�����LED���ͺţ��˺������ش�����룬��ʾ��֧�֡�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//             index      LED�Ƶ������ţ���0��ʼ�����ֻ��һ���ɿ������ȵ�LED����ò���Ϊ0 ��
    '//             uBrightness ָ�룬����LED����ֵ����Χ0��255. 0��ʾ�رգ�255������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���򷵻� ��0ֵ���ο�CameraStatus.h�д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetLedBrightness Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal index As Integer, _
        ByRef uBrightness As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraEnableTransferRoi
    '// ��������    : ʹ�ܻ��߽�ֹ����Ķ������书�ܣ������ù��ܵ��ͺţ��˺������ش�����룬��ʾ��֧�֡�
    '//              �ù�����Ҫ����������˽��ɼ������������з֣�ֻ����ָ���Ķ����������ߴ���֡�ʡ�
    '//              ��������䵽PC�Ϻ󣬻��Զ�ƴ�ӳ��������棬û�б�����Ĳ��֣����ú�ɫ��䡣
    '// ����            : hCamera       ����ľ������CameraInit������á�
    '//             uEnableMask ����ʹ��״̬���룬��Ӧ�ı���λΪ1��ʾʹ�ܡ�0Ϊ��ֹ��ĿǰSDK֧��4���ɱ༭����index��ΧΪ0��3����bit0 ��bit1��bit2��bit3����4�������ʹ��״̬��
    '/ ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��
    '//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraEnableTransferRoi Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uEnableMask As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraSetTransferRoi
    '// ��������    : �����������Ĳü�����������ˣ�ͼ��Ӵ������ϱ��ɼ��󣬽��ᱻ�ü���ָ�������������ͣ��˺������ش�����룬��ʾ��֧�֡�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//             index      ROI����������ţ���0��ʼ��
    '//             X1,Y1      ROI��������Ͻ�����
    '//             X2,Y2      ROI��������Ͻ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��
    '//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraSetTransferRoi Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal index As Integer, _
        ByVal X1 As Integer, _
        ByVal Y1 As Integer, _
        ByVal X2 As Integer, _
        ByVal Y2 As Integer _
    ) As Integer


    '/******************************************************/
    '// ������      : CameraGetTransferRoi
    '// ��������    : �����������Ĳü�����������ˣ�ͼ��Ӵ������ϱ��ɼ��󣬽��ᱻ�ü���ָ�������������ͣ��˺������ش�����룬��ʾ��֧�֡�
    '// ����            : hCamera      ����ľ������CameraInit������á�
    '//             index      ROI����������ţ���0��ʼ��
    '//             pX1,pY1      ROI��������Ͻ�����
    '//             pX2,pY2      ROI��������Ͻ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
    '//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��
    '//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGetTransferRoi Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal index As Integer, _
        ByRef pX1 As Integer, _
        ByRef pY1 As Integer, _
        ByRef pX2 As Integer, _
        ByRef pY2 As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraAlignMalloc
    '// ��������    : ����һ�ζ�����ڴ�ռ䡣���ܺ�malloc���ƣ���
    '//                                             �Ƿ��ص��ڴ�����alignָ�����ֽ�������ġ�
    '// ����            : size         �ռ�Ĵ�С��
    '//            align    ��ַ������ֽ�����
    '// ����ֵ   : �ɹ�ʱ�����ط�0ֵ����ʾ�ڴ��׵�ַ��ʧ�ܷ���NULL��
    '/******************************************************/
    Public Declare Function CameraAlignMalloc Lib "MVCAMSDK.dll" _
    ( _
        ByVal size As Integer, _
        ByVal align As Integer _
    ) As IntPtr

    '/******************************************************/
    '// ������      : CameraAlignFree
    '// ��������    : �ͷ���CameraAlignMalloc����������ڴ�ռ䡣
    '// ����            : membuffer    ��CameraAlignMalloc���ص��ڴ��׵�ַ��
    '// ����ֵ   : �ޡ�
    '/******************************************************/
    Public Declare Sub CameraAlignFree Lib "MVCAMSDK.dll" _
    ( _
        ByVal membuffer As IntPtr _
    )


    '//*****************************************************/
    '// ������ 	: CameraSetIspOutFormat
    '// ��������	: ����CameraGetImageBuffer������ͼ����������ʽ��֧��
    '//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
    '//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
    '//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
    '//              Ĭ�������CAMERA_MEDIA_TYPE_BGR8��ʽ��
    '// ����	    : hCamera		����ľ������CameraInit������á� 
    '//             uFormat	Ҫ�趨��ʽ��CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8	��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '//*****************************************************/
    Public Declare Function CameraSetIspOutFormat Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal uFormat As Integer _
    ) As Integer

    '//*****************************************************/
    '// ������ 	: CameraGetIspOutFormat
    '// ��������	: ���CameraGetImageBuffer����ͼ����������ʽ��֧��
    '//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
    '//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
    '//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
    '// ����	    : hCamera		����ľ������CameraInit������á� 
    '//             puFormat	���ص�ǰ�趨�ĸ�ʽ���CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8	��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '//*****************************************************/
    Public Declare Function CameraGetIspOutFormat Lib "MVCAMSDK.dll" _
    ( _
      ByVal hCamera As Integer, _
        ByRef puFormat As Integer _
    ) As Integer

    Public Declare Function CameraGetImageBufferPriority Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef pFrameInfo As tSdkFrameHead, _
        ByRef pbyBuffer As IntPtr, _
        ByVal wTimes As Integer, _
        ByVal Priority As Integer _
    ) As Integer

    Public Declare Function CameraGetImageBufferPriorityEx Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByRef piWidth As Integer, _
        ByRef piHeight As Integer, _
        ByVal wTimes As Integer, _
        ByVal Priority As Integer _
    ) As IntPtr

    Public Declare Function CameraClearBuffer Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer _
    ) As Integer

    Public Declare Function CameraSetInPutIOMode Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iInputIOIndex As Integer, _
        ByVal iMode As Integer _
    ) As Integer

    Public Declare Function CameraSetOutPutIOMode Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iOutputIOIndex As Integer, _
        ByVal iMode As Integer _
    ) As Integer

    Public Declare Function CameraSetOutPutPWM Lib "MVCAMSDK.dll" _
    ( _
        ByVal hCamera As Integer, _
        ByVal iOutputIOIndex As Integer, _
        ByVal iCycle As Integer, _
        ByVal uDuty As Integer _
    ) As Integer

    '/******************************************************/
    '// ������   : CameraGrabber_CreateFromDevicePage
    '// �������� : ��������б����û�ѡ��Ҫ�򿪵����
    '// ����     : �������ִ�гɹ����غ���������Grabber
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_CreateFromDevicePage Lib "MVCAMSDK.dll" _
    ( _
            ByRef Grabber As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������   : CameraGrabber_Create
    '// �������� : ���豸������Ϣ����Grabber
    '// ����     : Grabber    �������ִ�гɹ����غ���������Grabber����
    '//                       pDevInfo      ��������豸������Ϣ����CameraEnumerateDevice������á�
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_Create Lib "MVCAMSDK.dll" _
    ( _
            ByRef Grabber As IntPtr, _
            ByRef pDevInfo As tSdkCameraDevInfo _
    ) As Integer

    '/******************************************************/
    '// ������   : CameraGrabber_Destroy
    '// �������� : ����Grabber
    '// ����     : Grabber
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_Destroy Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_SetHWnd
    '// ��������    : ����Ԥ����Ƶ����ʾ����
    '// ����                : Grabber
    '//                       hWnd  ���ھ��
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SetHWnd Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByVal hWnd As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_StartLive
    '// ��������    : ����Ԥ��
    '// ����                : Grabber
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_StartLive Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_StopLive
    '// ��������    : ֹͣԤ��
    '// ����                : Grabber
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_StopLive Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_SaveImage
    '// ��������    : ץͼ
    '// ����                : Grabber
    '//                       Image ����ץȡ����ͼ����Ҫ����CameraImage_Destroy�ͷţ�
    '//                       TimeOut ��ʱʱ�䣨���룩
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SaveImage Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByRef Image As IntPtr, _
            ByVal TimeOut As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_SaveImageAsync
    '// ��������    : �ύһ���첽��ץͼ�����ύ�ɹ����ץͼ��ɻ�ص��û����õ���ɺ���
    '// ����                : Grabber
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SaveImageAsync Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_SetSaveImageCompleteCallback
    '// ��������    : �����첽��ʽץͼ����ɺ���
    '// ����                : Grabber
    '//                       Callback ����ץͼ�������ʱ������
    '//                       Context ��Callback������ʱ����Ϊ��������Callback
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SetSaveImageCompleteCallback Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByVal pfnCameraGrabberSaveImageComplete As CB_CameraGrabberSaveImageComplete, _
            ByVal Context As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_SetFrameListener
    '// ��������    : ����֡��������
    '// ����                : Grabber
    '//                       Listener �����������˺�������0��ʾ������ǰ֡
    '//                       Context ��Listener������ʱ����Ϊ��������Listener
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SetFrameListener Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByVal pfnCameraGrabberFrameListener As CB_CameraGrabberFrameListener, _
            ByVal Context As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������	: CameraGrabber_SetRawCallback
    '// ��������	: ����RAW�ص�����
    '// ����		: Grabber
    '//			  Callback Raw�ص�����
    '//			  Context ��Callback������ʱ����Ϊ��������Callback
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SetRawCallback Lib "MVCAMSDK.dll" _
    ( _
     ByVal Grabber As IntPtr, _
           ByVal pfnCameraGrabberFrameCallback As CB_CameraGrabberFrameCallback, _
     ByVal Context As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������	: CameraGrabber_SetRGBCallback
    '// ��������	: ����RGB�ص�����
    '// ����		: Grabber
    '//			  Callback RGB�ص�����
    '//			  Context ��Callback������ʱ����Ϊ��������Callback
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_SetRGBCallback Lib "MVCAMSDK.dll" _
    ( _
     ByVal Grabber As IntPtr, _
           ByVal pfnCameraGrabberFrameCallback As CB_CameraGrabberFrameCallback, _
     ByVal Context As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_GetCameraHandle
    '// ��������    : ��ȡ������
    '// ����                : Grabber
    '//                       hCamera ���ص�������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_GetCameraHandle Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByRef CameraHandle As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_GetStat
    '// ��������    : ��ȡ֡ͳ����Ϣ
    '// ����                : Grabber
    '//                       stat ���ص�ͳ����Ϣ
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_GetStat Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByRef stat As tSdkGrabberStat _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraGrabber_GetCameraDevInfo
    '// ��������    : ��ȡ���DevInfo
    '// ����                : Grabber
    '//                       DevInfo ���ص����DevInfo
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraGrabber_GetCameraDevInfo Lib "MVCAMSDK.dll" _
    ( _
            ByVal Grabber As IntPtr, _
            ByRef DevInfo As tSdkCameraDevInfo _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_Create
    '// ��������    : ����һ���µ�Image
    '// ����                : Image
    '//                       pFrameBuffer ֡���ݻ�����
    '//                       pFrameHead ֡ͷ
    '//                       bCopy TRUE: ���Ƴ�һ���µ�֡����  FALSE: �����ƣ�ֱ��ʹ��pFrameBufferָ��Ļ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_Create Lib "MVCAMSDK.dll" _
    ( _
            ByRef Image As IntPtr, _
            ByVal pFrameBuffer As IntPtr, _
            ByRef pFrameHead As tSdkFrameHead, _
            ByVal bCopy As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_Destroy
    '// ��������    : ����Image
    '// ����                : Image
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_Destroy Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_GetData
    '// ��������    : ��ȡImage����
    '// ����                : Image
    '//                       DataBuffer ͼ������
    '//                       Head ͼ����Ϣ
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_GetData Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef DataBuffer As IntPtr, _
            ByRef head As IntPtr _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_Draw
    '// ��������    : ����Image��ָ������
    '// ����                : Image
    '//                       hWnd Ŀ�Ĵ���
    '//                       Algorithm �����㷨  0�����ٵ������Բ�  1���ٶ�����������
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_Draw Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByVal hWnd As IntPtr, _
            ByVal Algorithm As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_BitBlt
    '// ��������    : ����Image��ָ�����ڣ������ţ�
    '// ����                : Image
    '//                       hWnd Ŀ�Ĵ���
    '//                       xDst,yDst: Ŀ����ε����Ͻ�����
    '//                       cxDst,cyDst: Ŀ����εĿ��
    '//                       xSrc,ySrc: ͼ����ε����Ͻ�����
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_BitBlt Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByVal hWnd As IntPtr, _
            ByVal xDst As Integer, _
            ByVal yDst As Integer, _
            ByVal cxDst As Integer, _
            ByVal cyDst As Integer, _
            ByVal xSrc As Integer, _
            ByVal ySrc As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_SaveAsBmp
    '// ��������    : ��bmp��ʽ����Image
    '// ����                : Image
    '//                       FileName �ļ���
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_SaveAsBmp Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef lpszFileName As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_SaveAsJpeg
    '// ��������    : ��jpg��ʽ����Image
    '// ����                : Image
    '//                       FileName �ļ���
    '//                       Quality ��������(1-100)��100Ϊ������ѵ��ļ�Ҳ���
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_SaveAsJpeg Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef lpszFileName As Byte, _
            ByVal Quality As Integer _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_SaveAsPng
    '// ��������    : ��png��ʽ����Image
    '// ����                : Image
    '//                       FileName �ļ���
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_SaveAsPng Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef lpszFileName As Byte _
    ) As Integer

    '/******************************************************/
    '// ������      : CameraImage_SaveAsRaw
    '// ��������    : ����raw Image
    '// ����                : Image
    '//                       FileName �ļ���
    '//                       Format 0: 8Bit Raw     1: 16Bit Raw
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_SaveAsRaw Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef lpszFileName As Byte, _
            ByVal Format As Integer _
    ) As Integer

    '/******************************************************/
    '// ������	: CameraImage_IPicture
    '// ��������	: ��Image����һ��IPicture
    '// ����		: Image
    '//			  Picture �´�����IPicture
    '// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
    '//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
    '//            �д�����Ķ��塣
    '/******************************************************/
    Public Declare Function CameraImage_IPicture Lib "MVCAMSDK.dll" _
    ( _
            ByVal Image As IntPtr, _
            ByRef NewPic As IntPtr _
    ) As Integer

End Module