#!/bin/bash

while getopts 't:s:d:f:c:' c
do
  case $c in
    t) USER_PROCESS=$OPTARG ;;
    s) SCORE=$OPTARG ;;
    d) DURATION=$OPTARG ;;
    f) PNG_PATH=$OPTARG ;;
    c) COPY_COMMAND=$OPTARG ;;
  esac
done
export LD_LIBRARY_PATH=./

START_TIME=`date +%Y-%m-%d,%H:%m:%S`
START_TIMESTAMP=`date +%s`

rm user.log

echo "======================================================="
echo "选手程序开始运行 : $START_TIME"
# $USER_PROCESS > user.log 2>&1 &
# echo $! > pidfile.txt
nohup $USER_PROCESS > user.log 2>&1 & echo $! > pidfile.txt
echo -e "=======================================================\n"

USER_PROCESSID=`cat pidfile.txt`

END_TIME_TIMESTAMP=`date +%s`
while [ $[END_TIME_TIMESTAMP - START_TIMESTAMP] -lt $DURATION ]
do
  END_TIME_TIMESTAMP=`date +%s`
  COUNT=`ps aux | grep $USER_PROCESSID | grep -v grep | wc -l`
  if [ $COUNT -eq 0 ]
  then
    break
  fi
  sleep 1
  echo -en "\r选手程序已运行 : $[END_TIME_TIMESTAMP - START_TIMESTAMP] 秒"
done



echo -e "\n\n======================================================="
if [ $COUNT -ne 0 ]
then
  echo "选手程序运行时间超过预定时间,强制结束！"
  kill -9 $USER_PROCESSID
fi

END_TIME=`date +%Y-%m-%d,%H:%m:%S`
END_TIME_TIMESTAMP=`date +%s`
echo "选手程序结束运行 : $END_TIME"
echo "选手程序运行耗时 : $[END_TIME_TIMESTAMP - START_TIMESTAMP] 秒"
echo "======================================================="


echo -e "\n======================================================="
echo "拷贝图幅文件"
rm -rf ./select_Annotations
mkdir -p ./select_Annotations

array=(${COPY_COMMAND//,/ })
for var in ${array[@]}
do
   echo "拷贝图幅：$var"
   cp -r ./Annotations/$var* ./select_Annotations/
done
echo "======================================================="


echo -e "\n======================================================="
echo "打分程序开始运行!"
echo "各项分数为:"
$SCORE
echo "打分程序结束运行!"
echo "======================================================="


echo -e "\n======================================================="
echo "选手运行速度检测:"
COUNT=`grep Result $PNG_PATH | wc -l`
echo "选手程序一共生成图片: $COUNT 张"
FPS=`expr $COUNT / $[END_TIME_TIMESTAMP - START_TIMESTAMP]`
echo "选手程序FPS为: $FPS 帧"
echo "======================================================="
