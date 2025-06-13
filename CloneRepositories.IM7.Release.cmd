@echo off

call Build\FindBash.cmd

BASH% -c "./CloneRepositories.sh --imagemagick7"