#/bin/bash

clone_repository()
{
  echo ''
  echo "Cloning $3"

  branch="master"
  if [ ! -z "$4" ]; then
    branch="$4"
  fi

  dir="$3"
  if [ ! -z "$5" ]; then
    dir="$5"
  fi

  if [ "$2" == "full" ]; then
    if [ ! -d "$dir" ]; then
      git clone -b $branch $1/$3.git $dir
    fi
    cd $dir
    git pull
    cd ..
  else
    if [ ! -d "$dir" ]; then
      git clone -b $branch --depth 1 $1/$3.git $dir
    fi
  fi
  cd $dir
  git show --oneline -s
  cd ..
}

clone_repository $1 $2 'aom'
clone_repository $1 $2 'bzlib'
clone_repository $1 $2 'cairo'
clone_repository $1 $2 'contrib'
clone_repository $1 $2 'croco'
clone_repository $1 $2 'dcraw'
clone_repository $1 $2 'exr'
clone_repository $1 $2 'ffi'
clone_repository $1 $2 'flif'
clone_repository $1 $2 'freetype'
clone_repository $1 $2 'fribidi'
clone_repository $1 $2 'glib'
clone_repository $1 $2 'harfbuzz'
clone_repository $1 $2 'ImageMagick'
clone_repository $1 $2 'jbig'
clone_repository $1 $2 'jp2'
clone_repository $1 $2 'jpeg-turbo' 'master' 'jpeg'
clone_repository $1 $2 'lcms'
clone_repository $1 $2 'libde265'
clone_repository $1 $2 'libheif'
clone_repository $1 $2 'liblzma'
clone_repository $1 $2 'libraw'
clone_repository $1 $2 'librsvg'
clone_repository $1 $2 'libxml'
clone_repository $1 $2 'lqr'
clone_repository $1 $2 'openjpeg'
clone_repository $1 $2 'pango'
clone_repository $1 $2 'pixman'
clone_repository $1 $2 'png'
clone_repository $1 $2 'tiff'
clone_repository $1 $2 'VisualMagick'
clone_repository $1 $2 'webp'
clone_repository $1 $2 'win2k'
clone_repository $1 $2 'zlib'
