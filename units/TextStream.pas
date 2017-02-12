unit TextStream;

{$MODE Delphi}

interface

uses
  Classes, SysUtils;

type
  ECustomTextStreamError = class(EStreamError);
  ETimeoutTextStreamError = class(ECustomTextStreamError);
  EUnexpectedTextStreamError = class(ECustomTextStreamError);
  EParameterTextStreamError = class(ECustomTextStreamError);

  TTextOptions = set of (toHandleCR, toStripControls, toAppend);

  TTextStream = class(TObject)
  private
    Stream: TStream;
    Options: TTextOptions;
    InBuffer: array of Char;
    InBufferPos: Integer;
    InBufferPtr: Integer;
    InBufferSize: Integer;
    OutBuffer: array of Char;
    OutBufferPtr: Integer;
  public
    LineNumber: Integer;
    constructor Create(InStream: TStream; Flags: TTextOptions);
    destructor Destroy; override;
    function ReadLine(out Line: String): Boolean;
    procedure WriteLine(Line: String);
    procedure ReadText(Text: TStringList);
    procedure WriteText(Text: TStringList);
    procedure Flush;
    procedure Reset;
    procedure Clear;
    procedure SetReadBuffer(Size: Integer);
    procedure SetWriteBuffer(Size: Integer);
  end;

implementation

constructor TTextStream.Create(InStream: TStream; Flags: TTextOptions);
begin
  inherited Create;
  Stream := InStream;
  Options := Flags;
  SetLength(InBuffer, 4096);
  SetLength(OutBuffer, 4096);
end;

destructor TTextStream.Destroy;
begin
  try Flush; except end;
  Stream.Destroy;
  inherited;
end;

//
// Legge una linea di testo delimitata da LF nella stringa Line.
// Ritorna False se fine dello stream.
// Genera un EUnexpectedStreamError in caso di errore inaspettato dallo stream.
// NOTA: Se lo stream un è un TWinSocketStream, ritornare False vuol dire
//       Timeout.
// NOTA: Se ritorna False, Line può non essere vuota sono dati non delimitati
//       dall'LF prima dell'EOF o del Timeout.
// NOTA: Se specificato toAppend la linea letta dallo stream viene accodata
//       a "Line".
//
function TTextStream.ReadLine(out Line: String): Boolean;
var
  tmp: String[255];   // Buffer secondario, serve per ridurre le riallocazioni.
  c: Char;
  size: Integer;
begin
  if not (toAppend in Options) then
    Line := '';
  while True do
  begin
    if InBufferPtr >= InBufferSize then // Se il buffer è vuoto...
    begin
      InBufferPos := InBufferPos + InBufferSize;
      try Stream.Seek(InBufferPos, soFromBeginning); except end;
      InBufferPtr := 0;
      size := Stream.Read(InBuffer[0], Length(InBuffer));
      if size < 0 then
        raise EUnexpectedTextStreamError.Create(Format('Non-standard stream error, size = %d', [size]));
      InBufferSize := size;
      if InBufferSize = 0 then
      begin
        result := False;
        Exit;
      end
    end;
    tmp := '';
    while (InBufferPtr < InBufferSize) do
    begin
      c := InBuffer[InBufferPtr];
      InBufferPtr := InBufferPtr + 1;
      if c = #10 then
      begin
        Line := Line + tmp;
        LineNumber := LineNumber + 1;
        result := True;
        exit;
      end;
      if (toStripControls in Options) and (c < #32) and (c <> #9) then
        continue
      else if (toHandleCR in Options) and (c = #13) then
        continue;
      if Length(tmp) < 255 then
        tmp := tmp + c
      else
      begin
        Line := Line + tmp;   // Avviene una riallocazione ogni 255 caratteri.
        tmp := c;
      end;
    end;
    Line := Line + tmp;
  end;
end;

//
// Scrive una linea sullo stream (sempre append).
// Genera un ETimeoutStreamError in caso di timeout di scrittura.
// Genera un EUnexpectedStreamError in caso di errore inaspettato dallo stream.
//
procedure TTextStream.WriteLine(Line: String);
var
  i, len: Integer;
  eol: String[2];
begin
  if toHandleCR in Options then
    eol := #13#10
  else
    eol := #10;
  len := Length(Line);
  for i := 1 to len + Length(eol) do
  begin
    if OutBufferPtr >= Length(OutBuffer) then
    begin
      Flush;
      continue;
    end;
    if i > len then
      OutBuffer[OutBufferPtr] := eol[i - len]
    else
      OutBuffer[OutBufferPtr] := Line[i];
    OutBufferPtr := OutBufferPtr + 1;
  end;
end;

//
// Legge tutto uno stream in un uno TStringList.
// L'eventuale linea senza LF alla fine viene trattata come se l'avesse.
//
procedure TTextStream.ReadText(Text: TStringList);
var
  l: String;
begin
  while ReadLine(l) or (Length(l) > 0) do
    Text.Add(l);
end;

//
// Scrive uno TStringList nello stream.
//
procedure TTextStream.WriteText(Text: TStringList);
var
  i: Integer;
begin
  for i := 0 to Text.Count - 1 do
    WriteLine(Text.Strings[i]);
end;

//
// Forza l'aggiornamento del buffer di scrittura.
// Genera un ETimeoutStreamError in caso di timeout di scrittura.
// Genera un EUnexpectedStreamError in caso di errore inaspettato dallo stream.
//
procedure TTextStream.Flush;
var
  written, size: Integer;
begin
  try
    try Stream.Seek(0, soFromEnd); except end;
    written := 0;
    while written < OutBufferPtr do
    begin
      size := Stream.Write(OutBuffer[written], OutBufferPtr - written);
      if Size = 0 then
        raise ETimeoutTextStreamError.Create('Timeout error writing stream');
      if Size < 0 then
        raise EUnexpectedTextStreamError.Create(Format('Non-standard stream error, size = %d', [size]));
      written := written + size;
    end;
  finally
    OutBufferPtr := 0;
  end;
end;

//
// Si posiziona all'inizio dello stream (se lo stream supporta il seek).
// Comunque rimette LineCount a zero e invalida il buffer di lettura.
//
procedure TTextStream.Reset;
begin
  InBufferPos := 0;
  InBufferPtr := 0;
  InBufferSize := 0;
  LineNumber := 0;
end;

//
// Scarta il contenuto dei buffer e se lo stream è un file lo azzera.
//
procedure TTextStream.Clear;
begin
  Reset;
  try Stream.Size := 0; except end;
  OutBufferPtr := 0;
end;

procedure TTextStream.SetReadBuffer(Size: Integer);
begin
  if Size <= 0 then
    raise EParameterTextStreamError.Create('Invalid read buffer size');
  InBufferPos := 0;
  InBufferPtr := 0;
  InBufferSize := 0;
  SetLength(InBuffer, Size);
end;

procedure TTextStream.SetWriteBuffer(Size: Integer);
begin
  if Size <= 0 then
    raise EParameterTextStreamError.Create('Invalid write buffer size');
  Flush;
  SetLength(OutBuffer, Size);
end;

end.
