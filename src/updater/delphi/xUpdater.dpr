program xUpdater;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  xUpdaterClass in '..\src\xUpdaterClass.pas',
  ToolsUnit in '..\src\ToolsUnit.pas';

{$R ..\res\xUpdater_Icon.res}
{$R ..\res\xUpdater_VersionInfo.res}

begin
  if FileExists(ParamStr(1)) then
    with TxUpdater.Create(ParamStr(1)) do
      try
        Start;
      finally
        Free;
      end;
end.
