#!/bin/bash

SRC=src
BUILD=build
MAIN=disk.img

set -e

rm -rf $BUILD
mkdir $BUILD
mkdir $BUILD/obj
touch $MAIN
gcc -Wextra -g -Wall -c $SRC/interface.c -o $BUILD/obj/interface.o
gcc -Wextra -g -Wall -c $SRC/console.c -o $BUILD/obj/console.o
gcc $BUILD/obj/* $SRC/main.c -o $BUILD/main -Wall -Wextra -g
dd if=/dev/zero of=$MAIN bs=512 count=2880
mkfs.fat -F12 $MAIN
mcopy -i $MAIN test.txt "::test.txt"
mcopy -i $MAIN $SRC/main.c "::main.c"
mcopy -i $MAIN README.md "::readme.md"
mv $MAIN $BUILD/
