@echo off

cd ..
call CloneRepositories.cmd https://github.com/ImageMagick shallow

set INNOSETUP=C:\InnoSetup
if exist "C:\InnoSetup" goto strawberry

mkdir %INNOSETUP%
cd %INNOSETUP%
appveyor DownloadFile https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/innosetup-5.5.9-unicode.exe
cd ..

:strawberry

set STRAWBERRYX86=C:\Strawberry-x86
if exist %STRAWBERRYX86% goto x64

mkdir %STRAWBERRYX86%
cd %STRAWBERRYX86%
appveyor DownloadFile https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/strawberry-perl-5.20.1.1-32bit.msi
cd ..

:x64

set STRAWBERRY64=C:\Strawberry-x64
if exist %STRAWBERRY64% goto install

mkdir %STRAWBERRY64%
cd %STRAWBERRY64%
appveyor DownloadFile https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/strawberry-perl-5.20.1.1-64bit.msi
cd ..

:install

powershell -ExecutionPolicy Unrestricted .\Install-InnoSetup.ps1
if "%1" == "x86" powershell -ExecutionPolicy Unrestricted .\Install-StrawberryPerl-x86.ps1
if "%1" == "x64" powershell -ExecutionPolicy Unrestricted .\Install-StrawberryPerl-x64.ps1