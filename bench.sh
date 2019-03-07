#!/bin/bash

#./laser.sh b  

time ./laser.sh bench atom 1000000 100 0 ~/stream_file.txt
time ./laser.sh bench atom 10000 10000 0 ~/stream_file.txt
time ./laser.sh bench atom 100 1000000 0 ~/stream_file.txt

## ATOM
#./laser.sh bench atom 1000000 1 0 ~/stream_file.txt
#./laser.sh bench atom 100000 10 0 ~/stream_file.txt
#./laser.sh bench atom 10000 100 0 ~/stream_file.txt
#./laser.sh bench atom 1000 1000 0 ~/stream_file.txt
#./laser.sh bench atom 100 10000 0 ~/stream_file.txt
#./laser.sh bench atom 10 100000 0 ~/stream_file.txt
#./laser.sh bench atom 1 1000000 0 ~/stream_file.txt

## Diamond - dif input sizes
#./laser.sh bench diamond 100000 10 3 ~/stream_file.txt
#./laser.sh bench diamond 1000 1000 3 ~/stream_file.txt
#./laser.sh bench diamond 10 100000 3 ~/stream_file.txt

## Box 
#./laser.sh bench box 100000 10 3 ~/stream_file.txt
#./laser.sh bench box 1000 1000 3 ~/stream_file.txt
#./laser.sh bench box 10 100000 3 ~/stream_file.txt

## Conjunction
#./laser.sh bench conjunction 1000000 1 0 ~/stream_file.txt
#./laser.sh bench conjunction 100000 10 0 ~/stream_file.txt
#./laser.sh bench conjunction 10000 100 0 ~/stream_file.txt
#./laser.sh bench conjunction 1000 1000 0 ~/stream_file.txt
#./laser.sh bench conjunction 100 10000 0 ~/stream_file.txt
#./laser.sh bench conjunction 10 100000 0 ~/stream_file.txt
##./laser.sh bench conjunction 1 1000000 0 ~/stream_file.txt

## Diamond - dif window sizes:
#./laser.sh bench diamond 1000 1000 1 ~/stream_file.txt
#./laser.sh bench diamond 1000 1000 10 ~/stream_file.txt
#./laser.sh bench diamond 1000 1000 100 ~/stream_file.txt

## Box - dif window sizes:
#./laser.sh bench box 1000 1000 1 ~/stream_file.txt
#./laser.sh bench box 1000 1000 10 ~/stream_file.txt
#./laser.sh bench box 1000 1000 100 ~/stream_file.txt

