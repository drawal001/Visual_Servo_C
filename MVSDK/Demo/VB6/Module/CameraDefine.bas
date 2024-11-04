'///BIG5 TRANS ALLOWED

Attribute VB_Name = "CameraDefine"

'ͼ����任�ķ�ʽ emSdkLutMode
Public Const LUTMODE_PARAM_GEN = 0   'ͨ�����ڲ�����̬����LUT��
Public Const LUTMODE_PRESET = 1      'ʹ��Ԥ���LUT��
Public Const LUTMODE_USER_DEF = 2    'ʹ���û��Զ����LUT��

'�������Ƶ������ emSdkRunMode
Public Const RUNMODE_PLAY = 0         '����Ԥ��������ͼ�����ʾ�������������ڴ���ģʽ�����ȴ�����֡�ĵ�����
Public Const RUNMODE_PAUSE = 1        '��ͣ������ͣ�����ͼ�������ͬʱҲ����ȥ����ͼ��
Public Const RUNMODE_STOP = 2         'ֹͣ�������������ʼ��������ʹ���ֹͣģʽ

'SDK�ڲ���ʾ�ӿڵ���ʾ��ʽ emSdkDisplayMode
Public Const DISPLAYMODE_SCALE = 0    '������ʾģʽ�����ŵ���ʾ�ؼ��ĳߴ�
Public Const DISPLAYMODE_REAL = 1     '1:1��ʾģʽ����ͼ��ߴ������ʾ�ؼ��ĳߴ�ʱ��ֻ��ʾ�ֲ�

'¼��״̬ emSdkRecordMode
Public Const RECORD_STOP = 0          'ֹͣ
Public Const RECORD_START = 1         '¼����
Public Const RECORD_PAUSE = 2         '��ͣ

'ͼ��ľ������ emSdkMirrorDirection
Public Const MIRROR_DIRECTION_HORIZONTAL = 0    'ˮƽ����
Public Const MIRROR_DIRECTION_VERTICAL = 1      '��ֱ����

'�����Ƶ��֡�� emSdkFrameSpeed
Public Const FRAME_SPEED_LOW = 0        '����ģʽ
Public Const FRAME_SPEED_NORMAL = 1     '��ͨģʽ
Public Const FRAME_SPEED_HIGH = 2       '����ģʽ(��Ҫ�ϸߵĴ������,���豸���������ʱ���֡�ʵ��ȶ�����Ӱ��)
Public Const FRAME_SPEED_SUPER = 3      '������ģʽ(��Ҫ�ϸߵĴ������,���豸���������ʱ���֡�ʵ��ȶ�����Ӱ��)

'�����ļ��ĸ�ʽ���� emSdkFileType
Public Const FILE_JPG = 1               'JPG
Public Const FILE_BMP = 2               'BMP
Public Const FILE_RAW = 4               '��������bayer��ʽ�ļ�,���ڲ�֧��bayer��ʽ���������޷�����Ϊ�ø�ʽ
Public Const FILE_PNG = 8               'PNG

'����е�ͼ�񴫸����Ĺ���ģʽ emSdkSnapMode
Public Const CONTINUATION = 0           '�����ɼ�ģʽ
Public Const SOFT_TRIGGER = 1           '�������ģʽ�����������ָ��󣬴�������ʼ�ɼ�ָ��֡����ͼ�񣬲ɼ���ɺ�ֹͣ���
Public Const EXTERNAL_TRIGGER = 2       'Ӳ������ģʽ�������յ��ⲿ�źţ���������ʼ�ɼ�ָ��֡����ͼ�񣬲ɼ���ɺ�ֹͣ���

'�Զ��ع�ʱ��Ƶ����Ƶ�� emSdkLightFrequency
Public Const LIGHT_FREQUENCY_50HZ = 0   '50HZ,һ��ĵƹⶼ��50HZ
Public Const LIGHT_FREQUENCY_60HZ = 1   '60HZ,��Ҫ��ָ��ʾ����

'��������ò�������ΪA,B,C,D 4����б��档emSdkParameterTeam
Public Const PARAMETER_TEAM_DEFAULT = &HFF
Public Const PARAMETER_TEAM_A = 0
Public Const PARAMETER_TEAM_B = 1
Public Const PARAMETER_TEAM_C = 2
Public Const PARAMETER_TEAM_D = 3

'/*emSdkParameterMode �����������ģʽ���������ط�Ϊ���ļ��ʹ��豸�������ַ�ʽ
'PARAM_MODE_BY_MODEL:����ͬ�ͺŵ��������ABCD��������ļ����޸�
'                   һ̨����Ĳ����ļ�����Ӱ�쵽����ͬ�ͺŵ�
'                   ����������ء�
'PARAM_MODE_BY_NAME:�����豸����ͬ�����������ABCD��������ļ���
'                Ĭ������£���������ֻ����ĳ�ͺ�һ̨���ʱ��
'                 �豸������һ���ģ�����ϣ��ĳһ̨����ܹ�����
'                 ��ͬ�Ĳ����ļ��������ͨ���޸����豸���ķ�ʽ
'                 ���������ָ���Ĳ����ļ���
'PARAM_MODE_BY_SN:��������Լ���Ψһ���к�������ABCD��������ļ���
'               ���к��ڳ���ʱ�Ѿ��̻�������ڣ�ÿ̨��������к�
'               ������ͬ��ͨ�����ַ�ʽ��ÿ̨����Ĳ����ļ����Ƕ����ġ�
'�����Ը����Լ���ʹ�û��������ʹ�����ϼ��ַ�ʽ���ز��������磬��
'MV-U300Ϊ������ϣ����̨���ͺŵ���������� �����϶�����4���������ô��
'ʹ��PARAM_MODE_BY_MODEL��ʽ;�����ϣ������ĳһ̨����ĳ��̨MV-U300��
'ʹ���Լ������ļ��������MV-U300��Ҫʹ����ͬ�Ĳ����ļ�����ôʹ��
'PARAM_MODE_BY_NAME��ʽ;�����ϣ��ÿ̨MV-U300��ʹ�ò�ͬ�Ĳ����ļ�����ô
'ʹ��PARAM_MODE_BY_SN��ʽ��
'�����ļ����ڰ�װĿ¼�� \Camera\Configs Ŀ¼�£���configΪ��׺�����ļ���
'*/
Public Const PARAM_MODE_BY_MODEL = 0     '��������ͺ������ļ��м��ز���������MV-U300
Public Const PARAM_MODE_BY_NAME = 1      '�����豸�ǳ�(tSdkCameraDevInfo.acFriendlyName)���ļ��м��ز���������MV-U300,���ǳƿ��Զ���
Public Const PARAM_MODE_BY_SN = 2        '�����豸��Ψһ���кŴ��ļ��м��ز��������к��ڳ���ʱ�Ѿ�д���豸��ÿ̨���ӵ�в�ͬ�����кš�
Public Const PARAM_MODE_IN_DEVICE = 3    '���豸�Ĺ�̬�洢���м��ز������������е��ͺŶ�֧�ִ�����ж�д�����飬��tSdkCameraCapbility.bParamInDevice����

'SDK���ɵ��������ҳ������ֵ emSdkPropSheetMask
Public Const PROP_SHEET_INDEX_EXPOSURE = 0
Public Const PROP_SHEET_INDEX_ISP_COLOR = 1
Public Const PROP_SHEET_INDEX_ISP_LUT = 2
Public Const PROP_SHEET_INDEX_ISP_SHAPE = 3
Public Const PROP_SHEET_INDEX_VIDEO_FORMAT = 4
Public Const PROP_SHEET_INDEX_RESOLUTION = 5
Public Const PROP_SHEET_INDEX_IO_CTRL = 6
Public Const PROP_SHEET_INDEX_TRIGGER_SET = 7
Public Const PROP_SHEET_INDEX_OVERLAY = 8
Public Const PROP_SHEET_INDEX_DEVICE_INFO = 9

'SDK���ɵ��������ҳ��Ļص���Ϣ���� emSdkPropSheetMsg
Public Const SHEET_MSG_LOAD_PARAM_DEFAULT = 0    '�������ָ���Ĭ�Ϻ󣬴�������Ϣ
Public Const SHEET_MSG_LOAD_PARAM_GROUP = 1      '����ָ�������飬��������Ϣ
Public Const SHEET_MSG_LOAD_PARAM_FROMFILE = 2   '��ָ���ļ����ز����󣬴�������Ϣ
Public Const SHEET_MSG_SAVE_PARAM_GROUP = 3      '��ǰ�����鱻����ʱ����������Ϣ

'���ӻ�ѡ��ο����ڵ����� emSdkRefWinType
Public Const REF_WIN_AUTO_EXPOSURE = 0
Public Const REF_WIN_WHITE_BALANCE = 1

'���ӻ�ѡ��ο����ڵ����� emSdkResolutionMode
Public Const RES_MODE_PREVIEW = 0
Public Const RES_MODE_SNAPSHOT = 1

'��ƽ��ʱɫ��ģʽ emSdkClrTmpMode
Public Const CT_MODE_AUTO = 0        '�Զ�ʶ��ɫ��
Public Const CT_MODE_PRESET = 1      'ʹ��ָ����Ԥ��ɫ��
Public Const CT_MODE_USER_DEF = 2    '�Զ���ɫ��(����;���)

'LUT����ɫͨ�� emSdkLutChannel
Public Const LUT_CHANNEL_ALL = 0     'R,B,G��ͨ��ͬʱ����
Public Const LUT_CHANNEL_RED = 1     '��ɫͨ��
Public Const LUT_CHANNEL_GREEN = 2   '��ɫͨ��
Public Const LUT_CHANNEL_BLUE = 3    '��ɫͨ��

'ISP����Ԫ emSdkIspProcessor
Public Const ISP_PROCESSSOR_PC = 0       'ʹ��PC�����ISPģ��
Public Const ISP_PROCESSSOR_DEVICE = 1   'ʹ������Դ���Ӳ��ISPģ��



'������źſ��Ʒ�ʽ emStrobeControl
Public Const   STROBE_SYNC_WITH_TRIG_AUTO = 0       '�ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš���ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
Public Const   STROBE_SYNC_WITH_TRIG_MANUAL = 1     '�ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)���ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)����Ч���Կ�����(CameraSetStrobePolarity)��
Public Const   STROBE_ALWAYS_HIGH = 2               'ʼ��Ϊ�ߣ�����STROBE�źŵ���������
Public Const   STROBE_ALWAYS_LOW = 3                'ʼ��Ϊ�ͣ�����STROBE�źŵ���������


'Ӳ���ⴥ�����ź����� emExtTrigSignal
Public Const   EXT_TRIG_LEADING_EDGE = 0        '�����ش�����Ĭ��Ϊ�÷�ʽ
Public Const   EXT_TRIG_TRAILING_EDGE = 1       '�½��ش���
Public Const   EXT_TRIG_HIGH_LEVEL = 2          '�ߵ�ƽ����,��ƽ��Ⱦ����ع�ʱ�䣬�������ͺŵ����֧�ֵ�ƽ������ʽ��
Public Const   EXT_TRIG_LOW_LEVEL = 3           '�͵�ƽ����,


'Ӳ���ⴥ��ʱ�Ŀ��ŷ�ʽ emExtTrigShutterMode
Public Const   EXT_TRIG_EXP_STANDARD = 0        '��׼��ʽ��Ĭ��Ϊ�÷�ʽ��
Public Const   EXT_TRIG_EXP_GRR = 1             'ȫ�ָ�λ��ʽ�����ֹ������ŵ�CMOS�ͺŵ����֧�ָ÷�ʽ������ⲿ��е���ţ����Դﵽȫ�ֿ��ŵ�Ч�����ʺ��ĸ����˶�������



'������豸��Ϣ��ֻ����Ϣ�������޸�
Public Type tSdkCameraDevInfo
    acProductSeries(31)     As Byte   '��Ʒϵ��
    acProductName(31)       As Byte   '��Ʒ����
    acFriendlyName(31)      As Byte   '�ǳƣ����#��������������
    acLinkName(31)          As Byte   '��������,�ڲ�ʹ��
    acDriverVersion(31)     As Byte   '�����汾
    acSensorType(31)        As Byte   'sensor����
    acPortType(31)          As Byte   '�ӿ�����
    acSn(31)                As Byte   '��ƷΨһ���к�
    uInstance               As Long   '���ͺ�����ڸõ����ϵ�ʵ�������ţ���������ͬ�ͺŶ����
End Type

'����ķֱ����趨��Χ
Public Type tSdkResolutionRange
    iHeightMax              As Long  'ͼ�����߶�
    iHeightMin              As Long  'ͼ����С�߶�
    iWidthMax               As Long  'ͼ�������
    iWidthMin               As Long  'ͼ����С���
    uSkipModeMask           As Long  'SKIPģʽ���룬Ϊ0����ʾ��֧��SKIP ��bit0Ϊ1,��ʾ֧��SKIP 2x2 ;bit1Ϊ1����ʾ֧��SKIP 3x3....
    uBinSumModeMask         As Long  'BIN(���)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
    uBinAverageModeMask     As Long  'BIN(���ֵ)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
    uResampleMask           As Long  'Ӳ���ز���������
End Type


'����ķֱ�������
Public Type tSdkImageResolution
    iIndex              As Long  ' �����ţ�[0,N]��ʾԤ��ķֱ���(N ΪԤ��ֱ��ʵ���������һ�㲻����20),OXFF ��ʾ�Զ���ֱ���(ROI)
    acDescription(31)   As Byte  ' �÷ֱ��ʵ�������Ϣ����Ԥ��ֱ���ʱ����Ϣ��Ч���Զ���ֱ��ʿɺ��Ը���Ϣ
    uBinSumMode         As Long  ' �Ƿ�BIN�ĳߴ�,16bitΪ1��ʾsum��Ϊ0��ʾaverage����16λ��ʾBIN�����С��Ϊ0��ʾ��ֹBINģʽ,��Χ���ܳ���tSdkResolutionRange��uBinModeMask
    uBinAverageMode     As Long  ' BIN(���ֵ)��ģʽ,��Χ���ܳ���tSdkResolutionRange��uBinAverageModeMask
    uSkipMode           As Long  ' �Ƿ�SKIP�ĳߴ磬Ϊ0��ʾ��ֹSKIPģʽ��1��ʾSKIP2X2��2��ʾSKIP3X3���Դ�����,��Χ���ܳ���tSdkResolutionRange��uSkipModeMask
    uResampleMask       As Long  ' Ӳ���ز���������
    iHOffsetFOV         As Long  ' �ɼ��ӳ������Sensor����ӳ����ϽǵĴ�ֱƫ��
    iVOffsetFOV         As Long  ' �ɼ��ӳ������Sensor����ӳ����Ͻǵ�ˮƽƫ��
    iWidthFOV           As Long  ' �ɼ��ӳ��Ŀ��
    iHeightFOV          As Long  ' �ɼ��ӳ��ĸ߶�
    iWidth              As Long  ' ������������ͼ��Ŀ��
    iHeight             As Long  ' ������������ͼ��ĸ߶�
    iWidthZoomHd        As Long  ' Ӳ�����ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
    iHeightZoomHd       As Long  ' Ӳ�����ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
    iWidthZoomSw        As Long  ' ������ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
    iHeightZoomSw       As Long  ' ������ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
End Type

'�����ƽ��ģʽ������Ϣ
Public Type tSdkColorTemperatureDes
    iIndex              As Long    'ģʽ������
    acDescription(31)   As Byte    '������Ϣ
End Type

'���֡��������Ϣ
Public Type tSdkFrameSpeed
    iIndex              As Long    ' ֡�������ţ�һ��0��Ӧ�ڵ���ģʽ��1��Ӧ����ͨģʽ��2��Ӧ�ڸ���ģʽ
    acDescription(31)   As Byte    ' ������Ϣ
End Type

'����ع⹦�ܷ�Χ����
Public Type tSdkExpose
    uiTargetMin         As Long     '�Զ��ع�����Ŀ����Сֵ
    uiTargetMax         As Long     '�Զ��ع�����Ŀ�����ֵ
    uiAnalogGainMin     As Long     'ģ���������Сֵ����λΪfAnalogGainStep�ж���
    uiAnalogGainMax     As Long     'ģ����������ֵ����λΪfAnalogGainStep�ж���
    fAnalogGainStep     As Single   'ģ������ÿ����1����Ӧ�����ӵķŴ��������磬uiAnalogGainMinһ��Ϊ16��fAnalogGainStepһ��Ϊ0.125����ô��С�Ŵ�������16*0.125 = 2��
    uiExposeTimeMin     As Long     '�ֶ�ģʽ�£��ع�ʱ�����Сֵ����λ:�С�����CameraGetExposureLineTime���Ի��һ�ж�Ӧ��ʱ��(΢��),�Ӷ��õ���֡���ع�ʱ��
    uiExposeTimeMax     As Long     '�ֶ�ģʽ�£��ع�ʱ������ֵ����λ:��
End Type

'����ģʽ����
Public Type tSdkTrigger
    iIndex              As Long      'ģʽ������
    acDescription(31)   As Byte      '��ģʽ��������Ϣ
End Type


'����ְ���С����(��Ҫ��������������Ч)
Public Type tSdkPackLength
    iIndex              As Long       '�ְ���С������
    acDescription(31)   As Byte       '��Ӧ��������Ϣ
End Type

'Ԥ���LUT������
Public Type tSdkPresetLut
    iIndex              As Long     '���
    acDescription(31)   As Byte     '������Ϣ
End Type

'AE�㷨����
Public Type tSdkAeAlgorithm
    iIndex              As Long     '���
    acDescription(31)   As Byte     '������Ϣ
End Type

'RAWתRGB�㷨����
Public Type tSdkBayerDecodeAlgorithm
    iIndex              As Long      '���
    acDescription(31)   As Byte      '������Ϣ
End Type

'֡��ͳ����Ϣ
Public Type tSdkFrameStatistic
     iTotal     As Long             '��ǰ�ɼ�����֡������������֡��
     iCapture   As Long             '��ǰ�ɼ�����Ч֡������
     iLost      As Long             '��ǰ��֡������
End Type

'��������ͼ�����ݸ�ʽ
Public Type tSdkMediaType
    iIndex              As Long     '��ʽ������
    acDescription(31)   As Byte     '������Ϣ
    iMediaType          As Long     '��Ӧ��ͼ���ʽ���룬��CAMERA_MEDIA_TYPE_BAYGR8���ڱ��ļ����ж��塣
End Type

'٤����趨��Χ
Public Type tGammaRange
    iMin As Long       '��Сֵ
    iMax As Long       '���ֵ
End Type

'�Աȶȵ��趨��Χ
Public Type tContrastRange
     iMin As Long       '��Сֵ
     iMax As Long       '���ֵ
End Type

'RGB��ͨ������������趨��Χ
Public Type tRgbGainRange
     iRGainMin As Long               '��ɫ�������Сֵ
     iRGainMax As Long               '��ɫ��������ֵ
     iGGainMin As Long               '��ɫ�������Сֵ
     iGGainMax As Long               '��ɫ��������ֵ
     iBGainMin As Long               '��ɫ�������Сֵ
     iBGainMax As Long               '��ɫ��������ֵ
End Type

'���Ͷ��趨�ķ�Χ
Public Type tSaturationRange
     iMin As Long            '��Сֵ
     iMax As Long            '���ֵ
End Type

'�񻯵��趨��Χ
Public Type tSharpnessRange
     iMin As Long                '��Сֵ
     iMax As Long                '���ֵ
End Type

'ISPģ���ʹ����Ϣ
Public Type tSdkIspCapacity
     bMonoSensor As Long             '��ʾ���ͺ�����Ƿ�Ϊ�ڰ����,����Ǻڰ����������ɫ��صĹ��ܶ��޷�����
     bWbOnce As Long                 '��ʾ���ͺ�����Ƿ�֧���ֶ���ƽ�⹦��
     bAutoWb As Long                 '��ʾ���ͺ�����Ƿ�֧���Զ���ƽ�⹦��
     bAutoExposure As Long           '��ʾ���ͺ�����Ƿ�֧���Զ��ع⹦��
     bManualExposure As Long         '��ʾ���ͺ�����Ƿ�֧���ֶ��ع⹦��
     bAntiFlick As Long              '��ʾ���ͺ�����Ƿ�֧�ֿ�Ƶ������
     bDeviceIsp As Long              '��ʾ���ͺ�����Ƿ�֧��Ӳ��ISP����
     bForceUseDeviceIsp As Long      'bDeviceIsp��bForceUseDeviceIspͬʱΪTRUEʱ����ʾǿ��ֻ��Ӳ��ISP������ȡ����
     bZoomHD As Long                 '���Ӳ���Ƿ�֧��ͼ���������(ֻ������С)��
End Type

'/* �������ϵ��豸������Ϣ����Щ��Ϣ�������ڶ�̬����UI */
Public Type tSdkCameraCapbility
        pTriggerDesc As Long           ' ����ģʽ ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        iTriggerDesc As Long           ' ����ģʽ�ĸ�������pTriggerDesc����Ĵ�С
        pImageSizeDesc As Long         ' Ԥ��ֱ���ѡ�� tSdkImageResolution
        iImageSizeDesc As Long         ' Ԥ��ֱ��ʵĸ�������pImageSizeDesc����Ĵ�С
        pClrTempDesc As Long           ' Ԥ��ɫ��ģʽ�����ڰ�ƽ�� tSdkColorTemperatureDes
        iClrTempDesc As Long
        pMediaTypeDesc As Long         ' ������ͼ���ʽ tSdkMediaType
        iMediaTypdeDesc As Long        ' ������ͼ���ʽ�������������pMediaTypeDesc����Ĵ�С��
        pFrameSpeedDesc As Long        ' �ɵ���֡�����ͣ���Ӧ��������ͨ ���� �ͳ��������ٶ����� tSdkFrameSpeed
        iFrameSpeedDesc As Long        ' �ɵ���֡�����͵ĸ�������pFrameSpeedDesc����Ĵ�С��
        pPackLenDesc As Long           ' ��������ȣ�һ�����������豸 tSdkPackLength
        iPackLenDesc As Long           ' �ɹ�ѡ��Ĵ���ְ����ȵĸ�������pPackLenDesc����Ĵ�С��
        iOutputIoCounts As Long        ' �ɱ�����IO�ĸ���
        iInputIoCounts As Long         ' �ɱ������IO�ĸ���
        pPresetLutDesc As Long         ' ���Ԥ���LUT�� tSdkPresetLut
        iPresetLut As Long             ' ���Ԥ���LUT��ĸ�������pPresetLutDesc����Ĵ�С
        iUserDataMaxLen As Long        ' ָʾ����������ڱ����û�����������󳤶ȡ�Ϊ0��ʾ�ޡ�
        bParamInDevice As Long         ' ָʾ���豸�Ƿ�֧�ִ��豸�ж�д�����顣1Ϊ֧�֣�0��֧�֡�
        pAeAlmSwDesc As Long           ' ����Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        iAeAlmSwDesc As Long           ' ����Զ��ع��㷨����
        pAeAlmHdDesc As Long           ' Ӳ���Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        iAeAlmHdDesc As Long           ' Ӳ���Զ��ع��㷨����
        pBayerDecAlmSwDesc As Long     ' ���Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
        iBayerDecAlmSwDesc As Long     ' ���Bayerת��ΪRGB���ݵ��㷨����
        pBayerDecAlmHdDesc As Long     ' Ӳ��Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
        iBayerDecAlmHdDesc As Long     ' Ӳ��Bayerת��ΪRGB���ݵ��㷨����
        '/* ͼ������ĵ��ڷ�Χ����,���ڶ�̬����UI*/
        sExposeDesc As tSdkExpose               ' �ع�ķ�Χֵ
        sResolutionRange As tSdkResolutionRange ' �ֱ��ʷ�Χ����
        sRgbGainRange As tRgbGainRange          ' ͼ���������淶Χ����
        sSaturationRange As tSaturationRange    ' ���Ͷȷ�Χ����
        sGammaRange As tGammaRange              ' ٤��Χ����
        sContrastRange As tContrastRange        ' �Աȶȷ�Χ����
        sSharpnessRange As tSharpnessRange      ' �񻯷�Χ����
        sIspCapacity As tSdkIspCapacity         ' ISP��������
End Type


'ͼ��֡ͷ��Ϣ
Public Type tSdkFrameHead
            uiMediaType     As Long      ' ͼ���ʽ,Image Format
            uBytes          As Long      ' ͼ�������ֽ���,Total bytes
            iWidth          As Long      ' �߶� Image width
            iHeight         As Long      ' ��� Image height
            iWidthZoomSw    As Long      ' ������ŵĿ��,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
            iHeightZoomSw   As Long      ' ������ŵĸ߶�,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
            bIsTrigger      As Long      ' ָʾ�Ƿ�Ϊ����֡ is trigger
            uiTimeStamp     As Long      ' ��֡�Ĳɼ�ʱ�䣬��λ0.1����
            uiExpTime       As Long      ' ��ǰͼ����ع�ֵ����λΪ΢��us
            fAnalogGain     As Single    ' ��ǰͼ���ģ�����汶��
            iGamma          As Long      ' ��֡ͼ���٤���趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
            iContrast       As Long      ' ��֡ͼ��ĶԱȶ��趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
            iSaturation     As Long      ' ��֡ͼ��ı��Ͷ��趨ֵ�����ںڰ���������壬Ϊ0
            fRgain          As Single    ' ��֡ͼ����ĺ�ɫ�������汶�������ںڰ���������壬Ϊ1
            fGgain          As Single    ' ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
            fBgain          As Single    ' ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
End Type


'ͼ��֡����
Public Type tSdkFrame
             head    As tSdkFrameHead       '֡ͷ
             pBuffer As Long                '������
End Type

'Grabberͳ����Ϣ
Public Type tSdkGrabberStat
	Width As Long  			' ֡ͼ����
	Height As Long			' ֡ͼ��߶�
	Disp As Long			' ��ʾ֡����
	Capture As Long			' �ɼ�����Ч֡������
	Lost As Long			' ��֡������
	Error As Long			' ��֡������
	DispFps As Single		' ��ʾ֡��
	CapFps As Single		' ����֡��
End Type


'----------------------------IMAGE FORMAT DEFINE------------------------------------
'----------------------------ͼ���ʽ����-------------------------------------------
Public Const CAMERA_MEDIA_TYPE_MONO = &H1000000
Public Const CAMERA_MEDIA_TYPE_RGB = &H2000000
Public Const CAMERA_MEDIA_TYPE_COLOR = &H2000000
Public Const CAMERA_MEDIA_TYPE_CUSTOM = &H80000000
Public Const CAMERA_MEDIA_TYPE_COLOR_MASK = &HFF000000
Public Const CAMERA_MEDIA_TYPE_OCCUPY1BIT = &H10000
Public Const CAMERA_MEDIA_TYPE_OCCUPY2BIT = &H20000
Public Const CAMERA_MEDIA_TYPE_OCCUPY4BIT = &H40000
Public Const CAMERA_MEDIA_TYPE_OCCUPY8BIT = &H80000
Public Const CAMERA_MEDIA_TYPE_OCCUPY10BIT = &HA0000
Public Const CAMERA_MEDIA_TYPE_OCCUPY12BIT = &HC0000
Public Const CAMERA_MEDIA_TYPE_OCCUPY16BIT = &H100000
Public Const CAMERA_MEDIA_TYPE_OCCUPY24BIT = &H180000
Public Const CAMERA_MEDIA_TYPE_OCCUPY32BIT = &H200000
Public Const CAMERA_MEDIA_TYPE_OCCUPY36BIT = &H240000
Public Const CAMERA_MEDIA_TYPE_OCCUPY48BIT = &H300000
Public Const CAMERA_MEDIA_TYPE_EFFECTIVE_PIXEL_SIZE_MASK = &HFF0000
Public Const CAMERA_MEDIA_TYPE_EFFECTIVE_PIXEL_SIZE_SHIFT = 16


Public Const CAMERA_MEDIA_TYPE_ID_MASK = &HFFFF
Public Const CAMERA_MEDIA_TYPE_COUNT = &H46

'mono*/
Public Const CAMERA_MEDIA_TYPE_MONO1P = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY1BIT Or &H37)
Public Const CAMERA_MEDIA_TYPE_MONO2P = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY2BIT Or &H38)
Public Const CAMERA_MEDIA_TYPE_MONO4P = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY4BIT Or &H39)
Public Const CAMERA_MEDIA_TYPE_MONO8 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &H1)
Public Const CAMERA_MEDIA_TYPE_MONO8S = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &H2)
Public Const CAMERA_MEDIA_TYPE_MONO10 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H3)
Public Const CAMERA_MEDIA_TYPE_MONO10_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H4)
Public Const CAMERA_MEDIA_TYPE_MONO12 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H5)
Public Const CAMERA_MEDIA_TYPE_MONO12_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H6)
Public Const CAMERA_MEDIA_TYPE_MONO14 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H25)
Public Const CAMERA_MEDIA_TYPE_MONO16 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H7)

'Bayer */
Public Const CAMERA_MEDIA_TYPE_BAYGR8 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &H8)
Public Const CAMERA_MEDIA_TYPE_BAYRG8 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &H9)
Public Const CAMERA_MEDIA_TYPE_BAYGB8 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &HA)
Public Const CAMERA_MEDIA_TYPE_BAYBG8 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY8BIT Or &HB)

Public Const CAMERA_MEDIA_TYPE_BAYGR10_MIPI = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY10BIT Or &H26)
Public Const CAMERA_MEDIA_TYPE_BAYRG10_MIPI = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY10BIT Or &H27)
Public Const CAMERA_MEDIA_TYPE_BAYGB10_MIPI = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY10BIT Or &H28)
Public Const CAMERA_MEDIA_TYPE_BAYBG10_MIPI = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY10BIT Or &H29)


Public Const CAMERA_MEDIA_TYPE_BAYGR10 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &HC)
Public Const CAMERA_MEDIA_TYPE_BAYRG10 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &HD)
Public Const CAMERA_MEDIA_TYPE_BAYGB10 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &HE)
Public Const CAMERA_MEDIA_TYPE_BAYBG10 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &HF)

Public Const CAMERA_MEDIA_TYPE_BAYGR12 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H10)
Public Const CAMERA_MEDIA_TYPE_BAYRG12 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H11)
Public Const CAMERA_MEDIA_TYPE_BAYGB12 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H12)
Public Const CAMERA_MEDIA_TYPE_BAYBG12 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H13)


Public Const CAMERA_MEDIA_TYPE_BAYGR10_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H26)
Public Const CAMERA_MEDIA_TYPE_BAYRG10_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H27)
Public Const CAMERA_MEDIA_TYPE_BAYGB10_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H28)
Public Const CAMERA_MEDIA_TYPE_BAYBG10_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H29)

Public Const CAMERA_MEDIA_TYPE_BAYGR12_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H2A)
Public Const CAMERA_MEDIA_TYPE_BAYRG12_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H2B)
Public Const CAMERA_MEDIA_TYPE_BAYGB12_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H2C)
Public Const CAMERA_MEDIA_TYPE_BAYBG12_PACKED = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H2D)

Public Const CAMERA_MEDIA_TYPE_BAYGR16 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H2E)
Public Const CAMERA_MEDIA_TYPE_BAYRG16 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H2F)
Public Const CAMERA_MEDIA_TYPE_BAYGB16 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H30)
Public Const CAMERA_MEDIA_TYPE_BAYBG16 = (CAMERA_MEDIA_TYPE_MONO Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H31)

'RGB */
Public Const CAMERA_MEDIA_TYPE_RGB8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H14)
Public Const CAMERA_MEDIA_TYPE_BGR8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H15)
Public Const CAMERA_MEDIA_TYPE_RGBA8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY32BIT Or &H16)
Public Const CAMERA_MEDIA_TYPE_BGRA8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY32BIT Or &H17)
Public Const CAMERA_MEDIA_TYPE_RGB10 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H18)
Public Const CAMERA_MEDIA_TYPE_BGR10 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H19)
Public Const CAMERA_MEDIA_TYPE_RGB12 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H1A)
Public Const CAMERA_MEDIA_TYPE_BGR12 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H1B)
Public Const CAMERA_MEDIA_TYPE_RGB16 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H33)
Public Const CAMERA_MEDIA_TYPE_RGB10V1_PACKED = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY32BIT Or &H1C)
Public Const CAMERA_MEDIA_TYPE_RGB10P32 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY32BIT Or &H1D)
Public Const CAMERA_MEDIA_TYPE_RGB12V1_PACKED = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY36BIT Or &H34)
Public Const CAMERA_MEDIA_TYPE_RGB565P = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H35)
Public Const CAMERA_MEDIA_TYPE_BGR565P = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H36)

'YUV and YCbCr*/
Public Const CAMERA_MEDIA_TYPE_YUV411_8_UYYVYY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H1E)
Public Const CAMERA_MEDIA_TYPE_YUV422_8_UYVY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H1F)
Public Const CAMERA_MEDIA_TYPE_YUV422_8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H32)
Public Const CAMERA_MEDIA_TYPE_YUV8_UYV = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H20)
Public Const CAMERA_MEDIA_TYPE_YCBCR8_CBYCR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H3A)
'CAMERA_MEDIA_TYPE_YCBCR422_8 : YYYYCbCrCbCr
Public Const CAMERA_MEDIA_TYPE_YCBCR422_8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H3B)
Public Const CAMERA_MEDIA_TYPE_YCBCR422_8_CBYCRY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H43)
Public Const CAMERA_MEDIA_TYPE_YCBCR411_8_CBYYCRYY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H3C)
Public Const CAMERA_MEDIA_TYPE_YCBCR601_8_CBYCR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H3D)
Public Const CAMERA_MEDIA_TYPE_YCBCR601_422_8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H3E)
Public Const CAMERA_MEDIA_TYPE_YCBCR601_422_8_CBYCRY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H44)
Public Const CAMERA_MEDIA_TYPE_YCBCR601_411_8_CBYYCRYY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H3F)
Public Const CAMERA_MEDIA_TYPE_YCBCR709_8_CBYCR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H40)
Public Const CAMERA_MEDIA_TYPE_YCBCR709_422_8 = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H41)
Public Const CAMERA_MEDIA_TYPE_YCBCR709_422_8_CBYCRY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY16BIT Or &H45)
Public Const CAMERA_MEDIA_TYPE_YCBCR709_411_8_CBYYCRYY = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY12BIT Or &H42)

'RGB Planar */
Public Const CAMERA_MEDIA_TYPE_RGB8_PLANAR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY24BIT Or &H21)
Public Const CAMERA_MEDIA_TYPE_RGB10_PLANAR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H22)
Public Const CAMERA_MEDIA_TYPE_RGB12_PLANAR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H23)
Public Const CAMERA_MEDIA_TYPE_RGB16_PLANAR = (CAMERA_MEDIA_TYPE_COLOR Or CAMERA_MEDIA_TYPE_OCCUPY48BIT Or &H24)

