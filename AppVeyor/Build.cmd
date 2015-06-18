@echo off
call "%vs120comntools%vsvars32.bat"

powershell -ExecutionPolicy Unrestricted .\Build.ps1 %1 %2
