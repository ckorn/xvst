program xUpdater;

{$mode objfpc}{$H+}

uses
  SysUtils,
  xUpdaterClass in '..\src\xUpdaterClass.pas',
  ToolsUnit in '..\src\ToolsUnit.pas';

{$IFDEF WIN32}
{$R ..\res\xUpdater_Icon.res}
{$R ..\res\xUpdater_VersionInfo.res}
{$ENDIF}

begin
  if FileExists(ParamStr(1)) then
    with TxUpdater.Create(ParamStr(1)) do
      try
        Start;
      finally
        Free;
      end;
end.

