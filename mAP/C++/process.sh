#!/bin/bash

while getopts 't:a:s:' c
do
  case $c in
    t) TXTFILE=$OPTARG ;;
    a) ALLXML=$OPTARG ;;
    s) SELECTXML=$OPTARG ;;
  esac
done
export LD_LIBRARY_PATH=./

COPY_COMMAND=`grep Result $TXTFILE`

#array=(${COPY_COMMAND//\r\r/ })
array=${COPY_COMMAND}

for var in ${array[@]}
do
  filename=(${var//\r/.})
  filepath=$ALLXML$filename.xml
  if [ -f "$filepath" ]; then
    cp $filepath $SELECTXML
    ./ResultEvalution $TXTFILE $SELECTXML
  fi
done
