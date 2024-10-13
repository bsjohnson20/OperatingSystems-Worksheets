#!/bin/bash

# vars
task=1
dir="temp${task}/"
cd src
# directories 
mkdir ../output -p
mkdir temp${task} -p


cd temp${task}
cp ../asm_io.inc .

# compiling
nasm -f elf ../task${task}.asm -o task${task}.o
nasm -f elf ../asm_io.asm -o asm_io.o
gcc -m32 -c ../driver.c -o driver.o

rm ../../output/task${task}
gcc -m32 driver.o task${task}.o asm_io.o -o ../../output/task${task}
# nasm -f elf ../task${task}.asm -o task${task}.o
cd ..
rm -rf temp${task}
cd ..
./output/task${task}
