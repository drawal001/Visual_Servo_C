unit Snapshot;
//BIG5 TRANS ALLOWED
interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls;

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
BITMAPFILEHEADER = packed record
    bfType:shortint;
    bfSize:Cardinal;
    bfReserved1:shortint;
    bfReserved2:shortint;
    bfOffBits:Cardinal;
end;

type
BITMAPINFOHEADER = packed record
    biSize:Cardinal; //ָ���˽ṹ��ĳ��ȣ�Ϊ40
    biWidth:Integer; //λͼ��
    biHeight:Integer; //λͼ��
    biPlanes:shortint; //ƽ������Ϊ1
    biBitCount:shortint //������ɫλ����������1��2��4��8��16��24���µĿ�����32
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
  BitmapFileHead:BITMAPINFOHEADER;
  BitmapInfoHead:BITMAPINFOHEADE;
  Bitmap:TBitmap;
  iLastImageHeight:Integer;
  iLastImageWidth:Integer;


implementation

{$R *.dfm}

procedure TSnapshotForm.FormCreate(Sender: TObject);
begin
  iLastImageHeight := 0;
  iLastImageWidth := 0;
  BitmapStream := TmemoryStream.Create();
  Bitmap := TBitmap.Create();
  BitmapStream.Seek(0,soFromBeginning);
  //����һ��BITMAP ���͵��ļ�ͷ,д�뵽���У�������ֱ��ʸı�ʱ��ֻ�޸Ĳ���������
  BitmapStream.Write('ICE',3);

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
begin
   //�ȼ��ͼ��ֱ����Ƿ����˱仯������仯�ˣ������λͼͷ����Ϣ��
   if (iLastImageWidth <> pFrameHead.iWidth)
        or (iLastImageHeight <> pFrameHead.iHeight) then
        begin
        
        end;
   //
   BitmapStream.Write(BitmapHead,54);
   BitmapStream.Write(PByte,iLastImageWidth*iLastImageHeight*3);

   BitmapStream.Position := 0;
   Bitmap.LoadFromStream(BitmapStream);
   SnapshotImage.Picture.Assign( Bitmap );
end;

end.
