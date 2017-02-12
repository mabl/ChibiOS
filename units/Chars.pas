unit Chars;

{$MODE Delphi}

interface

function IsSpace(c: Char): Boolean;
function IsDigit(c: Char): Boolean;
function IsHexDigit(c: Char): Boolean;
function IsLetter(c: Char): Boolean;
function IsLetterOrDigit(c: Char): Boolean;
function HexValue(c: Char): Integer;
function DecValue(c: Char): Integer;
function LoCase(c: Char): Char;

implementation

function IsSpace(c: Char): Boolean;
begin
  if c <= ' ' then
    result := True
  else
    result := False;
end;

function IsDigit(c: Char): Boolean;
begin
  if (c >= '0') and (c <= '9') then
    result := True
  else
    result := False;
end;

function IsHexDigit(c: Char): Boolean;
begin
  if (c >= '0') and (c <= '9') or
     (c >= 'a') and (c <= 'f') or
     (c >= 'A') and (c <= 'F') then
    result := True
  else
    result := False;
end;

function IsLetter(c: Char): Boolean;
begin
  if (c >= 'a') and (c <= 'z') or
     (c >= 'A') and (c <= 'Z') then
    result := True
  else
    result := False;
end;

function IsLetterOrDigit(c: Char): Boolean;
begin
  if (c >= '0') and (c <= '9') or
     (c >= 'a') and (c <= 'z') or
     (c >= 'A') and (c <= 'Z') then
    result := True
  else
    result := False;
end;

function HexValue(c: Char): Integer;
begin
  if IsDigit(c) then
    result := Ord(c) - Ord('0')
  else if (c >= 'a') and (c <= 'f') then
    result := Ord(c) - Ord('a') + 10
  else if (c >= 'A') and (c <= 'F') then
    result := Ord(c) - Ord('A') + 10
  else
    result := 0;
end;

function DecValue(c: Char): Integer;
begin
  result := Ord(c) - Ord('0');
end;

function LoCase(c: Char): Char;
begin
  if (c >= 'A') and (c <= 'Z') then
    result := Chr(ord(c) + $20)
  else
    result := c;
end;

end.
