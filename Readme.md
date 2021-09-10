# Building ImageMagick on Windows.

This document describes the requirements and instructions to build ImageMagick for Windows on your own machine.

### Requirements

- Visual Studio 2013 (or newer)
- Git for Windows
- AMD APP SDK (optional for OpenCL support)

### Install Visual Studio dependencies

To build ImageMagick with Visual Studio the following components should be installed:

- Desktop development with C++ (workload)
- Visual C++ ATL for x86 and x64
- Visual C++ MFC for x86 and x64
- Windows XP support for C++ (optional if the output needs to run on Windows XP)

Nameing of these components might differ between version of Visual Studio. The example above use the Visual Studio
2017 names.

### Clone the dependencies

The ImageMagick library is build with a number of third party libraries. Run `CloneRepositories.cmd` to clone
these libraries and the ImageMagick library.

### Build configure.exe

One of the folders that was created in the previous step is called `VisualMagick` and this folder contains a
folder called `configure`. This folder contains the solution file `configure.2019.sln` for the latest 2019
version of Visual Studio, and `configure.sln` for older versions. Open one of those and start a `Release`
build of the project. This will create a file called `configure.exe` in the folder. Running this program
will start a Wizard that allows configuration of ImageMagick and its individual components.

### Build ImageMagick

Depending on which options where chosen when running `configure.exe` one of the following solutions will be created
in the `VisualMagick` folder:

- VisualDynamicMT.sln (Dynamic Multi-threaded DLL runtimes)
- VisualStaticMTD.sln (Static Multi-threaded DLL runtimes)
- VisualStaticMT.sln (Static Multi-threaded runtimes)

Open the solution to start building ImageMagick. The binaries will be created in the `VisualMagick\bin` folder.
