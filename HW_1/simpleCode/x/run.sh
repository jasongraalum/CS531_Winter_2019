#!/bin/bash

for i in {1..50}
do
	echo 10000000 | ./simpleCode > log/$i.out 
	mv gmon.out gmon/gmon.out.$i
	gprof -b simpleCode gmon/gmon.out.$i > gprof/$i.rpt
done

grep  '^\[1\]' gprof/*.rpt | awk '{print "main, " $2 ", " $3 ", " $5}' > data.csv
grep  '^\[2\]' gprof/*.rpt | awk '{print "BinarySearch, " $2 ", " $3 ", " $5}' >> data.csv
