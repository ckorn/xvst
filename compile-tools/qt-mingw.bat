echo xVideoServiceThief MinGW automated compiler script
echo Note: Configure your envirotment variables before execute this Bat
echo Copyright Xesc and Technolgy 2010

set QtVersion=4.6.0
set QtDir=C:\ProgramacioLibs\Qt\%QtVersion%\
set UPX=C:\Documents and Settings\Administrador\Escritorio\upx.exe
set MAKE=mingw32-make.exe

call "%QtDir%\bin\qtenv.bat"