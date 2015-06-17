@echo off

set type="full"
if not "%1"=="" set type=%1

set bash="%PROGRAMFILES%\Git\bin\bash.exe"
if exist %bash% goto execute

set bash="%PROGRAMFILES(x86)%\Git\bin\bash.exe"
if exist %bash% goto execute

echo Failed to find bash.exe
echo %bash%
exit /b 1

:execute
%bash% --login -i -c "./CloneRepositories.sh %type%"
