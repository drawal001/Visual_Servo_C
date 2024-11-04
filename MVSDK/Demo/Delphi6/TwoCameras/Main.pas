//����ִ�����̣�
//1,initialization�У�ɨ����������û�����ӵ����������ֱ���˳�
//2,FormCreate�У���ʼ�����(�����ڴ桢��ʼ����ʾ������ͼ��ץȡ�ص�����CameraGrabCallBack)
//3,CameraGrabCallBack,ͼ��ץȡ�ص�������SDK��ÿ����һ֡���ú����ᱻ���ã�
//  �������У�CameraGrabCallBack��ʵ����ͼ�����ʾ��
//BIG5 TRANS ALLOWED
unit Main;

interface


{��������SDK��Ԫ (CameraApi,CameraDefine, CameraStatus) }
uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, TSnapshot, CameraApi, CameraDefine, CameraStatus;

type
  TForm1 = class(TForm)
    BtnPlay: TButton;
    BtnSettings: TButton;
    StatusBar: TStaticText;
    InfoUpdateTimer: TTimer;
    Displayer: TPanel;
    GrabTimer: TTimer;
    Displayer2: TPanel;
    Camera2Settings: TButton;
    BtnPlay2: TButton;
    procedure FormCreate(Sender: TObject);
    procedure BtnSettingsClick(Sender: TObject);
    procedure InfoUpdateTimerTimer(Sender: TObject);
    procedure Camera2SettingsClick(Sender: TObject);
    procedure BtnPlay2Click(Sender: TObject);
    procedure BtnPlayClick(Sender: TObject);


  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form1:TForm1;
  m_hCamera1:Integer;  //���1���
  m_hCamera2:Integer;  //���2���
  iStatus:Integer;    //��������ֵ
  CameraList:array[1..8] of tSdkCameraDevInfo; //����б����������СΪ8����ʾ���ֻɨ��8���豸
                                               //�����Ҫɨ�������豸���뽫�������С��Ϊ����Ҫ������
  iCameraNumber:Integer;        //����ĸ���
  m_iDisplayCount:Integer;      //�Ѿ���ʾ��֡��
  m_pRgbBufferAlligned1:PByte;   //���1����16�ֽڶ���������ĵ�ַ
  sCameraCapability1:tSdkCameraCapbility; //���1���������ṹ��
  m_pRgbBuffer2:PByte;           //���2 ͼ����Ļ�����
  m_pRgbBufferAlligned2:PByte;   //���2 m_pRgbBuffer����16�ֽڶ���������ĵ�ַ
  sCameraCapability2:tSdkCameraCapbility; //���2���������ṹ��

  m_bPause:LongBool;                     //����ָʾ���1�Ƿ���ͣ�ɼ�
  m_bPause2:LongBool;                    //����ָʾ���2�Ƿ���ͣ�ɼ�

  m_iLastFrameWidth:Integer;             //�������1��һ�β��񵽵�ͼ��Ŀ��
  m_iLastFrameHeight:Integer;            //�������1��һ�β��񵽵�ͼ��ĸ߶�

  m_iLastFrameWidth2:Integer;             //�������2��һ�β��񵽵�ͼ��Ŀ��
  m_iLastFrameHeight2:Integer;            //�������2��һ�β��񵽵�ͼ��ĸ߶�

implementation

{$R *.dfm}

//���1ͼ�񲶻�Ļص�������������ʹ�ø÷�ʽ����ȡͼ����Ҳ���ò��ö��̻߳��߶�ʱ����Ȼ��
//���ϵ���CameraGetImageBuffer����ȡͼ��
procedure CameraGrabCallBack(
hCamera:CameraHandle;
pFrameBuffer:PByte;
pFrameHead:PtSdkFrameHead;
pContext:Pointer
); stdcall;
var
pDisplayFram:PInteger;

begin

pDisplayFram :=  PInteger(pContext);
pDisplayFram^ :=  pDisplayFram^+1;
    //CameraImageProcess������������������16�ֽڶ���Ļ�����
    CameraImageProcess(
                        m_hCamera1,
                        pFrameBuffer,
                        m_pRgbBufferAlligned1,
                        pFrameHead
                        );
    CameraImageOverlay(m_hCamera1, m_pRgbBufferAlligned1, pFrameHead);
    CameraDisplayRGB24(m_hCamera1, m_pRgbBufferAlligned1, pFrameHead);

    if (m_iLastFrameWidth <> pFrameHead.iWidth)
        or (m_iLastFrameHeight <> pFrameHead.iHeight) then
            Form1.Displayer.Invalidate();

    m_iLastFrameWidth := pFrameHead.iWidth;
    m_iLastFrameHeight := pFrameHead.iHeight;


end;


//���2ͼ�񲶻�Ļص�������������ʹ�ø÷�ʽ����ȡͼ����Ҳ���ò��ö��̻߳��߶�ʱ����Ȼ��
//���ϵ���CameraGetImageBuffer����ȡͼ��
procedure CameraGrabCallBack2(
hCamera:CameraHandle;
pFrameBuffer:PByte;
pFrameHead:PtSdkFrameHead;
pContext:Pointer
); stdcall;
var
pDisplayFram:PInteger;
begin

    //CameraImageProcess������������������16�ֽڶ���Ļ�����
    CameraImageProcess(
                        m_hCamera2,
                        pFrameBuffer,
                        m_pRgbBufferAlligned2,
                        pFrameHead
                        );
    CameraImageOverlay(m_hCamera2, m_pRgbBufferAlligned2, pFrameHead);
    CameraDisplayRGB24(m_hCamera2, m_pRgbBufferAlligned2, pFrameHead);

    if (m_iLastFrameWidth2 <> pFrameHead.iWidth)
        or (m_iLastFrameHeight2 <> pFrameHead.iHeight) then
            Form1.Displayer2.Invalidate();

    m_iLastFrameWidth2 := pFrameHead.iWidth;
    m_iLastFrameHeight2 := pFrameHead.iHeight;


end;

procedure CameraSettingPageCallback(
hCamera:CameraHandle;
MSG:Cardinal;
uParam:Cardinal;
pContext:Pointer
); stdcall;
begin



end;


procedure TForm1.FormCreate(Sender: TObject);
var
iStatus:Integer;
begin
   //----------------------��ʼ����� 1 --------------------------------
   iStatus := CameraInit(@CameraList[1],-1,-1,@m_hCamera1);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('�����ʼ��ʧ�ܣ����򼴽��˳���');
        halt;
        end;
   //���������ԣ��������ֱ���������һ���ڴ棬����ͼ������м仺����,�����1024�����������е�ַ����
   iStatus := CameraGetCapability(m_hCamera1,@sCameraCapability1); //check iStatus if need
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;
   //��ΪDelphi6 �� GetMemory���صĵ�ַ��һ����16�ֽڶ���ģ�������ķ�ʽ���������
   m_pRgbBufferAlligned1 :=  CameraAlignMalloc(sCameraCapability1.sResolutionRange.iWidthMax*
                             sCameraCapability1.sResolutionRange.iHeightMax*4,16);

   //�����豸�������ô���
   iStatus := CameraCreateSettingPage(
                                m_hCamera1,
                                Form1.Handle,
				@CameraList[1].acFriendlyName,
                                @CameraSettingPageCallback,
                                Pointer(0),
                                0);
                                
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;

   //����ͼ�񲶻�Ļص�����
   iStatus := CameraSetCallbackFunction(
                                m_hCamera1,
                                @CameraGrabCallBack,
                                Pointer(@m_iDisplayCount),
                                Pointer(0)
                                );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('����ͼ��ץȡ�ص�����ʧ�ܣ����򼴽��˳���');
        CameraUnInit(m_hCamera1);
        m_hCamera1 := 0;
        halt;
        end;
        
   //��ʼ����ʾģ�飬����ʹ����SDK�з�װ����ʾ�ӿڽ���ͼ����ʾ����Ҳ�����Լ���������ʽ
   //ʵ��ͼ�����ʾ���������Ҫʹ��SDK�е���ʾ��غ����������������µĳ�ʼ����
   iStatus := CameraDisplayInit(m_hCamera1,Form1.Displayer.Handle);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;
   //������ڿ��Զ�̬�ı��С���ٴ�CameraSetDisplaySize����Ϊ�µĿ��ֵ���ɡ�
   iStatus := CameraSetDisplaySize(
                                   m_hCamera1,
                                   Form1.Displayer.Width,
                                   Form1.Displayer.Height
                                   );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;


   //�����1��ʼ����
   CameraPlay(m_hCamera1);
   m_bPause := FALSE;

    //---------------------��ʼ����� 2 -------------------------
   iStatus := CameraInit(@CameraList[2],-1,-1,@m_hCamera2);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('���2��ʼ��ʧ�ܣ����򼴽��˳���');
        halt;
        end;
   //���������ԣ��������ֱ���������һ���ڴ棬����ͼ������м仺����,�����1024�����������е�ַ����
   iStatus := CameraGetCapability(m_hCamera2,@sCameraCapability2); //check iStatus if need
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;

   //��ΪDelphi6 �� GetMemory���صĵ�ַ��һ����16�ֽڶ���ģ�������ķ�ʽ���������
   m_pRgbBufferAlligned2 :=  CameraAlignMalloc(sCameraCapability2.sResolutionRange.iWidthMax*
                             sCameraCapability2.sResolutionRange.iHeightMax*4,16);

   //�����豸�������ô���
   iStatus := CameraCreateSettingPage(
                                m_hCamera2,
                                Form1.Handle,
				@CameraList[2].acFriendlyName,
                                Pointer(0),
                                Pointer(0),
                                0);
                                
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;

   //����ͼ�񲶻�Ļص�����
   iStatus := CameraSetCallbackFunction(
                                m_hCamera2,
                                @CameraGrabCallBack2,
                                Pointer(@m_iDisplayCount),
                                Pointer(0)
                                );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('����ͼ��ץȡ�ص�����ʧ�ܣ����򼴽��˳���');
        CameraUnInit(m_hCamera2);
        m_hCamera1 := 0;
        halt;
        end;
        
   //��ʼ����ʾģ�飬����ʹ����SDK�з�װ����ʾ�ӿڽ���ͼ����ʾ����Ҳ�����Լ���������ʽ
   //ʵ��ͼ�����ʾ���������Ҫʹ��SDK�е���ʾ��غ����������������µĳ�ʼ����
   iStatus := CameraDisplayInit(m_hCamera2,Form1.Displayer2.Handle);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;
   //������ڿ��Զ�̬�ı��С���ٴ�CameraSetDisplaySize����Ϊ�µĿ��ֵ���ɡ�
   iStatus := CameraSetDisplaySize(
                                   m_hCamera2,
                                   Form1.Displayer2.Width,
                                   Form1.Displayer2.Height
                                   );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;


   //�����2��ʼ����
   CameraPlay(m_hCamera2);
   m_bPause2 := FALSE;
end;


procedure TForm1.BtnSettingsClick(Sender: TObject);
begin
     CameraShowSettingPage(m_hCamera1,TRUE);
end;

procedure TForm1.InfoUpdateTimerTimer(Sender: TObject);
begin

       StatusBar.Caption :=  Format(
       '| Resolution:%d X %d | Display frames:%d |',
       [m_iLastFrameWidth,
       m_iLastFrameHeight,
       m_iDisplayCount]
       );
end;

procedure TForm1.Camera2SettingsClick(Sender: TObject);
begin
      CameraShowSettingPage(m_hCamera2,TRUE);
end;

procedure TForm1.BtnPlay2Click(Sender: TObject);
begin
     if m_bPause2 = TRUE then
        begin
        m_bPause2 := FALSE;
        BtnPlay2.Caption := 'Pause';
        CameraPlay(m_hCamera2);
        end
     else
        begin
        m_bPause2 := TRUE;
        BtnPlay2.Caption := 'Play';
        CameraPause(m_hCamera2);//��ͣ�����ֹͣ���ͼ��Ԥ����ץ�Ķ���Ч��
        end;
end;

procedure TForm1.BtnPlayClick(Sender: TObject);
begin
  if m_bPause = TRUE then
        begin
        m_bPause := FALSE;
        BtnPlay.Caption := 'Pause';
        CameraPlay(m_hCamera1);
        end
     else
        begin
        m_bPause := TRUE;
        BtnPlay.Caption := 'Play';
        CameraPause(m_hCamera1);//��ͣ�����ֹͣ���ͼ��Ԥ����ץ�Ķ���Ч��
        end;
end;

initialization
  //��Ԫ��ʼ��ʱ�ж���������������г�ʼ��
   m_hCamera1     := 0;
   m_pRgbBufferAlligned1  := nil;

   m_hCamera2     := 0;
   m_pRgbBufferAlligned2  := nil;

   iCameraNumber := high(CameraList);      //iCameraNumber�������������ɨ����������������Ϊ0��


   //ɨ�����
   iStatus := CameraEnumerateDevice(@CameraList,@iCameraNumber);
   if (iStatus <> CAMERA_STATUS_SUCCESS) or (iCameraNumber < 2) then
        begin
        ShowMessage('This demo need two cameras!');
        halt;
        end;


                                
finalization
   //��Ԫ�˳�ʱ�ر�������ͷ���Դ

   //��� m_hCamera1��Ϊ0�����ʾ�Ѿ���ʼ�������������з���ʼ����
   if m_hCamera1 <> 0 then
   CameraUnInit(m_hCamera1);

   if m_pRgbBufferAlligned1 <> nil then
   CameraAlignFree(m_pRgbBufferAlligned1);

    if m_hCamera2 <> 0 then
   CameraUnInit(m_hCamera2);

   if m_pRgbBufferAlligned2 <> nil then
   CameraAlignFree(m_pRgbBufferAlligned2);
   
end.
