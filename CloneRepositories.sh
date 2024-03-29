#/bin/bash
set -e

commit=$1

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
    git show --oneline -s
    cd ..
}

clone_date()
{
    local repo=$1
    local date=$2

    clone $repo

    cd $repo
    git checkout `git rev-list -n 1 --before="$date" origin/main`
    git show --oneline -s
    cd ..
}

clone_branch()
{
    local repo=$1
    local branch=$2

    clone $repo

    cd $repo
    git checkout $branch
    git show --oneline -s
    cd ..
}

if [ -d "../ImageMagick" ]; then
    branch_name=$(git -C ../ImageMagick symbolic-ref --short HEAD 2>/dev/null || true)

    if [ "$branch_name" != "main" ]; then
        echo "Copying repository from ../ImageMagick"
        cp -R ../ImageMagick ImageMagick
        git -C ImageMagick show --oneline -s
    fi
fi

if [ ! -d "ImageMagick" ]; then
    if [ -z "$commit" ]; then
        commit=$(git ls-remote "https://github.com/ImageMagick/ImageMagick" "main" | cut -f 1)
    fi

    clone_commit 'ImageMagick' "$commit"
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
