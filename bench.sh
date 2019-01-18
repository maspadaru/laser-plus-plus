#!/bin/bash

./laser.sh b  

./laser.sh bench atom 1000000 100 0 
./laser.sh bench atom 10000 10000 0 
./laser.sh bench atom 100 1000000 0 

# ATOM
#./laser.sh bench atom 1000000 1 0 
#./laser.sh bench atom 100000 10 0 
#./laser.sh bench atom 10000 100 0
#./laser.sh bench atom 1000 1000 0
#./laser.sh bench atom 100 10000 0
#./laser.sh bench atom 10 100000 0
#./laser.sh bench atom 1 1000000 0

# Diamond - dif input sizes
#./laser.sh bench diamond 100000 10 3 
#./laser.sh bench diamond 1000 1000 3
#./laser.sh bench diamond 10 100000 3

# Box 
#./laser.sh bench box 100000 10 3 
#./laser.sh bench box 1000 1000 3 
#./laser.sh bench box 10 100000 3

# Conjunction
#./laser.sh bench conjunction 1000000 1 0 
#./laser.sh bench conjunction 100000 10 0 
#./laser.sh bench conjunction 10000 100 0
#./laser.sh bench conjunction 1000 1000 0
#./laser.sh bench conjunction 100 10000 0
#./laser.sh bench conjunction 10 100000 0
#./laser.sh bench conjunction 1 1000000 0

# Diamond - dif window sizes:
#./laser.sh bench diamond 1000 1000 1 
#./laser.sh bench diamond 1000 1000 10
#./laser.sh bench diamond 1000 1000 100

# Box - dif window sizes:
#./laser.sh bench box 1000 1000 1 
#./laser.sh bench box 1000 1000 10
#./laser.sh bench box 1000 1000 100
