unit TSnapshot;
//BIG5 TRANS ALLOWED
interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, CameraApi, CameraDefine, CameraStatus;

type
  TSnapshotForm = class(TForm)
    SnapshotImage: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  
type
PBITMAPFILEHEADER = ^BITMAPFILEHEADER;
BITMAPFILEHEADER = packed record
    bfType:array[1..2] of char;
    bfSize:Cardinal;
    bfReserved1:WORD;
    bfReserved2:WORD;
    bfOffBits:Cardinal;
end;

type
PBITMAPINFOHEADER = ^BITMAPINFOHEADER;
BITMAPINFOHEADER = packed record
    biSize:Cardinal; //ָ���˽ṹ��ĳ��ȣ�Ϊ40
    biWidth:Integer; //λͼ��
    biHeight:Integer; //λͼ��
    biPlanes:WORD; //ƽ������Ϊ1
    biBitCount:WORD; //������ɫλ����������1��2��4��8��16��24���µĿ�����32
    biCompression:Cardinal; //ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
    biSizeImage:Cardinal; //ʵ��λͼ����ռ�õ��ֽ���
    biXPelsPerMeter:Integer; //X����ֱ���
    biYPelsPerMeter:Integer; //Y����ֱ���
    biClrUsed:Cardinal; //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
    biClrImportant:Cardinal; //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
end;


var
  SnapshotForm: TSnapshotForm;
  BitmapStream:TMemoryStream;
  BitmapFileHead:BITMAPFILEHEADER;
  BitmapInfoHead:BITMAPINFOHEADER;
  Bitmap:TBitmap;
  iLastImageHeight:Integer;
  iLastImageWidth:Integer;
  BitmapHead:array[1..54] of Byte;


procedure UpdateImage(
                        pFrameBuffer:PByte;
                        pFrameHead:PtSdkFrameHead
                      );

implementation

{$R *.dfm}

procedure TSnapshotForm.FormCreate(Sender: TObject);
var
pBitmapFileHead:PBITMAPFILEHEADER;
pBitmapInfoHead:PBITMAPINFOHEADER;
begin
  iLastImageHeight := 0;
  iLastImageWidth := 0;
  BitmapStream := TmemoryStream.Create();
  BitmapStream.SetSize(1280*1024*3+54);
  Bitmap := TBitmap.Create();
  BitmapStream.Seek(0,soFromBeginning);
  //����һ��BITMAP ���͵��ļ�ͷ,д�뵽���У�������ֱ��ʸı�ʱ��ֻ�޸Ĳ���������
  pBitmapFileHead := PBITMAPFILEHEADER(@BitmapHead[1]);
  pBitmapInfoHead := PBITMAPINFOHEADER(@BitmapHead[15]);
  pBitmapFileHead.bfType[1]         := 'B';
  pBitmapFileHead.bfType[2]         := 'M';
  pBitmapFileHead.bfSize            := 54;
  pBitmapFileHead.bfReserved1       := 0;
  pBitmapFileHead.bfReserved2       := 0;
  pBitmapFileHead.bfOffBits         := 54;
  pBitmapInfoHead.biSize            := 40;
  pBitmapInfoHead.biWidth           := 0;//ͼ��Ŀ�ߣ���Ҫ��̬�޸�
  pBitmapInfoHead.biHeight          := 0;
  pBitmapInfoHead.biPlanes          := 1;
  pBitmapInfoHead.biBitCount        := 24;
  pBitmapInfoHead.biCompression     := 0;
  pBitmapInfoHead.biSizeImage       := 0;
  pBitmapInfoHead.biXPelsPerMeter   := 0;
  pBitmapInfoHead.biYPelsPerMeter   := 0;
  pBitmapInfoHead.biClrUsed         := 0;
  pBitmapInfoHead.biClrImportant    := 0;
  

end;

procedure TSnapshotForm.FormDestroy(Sender: TObject);
begin
  BitmapStream.Destroy();
  Bitmap.Destroy();
end;

procedure UpdateImage(
                        pFrameBuffer:PByte;
                        pFrameHead:PtSdkFrameHead
                      );
var
pBitmapFileHead:PBITMAPFILEHEADER;
pBitmapInfoHead:PBITMAPINFOHEADER;
begin
        //pFrameHead.iWidth := 64;
        //pFrameHead.iHeight := 64;
   //�ȼ��ͼ��ֱ����Ƿ����˱仯������仯�ˣ������λͼͷ����Ϣ��
   if (iLastImageWidth <> pFrameHead.iWidth)
        or (iLastImageHeight <> pFrameHead.iHeight) then
        begin
        pBitmapFileHead := PBITMAPFILEHEADER(@BitmapHead[1]);
        pBitmapInfoHead := PBITMAPINFOHEADER(@BitmapHead[15]);
        pBitmapFileHead.bfSize := 54 +  (pFrameHead.iWidth*pFrameHead.iHeight*3);
        pBitmapInfoHead.biWidth := pFrameHead.iWidth;
        pBitmapInfoHead.biHeight := pFrameHead.iHeight;
        iLastImageWidth := pFrameHead.iWidth;
        iLastImageHeight :=  pFrameHead.iHeight;
        end;
   
   BitmapStream.Position := 0;
   BitmapStream.Write(BitmapHead,54);
   BitmapStream.Write(pFrameBuffer^,iLastImageWidth*iLastImageHeight*3);
   //BitmapStream.SaveToFile('c:\test.Bmp');  //�����Ҫ������ļ����ͷ�ע����һ��

   BitmapStream.Position := 0;
   Bitmap.LoadFromStream(BitmapStream);
   SnapshotForm.SnapshotImage.Picture.Assign( Bitmap );
   SnapshotForm.Show();
end;

end.
