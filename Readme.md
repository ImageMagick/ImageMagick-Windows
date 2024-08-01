# Building ImageMagick on Windows.

This document describes the requirements and instructions to build ImageMagick for Windows on your own machine.

### Requirements

- Visual Studio 2017 (or newer)
- Git for Windows
- AMD APP SDK (optional for OpenCL support)

### Install Visual Studio dependencies

To build ImageMagick with Visual Studio the following components should be installed:

- Desktop development with C++ (workload)
- Visual C++ ATL for x86 and x64
- Visual C++ MFC for x86 and x64
- Windows XP support for C++ (optional if the output needs to run on Windows XP)

Naming of these components might differ between versions of Visual Studio. The example above use the Visual Studio
2017 names.

### Clone the dependencies

The ImageMagick library is built with a number of third party libraries. Run `CloneRepositories.IM7.cmd` to clone
these libraries and the ImageMagick library. To clone the legacy ImageMagick 6 library and it's dependencies
run `CloneRepositories.IM6.cmd`.

### Build Configure.exe

One of the folders in this project is called `Configure`. This folder contains the solution file `Configure.sln`
for the latest 2022 version of Visual Studio, and `Configure.2017.sln` for older versions. Open one of those and
start a `Release` build of the project. This will create a file called `Configure.exe` in the folder. Running this
program will start a Wizard that allows configuration of ImageMagick and its individual components.

### Build ImageMagick

Depending on which options were chosen when running `Configure.exe` one of the following solutions will be created
in the root of the repository:

- IM7.Dynamic.sln (Dynamic Multi-threaded DLL runtimes)
- IM7.StaticDLL.sln (Static Multi-threaded DLL runtimes)
- IM7.Static.sln (Static Multi-threaded runtimes)

Open the solution to start building ImageMagick. The binaries will be created in the `Artifacts\bin` folder.
