unit ParseTextFile;

{$MODE Delphi}

interface

uses classes, sysutils, LCLIntf, LCLType, LMessages;

const
  MaxRecordLen = 256;

type
  TTextFileParser = class(TObject)
  private
    Stream: THandleStream;
    buf: array [0..MaxRecordLen] of Char;
    fp: PChar;
    procedure SkipRecord;
    function GetField(maxsize: Cardinal): String;
  public
    CurrentRecord: Cardinal;
    constructor Create(const FileName: string; Flag: Boolean);
    destructor Destroy; override;
    procedure Rewind;
    function NextRecord: Boolean;
    procedure SkipBlanks;
    function FindFirstKey(key: String): Boolean;
    function FindNextKey(key: String): Boolean;
    procedure Rescan;
    function GetUnsignedField: Cardinal;
    function GetStringField(maxsize: Cardinal): String;
    function GetQuotedStringField(maxsize: Cardinal): String;
    procedure MatchFor(c: Char);
    function UngetChar: Char;
    function GetChar: Char;
    function CheckForEndOfRecord: Boolean;
    procedure CheckForNoMoreData;
    procedure ClearAll;
    procedure AppendRecord(Data: String);
  end;

  EParseError = class(Exception);
  ESysError = class(Exception);
  EFileNotFound = class(ESysError);
  ELockError = class(ESysError);
  ERecordSize = class(Exception);
  ELineTooLong = class(EParseError);
  EEndOfRecord = class(EParseError);
  ENoEndOfRecord = class(EParseError);
  ESyntax = class(EParseError);
  EFieldSyntax = class(EParseError);
  EFieldSize = class(EParseError);
  EFieldType = class(EParseError);

implementation

constructor TTextFileParser.Create(const FileName: string; Flag: Boolean);
var
  hFile: DWord;
begin
  inherited Create;
  if Flag then
    hFile := CreateFile(PChar(FileName), GENERIC_READ or GENERIC_WRITE,
                        FILE_SHARE_READ or FILE_SHARE_WRITE, nil,
                        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)
  else
    hFile := CreateFile(PChar(FileName), GENERIC_READ or GENERIC_WRITE,
                        FILE_SHARE_READ or FILE_SHARE_WRITE, nil,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if hFile = INVALID_HANDLE_VALUE then
    raise EFileNotFound.CreateFmt('Impossibile aprire %s.', [FileName]);
  while True do
  begin
    if not LockFile(hFile, 0, 0, $ffffffff, 0) then
    begin
      if GetLastError = ERROR_LOCK_VIOLATION then
      begin
        Sleep(300);
        continue;
      end;
      FileClose(hFile); { *Converted from CloseHandle* }
      raise EFileNotFound.CreateFmt('Errore di lock su %s.', [FileName]);
    end;
    break;
  end;
  Stream := THandleStream.Create(hFile);
  fp := @buf;
  fp^ := chr(0);
end;

destructor TTextFileParser.Destroy;
begin
  UnlockFile(Stream.Handle, 0, 0, $ffffffff, 0);
  FileClose(Stream.Handle); { *Converted from CloseHandle* }
  Stream.Destroy;
  inherited Destroy;
end;

procedure TTextFileParser.SkipRecord;
var
  tmp: array [0..1] of Char;
begin
  while (Stream.Read(tmp, 1) > 0) and (tmp[0] <> chr(10)) do
    ;
end;

function TTextFileParser.GetField(maxsize: Cardinal): String;
var
  buf: String;
  sz: Cardinal;
begin
  sz := 0;
  buf := '';
  while (fp^ <> ' ') and (fp^ <> chr(9)) and (fp^ <> chr(0)) do
  begin
    buf := buf + fp^;
    fp := fp + 1;
    sz := sz + 1;
    if sz > maxsize then
      raise EFieldSize.CreateFmt('Dimensione campo eccessiva a linea %d', [CurrentRecord]);
  end;
  GetField := buf;
end;

//
// Salta i blank fino al prossimo campo. Se viene incontrata la fine del record
// genera un eccezione EEndOfRecord.
//
procedure TTextFileParser.SkipBlanks;
begin
  while (fp^ = ' ') or (fp^ = chr(9)) do
    fp := fp + 1;
end;

//
// Ritorna True se incontra la fine del record.
//
function TTextFileParser.CheckForEndOfRecord: Boolean;
begin
  while (fp^ = ' ') or (fp^ = chr(9)) do
    fp := fp + 1;
  if fp^ = chr(0) then
    CheckForEndOfRecord := True
  else
    CheckForEndOfRecord := False;
end;

//
// Genera una eccezione ENoEndOfRecord se non incontra la fine del record.
//
procedure TTextFileParser.CheckForNoMoreData;
begin
  while (fp^ = ' ') or (fp^ = chr(9)) do
    fp := fp + 1;
  if fp^ <> chr(0) then
    raise ENoEndOfRecord.CreateFmt('Troppi campi in linea %d', [CurrentRecord]);
end;

//
// Controlla un carattere nel record di ingresso a partire dalla posizione
// corrente non ignorando i blank, se non trovato genera una eccezione ESyntax.
//
procedure TTextFileParser.MatchFor(c: Char);
begin
  if fp^ <> c then
    raise ESyntax.CreateFmt('Errore di sintassi in linea %d', [CurrentRecord]);
  if fp^ <> chr(0) then
    fp := fp + 1;
end;

//
// Ritorna il prossimo carattere ma non avanza.
//
function TTextFileParser.UngetChar: Char;
begin
  result := fp^;
end;

//
// Ritorna il prossimo carattere. Se viene incontrata la fine del record genera
// un eccezione EEndOfRecord.
//
function TTextFileParser.GetChar: Char;
begin
  if fp^ = chr(0) then
    raise EEndOfRecord.Create('Fine del record');
  result := fp^;
  fp := fp + 1;
end;

//
// Riposiziona all' inizio del file.
//
procedure TTextFileParser.Rewind;
begin
  Stream.Seek(0, soFromBeginning);
  fp := @buf;
  fp^ := chr(0);
end;

//
// Legge un record eliminando i commenti e ignorando i record vuoti.
// Ritorna True se incontra la fine del file.
//
function TTextFileParser.NextRecord: Boolean;
var
  p: PChar;
  n: Longint;
  tmp: array [0..1] of Char;
begin
  repeat
    p := @buf;
    repeat
      n := Stream.Read(tmp, 1);
      //
      // Condizione di fine file.
      //
      if (n = 0) and (p - @buf = 0) then
      begin
        NextRecord := True;
        Exit;
      end;
      //
      // Condizione di fine record.
      //
      if (n = 0) or (tmp[0] = chr(10)) then
        Break;
      //
      // Se ho incontrato un commento o un CR, ignoro il resto del record.
      //
      if (tmp[0] = '#') or (tmp[0] = chr(13)) then
      begin
        SkipRecord;
        Break;
      end;
      //
      // Condizione di trabocco buffer.
      //
      if p - @buf >= MaxRecordLen then
      begin
        p^ := chr(0);
        fp := @buf;
        SkipRecord;
        CurrentRecord := CurrentRecord + 1;
        raise ELineTooLong.CreateFmt('Dimensione record %d eccessiva, troncato', [CurrentRecord]);
      end;
      p^ := tmp[0];
      p := p + 1;
    until False;
    p^ := chr(0);
    fp := @buf;
    CurrentRecord := CurrentRecord + 1;
  until not CheckForEndOfRecord;
  NextRecord := False;
end;

//
// Cerca il primo record con il primo campo uguale (case insensitive) a "key",
// il campo viene 'consumato'.
// Ritorna True se non trovato.
//
function TTextFileParser.FindFirstKey(key: String): Boolean;
begin
  Rewind;
  result := FindNextKey(key);
end;

//
// Cerca il successivo record contenente "key" nel primo campo, il campo viene
// 'consumato'.
// Ritorna True se non trovato.
//
function TTextFileParser.FindNextKey(key: String): Boolean;
var
  kbuf: String;
begin
  key := UpperCase(key);
  while not Nextrecord do
  begin
    try
      SkipBlanks;
      kbuf := GetField(255);
    except
      continue;
    end;
    if UpperCase(kbuf) = key then
    begin
      result := False;
      Exit;
    end;
  end;
  result := True;
end;

//
// Ricomincia la scansione dei campi all'interno del record.
//
procedure TTextFileParser.Rescan;
begin
  fp := @buf;
end;

//
// Legge un campo delimitato da un non numerico e lo ritorna come intero
// senza segno.
//
function TTextFileParser.GetUnsignedField: Cardinal;
var
  n: Cardinal;
begin
  if (fp^ < '0') or (fp^ > '9') then
    raise EFieldType.CreateFmt('Tipo campo errato in linea %d', [CurrentRecord]);
  n := 0;
  while (fp^ >= '0') and (fp^ <= '9') do
  begin
    n := n * 10 + Ord(fp^) - Ord('0');
    fp := fp + 1;
  end;
  result := n;
end;

//
// Legge un campo terminato da un blank e lo ritorna come stringa.
//
function TTextFileParser.GetStringField(maxsize: Cardinal): String;
begin
  if maxsize > 255 then
    raise EFieldSize.Create('Dimensione campo stringa eccessiva');
  if (fp^ = ' ') or (fp = chr(9)) or (fp^ = chr(0)) then
    raise EFieldType.CreateFmt('Tipo campo errato in linea %d', [CurrentRecord]);
  GetStringField := GetField(maxsize);
end;

//
// Legge un campo delimitato da doppi apici e lo ritorna come stringa.
// I doppi apici non vengono ritornati.
//
function TTextFileParser.GetQuotedStringField(maxsize: Cardinal): String;
var
  fld: String[255];
  sz: Cardinal;
begin
  if maxsize > 255 then
    raise EFieldSize.Create('Dimensione campo stringa eccessiva');
  if fp^ <> '"' then
    raise EFieldType.CreateFmt('Tipo campo errato in linea %d', [CurrentRecord]);
  fp := fp + 1;
  fld := '';
  sz := 0;
  while (fp^ <> '"') and (fp^ <> chr(0)) do
  begin
    fld := fld + fp^;
    fp := fp + 1;
    sz := sz + 1;
    if sz > maxsize then
      raise EFieldSize.CreateFmt('Dimensione campo eccessiva a linea %d', [CurrentRecord]);
  end;
  if fp^ = chr(0) then
    raise EFieldSyntax.CreateFmt('Errore di sintassi in linea %d', [CurrentRecord]);
  fp := fp + 1;
  GetQuotedStringField := fld;
end;

procedure TTextFileParser.ClearAll;
begin
  SetFilePointer(Stream.Handle, 0, nil, FILE_BEGIN);
  SetEndOfFile(Stream.Handle);
  fp := @buf;
  fp^ := chr(0);
end;

procedure TTextFileParser.AppendRecord(Data: String);
var
  pos: LongInt;
  size: Integer;
  buf: array [0..256] of Char;
begin
  size := Length(Data);
  if size > 255 then
    raise ERecordSize.Create('Dimensione record eccessiva');
  pos := Stream.Position;
  Stream.Seek(0, soFromEnd);
  StrPCopy(buf, Data);
  try
    Stream.WriteBuffer(buf, size);
    Stream.WriteBuffer(#13#10, 2);
  except
    Stream.Seek(pos, soFromBeginning);
    raise;
  end;
  Stream.Seek(pos, soFromBeginning);
end;

end.

