@echo off

call compile-tools/qt-vc++.bat

"%QtDir%\bin\qmake" -set build_mode static_build
"%QtDir%\bin\qmake" -spec win32-msvc2008 -tp vc