#!/bin/bash

date > pthread.dat
date > serial.dat
for i in `seq 0 299`;
do
    echo -n "$i, " >> pthread.dat
    echo -n "$i, " >> serial.dat
    ./dotproduct_pthread 10000 2 >> pthread.dat
    ./dotproduct_serial 10000 >> serial.dat
done
