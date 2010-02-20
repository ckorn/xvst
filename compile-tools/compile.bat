@echo off

:qt-config
if "%1" == "vc++" (
  call compile-tools\qt-vc++.bat
  goto :pre-clean
)
if "%1" == "mingw" (
  call compile-tools\qt-mingw.bat
  goto :pre-clean
)

:config-missing
echo No configuration (VC++ or MinGW) specified
echo Example: "compile-tools/compile.bat" vc++ clean
goto :END

:pre-clean
if "%2" == "no-clean" goto :do-qmake

:Clean
RMDIR build /s /q
%MAKE% clean 
DEL Makefile
DEL Makefile.Release
DEL Makefile.Debug
DEL object_script.xVideoServiceThief.Release

:do-qmake
goto :do-qmake-%1%

:do-qmake-vc++
"%QtDir%\bin\qmake" -set build_mode static_build
"%QtDir%\bin\qmake" -spec win32-msvc2008 CONFIG+=release
goto :compile

:do-qmake-mingw
"%QtDir%\bin\qmake" -set build_mode dynamic_build
"%QtDir%\bin\qmake" CONFIG+=release
goto :compile

:compile
"%MAKE%"

:upx
"%UPX%" "bin\xVideoServiceThief.exe" -9

:Install languages
%QtDir%\bin\lrelease.exe xVideoServiceThief.pro
call ".\Installer\windows-install-languages.bat" "%CD%\Installer\"                                                                               

:Install plugins
call ".\Installer\windows-install-plugins.bat" "%CD%\Installer\"                                                                  

:END
pause