#!/bin/bash

echo 10000 | ./simpleCode > log.10K
gprof -b simpleCode gmon.out > 10K.rpt
mv gmon.out gmon.out.10K

echo 100000 | ./simpleCode > log.100K
gprof -b simpleCode gmon.out > 100K.rpt
mv gmon.out gmon.out.100K

echo 1000000 | ./simpleCode > log.1M
gprof -b simpleCode gmon.out > 1M.rpt
mv gmon.out gmon.out.1M

echo 10000000 | ./simpleCode > log.10M
gprof -b simpleCode gmon.out > 10M.rpt
mv gmon.out gmon.out.10M

echo 100000000 | ./simpleCode > log.100M
gprof -b simpleCode gmon.out > 100M.rpt
mv gmon.out gmon.out.100M

