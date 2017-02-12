unit ByteArrayUtils;

{$MODE Delphi}

interface

uses SysUtils;

type
  TDynamicByteArray = array of Byte;

function IsByteArrayFilled(const A: TDynamicByteArray; Filler: Byte): Boolean;
function CompareByteArray(const A, B: TDynamicByteArray): Boolean;
procedure CopyByteArray(const S: TDynamicByteArray; var D: TDynamicByteArray);
function ConcatByteArray(const A, B: TDynamicByteArray): TDynamicByteArray;
function ConvertStaticArray(S: array of Byte): TDynamicByteArray;
procedure FillByteArray(const A: TDynamicByteArray;
                        Filler: Byte); overload;
procedure FillByteArray(const A: TDynamicByteArray;
                        Filler: TDynamicByteArray); overload;
function ByteArrayToHexStr(const A: TDynamicByteArray;
                           const Separator: String): String;
function HexStringToByteArray(const HexString: String): TDynamicByteArray;
function ByteArrayToString(A: TDynamicByteArray): String;

implementation

uses
  Chars;

procedure CopyByteArray(const S: TDynamicByteArray; var D: TDynamicByteArray);
var
  i: Integer;
begin
  SetLength(D, Length(S));
  for i := 0 to Length(S) - 1 do
    D[i] := S[i];
end;

function ConcatByteArray(const A, B: TDynamicByteArray): TDynamicByteArray;
var
  i: Integer;
begin
  SetLength(result, Length(A) + Length(B));
  for i := 0 to Length(A) - 1 do
    result[i] := A[i];
  for i := 0 to Length(B) - 1 do
    result[i + Length(A)] := B[i];
end;

function ConvertStaticArray(S: array of Byte): TDynamicByteArray;
var
  i: Integer;
begin
  SetLength(result, Length(S));
  for i := Low(S) to High(S) do
    result[i - Low(S)] := S[i];
end;

procedure FillByteArray(const A: TDynamicByteArray; Filler: Byte);
var
  i: Integer;
begin
  for i := 0 to Length(A) - 1 do
    A[i] := Filler;
end;

procedure FillByteArray(const A: TDynamicByteArray;
                        Filler: TDynamicByteArray);
var
  i, l: Integer;
begin
  l := Length(Filler);
  for i := 0 to Length(A) - 1 do
    A[i] := Filler[i mod l];
end;

function CompareByteArray(const A, B: TDynamicByteArray): Boolean;
var
  i: Integer;
begin
  if Length(A) = Length(B) then
  begin
    for i := 0 to Length(A) - 1 do
      if A[i] <> B[i] then
      begin
        result := False;
        Exit;
      end;
    result := True;
  end
  else
    result := False;
end;

function IsByteArrayFilled(const A: TDynamicByteArray; Filler: Byte): Boolean;
var
  i: Integer;
begin
  for i := 0 to Length(A) - 1 do
    if A[i] <> Filler then
    begin
      result := False;
      Exit;
    end;
  result := True;
end;

function ByteArrayToHexStr(const A: TDynamicByteArray;
                           const Separator: String): String;
var
  i: Integer;
begin
  result := '';
  for i := 0 to Length(A) - 1 do
  begin
    result := result + Format('%.2x', [A[i]]);
    if i < Length(A) - 1 then
      result := result + Separator;
  end;
end;

function HexStringToByteArray(const HexString: String): TDynamicByteArray;
var
  i, j: Integer;
  c1, c2: Char;
begin
  i := 1;
  j := 0;
  SetLength(result, 0);
  while i < Length(HexString) do
  begin
    c1 := HexString[i];
    i := i + 1;
    if i <= Length(HexString) then
    begin
      c2 := HexString[i];
      i := i + 1;
    end
    else
      c2 := '0';
    if (not IsHexDigit(c1)) or (not IsHexDigit(c2)) then
      raise EConvertError.Create('Invalid hex digit');
    j := j + 1;
    SetLength(result, j);
    result[j - 1] := (HexValue(c1) shl 4) or HexValue(c2);
  end;
end;

function ByteArrayToString(A: TDynamicByteArray): String;
var
  i: Integer;
begin
  result := '';
  for i := 0 to Length(A) - 1 do
    if A[i] = 0 then
      Exit
    else
      result := result + Chr(A[i]);
end;

end.
