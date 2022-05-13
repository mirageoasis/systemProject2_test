#!/bin/bash

PORT=60175
IP=127.0.0.1

FILE_NAME=""
DIR_NAME=""

EVENT="event_based"
THREAD="thread_based"
NORM="_normal_"
SHOW="_show_"
SB="_sb_"
CSV=".csv"
RES="result"

#
MAX_ORDER=1
MAX_CLIENT=1
#

## 명령어 모드
NORMAL=0
ONLY_SHOW=1
ONLY_SB=2
##
##주식 개수
STOCK_NOW=10
STOCK_INC=10
##
## stock file는 걍 모두 저장
## resource는 stock server이외에 모두 저장


# 폴더 이름 설정하고 // 서버종류_모드_숫자
# resource 복사하면서 폴더 만들고
# stockserver 복사하고
# stock.txt을 복사하고
# 숫자.txt 를 stock.txt로 만든다
# 입장하고
# make
# stockserver
# client 
# 파일 result 로 복사하고
# make clean
# fuser -k
# cd ..
#rmdir


## 10 NORMAL THREAD EVENT
while [ $STOCK_NOW -le 40 ]
do
    DIR_NAME=$EVENT$NORM$STOCK_NOW
    FILE_NAME=$EVENT$NORM$STOCK_NOW$CSV

    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $NORMAL
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME


    DIR_NAME=$THREAD$NORM$STOCK_NOW
    FILE_NAME=$THREAD$NORM$STOCK_NOW$CSV
    
    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $NORMAL
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME
    ## 10 NORMAL THREAD EVENT


    ## 10 SHOW THREAD EVENT
    DIR_NAME=$EVENT$SHOW$STOCK_NOW
    FILE_NAME=$EVENT$SHOW$STOCK_NOW$CSV

    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $ONLY_SHOW
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME


    DIR_NAME=$THREAD$SHOW$STOCK_NOW
    FILE_NAME=$THREAD$SHOW$STOCK_NOW$CSV

    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $ONLY_SHOW
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME
    ## 10 SHOW THREAD EVENT


    ## 10 SB THREAD EVENT
    DIR_NAME=$EVENT$SB$STOCK_NOW
    FILE_NAME=$EVENT$SB$STOCK_NOW$CSV

    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $ONLY_SB
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME


    DIR_NAME=$THREAD$SB$STOCK_NOW
    FILE_NAME=$THREAD$SB$STOCK_NOW$CSV

    cp -r resource $DIR_NAME
    cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
    cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

    cd $DIR_NAME
    make
    ./stockserver ${PORT} &
    sleep 1
    #taskset -c 0 stockserver
    ./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $STOCK_NOW $ONLY_SB
    cp result.csv ../result/$FILE_NAME
    make clean
    fuser -k ${PORT}/tcp
    cd ..
    rm -rf -R $DIR_NAME

    STOCK_NOW=$(($STOCK_INC+$STOCK_NOW))    
done
## 10 SB THREAD EVENT

exit

## STOCK NUMBER CHANGE 
STOCK_NOW=$(($STOCK_INC+$STOCK_NOW))
## STOCK NUMBER CHANGE


## 20 NORMAL THREAD EVENT
DIR_NAME=$EVENT$NORM$STOCK_NOW
FILE_NAME=$EVENT$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$NORM$STOCK_NOW
FILE_NAME=$THREAD$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 20 NORMAL THREAD EVENT


## 20 SHOW THREAD EVENT
DIR_NAME=$EVENT$SHOW$STOCK_NOW
FILE_NAME=$EVENT$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SHOW$STOCK_NOW
FILE_NAME=$THREAD$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 20 SHOW THREAD EVENT


## 20 SB THREAD EVENT
DIR_NAME=$EVENT$SB$STOCK_NOW
FILE_NAME=$EVENT$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SB$STOCK_NOW
FILE_NAME=$THREAD$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 20 SB THREAD EVENT

## STOCK NUMBER CHANGE 
STOCK_NOW=$(($STOCK_INC+$STOCK_NOW))
## STOCK NUMBER CHANGE


## 30 NORMAL THREAD EVENT
DIR_NAME=$EVENT$NORM$STOCK_NOW
FILE_NAME=$EVENT$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$NORM$STOCK_NOW
FILE_NAME=$THREAD$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 NORMAL THREAD EVENT


## 30 SHOW THREAD EVENT
DIR_NAME=$EVENT$SHOW$STOCK_NOW
FILE_NAME=$EVENT$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SHOW$STOCK_NOW
FILE_NAME=$THREAD$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 SHOW THREAD EVENT


## 30 SB THREAD EVENT
DIR_NAME=$EVENT$SB$STOCK_NOW
FILE_NAME=$EVENT$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SB$STOCK_NOW
FILE_NAME=$THREAD$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 SB THREAD EVENT

## STOCK NUMBER CHANGE 
STOCK_NOW=$(($STOCK_INC+$STOCK_NOW))
## STOCK NUMBER CHANGE


## 30 NORMAL THREAD EVENT
DIR_NAME=$EVENT$NORM$STOCK_NOW
FILE_NAME=$EVENT$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$NORM$STOCK_NOW
FILE_NAME=$THREAD$NORM$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $NORMAL
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 NORMAL THREAD EVENT


## 30 SHOW THREAD EVENT
DIR_NAME=$EVENT$SHOW$STOCK_NOW
FILE_NAME=$EVENT$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SHOW$STOCK_NOW
FILE_NAME=$THREAD$SHOW$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SHOW
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 SHOW THREAD EVENT


## 30 SB THREAD EVENT
DIR_NAME=$EVENT$SB$STOCK_NOW
FILE_NAME=$EVENT$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME


DIR_NAME=$THREAD$SB$STOCK_NOW
FILE_NAME=$THREAD$SB$STOCK_NOW$CSV

cp -r resource $DIR_NAME
cp $EVENT/stockserver.c $DIR_NAME/stockserver.c
cp stock_files/$STOCK_NOW.txt $DIR_NAME/stock.txt

cd $DIR_NAME
make
./stockserver ${PORT} &
#taskset -c 0 stockserver
./multiclient $IP $PORT $MAX_CLIENT $MAX_ORDER $ONLY_SB
cp result.csv ../result/$FILE_NAME
make clean
fuser -k ${PORT}/tcp
cd ..
rm -rf -R $DIR_NAME
## 30 SB THREAD EVENT