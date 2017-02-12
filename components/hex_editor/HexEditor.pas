{ This file was automatically created by Lazarus. Do not edit!
  This source is only used to compile and install the package.
 }

unit HexEditor;

interface

uses
  HexEdit, LazarusPackageIntf;

implementation

procedure Register;
begin
  RegisterUnit('HexEdit', @HexEdit.Register);
end;

initialization
  RegisterPackage('HexEditor', @Register);
end.
