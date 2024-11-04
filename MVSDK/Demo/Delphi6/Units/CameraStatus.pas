unit CameraStatus;
//* SDK�ӿڷ���ֵ���� */
interface
//BIG5 TRANS ALLOWED
const

CAMERA_STATUS_SUCCESS                 = 0;   // �����ɹ�
CAMERA_STATUS_FAILED                  =-1;   // ����ʧ��
CAMERA_STATUS_INTERNAL_ERROR          =-2;   // �ڲ�����
CAMERA_STATUS_UNKNOW                  =-3;   // δ֪����
CAMERA_STATUS_NOT_SUPPORTED           =-4;   // ��֧�ָù���
CAMERA_STATUS_NOT_INITIALIZED         =-5;   // ��ʼ��δ���
CAMERA_STATUS_PARAMETER_INVALID       =-6;   // ������Ч
CAMERA_STATUS_PARAMETER_OUT_OF_BOUND  =-7;   // ����Խ��
CAMERA_STATUS_UNENABLED               =-8;   // δʹ��
CAMERA_STATUS_USER_CANCEL             =-9;   // �û��ֶ�ȡ���ˣ�����roi�����ȡ��������
CAMERA_STATUS_PATH_NOT_FOUND          =-10;  // ע�����û���ҵ���Ӧ��·��
CAMERA_STATUS_SIZE_DISMATCH           =-11;  // ���ͼ�����ݳ��ȺͶ���ĳߴ粻ƥ��
CAMERA_STATUS_TIME_OUT                =-12;  // ��ʱ����
CAMERA_STATUS_IO_ERROR                =-13;  // Ӳ��IO����
CAMERA_STATUS_COMM_ERROR              =-14;  // ͨѶ����
CAMERA_STATUS_BUS_ERROR               =-15;  // ���ߴ���
CAMERA_STATUS_NO_DEVICE_FOUND         =-16;  // û�з����豸
CAMERA_STATUS_NO_LOGIC_DEVICE_FOUND   =-17;  // δ�ҵ��߼��豸
CAMERA_STATUS_DEVICE_IS_OPENED        =-18;  // �豸�Ѿ���
CAMERA_STATUS_DEVICE_IS_CLOSED        =-19;  // �豸�Ѿ��ر�
CAMERA_STATUS_DEVICE_VEDIO_CLOSED     =-20;  // û�д��豸��Ƶ������¼����صĺ���ʱ����������Ƶû�д򿪣���ط��ظô���
CAMERA_STATUS_NO_MEMORY               =-21;  // û���㹻ϵͳ�ڴ�
CAMERA_STATUS_FILE_CREATE_FAILED      =-22;  // �����ļ�ʧ��
CAMERA_STATUS_FILE_INVALID            =-23;  // �ļ���ʽ��Ч
CAMERA_STATUS_WRITE_PROTECTED         =-24;  // д����������д
CAMERA_STATUS_GRAB_FAILED             =-25;  // ���ݲɼ�ʧ��
CAMERA_STATUS_LOST_DATA               =-26;  // ���ݶ�ʧ��������
CAMERA_STATUS_EOF_ERROR               =-27;  // δ���յ�֡������
CAMERA_STATUS_BUSY                    =-28;  // ��æ(��һ�β������ڽ�����)���˴β������ܽ���
CAMERA_STATUS_WAIT                    =-29;  // ��Ҫ�ȴ�(���в���������������)�������ٴγ���
CAMERA_STATUS_IN_PROCESS              =-30;  // ���ڽ��У��Ѿ���������
CAMERA_STATUS_IIC_ERROR               =-31;  // IIC�������
CAMERA_STATUS_SPI_ERROR               =-32;  // SPI�������
CAMERA_STATUS_USB_CONTROL_ERROR       =-33;  // USB���ƴ������
CAMERA_STATUS_USB_BULK_ERROR          =-34;  // USB BULK�������
CAMERA_STATUS_SOCKET_INIT_ERROR       =-35;  // ���紫���׼���ʼ��ʧ��
CAMERA_STATUS_GIGE_FILTER_INIT_ERROR  =-36;  // ��������ں˹���������ʼ��ʧ�ܣ������Ƿ���ȷ��װ���������������°�װ��
CAMERA_STATUS_NET_SEND_ERROR          =-37;  // �������ݷ��ʹ���
CAMERA_STATUS_DEVICE_LOST             =-38;  // ���������ʧȥ���ӣ�������ⳬʱ
CAMERA_STATUS_DATA_RECV_LESS          =-39;  // ���յ����ֽ������������ 
CAMERA_STATUS_FUNCTION_LOAD_FAILED    =-40;  // ���ļ��м��س���ʧ��
CAMERA_STATUS_CRITICAL_FILE_LOST      =-41;  // ����������������ļ���ʧ��
CAMERA_STATUS_SENSOR_ID_DISMATCH      =-42;  // �̼��ͳ���ƥ�䣬ԭ���������˴���Ĺ̼���
CAMERA_STATUS_OUT_OF_RANGE            =-43;  // ����������Ч��Χ��
CAMERA_STATUS_REGISTRY_ERROR          =-44;  // ��װ����ע����������°�װ���򣬻������а�װĿ¼Setup/Installer.exe
CAMERA_STATUS_ACCESS_DENY             =-45;  // ��ֹ���ʡ�ָ������Ѿ�����������ռ��ʱ����������ʸ�������᷵�ظ�״̬��(һ��������ܱ��������ͬʱ����)
//��AIA�ƶ��ı�׼��ͬ�Ĵ����룬һ������SDK�ڲ�
CAMERA_AIA_PACKET_RESEND                          =$0100; //��֡��Ҫ�ش� ,
CAMERA_AIA_NOT_IMPLEMENTED                        =$8001; //�豸��֧�ֵ�����
CAMERA_AIA_INVALID_PARAMETER                      =$8002; //��������Ƿ�
CAMERA_AIA_INVALID_ADDRESS                        =$8003; //���ɷ��ʵĵ�ַ
CAMERA_AIA_WRITE_PROTECT                          =$8004; //���ʵĶ��󲻿�д
CAMERA_AIA_BAD_ALIGNMENT                          =$8005; //���ʵĵ�ַû�а���Ҫ�����
CAMERA_AIA_ACCESS_DENIED                          =$8006; //û�з���Ȩ��
CAMERA_AIA_BUSY                                   =$8007; //�������ڴ�����
CAMERA_AIA_DEPRECATED                             =$8008; //$8008-$0800B  $800F  ��ָ���Ѿ�����
CAMERA_AIA_PACKET_UNAVAILABLE                     =$800C; //����Ч
CAMERA_AIA_DATA_OVERRUN                           =$800D; //���������ͨ�����յ������ݱ���Ҫ�Ķ�
CAMERA_AIA_INVALID_HEADER                         =$800E; //���ݰ�ͷ����ĳЩ������Э�鲻ƥ��
CAMERA_AIA_PACKET_NOT_YET_AVAILABLE               =$8010; //ͼ��ְ����ݻ�δ׼���ã������ڴ���ģʽ��Ӧ�ó�����ʳ�ʱ
CAMERA_AIA_PACKET_AND_PREV_REMOVED_FROM_MEMORY    =$8011; //��Ҫ���ʵķְ��Ѿ������ڡ��������ش�ʱ�����Ѿ����ڻ�������
CAMERA_AIA_PACKET_REMOVED_FROM_MEMORY             =$8012; //CAMERA_AIA_PACKET_AND_PREV_REMOVED_FROM_MEMORY
CAMERA_AIA_NO_REF_TIME                            =$0813; //û�вο�ʱ��Դ��������ʱ��ͬ��������ִ��ʱ
CAMERA_AIA_PACKET_TEMPORARILY_UNAVAILABLE         =$0814; //�����ŵ��������⣬��ǰ�ְ���ʱ�����ã����Ժ���з���
CAMERA_AIA_OVERFLOW                               =$0815; //�豸�����������ͨ���Ƕ�������
CAMERA_AIA_ACTION_LATE                            =$0816; //����ִ���Ѿ�������Ч��ָ��ʱ��
CAMERA_AIA_ERROR                                  =$8FFF; //����


implementation

end.
