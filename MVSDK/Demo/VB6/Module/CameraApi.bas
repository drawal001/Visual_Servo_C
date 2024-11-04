Attribute VB_Name = "CameraApi"
'///BIG5 TRANS ALLOWED
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
    ByVal iLanguageSel As Long _
) As Long

'/******************************************************/
'// ������      : CameraEnumerateDevice
'// ��������    : ö���豸���������豸�б��ڵ���CameraInit
'//                       ֮ǰ��������øú���������豸����Ϣ��
'// ����       : pCameraList �豸�б�����ָ�롣
'//              piNums        �豸�ĸ���ָ�룬����ʱ����pCameraList
'//                            �����Ԫ�ظ�������������ʱ������ʵ���ҵ����豸������
'//              ע�⣬piNumsָ���ֵ�����ʼ�����Ҳ�����pCameraList����Ԫ�ظ�����
'//              �����п�������ڴ������
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraEnumerateDevice Lib "MVCAMSDK.dll" _
( _
        ByRef pCameraList As tSdkCameraDevInfo, _
        ByRef piNums As Long _
) As Long


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
        ByVal iParamLoadMode As Long, _
        ByVal emTeam As Long, _
        ByRef pCameraHandle As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal pCallBack As Long, _
        ByVal pContext As Long, _
        ByVal pCallBack As Long _
) As Long

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
        ByVal hCamera As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByRef sDescribe As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal pbyIn As Long, _
        ByVal pbyOut As Long, _
        ByRef pFrInfo As tSdkFrameHead _
) As Long

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
        ByVal hCamera As Long, _
        ByVal hWndDisplay As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal pbyRGB24 As Long, _
        ByRef pFrInfo As tSdkFrameHead _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iWidth As Long, _
        ByVal iHeight As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pFrameInfo As tSdkFrameHead, _
    ByRef pbyBuffer As Long, _
    ByVal wTimes As Long _
) As Long


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
    ByVal hCamera As Long, _
    ByRef pFrameInfo As tSdkFrameHead, _
    ByRef pbyBuffer As Long, _
    ByVal uWaitTimeMs As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByVal pbyBuffer As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long
 
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
    ByVal hCamera As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByVal iFormat As Long, _
    ByRef pcSavePath As Byte, _
    ByVal b2GLimit As Long, _
    ByVal dwQuality As Long, _
    ByVal iFrameRate As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal pbyImageBuffer As Long, _
    ByRef pFrInfo As tSdkFrameHead _
) As Long

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
        ByVal hCamera As Long, _
    ByRef lpszFileName As Byte, _
    ByVal pbyImageBuffer As Long, _
    ByRef pFrInfo As tSdkFrameHead, _
    ByVal byFileType As Byte, _
    ByVal byQuality As Byte _
) As Long

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
    ByVal hCamera As Long, _
    ByRef psCurVideoSize As tSdkImageResolution _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pImageResolution As tSdkImageResolution _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piMediaType As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iMediaType As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bAeState As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pAeState As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iSharpness As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piSharpness As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal emLutMode As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pemLutMode As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iChannel As Long, _
    ByVal pLut As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iChannel As Long, _
    ByVal pLut As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iChannel As Long, _
    ByVal pLut As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bAuto As Long _
) As Long

'/******************************************************/
'// ������      : CameraGetWbMode
'// ��������    : ��õ�ǰ�İ�ƽ��ģʽ��
'// ����            : hCamera    ����ľ������CameraInit������á�
'//            pbAuto    ָ�룬����TRUE��ʾ�Զ�ģʽ��FALSE
'//                                              Ϊ�ֶ�ģʽ��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetWbMode Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
    ByRef pbAuto As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piSel As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long


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
        ByVal hCamera As Long, _
    ByVal iAeTarget As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piAeTarget As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal fExposureTime As Double _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pfLineTime As Double _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pfExposureTime As Double _
) As Long

'/******************************************************/
'// ������   : CameraGetExposureTimeRange
'// �������� : ���������ع�ʱ�䷶Χ
'// ����     : hCamera        ����ľ������CameraInit������á�
'//            pfMin                    ָ�룬�����ع�ʱ�����Сֵ����λ΢�롣
'//            pfMax                    ָ�룬�����ع�ʱ������ֵ����λ΢�롣
'//            pfStep           ָ�룬�����ع�ʱ��Ĳ���ֵ����λ΢�롣
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExposureTimeRange Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef pfMin As Double, _
        ByRef pfMax As Double, _
        ByRef pfStep As Double _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iAnalogGain As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piAnalogGain As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iRGain As Long, _
    ByVal iGGain As Long, _
    ByVal iBGain As Long _
) As Long


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
        ByVal hCamera As Long, _
    ByRef piRGain As Long, _
    ByRef piGGain As Long, _
    ByRef piBGain As Long _
) As Long


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
        ByVal hCamera As Long, _
    ByVal iGamma As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piGamma As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iContrast As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piContrast As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iSaturation As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piSaturation As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piFrequencySel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iFrequencySel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iFrameSpeed As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piFrameSpeed As Long _
) As Long


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
        ByVal hCamera As Long, _
    ByVal iTarget As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piMode As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal uMask As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iTeam As Long _
) As Long

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
Public Declare Function CameraSaveParameterToFile Lib "MVCAMSDK.dll" _
( _
	ByVal hCamera As Long, _
	ByRef sFileName As Byte _
)  As Long

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
Public Declare Function CameraReadParameterFromFile Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
    ByRef sFileName As Byte _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iTeam As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piTeam As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iPackSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piPackSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbIsVisible As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bIsVisible As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piHOff As Long, _
    ByRef piVOff As Long, _
    ByRef piWidth As Long, _
    ByRef piHeight As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iHOff As Long, _
    ByVal iVOff As Long, _
    ByVal iWidth As Long, _
    ByVal iHeight As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iDir As Long, _
    ByVal bEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iDir As Long, _
    ByRef pbEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piHOff As Long, _
    ByRef piVOff As Long, _
    ByRef piWidth As Long, _
    ByRef piHeight As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iHOff As Long, _
    ByVal iVOff As Long, _
    ByVal iWidth As Long, _
    ByVal iHeight As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbShow As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bShow As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal pRgbBuffer As Long, _
    ByRef pFrInfo As tSdkFrameHead _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iLine As Long, _
    ByVal x As Long, _
    ByVal y As Long, _
    ByVal uColor As Long, _
    ByVal bVisible As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iLine As Long, _
    ByRef px As Long, _
    ByRef py As Long, _
    ByRef pcolor As Long, _
    ByRef pbVisible As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pCameraInfo As tSdkCameraCapbility _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pbySN As Byte, _
    ByVal iLevel As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pbySN As Byte, _
    ByVal iLevel As Long _
) As Long
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
        ByVal hCamera As Long, _
    ByVal uDelayTimeUs As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef puDelayTimeUs As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iCount As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piCount As Long _
) As Long

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
    ByVal hCamera As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iModeSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piModeSel As Long _
) As Long


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
    ByVal hCamera As Long, _
    ByRef pImageResolution As tSdkImageResolution _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pImageResolution As tSdkImageResolution _
) As Long

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
    ByVal hCamera As Long, _
    ByRef pImageCustom As tSdkImageResolution _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iWinType As Long, _
    ByVal hParent As Long, _
    ByRef piHOff As Long, _
    ByRef piVOff As Long, _
    ByRef piWidth As Long, _
    ByRef piHeight As Long _
) As Long


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
        ByVal hCamera As Long, _
        ByVal hParent As Long, _
        ByRef pWinText As Byte, _
        ByVal pCallbackFunc As Long, _
        ByVal pCallbackCtx As Long, _
        ByVal uReserved As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal bShow As Long _
) As Long


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
        ByVal hCamera As Long, _
    ByVal index As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal dwCtrlCode As Long, _
    ByVal dwParam As Long, _
    ByVal lpData As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByRef psFrameStatistic As tSdkFrameStatistic _
) As Long

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
        ByVal hCamera As Long, _
    ByVal bEnable As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef pEnable As Long _
) As Long

'/******************************************************/
'// ������      : CameraRstTimeStamp
'// ��������    : ��λͼ��ɼ���ʱ�������0��ʼ��
'// ����            : ByVal hCamera As Long, _
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraRstTimeStamp Lib "MVCAMSDK.dll" _
( _
    ByVal hCamera As Long _
) As Long

 
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
    ByVal hCamera As Long, _
    ByVal uStartAddr As Long, _
    ByVal pbData As Long, _
    ByVal ilen As Long _
) As Long


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
    ByVal hCamera As Long, _
    ByVal uStartAddr As Long, _
    ByVal pbData As Long, _
    ByVal ilen As Long _
) As Long


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
        ByVal hCamera As Long, _
        ByRef pName As Byte _
) As Long


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
        ByVal hCamera As Long, _
        ByRef pName As Byte _
) As Long

'/******************************************************/
'// ������      : CameraSdkGetVersionString
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
) As Long

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
        ByVal hCamera As Long, _
        ByRef pNeedUpdate As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByRef pVersion As Byte _
) As Long

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
        ByVal hCamera As Long, _
        ByRef pCameraInfo As tSdkCameraDevInfo _
) As Long

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
        ByVal hCamera As Long, _
        ByRef pVersion As Byte _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iOutputIOIndex As Long, _
        ByVal uState As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iInputIOIndex As Long, _
        ByRef puState As Long _
) As Long

'/******************************************************/
'// ������   : CameraSetInPutIOMode
'// �������� : ��������IO��ģʽ�����
'//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
'//              iInputIoCounts������
'// ����     : hCamera ����ľ������CameraInit������á�          
'//            iInputIOIndex IO�������ţ���0��ʼ��
'//            iMode IOģʽ,�ο�CameraDefine.h��emCameraGPIOMode
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetInPutIOMode Lib "MVCAMSDK.dll" _
( _
	ByVal hCamera As Long, _
	ByVal iInputIOIndex As Long, _
	ByVal iMode As Long _
) As Long

'/******************************************************/
'// ������   : CameraSetOutPutIOMode
'// �������� : �������IO��ģʽ�����
'//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
'//              iOutputIoCounts������
'// ����     : hCamera ����ľ������CameraInit������á�          
'//            iOutputIOIndex IO�������ţ���0��ʼ��
'//            iMode IOģʽ,�ο�CameraDefine.h��emCameraGPIOMode
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetOutPutIOMode Lib "MVCAMSDK.dll" _
( _
	ByVal hCamera As Long, _
	ByVal iOutputIOIndex As Long, _
	ByVal iMode As Long _
) As Long

'/******************************************************/
'// ������   : CameraSetOutPutPWM
'// �������� : ����PWM������Ĳ��������
'//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
'//              iOutputIoCounts������
'// ����     : hCamera ����ľ������CameraInit������á�          
'//            iOutputIOIndex IO�������ţ���0��ʼ��
'//            iCycle PWM�����ڣ���λ(us)
'//			  uDuty  ռ�ñȣ�ȡֵ1%~99%
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetOutPutPWM Lib "MVCAMSDK.dll" _
( _
	ByVal hCamera As Long, _
	ByVal iOutputIOIndex As Long, _
	ByVal iCycle As Long, _
	ByVal uDuty As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iIspProcessor As Long, _
    ByVal iAeAlgorithmSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iIspProcessor As Long, _
    ByRef piAlgorithmSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iIspProcessor As Long, _
    ByVal iAlgorithmSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iIspProcessor As Long, _
    ByRef piAlgorithmSel As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByVal iIspProcessor As Long _
) As Long

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
        ByVal hCamera As Long, _
    ByRef piIspProcessor As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iBlackLevel As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByRef piBlackLevel As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByVal iWhiteLevel As Long _
) As Long

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
        ByVal hCamera As Long, _
        ByRef piWhiteLevel As Long _
) As Long

'/******************************************************/
'// ������  : CameraEnumerateDeviceEx
'// ��������    : ö���豸���������豸�б��ڵ���CameraInitEx
'//           ֮ǰ��������øú���ö���豸��
'// ����         :
'// ����ֵ     : �����豸������0��ʾ�ޡ�
'/******************************************************/
Public Declare Function CameraEnumerateDeviceEx Lib "MVCAMSDK.dll" _
( _
) As Long

'/******************************************************/
'// ������  : CameraInitEx
'// ��������    : �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
'//           �����صĲ����ӿڡ�
'// ����        : iDeviceIndex    ����������ţ�CameraEnumerateDeviceEx�������������
'//            iParamLoadMode  �����ʼ��ʱʹ�õĲ������ط�ʽ��-1��ʾʹ���ϴ��˳�ʱ�Ĳ������ط�ʽ��
'//            emTeam         ��ʼ��ʱʹ�õĲ����顣-1��ʾ�����ϴ��˳�ʱ�Ĳ����顣
'//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
'//                          ���ظ��������Ч������ڵ����������
'//                          ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
'//                          ���ڶ����֮������֡�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraInitEx Lib "MVCAMSDK.dll" _
( _
        ByVal iDeviceIndex As Long, _
        ByVal iParamLoadMode As Long, _
        ByVal emTeam As Long, _
        ByRef pCameraHandle As Long _
) As Long

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
Public Declare Function CameraInitEx2 Lib "MVCAMSDK.dll" _
( _
        ByRef CameraName As Byte, _
        ByRef pCameraHandle As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetImageBufferEx
'// ��������    : ���һ֡ͼ�����ݡ��ýӿڻ�õ�ͼ���Ǿ���������RGB��ʽ���ú������ú�
'//           ����Ҫ���� CameraReleaseImageBuffer �ͷţ�Ҳ��Ҫ����free֮��ĺ����ͷ�
'//              ���ͷŸú������ص�ͼ�����ݻ�������
'// ����        : hCamera     ����ľ������CameraInit������á�
'//            piWidth    ����ָ�룬����ͼ��Ŀ��
'//            piHeight   ����ָ�룬����ͼ��ĸ߶�
'//            UINT wTimes ץȡͼ��ĳ�ʱʱ�䡣��λ���롣��
'//                       wTimesʱ���ڻ�δ���ͼ����ú���
'//                       �᷵�س�ʱ��Ϣ��
'// ����ֵ   : �ɹ�ʱ������RGB���ݻ��������׵�ַ;
'//            ���򷵻�0��
'/******************************************************/
Public Declare Function CameraGetImageBufferEx Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef piWidth As Long, _
        ByRef piHeight As Long, _
        ByVal wTimes As Long _
) As Long

'/******************************************************/
'// ������  : CameraImageProcessEx
'// ��������    : ����õ����ԭʼ���ͼ�����ݽ��д������ӱ��Ͷȡ�
'//           ��ɫ�����У��������ȴ���Ч�������õ�RGB888
'//           ��ʽ��ͼ�����ݡ�
'// ����        : hCamera      ����ľ������CameraInit������á�
'//            pbyIn         ����ͼ�����ݵĻ�������ַ������ΪNULL��
'//            pbyOut        �����ͼ������Ļ�������ַ������ΪNULL��
'//            pFrInfo       ����ͼ���֡ͷ��Ϣ��������ɺ�֡ͷ��Ϣ
'//            uOutFormat    �������ͼ��������ʽ������CAMERA_MEDIA_TYPE_MONO8 CAMERA_MEDIA_TYPE_RGB CAMERA_MEDIA_TYPE_RGBA8������һ�֡�
'//                          pbyIn��Ӧ�Ļ�������С�������uOutFormatָ���ĸ�ʽ��ƥ�䡣
'//            uReserved     Ԥ����������������Ϊ0
'//                    �е�ͼ���ʽuiMediaType����֮�ı䡣
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraImageProcessEx Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal pbyIn As Long, _
        ByVal pbyOut As Long, _
        ByRef pFrInfo As tSdkFrameHead, _
        ByVal uOutFormat As Long, _
        ByVal uReserved As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetStrobeMode
'// ��������    : ����IO���Ŷ����ϵ�STROBE�źš����źſ���������ƿ��ƣ�Ҳ�������ⲿ��е���ſ��ơ�
'// ����        : hCamera ����ľ������CameraInit������á�
'//             iMode   ��ΪSTROBE_SYNC_WITH_TRIG_AUTO      �ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš�
'//                                                         ��ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
'//                     ��ΪSTROBE_SYNC_WITH_TRIG_MANUALʱ���ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)��
'//                                                         �ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)��
'//                                                         ��Ч���Կ�����(CameraSetStrobePolarity)��
'//                     ��ΪSTROBE_ALWAYS_HIGHʱ��STROBE�źź�Ϊ��,������������
'//                     ��ΪSTROBE_ALWAYS_LOWʱ��STROBE�źź�Ϊ��,������������
'//
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetStrobeMode Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal iMode As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetStrobeMode
'// ��������    : ���ߵ�ǰSTROBE�ź����õ�ģʽ��
'// ����        : hCamera ����ľ������CameraInit������á�
'//             piMode  ָ�룬����STROBE_SYNC_WITH_TRIG_AUTO,STROBE_SYNC_WITH_TRIG_MANUAL��STROBE_ALWAYS_HIGH����STROBE_ALWAYS_LOW��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetStrobeMode Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef piMode As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetStrobeDelayTime
'// ��������    : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Դ����ź���ʱʱ�䡣
'// ����        : hCamera       ����ľ������CameraInit������á�
'//             uDelayTimeUs  ��Դ����źŵ���ʱʱ�䣬��λΪus������Ϊ0��������Ϊ������
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetStrobeDelayTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal uDelayTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetStrobeDelayTime
'// ��������    : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú����������Դ����ź���ʱʱ�䡣
'// ����        : hCamera           ����ľ������CameraInit������á�
'//             upDelayTimeUs     ָ�룬������ʱʱ�䣬��λus��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetStrobeDelayTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef upDelayTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetStrobePulseWidth
'// ��������    : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú��������������ȡ�
'// ����        : hCamera       ����ľ������CameraInit������á�
'//             uTimeUs       ����Ŀ�ȣ���λΪʱ��us��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetStrobePulseWidth Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal uTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetStrobePulseWidth
'// ��������    : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�������������ȡ�
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             upTimeUs  ָ�룬���������ȡ���λΪʱ��us��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetStrobePulseWidth Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef upTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetStrobePolarity
'// ��������    : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Ч��ƽ�ļ��ԡ�Ĭ��Ϊ����Ч���������źŵ���ʱ��STROBE�źű����ߡ�
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             iPolarity STROBE�źŵļ��ԣ�0Ϊ�͵�ƽ��Ч��1Ϊ�ߵ�ƽ��Ч��Ĭ��Ϊ�ߵ�ƽ��Ч��
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetStrobePolarity Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal uPolarity As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetStrobePolarity
'// ��������    : ��������ǰSTROBE�źŵ���Ч���ԡ�Ĭ��Ϊ�ߵ�ƽ��Ч��
'// ����        : hCamera       ����ľ������CameraInit������á�
'//             ipPolarity    ָ�룬����STROBE�źŵ�ǰ����Ч���ԡ�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetStrobePolarity Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef upPolarity As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetExtTrigSignalType
'// ��������    : ��������ⴥ���źŵ����ࡣ�ϱ��ء��±��ء����߸ߡ��͵�ƽ��ʽ��
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             iType     �ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
'//                       emExtTrigSignal���Ͷ��塣

'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetExtTrigSignalType Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal iType As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetExtTrigSignalType
'// ��������    : ��������ǰ�ⴥ���źŵ����ࡣ
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             ipType    ָ�룬�����ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
'//                       emExtTrigSignal���Ͷ��塣
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExtTrigSignalType Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef ipType As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetExtTrigShutterType
'// ��������    : �����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
'//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             iType     �ⴥ�����ŷ�ʽ���ο�CameraDefine.h��emExtTrigShutterMode���͡�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetExtTrigShutterType Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal iType As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetExtTrigShutterType
'// ��������    : ����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
'//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
'// ����        : hCamera   ����ľ������CameraInit������á�
'//             ipType    ָ�룬���ص�ǰ�趨���ⴥ�����ŷ�ʽ������ֵ�ο�
'//                       CameraDefine.h��emExtTrigShutterMode���͡�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExtTrigShutterType Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef ipType As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetExtTrigDelayTime
'// ��������    : �����ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣
'//              �����õ�ֵuDelayTimeUs��Ϊ0ʱ��������յ��ⴥ���źź󣬽���ʱuDelayTimeUs��΢����ٽ���ͼ�񲶻�
'// ����        : hCamera       ����ľ������CameraInit������á�
'//             uDelayTimeUs  ��ʱʱ�䣬��λΪ΢�룬Ĭ��Ϊ0.
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetExtTrigDelayTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal uDelayTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetExtTrigDelayTime
'// ��������    : ������õ��ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣
'// ����        : hCamera   ����ľ������CameraInit������á�
'//            UINT* upDelayTimeUs
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExtTrigDelayTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef upDelayTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraSetExtTrigJitterTime
'// ��������    : ��������ⴥ���źŵ�����ʱ�䡣Ĭ��Ϊ0����λΪ΢�롣
'// ����        : hCamera   ����ľ������CameraInit������á�
'//            UINT uTimeUs
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraSetExtTrigJitterTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByVal uTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetExtTrigJitterTime
'// ��������    : ������õ�����ⴥ������ʱ�䣬Ĭ��Ϊ0.��λΪ΢��
'// ����        : hCamera   ����ľ������CameraInit������á�
'//            UINT* upTimeUs
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExtTrigJitterTime Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef upTimeUs As Long _
) As Long

'/******************************************************/
'// ������  : CameraGetExtTrigCapability
'// ��������    : �������ⴥ������������
'// ����        : hCamera           ����ľ������CameraInit������á�
'//             puCapabilityMask  ָ�룬���ظ�����ⴥ���������룬����ο�CameraDefine.h��
'//                               EXT_TRIG_MASK_ ��ͷ�ĺ궨�塣
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0) As Long
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraGetExtTrigCapability Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long, _
        ByRef puCapabilityMask As Long _
) As Long



'/******************************************************/
'// ������      : CameraReConnect
'// ��������    : ���������豸������USB�豸������ߺ�����
'// ����            : hCamera      ����ľ������CameraInit������á�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraReConnect Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long _
) As Long

'/******************************************************/
'// ������      : CameraConnectTest
'// ��������    : �������������״̬�����ڼ������Ƿ����
'// ����            : hCamera      ����ľ������CameraInit������á�
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.bas
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraConnectTest Lib "MVCAMSDK.dll" _
( _
        ByVal hCamera As Long _
) As Long



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
    ByVal hCamera As Long, _
    ByVal index As Long, _
    ByVal uBrightness As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByVal index As Long, _
    ByRef uBrightness As Long _
) As Long

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
    ByVal hCamera As Long, _
    ByVal uEnableMask As Long _
) As Long


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
    ByVal hCamera As Long, _
    ByVal index As Long, _
    ByVal X1 As Long, _
    ByVal Y1 As Long, _
    ByVal X2 As Long, _
    ByVal Y2 As Long _
) As Long


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
    ByVal hCamera As Long, _
    ByVal index As Long, _
    ByRef pX1 As Long, _
    ByRef pY1 As Long, _
    ByRef pX2 As Long, _
    ByRef pY2 As Long _
) As Long

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
    ByVal size As Long, _
    ByVal align As Long _
) As Long
    
'/******************************************************/
'// ������      : CameraAlignFree
'// ��������    : �ͷ���CameraAlignMalloc����������ڴ�ռ䡣
'// ����            : membuffer    ��CameraAlignMalloc���ص��ڴ��׵�ַ��
'// ����ֵ   : �ޡ�
'/******************************************************/
Public Declare Sub CameraAlignFree Lib "MVCAMSDK.dll" _
( _
    ByVal membuffer As Long _
)


'//*****************************************************/
'// ������      : CameraSetIspOutFormat
'// ��������    : ����CameraGetImageBuffer������ͼ����������ʽ��֧��
'//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
'//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
'//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
'//              Ĭ�������CAMERA_MEDIA_TYPE_BGR8��ʽ��
'// ����            : hCamera           ����ľ������CameraInit������á�
'//             uFormat Ҫ�趨��ʽ��CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8  ��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'//*****************************************************/
Public Declare Function CameraSetIspOutFormat Lib "MVCAMSDK.dll" _
( _
    ByVal hCamera As Long, _
    ByVal uFormat As Long _
) As Long

'//*****************************************************/
'// ������      : CameraGetIspOutFormat
'// ��������    : ���CameraGetImageBuffer����ͼ����������ʽ��֧��
'//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
'//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
'//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
'// ����            : hCamera           ����ľ������CameraInit������á�
'//             puFormat        ���ص�ǰ�趨�ĸ�ʽ���CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8 ��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'//*****************************************************/
Public Declare Function CameraGetIspOutFormat Lib "MVCAMSDK.dll" _
( _
                ByVal hCamera As Long, _
    ByRef puFormat As Long _
) As Long

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
        ByRef Grabber As Long _
) As Long

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
        ByRef Grabber As Long, _
        ByRef pDevInfo As tSdkCameraDevInfo _
) As Long

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
        ByVal Grabber As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByVal hWnd As Long _
) As Long

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
        ByVal Grabber As Long _
) As Long

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
        ByVal Grabber As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByRef Image As Long, _
        ByVal TimeOut As Long _
) As Long

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
        ByVal Grabber As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByVal pfnCameraGrabberSaveImageComplete As Long, _
        ByVal Context As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByVal pfnCameraGrabberFrameListener As Long, _
        ByVal Context As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByRef CameraHandle As Long _
) As Long

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
        ByVal Grabber As Long, _
        ByRef stat As tSdkGrabberStat _
) As Long

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
        ByVal Grabber As Long, _
        ByRef DevInfo As tSdkCameraDevInfo _
) As Long

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
        ByRef Image As Long, _
        ByVal pFrameBuffer As Long, _
        ByRef pFrameHead As tSdkFrameHead, _
        ByVal bCopy As Long _
) As Long

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
        ByVal Image As Long _
) As Long

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
        ByVal Image As Long, _
        ByRef DataBuffer As Long, _
        ByRef head As Long _
) As Long

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
        ByVal Image As Long, _
        ByVal hWnd As Long, _
        ByVal Algorithm As Long _
) As Long

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
        ByVal Image As Long, _
        ByVal hWnd As Long, _
        ByVal xDst As Long, _
        ByVal yDst As Long, _
        ByVal cxDst As Long, _
        ByVal cyDst As Long, _
        ByVal xSrc As Long, _
        ByVal ySrc As Long _
) As Long

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
        ByVal Image As Long, _
        ByRef lpszFileName As Byte _
) As Long

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
        ByVal Image As Long, _
        ByRef lpszFileName As Byte, _
        ByVal Quality As Long _
) As Long

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
        ByVal Image As Long, _
        ByRef lpszFileName As Byte _
) As Long

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
        ByVal Image As Long, _
        ByRef lpszFileName As Byte, _
        ByVal Format As Long _
) As Long

'/******************************************************/
'// ������      : CameraImage_IPicture
'// ��������    : ��Image����һ��IPicture
'// ����                : Image
'//                       Picture �´�����IPicture
'// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
'//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
'//            �д�����Ķ��塣
'/******************************************************/
Public Declare Function CameraImage_IPicture Lib "MVCAMSDK.dll" _
( _
        ByVal Image As Long, _
        ByRef NewPic As IPictureDisp _
) As Long
