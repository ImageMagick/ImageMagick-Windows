@echo off

call Build\FindBash.cmd

BASH% -c "./CloneRepositories.sh --development --imagemagick6"