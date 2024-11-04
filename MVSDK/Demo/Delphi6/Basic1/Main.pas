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
  Dialogs, StdCtrls, ExtCtrls, CameraApi, CameraDefine, CameraStatus;

type
  TForm1 = class(TForm)
    BtnPlay: TButton;
    BtnSettings: TButton;
    BtnSnapshot: TButton;
    StatusBar: TStaticText;
    InfoUpdateTimer: TTimer;
    Displayer: TPanel;
    procedure FormCreate(Sender: TObject);
    procedure BtnSettingsClick(Sender: TObject);
    procedure BtnPlayClick(Sender: TObject);
    procedure InfoUpdateTimerTimer(Sender: TObject);
    procedure BtnSnapshotClick(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form1: TForm1;
  m_hCamera:Integer;  //������
  iStatus:Integer;    //��������ֵ
  CameraList:array[1..8] of tSdkCameraDevInfo; //����б����������СΪ8����ʾ���ֻɨ��8���豸
                                               //�����Ҫɨ�������豸���뽫�������С��Ϊ����Ҫ������
  iCameraNumber:Integer;        //����ĸ���
  m_iDisplayCount:Integer;      //�Ѿ���ʾ��֡��
  m_pRgbBufferAlligned:PByte;   //����16�ֽڶ���������ĵ�ַ
  sCameraCapability:tSdkCameraCapbility; //������������ṹ��
  m_bPause:LongBool;                     //����ָʾ�Ƿ���ͣ�ɼ�
  m_iLastFrameWidth:Integer;             //������һ�β��񵽵�ͼ��Ŀ��
  m_iLastFrameHeight:Integer;            //������һ�β��񵽵�ͼ��ĸ߶�
  m_iSnapshotTimes:Integer;              //��¼ץ�µĴ���
implementation

{$R *.dfm}

//ͼ�񲶻�Ļص�������������ʹ�ø÷�ʽ����ȡͼ����Ҳ���ò��ö��̻߳��߶�ʱ����Ȼ��
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
                        m_hCamera,
                        pFrameBuffer,
                        m_pRgbBufferAlligned,
                        pFrameHead
                        );
    CameraImageOverlay(m_hCamera, m_pRgbBufferAlligned, pFrameHead);
    CameraDisplayRGB24(m_hCamera, m_pRgbBufferAlligned, pFrameHead);

    if (m_iLastFrameWidth <> pFrameHead.iWidth)
        or (m_iLastFrameHeight <> pFrameHead.iHeight) then
            Form1.Displayer.Invalidate();

    m_iLastFrameWidth := pFrameHead.iWidth;
    m_iLastFrameHeight := pFrameHead.iHeight;


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
sImageSize:tSdkImageResolution;
begin
       //��ʼ�����
   iStatus := CameraInit(@CameraList[1],-1,-1,@m_hCamera);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('�����ʼ��ʧ�ܣ����򼴽��˳���');
        halt;
        end;
   //���������ԣ��������ֱ���������һ���ڴ棬����ͼ������м仺����,�����1024�����������е�ַ����
   iStatus := CameraGetCapability(m_hCamera,@sCameraCapability); //check iStatus if need
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;

   m_pRgbBufferAlligned := CameraAlignMalloc(sCameraCapability.sResolutionRange.iWidthMax * sCameraCapability.sResolutionRange.iHeightMax * 4,16);
   //��ΪDelphi6 �� GetMemory���صĵ�ַ����16�ֽڶ���ģ������SDK�ṩ��CameraAlignMalloc�����ڴ�

   //�����豸�������ô���
   iStatus := CameraCreateSettingPage(
                                m_hCamera,
                                Form1.Handle,
				@CameraList[1].acFriendlyName,
                                @CameraSettingPageCallback,
                                Pointer(0),
                                0);
                                
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        halt;

   //����ͼ�񲶻�Ļص�����
   iStatus := CameraSetCallbackFunction(
                                m_hCamera,
                                @CameraGrabCallBack,
                                Pointer(@m_iDisplayCount),
                                Pointer(0)
                                );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('����ͼ��ץȡ�ص�����ʧ�ܣ����򼴽��˳���');
        CameraUnInit(m_hCamera);
        m_hCamera := 0;
        halt;
        end;
        
   //��ʼ����ʾģ�飬����ʹ����SDK�з�װ����ʾ�ӿڽ���ͼ����ʾ����Ҳ�����Լ���������ʽ
   //ʵ��ͼ�����ʾ���������Ҫʹ��SDK�е���ʾ��غ����������������µĳ�ʼ����
   iStatus := CameraDisplayInit(m_hCamera,Form1.Displayer.Handle);
   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;
   //������ڿ��Զ�̬�ı��С���ٴ�CameraSetDisplaySize����Ϊ�µĿ��ֵ���ɡ�
   iStatus := CameraSetDisplaySize(
                                   m_hCamera,
                                   Form1.Displayer.Width,
                                   Form1.Displayer.Height
                                   );

   if iStatus <>  CAMERA_STATUS_SUCCESS then
        begin
        halt;
        end;

     //����ץ�ĵķֱ��ʺ�Ԥ����ͬ
     // iIndex Ϊ0XFF����������Ϊ0 ����ʾ��Ԥ����ͬ�ķֱ��ʽ���ץ�ġ�
     // �����Ҫ��������ץ�ĵķֱ��ʣ�����������Ϊ��Ҫ�ķֱ���
     sImageSize.iIndex := $ff;
     sImageSize.iWidth := 0;
     sImageSize.iHeight:= 0;
     sImageSize.iHOffsetFOV := 0;
     sImageSize.iVOffsetFOV := 0;
     sImageSize.iWidthFOV := 0;
     sImageSize.iHeightFOV := 0;
     
     CameraSetResolutionForSnap(m_hCamera,@sImageSize);

   //�������ʼ����
   CameraPlay(m_hCamera);
   m_bPause := FALSE;

end;

procedure TForm1.BtnSettingsClick(Sender: TObject);
begin
     CameraShowSettingPage(m_hCamera,TRUE);
end;

procedure TForm1.BtnPlayClick(Sender: TObject);
begin
     if m_bPause = TRUE then
        begin
        m_bPause := FALSE;
        BtnPlay.Caption := 'Pause';
        CameraPlay(m_hCamera);
        end
     else
        begin
        m_bPause := TRUE;
        BtnPlay.Caption := 'Play';
        CameraPause(m_hCamera);//��ͣ�����ֹͣ���ͼ��Ԥ����ץ�Ķ���Ч��
        end;
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

procedure TForm1.BtnSnapshotClick(Sender: TObject);
var
pRawBuffer:PByte;
pRgbBufferAlligned:PByte;
iStatus:Integer;
FrameInfo:tSdkFrameHead;
sPath:array[0..255] of char;
sFileName:string;
sCurPath:string;
begin
      //�����ļ���

      sCurPath := ExtractFilePath(Application.ExeName);
      sFileName := Format('%sSnapshot%d.BMP',[sCurPath,m_iSnapshotTimes]);
      
      strPCopy(sPath,sFileName);

      //CameraSnapToBufferץ��һ֡ͼ���ڴ���
	  // !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
      iStatus := CameraSnapToBuffer(m_hCamera,@FrameInfo,@pRawBuffer,1000);

      if  iStatus = CAMERA_STATUS_SUCCESS then
      begin
      pRgbBufferAlligned := CameraAlignMalloc(FrameInfo.iWidth*FrameInfo.iHeight*4,16);
      CameraImageProcess(m_hCamera,pRawBuffer,pRgbBufferAlligned,@FrameInfo);
      CameraReleaseImageBuffer(m_hCamera,pRawBuffer);

      //�������SDK�ӿ�CameraSaveImage����ͼƬ���棬��Ҳ�����Խ�pRgbBufferAlligned������ʾ����ʹ��
      //����Ľӿڽ��б���ͼƬ��Basic2��������ʾ����ν�pRgbBufferAlligned�е�
      //ͼ����ʾ��Timage�ؼ��ϡ�
      iStatus := CameraSaveImage(
                      m_hCamera,
                      @sPath,
                      pRgbBufferAlligned,//���Ҫ����RAW���ݣ�����ҪCameraImageProcess�����ﻺ���� pRawBuffer
                      @FrameInfo,
                      2,//FILE_JPG = 1 ;FILE_BMP = 2; FILE_RAW = 4;FILE_PNG = 8
                      100 //ͼ���������ӣ�����FILE_JPG����ʱ��Ч��100Ϊ���1��С
                      );
      CameraAlignFree(pRgbBufferAlligned);
      m_iSnapshotTimes := m_iSnapshotTimes + 1;
      if  iStatus = CAMERA_STATUS_SUCCESS then
        ShowMessage('Snapshot OK!File save in:'+sFileName)
      else
        ShowMessage('Failed to save image')
      end
      else
        ShowMessage('Snapshot failed!Do you set the camera in pause mode ?Resume it and try angin');
end;

initialization
  //��Ԫ��ʼ��ʱ�ж���������������г�ʼ��
   m_hCamera     := 0;
   m_pRgbBufferAlligned := nil;
   iCameraNumber := high(CameraList);     //iCameraNumber�������������ɨ����������������Ϊ0��

   //ɨ�����
   iStatus := CameraEnumerateDevice(@CameraList,@iCameraNumber);
   if iStatus <> CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('û�м�⵽��������򼴽��˳������������������б����̡�');
        halt;
        end;

finalization
   //��Ԫ�˳�ʱ�ر�������ͷ���Դ
         
   if m_hCamera <> 0 then
   CameraUnInit(m_hCamera);

   if m_pRgbBufferAlligned <> nil then
   CameraAlignFree(m_pRgbBufferAlligned);


end.
