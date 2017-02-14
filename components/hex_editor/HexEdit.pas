unit HexEdit;

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, StdCtrls, Classes, Graphics, Controls, Forms,
  Dialogs, clipbrd, ByteArrayUtils, Chars;

const
  MinRows = 4;
  RowPixels = 14;
  ColPixels = 8;
  CursorRow = 13;
  
type
  TAddressSize = (as16bit, as32bit);
  TRecordSize = (rs8byte, rs16byte);
  TCurMode = (cmHex, cmAscii);

  THexEdit = class(TCustomControl)
  private
    { Private declarations }
    InitDone: Boolean;          // Impostato appena il componente è iniz.
    AddrPixSize: Integer;       // Larghezza campo indirizzi in pixel.
    RecPixSize: Integer;        // Larghezza campo hex in pixel.
    AsciiPixSize: Integer;      // Larghezza campo ascii in pixel.
    RecSize: DWord;             // Numero byte per riga.
    Buffer: TDynamicByteArray;  // Buffer dell'editor.
    Backup: TDynamicByteArray;  // Backup del buffer.
    Offset: DWord;              // Primo byte visibile.
    Limit: DWord;               // Ultimo byte visibile (relativo a Offset).
    FRows: DWord;               // Numero righe di edit nella finestra.
    TotalRows: DWord;           // Numero righe di edit totali.
    CurMode: TCurMode;          // Modo cursore.
    Cursor: DWord;              // Indirizzo cursore (relativo a Offset).
    Scroll: TScrollBar;
    FAddressSize: TAddressSize; // Tipo campo indirizzi.
    FRecordSize: TRecordSize;   // Tipo campo hex.
//    FBorderStyle: TBorderStyle; // Tipo bordo.
    function GetAddressSize: TAddressSize;
    procedure SetAddressSize(Size: TAddressSize);
    function GetRecordSize: TRecordSize;
    procedure SetRecordSize(Size: TRecordSize);
    function GetRows: Integer;
    procedure SetRows(Rows: Integer);
    function GetColor: TColor;
    procedure SetColor(NewColor: TColor);
    function GetTextColor: TColor;
    procedure SetTextColor(NewColor: TColor);
    function GetBufferSize: DWord;
    procedure SetBufferSize(Size: DWord);
//    procedure SetBorderStyle(Style: TBorderStyle);
    function ComputeBevelOffset: Integer;
    function ComputeWidth: Integer;
    procedure ComputeScrollMetrics;
    procedure SetScrollPosition;
    procedure CurOff;
    procedure CurOn;
    procedure CursorCoords(out c, r: Integer);
    procedure OutByte(Pos: DWord);
    procedure CurUp;
    procedure CurDown;
    procedure CurLeft;
    procedure CurRight;
    procedure ScrollLineDown;
    procedure ScrollLineUp;
    procedure ScrollPageDown;
    procedure ScrollPageUp;
    procedure CanResizeEvent(Sender: TObject;
                             var NewWidth, NewHeight: Integer;
                             var Resize: Boolean);
    procedure KeyDownEvent(Sender: TObject;
                           var Key: Word;
                           Shift: TShiftState);
    procedure KeyPressEvent(Sender: TObject; var Key: Char);
    procedure MouseDownEvent(Sender: TObject;
                             Button: TMouseButton;
                             Shift: TShiftState;
                             X, Y: Integer);
    procedure ScrollEvent(Sender: TObject; ScrollCode:
                          TScrollCode;
                          var ScrollPos: Integer);
    procedure FocusOn(Sender: TObject);
    procedure FocusOff(Sender: TObject);
    procedure RecalcLimit;
    procedure RecalcRows;
    procedure WMGetDlgCode(var Message: TWMGetDlgCode); message WM_GETDLGCODE;
  protected
    { Protected declarations }
    procedure Loaded; override;
    function CanAutoSize(var NewWidth, NewHeight: Integer): Boolean; override;
  public
    { Public declarations }
    Modified: Boolean;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Paint; override;
    procedure SetBuffer(NewBuffer: TDynamicByteArray);
    function GetBuffer: TDynamicByteArray;
    procedure Commit;
    procedure Rollback;
    procedure UndoCurrent;
    procedure Overwrite(text: String);
    property EditBuffer: TDynamicByteArray read GetBuffer write SetBuffer;
  published
    { Published declarations }
    property BorderStyle: TBorderStyle read GetBorderStyle write SetBorderStyle;
    property AddressSize: TAddressSize read GetAddressSize write SetAddressSize;
    property RecordSize: TRecordSize read GetRecordSize write SetRecordSize;
    property Rows: Integer read GetRows write SetRows;
    property Color: TColor read GetColor write SetColor;
    property TextColor: TColor read GetTextColor write SetTextColor;
    property BufferSize: DWord read GetBufferSize write SetBufferSize;
    property ShowHint;
    property DoubleBuffered;
//    property TabOrder;
    property TabStop;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('ChibiOS', [THexEdit]);
end;

//
// Intercetto il messaggio di WM_KEYDOWN per poter utilizzare le frecce.
//
procedure THexEdit.WMGetDlgCode(var Message: TWMGetDlgCode);
begin
  inherited;
  Message.Result := Message.Result or DLGC_WANTARROWS;
end;

function THexEdit.GetAddressSize: TAddressSize;
begin
  result := FAddressSize;
end;

procedure THexEdit.SetAddressSize(Size: TAddressSize);
begin
//MessageDlg('SetAddressSize', mtInformation, [mbOk], 0);
  FAddressSize := Size;
  case Size of
  as16bit:
    AddrPixSize := ColPixels * 5;
  as32bit:
    AddrPixSize := ColPixels * 9;
  end;
  Width := ComputeWidth;
//  Paint;
end;

function THexEdit.GetRecordSize: TRecordSize;
begin
  result := FRecordSize;
end;

procedure THexEdit.SetRecordSize(Size: TRecordSize);
begin
//MessageDlg('SetRecordSize', mtInformation, [mbOk], 0);
  FRecordSize := Size;
  case Size of
  rs8byte:
    begin
      RecPixSize := ColPixels * 24;
      AsciiPixSize := ColPixels * 8;
      RecSize := 8;
    end;
  rs16byte:
    begin
      RecPixSize := ColPixels * 48;
      AsciiPixSize := ColPixels * 16;
      RecSize := 16;
    end;
  end;
  Width := ComputeWidth;
  RecalcLimit;
  if InitDone then
  begin
    ComputeScrollMetrics;
    RecalcRows;
  end;
//  Paint;
end;

function THexEdit.GetRows: Integer;
begin
  result := FRows;
end;

procedure THexEdit.SetRows(Rows: Integer);
begin
//MessageDlg('SetRows', mtInformation, [mbOk], 0);
  if (Rows < MinRows) then
    Rows := MinRows;
  FRows := Rows;
  Height := FRows * RowPixels + DWord(ComputeBevelOffset);
  RecalcLimit;
  if InitDone then
  begin
    ComputeScrollMetrics;
    SetScrollPosition;
  end;
//  Paint;
end;

function THexEdit.GetColor: TColor;
begin
  result := Canvas.Brush.Color;
end;

procedure THexEdit.SetColor(NewColor: TColor);
begin
  Canvas.Brush.Color := NewColor;
  Paint;
end;

function THexEdit.GetTextColor: TColor;
begin
  result := Canvas.Font.Color;
end;

procedure THexEdit.SetTextColor(NewColor: TColor);
begin
  Canvas.Font.Color := NewColor;
  Paint;
end;

function THexEdit.GetBufferSize: DWord;
begin
  result := Length(Buffer);
end;

procedure THexEdit.SetBufferSize(Size: DWord);
begin
//MessageDlg('SetBufferSize', mtInformation, [mbOk], 0);
  SetLength(Buffer, Size);
  SetLength(Backup, Size);
  RecalcLimit;
  RecalcRows;
  ComputeScrollMetrics;
  if InitDone then
  begin
    ComputeScrollMetrics;
    RecalcRows;
  end;
  if InitDone or (csDesigning in ComponentState) then
    Paint;
  Modified := True;
end;

//procedure THexEdit.SetBorderStyle(Style: TBorderStyle);
//begin
//  FBorderStyle := Style;
//  BorderStyle := Style;
//  if Style = bsSingle then
//    BevelKind := bkTile
//  else
//    BevelKind := bkNone;
//end;

function THexEdit.ComputeBevelOffset:Integer;
begin
//  if BorderStyle = bsNone then
    result := 0
//  else
//    result := BevelWidth * 4;
end;

function THexEdit.ComputeWidth: Integer;
begin
  result := AddrPixSize + RecPixSize + AsciiPixSize + Scroll.Width + 2 + ComputeBevelOffset;
end;

procedure THexEdit.ComputeScrollMetrics;
var
  i: Integer;
begin
  i := TotalRows - FRows;
  if i < 0 then
    i := 0;
  Scroll.Max := i;
  Scroll.LargeChange := FRows;
//  Scroll.PageSize := FRows;
end;

procedure THexEdit.SetScrollPosition;
begin
  Scroll.Top := 0;
  Scroll.Left := ClientWidth - Scroll.Width;
  Scroll.Height := ClientHeight;
end;

procedure THexEdit.RecalcLimit;
begin
  if (DWord(Length(Buffer)) - Offset) < (FRows * RecSize) then
    Limit := DWord(Length(Buffer)) - Offset
  else
    Limit := FRows * RecSize;
  if Cursor >= Limit then
    Cursor := Limit - 1;
end;

procedure THexEdit.RecalcRows;
begin
  TotalRows := DWord(Length(Buffer)) div RecSize;
  if (DWord(Length(Buffer)) mod RecSize) > 0 then
    TotalRows := TotalRows + 1;
end;

procedure THexEdit.CursorCoords(out c, r: Integer);
begin
  c := Cursor mod RecSize;
  r := Cursor div RecSize;
end;

procedure THexEdit.CurOff;
var
  r, c, x, y: Integer;
begin
  with Canvas do
  begin
    Pen.Color := Canvas.Brush.Color;
    if CurMode = cmHex then
    begin
      CursorCoords(c, r);
      x := AddrPixSize + c * ColPixels * 3;
      y := r * RowPixels;
      MoveTo(x, y + CursorRow);
      LineTo(x + ColPixels * 2, y + CursorRow);
    end
    else
    begin
      CursorCoords(c, r);
      x := AddrPixSize + RecPixSize + c * ColPixels;
      y := r * RowPixels;
      MoveTo(x, y + CursorRow);
      LineTo(x + ColPixels, y + CursorRow);
    end;
  end;
end;

procedure THexEdit.CurOn;
var
  r, c, x, y: Integer;
begin
  with Canvas do
  begin
    Pen.Color := Canvas.Font.Color;
    if CurMode = cmHex then
    begin
      CursorCoords(c, r);
      x := AddrPixSize + c * ColPixels * 3;
      y := r * RowPixels;
      MoveTo(x, y + CursorRow);
      LineTo(x + ColPixels * 2, y + CursorRow);
    end
    else
    begin
      CursorCoords(c, r);
      x := AddrPixSize + RecPixSize + c * ColPixels;
      y := r * RowPixels;
      MoveTo(x, y + CursorRow);
      LineTo(x + ColPixels, y + CursorRow);
    end;
  end;
end;

procedure THexEdit.OutByte(Pos: Dword);
var
  c, r, x, y: Integer;
  s: String[1];
begin
  if Pos >= (RecSize * FRows) then
    Exit;
  c := Pos mod RecSize;
  r := Pos div RecSize;
  if Pos >= Limit then
  begin
    Canvas.Font.Style := [];
    x := AddrPixSize + c * ColPixels * 3;
    y := r * RowPixels;
    Canvas.TextOut(x, y, '   ');
    x := AddrPixSize + RecPixSize + c * ColPixels;
    y := r * RowPixels;
    Canvas.TextOut(x, y, ' ');
    Exit;
  end;
  if Buffer[Offset + Pos] <> Backup[Offset + Pos] then
    Canvas.Font.Style := [fsBold]
  else
    Canvas.Font.Style := [];
  x := AddrPixSize + c * ColPixels * 3;
  y := r * RowPixels;
  Canvas.TextOut(x, y, Format('%.2x ', [Buffer[Offset + Pos]]));
  x := AddrPixSize + RecPixSize + c * ColPixels;
  y := r * RowPixels;
  if (Buffer[Offset + Pos] < $20) or (Buffer[Offset + Pos] >= $7f) then
    s := '.'
  else
    s := Chr(Buffer[Offset + Pos]);
  Canvas.TextOut(x, y, Format('%s', [s]));
end;

procedure THexEdit.CurUp;
begin
  if (Cursor - RecSize) > Limit then
  begin
    ScrollLineUp;
    Exit;
  end;
  CurOff;
  Cursor := Cursor - RecSize;
  CurOn;
end;

procedure THexEdit.CurDown;
begin
  if (Cursor + RecSize) >= Limit then
  begin
    ScrollLineDown;
    Exit;
  end;
  CurOff;
  Cursor := Cursor + RecSize;
  CurOn;
end;

procedure THexEdit.CurLeft;
begin
  if (Offset + Cursor) = 0 then
    Exit;
  if (Cursor - 1) >= Limit then
  begin
    Cursor := RecSize - 1;
    ScrollLineUp;
    Exit;
  end;
  CurOff;
  Cursor := Cursor - 1;
  CurOn;
end;

procedure THexEdit.CurRight;
begin
  if (Offset + Cursor + 1) >= DWord(Length(Buffer)) then
  begin
    CurOn;
    Exit;
  end;
  if (Cursor + 1) >= Limit then
  begin
    Cursor := 0;
    ScrollLineDown;
    Exit;
  end;
  CurOff;
  Cursor := Cursor + 1;
  CurOn;
end;

procedure THexEdit.ScrollLineDown;
begin
  if (RecSize * FRows) > Limit then
    Exit;  // Non faccio scrollare se l'ultima riga è parziale.
  if (Offset + RecSize * FRows) >= DWord(Length(Buffer)) then
    Exit;  // Controllo limite superiore.
  Offset := Offset + RecSize;
  RecalcLimit;
  Paint;
end;

procedure THexEdit.ScrollLineUp;
begin
  if Offset - RecSize > DWord(Length(Buffer)) then
    Exit;
  Offset := Offset - RecSize;
  RecalcLimit;
  Paint;
end;

procedure THexEdit.ScrollPageDown;
var
 ps: DWord;
begin
  ps := RecSize * FRows;
  if DWord(Length(Buffer)) < ps then
  begin
    Offset := 0;
    Exit;
  end;
  Offset := Offset + ps;
  if Offset >= (DWord(Length(Buffer)) - ps) then
  begin
    Offset := (DWord(Length(Buffer)) - ps + RecSize - 1) and not (RecSize - 1);
  end;
  RecalcLimit;
  Paint;
end;

procedure THexEdit.ScrollPageUp;
begin
  if (Offset - RecSize * FRows) >= DWord(Length(Buffer)) then
    Offset := 0
  else
    Offset := Offset - RecSize * FRows;
  RecalcLimit;
  Paint;
end;

procedure THexEdit.CanResizeEvent(Sender: TObject;
                                  var NewWidth, NewHeight: Integer;
                                  var Resize: Boolean);
begin
  NewWidth := ComputeWidth;
  if NewHeight < MinRows * RowPixels then
    NewHeight := MinRows * RowPixels;
  FRows := NewHeight div RowPixels;
  NewHeight := FRows * RowPixels + DWord(ComputeBevelOffset);
end;

function THexEdit.CanAutoSize(var NewWidth, NewHeight: Integer): Boolean;
begin
  result := false;
end;

procedure THexEdit.KeyDownEvent(Sender: TObject;
                                var Key: Word;
                                Shift: TShiftState);
begin
  case Key of
  VK_PRIOR: // Page up.
    begin
      if Shift = [] then
        ScrollPageUp;
    end;
  VK_NEXT:  // Page down.
    begin
      if Shift = [] then
        ScrollPageDown;
    end;
  VK_DOWN:
    begin
      if Shift = [] then
        CurDown
      else if shift = [ssCtrl] then
        ScrollLineDown;
    end;
  VK_UP:
    begin
      if Shift = [] then
        CurUp
      else if shift = [ssCtrl] then
        ScrollLineUp;
    end;
  VK_LEFT:
    begin
      if Shift = [] then
        CurLeft;
    end;
  VK_RIGHT:
    begin
      if Shift = [] then
        CurRight;
    end;
  VK_RETURN:
    begin
      if CurMode = cmHex then
        CurMode := cmAscii
      else
        CurMode := cmHex;
      Paint;
    end;
  Ord('Z'):
    begin;
      if Shift = [ssCtrl] then
      begin
        UndoCurrent;
      end;
    end;
  Ord('V'):
    begin;
      if Shift = [ssCtrl] then
      begin
        if Clipboard.HasFormat(CF_TEXT	) then
          Overwrite(Clipboard.AsText);
      end;
    end;
  end;
end;

procedure THexEdit.Overwrite(text: String);
var
  i, j: Integer;
  b: TDynamicByteArray;
begin
  if CurMode = cmHex then
  begin
    text := StringReplace(text, ' ', '', [rfReplaceAll]);
    text := StringReplace(text, #9, '', [rfReplaceAll]);
    text := StringReplace(text, #10, '', [rfReplaceAll]);
    text := StringReplace(text, #13, '', [rfReplaceAll]);
    if Length(text) = 0 then
      Exit;

    SetLength(b, 0);
    try
      b := HexStringToByteArray(text);
    except
      Exit;
    end;

    i := Offset + Cursor;
    j := 0;
    while (i < Length(Buffer)) and (j < Length(b)) do
    begin
      Buffer[i] := b[j];
      Inc(i);
      Inc(j);
    end;
  end
  else
  begin
    if Length(text) = 0 then
      Exit;
    i := Offset + Cursor;
    j := 1;
    while (i < Length(Buffer)) and (j <= Length(text)) do
    begin
      Buffer[i] := Ord(text[j]);
      Inc(i);
      Inc(j);
    end;
  end;
  Modified := True;
  Paint;
end;

procedure THexEdit.KeyPressEvent(Sender: TObject; var Key: Char);
begin
  if CurMode = cmHex then
  begin
    if Key = ' ' then
    begin
      CurRight;
      Exit;
    end;
    if not IsHexDigit(Key) then
      Exit;
    Buffer[Offset + Cursor] := (Buffer[Offset + Cursor] shl 4) or
                               HexValue(Key);
    OutByte(Cursor);
    CurOn;
  end
  else
  begin
    if (Key < Chr($20)) or (Key >= Chr($7f)) then
      Exit;
    Buffer[Offset + Cursor] := Ord(Key);
    OutByte(Cursor);
    CurRight;
  end;
  Modified := True;
end;

procedure THexEdit.MouseDownEvent(Sender: TObject;
                                  Button: TMouseButton;
                                  Shift: TShiftState;
                                  X, Y: Integer);
var
  cellx, celly, newpos: DWord;
begin
  SetFocus;
  if X < AddrPixSize then                 // Click su indirizzi.
    Exit;
  celly := Y div RowPixels;
  if X < (AddrPixSize + RecPixSize) then  // Click su campo dati hex.
  begin
    cellx := (X - AddrPixSize) div (ColPixels * 3);
    CurMode := cmHex;
  end
  else
  begin                                   // Click su Campo dati ascii.
    cellx := (X - AddrPixSize - RecPixSize) div ColPixels;
    CurMode := cmAscii;
  end;
  newpos := celly * RecSize + cellx;
  if newpos >= Limit then
    Exit;
  Cursor := newpos;
  Paint;
end;

procedure THexEdit.ScrollEvent(Sender: TObject; ScrollCode:
                               TScrollCode;
                               var ScrollPos: Integer);
begin
  case ScrollCode of
  scLineUp:
    ScrollLineUp;
  scLineDown:
    ScrollLineDown;
  scPageUp:
    ScrollPageUp;
  scPageDown:
    ScrollPageDown;
  scPosition:
    begin
      Offset := DWord(ScrollPos) * RecSize;
      RecalcLimit;
      Paint;
    end;
  scTrack:
    begin
      Offset := DWord(ScrollPos) * RecSize;
      RecalcLimit;
      Paint;
    end;
  end;
end;

procedure THexEdit.FocusOn(Sender: TObject);
begin
  CurOn;
end;

procedure THexEdit.FocusOff(Sender: TObject);
begin
  CurOff;
end;

constructor THexEdit.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  // Defaults scrollbar.
  Scroll := TScrollBar.Create(self);
  Scroll.Parent := self;
  Scroll.Kind := sbVertical;
  Scroll.TabStop := False;
  Scroll.Min := 0;
  Scroll.Max := 0;                // Modificato in ComputeScrollMetrics.
  Scroll.SmallChange := 1;
  Scroll.LargeChange := 1;        // Modificato in ComputeScrollMetrics.
  Scroll.OnScroll := ScrollEvent;
  if csDesigning in ComponentState then
  begin
    // Defaults creando il componente a design time.
    SetRows(5);
    SetAddressSize(as16bit);
    SetRecordSize(rs8byte);
    TabStop := True;
    BorderStyle := bsSingle;
    Width := ComputeWidth;
  end;
  // Intercettazione eventi.
//  OnCanResize := CanResizeEvent;
  OnKeyDown := KeyDownEvent;
  OnKeyPress := KeyPressEvent;
  OnEnter := FocusOn;
  OnExit := FocusOff;
  OnMouseDown := MouseDownEvent;
  // Property varie.
//  BevelInner := bvLowered;
  // Impostazione font.
  Canvas.Font.Name := 'Courier New';
  Canvas.Font.Size := 10;
  Offset := 0;
  Cursor := 0;
  CurMode := cmHex;
  // Inizializzazione buffers.
  SetLength(Buffer, 1);
  SetLength(Backup, 1);
  TotalRows := 1;
  Limit := 1;
end;

procedure THexEdit.Loaded;
begin
  inherited;
  InitDone := True;
  RecalcLimit;
  RecalcRows;
  ComputeScrollMetrics;
end;

destructor THexEdit.Destroy;
begin
  inherited;
end;

procedure THexEdit.Paint;
var
  i: DWord;
  s: String;
begin
  SetScrollPosition;
  Canvas.Font.Style := [];
  // Draw campo indirizzi.
  for i := 0 to Frows - 1 do
  begin
    if FAddressSize = as16bit then
      s := Format('%.4x ', [Offset + i * RecSize])
    else
      s := Format('%.8x ', [Offset + i * RecSize]);
    Canvas.TextOut(0, i * RowPixels, s);
  end;
  // Draw campo dati esadecimali e ascii.
  i := 0;
  while i < RecSize * FRows do
  begin
    OutByte(i);
    i := i + 1;
  end;
  // Cursore.
  if not (csDesigning in ComponentState) and Focused then
    CurOn;
end;

procedure THexEdit.SetBuffer(NewBuffer: TDynamicByteArray);
begin
//  Buffer := NewBuffer;
  CopyByteArray(NewBuffer, Buffer);
  CopyByteArray(NewBuffer, Backup);
  RecalcLimit;
  RecalcRows;
  ComputeScrollMetrics;
  Paint;
  Modified := False;
end;

function THexEdit.GetBuffer: TDynamicByteArray;
begin
  result := Buffer;
end;

procedure THexEdit.Commit;
begin
  CopyByteArray(Buffer, Backup);
  Paint;
  Modified := False;
end;

procedure THexEdit.Rollback;
begin
  CopyByteArray(Backup, Buffer);
  Paint;
  Modified := False;
end;

procedure THexEdit.UndoCurrent;
begin
  Buffer[Cursor] := Backup[Cursor];
//  OutByte(Cursor);
//  CurOn;
  Paint;
end;

end.
