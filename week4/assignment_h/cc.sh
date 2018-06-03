#!/bin/bash

#CCC="arm-linux-gcc"
CCC="gcc"
FLAGS="-Wall -Wextra"

for i in $(ls *.c); do
    echo `basename $i .c`.o
    $CCC -o `basename $i .c`.o -c $FLAGS $i || exit
done

echo

for i in $(ls T*.o); do
    echo `basename $i .o`
    $CCC -o `basename $i .o` $i A*.o C*.o  H*.o -lrt || exit
done


