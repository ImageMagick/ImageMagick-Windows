@echo off

cd ..
call CloneRepositories.cmd https://github.com/ImageMagick shallow

if not exist "C:\Program Files (x86)\Inno Setup 5" choco install innosetup -y -version 5.5.2
if %errorlevel% neq 0 exit /b %errorlevel%

set OPTION=
if "%1" == "x86" SET OPTION=-x86
choco install strawberryperl -y %OPTION% -version 5.20.1.1
if %errorlevel% neq 0 exit /b %errorlevel%
