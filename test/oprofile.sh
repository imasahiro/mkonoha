#!/bin/bash
DATE=`date "+%s"`
mkdir $DATE
CC=gcc
OUT=./build/vmtest
export CFLAGS="-I$(build) -I. -O3 -g -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -fno-common -D__N__=40"
rm ${OUT}
make

sudo opcontrol --start-daemon
sudo opcontrol --vmlinux=/usr/src/linux/vmlinux
sudo opcontrol --no-vmlinux
sudo opcontrol --reset
#sudo opcontrol --event="CPU_CLK_UNHALTED:6000" --event="LLC_MISSES:6000"
sudo opcontrol --event="CPU_CLK_UNHALTED:6000" --event="BR_MISS_PRED_RETIRED:3000"
sudo opcontrol --start;
${OUT} ./test/fibo.k
sudo opcontrol --stop
sudo opcontrol --dump
opannotate -a ${OUT} > $DATE/asm.log.txt
opannotate -s ${OUT} > $DATE/code.log.txt
opreport   -l ${OUT} > $DATE/func.log.txt
sudo opcontrol --shutdown
echo $DATE
objdump -S build/vmtest > dump

vi dump $DATE/func.log.txt $DATE/asm.log.txt
