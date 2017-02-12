unit LineParser;

{$MODE Delphi}

interface

uses Classes, Sysutils, ByteArrayUtils;

type
  EParse = class(Exception);

  TBaseToken = class(TObject)
  public
    function Make: String; virtual; abstract;
  end;

  TLineParser = class(TObject)
  private
    Line: String;
    LineIndex: Integer;
  public
    constructor Create;
    procedure SetLine(l: string);
    function GetLine: string;
    procedure Next;
    procedure Prev;
    function Current: Char;
    function EndOfLine: Boolean;
    procedure SkipBlank;
    procedure MatchFor(c: Char);
    function EndOfTokens: Boolean;
    function EolOrReminder: Boolean;
    function GetToken: TBaseToken;
    function GetReminder: String;
    function GetInteger: Integer; overload;
    function GetInteger(var HexFlag: Boolean): Integer; overload;
    function GetIdentifier: String;
    function GetString: String;
    function GetCryptoBlock: TDynamicByteArray;
    function GetBinary: TDynamicByteArray;
    function GetSet: TStringList;
    property Position: Integer read LineIndex write LineIndex;
  end;

  TReminderToken = class(TBaseToken)
  public
    Value: String;
    constructor Create(v: String); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser): String;
    class function Produce(v: String): String;
  end;

  TIntegerToken = class(TBaseToken)
  public
    Value: Integer;
    HexFlag: Boolean;
    constructor Create(v: Integer; Flag: Boolean); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser; out HexFlag: Boolean): Integer;
    class function Produce(v: Integer; HexFlag: Boolean): String;
  end;

  TIdentifierToken = class(TBaseToken)
  public
    Value: String;
    constructor Create(v: String); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser): String;
    class function Produce(v: String): String;
  end;

  TStringToken = class(TBaseToken)
  public
    Value: String;
    constructor Create(v: String); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser): String;
    class function Produce(v: String): String;
  end;

  TCryptoBlockToken = class(TBaseToken)
  public
    Value: TDynamicByteArray;
    constructor Create(v: TDynamicByteArray); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser): TDynamicByteArray;
    class function Produce(v: TDynamicByteArray): String; overload;
    class function Produce(v: array of Byte): String; overload;
  end;

  TBinaryToken = class(TBaseToken)
  public
    Value: TDynamicByteArray;
    constructor Create(v: TDynamicByteArray); overload;
    constructor Create(Line: TLineParser); overload;
    function Make: String; override;
    class function Parse(Line: TLineParser): TDynamicByteArray;
    class function Produce(v: TDynamicByteArray): String; overload;
    class function Produce(v: array of Byte): String; overload;
  end;

  TSetToken = class(TBaseToken)
  public
    Value: TStringList;
    constructor Create(v: TStringList); overload;
    constructor Create(Line: TLineParser); overload;
    destructor Destroy; override;
    function Make: String; override;
    class function Parse(Line: TLineParser): TStringList;
    class function Produce(v: TStringList): String; overload;
  end;
  
implementation

uses Chars;

{ Metodi di TLineParser. }
constructor TLineParser.Create;
begin
  inherited;
  Line := '';
  LineIndex := 1;
end;

procedure TLineParser.SetLine(l: string);
begin
  Line := l;
  LineIndex := 1;
end;

function TLineParser.GetLine: string;
begin
  result := Line;
end;

procedure TLineParser.Next;
begin
  LineIndex := LineIndex + 1;
end;

procedure TLineParser.Prev;
begin
  if LineIndex > 1 then
    LineIndex := LineIndex - 1;
end;

function TLineParser.Current: Char;
begin
  result := Line[LineIndex];
end;

function TLineParser.EndOfLine: Boolean;
begin
  if (Length(Line) - LineIndex) < 0 then
    result := True
  else
    result := False;
end;

procedure TLineParser.SkipBlank;
begin
  while LineIndex <= Length(Line) do
  begin
    if not IsSpace(Line[LineIndex]) then
      Exit;
    LineIndex := LineIndex + 1;
  end;
  raise EParse.Create('NOT FOUND');
end;

procedure TLineParser.MatchFor(c: Char);
begin
  if LineIndex > Length(Line) then
    raise EParse.Create('EOL');
  if Line[LineIndex] <> c then
    raise EParse.Create('Mismatch');
  LineIndex := LineIndex + 1;
end;

function TLineParser.EndOfTokens: Boolean;
begin
  while LineIndex <= Length(Line) do
  begin
    if not IsSpace(Line[LineIndex]) then
    begin
      result := False;
      Exit;
    end;
    LineIndex := LineIndex + 1;
  end;
  result := True;
end;

function TLineParser.EolOrReminder: Boolean;
begin
  result := True;
  if EndOfTokens then
    Exit;
  try
    GetReminder;
  except
    result := False;
  end;
end;

function TLineParser.GetToken: TBaseToken;
begin
  SkipBlank;
  if Line[LineIndex] = '/' then
  begin
    result := TReminderToken.Create(self);
    Exit;
  end;
  if IsDigit(Line[LineIndex]) or
     (Line[LineIndex] = '-') or
     (Line[LineIndex] = '$') then
  begin
    result := TIntegerToken.Create(self);
    Exit;
  end;
  if IsLetter(Line[LineIndex]) then
  begin
    result := TIdentifierToken.Create(self);
    Exit;
  end;
  if Line[LineIndex] = '"' then
  begin
    result := TStringToken.Create(self);
    Exit;
  end;
  if Line[LineIndex] = '[' then
  begin
    result := TCryptoBlockToken.Create(self);
    Exit;
  end;
  if Line[LineIndex] = '<' then
  begin
    result := TBinaryToken.Create(self);
    Exit;
  end;
  if Line[LineIndex] = '(' then
  begin
    result := TSetToken.Create(self);
    Exit;
  end;
  raise EParse.Create('Token syntax');
end;

function TLineParser.GetReminder: String;
begin
  SkipBlank;
  result := TReminderToken.Parse(self);
end;

function TLineParser.GetInteger(var HexFlag: Boolean): Integer;
begin
  SkipBlank;
  result := TIntegerToken.Parse(self, HexFlag);
end;

function TLineParser.GetInteger: Integer;
var
  dummy: Boolean;
begin
  SkipBlank;
  result := TIntegerToken.Parse(self, dummy);
end;

function TLineParser.GetIdentifier: String;
begin
  SkipBlank;
  result := TIdentifierToken.Parse(self);
end;

function TLineParser.GetString: String;
begin
  SkipBlank;
  result := TStringToken.Parse(self);
end;

function TLineParser.GetCryptoBlock: TDynamicByteArray;
begin
  SkipBlank;
  result := TCryptoBlockToken.Parse(self);
end;

function TLineParser.GetBinary: TDynamicByteArray;
begin
  SkipBlank;
  result := TBinaryToken.Parse(self);
end;

function TLineParser.GetSet: TStringList;
begin
  SkipBlank;
  result := TSetToken.Parse(self);
end;

{ Metodi di TReminderToken. }
constructor TReminderToken.Create(v: String);
begin
  Value := Trim(v);
end;

constructor TReminderToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

function TReminderToken.Make: String;
begin
  result := Produce(Value);
end;

class function TReminderToken.Parse(Line: TLineParser): String;
begin
  Line.MatchFor('/');
  Line.MatchFor('/');
  result := '';
  while not Line.EndOfLine do
  begin
    result := result + Line.Current;
    Line.Next;
  end;
  result := Trim(result);
end;

class function TReminderToken.Produce(v: String): String;
begin
  result := '// ' + v;
end;

{ Metodi di TIntegerToken. }
constructor TIntegerToken.Create(v: Integer; Flag: Boolean);
begin
  inherited Create;
  Value := v;
  HexFlag := Flag;
end;

constructor TIntegerToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line, HexFlag);
end;

function TIntegerToken.Make: String;
begin
 result := Produce(Value, HexFlag);
end;

class function TIntegerToken.Parse(Line: TLineParser; out HexFlag: Boolean): Integer;
var
  nf: Boolean;
begin
  result := 0;
  if Line.Current = '$' then
  begin
    Line.Next;
    if not IsHexDigit(Line.Current) then
      raise EParse.Create('Invalid hex constant');
    while not Line.EndOfLine and IsHexDigit(Line.Current) do
    begin
      result := result * 16 + HexValue(Line.Current);
      Line.Next;
    end;
    HexFlag := True;
    Exit;
  end;
  if Line.Current = '-' then
  begin
    Line.Next;
    nf := True;
  end
  else
    nf := False;
  if not IsDigit(Line.Current) then
    raise EParse.Create('Invalide decimal constant');
  while not Line.EndOfLine and IsDigit(Line.Current) do
  begin
    result := result * 10 + DecValue(Line.Current);
    Line.Next;
  end;
  if nf then
    result := -result;
  HexFlag := False;
end;

class function TIntegerToken.Produce(v: Integer; HexFlag: Boolean): String;
begin
  if HexFlag then
    result := Format('$%x', [v])
  else
    result := IntToStr(v);
end;

{ Metodi di TIdentifierToken. }
constructor TIdentifierToken.Create(v: String);
var
  i: Integer;
begin
  inherited Create;
  { Controllo che sia un identificatore valido. }
  if (Length(v) = 0) or not IsLetter(v[1]) then
    raise EParse.Create('Invalid identifier');
  for i := 2 to Length(v) do
    if not IsLetterOrDigit(v[i]) and (v[i] <> '_') then
      raise EParse.Create('Invalid identifier');
  Value := v;
end;

constructor TIdentifierToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

function TIdentifierToken.Make: String;
begin
  result := Value;
end;

class function TIdentifierToken.Parse(Line: TLineParser): String;
begin
  if not IsLetter(Line.Current) then
    raise EParse.Create('Invalid identifier');
  result := Line.Current;
  Line.Next;
  while not Line.EndOfLine and
       (IsLetterOrDigit(Line.Current) or (Line.Current = '_')) do
  begin
    result := result + Line.Current;
    Line.Next;
  end;
end;

class function TIdentifierToken.Produce(v: String): String;
begin
  result := v;
end;

{ Metodi di TStringToken. }
constructor TStringToken.Create(v: String);
begin
  inherited Create;
  Value := v;
end;

constructor TStringToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

function TStringToken.Make: String;
begin
  result := Produce(Value);
end;

class function TStringToken.Parse(Line: TLineParser): String;
var
  c: Char;
begin
  Line.MatchFor('"');
  result := '';
  while True do
  begin
    c := Line.Current;
    if c = '"' then
    begin
      Line.Next;
      if Line.EndOfLine or (Line.Current <> '"') then
          Exit;
      result := result + '"';
      Line.Next;
      Continue;
    end;
    result := result + c;
    Line.Next;
  end;
end;

class function TStringToken.Produce(v: String): String;
var
  i: Integer;
begin
  result := '"';
  for i := 1 to Length(v) do
    if v[i] = '"' then
      result := result + '""'
    else
      result := result + v[i];
  result := result + '""'
end;

{ Metodi di TCryptoBlockToken. }
constructor TCryptoBlockToken.Create(v: TDynamicByteArray);
begin
  inherited Create;
  Value := @v;
end;

constructor TCryptoBlockToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

function TCryptoBlockToken.Make: String;
begin
  result := Produce(Value);
end;

class function TCryptoBlockToken.Parse(Line: TLineParser): TDynamicByteArray;
var
  temp: TDynamicByteArray;
  i, j, sz: Integer;
  c1, c2: Char;
  dummy: Boolean;
begin
  Line.MatchFor('[');
  sz := TIntegerToken.Parse(Line, dummy);
  SetLength(result, sz);
  SetLength(temp, (sz + 15) and not 15);
  Line.MatchFor(':');
  i := 0;
  while True do
  begin
    c1 := Line.Current;
    Line.Next;
    if c1 = ']' then
      break;
    c2 := Line.Current;
    Line.Next;
    if not IsHexDigit(c1) or not IsHexDigit(c2) then
      raise EParse.Create('Invalid data field');
    temp[i] := (HexValue(c1) shl 4) or HexValue(c2);
    i := i + 1;
  end;
  { ***Qui inserire decifratura di temp*** }
  for j := 0 to sz - 1 do
    result[j] := temp[j];
end;

class function TCryptoBlockToken.Produce(v: TDynamicByteArray): String;
var
  i: Integer;
  temp: TDynamicByteArray;
  hex: String;
begin
  SetLength(temp, (Length(v) + 15) and not 15);
  for i := 0 to Length(temp) - 1 do
  begin
    if i < Length(v) then
      temp[i] := v[i]
    else
      temp[i] := 0;
  end;
  { ***Qui inserire cifratura di temp*** }
  hex := '';
  for i := 0 to Length(temp) - 1 do
    hex := hex + Format('%1x%1x', [(temp[i] shr 4) and 15, temp[i] and 15]);
  result := '[' + IntToStr(Length(v)) + ':' + hex + ']';
end;

class function TCryptoBlockToken.Produce(v: array of Byte): String;
var
  i: Integer;
  temp: TDynamicByteArray;
  hex: String;
begin
  SetLength(temp, (High(v) + 15) and not 15);
  for i := 0 to Length(temp) - 1 do
  begin
    if i < Length(v) then
      temp[i] := v[i]
    else
      temp[i] := 0;
  end;
  { ***Qui inserire cifratura di temp*** }
  hex := '';
  for i := 0 to Length(temp) - 1 do
    hex := hex + Format('%1x%1x', [(temp[i] shr 4) and 15, temp[i] and 15]);
  result := '[' + IntToStr(Length(v)) + ':' + hex + ']';
end;

{ Metodi di TBinaryToken. }
constructor TBinaryToken.Create(v: TDynamicByteArray);
begin
  inherited Create;
  Value := @v;
end;

constructor TBinaryToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

function TBinaryToken.Make: String;
begin
  result := Produce(Value);
end;

class function TBinaryToken.Parse(Line: TLineParser): TDynamicByteArray;
var
  i, p: Integer;
  c1, c2: Char;
begin
  Line.MatchFor('<');
  p := Line.Position;
  // Controllo la sintassi del campo e ne calcolo la lunghezza.
  i := 0;
  while True do
  begin
    c1 := Line.Current;
    Line.Next;
    if c1 = '>' then
      break;
    c2 := Line.Current;
    Line.Next;
    if not IsHexDigit(c1) or not IsHexDigit(c2) then
      raise EParse.Create('Invalid binary field');
    i := i + 1;
  end;
  SetLength(result, i);
  Line.Position := p;
  i := 0;
  while True do
  begin
    c1 := Line.Current;
    Line.Next;
    if c1 = '>' then
      break;
    c2 := Line.Current;
    Line.Next;
    result[i] := (HexValue(c1) shl 4) or HexValue(c2);
    i := i + 1;
  end;
end;

class function TBinaryToken.Produce(v: TDynamicByteArray): String;
begin
  result := '<' + ByteArrayToHexStr(v, '') + '>';
end;

class function TBinaryToken.Produce(v: array of Byte): String;
var
  i: Integer;
  hex: String;
begin
  hex := '';
  for i := 0 to Length(v) - 1 do
    hex := hex + Format('%1x%1x', [v[i] shr 4, v[i] and 15]);
  result := '<' + hex + '>';
end;

{ Metodi di TSetToken. }
constructor TSetToken.Create(v: TStringList);
begin
  inherited Create;
  Value := v;  // Si dovrebbe controllare che siano tutti ID validi.
end;

constructor TSetToken.Create(Line: TLineParser);
begin
  inherited Create;
  Value := Parse(Line);
end;

destructor TSetToken.Destroy;
begin
  Value.Free;
  inherited;
end;

function TSetToken.Make: String;
begin
  result := Produce(Value);
end;

class function TSetToken.Parse(Line: TLineParser): TStringList;
begin
  result := TStringList.Create;
  try
    Line.MatchFor('(');
    Line.SkipBlank;
    if Line.Current = ')' then    // Caso particolare, set vuoto.
      Exit;
    while True do
    begin
      Line.SkipBlank;
      result.Add(TIdentifierToken.Parse(Line));
      Line.SkipBlank;
      if Line.Current <> ',' then
        Break;
      Line.Next;
    end;
    Line.MatchFor(')');
  except
    result.Free;
    raise;
  end;
end;

class function TSetToken.Produce(v: TStringList): String;
var
  i: Integer;
begin
  result := '(';
  i := 0;
  while i < v.Count do
  begin
    result := result + v[i];
    if i < v.Count - 1 then
      result := result + ', ';
    i := i + 1;
  end;
  result := result + ')';
end;

end.
