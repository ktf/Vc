#!/bin/zsh -e

start=$1
if [[ ! -f "$start" ]]; then
  echo "Usage: $0 <header-file>"
  exit 1
fi

setopt extendedglob
IFS="\n\0"
seen=()

parse_file() {
  #echo "${seen[@]}"
  if [[ "$1" = ${(~j.|.)seen} ]]; then
    return
  fi
  seen=(${seen[@]} "$1")
  [[ "$1" =~ "/" ]] && pushd "${1%/*}"
  while read -r line; do
    match='*include*'
    case "$line" in
      \ #\#\ #include\ #\"*\")
        echo $line|cut -d\" -f2|read include
        parse_file "$include"
        ;;
      *)
        echo "$line"
        ;;
    esac
  done < "${1##*/}"
  [[ "$1" =~ "/" ]] && popd
}

parse_file "$start"
