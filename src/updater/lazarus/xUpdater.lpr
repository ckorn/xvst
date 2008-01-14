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
  // about...
  WriteLn('Xesc & Technology (R) Updater Utility Version 1.0.0.5');
  WriteLn('Copyright (C) Xesc & Technology Corporation. All rights reserved.');
  // start updater
  if FileExists(ParamStr(1)) then
    with TxUpdater.Create(ParamStr(1)) do
      try
        WriteLn('Executing install script...');
        Start;
      finally
        Free;
        WriteLn('Install Script executed...');
      end
  else
    WriteLn('Install script file do not exists: "' + ParamStr(1) + '"');
end.

