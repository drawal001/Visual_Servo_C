unit CameraDefine;
//BIG5 TRANS ALLOWED
interface

//----------------------�Զ�����������---------------------------------------------
type
CameraHandle = Integer;         //������
PCameraHandle = ^CameraHandle;  //��������ָ��
Ppbyte = ^PByte;
Ppchar = ^PChar;
PLongBool = ^LongBool;
HWND = LongWord;
PPointer = ^Pointer;

//----------------------ת��C++��(CameraDefine.h)��ö������-----------------------
const

//ͼ����任�ķ�ʽ emSdkLutMode
LUTMODE_PARAM_GEN = 0;   //ͨ�����ڲ�����̬����LUT��
LUTMODE_PRESET    = 1;   //ʹ��Ԥ���LUT��
LUTMODE_USER_DEF  = 2;   //ʹ���û��Զ����LUT��

//�������Ƶ������ emSdkRunMode
RUNMODE_PLAY  = 0;     //����Ԥ��������ͼ�����ʾ�������������ڴ���ģʽ�����ȴ�����֡�ĵ�����
RUNMODE_PAUSE = 1;     //��ͣ������ͣ�����ͼ�������ͬʱҲ����ȥ����ͼ��
RUNMODE_STOP  = 2;     //ֹͣ�������������ʼ��������ʹ���ֹͣģʽ

//SDK�ڲ���ʾ�ӿڵ���ʾ��ʽ emSdkDisplayMode
DISPLAYMODE_SCALE = 0;   //������ʾģʽ�����ŵ���ʾ�ؼ��ĳߴ�
DISPLAYMODE_REAL  = 1;   //1:1��ʾģʽ����ͼ��ߴ������ʾ�ؼ��ĳߴ�ʱ��ֻ��ʾ�ֲ�  

//¼��״̬ emSdkRecordMode
RECORD_STOP  = 0;      //ֹͣ
RECORD_START = 1;      //¼����
RECORD_PAUSE = 2;      //��ͣ

//ͼ��ľ������ emSdkMirrorDirection
MIRROR_DIRECTION_HORIZONTAL = 0;   //ˮƽ����
MIRROR_DIRECTION_VERTICAL   = 1;   //��ֱ����

//�����Ƶ��֡�� emSdkFrameSpeed
FRAME_SPEED_LOW    = 0;   //����ģʽ
FRAME_SPEED_NORMAL = 1;   //��ͨģʽ
FRAME_SPEED_HIGH   = 2;   //����ģʽ(��Ҫ�ϸߵĴ������,���豸���������ʱ���֡�ʵ��ȶ�����Ӱ��)
FRAME_SPEED_SUPER  = 3;   //������ģʽ(��Ҫ�ϸߵĴ������,���豸���������ʱ���֡�ʵ��ȶ�����Ӱ��)

//�����ļ��ĸ�ʽ���� emSdkFileType
FILE_JPG = 1;         //JPG
FILE_BMP = 2;         //BMP
FILE_RAW = 4;         //��������bayer��ʽ�ļ�,���ڲ�֧��bayer��ʽ���������޷�����Ϊ�ø�ʽ
FILE_PNG = 8;         //PNG

//����е�ͼ�񴫸����Ĺ���ģʽ emSdkSnapMode
CONTINUATION     = 0;     //�����ɼ�ģʽ
SOFT_TRIGGER     = 1;     //�������ģʽ�����������ָ��󣬴�������ʼ�ɼ�ָ��֡����ͼ�񣬲ɼ���ɺ�ֹͣ���
EXTERNAL_TRIGGER = 2;     //Ӳ������ģʽ�������յ��ⲿ�źţ���������ʼ�ɼ�ָ��֡����ͼ�񣬲ɼ���ɺ�ֹͣ���

//�Զ��ع�ʱ��Ƶ����Ƶ�� emSdkLightFrequency
LIGHT_FREQUENCY_50HZ = 0; //50HZ,һ��ĵƹⶼ��50HZ
LIGHT_FREQUENCY_60HZ = 1; //60HZ,��Ҫ��ָ��ʾ����

//��������ò�������ΪA,B,C,D 4����б��档emSdkParameterTeam
PARAMETER_TEAM_DEFAULT = 0xff;
PARAMETER_TEAM_A       = 0;
PARAMETER_TEAM_B       = 1;
PARAMETER_TEAM_C       = 2;
PARAMETER_TEAM_D       = 3;

///*emSdkParameterMode �����������ģʽ���������ط�Ϊ���ļ��ʹ��豸�������ַ�ʽ
//PARAM_MODE_BY_MODEL:����ͬ�ͺŵ��������ABCD��������ļ����޸�
//                   һ̨����Ĳ����ļ�����Ӱ�쵽����ͬ�ͺŵ�
//                   ����������ء�                   
//PARAM_MODE_BY_NAME:�����豸����ͬ�����������ABCD��������ļ���
//                Ĭ������£���������ֻ����ĳ�ͺ�һ̨���ʱ��
//                 �豸������һ���ģ�����ϣ��ĳһ̨����ܹ�����
//                 ��ͬ�Ĳ����ļ��������ͨ���޸����豸���ķ�ʽ
//                 ���������ָ���Ĳ����ļ���                 
//PARAM_MODE_BY_SN:��������Լ���Ψһ���к�������ABCD��������ļ���
//               ���к��ڳ���ʱ�Ѿ��̻�������ڣ�ÿ̨��������к�
//               ������ͬ��ͨ�����ַ�ʽ��ÿ̨����Ĳ����ļ����Ƕ����ġ�
//�����Ը����Լ���ʹ�û��������ʹ�����ϼ��ַ�ʽ���ز��������磬��
//MV-U300Ϊ������ϣ����̨���ͺŵ���������� �����϶�����4���������ô��
//ʹ��PARAM_MODE_BY_MODEL��ʽ;�����ϣ������ĳһ̨����ĳ��̨MV-U300��
//ʹ���Լ������ļ��������MV-U300��Ҫʹ����ͬ�Ĳ����ļ�����ôʹ��
//PARAM_MODE_BY_NAME��ʽ;�����ϣ��ÿ̨MV-U300��ʹ�ò�ͬ�Ĳ����ļ�����ô
//ʹ��PARAM_MODE_BY_SN��ʽ��
//�����ļ����ڰ�װĿ¼�� \Camera\Configs Ŀ¼�£���configΪ��׺�����ļ���
//*/
PARAM_MODE_BY_MODEL  = 0;   //��������ͺ������ļ��м��ز���������MV-U300
PARAM_MODE_BY_NAME   = 1;   //�����豸�ǳ�(tSdkCameraDevInfo.acFriendlyName)���ļ��м��ز���������MV-U300,���ǳƿ��Զ���
PARAM_MODE_BY_SN     = 2;   //�����豸��Ψһ���кŴ��ļ��м��ز��������к��ڳ���ʱ�Ѿ�д���豸��ÿ̨���ӵ�в�ͬ�����кš�
PARAM_MODE_IN_DEVICE = 3;   //���豸�Ĺ�̬�洢���м��ز������������е��ͺŶ�֧�ִ�����ж�д�����飬��tSdkCameraCapbility.bParamInDevice����

//SDK���ɵ��������ҳ������ֵ emSdkPropSheetMask
PROP_SHEET_INDEX_EXPOSURE      = 0;
PROP_SHEET_INDEX_ISP_COLOR     = 1;
PROP_SHEET_INDEX_ISP_LUT       = 2;
PROP_SHEET_INDEX_ISP_SHAPE     = 3;   
PROP_SHEET_INDEX_VIDEO_FORMAT  = 4;
PROP_SHEET_INDEX_RESOLUTION    = 5;
PROP_SHEET_INDEX_IO_CTRL       = 6;
PROP_SHEET_INDEX_TRIGGER_SET   = 7;
PROP_SHEET_INDEX_OVERLAY       = 8;
PROP_SHEET_INDEX_DEVICE_INFO   = 9;

//SDK���ɵ��������ҳ��Ļص���Ϣ���� emSdkPropSheetMsg
SHEET_MSG_LOAD_PARAM_DEFAULT  = 0;   //�������ָ���Ĭ�Ϻ󣬴�������Ϣ
SHEET_MSG_LOAD_PARAM_GROUP    = 1;   //����ָ�������飬��������Ϣ
SHEET_MSG_LOAD_PARAM_FROMFILE = 2;   //��ָ���ļ����ز����󣬴�������Ϣ
SHEET_MSG_SAVE_PARAM_GROUP    = 3;   //��ǰ�����鱻����ʱ����������Ϣ

//���ӻ�ѡ��ο����ڵ����� emSdkRefWinType
REF_WIN_AUTO_EXPOSURE = 0;
REF_WIN_WHITE_BALANCE = 1;

//���ӻ�ѡ��ο����ڵ����� emSdkResolutionMode
RES_MODE_PREVIEW  = 0;
RES_MODE_SNAPSHOT = 1;

//��ƽ��ʱɫ��ģʽ emSdkClrTmpMode
CT_MODE_AUTO       = 0;   //�Զ�ʶ��ɫ��
CT_MODE_PRESET     = 1;   //ʹ��ָ����Ԥ��ɫ��
CT_MODE_USER_DEF   = 2;   //�Զ���ɫ��(����;���)

//LUT����ɫͨ�� emSdkLutChannel
LUT_CHANNEL_ALL    = 0;   //R,B,G��ͨ��ͬʱ����
LUT_CHANNEL_RED    = 1;   //��ɫͨ��
LUT_CHANNEL_GREEN  = 2;   //��ɫͨ��
LUT_CHANNEL_BLUE   = 3;   //��ɫͨ��

//ISP����Ԫ emSdkIspProcessor
ISP_PROCESSSOR_PC      = 0;   //ʹ��PC�����ISPģ��
ISP_PROCESSSOR_DEVICE  = 1;   //ʹ������Դ���Ӳ��ISPģ��

//������źſ��Ʒ�ʽ emStrobeControl
STROBE_SYNC_WITH_TRIG_AUTO      = 0;     //�ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš���ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
STROBE_SYNC_WITH_TRIG_MANUAL    = 1;     //�ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)���ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)����Ч���Կ�����(CameraSetStrobePolarity)��
STROBE_ALWAYS_HIGH              = 2;     //ʼ��Ϊ�ߣ�����STROBE�źŵ���������
STROBE_ALWAYS_LOW               = 3;     //ʼ��Ϊ�ͣ�����STROBE�źŵ���������

//Ӳ���ⴥ�����ź����� emExtTrigSignal
EXT_TRIG_LEADING_EDGE   = 0;       //�����ش�����Ĭ��Ϊ�÷�ʽ
EXT_TRIG_TRAILING_EDGE  = 1;       //�½��ش���
EXT_TRIG_HIGH_LEVEL     = 2;       //�ߵ�ƽ����,��ƽ��Ⱦ����ع�ʱ�䣬�������ͺŵ����֧�ֵ�ƽ������ʽ��
EXT_TRIG_LOW_LEVEL      = 3;       //�͵�ƽ����,


//Ӳ���ⴥ��ʱ�Ŀ��ŷ�ʽ emExtTrigShutterMode
EXT_TRIG_EXP_STANDARD    = 0;        //��׼��ʽ��Ĭ��Ϊ�÷�ʽ��
EXT_TRIG_EXP_GRR        = 1;         //ȫ�ָ�λ��ʽ�����ֹ������ŵ�CMOS�ͺŵ����֧�ָ÷�ʽ������ⲿ��е���ţ����Դﵽȫ�ֿ��ŵ�Ч�����ʺ��ĸ����˶�������

//----------------------ת��C++��(CameraDefine.h)�Ľṹ������-----------------------

//���������Ϣ(ֻ�������޸�)
type
PtSdkCameraDevInfo = ^ tSdkCameraDevInfo;
tSdkCameraDevInfo = record
acProductSeries:array[1..32] of char;    // ��Ʒϵ��
acProductName:array[1..32] of char;      // ��Ʒ����
acFriendlyName:array[1..32] of char;     // �ǳƣ��������豸�У��û����Զ�̬�޸ģ��������ֶ����ͬʱʹ��
acLinkName:array[1..32] of char;         // �ڲ�ʹ�õ����ӷ�����
acDriverVersion:array[1..32] of char;    // �����汾
acSensorType:array[1..32] of char;       // sensor����
acPortType:array[1..32] of char;         // �ӿ�����
acSn:array[1..32] of char;               // ��ƷΨһ���к�
uInstance:Cardinal;                       // ���ͺ�����ڸõ����ϵ�ʵ�������ţ���������ͬ�ͺŶ����
end;

//����ķֱ����趨��Χ
type
ptSdkResolutionRange = ^tSdkResolutionRange;
tSdkResolutionRange = record
iHeightMax:Integer;             //ͼ�����߶�
iHeightMin:Integer;             //ͼ����С�߶�
iWidthMax:Integer;              //ͼ�������
iWidthMin:Integer;              //ͼ����С���
uSkipModeMask:Cardinal;         //SKIPģʽ���룬Ϊ0����ʾ��֧��SKIP ��bit0Ϊ1,��ʾ֧��SKIP 2x2 ;bit1Ϊ1����ʾ֧��SKIP 3x3....
uBinSumModeMask:Cardinal;       //BIN(���)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
uBinAverageModeMask:Cardinal;   //BIN(���ֵ)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
uResampleMask:Cardinal;         //Ӳ���ز���������
end;

//����ķֱ�������
type
PtSdkImageResolution = ^tSdkImageResolution;
tSdkImageResolution = record
iIndex:Integer;                    // �����ţ�[0,N]��ʾԤ��ķֱ���(N ΪԤ��ֱ��ʵ���������һ�㲻����20),OXFF ��ʾ�Զ���ֱ���(ROI)
acDescription:array[1..32] of char;// �÷ֱ��ʵ�������Ϣ����Ԥ��ֱ���ʱ����Ϣ��Ч���Զ���ֱ��ʿɺ��Ը���Ϣ
uBinSumMode:Cardinal;              // �Ƿ�BIN�ĳߴ�,16bitΪ1��ʾsum��Ϊ0��ʾaverage����16λ��ʾBIN�����С��Ϊ0��ʾ��ֹBINģʽ,��Χ���ܳ���tSdkResolutionRange��uBinModeMask
uBinAverageMode:Cardinal;          // BIN(���ֵ)��ģʽ,��Χ���ܳ���tSdkResolutionRange��uBinAverageModeMask
uSkipMode:Cardinal;                // �Ƿ�SKIP�ĳߴ磬Ϊ0��ʾ��ֹSKIPģʽ��1��ʾSKIP2X2��2��ʾSKIP3X3���Դ�����,��Χ���ܳ���tSdkResolutionRange��uSkipModeMask
uResampleMask:Cardinal;            // Ӳ���ز���������
iHOffsetFOV:Integer;               // �ɼ��ӳ������Sensor����ӳ����ϽǵĴ�ֱƫ��
iVOffsetFOV:Integer;               // �ɼ��ӳ������Sensor����ӳ����Ͻǵ�ˮƽƫ��
iWidthFOV:Integer;                 // �ɼ��ӳ��Ŀ��
iHeightFOV:Integer;                // �ɼ��ӳ��ĸ߶�
iWidth:Integer;                    // ������������ͼ��Ŀ��
iHeight:Integer;                   // ������������ͼ��ĸ߶�
iWidthZoomHd:Integer;              // Ӳ�����ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
iHeightZoomHd:Integer;             // Ӳ�����ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
iWidthZoomSw:Integer;              // ������ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
iHeightZoomSw:Integer;             // ������ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
end;

//�����ƽ��ģʽ������Ϣ
type
PtSdkColorTemperatureDes = ^tSdkColorTemperatureDes;
tSdkColorTemperatureDes = record
iIndex:Integer;                          // ģʽ������
acDescription:array[1..32] of char;      // ������Ϣ
end;

//���֡��������Ϣ
type
PtSdkFrameSpeed = ^tSdkFrameSpeed;
tSdkFrameSpeed = record
iIndex:Integer;                      // ֡�������ţ�һ��0��Ӧ�ڵ���ģʽ��1��Ӧ����ͨģʽ��2��Ӧ�ڸ���ģʽ
acDescription:array[1..32] of char;  // ������Ϣ      
end;

//����ع⹦�ܷ�Χ����
type
PtSdkExpose = ^tSdkExpose;
tSdkExpose = record           
uiTargetMin:Cardinal;      //�Զ��ع�����Ŀ����Сֵ
uiTargetMax:Cardinal;      //�Զ��ع�����Ŀ�����ֵ
uiAnalogGainMin:Cardinal;  //ģ���������Сֵ����λΪfAnalogGainStep�ж���      
uiAnalogGainMax:Cardinal;  //ģ����������ֵ����λΪfAnalogGainStep�ж���        
fAnalogGainStep:Single;    //ģ������ÿ����1����Ӧ�����ӵķŴ��������磬uiAnalogGainMinһ��Ϊ16��fAnalogGainStepһ��Ϊ0.125����ô��С�Ŵ�������16*0.125 = 2��
uiExposeTimeMin:Cardinal;  //�ֶ�ģʽ�£��ع�ʱ�����Сֵ����λ:�С�����CameraGetExposureLineTime���Ի��һ�ж�Ӧ��ʱ��(΢��),�Ӷ��õ���֡���ع�ʱ��    
uiExposeTimeMax:Cardinal;  //�ֶ�ģʽ�£��ع�ʱ������ֵ����λ:��        
end;

//����ģʽ����
type
PtSdkTrigger = ^tSdkTrigger;
tSdkTrigger = record
iIndex:Integer;                         //ģʽ������
acDescription:array[1..32] of char;     //��ģʽ��������Ϣ
end;

//����ְ���С����(��Ҫ��������������Ч)
type
PtSdkPackLength = ^tSdkPackLength;
tSdkPackLength = record
iIndex:Integer;                         //�ְ���С������
acDescription:array[1..32] of char;     //��Ӧ��������Ϣ     
iPackSize:Cardinal;
end;

//Ԥ���LUT������
type
PtSdkPresetLut = ^tSdkPresetLut;
tSdkPresetLut = record
iIndex:Integer;                         //���
acDescription:array[1..32] of char;     //������Ϣ
end;

//AE�㷨����
type
PtSdkAeAlgorithm = ^tSdkAeAlgorithm;
tSdkAeAlgorithm = record
iIndex:Integer;                         //���
acDescription:array[1..32] of char;     //������Ϣ
end;


//RAWתRGB�㷨����
type
PtSdkBayerDecodeAlgorithm = ^tSdkBayerDecodeAlgorithm;
tSdkBayerDecodeAlgorithm = record
iIndex:Integer;                         //���     
acDescription:array[1..32] of char;     //������Ϣ
end;

//֡��ͳ����Ϣ
type
PtSdkFrameStatistic = ^tSdkFrameStatistic;
tSdkFrameStatistic = record
iTotal:Integer;       //��ǰ�ɼ�����֡������������֡��
iCapture:Integer;     //��ǰ�ɼ�����Ч֡������
iLost:Integer;        //��ǰ��֡������
end;

//��������ͼ�����ݸ�ʽ
type
PtSdkMediaType = ^tSdkMediaType;
tSdkMediaType = record
iIndex:Integer;                     //��ʽ������
acDescription:array[1..32] of char; //������Ϣ
iMediaType:Cardinal;                //��Ӧ��ͼ���ʽ���룬��CAMERA_MEDIA_TYPE_BAYGR8���ڱ��ļ����ж��塣
end;

//٤����趨��Χ
type
PtGammaRange = ^tGammaRange; 
tGammaRange = record
iMin:Integer;                 //��Сֵ
iMax:Integer;                 //���ֵ
end;

//�Աȶȵ��趨��Χ
type
PtContrastRange = ^tContrastRange;
tContrastRange = record
iMin:Integer;     //��Сֵ
iMax:Integer;     //���ֵ
end;

//RGB��ͨ������������趨��Χ
type
PtRgbGainRange = ^tRgbGainRange;
tRgbGainRange = record
iRGainMin:Integer;    //��ɫ�������Сֵ
iRGainMax:Integer;    //��ɫ��������ֵ
iGGainMin:Integer;    //��ɫ�������Сֵ
iGGainMax:Integer;    //��ɫ��������ֵ
iBGainMin:Integer;    //��ɫ�������Сֵ
iBGainMax:Integer;    //��ɫ��������ֵ
end;

//���Ͷ��趨�ķ�Χ
type
PtSaturationRange = ^tSaturationRange;
tSaturationRange = record  
iMin:Integer;   //��Сֵ
iMax:Integer;   //���ֵ
end;

//�񻯵��趨��Χ
type
PtSharpnessRange= ^tSharpnessRange;
tSharpnessRange = record  
iMin:Integer;   //��Сֵ
iMax:Integer;   //���ֵ
end;

//ISPģ���ʹ����Ϣ
type
PtSdkIspCapacity = ^tSdkIspCapacity;
tSdkIspCapacity = record
bMonoSensor:LongBool;         //��ʾ���ͺ�����Ƿ�Ϊ�ڰ����,����Ǻڰ����������ɫ��صĹ��ܶ��޷�����
bWbOnce:LongBool;             //��ʾ���ͺ�����Ƿ�֧���ֶ���ƽ�⹦��
bAutoWb:LongBool;             //��ʾ���ͺ�����Ƿ�֧���Զ���ƽ�⹦��
bAutoExposure:LongBool;       //��ʾ���ͺ�����Ƿ�֧���Զ��ع⹦��
bManualExposure:LongBool;     //��ʾ���ͺ�����Ƿ�֧���ֶ��ع⹦��
bAntiFlick:LongBool;          //��ʾ���ͺ�����Ƿ�֧�ֿ�Ƶ������
bDeviceIsp:LongBool;          //��ʾ���ͺ�����Ƿ�֧��Ӳ��ISP����
bForceUseDeviceIsp:LongBool;  //bDeviceIsp��bForceUseDeviceIspͬʱΪTRUEʱ����ʾǿ��ֻ��Ӳ��ISP������ȡ����
bZoomHD:LongBool;             //���Ӳ���Ƿ�֧��ͼ���������(ֻ������С)��
end;

// �������ϵ��豸������Ϣ����Щ��Ϣ�������ڶ�̬����UI */
type
PtSdkCameraCapbility = ^tSdkCameraCapbility;
tSdkCameraCapbility = record
pTriggerDesc:^tSdkTrigger;                // ����ģʽ
iTriggerDesc:Integer;                     // ����ģʽ�ĸ�������pTriggerDesc����Ĵ�С
pImageSizeDesc:^tSdkImageResolution;      // Ԥ��ֱ���ѡ��
iImageSizeDesc:Integer;                   // Ԥ��ֱ��ʵĸ�������pImageSizeDesc����Ĵ�С
pClrTempDesc:^tSdkColorTemperatureDes;    // Ԥ��ɫ��ģʽ�����ڰ�ƽ��
iClrTempDesc:Integer;
pMediaTypeDesc:^tSdkMediaType;            // ������ͼ���ʽ
iMediaTypdeDesc:Integer;                  // ������ͼ���ʽ�������������pMediaTypeDesc����Ĵ�С��
pFrameSpeedDesc:^tSdkFrameSpeed;          // �ɵ���֡�����ͣ���Ӧ��������ͨ ���� �ͳ��������ٶ�����
iFrameSpeedDesc:Integer;                  // �ɵ���֡�����͵ĸ�������pFrameSpeedDesc����Ĵ�С��
pPackLenDesc:^tSdkPackLength;             // ��������ȣ�һ�����������豸
iPackLenDesc:Integer;                     // �ɹ�ѡ��Ĵ���ְ����ȵĸ�������pPackLenDesc����Ĵ�С��
iOutputIoCounts:Integer;                  // �ɱ������IO�ĸ���
iInputIoCounts:Integer;                   // �ɱ������IO�ĸ���
pPresetLutDesc:^tSdkPresetLut;            // ���Ԥ���LUT��
iPresetLut:Integer;                       // ���Ԥ���LUT��ĸ�������pPresetLutDesc����Ĵ�С
iUserDataMaxLen:Integer;                  // ָʾ����������ڱ����û�����������󳤶ȡ�Ϊ0��ʾ�ޡ�
bParamInDevice:LongBool;                  // ָʾ���豸�Ƿ�֧�ִ��豸�ж�д�����顣1Ϊ֧�֣�0��֧�֡�
pAeAlmSwDesc:^tSdkAeAlgorithm;            // ����Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
iAeAlmSwDesc:Integer;                     // ����Զ��ع��㷨����
pAeAlmHdDesc:^tSdkAeAlgorithm;            // Ӳ���Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
iAeAlmHdDesc:Integer;                     // Ӳ���Զ��ع��㷨����
pBayerDecAlmSwDesc:^tSdkBayerDecodeAlgorithm;   // ���Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
iBayerDecAlmSwDesc:Integer;                     // ���Bayerת��ΪRGB���ݵ��㷨����
pBayerDecAlmHdDesc:^tSdkBayerDecodeAlgorithm;   // Ӳ��Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
iBayerDecAlmHdDesc:Integer;                     // Ӳ��Bayerת��ΪRGB���ݵ��㷨����
// ͼ������ĵ��ڷ�Χ����,���ڶ�̬����UI
sExposeDesc:tSdkExpose;                 // �ع�ķ�Χֵ
sResolutionRange:tSdkResolutionRange;   // �ֱ��ʷ�Χ���� 
sRgbGainRange:tRgbGainRange;            // ͼ���������淶Χ����
sSaturationRange:tSaturationRange;      // ���Ͷȷ�Χ����
sGammaRange:tGammaRange;                // ٤��Χ����
sContrastRange:tContrastRange;          // �Աȶȷ�Χ����
sSharpnessRange:tSharpnessRange;        // �񻯷�Χ����
sIspCapacity:tSdkIspCapacity;           // ISP��������
end;

//ͼ��֡ͷ��Ϣ
type
PtSdkFrameHead = ^tSdkFrameHead;
PPtSdkFrameHead = ^PtSdkFrameHead;
tSdkFrameHead = record
uiMediaType:Cardinal;   // ͼ���ʽ,Image Format
uBytes:Cardinal;        // ͼ�������ֽ���,Total bytes
iWidth:Integer;         // ��� Image width
iHeight:Integer;        // �߶� Image height
iWidthZoomSw:Integer;   // ������ŵĿ��,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
iHeightZoomSw:Integer;  // ������ŵĸ߶�,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
bIsTrigger:LongBool;    // ָʾ�Ƿ�Ϊ����֡ is trigger
uiTimeStamp:Cardinal;   // ��֡�Ĳɼ�ʱ�䣬��λ0.1����
uiExpTime:Cardinal;     // ��ǰͼ����ع�ֵ����λΪ΢��us
fAnalogGain:Single;     // ��ǰͼ���ģ�����汶��
iGamma:Integer;         // ��֡ͼ���٤���趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
iContrast:Integer;      // ��֡ͼ��ĶԱȶ��趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
iSaturation:Integer;    // ��֡ͼ��ı��Ͷ��趨ֵ�����ںڰ���������壬Ϊ0
fRgain:Single;          // ��֡ͼ����ĺ�ɫ�������汶�������ںڰ���������壬Ϊ1
fGgain:Single;          // ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
fBgain:Single;          // ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
end;

//ͼ��֡����
type
PtSdkFrame = ^tSdkFrame;
tSdkFrame = record
head:tSdkFrameHead;     //֡ͷ
pBuffer:^Byte;          //������
end;

//ͼ��ץȡ�ص�����
type
CAMERA_SNAP_PROC = procedure (
hCamera:CameraHandle;
pFrameBuffer:PByte;
pFrameHead:PtSdkFrameHead;
pContext:Pointer
); stdcall;

//SDK���ɵ��������ҳ�����Ϣ�ص���������
type
CAMERA_PAGE_MSG_PROC = procedure (
hCamera:CameraHandle;
MSG:Cardinal;
uParam:Cardinal;
pContext:Pointer
); stdcall;

//////////////////////////////////////////////////////////////////////////
// Grabber ���

// Grabberͳ����Ϣ
type
PtSdkGrabberStat = ^tSdkGrabberStat;
tSdkGrabberStat = record
	Width: Integer;
	Height: Integer;	// ֡ͼ���С
	Disp: Integer;		// ��ʾ֡����
	Capture: Integer;	// �ɼ�����Ч֡������
	Lost: Integer;		// ��֡������
	Error: Integer;		// ��֡������
	DispFps: Single;	// ��ʾ֡��
	CapFps: Single;		// ����֡��
end;

// ͼ�񲶻�Ļص���������
type
pfnCameraGrabberFrameListener = function(
	Grabber:Pointer;
	Phase:Integer;
	pFrameBuffer:PByte;
	pFrameHead:PtSdkFrameHead;
	Context:Pointer
):Integer; stdcall;

// �첽ץͼ�Ļص���������
type
pfnCameraGrabberSaveImageComplete = procedure(
	Grabber:Pointer;
	Image:Pointer;	// ��Ҫ����CameraImage_Destroy�ͷ�
	Status:Integer;
	Context:Pointer
); stdcall;

//----------------------------IMAGE FORMAT DEFINE------------------------------------
//----------------------------ͼ���ʽ����-------------------------------------------
const
CAMERA_MEDIA_TYPE_MONO                           =$01000000;
CAMERA_MEDIA_TYPE_RGB                            =$02000000;
CAMERA_MEDIA_TYPE_COLOR                          =$02000000;
CAMERA_MEDIA_TYPE_CUSTOM                         =$80000000;
CAMERA_MEDIA_TYPE_COLOR_MASK                     =$FF000000;
CAMERA_MEDIA_TYPE_OCCUPY1BIT                     =$00010000;
CAMERA_MEDIA_TYPE_OCCUPY2BIT                     =$00020000;
CAMERA_MEDIA_TYPE_OCCUPY4BIT                     =$00040000;
CAMERA_MEDIA_TYPE_OCCUPY8BIT                     =$00080000;
CAMERA_MEDIA_TYPE_OCCUPY10BIT                    =$000A0000;
CAMERA_MEDIA_TYPE_OCCUPY12BIT                    =$000C0000;
CAMERA_MEDIA_TYPE_OCCUPY16BIT                    =$00100000;
CAMERA_MEDIA_TYPE_OCCUPY24BIT                    =$00180000;
CAMERA_MEDIA_TYPE_OCCUPY32BIT                    =$00200000;
CAMERA_MEDIA_TYPE_OCCUPY36BIT                    =$00240000;
CAMERA_MEDIA_TYPE_OCCUPY48BIT                    =$00300000;
CAMERA_MEDIA_TYPE_EFFECTIVE_PIXEL_SIZE_MASK      =$00FF0000;
CAMERA_MEDIA_TYPE_EFFECTIVE_PIXEL_SIZE_SHIFT     =16;
CAMERA_MEDIA_TYPE_ID_MASK                        =$0000FFFF;
CAMERA_MEDIA_TYPE_COUNT                          =$46;

//*mono*/
CAMERA_MEDIA_TYPE_MONO1P             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY1BIT or  $0037);
CAMERA_MEDIA_TYPE_MONO2P             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY2BIT or  $0038);
CAMERA_MEDIA_TYPE_MONO4P             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY4BIT or  $0039);
CAMERA_MEDIA_TYPE_MONO8              =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $0001);
CAMERA_MEDIA_TYPE_MONO8S             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $0002);
CAMERA_MEDIA_TYPE_MONO10             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0003);
CAMERA_MEDIA_TYPE_MONO10_PACKED      =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0004);
CAMERA_MEDIA_TYPE_MONO12             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0005);
CAMERA_MEDIA_TYPE_MONO12_PACKED      =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0006);
CAMERA_MEDIA_TYPE_MONO14             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0025);
CAMERA_MEDIA_TYPE_MONO16             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0007);

//*Bayer */
CAMERA_MEDIA_TYPE_BAYGR8             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $0008);
CAMERA_MEDIA_TYPE_BAYRG8             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $0009);
CAMERA_MEDIA_TYPE_BAYGB8             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $000A);
CAMERA_MEDIA_TYPE_BAYBG8             =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY8BIT or  $000B);

CAMERA_MEDIA_TYPE_BAYGR10_MIPI       =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY10BIT or  $0026);
CAMERA_MEDIA_TYPE_BAYRG10_MIPI       =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY10BIT or  $0027);
CAMERA_MEDIA_TYPE_BAYGB10_MIPI       =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY10BIT or  $0028);
CAMERA_MEDIA_TYPE_BAYBG10_MIPI       =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY10BIT or  $0029);


CAMERA_MEDIA_TYPE_BAYGR10            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $000C);
CAMERA_MEDIA_TYPE_BAYRG10            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $000D);
CAMERA_MEDIA_TYPE_BAYGB10            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $000E);
CAMERA_MEDIA_TYPE_BAYBG10            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $000F);

CAMERA_MEDIA_TYPE_BAYGR12            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0010);
CAMERA_MEDIA_TYPE_BAYRG12            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0011);
CAMERA_MEDIA_TYPE_BAYGB12            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0012);
CAMERA_MEDIA_TYPE_BAYBG12            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0013);


CAMERA_MEDIA_TYPE_BAYGR10_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0026);
CAMERA_MEDIA_TYPE_BAYRG10_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0027);
CAMERA_MEDIA_TYPE_BAYGB10_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0028);
CAMERA_MEDIA_TYPE_BAYBG10_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0029);

CAMERA_MEDIA_TYPE_BAYGR12_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $002A);
CAMERA_MEDIA_TYPE_BAYRG12_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $002B);
CAMERA_MEDIA_TYPE_BAYGB12_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $002C);
CAMERA_MEDIA_TYPE_BAYBG12_PACKED     =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $002D);

CAMERA_MEDIA_TYPE_BAYGR16            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $002E);
CAMERA_MEDIA_TYPE_BAYRG16            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $002F);
CAMERA_MEDIA_TYPE_BAYGB16            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0030);
CAMERA_MEDIA_TYPE_BAYBG16            =(CAMERA_MEDIA_TYPE_MONO or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0031);

//*RGB */
CAMERA_MEDIA_TYPE_RGB8               =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $0014);
CAMERA_MEDIA_TYPE_BGR8               =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $0015);
CAMERA_MEDIA_TYPE_RGBA8              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY32BIT or  $0016);
CAMERA_MEDIA_TYPE_BGRA8              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY32BIT or  $0017);
CAMERA_MEDIA_TYPE_RGB10              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0018);
CAMERA_MEDIA_TYPE_BGR10              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0019);
CAMERA_MEDIA_TYPE_RGB12              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $001A);
CAMERA_MEDIA_TYPE_BGR12              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $001B);
CAMERA_MEDIA_TYPE_RGB16              =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0033);
CAMERA_MEDIA_TYPE_RGB10V1_PACKED     =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY32BIT or  $001C);
CAMERA_MEDIA_TYPE_RGB10P32           =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY32BIT or  $001D);
CAMERA_MEDIA_TYPE_RGB12V1_PACKED     =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY36BIT or  $0034);
CAMERA_MEDIA_TYPE_RGB565P            =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0035);
CAMERA_MEDIA_TYPE_BGR565P            =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0036);

//*YUV and YCbCr*/
CAMERA_MEDIA_TYPE_YUV411_8_UYYVYY    =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $001E);
CAMERA_MEDIA_TYPE_YUV422_8_UYVY      =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $001F);
CAMERA_MEDIA_TYPE_YUV422_8           =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0032);
CAMERA_MEDIA_TYPE_YUV8_UYV           =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $0020);
CAMERA_MEDIA_TYPE_YCBCR8_CBYCR       =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $003A);
//CAMERA_MEDIA_TYPE_YCBCR422_8 : YYYYCbCrCbCr
CAMERA_MEDIA_TYPE_YCBCR422_8         =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $003B);
CAMERA_MEDIA_TYPE_YCBCR422_8_CBYCRY      =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0043);
CAMERA_MEDIA_TYPE_YCBCR411_8_CBYYCRYY    =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $003C);
CAMERA_MEDIA_TYPE_YCBCR601_8_CBYCR       =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $003D);
CAMERA_MEDIA_TYPE_YCBCR601_422_8         =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $003E);
CAMERA_MEDIA_TYPE_YCBCR601_422_8_CBYCRY  =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0044);
CAMERA_MEDIA_TYPE_YCBCR601_411_8_CBYYCRYY    =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $003F);
CAMERA_MEDIA_TYPE_YCBCR709_8_CBYCR           =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $0040);
CAMERA_MEDIA_TYPE_YCBCR709_422_8             =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0041);
CAMERA_MEDIA_TYPE_YCBCR709_422_8_CBYCRY      =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY16BIT or  $0045);
CAMERA_MEDIA_TYPE_YCBCR709_411_8_CBYYCRYY    =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY12BIT or  $0042);

//*RGB Planar */
CAMERA_MEDIA_TYPE_RGB8_PLANAR        =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY24BIT or  $0021);
CAMERA_MEDIA_TYPE_RGB10_PLANAR       =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0022);
CAMERA_MEDIA_TYPE_RGB12_PLANAR       =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0023);
CAMERA_MEDIA_TYPE_RGB16_PLANAR       =(CAMERA_MEDIA_TYPE_COLOR or  CAMERA_MEDIA_TYPE_OCCUPY48BIT or  $0024);
implementation

end.
