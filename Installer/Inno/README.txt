[ The files in this directory are to support the ImageMagick
  setup.exe-style installer package. The following are the instructions
  for how to build a Q:8 DLL-based distribution installer package
  using Visual Studio. ]

	   Steps for building VisualMagick Distribution Package

0) Install prerequisite software

  a) Download and install the Inno Setup Compiler from
      "http://www.jrsoftware.org/isinfo.php".

  b) Download and install Strawberry Perl from http://www.strawberryperl.com.

1) Open workspace VisualMagick\configure\configure.sln by double-clicking
   from Windows Explorer.

   a) Select "Rebuild All"
   b) Run the configure program
   c) Select DLL build
   d) Check "Include all demo, test, appl, and contrib directories"
   e) Check "Use registry settings or hard-coded paths to locate installed components"
   f) Finish remaining configure wizard screens to complete.
   g) Close Visual Studio

2) Open workspace VisualMagick\VisualDynamicMT.sln by double-clicking from
   Windows Explorer or via Visual C++ dialog.

   a) Build -> "Rebuild All"

3) Open Windows Command Shell Window

   a) cd PerlMagick
   b) nmake clean (only if this is a rebuild)
   c) perl Makefile.nt
   d) nmake release

NOTE: access to nmake requires that there be a path to it. Depending n
how the install of Visual Studio was done, this may not be the case.
Visual Studio provides a batch script in VC98\Bin called VCVARS32.BAT
that can be used to do this manually after you open up a command
prompt.

4) Open VisualMagick\installer\im-x86-dll-Q8.iss by double-clicking from
   Windows Explorer.

   a) File -> Compile
   b) Test install by clicking on green triangle

5)
   Distribution package is available as

     VisualMagick\installer\output\ImageMagick-7.X-X-X-Q8-dll.exe

