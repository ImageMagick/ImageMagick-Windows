#/bin/bash

clone_repository()
{
  echo ''
  echo "Cloning $3"

  branch="main"
  dir="$3"

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

clone_imagemagick()
{
  if [ ! -d "../ImageMagick" ]; then
    clone_repository $1 $2 'ImageMagick'
  else
    cp -R ../ImageMagick ImageMagick
  fi
}

if [ "$2" == "source" ]; then
  mkdir -p "source"
  cd "source"
fi

clone_repository $1 $2 'aom'
clone_repository $1 $2 'brotli'
clone_repository $1 $2 'bzlib'
clone_repository $1 $2 'cairo'
clone_repository $1 $2 'contrib'
clone_repository $1 $2 'croco'
clone_repository $1 $2 'dcraw'
clone_repository $1 $2 'exr'
clone_repository $1 $2 'ffi'
clone_repository $1 $2 'fftw'
clone_repository $1 $2 'flif'
clone_repository $1 $2 'freetype'
clone_repository $1 $2 'fribidi'
clone_repository $1 $2 'glib'
clone_repository $1 $2 'harfbuzz'
clone_repository $1 $2 'highway'
clone_repository $1 $2 'jbig'
clone_repository $1 $2 'jp2'
clone_repository $1 $2 'jpeg-turbo'
clone_repository $1 $2 'jpeg-xl'
clone_repository $1 $2 'lcms'
clone_repository $1 $2 'libde265'
clone_repository $1 $2 'libheif'
clone_repository $1 $2 'liblzma'
clone_repository $1 $2 'libraw'
clone_repository $1 $2 'librsvg'
clone_repository $1 $2 'libxml'
clone_repository $1 $2 'libzip'
clone_repository $1 $2 'lqr'
clone_repository $1 $2 'openjpeg'
clone_repository $1 $2 'pango'
clone_repository $1 $2 'pixman'
clone_repository $1 $2 'png'
clone_repository $1 $2 'raqm'
clone_repository $1 $2 'tiff'
clone_repository $1 $2 'VisualMagick'
clone_repository $1 $2 'webp'
clone_repository $1 $2 'win2k'
clone_repository $1 $2 'zlib'

clone_imagemagick $1 $2

if [ "$2" == "source" ]; then
  for folder in *; do
    if [ -d "$folder" ]; then
       cd $folder
       rm -Rf .git
       cd ..
    fi
  done
fi

