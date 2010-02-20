echo xVideoServiceThief VC++ automated compiler script
echo Note: Configure with your envirotment variables before execute this Bat
echo Copyright Xesc and Technolgy 2010

set QtVersion=4.6.2
set QtDir=C:\ProgramacioLibs\Qt\%QtVersion%-VC++
set VCDir=C:\Archivos de programa\Microsoft Visual Studio 9.0
set UPX=C:\Documents and Settings\Administrador\Escritorio\upx.exe
set MAKE=nmake

call "%VCDir%\VC\vcvarsall.bat" x86