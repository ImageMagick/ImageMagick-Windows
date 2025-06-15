#/bin/bash
set -e

clone()
{
    local repo=$1
    local folder=$2

    echo ''
    echo "Cloning $repo"

    if [ -z "$folder" ]; then
        folder=$repo
    fi

    if [ -d "$folder" ]; then
        cd $folder
    else
        git clone https://github.com/ImageMagick/$repo.git $folder
        if [ $? != 0 ]; then echo "Error during checkout"; exit; fi

        cd $folder
        git remote add sshpush git@github.com:ImageMagick/$repo.git
        git config remote.pushDefault sshpush
    fi

    git checkout new-config 2>/dev/null || git checkout -b new-config
    git reset --hard
    git pull origin new-config
    cd ..
}

clone_configure()
{
    clone 'Configure'
}

clone_dependencies()
{
    if [ ! -d "Dependencies" ]; then
        mkdir -p "Dependencies"
    fi

    cd "Dependencies"

    clone 'aom'
    clone 'brotli'
    clone 'bzlib'
    clone 'cairo'
    clone 'croco'
    clone 'de265'
    clone 'deflate'
    clone 'exr'
    clone 'ffi'
    clone 'freetype'
    clone 'fribidi'
    clone 'glib'
    clone 'harfbuzz'
    clone 'heif'
    clone 'highway'
    clone 'jpeg-turbo'
    clone 'jpeg-turbo-12'
    clone 'jpeg-turbo-16'
    clone 'jpeg-xl'
    clone 'lcms'
    clone 'lqr'
    clone 'lzma'
    clone 'openh264'
    clone 'openjpeg'
    clone 'pango'
    clone 'pixman'
    clone 'png'
    clone 'raqm'
    clone 'raw'
    clone 'rsvg'
    clone 'tiff'
    clone 'webp'
    clone 'xml'
    clone 'zip'
    clone 'zlib'

    cd ..
}

clone_optional_applications()
{
    if [ ! -d "OptionalApplications" ]; then
        mkdir -p "OptionalApplications"
    fi

    cd "OptionalApplications"
    
    clone 'dcraw'
    clone 'IMDisplay'

    cd ..
}

clone_optional_dependencies()
{
    if [ ! -d "OptionalDependencies" ]; then
        mkdir -p "OptionalDependencies"
    fi

    cd "OptionalDependencies"

    clone 'fftw'
    clone 'flif'
    clone 'jbig'

    cd ..
}

download_release()
{
    local project=$1
    local release=$2
    local file=$3

    echo "Downloading $file from $release of $project"

    curl -sS -L "https://github.com/ImageMagick/$project/releases/download/$release/$file" -o "$file"
}

download_configure()
{
    if [[ "$OSTYPE" == "msys"* ]]; then
        cd "Configure"

        configure_release="2025.05.25.1917"
        download_release "Configure" "$configure_release" "Configure.Release.x64.exe"
        download_release "Configure" "$configure_release" "Configure.Release.arm64.exe"
        download_release "Configure" "$configure_release" "Configure.Release.x86.exe"

        cd ..
    fi
}

development=true
imagemagick=0

while [[ $# -gt 0 ]]; do
  case $1 in
    --development)
      development=true
      shift
      ;;
    --imagemagick6)
      imagemagick=6
      shift
      ;;
    --imagemagick7)
      imagemagick=7
      shift
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

if [[ "$development" == true ]]; then
    echo "Cloning dependencies for development"
    clone_configure
    clone_dependencies
    clone_optional_dependencies
    clone_optional_applications
else
    echo "Cloning dependencies for release"
    download_configure
fi

if [[ "$imagemagick" == 6 ]]; then
    echo "Cloning ImageMagick 6"
    clone 'ImageMagick6' 'ImageMagick'
elif [[ "$imagemagick" == 7 ]]; then
    echo "Cloning ImageMagick 7"
    clone 'ImageMagick'
fi