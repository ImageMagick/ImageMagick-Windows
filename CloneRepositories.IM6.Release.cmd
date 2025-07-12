@echo off

if "%~1"=="" (
    echo Error: Missing required argument
    echo Usage: %~nx0 ^<dependencies-artifact^>
    exit /b 1
)

call Build\FindBash.cmd

%BASH% -c "./CloneRepositories.sh --imagemagick6 --dependencies-artifact %1"