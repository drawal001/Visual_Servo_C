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
    BtnSnapshot: TButton;
    StatusBar: TStaticText;
    InfoUpdateTimer: TTimer;
    Displayer: TPanel;
    GrabTimer: TTimer;
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
  Form1:TForm1;
  SnapshotFrm:TSnapshotForm;
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
     // ��������ץ�ĵķֱ��ʣ�����������Ϊ��Ҫ�ķֱ���
     // ��������ʾ����ν��й̶��ֱ���ץ�ģ�������Ԥ����ʲô�ֱ��ʣ�ץ�ĵ�ͼ��
     // �Ǹ�����������ķֱ��ʡ�
     sImageSize.iIndex := $ff;
     sImageSize.iWidth := sCameraCapability.sResolutionRange.iWidthMax;  //���Ҫ��Ԥ��һ���ķֱ���ץ�ģ�iWidth := 0
     sImageSize.iHeight:= sCameraCapability.sResolutionRange.iHeightMax; //���Ҫ��Ԥ��һ���ķֱ���ץ�ģ�iHeight := 0
     sImageSize.iHOffsetFOV := 0;
     sImageSize.iVOffsetFOV := 0;
     sImageSize.iWidthFOV := sCameraCapability.sResolutionRange.iWidthMax;
     sImageSize.iHeightFOV := sCameraCapability.sResolutionRange.iHeightMax;
     sImageSize.uBinSumMode := 0;
     sImageSize.uBinAverageMode := 0;
     sImageSize.uSkipMode := 0;
     sImageSize.uResampleMask := 0;
     sImageSize.iWidthZoomHd := 0;
     sImageSize.iHeightZoomHd := 0;
     sImageSize.iWidthZoomSw := 0;
     sImageSize.iHeightZoomSw := 0;
     
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
begin
      //ץ��һ֡ͼ���ڴ���
	  // !!!!!!ע�⣺CameraSnapToBuffer ���л��ֱ������գ��ٶȽ�������ʵʱ��������CameraGetImageBuffer����ȡͼ���߻ص�������
      iStatus := CameraSnapToBuffer(m_hCamera,@FrameInfo,@pRawBuffer,1000);
      
      if  iStatus = CAMERA_STATUS_SUCCESS then
      begin
      //�������Ҫ����ץ�ĵĻ�����������һ����������ֱ���ƥ��Ļ�������
      //������ÿ��ץ�ĵ�ʱ����������pRgbBuffer��
      pRgbBufferAlligned := CameraAlignMalloc(FrameInfo.iWidth*FrameInfo.iHeight*4,16);
 
      //�����ԭʼ����pRawBufferת��ΪRGB��ʽ
      CameraImageProcess(m_hCamera,pRawBuffer,pRgbBufferAlligned,@FrameInfo);

      //�ͷŻ�������pRawBuffer��SDK�ڲ�ʹ�õĻ��壬�������Delphi���ڴ��ͷź���
      //���ͷ�pRawBuffer��
      CameraReleaseImageBuffer(m_hCamera,pRawBuffer);

      //����TImage�ؼ���ʾ
      UpdateImage(pRgbBufferAlligned,@FrameInfo);

      CameraAlignFree(pRgbBufferAlligned);
      end
      else
      ShowMessage('Snapshot failed,do you set the camera in pause mode?Resume it and try angain');

end;

initialization
  //��Ԫ��ʼ��ʱ�ж���������������г�ʼ��
   m_hCamera     := 0;
   m_pRgbBufferAlligned  := nil;
   iCameraNumber := high(CameraList);      //iCameraNumber�������������ɨ����������������Ϊ0��


   //ɨ�����
   iStatus := CameraEnumerateDevice(@CameraList,@iCameraNumber);
   if iStatus <> CAMERA_STATUS_SUCCESS then
        begin
        ShowMessage('No camera was found!');
        halt;
        end;


                                
finalization
   //��Ԫ�˳�ʱ�ر�������ͷ���Դ

   //��� m_hCamera��Ϊ0�����ʾ�Ѿ���ʼ�������������з���ʼ����
   if m_hCamera <> 0 then
   CameraUnInit(m_hCamera);

   if m_pRgbBufferAlligned <> nil then
   CameraAlignFree(m_pRgbBufferAlligned);
   
end.
