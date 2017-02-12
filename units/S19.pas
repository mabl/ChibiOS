unit S19;

{$MODE Delphi}

interface

uses
  SysUtils, Classes, LineParser, ByteArrayUtils, Chars;
  
type
  EParseS19 = class(EParse);
  EProduceS19 = class(EParse);
  ES19BufferOverflow = class(EParse);

  TGenericS19Record = class(TObject);

  TDataS19Record = class(TGenericS19Record)
  public
    StartAddress: Word;
    RecordData: TDynamicByteArray;
    constructor Create(Address: Word; Data: TDynamicByteArray);
  end;

  TTerminationS19Record = class(TGenericS19Record)
  public
    StartAddress: Word;
    constructor Create(Address: Word);
  end;

  TS19 = class(TObject)
  public
    class function ParseRecord(Text: String): TGenericS19Record;
    class function ProduceRecord(Address: Word; A: TDynamicByteArray): String;
    class procedure Load(Buffer: TDynamicByteArray;
                         Filler: Byte;
                         S19: TStrings);
    class procedure Save(Buffer: TDynamicByteArray;
                         Filler: Byte;
                         S19: TStrings);
  end;

implementation

constructor TDataS19Record.Create(Address: Word; Data: TDynamicByteArray);
begin
  StartAddress := Address;
  RecordData := Data;
end;

constructor TTerminationS19Record.Create(Address: Word);
begin
  StartAddress := Address;
end;

//
// Parserizza una stringa e ritorna un oggetto rappresentante lo S-Record
// rilevato.
// Se la stringa in input non è un S-Record valido allora viene generata
// un eccezione EParseS19.
class function TS19.ParseRecord(Text: String): TGenericS19Record;
var
  l: TLineParser;
  b1, b2, chk: Byte;
  size, i: Integer;
  a: TDynamicByteArray;

  function GetByte(L: TLineParser): Byte;
  var
    c1, c2: Char;
  begin
    c1 := L.Current;
    L.Next;
    c2 := L.Current;
    L.Next;
    if not IsHexDigit(c1) or not IsHexDigit(c2) then
      raise EParseS19.Create('Invalid hex data');
    result := (HexValue(c1) shl 4) or HexValue(c2);
  end;

begin
  result := nil;  // Sopprime un warning.
  l := TLineParser.Create;
  try
    l.SetLine(Text);
    l.MatchFor('S');
    case l.Current of
    '1':
      begin
        l.Next;
        size := GetByte(l);
        if (size < 3) or (size > 19) then
          raise EParseS19.Create('Invalid S1 record size');
        SetLength(a, size - 3);
        b1 := GetByte(l);
        b2 := GetByte(l);
        chk := size + b1 + b2;
        for i := 0 to size - 4 do
        begin
          a[i] := GetByte(l);
          chk := chk + a[i];
        end;
        if GetByte(l) <> not chk then
          raise EParseS19.Create('Invalid S9 record checksum');
        result := TDataS19Record.Create((Word(b1) shl 8) or b2, a);
      end;
    '9':
      begin
        l.Next;
        if GetByte(l) <> 3 then
          raise EParseS19.Create('Invalid S9 record size');
        b1 := GetByte(l);
        b2 := GetByte(l);
        chk := GetByte(l);
        if chk <> not Byte(b1 + b2 + 3) then
          raise EParseS19.Create('Invalid S9 record checksum');
        result := TTerminationS19Record.Create((Word(b1) shl 8) or b2);
      end;
    else
      raise EParseS19.Create('Unsupported S-Record');
    end;
  finally
    l.Free;
  end;
end;

//
// Produce un record S1 rappresentante il buffer passato.
// In caso d'errore genera un eccezione EProduceS19.
class function TS19.ProduceRecord(Address: Word; A: TDynamicByteArray): String;
var
  size, chk: Byte;

  function CheckSum(A: TDynamicByteArray): Integer;
  var
    i: Integer;
  begin
    result := 0;
    for i := 0 to Length(A) - 1 do
      result := result + A[i];
  end;

begin
  if Length(A) > 16 then
    raise EProduceS19.Create('S-Record size must be <= 16');
  size := Length(A) + 3;
  chk := not (size + (Address shr 8) + Address + CheckSum(A));
  result := 'S1' + Format('%.2x%.4x', [size, Address]) +
            ByteArrayToHexStr(A, '') + Format('%.2x', [chk]);
end;

//
// Carica un file S19 da un TString in un array binario.
// L'array viene preventivamente inizializzato a "Filler".
// Se il file eccede la dimensione del buffer il processo viene interrotto
// e viene generata un eccezione ES19BufferOverflow ma il caricamento fino
// a quel punto è stato completato correttamente.
class procedure TS19.Load(Buffer: TDynamicByteArray;
                          Filler: Byte;
                          S19: TStrings);
var
  i, j: Integer;
  r: TGenericS19Record;
begin
  FillByteArray(Buffer, Filler);
  for i := 0 to S19.Count - 1 do
  begin
    r := TS19.ParseRecord(S19[i]);
    if r is TTerminationS19Record then
      Exit;
    // Scrivo nel buffer.
    for j := 0 to Length(TDataS19Record(r).RecordData) - 1 do
    begin
      if (TDataS19Record(r).StartAddress + j) >= Length(Buffer) then
        raise ES19BufferOverflow.Create('Buffer overflow reading S19');
      Buffer[TDataS19Record(r).StartAddress + j] :=
                                 TDataS19Record(r).RecordData[j];
    end;
  end;
end;

//
// Salva un array binario in un TString rappresentante un file S19.
// Ottimizza il file in modo da non produrre record per vaste zone piene di
// "Filler".
class procedure TS19.Save(Buffer: TDynamicByteArray;
                          Filler: Byte;
                          S19: TStrings);
var
  start, size: Integer;
begin
  start := 0;
  while start < Length(Buffer) do
  begin
    if Buffer[start] = Filler then  // Cerco prima locazione <> filler.
    begin
      start := start + 1;
      Continue;
    end;
    size := (start or 15) + 1 - start;
    if start + size > Length(Buffer) then
      size := Length(Buffer) - start;
    while Buffer[start + size - 1] = Filler do
      size := size - 1;
    S19.Add(ProduceRecord(start, Copy(Buffer, start, size)));
    start := (start or 15) + 1;
  end;
  S19.Add('S9030000FC');
end;

end.
