#!/bin/bash

PORT=60175
IP=172.30.10.11

cd event_based_10
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp

cd ..

cd event_based_20
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp

cd ..

cd event_based_30
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd event_based_40
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd thread_based_10
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd thread_based_20
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd thread_based_30
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd thread_based_40
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd only_show_40_thread
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd only_sb_40_thread
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp

cd ..

cd only_show_40_event
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp


cd ..

cd only_sb_40_event
make
./stockserver ${PORT} &
./multiclient ${IP} ${PORT}
make clean
fuser -k ${PORT}/tcp
