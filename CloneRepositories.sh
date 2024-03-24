#/bin/bash
set -e

commit=$1

if [ "$2" == "source" ]; then
    mkdir -p "source"
    cd "source"
fi

clone()
{
    local repo=$1

    echo ''
    echo "Cloning $repo"

    if [ ! -d "$repo" ]; then
        git clone https://github.com/ImageMagick/$repo.git $repo
        if [ $? != 0 ]; then echo "Error during checkout"; exit; fi
    fi
    cd $repo
    git reset --hard
    git pull origin main
    cd ..
}

clone_commit()
{
    local repo=$1
    local commit=$2

    clone $repo

    cd $repo
    git checkout $commit
    cd ..
}

clone_date()
{
    local repo=$1
    local date=$2

    clone $repo

    cd $repo
    git checkout `git rev-list -n 1 --before="$date" origin/main`
    cd ..
}

clone_branch()
{
    local repo=$1
    local branch=$2

    clone $repo

    cd $repo
    git checkout -b branch
    cd ..
}

if [ -z "$commit" ]; then
    commit=$(git ls-remote "https://github.com/ImageMagick/ImageMagick" "main" | cut -f 1)
fi

clone_commit 'ImageMagick' "$commit"

# get a commit date from the current ImageMagick checkout
cd ImageMagick
declare -r commitDate=`git log -1 --format=%ci`
echo "Set latest commit date as $commitDate"
cd ..

clone_date 'aom' "$commitDate"
clone_date 'brotli' "$commitDate"
clone_date 'bzlib' "$commitDate"
clone_date 'cairo' "$commitDate"
clone_date 'contrib' "$commitDate"
clone_date 'croco' "$commitDate"
clone_date 'dcraw' "$commitDate"
clone_date 'de265' "$commitDate"
clone_date 'deflate' "$commitDate"
clone_date 'exr' "$commitDate"
clone_date 'ffi' "$commitDate"
clone_date 'fftw' "$commitDate"
clone_date 'flif' "$commitDate"
clone_date 'freetype' "$commitDate"
clone_date 'fribidi' "$commitDate"
clone_date 'glib' "$commitDate"
clone_date 'harfbuzz' "$commitDate"
clone_date 'heif' "$commitDate"
clone_date 'highway' "$commitDate"
clone_date 'jasper' "$commitDate"
clone_date 'jbig' "$commitDate"
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
clone_date 'VisualMagick' "$commitDate"
clone_date 'webp' "$commitDate"
clone_date 'win2k' "$commitDate"
clone_date 'xml' "$commitDate"
clone_date 'zip' "$commitDate"
clone_date 'zlib' "$commitDate"
