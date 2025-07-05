@echo off

call Build\FindBash.cmd

BASH% -c "./CloneRepositories.sh --clone-configure --clone-dependencies --clone-optional --imagemagick6"