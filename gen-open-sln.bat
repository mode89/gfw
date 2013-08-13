@echo off

rem Make solution directory

set START_DIR=%CD%

if not exist build ( mkdir build )

rem Generate solution

cd build

cmake ..
if %errorlevel% neq 0 (
    echo Cannot generate soltion
    pause
    goto end
)

start MASTER.sln

:end

cd %START_DIR%
