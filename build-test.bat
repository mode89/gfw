@echo off

rem Setup Visual Studio environment

for /F "delims=," %%F in ('dir /S /B C:\*vcvarsall.bat') do set VCVARSALL_BAT=%%F
call "%VCVARSALL_BAT%"

rem Make build directory

if not exist build-test ( mkdir build-test )
cd build-test

rem Build release version with MinGW

if not exist mingw-release ( mkdir mingw-release )
cd mingw-release

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../..
mingw32-make

cd ..

rem Build release version with Visual Studio

if not exist msvc-release ( mkdir msvc-release )
cd msvc-release

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ../..
nmake

pause
