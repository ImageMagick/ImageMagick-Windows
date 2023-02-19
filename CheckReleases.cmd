@echo off

set REPOS="https://github.com/ImageMagick"
if not "%1"=="" set REPOS=%1

set TYPE="full"
if not "%2"=="" set TYPE=%2

set BASH="%PROGRAMFILES%\Git\bin\bash.exe"
if exist %BASH% goto EXECUTE

set BASH="%PROGRAMFILES(x86)%\Git\bin\bash.exe"
if exist %BASH% goto EXECUTE

set BASH="%ProgramW6432%\Git\bin\bash.exe"
if exist %BASH% goto EXECUTE

set BASH="%USERPROFILE%\scoop\apps\git\current\bin\bash.exe"
if exist %BASH% goto EXECUTE

for /F "tokens=*" %%g in ('where bash') do (SET BASH=%%g)
if exist %BASH% goto EXECUTE

echo Failed to find bash.exe
echo %BASH%
exit /b 1

:EXECUTE
%BASH% --login -i -c "./CheckReleases.sh %REPOS% %TYPE%"
