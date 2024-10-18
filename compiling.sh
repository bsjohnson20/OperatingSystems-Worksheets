#!/bin/bash
cd src
mkdir ../output -p
task=$1
runit=$2

# if runit null, set to 0
if [ -z $runit ]
then
    runit=0
fi

mkdir temp${task} -p
dir="temp${task}/"

cd temp${task}
cp ../asm_io.inc .
nasm -f elf ../task${task}.asm -o task${task}.o
nasm -f elf ../asm_io.asm -o asm_io.o
gcc -m32 -c ../driver.c -o driver.o

rm ../../output/task${task}
gcc -m32 driver.o task${task}.o asm_io.o -o ../../output/task${task}
# nasm -f elf ../task${task}.asm -o task${task}.o
cd ..
rm -rf temp${task}
cd ..

if [ $runit == 1 ]
then
    ./output/task${task}
fi

