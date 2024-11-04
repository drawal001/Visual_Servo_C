unit CameraApi;
//BIG5 TRANS ALLOWED
interface
uses CameraDefine;
///****************************************************/
// ������ 	: CameraSdkInit
// ��������	: ���SDK��ʼ�����ڵ����κ�SDK�����ӿ�ǰ������
//			  �ȵ��øýӿڽ��г�ʼ�����ú�����������������
//			  �ڼ�ֻ��Ҫ����һ�Ρ�	 
// ����	    : iLanguageSel ����ѡ��SDK�ڲ���ʾ��Ϣ�ͽ��������,
//						   0:��ʾӢ��,1:��ʾ���ġ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSdkInit(
    iLanguageSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL' name 'CameraSdkInit'


///****************************************************/
// ������ 	: CameraEnumerateDevice
// ��������	: ö���豸���������豸�б��ڵ���CameraInit
//			  ֮ǰ��������øú���������豸����Ϣ��		
// ����	    : pCameraList �豸�б�����ָ�롣
//             piNums        �豸�ĸ���ָ�룬����ʱ����pCameraList
//                            �����Ԫ�ظ�������������ʱ������ʵ���ҵ����豸������
//              ע�⣬piNumsָ���ֵ�����ʼ�����Ҳ�����pCameraList����Ԫ�ظ�����
//              �����п�������ڴ������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraEnumerateDevice(
    pCameraList:PtSdkCameraDevInfo;
    piNums:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraInit
// ��������	: �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
//			  �����صĲ����ӿڡ�		
// ����	    : pCameraInfo    ��������豸������Ϣ����CameraEnumerateDevice
//							 ������á�	
//            iParamLoadMode  �����ʼ��ʱʹ�õĲ������ط�ʽ��-1��ʾʹ���ϴ��˳�ʱ�Ĳ������ط�ʽ��
//            emTeam         ��ʼ��ʱʹ�õĲ����顣-1��ʾ�����ϴ��˳�ʱ�Ĳ����顣
//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
//							 ���ظ��������Ч������ڵ����������
//							 ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
//							 ���ڶ����֮������֡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraInit(
    pCameraInfo:PtSdkCameraDevInfo;
    iParamLoadMode:Integer;
    emTeam:Integer;
    pCameraHandle:PCameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetCallbackFunction
// ��������	: ����ͼ�񲶻�Ļص��������������µ�ͼ������֡ʱ��
//			  pCallBack��ָ��Ļص������ͻᱻ���á�	
// ����	    : hCamera	����ľ������CameraInit������á�
//            pCallBack �ص�����ָ�롣
//            pContext  �ص������ĸ��Ӳ������ڻص�����������ʱ
//						�ø��Ӳ����ᱻ���룬����ΪNULL��������
//						������ʱЯ��������Ϣ��
//            pCallbackOld  ���ڱ��浱ǰ�Ļص�����������ΪNULL��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetCallbackFunction(
    hCamera:CameraHandle;
    pCallBack:CAMERA_SNAP_PROC;
    pContext:Pointer;
    pCallbackOld:CAMERA_SNAP_PROC
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraUnInit
// ��������	: �������ʼ�����ͷ���Դ��
// ����	    : hCamera ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraUnInit(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetInformation
// ��������	: ��������������Ϣ
// ����	    : hCamera ����ľ������CameraInit������á�
//            pbuffer ָ�����������Ϣָ���ָ�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetInformation(
    hCamera:CameraHandle;
    pbuffer:Ppchar
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraImageProcess
// ��������	: ����õ����ԭʼ���ͼ�����ݽ��д������ӱ��Ͷȡ�
//			  ��ɫ�����У��������ȴ���Ч�������õ�RGB888
//			  ��ʽ��ͼ�����ݡ�	
// ����	    : hCamera  ����ľ������CameraInit������á�
//            pbyIn	   ����ͼ�����ݵĻ�������ַ������ΪNULL��	
//            pbyOut   �����ͼ������Ļ�������ַ������ΪNULL��
//            pFrInfo  ����ͼ���֡ͷ��Ϣ��������ɺ�֡ͷ��Ϣ
//					   �е�ͼ���ʽuiMediaType����֮�ı䡣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraImageProcess(
    hCamera:CameraHandle; 
    pbyIn:PByte;
    pbyOut:PByte;
    pFrInfo:PtSdkFrameHead
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraImageProcessEx
// �������� : ����õ����ԭʼ���ͼ�����ݽ��д������ӱ��Ͷȡ�
//            ��ɫ�����У��������ȴ���Ч�������õ�RGB888
//            ��ʽ��ͼ�����ݡ�  
// ����     : hCamera      ����ľ������CameraInit������á�
//            pbyIn      ����ͼ�����ݵĻ�������ַ������ΪNULL�� 
//            pbyOut        �����ͼ������Ļ�������ַ������ΪNULL��
//            pFrInfo       ����ͼ���֡ͷ��Ϣ��������ɺ�֡ͷ��Ϣ
//            uOutFormat    �������ͼ��������ʽ������CAMERA_MEDIA_TYPE_MONO8 CAMERA_MEDIA_TYPE_RGB CAMERA_MEDIA_TYPE_RGBA8������һ�֡�
//                          pbyIn��Ӧ�Ļ�������С�������uOutFormatָ���ĸ�ʽ��ƥ�䡣
//            uReserved     Ԥ����������������Ϊ0     
//                     �е�ͼ���ʽuiMediaType����֮�ı䡣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraImageProcessEx(
    hCamera:CameraHandle; 
    pbyIn:PByte; 
    pbyOut:PByte;
    pFrInfo:PtSdkFrameHead;
    uOutFormat:Integer;
    uReserved:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///****************************************************/
// ������ 	: CameraDisplayInit
// ��������	: ��ʼ��SDK�ڲ�����ʾģ�顣�ڵ���CameraDisplayRGB24
//			  ǰ�����ȵ��øú�����ʼ����������ڶ��ο����У�
//			  ʹ���Լ��ķ�ʽ����ͼ����ʾ(������CameraDisplayRGB24)��
//			  ����Ҫ���ñ�������	
// ����	    : hCamera		����ľ������CameraInit������á�
//            hWndDisplay	��ʾ���ڵľ����һ��Ϊ���ڵ�m_hWnd��Ա��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraDisplayInit(
    hCamera:CameraHandle;
    hWndDisplay:HWND
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraDisplayRGB24
// ��������	: ��ʾͼ�񡣱�����ù�CameraDisplayInit����
//			  ��ʼ�����ܵ��ñ�������	
// ����	    : hCamera  ����ľ������CameraInit������á�
//            pbyRGB24 ͼ������ݻ�������RGB888��ʽ��
//            pFrInfo  ͼ���֡ͷ��Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraDisplayRGB24(
    hCamera:CameraHandle;
    pbyRGB24:PByte;
    pFrInfo:PtSdkFrameHead
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetDisplayMode
// ��������	: ������ʾ��ģʽ��������ù�CameraDisplayInit
//			  ���г�ʼ�����ܵ��ñ�������
// ����	    : hCamera  ����ľ������CameraInit������á�
//            iMode    ��ʾģʽ��DISPLAYMODE_SCALE����
//					   DISPLAYMODE_REAL,����μ�CameraDefine.pas��Ԫ
//					   ��emSdkDisplayMode�Ķ��塣		 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetDisplayMode(
    hCamera:CameraHandle;
    iMode:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetDisplayOffset
// ��������	: ������ʾ����ʼƫ��ֵ��������ʾģʽΪDISPLAYMODE_REAL
//			  ʱ��Ч��������ʾ�ؼ��Ĵ�СΪ320X240����ͼ���
//			  �ĳߴ�Ϊ640X480����ô��iOffsetX = 160,iOffsetY = 120ʱ
//			  ��ʾ���������ͼ��ľ���320X240��λ�á�������ù�
//			  CameraDisplayInit���г�ʼ�����ܵ��ñ�������
// ����	    : hCamera   ����ľ������CameraInit������á�
//            iOffsetX  ƫ�Ƶ�X���ꡣ
//            iOffsetY  ƫ�Ƶ�Y���ꡣ
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetDisplayOffset(
    hCamera:CameraHandle;
    iOffsetX:Integer;
    iOffsetY:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetDisplaySize
// ��������	: ������ʾ�ؼ��ĳߴ硣������ù�
//			  CameraDisplayInit���г�ʼ�����ܵ��ñ�������
// ����	    : hCamera   ����ľ������CameraInit������á�
//            iWidth    ���
//            iHeight   �߶�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetDisplaySize(
    hCamera:CameraHandle; 
    iWidth:Integer;
    iHeight:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetImageBuffer
// ��������	: ���һ֡ͼ�����ݡ�Ϊ�����Ч�ʣ�SDK��ͼ��ץȡʱ�������㿽�����ƣ�
//			  CameraGetImageBufferʵ�ʻ�����ں��е�һ����������ַ��
//			  �ú����ɹ����ú󣬱������CameraReleaseImageBuffer�ͷ���
//			  CameraGetImageBuffer�õ��Ļ�����,�Ա����ں˼���ʹ��
//			  �û�������	
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            pFrameInfo  ͼ���֡ͷ��Ϣָ�롣
//            pbyBuffer   ָ��ͼ������ݵĻ�����ָ�롣����
//						  �������㿽�����������Ч�ʣ����
//						  ����ʹ����һ��ָ��ָ���ָ�롣
//            UINT wTimes ץȡͼ��ĳ�ʱʱ�䡣��λ���롣��
//						  wTimesʱ���ڻ�δ���ͼ����ú���
//						  �᷵�س�ʱ��Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetImageBuffer(
    hCamera:CameraHandle;
    pFrameInfo:PtSdkFrameHead;
    pbyBuffer:Ppbyte;
    wTimes:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSnapToBuffer
// ��������	: ץ��һ��ͼ�񵽻������С���������ץ��ģʽ������
//			  �Զ��л���ץ��ģʽ�ķֱ��ʽ���ͼ�񲶻�Ȼ��
//			  ���񵽵����ݱ��浽�������С�
//			  �ú����ɹ����ú󣬱������CameraReleaseImageBuffer
//			  �ͷ���CameraSnapToBuffer�õ��Ļ�������������ο�
//			  CameraGetImageBuffer�����Ĺ����������֡�	
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            pFrameInfo  ָ�룬����ͼ���֡ͷ��Ϣ��
//            pbyBuffer   ָ��ָ���ָ�룬��������ͼ�񻺳����ĵ�ַ��
//            uWaitTimeMs ��ʱʱ�䣬��λ���롣�ڸ�ʱ���ڣ������Ȼû��
//						  �ɹ���������ݣ��򷵻س�ʱ��Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSnapToBuffer(
    hCamera:CameraHandle;
    pFrameInfo:PtSdkFrameHead;
    pbyBuffer:Ppbyte;
    uWaitTimeMs:Cardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraReleaseImageBuffer
// ��������	: �ͷ���CameraGetImageBuffer��õĻ�������
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            pbyBuffer	  ��CameraGetImageBuffer��õĻ�������ַ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraReleaseImageBuffer(
    hCamera:CameraHandle; 
    pbyBuffer:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraPlay
// ��������	: ��SDK���빤��ģʽ����ʼ��������������͵�ͼ��
//			  ���ݡ������ǰ����Ǵ���ģʽ������Ҫ���յ�
//			  ����֡�Ժ�Ż����ͼ��
// ����	    : hCamera	  ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraPlay(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraPause
// ��������	: ��SDK������ͣģʽ�����������������ͼ�����ݣ�
//			  ͬʱҲ�ᷢ�������������ͣ������ͷŴ������
//			  ��ͣģʽ�£����Զ�����Ĳ����������ã���������Ч��	
// ����	    : hCamera	  ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraPause(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraStop
// ��������	: ��SDK����ֹͣ״̬��һ���Ƿ���ʼ��ʱ���øú�����
//			  �ú��������ã������ٶ�����Ĳ����������á�
// ����	    : hCamera	  ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraStop(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraInitRecord
// ��������	: ��ʼ��һ��¼��
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            iFormat	  ¼��ĸ�ʽ����ǰֻ֧�ֲ�ѹ����MSCV���ַ�ʽ��	
//						  0:��ѹ����1:MSCV��ʽѹ����
//            pcSavePath  ¼���ļ������·����
//            b2GLimit	  ���ΪTRUE,���ļ�����2Gʱ�Զ��ָ
//            dwQuality   ¼����������ӣ�Խ��������Խ�á���Χ1��100.
//            iFrameRate  ¼���֡�ʡ������趨�ı�ʵ�ʲɼ�֡�ʴ�
//						  �����Ͳ���©֡��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraInitRecord(
    hCamera:CameraHandle;
    iFormat:Integer;
    pcSavePath:Pchar;
    b2GLimit:LongBool;
    dwQuality:Cardinal;
    iFrameRate:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraStopRecord
// ��������	: ��������¼�񡣵�CameraInitRecord�󣬿���ͨ���ú���
//			  ������һ��¼�񣬲�����ļ����������
// ����	    : hCamera	  ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraStopRecord(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraPushFrame
// ��������	: ��һ֡���ݴ���¼�����С��������CameraInitRecord
//			  ���ܵ��øú�����CameraStopRecord���ú󣬲����ٵ���
//			  �ú������������ǵ�֡ͷ��Ϣ��Я����ͼ��ɼ���ʱ���
//			  ��Ϣ�����¼����Ծ�׼��ʱ��ͬ����������֡�ʲ��ȶ�
//			  ��Ӱ�졣
// ����	    : hCamera			����ľ������CameraInit������á�
//            pbyImageBuffer    ͼ������ݻ�������������RGB��ʽ��
//            pFrInfo           ͼ���֡ͷ��Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraPushFrame(
    hCamera:CameraHandle;
    pbyImageBuffer:PByte;
    pFrInfo:PtSdkFrameHead
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSaveImage
// ��������	: ��ͼ�񻺳��������ݱ����ͼƬ�ļ���
// ����	    : hCamera		 ����ľ������CameraInit������á�
//            lpszFileName   ͼƬ�����ļ�����·����
//            pbyImageBuffer ͼ������ݻ�������
//            pFrInfo        ͼ���֡ͷ��Ϣ��
//            byFileType     ͼ�񱣴�ĸ�ʽ��ȡֵ��Χ�μ�CameraDefine.pas��Ԫ
//							 ��emSdkFileType�����Ͷ��塣Ŀǰ֧��  
//							 BMP��JPG��PNG��RAW���ָ�ʽ������RAW��ʾ
//							 ��������ԭʼ���ݣ�����RAW��ʽ�ļ�Ҫ��
//							 pbyImageBuffer��pFrInfo����CameraGetImageBuffer
//							 ��õ����ݣ�����δ��CameraImageProcessת��
//							 ��BMP��ʽ����֮�����Ҫ�����BMP��JPG����
//							 PNG��ʽ����pbyImageBuffer��pFrInfo����
//							 CameraImageProcess������RGB��ʽ���ݡ�
//						     �����÷����Բο�Advanced�����̡�		
//            byQuality      ͼ�񱣴���������ӣ���������ΪJPG��ʽ
//						     ʱ�ò�����Ч����Χ1��100�������ʽ
//                           ����д��0��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSaveImage(
    hCamera:CameraHandle;
    lpszFileName:Pchar;
    pbyImageBuffer:PByte;
    pFrInfo:PtSdkFrameHead;
    byFileType:Byte;
    byQuality:Byte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetImageResolution
// ��������	: ��õ�ǰԤ���ķֱ��ʡ�
// ����	    : hCamera		 ����ľ������CameraInit������á�
//            psCurVideoSize �ṹ��ָ�룬���ڷ��ص�ǰ�ķֱ��ʡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetImageResolution(
    hCamera:CameraHandle;
    psCurVideoSize:PtSdkImageResolution
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetImageResolution
// ��������	: ����Ԥ���ķֱ��ʡ�
// ����	    : hCamera		   ����ľ������CameraInit������á�
//            pImageResolution �ṹ��ָ�룬���ڷ��ص�ǰ�ķֱ��ʡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetImageResolution(
    hCamera:CameraHandle;
    pImageResolution:PtSdkImageResolution
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetMediaType
// ��������	: ��������ǰ���ԭʼ���ݵĸ�ʽ�����š�
// ����	    : hCamera		����ľ������CameraInit������á�
//            piMediaType   ָ�룬���ڷ��ص�ǰ��ʽ���͵������š�
//							��CameraGetCapability�����������ԣ�
//							��tSdkCameraCapbility�ṹ���е�pMediaTypeDesc
//							��Ա�У����������ʽ���������֧�ֵĸ�ʽ��
//							piMediaType��ָ��������ţ����Ǹ�����������š�
//							pMediaTypeDesc[*piMediaType].iMediaType���ʾ��ǰ��ʽ�� 
//							���롣�ñ�����μ�CameraDefine.pas��Ԫ��[ͼ���ʽ����]���֡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetMediaType(
    hCamera:CameraHandle;
    piMediaType:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetMediaType
// ��������	: ������������ԭʼ���ݸ�ʽ��
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            iMediaType  ��CameraGetCapability�����������ԣ�
//						  ��tSdkCameraCapbility�ṹ���е�pMediaTypeDesc
//						  ��Ա�У����������ʽ���������֧�ֵĸ�ʽ��
//						  iMediaType���Ǹ�����������š�
//						  pMediaTypeDesc[iMediaType].iMediaType���ʾ��ǰ��ʽ�� 
//						  ���롣�ñ�����μ�CameraDefine.pas��Ԫ��[ͼ���ʽ����]���֡�   
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetMediaType(
    hCamera:CameraHandle;
    iMediaType:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetAeState
// ��������	: ��������ع��ģʽ���Զ������ֶ���
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            bAeState    TRUE��ʹ���Զ��ع⣻FALSE��ֹͣ�Զ��ع⡣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAeState(
    hCamera:CameraHandle;
    bAeState:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetAeState
// ��������	: ��������ǰ���ع�ģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pAeState   ָ�룬���ڷ����Զ��ع��ʹ��״̬��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetAeState(
    hCamera:CameraHandle;
    pAeState:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetSharpness
// ��������	: ����ͼ��Ĵ�����񻯲�����
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iSharpness �񻯲�������Χ��CameraGetCapability
//					     ��ã�һ����[0,100]��0��ʾ�ر��񻯴���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetSharpness(
    hCamera:CameraHandle;
    iSharpness:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetSharpness
// ��������	: ��ȡ��ǰ���趨ֵ��
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            piSharpness ָ�룬���ص�ǰ�趨���񻯵��趨ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetSharpness(
    hCamera:CameraHandle;
    piSharpness:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetLutMode
// ��������	: ��������Ĳ��任ģʽLUTģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            emLutMode  LUTMODE_PARAM_GEN ��ʾ��٤��ͶԱȶȲ�����̬����LUT��
//						 LUTMODE_PRESET    ��ʾʹ��Ԥ���LUT��
//						 LUTMODE_USER_DEF  ��ʾʹ���û��Զ���LUT��
//						 LUTMODE_PARAM_GEN�Ķ���ο�CameraDefine.pas��Ԫ��emSdkLutMode���͡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetLutMode(
    hCamera:CameraHandle;
    emLutMode:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetLutMode
// ��������	: �������Ĳ��任ģʽLUTģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pemLutMode ָ�룬���ص�ǰLUTģʽ��������CameraSetLutMode
//						 ��emLutMode������ͬ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetLutMode(
    hCamera:CameraHandle;
    pemLutMode:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSelectLutPreset
// ��������	: ѡ��Ԥ��LUTģʽ�µ�LUT��������ʹ��CameraSetLutMode
//			  ��LUTģʽ����ΪԤ��ģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iSel		 ��������š���ĸ�����CameraGetCapability
//						 ��á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSelectLutPreset(
    hCamera:CameraHandle;
    iSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetLutPresetSel
// ��������	: ���Ԥ��LUTģʽ�µ�LUT�������š�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piSel      ָ�룬���ر�������š�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetLutPresetSel(
    hCamera:CameraHandle;
    piSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraSetCustomLut
// �������� : �����Զ����LUT��������ʹ��CameraSetLutMode
//            ��LUTģʽ����Ϊ�Զ���ģʽ��
// ����     : hCamera    ����ľ������CameraInit������á�
//             iChannel ָ��Ҫ�趨��LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
//                      ����ͨ����LUT����ͬʱ�滻��
//                      �ο�CameraDefine.h��emSdkLutChannel���塣
//            pLut       ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
//                   4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��   
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas
//            �д�����Ķ��塣
///*****************************************************/
function CameraSetCustomLut(
    hCamera:CameraHandle;
    iChannel:Integer;
    pLut:PWord
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraGetCustomLut
// �������� : ��õ�ǰʹ�õ��Զ���LUT��
// ����     : hCamera    ����ľ������CameraInit������á�
//             iChannel ָ��Ҫ��õ�LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
//                      ���غ�ɫͨ����LUT��
//                      �ο�CameraDefine.h��emSdkLutChannel���塣
//            pLut       ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
//                   4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��   
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas
//            �д�����Ķ��塣
///*****************************************************/
function CameraGetCustomLut(
    hCamera:CameraHandle;
    iChannel:Integer;
    pLut:PWord
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraGetCurrentLut
// �������� : ��������ǰ��LUT�����κ�LUTģʽ�¶����Ե���,
//            ����ֱ�۵Ĺ۲�LUT���ߵı仯��
// ����     : hCamera    ����ľ������CameraInit������á�
//             iChannel ָ��Ҫ��õ�LUT��ɫͨ������ΪLUT_CHANNEL_ALLʱ��
//                      ���غ�ɫͨ����LUT��
//                      �ο�CameraDefine.h��emSdkLutChannel���塣
//            pLut       ָ�룬ָ��LUT��ĵ�ַ��LUT��Ϊ�޷��Ŷ��������飬�����СΪ
//                   4096���ֱ������ɫͨ����0��4096(12bit��ɫ����)��Ӧ��ӳ��ֵ��   
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraGetCurrentLut(
    hCamera:CameraHandle;
    iChannel:Integer;
    pLut:PWord
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetWbMode
// ��������	: ���������ƽ��ģʽ����Ϊ�ֶ����Զ����ַ�ʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            bAuto	     TRUE�����ʾʹ���Զ�ģʽ��
//						 FALSE�����ʾʹ���ֶ�ģʽ��ͨ������
//							   CameraSetOnceWB������һ�ΰ�ƽ�⡣				
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetWbMode(
    hCamera:CameraHandle;
    bAuto:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetWbMode
// ��������	: ��õ�ǰ�İ�ƽ��ģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbAuto	 ָ�룬����TRUE��ʾ�Զ�ģʽ��FALSE
//						 Ϊ�ֶ�ģʽ��	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetWbMode(
    hCamera:CameraHandle;
    pbAuto:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetPresetClrTemp
// ��������	: ����ɫ��ģʽ
// ����	    : hCamera	 ����ľ������CameraInit������á�
//             iSel     �����š�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetPresetClrTemp(
    hCamera:CameraHandle;
    iSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetPresetClrTemp
// ��������	: 
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            int* piSel
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetPresetClrTemp(
    hCamera:CameraHandle;
    piSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetOnceWB
// ��������	: ���ֶ���ƽ��ģʽ�£����øú��������һ�ΰ�ƽ�⡣
//			  ��Ч��ʱ��Ϊ���յ���һ֡ͼ������ʱ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetOnceWB(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetOnceBB
// ��������	: ִ��һ�κ�ƽ�������
// ����	    : hCamera	 ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����;��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetOnceBB(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'


///****************************************************/
// ������ 	: CameraSetAeTarget
// ��������	: �趨�Զ��ع������Ŀ��ֵ���趨��Χ��CameraGetCapability
//			  ������á�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iAeTarget  ����Ŀ��ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAeTarget(
    hCamera:CameraHandle; 
    iAeTarget:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetAeTarget
// ��������	: ����Զ��ع������Ŀ��ֵ��
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            *piAeTarget ָ�룬����Ŀ��ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetAeTarget(
    hCamera:CameraHandle; 
    piAeTarget:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetExposureTime
// ��������	: �����ع�ʱ�䡣��λΪ΢�롣����CMOS�����������ع�
//			  �ĵ�λ�ǰ�����������ģ���ˣ��ع�ʱ�䲢������΢��
//			  ���������ɵ������ǻᰴ��������ȡ�ᡣ�ڵ���
//			  �������趨�ع�ʱ��󣬽����ٵ���CameraGetExposureTime
//			  �����ʵ���趨��ֵ��
// ����	    : hCamera	     ����ľ������CameraInit������á�
//            fExposureTime  �ع�ʱ�䣬��λ΢�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetExposureTime(
    hCamera:CameraHandle; 
    fExposureTime:Double
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetExposureLineTime
// ��������	: ���һ�е��ع�ʱ�䡣����CMOS�����������ع�
//			  �ĵ�λ�ǰ�����������ģ���ˣ��ع�ʱ�䲢������΢��
//			  ���������ɵ������ǻᰴ��������ȡ�ᡣ���������
//		      ���þ��Ƿ���CMOS����ع�һ�ж�Ӧ��ʱ�䡣
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            float *pfLineTime ָ�룬����һ�е��ع�ʱ�䣬��λΪ΢�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraGetExposureLineTime(
    hCamera:CameraHandle; 
    pfLineTime:PDouble
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetExposureTime
// ��������	: ���������ع�ʱ�䡣��μ�CameraSetExposureTime
//			  �Ĺ���������
// ����	    : hCamera	       ����ľ������CameraInit������á�
//            pfExposureTime   ָ�룬���ص�ǰ���ع�ʱ�䣬��λ΢�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetExposureTime(
    hCamera:CameraHandle;
    pfExposureTime:PDouble
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetAnalogGain
// ��������	: ���������ͼ��ģ������ֵ����ֵ����CameraGetCapability���
//			  ��������Խṹ����sExposeDesc.fAnalogGainStep����
//			  �õ�ʵ�ʵ�ͼ���źŷŴ�����
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            iAnalogGain �趨��ģ������ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAnalogGain(
    hCamera:CameraHandle;
    iAnalogGain:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetAnalogGain
// ��������	: ���ͼ���źŵ�ģ������ֵ���μ�CameraSetAnalogGain
//			  ��ϸ˵����
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            piAnalogGain ָ�룬���ص�ǰ��ģ������ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetAnalogGain(
    hCamera:CameraHandle; 
    piAnalogGain:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetGain
// ��������	: ����ͼ����������档�趨��Χ��CameraGetCapability
//			  ��õ�������Խṹ����sRgbGainRange��Ա������
//			  ʵ�ʵķŴ������趨ֵ/100��
// ����	    : hCamera  ����ľ������CameraInit������á�
//            iRGain   ��ɫͨ��������ֵ��	
//            iGGain   ��ɫͨ��������ֵ��
//            iBGain   ��ɫͨ��������ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetGain(
    hCamera:CameraHandle; 
    RGain:Integer; 
    iGGain:Integer; 
    iBGain:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///****************************************************/
// ������ 	: CameraGetGain
// ��������	: ���ͼ������������档������μ�CameraSetGain
//			  �Ĺ����������֡�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piRGain	 ָ�룬���غ�ɫͨ������������ֵ��
//            piGGain    ָ�룬������ɫͨ������������ֵ��
//            piBGain    ָ�룬������ɫͨ������������ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetGain(
    hCamera:CameraHandle; 
    piRGain:PInteger; 
    piGGain:PInteger; 
    piBGain:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///****************************************************/
// ������ 	: CameraSetGamma
// ��������	: �趨LUT��̬����ģʽ�µ�Gammaֵ���趨��ֵ��
//			  ���ϱ�����SDK�ڲ�������ֻ�е�������ڶ�̬
//			  �������ɵ�LUTģʽʱ���Ż���Ч����ο�CameraSetLutMode
//			  �ĺ���˵�����֡�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iGamma     Ҫ�趨��Gammaֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetGamma(
    hCamera:CameraHandle; 
    iGamma:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetGamma
// ��������	: ���LUT��̬����ģʽ�µ�Gammaֵ����ο�CameraSetGamma
//			  �����Ĺ���������
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piGamma    ָ�룬���ص�ǰ��Gammaֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetGamma(
    hCamera:CameraHandle; 
    piGamma:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetContrast
// ��������	: �趨LUT��̬����ģʽ�µĶԱȶ�ֵ���趨��ֵ��
//			  ���ϱ�����SDK�ڲ�������ֻ�е�������ڶ�̬
//			  �������ɵ�LUTģʽʱ���Ż���Ч����ο�CameraSetLutMode
//			  �ĺ���˵�����֡�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iContrast  �趨�ĶԱȶ�ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetContrast(
    hCamera:CameraHandle; 
    iContrast:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetContrast
// ��������	: ���LUT��̬����ģʽ�µĶԱȶ�ֵ����ο�
//			  CameraSetContrast�����Ĺ���������
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piContrast ָ�룬���ص�ǰ�ĶԱȶ�ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetContrast(
    hCamera:CameraHandle; 
    piContrast:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetSaturation
// ��������	: �趨ͼ����ı��Ͷȡ��Ժڰ������Ч��
//			  �趨��Χ��CameraGetCapability��á�100��ʾ
//			  ��ʾԭʼɫ�ȣ�����ǿ��
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            iSaturation  �趨�ı��Ͷ�ֵ�� 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetSaturation(
    hCamera:CameraHandle; 
    iSaturation:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetSaturation
// ��������	: ���ͼ����ı��Ͷȡ�
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            piSaturation ָ�룬���ص�ǰͼ����ı��Ͷ�ֵ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetSaturation(
    hCamera:CameraHandle; 
    piSaturation:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetMonochrome
// ��������	: ���ò�ɫתΪ�ڰ׹��ܵ�ʹ�ܡ�
// ����	    : hCamera	����ľ������CameraInit������á�
//            bEnable   TRUE����ʾ����ɫͼ��תΪ�ڰס�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetMonochrome(
    hCamera:CameraHandle; 
    bEnable:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetMonochrome
// ��������	: ��ò�ɫת���ڰ׹��ܵ�ʹ��״����
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbEnable   ָ�롣����TRUE��ʾ�����˲�ɫͼ��
//						 ת��Ϊ�ڰ�ͼ��Ĺ��ܡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetMonochrome(
    hCamera:CameraHandle; 
    pbEnable:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetInverse
// ��������	: ���ò�ͼ����ɫ��ת���ܵ�ʹ�ܡ�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            bEnable    TRUE����ʾ����ͼ����ɫ��ת���ܣ�
//						 ���Ի�����ƽ����Ƭ��Ч����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetInverse(
    hCamera:CameraHandle; 
    bEnable:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetInverse
// ��������	: ���ͼ����ɫ��ת���ܵ�ʹ��״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbEnable   ָ�룬���ظù���ʹ��״̬�� 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetInverse(
    hCamera:CameraHandle; 
    pbEnable:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetAntiFlick
// ��������	: �����Զ��ع�ʱ��Ƶ�����ܵ�ʹ��״̬�������ֶ�
//			  �ع�ģʽ����Ч��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            bEnable    TRUE��������Ƶ������;FALSE���رոù��ܡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAntiFlick(
    hCamera:CameraHandle;
    bEnable:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetAntiFlick
// ��������	: ����Զ��ع�ʱ��Ƶ�����ܵ�ʹ��״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbEnable   ָ�룬���ظù��ܵ�ʹ��״̬��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetAntiFlick(
    hCamera:CameraHandle; 
    pbEnable:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetLightFrequency
// ��������	: ����Զ��ع�ʱ����Ƶ����Ƶ��ѡ��
// ����	    : hCamera	     ����ľ������CameraInit������á�
//            piFrequencySel ָ�룬����ѡ��������š�0:50HZ 1:60HZ
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetLightFrequency(
    hCamera:CameraHandle; 
    piFrequencySel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetLightFrequency
// ��������	: �����Զ��ع�ʱ��Ƶ����Ƶ�ʡ�
// ����	    : hCamera	    ����ľ������CameraInit������á�
//            iFrequencySel 0:50HZ , 1:60HZ 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetLightFrequency(
    hCamera:CameraHandle;
    iFrequencySel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetFrameSpeed
// ��������	: �趨������ͼ���֡�ʡ�����ɹ�ѡ���֡��ģʽ��
//			  CameraGetCapability��õ���Ϣ�ṹ����iFrameSpeedDesc
//			  ��ʾ���֡��ѡ��ģʽ������
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            iFrameSpeed ѡ���֡��ģʽ�����ţ���Χ��0��
//						  CameraGetCapability��õ���Ϣ�ṹ����iFrameSpeedDesc - 1	 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetFrameSpeed(
    hCamera:CameraHandle; 
    iFrameSpeed:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetFrameSpeed
// ��������	: ���������ͼ���֡��ѡ�������š������÷��ο�
//			  CameraSetFrameSpeed�����Ĺ����������֡�
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            piFrameSpeed ָ�룬����ѡ���֡��ģʽ�����š� 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetFrameSpeed(
    hCamera:CameraHandle; 
    piFrameSpeed:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������ 	: CameraSetParameterMode
// ��������	: �趨������ȡ��Ŀ�����
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iMode	 ������ȡ�Ķ��󡣲ο�CameraDefine.pas
//					��emSdkParameterMode�����Ͷ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas
//            �д�����Ķ��塣
///*****************************************************/
function CameraSetParameterMode(
    hCamera:CameraHandle; 
    iTarget:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetParameterMode
// ��������	: 
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            int* piMode
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetParameterMode(
    hCamera:CameraHandle; 
    piMode:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetParameterMask
// ��������	: ���ò�����ȡ�����롣�������غͱ���ʱ����ݸ�
//			  ��������������ģ��������Ƿ���ػ��߱��档
// ����	    : hCamera	����ľ������CameraInit������á�
//            uMask     ���롣�ο�CameraDefine.pas��Ԫ��PROP_SHEET_INDEX
//						���Ͷ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetParameterMask(
    hCamera:CameraHandle; 
    uMask:Cardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSaveParameter
// ��������	: ���浱ǰ���������ָ���Ĳ������С�����ṩ��A,B,C,D
//			  A,B,C,D����ռ������в����ı��档	
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iTeam      PARAMETER_TEAM_A ���浽A����,
//						 PARAMETER_TEAM_B ���浽B����,
//						 PARAMETER_TEAM_C ���浽C����,
//						 PARAMETER_TEAM_D ���浽D����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSaveParameter(
    hCamera:CameraHandle; 
    iTeam:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraReadParameterFromFile
// ��������	: ��PC��ָ���Ĳ����ļ��м��ز������ҹ�˾�������
//			  ������PC��Ϊ.config��׺���ļ���λ�ڰ�װ�µ�
//			  Camera\Configs�ļ����С�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            *sFileName �����ļ�������·����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraReadParameterFromFile(
    hCamera:CameraHandle;
    sFileName:PChar
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraLoadParameter
// ��������	: ����ָ����Ĳ���������С�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iTeam		 PARAMETER_TEAM_A ����A�����,
//						 PARAMETER_TEAM_B ����B�����,
//						 PARAMETER_TEAM_C ����C�����,
//						 PARAMETER_TEAM_D ����D�����,
//						 PARAMETER_TEAM_DEFAULT ����Ĭ�ϲ����� 		
//						 ���Ͷ���ο�CameraDefine.pas��Ԫ��emSdkParameterTeam����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraLoadParameter(
    hCamera:CameraHandle; 
    iTeam:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetCurrentParameterGroup
// ��������	: ��õ�ǰѡ��Ĳ����顣
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piTeam     ָ�룬���ص�ǰѡ��Ĳ����顣����ֵ
//						 �ο�CameraLoadParameter��iTeam������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetCurrentParameterGroup(
    hCamera:CameraHandle; 
    piTeam:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetTransPackLen
// ��������	: �����������ͼ�����ݵķְ���С��
//			  Ŀǰ��SDK�汾�У��ýӿڽ���GIGE�ӿ������Ч��
//			  �����������紫��ķְ���С������֧�־�֡��������
//			  ���ǽ���ѡ��8K�ķְ���С��������Ч�Ľ��ʹ���
//			  ��ռ�õ�CPU����ʱ�䡣
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iPackSel	 �ְ�����ѡ��������š��ְ����ȿ���
//						 ���������Խṹ����pPackLenDesc��Ա������
//						 iPackLenDesc��Ա���ʾ����ѡ�ķְ�ģʽ������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetTransPackLen(
    hCamera:CameraHandle; 
    iPackSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetTransPackLen
// ��������	: ��������ǰ����ְ���С��ѡ�������š�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piPackSel  ָ�룬���ص�ǰѡ��ķְ���С�����š�
//						 �μ�CameraSetTransPackLen��iPackSel��
//						 ˵����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetTransPackLen(
    hCamera:CameraHandle; 
    piPackSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraIsAeWinVisible
// ��������	: ����Զ��ع�ο����ڵ���ʾ״̬��
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            pbIsVisible  ָ�룬����TRUE�����ʾ��ǰ���ڻ�
//						   ��������ͼ�������ϡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraIsAeWinVisible(
    hCamera:CameraHandle;
    pbIsVisible:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetAeWinVisible
// ��������	: �����Զ��ع�ο����ڵ���ʾ״̬�������ô���״̬
//			  Ϊ��ʾ������CameraImageOverlay���ܹ�������λ��
//			  �Ծ��εķ�ʽ������ͼ���ϡ�
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            bIsVisible  TRUE������Ϊ��ʾ��FALSE������ʾ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAeWinVisible(
    hCamera:CameraHandle;
    bIsVisible:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetAeWindow
// ��������	: ����Զ��ع�ο����ڵ�λ�á�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piHOff     ָ�룬���ش���λ�����ϽǺ�����ֵ��
//            piVOff     ָ�룬���ش���λ�����Ͻ�������ֵ��
//            piWidth    ָ�룬���ش��ڵĿ�ȡ�
//            piHeight   ָ�룬���ش��ڵĸ߶ȡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetAeWindow(
    hCamera:CameraHandle; 
    piHOff:PInteger; 
    piVOff:PInteger; 
    piWidth:PInteger; 
    piHeight:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetAeWindow
// ��������	: �����Զ��ع�Ĳο����ڡ�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iHOff		 �������Ͻǵĺ�����
//            iVOff      �������Ͻǵ�������
//            iWidth     ���ڵĿ�� 
//            iHeight    ���ڵĸ߶�
//			  ���iHOff��iVOff��iWidth��iHeightȫ��Ϊ0����
//			  ��������Ϊÿ���ֱ����µľ���1/2��С����������
//			  �ֱ��ʵı仯������仯�����iHOff��iVOff��iWidth��iHeight
//			  �������Ĵ���λ�÷�Χ�����˵�ǰ�ֱ��ʷ�Χ�ڣ� 
//		      ���Զ�ʹ�þ���1/2��С���ڡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetAeWindow(
    hCamera:CameraHandle; 
    iHOff:Integer; 
    iVOff:Integer; 
    iWidth:Integer; 
    iHeight:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetMirror
// ��������	: ����ͼ������������������Ϊˮƽ�ʹ�ֱ��������
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iDir		 ��ʾ����ķ���0����ʾˮƽ����1����ʾ��ֱ����
//            bEnable	 TRUE��ʹ�ܾ���;FALSE����ֹ����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetMirror(
    hCamera:CameraHandle; 
    iDir:Integer; 
    bEnable:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetMirror
// ��������	: ���ͼ��ľ���״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iDir		 ��ʾҪ��õľ�����
//						 0����ʾˮƽ����1����ʾ��ֱ����
//            pbEnable   ָ�룬����TRUE�����ʾiDir��ָ�ķ���
//						 ����ʹ�ܡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetMirror(
    hCamera:CameraHandle; 
    iDir:Integer; 
    pbEnable:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetWbWindow
// ��������	: ��ð�ƽ��ο����ڵ�λ�á�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            PiHOff	 ָ�룬���زο����ڵ����ϽǺ�����	��
//            PiVOff     ָ�룬���زο����ڵ����Ͻ�������	��
//            PiWidth    ָ�룬���زο����ڵĿ�ȡ�
//            PiHeight   ָ�룬���زο����ڵĸ߶ȡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetWbWindow(
    hCamera:CameraHandle; 
    PiHOff:PInteger; 
    PiVOff:PInteger; 
    PiWidth:PInteger; 
    PiHeight:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetWbWindow
// ��������	: ���ð�ƽ��ο����ڵ�λ�á�
// ����	    : hCamera	����ľ������CameraInit������á�
//            iHOff		�ο����ڵ����ϽǺ����ꡣ
//            iVOff     �ο����ڵ����Ͻ������ꡣ
//            iWidth    �ο����ڵĿ�ȡ�
//            iHeight   �ο����ڵĸ߶ȡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetWbWindow(
    hCamera:CameraHandle; 
    iHOff:Integer; 
    iVOff:Integer; 
    iWidth:Integer; 
    iHeigh:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraIsWbWinVisible
// ��������	: ��ð�ƽ�ⴰ�ڵ���ʾ״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbShow	 ָ�룬����TRUE�����ʾ�����ǿɼ��ġ�	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraIsWbWinVisible(
    hCamera:CameraHandle;
    pbShow:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetWbWinVisible
// ��������	: ���ð�ƽ�ⴰ�ڵ���ʾ״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            bShow      TRUE�����ʾ����Ϊ�ɼ����ڵ���
//						 CameraImageOverlay��ͼ�������Ͻ��Ծ���
//						 �ķ�ʽ���Ӱ�ƽ��ο����ڵ�λ�á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetWbWinVisible(
    hCamera:CameraHandle; 
    bShow:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraImageOverlay
// ��������	: �������ͼ�������ϵ���ʮ���ߡ���ƽ��ο����ڡ�
//			  �Զ��ع�ο����ڵ�ͼ�Ρ�ֻ������Ϊ�ɼ�״̬��
//			  ʮ���ߺͲο����ڲ��ܱ������ϡ�
//			  ע�⣬�ú���������ͼ�������RGB��ʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pRgbBuffer ͼ�����ݻ�������
//            pFrInfo    ͼ���֡ͷ��Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraImageOverlay(
    hCamera:CameraHandle;
    pRgbBuffer:PByte;
    pFrInfo:PtSdkFrameHead
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetCrossLine
// ��������	: ����ָ��ʮ���ߵĲ�����
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iLine		 ��ʾҪ���õڼ���ʮ���ߵ�״̬����ΧΪ[0,8]����9����		 
//            x          ʮ��������λ�õĺ�����ֵ��
//            y			 ʮ��������λ�õ�������ֵ��
//            uColor     ʮ���ߵ���ɫ����ʽΪ(R|(G<<8)|(B<<16))
//            bVisible   ʮ���ߵ���ʾ״̬��TRUE����ʾ��ʾ��
//						 ֻ������Ϊ��ʾ״̬��ʮ���ߣ��ڵ���
//						 CameraImageOverlay��Żᱻ���ӵ�ͼ���ϡ�			
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetCrossLine(
    hCamera:CameraHandle; 
    iLine:Integer; 
    x:Integer;
    y:Integer;
    uColor:Cardinal;
    bVisible:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetCrossLine
// ��������	: ���ָ��ʮ���ߵ�״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iLine		 ��ʾҪ��ȡ�ĵڼ���ʮ���ߵ�״̬����ΧΪ[0,8]����9����	 
//            px		 ָ�룬���ظ�ʮ��������λ�õĺ����ꡣ
//            py		 ָ�룬���ظ�ʮ��������λ�õĺ����ꡣ
//            pcolor     ָ�룬���ظ�ʮ���ߵ���ɫ����ʽΪ(R|(G<<8)|(B<<16))��
//            pbVisible  ָ�룬����TRUE�����ʾ��ʮ���߿ɼ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetCrossLine(
    hCamera:CameraHandle; 
    iLine:Integer;
    px:PInteger;
    py:PInteger;
    pcolor:PCardinal;
    pbVisible:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetCapability
// ��������	: �����������������ṹ�塣�ýṹ���а��������
//			  �����õĸ��ֲ����ķ�Χ��Ϣ����������غ����Ĳ���
//			  ���أ�Ҳ�����ڶ�̬������������ý��档
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            pCameraInfo ָ�룬���ظ�������������Ľṹ�塣
//	                      tSdkCameraCapbility��CameraDefine.pas��Ԫ�ж��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetCapability(
    hCamera:CameraHandle; 
    pCameraInfo:PtSdkCameraCapbility
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraWriteSN
// ��������	: ������������кš��ҹ�˾������кŷ�Ϊ3����
//			  0�������ҹ�˾�Զ����������кţ�����ʱ�Ѿ�
//			  �趨�ã�1����2���������ο���ʹ�á�ÿ������
//			  �ų��ȶ���32���ֽڡ�
// ����	    : hCamera  ����ľ������CameraInit������á�
//            pbySN	   ���кŵĻ�������	
//            iLevel   Ҫ�趨�����кż���ֻ����1����2��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraWriteSN(
    hCamera:CameraHandle; 
    pbySN:PByte; 
    iLevel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraReadSN
// ��������	: ��ȡ���ָ����������кš����кŵĶ�����ο�
//		      CameraWriteSN�����Ĺ����������֡�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            pbySN		 ���кŵĻ�������
//            iLevel     Ҫ��ȡ�����кż���ֻ����1��2��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraReadSN(
    hCamera:CameraHandle; 
    pbySN:PByte; 
    iLevel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'
///****************************************************/
// ������ 	: CameraSetTriggerDelayTime
// ��������	: ����Ӳ������ģʽ�µĴ�����ʱʱ�䣬��λ΢�롣
//			  ��Ӳ�����ź����ٺ󣬾���ָ������ʱ���ٿ�ʼ�ɼ�
//			  ͼ�񡣽������ͺŵ����֧�ָù��ܡ�������鿴
//			  ��Ʒ˵���顣
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            uDelayTimeUs Ӳ������ʱ����λ΢�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetTriggerDelayTime(
    hCamera:CameraHandle; 
    uDelayTimeUs:Cardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetTriggerDelayTime
// ��������	: ��õ�ǰ�趨��Ӳ������ʱʱ�䡣
// ����	    : hCamera	    ����ľ������CameraInit������á�
//            puDelayTimeUs ָ�룬������ʱʱ�䣬��λ΢�롣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetTriggerDelayTime(
    hCamera:CameraHandle; 
    puDelayTimeUs:PCardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetTriggerCount
// ��������	: ���ô���ģʽ�µĴ���֡���������������Ӳ������
//			  ģʽ����Ч��Ĭ��Ϊ1֡����һ�δ����źŲɼ�һ֡ͼ��
// ����	    : hCamera	����ľ������CameraInit������á�
//            iCount    һ�δ����ɼ���֡����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetTriggerCount(
    hCamera:CameraHandle; 
    iCount:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetTriggerCount
// ��������	: ���һ�δ�����֡����
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            INT* piCount
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetTriggerCount(
    hCamera:CameraHandle; 
    piCount:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSoftTrigger
// ��������	: ִ��һ��������ִ�к󣬻ᴥ����CameraSetTriggerCount
//		      ָ����֡����
// ����	    : hCamera	 ����ľ������CameraInit������á�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSoftTrigger(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetTriggerMode
// ��������	: ��������Ĵ���ģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iModeSel   ģʽѡ�������š����趨��ģʽ��
//						 CameraGetCapability������ȡ����ο�
//					     CameraDefine.pas��Ԫ��tSdkCameraCapbility�Ķ��塣
//						 һ�������0��ʾ�����ɼ�ģʽ��1��ʾ
//						 �������ģʽ��2��ʾӲ������ģʽ��	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetTriggerMode(
    hCamera:CameraHandle; 
    iModeSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetTriggerMode
// ��������	: �������Ĵ���ģʽ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            piModeSel  ָ�룬���ص�ǰѡ����������ģʽ�������š�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetTriggerMode(
    hCamera:CameraHandle;
    piModeSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

    
///*****************************************************/
// ������   : CameraSetStrobeMode
// �������� : ����IO���Ŷ����ϵ�STROBE�źš����źſ���������ƿ��ƣ�Ҳ�������ⲿ��е���ſ��ơ�
// ����     : hCamera ����ľ������CameraInit������á�
//             iMode   ��ΪSTROBE_SYNC_WITH_TRIG_AUTO      �ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš�
//                                                         ��ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
//                     ��ΪSTROBE_SYNC_WITH_TRIG_MANUALʱ���ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)��
//                                                         �ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)��
//                                                         ��Ч���Կ�����(CameraSetStrobePolarity)��
//                     ��ΪSTROBE_ALWAYS_HIGHʱ��STROBE�źź�Ϊ��,������������
//                     ��ΪSTROBE_ALWAYS_LOWʱ��STROBE�źź�Ϊ��,������������
//
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetStrobeMode(
    hCamera:CameraHandle;
    iMode:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraGetStrobeMode
// �������� : ���ߵ�ǰSTROBE�ź����õ�ģʽ��
// ����     : hCamera ����ľ������CameraInit������á�
//             piMode  ָ�룬����STROBE_SYNC_WITH_TRIG_AUTO,STROBE_SYNC_WITH_TRIG_MANUAL��STROBE_ALWAYS_HIGH����STROBE_ALWAYS_LOW��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetStrobeMode(
    hCamera:CameraHandle;
    piMode:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetStrobeDelayTime
// �������� : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Դ����ź���ʱʱ�䡣
// ����     : hCamera       ����ľ������CameraInit������á�
//             uDelayTimeUs  ��Դ����źŵ���ʱʱ�䣬��λΪus������Ϊ0��������Ϊ������ 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetStrobeDelayTime(
    hCamera:CameraHandle;
    uDelayTimeUs:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetStrobeDelayTime
// �������� : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú����������Դ����ź���ʱʱ�䡣
// ����     : hCamera           ����ľ������CameraInit������á�
//             upDelayTimeUs     ָ�룬������ʱʱ�䣬��λus��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetStrobeDelayTime(
    hCamera:CameraHandle;
    upDelayTimeUs:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetStrobePulseWidth
// �������� : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú��������������ȡ�
// ����     : hCamera       ����ľ������CameraInit������á�
//             uTimeUs       ����Ŀ�ȣ���λΪʱ��us��  
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetStrobePulseWidth(
    hCamera:CameraHandle;
    uTimeUs:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetStrobePulseWidth
// �������� : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�������������ȡ�
// ����     : hCamera   ����ľ������CameraInit������á�
//             upTimeUs  ָ�룬���������ȡ���λΪʱ��us��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetStrobePulseWidth(
    hCamera:CameraHandle;
    upTimeUs:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetStrobePolarity
// �������� : ��STROBE�źŴ���STROBE_SYNC_WITH_TRIGʱ��ͨ���ú�����������Ч��ƽ�ļ��ԡ�Ĭ��Ϊ����Ч���������źŵ���ʱ��STROBE�źű����ߡ�
// ����     : hCamera   ����ľ������CameraInit������á�
//             iPolarity STROBE�źŵļ��ԣ�0Ϊ�͵�ƽ��Ч��1Ϊ�ߵ�ƽ��Ч��Ĭ��Ϊ�ߵ�ƽ��Ч��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetStrobePolarity(
    hCamera:CameraHandle;
    uPolarity:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetStrobePolarity
// �������� : ��������ǰSTROBE�źŵ���Ч���ԡ�Ĭ��Ϊ�ߵ�ƽ��Ч��
// ����     : hCamera       ����ľ������CameraInit������á�
//             ipPolarity    ָ�룬����STROBE�źŵ�ǰ����Ч���ԡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetStrobePolarity(
    hCamera:CameraHandle;
    upPolarity:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetExtTrigSignalType
// �������� : ��������ⴥ���źŵ����ࡣ�ϱ��ء��±��ء����߸ߡ��͵�ƽ��ʽ��
// ����     : hCamera   ����ľ������CameraInit������á�
//             iType     �ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
//                       emExtTrigSignal���Ͷ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetExtTrigSignalType(
    hCamera:CameraHandle;
    iType:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetExtTrigSignalType
// �������� : ��������ǰ�ⴥ���źŵ����ࡣ
// ����     : hCamera   ����ľ������CameraInit������á�
//             ipType    ָ�룬�����ⴥ���ź����࣬����ֵ�ο�CameraDefine.h��
//                       emExtTrigSignal���Ͷ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetExtTrigSignalType(
    hCamera:CameraHandle;
    ipType:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetExtTrigShutterType
// �������� : �����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
// ����     : hCamera   ����ľ������CameraInit������á�
//             iType     �ⴥ�����ŷ�ʽ���ο�CameraDefine.h��emExtTrigShutterMode���͡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetExtTrigShutterType(
    hCamera:CameraHandle;
    iType:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetExtTrigShutterType
// �������� : ����ⴥ��ģʽ�£�������ŵķ�ʽ��Ĭ��Ϊ��׼���ŷ�ʽ��
//              ���ֹ������ŵ�CMOS���֧��GRR��ʽ��
// ����     : hCamera   ����ľ������CameraInit������á�
//             ipType    ָ�룬���ص�ǰ�趨���ⴥ�����ŷ�ʽ������ֵ�ο�
//                       CameraDefine.h��emExtTrigShutterMode���͡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetExtTrigShutterType(
    hCamera:CameraHandle;
    ipType:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetExtTrigDelayTime
// �������� : �����ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣 
//              �����õ�ֵuDelayTimeUs��Ϊ0ʱ��������յ��ⴥ���źź󣬽���ʱuDelayTimeUs��΢����ٽ���ͼ�񲶻�
// ����     : hCamera       ����ľ������CameraInit������á�
//             uDelayTimeUs  ��ʱʱ�䣬��λΪ΢�룬Ĭ��Ϊ0.
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetExtTrigDelayTime(
    hCamera:CameraHandle;
    uDelayTimeUs:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetExtTrigDelayTime
// �������� : ������õ��ⴥ���ź���ʱʱ�䣬Ĭ��Ϊ0����λΪ΢�롣 
// ����     : hCamera   ����ľ������CameraInit������á�
//            UINT* upDelayTimeUs
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetExtTrigDelayTime(
    hCamera:CameraHandle;
    upDelayTimeUs:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraSetExtTrigJitterTime
// �������� : ��������ⴥ���źŵ�����ʱ�䡣Ĭ��Ϊ0����λΪ΢�롣
// ����     : hCamera   ����ľ������CameraInit������á�
//            UINT uTimeUs
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetExtTrigJitterTime(
    hCamera:CameraHandle;
    uTimeUs:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraGetExtTrigJitterTime
// �������� : ������õ�����ⴥ������ʱ�䣬Ĭ��Ϊ0.��λΪ΢��
// ����     : hCamera   ����ľ������CameraInit������á�
//            UINT* upTimeUs
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetExtTrigJitterTime(
    hCamera:CameraHandle;
    upTimeUs:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraGetExtTrigCapability
// �������� : �������ⴥ������������
// ����     : hCamera           ����ľ������CameraInit������á�
//             puCapabilityMask  ָ�룬���ظ�����ⴥ���������룬����ο�CameraDefine.h��
//                               EXT_TRIG_MASK_ ��ͷ�ĺ궨�塣   
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetExtTrigCapability(
    hCamera:CameraHandle;
    puCapabilityMask:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'



///****************************************************/
// ������ 	: CameraGetResolutionForSnap
// ��������	: ���ץ��ģʽ�µķֱ���ѡ�������š�
// ����	    : hCamera	       ����ľ������CameraInit������á�
//            pImageResolution ָ�룬����ץ��ģʽ�ķֱ��ʡ� 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetResolutionForSnap(
    hCamera:CameraHandle;
    pImageResolution:PtSdkImageResolution
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetResolutionForSnap
// ��������	: ����ץ��ģʽ��������ͼ��ķֱ��ʡ�
// ����	    : hCamera	      ����ľ������CameraInit������á�
//            pImageResolution ���pImageResolution->iWidth 
//							   �� pImageResolution->iHeight��Ϊ0��
//			                   ���ʾ�趨Ϊ���浱ǰԤ���ֱ��ʡ�ץ
//			                   �µ���ͼ��ķֱ��ʻ�͵�ǰ�趨��	
//							   Ԥ���ֱ���һ����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetResolutionForSnap(
    hCamera:CameraHandle;
    pImageResolution:PtSdkImageResolution
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraCustomizeResolution
// ��������	: �򿪷ֱ����Զ�����壬��ͨ�����ӻ��ķ�ʽ
//			  ������һ���Զ���ֱ��ʡ�
// ����	    : hCamera	   ����ľ������CameraInit������á�
//            pImageCustom ָ�룬�����Զ���ķֱ��ʡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraCustomizeResolution(
    hCamera:CameraHandle;
    pImageCustom:PtSdkImageResolution
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraCustomizeReferWin
// ��������	: �򿪲ο������Զ�����塣��ͨ�����ӻ��ķ�ʽ��
//			  ���һ���Զ��崰�ڵ�λ�á�һ�������Զ����ƽ��
//			  ���Զ��ع�Ĳο����ڡ�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            iWinType   Ҫ���ɵĲο����ڵ���;��0,�Զ��ع�ο����ڣ�
//						 1,��ƽ��ο����ڡ�
//            hParent    ���øú����Ĵ��ڵľ��������ΪNULL��
//            piHOff     ָ�룬�����Զ��崰�ڵ����ϽǺ����ꡣ
//            piVOff     ָ�룬�����Զ��崰�ڵ����Ͻ������ꡣ
//            piWidth    ָ�룬�����Զ��崰�ڵĿ�ȡ� 
//            piHeight   ָ�룬�����Զ��崰�ڵĸ߶ȡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraCustomizeReferWin(
    hCamera:CameraHandle;
    iWinType:Integer;
    hParent:HWND; 
    piHOff:PInteger;
    piVOff:PInteger;
    piWidth:PInteger;
    piHeight:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraShowSettingPage
// ��������	: ��������������ô�����ʾ״̬�������ȵ���CameraCreateSettingPage
//			  �ɹ���������������ô��ں󣬲��ܵ��ñ���������
//			  ��ʾ��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            bShow		 TRUE����ʾ;FALSE�����ء�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraShowSettingPage(
    hCamera:CameraHandle;
    bShow:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraCreateSettingPage
// ��������	: ������������������ô��ڡ����øú�����SDK�ڲ���
//			  ������������������ô��ڣ�ʡȥ�������¿������
//			  ���ý����ʱ�䡣ǿ�ҽ���ʹ����ʹ�øú�����
//			  SDKΪ�����������ô��ڡ�
// ����	    : hCamera	    ����ľ������CameraInit������á�
//            hParent       Ӧ�ó��������ڵľ��������ΪNULL��
//            pWinText      �ַ���ָ�룬������ʾ�ı�������
//            pCallbackFunc ������Ϣ�Ļص�����������Ӧ���¼�����ʱ��
//							pCallbackFunc��ָ��ĺ����ᱻ���ã�
//							�����л��˲���֮��Ĳ���ʱ��pCallbackFunc
//							���ص�ʱ������ڲ�����ָ������Ϣ���͡�
//							�������Է������Լ������Ľ�����������ɵ�UI
//							֮�����ͬ�����ò�������ΪNULL��	  
//            pCallbackCtx  �ص������ĸ��Ӳ���������ΪNULL��pCallbackCtx
//							����pCallbackFunc���ص�ʱ����Ϊ����֮һ���롣
//							������ʹ�øò�������һЩ�����жϡ�
//            uReserved     Ԥ������������Ϊ0��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraCreateSettingPage(
    hCamera:CameraHandle;
    hParent:HWND;
    pWinText:PChar;
    pCallbackFunc:CAMERA_PAGE_MSG_PROC;
    pCallbackCtx:Pointer;
    uReserved:Cardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetActiveSettingSubPage
// ��������	: ����������ô��ڵļ���ҳ�档������ô����ж��
//			  ��ҳ�湹�ɣ��ú��������趨��ǰ��һ����ҳ��
//			  Ϊ����״̬����ʾ����ǰ�ˡ�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            index      ��ҳ��������š��ο�CameraDefine.pas��Ԫ��
//						 PROP_SHEET_INDEX�Ķ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetActiveSettingSubPage(
    hCamera:CameraHandle;
    index:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSpecialControl
// ��������	: ���һЩ�������������õĽӿڣ����ο���ʱһ�㲻��Ҫ
//			  ���á�
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            dwCtrlCode �����롣
//            dwParam    �������룬��ͬ��dwCtrlCodeʱ�����岻ͬ��
//            lpData     ���Ӳ�������ͬ��dwCtrlCodeʱ�����岻ͬ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSpecialControl(
    hCamera:CameraHandle; 
    dwCtrlCode:Cardinal;
    dwParam:Cardinal;
    lpData:Pointer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetFrameStatistic
// ��������	: ����������֡�ʵ�ͳ����Ϣ����������֡�Ͷ�֡�������
// ����	    : hCamera	       ����ľ������CameraInit������á�
//            psFrameStatistic ָ�룬����ͳ����Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetFrameStatistic(
    hCamera:CameraHandle; 
    psFrameStatistic:PtSdkFrameStatistic
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraSetNoiseFilter
// ��������	: ����ͼ����ģ���ʹ��״̬��
// ����	    : hCamera	����ľ������CameraInit������á�
//            bEnable   TRUE��ʹ�ܣ�FALSE����ֹ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraSetNoiseFilter(
    hCamera:CameraHandle;
    bEnable:LongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraGetNoiseFilterState
// ��������	: ���ͼ����ģ���ʹ��״̬��
// ����	    : hCamera	 ����ľ������CameraInit������á�
//            *pEnable   ָ�룬����״̬��TRUE��Ϊʹ�ܡ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraGetNoiseFilterState(
    hCamera:CameraHandle;
    pEnable:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///****************************************************/
// ������ 	: CameraRstTimeStamp
// ��������	: ��λͼ��ɼ���ʱ�������0��ʼ��
// ����	    : hCamera:CameraHandle
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///****************************************************/
function CameraRstTimeStamp(
    hCamera:CameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'
 
    
///*****************************************************/
// ������   : CameraSaveUserData
// �������� : ���û��Զ�������ݱ��浽����ķ����Դ洢���С�
//              ÿ���ͺŵ��������֧�ֵ��û���������󳤶Ȳ�һ����
//              ���Դ��豸�����������л�ȡ�ó�����Ϣ��
// ����     : hCamera    ����ľ������CameraInit������á�
//            uStartAddr  ��ʼ��ַ����0��ʼ��
//            pbData      ���ݻ�����ָ��
//            ilen        д�����ݵĳ��ȣ�ilen + uStartAddr����
//                        С���û�����󳤶�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraSaveUserData(
    hCamera:CameraHandle;
    uStartAddr:Cardinal;
    pbData:PByte;
    ilen:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������   : CameraLoadUserData
// �������� : ������ķ����Դ洢���ж�ȡ�û��Զ�������ݡ�
//              ÿ���ͺŵ��������֧�ֵ��û���������󳤶Ȳ�һ����
//              ���Դ��豸�����������л�ȡ�ó�����Ϣ��
// ����     : hCamera    ����ľ������CameraInit������á�
//            uStartAddr  ��ʼ��ַ����0��ʼ��
//            pbData      ���ݻ�����ָ�룬���ض��������ݡ�
//            ilen        ��ȡ���ݵĳ��ȣ�ilen + uStartAddr����
//                        С���û�����󳤶�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraLoadUserData(
    hCamera:CameraHandle;
    uStartAddr:Cardinal;
    pbData:PByte;
    ilen:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetFriendlyName
// ��������	: ��ȡ�û��Զ�����豸�ǳơ�
// ����	    : hCamera    ����ľ������CameraInit������á�
//            pName      ָ�룬����ָ��0��β���ַ�����
//						 �豸�ǳƲ�����32���ֽڣ���˸�ָ��
//						 ָ��Ļ�����������ڵ���32���ֽڿռ䡣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraGetFriendlyName(
	hCamera:CameraHandle;
	pName:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetFriendlyName
// ��������	: �����û��Զ�����豸�ǳơ�
// ����	    : hCamera    ����ľ������CameraInit������á�
//            pName      ָ�룬ָ��0��β���ַ�����
//						 �豸�ǳƲ�����32���ֽڣ���˸�ָ��
//						 ָ���ַ�������С�ڵ���32���ֽڿռ䡣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetFriendlyName(
	hCamera:CameraHandle;
	pName:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSdkGetVersionString
// ��������	: 
// ����	    : pVersionString ָ�룬����SDK�汾�ַ�����
//                            ��ָ��ָ��Ļ�������С�������
//                            32���ֽ�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSdkGetVersionString(
	pVersionString:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraCheckFwUpdate
// ��������	: ���̼��汾���Ƿ���Ҫ������
// ����	    : hCamera ����ľ������CameraInit������á�
//            pNeedUpdate ָ�룬���ع̼����״̬��TRUE��ʾ��Ҫ����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraCheckFwUpdate(
	hCamera:CameraHandle;
	pNeedUpdate:PLongBool
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetFirmwareVision
// ��������	: ��ù̼��汾���ַ���
// ����	    : hCamera ����ľ������CameraInit������á�
//            pVersion ����ָ��һ������32�ֽڵĻ�������
//                      ���ع̼��İ汾�ַ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetFirmwareVision(
	hCamera:CameraHandle;
	pVersion:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetEnumInfo
// ��������	: ���ָ���豸��ö����Ϣ
// ����	    : hCamera ����ľ������CameraInit������á�
//            pCameraInfo ָ�룬�����豸��ö����Ϣ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetEnumInfo(
	hCamera:CameraHandle;
	pCameraInfo:PtSdkCameraDevInfo
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetInerfaceVersion
// ��������	: ���ָ���豸�ӿڵİ汾
// ����	    : hCamera ����ľ������CameraInit������á�
//            pVersion ָ��һ������32�ֽڵĻ����������ؽӿڰ汾�ַ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetInerfaceVersion(
	hCamera:CameraHandle;
	pVersion:PByte
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetIOState
// ��������	: ����ָ��IO�ĵ�ƽ״̬��IOΪ�����IO�����
//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
//              iOutputIoCounts������
// ����	    : hCamera ����ľ������CameraInit������á�
//            iOutputIOIndex IO�������ţ���0��ʼ��
//            uState Ҫ�趨��״̬��1Ϊ�ߣ�0Ϊ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetIOState(
	hCamera:CameraHandle;
	iOutputIOIndex:Integer;
	uState:Cardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetIOState
// ��������	: ����ָ��IO�ĵ�ƽ״̬��IOΪ������IO�����
//              Ԥ���ɱ�����IO�ĸ�����tSdkCameraCapbility��
//              iInputIoCounts������
// ����	    : hCamera ����ľ������CameraInit������á�          
//            iInputIOIndex IO�������ţ���0��ʼ��
//            puState ָ�룬����IO״̬,1Ϊ�ߣ�0Ϊ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetIOState(
	hCamera:CameraHandle;
	iInputIOIndex:Integer;
	puState:PCardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetAeAlgorithm
// ��������	: �����Զ��ع�ʱѡ����㷨����ͬ���㷨������
//			  ��ͬ�ĳ�����
// ����	    : hCamera 			����ľ������CameraInit������á� 
//            iIspProcessor 	ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
//								emSdkIspProcessor�Ķ���
//            iAeAlgorithmSel   Ҫѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
//								��iAeAlmSwDesc��iAeAlmHdDesc������	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetAeAlgorithm(
    hCamera:CameraHandle;
    iIspProcessor:Integer;
    iAeAlgorithmSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetAeAlgorithm
// ��������	: ��õ�ǰ�Զ��ع���ѡ����㷨
// ����	    : hCamera 			����ľ������CameraInit������á� 
//            iIspProcessor 	ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
//								emSdkIspProcessor�Ķ���
//            piAeAlgorithmSel  ���ص�ǰѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
//								��iAeAlmSwDesc��iAeAlmHdDesc������	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetAeAlgorithm(
    hCamera:CameraHandle;
    iIspProcessor:Integer;
    piAlgorithmSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetBayerDecAlgorithm
// ��������	: ����Bayer����ת��ɫ���㷨��
// ����	    : hCamera 			����ľ������CameraInit������á� 
//            iIspProcessor 	ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
//								emSdkIspProcessor�Ķ���
//            iAlgorithmSel     Ҫѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
//								��iBayerDecAlmSwDesc��iBayerDecAlmHdDesc������		
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetBayerDecAlgorithm(
    hCamera:CameraHandle;
    iIspProcessor:Integer;
    iAlgorithmSel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetBayerDecAlgorithm
// ��������	: ���Bayer����ת��ɫ��ѡ����㷨��
// ����	    : hCamera 			����ľ������CameraInit������á� 
//            iIspProcessor 	ѡ��ִ�и��㷨�Ķ��󣬲ο�CameraDefine.h
//								emSdkIspProcessor�Ķ���
//            piAlgorithmSel    ���ص�ǰѡ����㷨��š���0��ʼ�����ֵ��tSdkCameraCapbility
//								��iBayerDecAlmSwDesc��iBayerDecAlmHdDesc������	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetBayerDecAlgorithm(
    hCamera:CameraHandle;
    iIspProcessor:Integer;
    piAlgorithmSel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetIspProcessor
// ��������	: ����ͼ����Ԫ���㷨ִ�ж�����PC�˻��������
//			  ��ִ���㷨�����������ִ��ʱ���ή��PC�˵�CPUռ���ʡ�
// ����	    : hCamera		����ľ������CameraInit������á� 
//            iIspProcessor	�ο�CameraDefine.h��
//							emSdkIspProcessor�Ķ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetIspProcessor(
    hCamera:CameraHandle;
    iIspProcessor:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetIspProcessor
// ��������	: ���ͼ����Ԫ���㷨ִ�ж���
// ����	    : hCamera		 ����ľ������CameraInit������á� 
//            piIspProcessor ����ѡ��Ķ��󣬷���ֵ�ο�CameraDefine.h��
//							 emSdkIspProcessor�Ķ��塣
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetIspProcessor(
    hCamera:CameraHandle;
    piIspProcessor:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetBlackLevel
// ��������	: ����ͼ��ĺڵ�ƽ��׼��Ĭ��ֵΪ0
// ����	    : hCamera	  ����ľ������CameraInit������á� 
//            iBlackLevel Ҫ�趨�ĵ�ƽֵ����ΧΪ0��255��	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetBlackLevel(
	hCamera:CameraHandle;
	iBlackLevel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetBlackLevel
// ��������	: ���ͼ��ĺڵ�ƽ��׼��Ĭ��ֵΪ0
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//            piBlackLevel ���ص�ǰ�ĺڵ�ƽֵ����ΧΪ0��255��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetBlackLevel(
	hCamera:CameraHandle;
	piBlackLevel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraSetWhiteLevel
// ��������	: ����ͼ��İ׵�ƽ��׼��Ĭ��ֵΪ255
// ����	    : hCamera		����ľ������CameraInit������á� 
//            iWhiteLevel	Ҫ�趨�ĵ�ƽֵ����ΧΪ0��255��	
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraSetWhiteLevel(
	hCamera:CameraHandle;
	iWhiteLevel:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraGetWhiteLevel
// ��������	: ���ͼ��İ׵�ƽ��׼��Ĭ��ֵΪ255
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//            piWhiteLevel ���ص�ǰ�İ׵�ƽֵ����ΧΪ0��255��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function  CameraGetWhiteLevel(
	hCamera:CameraHandle;
	piWhiteLevel:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'


///*****************************************************/
// ������   : CameraEnumerateDeviceEx
// �������� : ö���豸���������豸�б��ڵ���CameraInitEx
//            ֮ǰ��������øú���ö���豸��
// ����      : 
// ����ֵ    : �����豸������0��ʾ�ޡ�
///*****************************************************/
function CameraEnumerateDeviceEx(
):Integer; stdcall;external 'MVCAMSDK.DLL'

///*****************************************************/
// ������ 	: CameraInitEx
// ��������	: �����ʼ������ʼ���ɹ��󣬲��ܵ����κ�����
//			  �����صĲ����ӿڡ�		
// ����	    : iDeviceIndex    ����������ţ�CameraEnumerateDeviceEx�������������	
//            iParamLoadMode  �����ʼ��ʱʹ�õĲ������ط�ʽ��-1��ʾʹ���ϴ��˳�ʱ�Ĳ������ط�ʽ��
//            emTeam         ��ʼ��ʱʹ�õĲ����顣-1��ʾ�����ϴ��˳�ʱ�Ĳ����顣
//            pCameraHandle  ����ľ��ָ�룬��ʼ���ɹ��󣬸�ָ��
//							 ���ظ��������Ч������ڵ����������
//							 ��صĲ����ӿ�ʱ������Ҫ����þ������Ҫ
//							 ���ڶ����֮������֡�
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.pas��Ԫ
//            �д�����Ķ��塣
///*****************************************************/
function CameraInitEx(
    iDeviceIndex:Integer;
    iParamLoadMode:Integer;
    emTeam:Integer;
    pCameraHandle:PCameraHandle
):Integer; stdcall;external 'MVCAMSDK.DLL'

    

///*****************************************************/
// ������ 	: CameraGetImageBufferEx
// ��������	: ���һ֡ͼ�����ݡ��ýӿڻ�õ�ͼ���Ǿ���������RGB��ʽ���ú������ú�
//			  ����Ҫ���� CameraReleaseImageBuffer �ͷţ�Ҳ��Ҫ����free֮��ĺ����ͷ�
//              ���ͷŸú������ص�ͼ�����ݻ�������
// ����	    : hCamera	  ����ľ������CameraInit������á�
//            piWidth    ����ָ�룬����ͼ��Ŀ��
//            piHeight   ����ָ�룬����ͼ��ĸ߶�
//            UINT wTimes ץȡͼ��ĳ�ʱʱ�䡣��λ���롣��
//						  wTimesʱ���ڻ�δ���ͼ����ú���
//						  �᷵�س�ʱ��Ϣ��
// ����ֵ   : �ɹ�ʱ������RGB���ݻ��������׵�ַ;
//            ���򷵻�0��
///*****************************************************/
function CameraGetImageBufferEx(
    hCamera:CameraHandle;
    piWidth:PInteger;
    piHeight:PInteger;
    wTimes:Integer
):PByte; stdcall;external 'MVCAMSDK.DLL'



//*****************************************************/
// ������ 	: CameraSetIspOutFormat
// ��������	: ����CameraGetImageBuffer������ͼ����������ʽ��֧��
//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
//              Ĭ�������CAMERA_MEDIA_TYPE_BGR8��ʽ��
// ����	    : hCamera		����ľ������CameraInit������á� 
//             uFormat	Ҫ�趨��ʽ��CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8	��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//*****************************************************/
function CameraSetIspOutFormat(
    hCamera:CameraHandle;
    uFormat:Integer          
):Integer; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraGetIspOutFormat
// ��������	: ���CameraGetImageBuffer����ͼ����������ʽ��֧��
//              CAMERA_MEDIA_TYPE_MONO8��CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8
//              �Լ�CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
//              (��CameraDefine.h�ж���)5�֣��ֱ��Ӧ8λ�Ҷ�ͼ���24RGB��32λRGB��24λBGR��32λBGR��ɫͼ��
// ����	    : hCamera		����ľ������CameraInit������á� 
//             puFormat	���ص�ǰ�趨�ĸ�ʽ���CAMERA_MEDIA_TYPE_MONO8����CAMERA_MEDIA_TYPE_RGB8��CAMERA_MEDIA_TYPE_RGBA8	��CAMERA_MEDIA_TYPE_BGR8��CAMERA_MEDIA_TYPE_BGRA8
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//*****************************************************/
function CameraGetIspOutFormat(
    hCamera:CameraHandle;
    puFormat:PInteger         
):Integer; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraGetErrorString
// ��������	: ��ô������Ӧ�������ַ���
// ����	    : iStatusCode		�����롣(������CameraStatus.h��)
// ����ֵ   : �ɹ�ʱ�����ش������Ӧ���ַ����׵�ַ;
//            ���򷵻�NULL��
//*****************************************************/
function CameraGetErrorString(
    hCamera:CameraHandle
):PByte; stdcall;external 'MVCAMSDK.DLL'


//*****************************************************/
// ������ 	: CameraSetLedBrightness
// ��������	: ���������LED���ȣ�����LED���ͺţ��˺������ش�����룬��ʾ��֧�֡�
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//             index      LED�Ƶ������ţ���0��ʼ�����ֻ��һ���ɿ������ȵ�LED����ò���Ϊ0 ��
//             uBrightness LED����ֵ����Χ0��255. 0��ʾ�رգ�255������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���򷵻� ��0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//*****************************************************/
function CameraSetLedBrightness(
    hCamera:CameraHandle;
    index:Integer;
    uBrightness:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraGetLedBrightness
// ��������	: ��������LED���ȣ�����LED���ͺţ��˺������ش�����룬��ʾ��֧�֡�
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//             index      LED�Ƶ������ţ���0��ʼ�����ֻ��һ���ɿ������ȵ�LED����ò���Ϊ0 ��
//             uBrightness ָ�룬����LED����ֵ����Χ0��255. 0��ʾ�رգ�255������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���򷵻� ��0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//*****************************************************/
function CameraGetLedBrightness(
    hCamera:CameraHandle;
    index:Integer;
    uBrightness:PInteger          
):Integer; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraEnableTransferRoi
// ��������	: ʹ�ܻ��߽�ֹ����Ķ������书�ܣ������ù��ܵ��ͺţ��˺������ش�����룬��ʾ��֧�֡�
//              �ù�����Ҫ����������˽��ɼ������������з֣�ֻ����ָ���Ķ����������ߴ���֡�ʡ�
//              ��������䵽PC�Ϻ󣬻��Զ�ƴ�ӳ��������棬û�б�����Ĳ��֣����ú�ɫ��䡣
// ����	    : hCamera	    ����ľ������CameraInit������á� 
//             uEnableMask ����ʹ��״̬���룬��Ӧ�ı���λΪ1��ʾʹ�ܡ�0Ϊ��ֹ��ĿǰSDK֧��4���ɱ༭����index��ΧΪ0��3����bit0 ��bit1��bit2��bit3����4�������ʹ��״̬��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��   
//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//*****************************************************/
function CameraEnableTransferRoi(
    hCamera:CameraHandle;
    uEnableMask:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


//*****************************************************/
// ������ 	: CameraSetTransferRoi
// ��������	: �����������Ĳü�����������ˣ�ͼ��Ӵ������ϱ��ɼ��󣬽��ᱻ�ü���ָ�������������ͣ��˺������ش�����룬��ʾ��֧�֡�
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//             index      ROI����������ţ���0��ʼ��
//             X1,Y1      ROI��������Ͻ�����
//             X2,Y2      ROI��������Ͻ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��   
//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//*****************************************************/
function CameraSetTransferRoi(
    hCamera:CameraHandle;
		index:Integer;
		X1:Integer;
		Y1:Integer;
		X2:Integer;
		Y2:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'


//*****************************************************/
// ������ 	: CameraGetTransferRoi
// ��������	: �����������Ĳü�����������ˣ�ͼ��Ӵ������ϱ��ɼ��󣬽��ᱻ�ü���ָ�������������ͣ��˺������ش�����룬��ʾ��֧�֡�
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//             index      ROI����������ţ���0��ʼ��
//             pX1,pY1      ROI��������Ͻ�����
//             pX2,pY2      ROI��������Ͻ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���ڲ�֧�ֶ�����ROI������ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��   
//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//*****************************************************/
function CameraGetTransferRoi(
		hCamera:CameraHandle;
		index:Integer;
		pX1:PInteger;
		pY1:PInteger;
		pX2:PInteger;
		pY2:PInteger
):Integer; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraAlignMalloc
// ��������	: ����һ�ζ�����ڴ�ռ䡣���ܺ�malloc���ƣ���
//						�Ƿ��ص��ڴ�����alignָ�����ֽ�������ġ�
// ����	    : size	   �ռ�Ĵ�С�� 
//            align    ��ַ������ֽ�����
// ����ֵ   : �ɹ�ʱ�����ط�0ֵ����ʾ�ڴ��׵�ַ��ʧ�ܷ���NULL��
//*****************************************************/
function CameraAlignMalloc(
        size:Integer;
        align:Integer
):PByte; stdcall;external 'MVCAMSDK.DLL'

//*****************************************************/
// ������ 	: CameraAlignFree
// ��������	: �ͷ���CameraAlignMalloc����������ڴ�ռ䡣
// ����	    : membuffer	   ��CameraAlignMalloc���ص��ڴ��׵�ַ�� 
// ����ֵ   : �ޡ�
//*****************************************************/
procedure CameraAlignFree(
    membuffer:PByte
); stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������ 	: CameraSetAutoConnect
// ��������	: �����Զ�ʹ������
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//			  bEnable	   ʹ�������������λTRUEʱ��SDK�ڲ��Զ��������Ƿ���ߣ����ߺ��Լ�������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���ڲ�֧�ֵ��ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��   
//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//******************************************************/
function CameraSetAutoConnect(
	hCamera:CameraHandle;
	bEnable:Integer
):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������ 	: CameraGetReConnectCounts
// ��������	: �������Զ������Ĵ�����ǰ����CameraSetAutoConnect ʹ������Զ��������ܡ�Ĭ����ʹ�ܵġ�
// ����	    : hCamera	   ����ľ������CameraInit������á� 
//			 puCounts	   ���ص����Զ������Ĵ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0)����ʾ�������״̬����;
//            ���ڲ�֧�ֵ��ͺţ��ú����᷵�� CAMERA_STATUS_NOT_SUPPORTED(-4) ��ʾ��֧��   
//            ������0ֵ���ο�CameraStatus.h�д�����Ķ��塣
//******************************************************/
function CameraGetReConnectCounts(
	hCamera:CameraHandle;
	puCounts:PCardinal
):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraEvaluateImageDefinition
// �������� : ͼƬ����������
// ����     : hCamera  ����ľ������CameraInit������á�
//			  iAlgorithSel ʹ�õ������㷨,���emEvaluateDefinitionAlgorith�еĶ���
//            pbyIn    ����ͼ�����ݵĻ�������ַ������ΪNULL�� 
//            pFrInfo  ����ͼ���֡ͷ��Ϣ
//			  DefinitionValue ���ص������ȹ�ֵ��Խ��Խ������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraEvaluateImageDefinition(
	hCamera:CameraHandle;
	iAlgorithSel:Integer;
	pbyIn:PByte;
	pFrInfo:PtSdkFrameHead;
	DefinitionValue: PDouble
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraDrawText
// �������� : �������ͼ�������л�������
// ����     : pRgbBuffer ͼ�����ݻ�����
//			  pFrInfo ͼ���֡ͷ��Ϣ
//			  pFontFileName �����ļ���
//			  FontWidth ������
//			  FontHeight ����߶�
//			  pText Ҫ���������
//			  (Left, Top, Width, Height) ���ֵ��������
//			  TextColor ������ɫRGB
//			  uFlags �����־,���emCameraDrawTextFlags�еĶ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraDrawText(
	pRgbBuffer: PByte;
	pFrInfo: PtSdkFrameHead;
	pFontFileName:Pchar;
	FontWidth:Cardinal;
	FontHeight:Cardinal;
	pText:Pchar;
	Left:Integer;
	Top:Integer;
	Width:Cardinal;
	Height:Cardinal;
	TextColor:Cardinal;
	uFlags:Cardinal
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraGigeGetIp
// �������� : ��ȡGIGE�����IP��ַ
// ����     : pCameraInfo ������豸������Ϣ������CameraEnumerateDevice������á� 
//			  CamIp ���IP(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
//			  CamMask �����������(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
//			  CamGateWay �������(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
//			  EtIp ����IP(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
//			  EtMask ������������(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
//			  EtGateWay ��������(ע�⣺���뱣֤����Ļ��������ڵ���16�ֽ�)
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGigeGetIp(
		pCameraInfo:PtSdkCameraDevInfo;
		CamIp:Pchar;
		CamMask:Pchar;
		CamGateWay:Pchar;
		EtIp:Pchar;
		EtMask:Pchar;
		EtGateWay:Pchar
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraGigeSetIp
// �������� : ����GIGE�����IP��ַ
// ����     : pCameraInfo ������豸������Ϣ������CameraEnumerateDevice������á� 
//			  Ip ���IP(�磺192.168.1.100)
//			  SubMask �����������(�磺255.255.255.0)
//			  GateWay �������(�磺192.168.1.1)
//			  bPersistent TRUE: �������Ϊ�̶�IP��FALSE����������Զ�����IP�����Բ���Ip, SubMask, GateWay��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGigeSetIp(
	pCameraInfo:PtSdkCameraDevInfo;
	Ip:Pchar;
	SubMask:Pchar;
	GateWay:Pchar;
	bPersistent:Integer
	):Integer; stdcall;external 'MVCAMSDK.DLL'
	
//******************************************************/
// ������   : CameraGrabber_CreateFromDevicePage
// �������� : ��������б����û�ѡ��Ҫ�򿪵����
// ����     : �������ִ�гɹ����غ���������Grabber
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_CreateFromDevicePage(
	Grabber:PPointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraGrabber_Create
// �������� : ���豸������Ϣ����Grabber
// ����     : Grabber    �������ִ�гɹ����غ���������Grabber����
//			  pDevInfo	��������豸������Ϣ����CameraEnumerateDevice������á� 
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_Create(
	Grabber:PPointer;
	pDevInfo:PtSdkCameraDevInfo
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������   : CameraGrabber_Destroy
// �������� : ����Grabber
// ����     : Grabber
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_Destroy(
	Grabber:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_SetHWnd
// ��������	: ����Ԥ����Ƶ����ʾ����
// ����		: Grabber
//			  hWnd  ���ھ��
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_SetHWnd(
	Grabber:Pointer;
	hWnd:HWND
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_StartLive
// ��������	: ����Ԥ��
// ����		: Grabber
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_StartLive(
	Grabber:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_StopLive
// ��������	: ֹͣԤ��
// ����		: Grabber
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_StopLive(
	Grabber:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_SaveImage
// ��������	: ץͼ
// ����		: Grabber
//			  Image ����ץȡ����ͼ����Ҫ����CameraImage_Destroy�ͷţ�
//			  TimeOut ��ʱʱ�䣨���룩
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_SaveImage(
	Grabber:Pointer;
	Image:PPointer;
	TimeOut:Cardinal
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_SaveImageAsync
// ��������	: �ύһ���첽��ץͼ�����ύ�ɹ����ץͼ��ɻ�ص��û����õ���ɺ���
// ����		: Grabber
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_SaveImageAsync(
	Grabber:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_SetSaveImageCompleteCallback
// ��������	: �����첽��ʽץͼ����ɺ���
// ����		: Grabber
//			  Callback ����ץͼ�������ʱ������
//			  Context ��Callback������ʱ����Ϊ��������Callback
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_SetSaveImageCompleteCallback(
	Grabber:Pointer;
	Callback:pfnCameraGrabberSaveImageComplete;
	Context:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_SetFrameListener
// ��������	: ����֡��������
// ����		: Grabber
//			  Listener �����������˺�������0��ʾ������ǰ֡
//			  Context ��Listener������ʱ����Ϊ��������Listener
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_SetFrameListener(
	Grabber:Pointer;
	Listener:pfnCameraGrabberFrameListener;
	Context:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_GetCameraHandle
// ��������	: ��ȡ������
// ����		: Grabber
//			  hCamera ���ص�������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_GetCameraHandle(
	Grabber:Pointer;
	hCamera:PCameraHandle
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_GetStat
// ��������	: ��ȡ֡ͳ����Ϣ
// ����		: Grabber
//			  stat ���ص�ͳ����Ϣ
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_GetStat(
	Grabber:Pointer;
	stat:PtSdkGrabberStat
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraGrabber_GetCameraDevInfo
// ��������	: ��ȡ���DevInfo
// ����		: Grabber
//			  DevInfo ���ص����DevInfo
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraGrabber_GetCameraDevInfo(
	Grabber:Pointer;
	DevInfo:PtSdkCameraDevInfo
	):Integer; stdcall;external 'MVCAMSDK.DLL'
	
//******************************************************/
// ������	: CameraImage_Create
// ��������	: ����һ���µ�Image
// ����		: Image
//			  pFrameBuffer ֡���ݻ�����
//			  pFrameHead ֡ͷ
//			  bCopy TRUE: ���Ƴ�һ���µ�֡����  FALSE: �����ƣ�ֱ��ʹ��pFrameBufferָ��Ļ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_Create(
	Image:PPointer;
	pFrameBuffer:PByte;
	pFrameHead:PtSdkFrameHead;
	bCopy:Integer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_Destroy
// ��������	: ����Image
// ����		: Image
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_Destroy(
	Image:Pointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_GetData
// ��������	: ��ȡImage����
// ����		: Image
//			  DataBuffer ͼ������
//			  Head ͼ����Ϣ
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_GetData(
	Image:Pointer;
	DataBuffer:Ppbyte;
	Head:PPtSdkFrameHead
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_Draw
// ��������	: ����Image��ָ������
// ����		: Image
//			  hWnd Ŀ�Ĵ���
//			  Algorithm �����㷨  0�����ٵ������Բ�  1���ٶ�����������
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_Draw(
	Image:Pointer;
	hWnd:HWND;
	Algorithm:Integer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_BitBlt
// ��������	: ����Image��ָ�����ڣ������ţ�
// ����		: Image
//			  hWnd Ŀ�Ĵ���
//			  xDst,yDst: Ŀ����ε����Ͻ�����
//			  cxDst,cyDst: Ŀ����εĿ��
//			  xSrc,ySrc: ͼ����ε����Ͻ�����
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_BitBlt(
	Image:Pointer;
	hWnd:HWND;
	xDst:Integer;
	yDst:Integer;
	cxDst:Integer;
	cyDst:Integer;
	xSrc:Integer;
	ySrc:Integer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_SaveAsBmp
// ��������	: ��bmp��ʽ����Image
// ����		: Image
//			  FileName �ļ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_SaveAsBmp(
	Image:Pointer;
	FileName:Pchar
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_SaveAsJpeg
// ��������	: ��jpg��ʽ����Image
// ����		: Image
//			  FileName �ļ���
//			  Quality ��������(1-100)��100Ϊ������ѵ��ļ�Ҳ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_SaveAsJpeg(
	Image:Pointer;
	FileName:Pchar;
	Quality:Byte
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_SaveAsPng
// ��������	: ��png��ʽ����Image
// ����		: Image
//			  FileName �ļ���
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_SaveAsPng(
	Image:Pointer;
	FileName:Pchar
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_SaveAsRaw
// ��������	: ����raw Image
// ����		: Image
//			  FileName �ļ���
//			  Format 0: 8Bit Raw     1: 16Bit Raw
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_SaveAsRaw(
	Image:Pointer;
	FileName:Pchar;
	Format:Integer
	):Integer; stdcall;external 'MVCAMSDK.DLL'

//******************************************************/
// ������	: CameraImage_IPicture
// ��������	: ��Image����һ��IPicture
// ����		: Image
//			  Picture �´�����IPicture
// ����ֵ   : �ɹ�ʱ������CAMERA_STATUS_SUCCESS (0);
//            ���򷵻ط�0ֵ�Ĵ�����,��ο�CameraStatus.h
//            �д�����Ķ��塣
//******************************************************/
function CameraImage_IPicture(
	Image:Pointer;
	NewPic:PPointer
	):Integer; stdcall;external 'MVCAMSDK.DLL'


implementation
//function CameraSdkInit;external 'MVCAMSDK.DLL' name 'CameraSdkInit';


end.
