Imports System.Runtime.InteropServices

Public Module CameraDefine
    'BIG5 TRANS ALLOWED
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
    Public Const STROBE_SYNC_WITH_TRIG_AUTO = 0       '�ʹ����ź�ͬ������������������ع�ʱ���Զ�����STROBE�źš���ʱ����Ч���Կ�����(CameraSetStrobePolarity)��
    Public Const STROBE_SYNC_WITH_TRIG_MANUAL = 1     '�ʹ����ź�ͬ����������STROBE��ʱָ����ʱ���(CameraSetStrobeDelayTime)���ٳ���ָ��ʱ�������(CameraSetStrobePulseWidth)����Ч���Կ�����(CameraSetStrobePolarity)��
    Public Const STROBE_ALWAYS_HIGH = 2               'ʼ��Ϊ�ߣ�����STROBE�źŵ���������
    Public Const STROBE_ALWAYS_LOW = 3                'ʼ��Ϊ�ͣ�����STROBE�źŵ���������


    'Ӳ���ⴥ�����ź����� emExtTrigSignal
    Public Const EXT_TRIG_LEADING_EDGE = 0        '�����ش�����Ĭ��Ϊ�÷�ʽ
    Public Const EXT_TRIG_TRAILING_EDGE = 1       '�½��ش���
    Public Const EXT_TRIG_HIGH_LEVEL = 2          '�ߵ�ƽ����,��ƽ��Ⱦ����ع�ʱ�䣬�������ͺŵ����֧�ֵ�ƽ������ʽ��
    Public Const EXT_TRIG_LOW_LEVEL = 3           '�͵�ƽ����,


    'Ӳ���ⴥ��ʱ�Ŀ��ŷ�ʽ emExtTrigShutterMode
    Public Const EXT_TRIG_EXP_STANDARD = 0        '��׼��ʽ��Ĭ��Ϊ�÷�ʽ��
    Public Const EXT_TRIG_EXP_GRR = 1             'ȫ�ָ�λ��ʽ�����ֹ������ŵ�CMOS�ͺŵ����֧�ָ÷�ʽ������ⲿ��е���ţ����Դﵽȫ�ֿ��ŵ�Ч�����ʺ��ĸ����˶�������

    '������豸��Ϣ��ֻ����Ϣ�������޸�
    Public Structure tSdkCameraDevInfo
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acProductSeries() As Byte '��Ʒϵ��
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acProductName() As Byte   '��Ʒ����
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acFriendlyName() As Byte  '�ǳƣ����#��������������
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acLinkName() As Byte      '��������,�ڲ�ʹ��
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDriverVersion() As Byte '�����汾
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acSensorType() As Byte    'sensor����
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acPortType() As Byte      '�ӿ�����
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acSn() As Byte            '��ƷΨһ���к�
        Dim uInstance As Integer         '���ͺ�����ڸõ����ϵ�ʵ�������ţ���������ͬ�ͺŶ����
    End Structure

    '����ķֱ����趨��Χ
    Public Structure tSdkResolutionRange
        Dim iHeightMax As Integer      'ͼ�����߶�
        Dim iHeightMin As Integer      'ͼ����С�߶�
        Dim iWidthMax As Integer       'ͼ�������
        Dim iWidthMin As Integer       'ͼ����С���
        Dim uSkipModeMask As Integer   'SKIPģʽ���룬Ϊ0����ʾ��֧��SKIP ��bit0Ϊ1,��ʾ֧��SKIP 2x2 ;bit1Ϊ1����ʾ֧��SKIP 3x3....
        Dim uBinSumModeMask As Integer 'BIN(���)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
        Dim uBinAverageModeMask As Integer  'BIN(���ֵ)ģʽ���룬Ϊ0����ʾ��֧��BIN ��bit0Ϊ1,��ʾ֧��BIN 2x2 ;bit1Ϊ1����ʾ֧��BIN 3x3....
        Dim uResampleMask As Integer   'Ӳ���ز���������
    End Structure


    '����ķֱ�������
    Public Structure tSdkImageResolution
        Dim iIndex As Integer              ' �����ţ�[0,N]��ʾԤ��ķֱ���(N ΪԤ��ֱ��ʵ���������һ�㲻����20),OXFF ��ʾ�Զ���ֱ���(ROI)
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte     ' �÷ֱ��ʵ�������Ϣ����Ԥ��ֱ���ʱ����Ϣ��Ч���Զ���ֱ��ʿɺ��Ը���Ϣ
        Dim uBinSumMode As Integer         ' �Ƿ�BIN�ĳߴ�,16bitΪ1��ʾsum��Ϊ0��ʾaverage����16λ��ʾBIN�����С��Ϊ0��ʾ��ֹBINģʽ,��Χ���ܳ���tSdkResolutionRange��uBinModeMask
        Dim uBinAverageMode As Integer     ' BIN(���ֵ)��ģʽ,��Χ���ܳ���tSdkResolutionRange��uBinAverageModeMask
        Dim uSkipMode As Integer           ' �Ƿ�SKIP�ĳߴ磬Ϊ0��ʾ��ֹSKIPģʽ��1��ʾSKIP2X2��2��ʾSKIP3X3���Դ�����,��Χ���ܳ���tSdkResolutionRange��uSkipModeMask
        Dim uResampleMask As Integer       ' Ӳ���ز���������
        Dim iHOffsetFOV As Integer         ' �ɼ��ӳ������Sensor����ӳ����ϽǵĴ�ֱƫ��
        Dim iVOffsetFOV As Integer         ' �ɼ��ӳ������Sensor����ӳ����Ͻǵ�ˮƽƫ��
        Dim iWidthFOV As Integer           ' �ɼ��ӳ��Ŀ��
        Dim iHeightFOV As Integer          ' �ɼ��ӳ��ĸ߶�
        Dim iWidth As Integer              ' ������������ͼ��Ŀ��
        Dim iHeight As Integer             ' ������������ͼ��ĸ߶�
        Dim iWidthZoomHd As Integer        ' Ӳ�����ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
        Dim iHeightZoomHd As Integer       ' Ӳ�����ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
        Dim iWidthZoomSw As Integer        ' ������ŵĿ��,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
        Dim iHeightZoomSw As Integer       ' ������ŵĸ߶�,����Ҫ���д˲����ķֱ��ʣ��˱�������Ϊ0.
    End Structure

    '�����ƽ��ģʽ������Ϣ
    Public Structure tSdkColorTemperatureDes
        Dim iIndex As Integer              'ģʽ������
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte     '������Ϣ
    End Structure

    '���֡��������Ϣ
    Public Structure tSdkFrameSpeed
        Dim iIndex As Integer    ' ֡�������ţ�һ��0��Ӧ�ڵ���ģʽ��1��Ӧ����ͨģʽ��2��Ӧ�ڸ���ģʽ
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte    ' ������Ϣ
    End Structure

    '����ع⹦�ܷ�Χ����
    Public Structure tSdkExpose
        Dim uiTargetMin As Integer         '�Զ��ع�����Ŀ����Сֵ
        Dim uiTargetMax As Integer         '�Զ��ع�����Ŀ�����ֵ
        Dim uiAnalogGainMin As Integer     'ģ���������Сֵ����λΪfAnalogGainStep�ж���
        Dim uiAnalogGainMax As Integer     'ģ����������ֵ����λΪfAnalogGainStep�ж���
        Dim fAnalogGainStep As Single   'ģ������ÿ����1����Ӧ�����ӵķŴ��������磬uiAnalogGainMinһ��Ϊ16��fAnalogGainStepһ��Ϊ0.125����ô��С�Ŵ�������16*0.125 = 2��
        Dim uiExposeTimeMin As Integer     '�ֶ�ģʽ�£��ع�ʱ�����Сֵ����λ:�С�����CameraGetExposureLineTime���Ի��һ�ж�Ӧ��ʱ��(΢��),�Ӷ��õ���֡���ع�ʱ��
        Dim uiExposeTimeMax As Integer     '�ֶ�ģʽ�£��ع�ʱ������ֵ����λ:��
    End Structure

    '����ģʽ����
    Public Structure tSdkTrigger
        Dim iIndex As Integer                  'ģʽ������
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte         '��ģʽ��������Ϣ
    End Structure

    '����ְ���С����(��Ҫ��������������Ч)
    Public Structure tSdkPackLength
        Dim iIndex As Integer                  '�ְ���С������
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte       '��Ӧ��������Ϣ
    End Structure

    'Ԥ���LUT������
    Public Structure tSdkPresetLut
        Dim iIndex As Integer                  '���
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte         '������Ϣ
    End Structure

    'AE�㷨����
    Public Structure tSdkAeAlgorithm
        Dim iIndex As Integer              '���
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte     '������Ϣ
    End Structure

    'RAWתRGB�㷨����
    Public Structure tSdkBayerDecodeAlgorithm
        Dim iIndex As Integer               '���
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte      '������Ϣ
    End Structure

    '֡��ͳ����Ϣ
    Public Structure tSdkFrameStatistic
        Dim iTotal As Integer                '��ǰ�ɼ�����֡������������֡��
        Dim iCapture As Integer              '��ǰ�ɼ�����Ч֡������
        Dim iLost As Integer                 '��ǰ��֡������
    End Structure

    '��������ͼ�����ݸ�ʽ
    Public Structure tSdkMediaType
        Dim iIndex As Integer              '��ʽ������
        <MarshalAs(UnmanagedType.ByValArray, SizeConst:=32)> _
        Dim acDescription() As Byte     '������Ϣ
        Dim iMediaType As Integer          '��Ӧ��ͼ���ʽ���룬��CAMERA_MEDIA_TYPE_BAYGR8���ڱ��ļ����ж��塣
    End Structure

    '٤����趨��Χ
    Public Structure tGammaRange
        Dim iMin As Integer       '��Сֵ
        Dim iMax As Integer       '���ֵ
    End Structure

    '�Աȶȵ��趨��Χ
    Public Structure tContrastRange
        Dim iMin As Integer       '��Сֵ
        Dim iMax As Integer       '���ֵ
    End Structure

    'RGB��ͨ������������趨��Χ
    Public Structure tRgbGainRange
        Dim iRGainMin As Integer               '��ɫ�������Сֵ
        Dim iRGainMax As Integer               '��ɫ��������ֵ
        Dim iGGainMin As Integer               '��ɫ�������Сֵ
        Dim iGGainMax As Integer               '��ɫ��������ֵ
        Dim iBGainMin As Integer               '��ɫ�������Сֵ
        Dim iBGainMax As Integer               '��ɫ��������ֵ
    End Structure

    '���Ͷ��趨�ķ�Χ
    Public Structure tSaturationRange
        Dim iMin As Integer            '��Сֵ
        Dim iMax As Integer            '���ֵ
    End Structure

    '�񻯵��趨��Χ
    Public Structure tSharpnessRange
        Dim iMin As Integer                '��Сֵ
        Dim iMax As Integer                '���ֵ
    End Structure

    'ISPģ���ʹ����Ϣ
    Public Structure tSdkIspCapacity
        Dim bMonoSensor As Integer             '��ʾ���ͺ�����Ƿ�Ϊ�ڰ����,����Ǻڰ����������ɫ��صĹ��ܶ��޷�����
        Dim bWbOnce As Integer                 '��ʾ���ͺ�����Ƿ�֧���ֶ���ƽ�⹦��
        Dim bAutoWb As Integer                 '��ʾ���ͺ�����Ƿ�֧���Զ���ƽ�⹦��
        Dim bAutoExposure As Integer           '��ʾ���ͺ�����Ƿ�֧���Զ��ع⹦��
        Dim bManualExposure As Integer         '��ʾ���ͺ�����Ƿ�֧���ֶ��ع⹦��
        Dim bAntiFlick As Integer              '��ʾ���ͺ�����Ƿ�֧�ֿ�Ƶ������
        Dim bDeviceIsp As Integer              '��ʾ���ͺ�����Ƿ�֧��Ӳ��ISP����
        Dim bForceUseDeviceIsp As Integer      'bDeviceIsp��bForceUseDeviceIspͬʱΪTRUEʱ����ʾǿ��ֻ��Ӳ��ISP������ȡ����
        Dim bZoomHD As Integer                 '���Ӳ���Ƿ�֧��ͼ���������(ֻ������С)��
    End Structure

    '/* �������ϵ��豸������Ϣ����Щ��Ϣ�������ڶ�̬����UI */
    Public Structure tSdkCameraCapbility
        Dim pTriggerDesc As IntPtr            ' ����ģʽ ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        Dim iTriggerDesc As Integer           ' ����ģʽ�ĸ�������pTriggerDesc����Ĵ�С
        Dim pImageSizeDesc As IntPtr          ' Ԥ��ֱ���ѡ�� tSdkImageResolution
        Dim iImageSizeDesc As Integer         ' Ԥ��ֱ��ʵĸ�������pImageSizeDesc����Ĵ�С
        Dim pClrTempDesc As IntPtr            ' Ԥ��ɫ��ģʽ�����ڰ�ƽ�� tSdkColorTemperatureDes
        Dim iClrTempDesc As Integer
        Dim pMediaTypeDesc As IntPtr          ' ������ͼ���ʽ tSdkMediaType
        Dim iMediaTypdeDesc As Integer        ' ������ͼ���ʽ�������������pMediaTypeDesc����Ĵ�С��
        Dim pFrameSpeedDesc As IntPtr         ' �ɵ���֡�����ͣ���Ӧ��������ͨ ���� �ͳ��������ٶ����� tSdkFrameSpeed
        Dim iFrameSpeedDesc As Integer        ' �ɵ���֡�����͵ĸ�������pFrameSpeedDesc����Ĵ�С��
        Dim pPackLenDesc As IntPtr            ' ��������ȣ�һ�����������豸 tSdkPackLength
        Dim iPackLenDesc As Integer           ' �ɹ�ѡ��Ĵ���ְ����ȵĸ�������pPackLenDesc����Ĵ�С��
        Dim iOutputIoCounts As Integer        ' �ɱ�����IO�ĸ���
        Dim iInputIoCounts As Integer         ' �ɱ������IO�ĸ���
        Dim pPresetLutDesc As IntPtr          ' ���Ԥ���LUT�� tSdkPresetLut
        Dim iPresetLut As Integer             ' ���Ԥ���LUT��ĸ�������pPresetLutDesc����Ĵ�С
        Dim iUserDataMaxLen As Integer        ' ָʾ����������ڱ����û�����������󳤶ȡ�Ϊ0��ʾ�ޡ�
        Dim bParamInDevice As Integer         ' ָʾ���豸�Ƿ�֧�ִ��豸�ж�д�����顣1Ϊ֧�֣�0��֧�֡�
        Dim pAeAlmSwDesc As IntPtr            ' ����Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        Dim iAeAlmSwDesc As Integer           ' ����Զ��ع��㷨����
        Dim pAeAlmHdDesc As IntPtr            ' Ӳ���Զ��ع��㷨����, ָ��tSdkTrigger����ĵ�ַ����Long���ͱ�ʾָ��
        Dim iAeAlmHdDesc As Integer           ' Ӳ���Զ��ع��㷨����
        Dim pBayerDecAlmSwDesc As IntPtr      ' ���Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
        Dim iBayerDecAlmSwDesc As Integer     ' ���Bayerת��ΪRGB���ݵ��㷨����
        Dim pBayerDecAlmHdDesc As IntPtr      ' Ӳ��Bayerת��ΪRGB���ݵ��㷨����, tSdkBayerDecodeAlgorithm����Long���ͱ�ʾָ��
        Dim iBayerDecAlmHdDesc As Integer     ' Ӳ��Bayerת��ΪRGB���ݵ��㷨����
        '/* ͼ������ĵ��ڷ�Χ����,���ڶ�̬����UI*/
        Dim sExposeDesc As tSdkExpose               ' �ع�ķ�Χֵ
        Dim sResolutionRange As tSdkResolutionRange ' �ֱ��ʷ�Χ����
        Dim sRgbGainRange As tRgbGainRange          ' ͼ���������淶Χ����
        Dim sSaturationRange As tSaturationRange    ' ���Ͷȷ�Χ����
        Dim sGammaRange As tGammaRange              ' ٤��Χ����
        Dim sContrastRange As tContrastRange        ' �Աȶȷ�Χ����
        Dim sSharpnessRange As tSharpnessRange      ' �񻯷�Χ����
        Dim sIspCapacity As tSdkIspCapacity         ' ISP��������
    End Structure


    'ͼ��֡ͷ��Ϣ
    Public Structure tSdkFrameHead
        Dim uiMediaType As Integer         ' ͼ���ʽ,Image Format
        Dim uBytes As Integer              ' ͼ�������ֽ���,Total bytes
        Dim iWidth As Integer              ' �߶� Image width
        Dim iHeight As Integer             ' ��� Image height
        Dim iWidthZoomSw As Integer        ' ������ŵĿ��,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
        Dim iHeightZoomSw As Integer       ' ������ŵĸ߶�,����Ҫ��������ü���ͼ�񣬴˱�������Ϊ0.
        Dim bIsTrigger As Integer          ' ָʾ�Ƿ�Ϊ����֡ is trigger
        Dim uiTimeStamp As Integer         ' ��֡�Ĳɼ�ʱ�䣬��λ0.1����
        Dim uiExpTime As Integer           ' ��ǰͼ����ع�ֵ����λΪ΢��us
        Dim fAnalogGain As Single       ' ��ǰͼ���ģ�����汶��
        Dim iGamma As Integer              ' ��֡ͼ���٤���趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
        Dim iContrast As Integer           ' ��֡ͼ��ĶԱȶ��趨ֵ������LUTģʽΪ��̬��������ʱ��Ч������ģʽ��Ϊ-1
        Dim iSaturation As Integer         ' ��֡ͼ��ı��Ͷ��趨ֵ�����ںڰ���������壬Ϊ0
        Dim fRgain As Single            ' ��֡ͼ����ĺ�ɫ�������汶�������ںڰ���������壬Ϊ1
        Dim fGgain As Single            ' ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
        Dim fBgain As Single            ' ��֡ͼ�������ɫ�������汶�������ںڰ���������壬Ϊ1
    End Structure


    'ͼ��֡����
    Public Structure tSdkFrame
        Dim head As tSdkFrameHead       '֡ͷ
        Dim pBuffer As IntPtr             '������
    End Structure

    'BMPͼ��ͷ����
    <StructLayout(LayoutKind.Sequential, CharSet:=CharSet.Unicode, Pack:=1)> _
    Public Structure BITMAPFILEHEADER
        Dim bfType As UInt16
        Dim bfSize As UInt32
        Dim bfReserved1 As UInt16
        Dim bfReserved2 As UInt16
        Dim bfOffBits As UInt32
    End Structure
    <StructLayout(LayoutKind.Sequential, CharSet:=CharSet.Unicode, Pack:=1)> _
    Public Structure BITMAPINFOHEADER
        Dim biSize As UInt32
        Dim biWidth As Integer
        Dim biHeight As Integer
        Dim biPlanes As UInt16
        Dim biBitCount As UInt16
        Dim biCompression As UInt32
        Dim biSizeImage As UInt32
        Dim biXPelsPerMeter As Integer
        Dim biYPelsPerMeter As Integer
        Dim biClrUsed As Integer
        Dim biClrImportant As Integer
        Dim BI_RGB As Integer
    End Structure


    'Grabberͳ����Ϣ
    Public Structure tSdkGrabberStat
        Dim Width As Integer            ' ֡ͼ����
        Dim Height As Integer           ' ֡ͼ��߶�
        Dim Disp As Integer             ' ��ʾ֡����
        Dim Capture As Integer          ' �ɼ�����Ч֡������
        Dim Lost As Integer             ' ��֡������
        Dim Err As Integer              ' ��֡������
        Dim DispFps As Single           ' ��ʾ֡��
        Dim CapFps As Single            ' ����֡��
    End Structure


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

End Module
