'BIG5 TRANS ALLOWED
Module CameraStatus
    Public Const CAMERA_STATUS_SUCCESS = 0                      ' �����ɹ�
    Public Const CAMERA_STATUS_FAILED = -1                      ' ����ʧ��
    Public Const CAMERA_STATUS_INTERNAL_ERROR = -2              ' �ڲ�����
    Public Const CAMERA_STATUS_UNKNOW = -3                      ' δ֪����
    Public Const CAMERA_STATUS_NOT_SUPPORTED = -4               ' ��֧�ָù���
    Public Const CAMERA_STATUS_NOT_INITIALIZED = -5             ' ��ʼ��δ���
    Public Const CAMERA_STATUS_PARAMETER_INVALID = -6           ' ������Ч
    Public Const CAMERA_STATUS_PARAMETER_OUT_OF_BOUND = -7      ' ����Խ��
    Public Const CAMERA_STATUS_UNENABLED = -8                   ' δʹ��
    Public Const CAMERA_STATUS_USER_CANCEL = -9                 ' �û��ֶ�ȡ���ˣ�����roi�����ȡ��������
    Public Const CAMERA_STATUS_PATH_NOT_FOUND = -10             ' ע�����û���ҵ���Ӧ��·��
    Public Const CAMERA_STATUS_SIZE_DISMATCH = -11              ' ���ͼ�����ݳ��ȺͶ���ĳߴ粻ƥ��
    Public Const CAMERA_STATUS_TIME_OUT = -12                   ' ��ʱ����
    Public Const CAMERA_STATUS_IO_ERROR = -13                   ' Ӳ��IO����
    Public Const CAMERA_STATUS_COMM_ERROR = -14                 ' ͨѶ����
    Public Const CAMERA_STATUS_BUS_ERROR = -15                  ' ���ߴ���
    Public Const CAMERA_STATUS_NO_DEVICE_FOUND = -16            ' û�з����豸
    Public Const CAMERA_STATUS_NO_LOGIC_DEVICE_FOUND = -17      ' δ�ҵ��߼��豸
    Public Const CAMERA_STATUS_DEVICE_IS_OPENED = -18           ' �豸�Ѿ���
    Public Const CAMERA_STATUS_DEVICE_IS_CLOSED = -19           ' �豸�Ѿ��ر�
    Public Const CAMERA_STATUS_DEVICE_VEDIO_CLOSED = -20        ' û�д��豸��Ƶ������¼����صĺ���ʱ����������Ƶû�д򿪣���ط��ظô���
    Public Const CAMERA_STATUS_NO_MEMORY = -21                  ' û���㹻ϵͳ�ڴ�
    Public Const CAMERA_STATUS_FILE_CREATE_FAILED = -22         ' �����ļ�ʧ��
    Public Const CAMERA_STATUS_FILE_INVALID = -23               ' �ļ���ʽ��Ч
    Public Const CAMERA_STATUS_WRITE_PROTECTED = -24            ' д����������д
    Public Const CAMERA_STATUS_GRAB_FAILED = -25                ' ���ݲɼ�ʧ��
    Public Const CAMERA_STATUS_LOST_DATA = -26                  ' ���ݶ�ʧ��������
    Public Const CAMERA_STATUS_EOF_ERROR = -27                  ' δ���յ�֡������
    Public Const CAMERA_STATUS_BUSY = -28                       ' ��æ(��һ�β������ڽ�����)���˴β������ܽ���
    Public Const CAMERA_STATUS_WAIT = -29                       ' ��Ҫ�ȴ�(���в���������������)�������ٴγ���
    Public Const CAMERA_STATUS_IN_PROCESS = -30                 ' ���ڽ��У��Ѿ���������
    Public Const CAMERA_STATUS_IIC_ERROR = -31                  ' IIC�������
    Public Const CAMERA_STATUS_SPI_ERROR = -32                  ' SPI�������
    Public Const CAMERA_STATUS_USB_CONTROL_ERROR = -33          ' USB���ƴ������
    Public Const CAMERA_STATUS_USB_BULK_ERROR = -34             ' USB BULK�������
    Public Const CAMERA_STATUS_SOCKET_INIT_ERROR = -35          ' ���紫���׼���ʼ��ʧ��
    Public Const CAMERA_STATUS_GIGE_FILTER_INIT_ERROR = -36     ' ��������ں˹���������ʼ��ʧ�ܣ������Ƿ���ȷ��װ���������������°�װ��
    Public Const CAMERA_STATUS_NET_SEND_ERROR = -37             ' �������ݷ��ʹ���
    Public Const CAMERA_STATUS_DEVICE_LOST = -38                ' ���������ʧȥ���ӣ�������ⳬʱ
    Public Const CAMERA_STATUS_DATA_RECV_LESS = -39             ' ���յ����ֽ������������ 
    Public Const CAMERA_STATUS_FUNCTION_LOAD_FAILED = -40       ' ���ļ��м��س���ʧ��
    Public Const CAMERA_STATUS_CRITICAL_FILE_LOST = -41         ' ����������������ļ���ʧ��
    Public Const CAMERA_STATUS_SENSOR_ID_DISMATCH = -42         ' �̼��ͳ���ƥ�䣬ԭ���������˴���Ĺ̼���
    Public Const CAMERA_STATUS_OUT_OF_RANGE = -43               ' ����������Ч��Χ��
    Public Const CAMERA_STATUS_REGISTRY_ERROR = -44             ' ��װ����ע����������°�װ���򣬻������а�װĿ¼Setup/Installer.exe
    Public Const CAMERA_STATUS_ACCESS_DENY = -45                ' ��ֹ���ʡ�ָ������Ѿ�����������ռ��ʱ����������ʸ�������᷵�ظ�״̬��(һ��������ܱ��������ͬʱ����)


    '��AIA�ƶ��ı�׼��ͬ

    Public Const CAMERA_AIA_PACKET_RESEND = &H100                             '��֡��Ҫ�ش�
    Public Const CAMERA_AIA_NOT_IMPLEMENTED = &H8001                          '�豸��֧�ֵ�����
    Public Const CAMERA_AIA_INVALID_PARAMETER = &H8002                        '��������Ƿ�
    Public Const CAMERA_AIA_INVALID_ADDRESS = &H8003                          '���ɷ��ʵĵ�ַ
    Public Const CAMERA_AIA_WRITE_PROTECT = &H8004                            '���ʵĶ��󲻿�д
    Public Const CAMERA_AIA_BAD_ALIGNMENT = &H8005                            '���ʵĵ�ַû�а���Ҫ�����
    Public Const CAMERA_AIA_ACCESS_DENIED = &H8006                            'û�з���Ȩ��
    Public Const CAMERA_AIA_BUSY = &H8007                                     '�������ڴ�����
    Public Const CAMERA_AIA_DEPRECATED = &H8008                               '&H8008-&H0800B  &H800F  ��ָ���Ѿ�����
    Public Const CAMERA_AIA_PACKET_UNAVAILABLE = &H800C                       '����Ч
    Public Const CAMERA_AIA_DATA_OVERRUN = &H800D                             '���������ͨ�����յ������ݱ���Ҫ�Ķ�
    Public Const CAMERA_AIA_INVALID_HEADER = &H800E                           '���ݰ�ͷ����ĳЩ������Э�鲻ƥ��
    Public Const CAMERA_AIA_PACKET_NOT_YET_AVAILABLE = &H8010                 'ͼ��ְ����ݻ�δ׼���ã������ڴ���ģʽ��Ӧ�ó�����ʳ�ʱ
    Public Const CAMERA_AIA_PACKET_AND_PREV_REMOVED_FROM_MEMORY = &H8011      '��Ҫ���ʵķְ��Ѿ������ڡ��������ش�ʱ�����Ѿ����ڻ�������
    Public Const CAMERA_AIA_PACKET_REMOVED_FROM_MEMORY = &H8012               'CAMERA_AIA_PACKET_AND_PREV_REMOVED_FROM_MEMORY
    Public Const CAMERA_AIA_NO_REF_TIME = &H813                               'û�вο�ʱ��Դ��������ʱ��ͬ��������ִ��ʱ
    Public Const CAMERA_AIA_PACKET_TEMPORARILY_UNAVAILABLE = &H814            '�����ŵ��������⣬��ǰ�ְ���ʱ�����ã����Ժ���з���
    Public Const CAMERA_AIA_OVERFLOW = &H815                                  '�豸�����������ͨ���Ƕ�������
    Public Const CAMERA_AIA_ACTION_LATE = &H816                               '����ִ���Ѿ�������Ч��ָ��ʱ��
    Public Const CAMERA_AIA_ERROR = &H8FFF                                    '����

End Module
