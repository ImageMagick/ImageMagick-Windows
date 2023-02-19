#/bin/bash

check_release()
{
  current=$(grep "DELEGATE_VERSION_NUM" "$1/ImageMagick/ImageMagick.version.h" | cut -d " " -f 7)
  current=${current//,/.}

  release_url=$(head -n 1 "$1/ImageMagick/release-info")
  format=$(echo $release_url | cut -d " " -f 2)

  release_url=$(echo $release_url | cut -d " " -f 1)
  data=$(curl -s $release_url)
  if [ "$format" == "base64" ]; then
    data=$(echo $data | base64 --decode)
  fi

  regex=$(tail -n 1 "$1/ImageMagick/release-info")
  if [[ $data =~ $regex ]]; then
    latest=${BASH_REMATCH[1]}
    dot_count=$(echo $latest | grep -o "\." | wc -l)
    if [ "$dot_count" = "1" ]; then
      latest="$latest.0"
    fi
  else
    latest="unable to load version"
  fi

  if [ "$current" = "$latest" ]; then
    return 0
  fi

  echo "$1${current} => ${latest} (${release_url})"
  return 1
}

exit_code=0

for d in */ ; do
  if [ -f "$d/ImageMagick/release-info" ]; then
    check_release $d
    if [ $? != 0 ]; then
      exit_code=1
    fi
  fi
done

exit $exit_code
