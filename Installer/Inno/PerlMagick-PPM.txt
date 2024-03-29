This directory contains PerlMagick built for Strawberry Perl.

Installing PerlMagick
=====================

Normally PerlMagick is installed via a checkbox in the ImageMagick
setup program. If PerlMagick was not installed at that time, it may be
installed from within this directory by executing the command

  ppm install Image-Magick.ppd

You should see output similar to:

  Installing package 'Image-Magick.ppd'...
  Writing C:\Perl\site\lib\auto\Image\Magick\.packlist
  ...\PerlMagick>

If you see the above, it means it's worked. If you get: 

  ...\PerlMagick>>ppm install Image-Magick.ppd
  'ppm' is not recognized as an internal or external command,
  operable program or batch file.
  ...\PerlMagick>

it means your installation of Stawberry Perl isn't installed, or isn't
configured properly - you need to make sure that the folder containing
ppm.bat (usually c:\perl\bin) is in your system path.


Uninstalling PerlMagick
=======================

If PerlMagick was installed via ImageMagick's setup program, then it is
automatically uninstalled when ImageMagick is uninstalled.  If
PerlMagick was installed manually, then it may be uninstalled via the
following command:

  ppm remove Image-Magick


Running the PerlMagick Demos
============================

The PerlMagick demos are available in the demo subdirectory.  If you
have a 'make' or 'nmake' program installed on your computer, then you
may run the demos like:

  cd demo
  nmake

If make is not available on your system, then you may still run the
demos individually like:

  perl demo.pl
  perl button.pl
  perl shapes.pl
  perl piddle.pl
  perl tree.pl
  perl steganography.pl
  perl shadow_text.pl

The output from each demo should be displayed to your screen.
