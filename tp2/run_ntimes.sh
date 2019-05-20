#!/bin/bash

max_threads=$1
n=$2

for ((j=1;j<=max_threads;j=j*2));do
    for ((i=1;i<=n;i++));do
        ./main $j
    done
done