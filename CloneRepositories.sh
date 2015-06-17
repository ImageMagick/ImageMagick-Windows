#/bin/bash

clone_repository()
{
  echo ''
  echo "Cloning $2"

  branch="master"

  dir="$2"
  if [ ! -z "$4" ]; then
    dir="$4"
  fi

  if [ "$1" == "full" ]; then
    if [ ! -d "$dir" ]; then
      git clone -b $branch https://github.com/ImageMagick/$2.git $dir
    fi
    cd $dir
    git pull
    cd ..
  else
    if [ ! -d "$dir" ]; then
      git clone -b $branch --depth 1 https://github.com/ImageMagick/$2.git $dir
    fi
  fi
}

clone_repository $1 'bzlib'
clone_repository $1 'cairo'
clone_repository $1 'contrib'
clone_repository $1 'croco'
clone_repository $1 'exr'
clone_repository $1 'ffi'
clone_repository $1 'glib'
clone_repository $1 'ImageMagick'
clone_repository $1 'jbig'
clone_repository $1 'jp2'
clone_repository $1 'jpeg'
clone_repository $1 'lcms'
clone_repository $1 'librsvg'
clone_repository $1 'libxml'
clone_repository $1 'lqr'
clone_repository $1 'openjpeg'
clone_repository $1 'pango'
clone_repository $1 'pixman'
clone_repository $1 'png'
clone_repository $1 'tiff'
clone_repository $1 'ttf'
clone_repository $1 'VisualMagick'
clone_repository $1 'webp'
clone_repository $1 'win2k'
clone_repository $1 'zlib'
