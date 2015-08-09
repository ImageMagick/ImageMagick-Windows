@echo off

cd ..
call CloneRepositories.cmd https://github.com/ImageMagick shallow
if not exist "C:\Program Files (x86)\Inno Setup 5" choco install innosetup -y -version 5.5.2
if exist "C:\Strawberry-x86" goto done
choco install strawberryperl -y -x86 -version 5.20.1.1
move C:\Strawberry C:\Strawberry-x86
choco install strawberryperl -y -force -version 5.20.1.1
move C:\Strawberry C:\Strawberry-x64
:done