#/bin/bash

check_release()
{
  current=$(grep "DELEGATE_VERSION_NUM" "$(dirname $1)/ImageMagick.version.h" | cut -d " " -f 7)
  current=${current//,/.}

  release_url=$(head -n 1 $1)
  format=$(echo $release_url | cut -d " " -f 2)

  release_url=$(echo $release_url | cut -d " " -f 1)
  data=$(curl -s $release_url)
  if [ "$format" == "base64" ]; then
    data=$(echo $data | base64 --decode)
  fi

  regex=$(tail -n 1 $1)
  if [[ $data =~ $regex ]]; then
    latest=${BASH_REMATCH[1]}
    dot_count=$(echo $latest | grep -o "\." | wc -l)
    if [ "$dot_count" = "1" ]; then
      latest="$latest.0"
    fi
  else
    latest="unable to load version"
  fi

  project=$(basename $(dirname $(dirname $1)))

  if [ "$current" = "$latest" ]; then
    return 0
  fi

  echo "$project ${current} => ${latest} (${release_url})"
  return 1
}

exit_code=0

for f in $(find . -name "release-info"); do
  check_release $f
done

exit $exit_code
