#/bin/bash
set -e

clone()
{
    local repo=$1
    local folder=$2

    echo ''
    echo "Cloning $repo"

    if [ ! -d "$folder" ]; then
        git clone https://github.com/ImageMagick/$repo.git $folder
        if [ $? != 0 ]; then echo "Error during checkout"; exit; fi
    fi

    cd $folder
    git reset --hard
    git pull origin main
    cd ..
}

clone_commit()
{
    local repo=$1
    local commit=$2
    local folder=$3

    clone $repo $folder

    cd $folder
    git checkout $commit >/dev/null
    git show --oneline -s
    cd ..
}

clone_date()
{
    local repo=$1
    local date=$2

    clone $repo $repo

    cd $repo
    local commit=$(git rev-list -n 1 --before="$date" origin/main)
    git checkout $commit >/dev/null
    git show --oneline -s
    cd ..
}

imagemagick=$1
commit=$2

if [ -z "$imagemagick" ]; then
    echo "Usage: $0 ImageMagick/ImageMagick6 [<commit>]"
    exit 1
fi

if [ -d "../$imagemagick" ]; then
    echo "Copying repository from ../$imagemagick"
    cp -R ../$imagemagick "ImageMagick"
    git -C "ImageMagick" show --oneline -s
else
    if [ -z "$commit" ]; then
        commit=$(git ls-remote "https://github.com/ImageMagick/$imagemagick" "main" | cut -f 1)
    fi

    clone_commit "$imagemagick" "$commit" "ImageMagick"
fi

# get a commit date from the current ImageMagick checkout
declare -r commitDate=`git -C ImageMagick log -1 --format=%ci`
echo "Set latest commit date as $commitDate"

if [ ! -d "Dependencies" ]; then
    mkdir -p "Dependencies"
fi

cd "Dependencies"

clone_date 'aom' "$commitDate"
clone_date 'brotli' "$commitDate"
clone_date 'bzlib' "$commitDate"
clone_date 'cairo' "$commitDate"
clone_date 'croco' "$commitDate"
clone_date 'de265' "$commitDate"
clone_date 'deflate' "$commitDate"
clone_date 'exr' "$commitDate"
clone_date 'ffi' "$commitDate"
clone_date 'freetype' "$commitDate"
clone_date 'fribidi' "$commitDate"
clone_date 'glib' "$commitDate"
clone_date 'harfbuzz' "$commitDate"
clone_date 'heif' "$commitDate"
clone_date 'highway' "$commitDate"
clone_date 'jpeg-turbo' "$commitDate"
clone_date 'jpeg-xl' "$commitDate"
clone_date 'lcms' "$commitDate"
clone_date 'lqr' "$commitDate"
clone_date 'lzma' "$commitDate"
clone_date 'openjpeg' "$commitDate"
clone_date 'pango' "$commitDate"
clone_date 'pixman' "$commitDate"
clone_date 'png' "$commitDate"
clone_date 'raqm' "$commitDate"
clone_date 'raw' "$commitDate"
clone_date 'rsvg' "$commitDate"
clone_date 'tiff' "$commitDate"
clone_date 'webp' "$commitDate"
clone_date 'xml' "$commitDate"
clone_date 'zip' "$commitDate"
clone_date 'zlib' "$commitDate"

cd ..

if [ ! -d "OptionalDependencies" ]; then
    mkdir -p "OptionalDependencies"
fi

cd "OptionalDependencies"

clone_date 'dcraw' "$commitDate"
clone_date 'fftw' "$commitDate"
clone_date 'flif' "$commitDate"
clone_date 'IMDisplay' "$commitDate"
clone_date 'jbig' "$commitDate"
