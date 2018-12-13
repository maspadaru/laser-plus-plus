#!/bin/bash

./laser.sh release  

# ATOM
./laser.sh benchscript atom 1000000 1 0 
./laser.sh benchscript atom 100000 10 0 
./laser.sh benchscript atom 10000 100 0
./laser.sh benchscript atom 1000 1000 0
./laser.sh benchscript atom 100 10000 0
./laser.sh benchscript atom 10 100000 0
./laser.sh benchscript atom 1 1000000 0

# Diamond - dif input sizes
./laser.sh benchscript diamond 100000 10 3 
./laser.sh benchscript diamond 1000 1000 3
./laser.sh benchscript diamond 10 100000 3

# Box 
./laser.sh benchscript box 100000 10 3 
./laser.sh benchscript box 1000 1000 3 
./laser.sh benchscript box 10 100000 3

# Conjunction
./laser.sh benchscript conjunction 1000000 1 0 
./laser.sh benchscript conjunction 100000 10 0 
./laser.sh benchscript conjunction 10000 100 0
./laser.sh benchscript conjunction 1000 1000 0
./laser.sh benchscript conjunction 100 10000 0
./laser.sh benchscript conjunction 10 100000 0
./laser.sh benchscript conjunction 1 1000000 0

# Diamond - dif window sizes:
./laser.sh benchscript diamond 1000 1000 1 
./laser.sh benchscript diamond 1000 1000 10
./laser.sh benchscript diamond 1000 1000 100

# Box - dif window sizes:
./laser.sh benchscript box 1000 1000 1 
./laser.sh benchscript box 1000 1000 10
./laser.sh benchscript box 1000 1000 100
