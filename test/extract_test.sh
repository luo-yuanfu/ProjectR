#!/bin/bash

#test : offset = 7000

rm -r test_set
mkdir test_set

Ntest=700
offset=7000
for((i=0;i<Ntest;i++))
do
j=$((offset+i))
cp ./pic/depth"$j".png ./test_set

done