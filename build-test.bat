@echo off
set START_DIR=%CD%

rem Make build directory

if not exist build-test ( mkdir build-test )
cd build-test

rem Build release version with MinGW

if "%MINGW_HOME%" == "" (
    echo Unable to find MinGW installation path. Please, define MINGW_HOME
    goto build_vc
)

if not exist mingw-release (
    mkdir mingw-release
)

cd mingw-release

set PATH=%PATH%;%MINGW_HOME%\bin
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../..
mingw32-make

cd ..

:build-vc

rem Setup Visual Studio environment

if not exist vcvarsall.bat (
    for /F "delims=," %%F in ('dir /S /B C:\*vcvarsall.bat') do ( set VCVARSALL_BAT=%%F )
)

if not exist vcvarsall.bat (
    @if not "%VCVARSALL_BAT%"=="" (
        echo call "%VCVARSALL_BAT%" > vcvarsall.bat
    ) else (
        echo Failed to locate vcvarsval.bat
        goto end
    )
)

call vcvarsall.bat

rem Build release version with Visual Studio

if not exist msvc-release ( mkdir msvc-release )
cd msvc-release

cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ../..
nmake

:end
cd %START_DIR%

pause
