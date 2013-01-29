@echo off

rem Make solution directory

set START_DIR=%CD%

if not exist build ( mkdir build )

rem Generate solution

cd build
if not exist GLOBAL_SOLUTION.sln (
    cmake ..
    @if not errorlevel=="" (
        echo Cannot generate soltion
        pause
        goto end
    )
)

start GLOBAL_SOLUTION.sln

:end

cd %START_DIR%
