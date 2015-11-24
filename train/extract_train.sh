#!/bin/bash

#train : offset =0
rm -r train_set
mkdir train_set
Ntrain=2100
for((i=0;i<Ntrain;i++))
do

cp ./pic/depth"$i".png ./train_set

done