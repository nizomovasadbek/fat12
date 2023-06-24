#!/bin/bash

SRC=src
BUILD=build
MAIN=disk.img

set -e

#for high level debugging

rm -rf $BUILD
mkdir -p $BUILD/obj
touch $MAIN
clang -O3 -Wextra -g -Wall -c $SRC/interface.c -o $BUILD/obj/interface.o -fsanitize=address
clang -O3 -Wextra -g -Wall -c $SRC/console.c -o $BUILD/obj/console.o -fsanitize=address
clang -O3 -Wextra -g -Wall -c $SRC/fat12.c -o $BUILD/obj/fat12.o -fsanitize=address
clang -O3 $BUILD/obj/* $SRC/main.c -o $BUILD/main -Wall -Wextra -g -fsanitize=address
dd if=/dev/zero of=$MAIN bs=512 count=2880
mkfs.fat -F12 $MAIN
mcopy -i $MAIN test.txt "::test.txt"
mcopy -i $MAIN $SRC/main.c "::main.c"
mcopy -i $MAIN README.md "::readme.md"
mv $MAIN $BUILD/
