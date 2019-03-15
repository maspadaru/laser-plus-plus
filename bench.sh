#!/bin/bash

# input streams generated using:
# pypy gen.py 100000009 1 1 > ~/stream_100M_1_1.txt
# pypy gen.py 100000009 1 2 > ~/stream_100M_1_2.txt
# pypy gen.py 100000009 1 4 > ~/stream_100M_1_4.txt
# pypy gen.py 100000009 1 8 > ~/stream_100M_1_8.txt
# pypy gen.py 100000009 2 1 > ~/stream_100M_2_1.txt
# pypy gen.py 100000009 2 2 > ~/stream_100M_2_2.txt
# pypy gen.py 100000009 4 1 > ~/stream_100M_4_1.txt
# pypy gen.py 100000009 8 1 > ~/stream_100M_8_1.txt

# I Benchmark diferent formulas
./laser.sh bench p1 1000 1000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p2 1000 1000 0 ~/stream_100M_1_1.txt 
./laser.sh bench p3 1000 1000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p4 1000 1000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p5 1000 1000 0 ~/stream_100M_2_2.txt  
./laser.sh bench p6 1000 1000 0 ~/stream_100M_2_2.txt  
./laser.sh bench p7 1000 1000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p8 1000 1000 3 ~/stream_100M_1_1.txt  

# II Benchmark Diferent input shapes
./laser.sh bench p1 10 100000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p1 1000 1000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p1 100000 10 0 ~/stream_100M_1_2.txt  

./laser.sh bench p5 10 100000 0 ~/stream_100M_2_2.txt  
./laser.sh bench p5 1000 1000 0 ~/stream_100M_2_2.txt  
./laser.sh bench p5 100000 10 0 ~/stream_100M_2_2.txt  

./laser.sh bench p7 10 100000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p7 1000 1000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p7 100000 10 3 ~/stream_100M_1_1.txt  

./laser.sh bench p8 10 100000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p8 1000 1000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p8 100000 10 3 ~/stream_100M_1_1.txt  

# III Benchmark diferent window sizes
./laser.sh bench p7 1000 1000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p7 1000 1000 9 ~/stream_100M_1_1.txt  
./laser.sh bench p7 1000 1000 27 ~/stream_100M_1_1.txt  

./laser.sh bench p8 1000 1000 3 ~/stream_100M_1_1.txt  
./laser.sh bench p8 1000 1000 9 ~/stream_100M_1_1.txt  
./laser.sh bench p8 1000 1000 27 ~/stream_100M_1_1.txt  

# IV Benchmark on diferent number of simple rules
./laser.sh bench p9 1000 1000 0 ~/stream_100M_2_1.txt  
./laser.sh bench p10 1000 1000 0 ~/stream_100M_4_1.txt  
./laser.sh bench p11 1000 1000 0 ~/stream_100M_8_1.txt  

# V Benchmark on number of variables present in atoms
./laser.sh bench p12 1000 1000 0 ~/stream_100M_1_2.txt  
./laser.sh bench p13 1000 1000 0 ~/stream_100M_1_4.txt  
./laser.sh bench p14 1000 1000 0 ~/stream_100M_1_8.txt  

# VI Benchmark to see influence of noise in the input stream
./laser.sh bench p15 1000 1000 0 ~/stream_100M_2_1.txt  
./laser.sh bench p15 1000 1000 0 ~/stream_100M_4_1.txt  
./laser.sh bench p15 1000 1000 0 ~/stream_100M_8_1.txt  

