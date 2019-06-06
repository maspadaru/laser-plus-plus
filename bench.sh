#!/bin/bash

# input streams generated using:
# pypy gen.py 1000009 1 1 > ~/data/stream_1M_1_1.txt
# pypy gen.py 1000009 1 2 > ~/data/stream_1M_1_2.txt
# pypy gen.py 1000009 1 4 > ~/data/stream_1M_1_4.txt
# pypy gen.py 1000009 1 8 > ~/data/stream_1M_1_8.txt
# pypy gen.py 1000009 2 1 > ~/data/stream_1M_2_1.txt
# pypy gen.py 1000009 2 2 > ~/data/stream_1M_2_2.txt
# pypy gen.py 1000009 4 1 > ~/data/stream_1M_4_1.txt
# pypy gen.py 1000009 8 1 > ~/data/stream_1M_8_1.txt
# pypy gen_transitive.py 1000009 > ~/data/stream_transitive.txt

# ============================================================================
./laser.sh bbm
# ============================================================================

# I Benchmark diferent atomic formulas
#./laser.sh bench ATM1 1000 1000 0 O ~/data/stream_1M_1_2.txt  
#./laser.sh bench ATM2 1000 1000 0 O ~/data/stream_1M_1_1.txt 
#./laser.sh bench ATM3 1000 1000 0 O ~/data/stream_1M_1_2.txt  
#./laser.sh bench ATM4 1000 1000 0 O ~/data/stream_1M_1_2.txt  

# II. Benchmark Conjunction
#./laser.sh bench CON1 1000 1000 0 O ~/data/stream_1M_2_2.txt  
#./laser.sh bench CON2 10 1000 0 O ~/data/stream_1M_2_2.txt  
#./laser.sh bench CON3 10 100 0 O ~/data/stream_transitive.txt  

# III. Benchmark Diamond diferent window sizes
#./laser.sh bench p7 1000 1000 3 O ~/data/stream_1M_1_1.txt  
#./laser.sh bench p7 1000 1000 9 O ~/data/stream_1M_1_1.txt  
#./laser.sh bench p7 1000 1000 27 O ~/data/stream_1M_1_1.txt  

# IV. Benchmark Box diferent window sizes
#./laser.sh bench p8 1000 1000 3 O ~/data/stream_1M_1_1.txt  
#./laser.sh bench p8 1000 1000 9 O ~/data/stream_1M_1_1.txt  
#./laser.sh bench p8 1000 1000 27 O ~/data/stream_1M_1_1.txt  

# V. Benchmark on diferent number of simple rules
#./laser.sh bench p9 1000 1000 0 O ~/data/stream_1M_2_1.txt  
#./laser.sh bench p10 1000 1000 0 O ~/data/stream_1M_4_1.txt  
#./laser.sh bench p11 1000 1000 0 O ~/data/stream_1M_8_1.txt  

# VI. Benchmark on number of variables present in atoms
#./laser.sh bench p12 1000 1000 0 O ~/data/stream_1M_1_2.txt  
#./laser.sh bench p13 1000 1000 0 O ~/data/stream_1M_1_4.txt  
#./laser.sh bench p14 1000 1000 0 O ~/data/stream_1M_1_8.txt  

# VII. Benchmark to see influence of noise in the input stream
#./laser.sh bench p15 1000 1000 0 O ~/data/stream_1M_2_1.txt  
#./laser.sh bench p15 1000 1000 0 O ~/data/stream_1M_4_1.txt  
#./laser.sh bench p15 1000 1000 0 O ~/data/stream_1M_8_1.txt  

# 8. Benchmarking Chase Algorithms 
# 8.1. Atomic head
./laser.sh bench EX1 1000 1000 0 S ~/data/stream_1M_1_2.txt  
./laser.sh bench EX1 1000 1000 0 R ~/data/stream_1M_1_2.txt  
./laser.sh bench EX1 1000 1000 0 I ~/data/stream_1M_1_2.txt  

# 8.1. Conjunctive head 
./laser.sh bench EX2 1000 1000 0 S ~/data/stream_1M_1_2.txt  
./laser.sh bench EX2 1000 1000 0 R ~/data/stream_1M_1_2.txt  
./laser.sh bench EX3 1000 1000 0 I ~/data/stream_1M_1_2.txt  

# 8.2. Large database - no unique predicate in head
./laser.sh bench EX3 100 1000 0 S ~/data/stream_1M_2_2.txt  
./laser.sh bench EX3 100 1000 0 R ~/data/stream_1M_2_2.txt  
./laser.sh bench EX3 100 1000 0 I ~/data/stream_1M_2_2.txt  

# 8.3. Large databases - unique predicate in head 
./laser.sh bench EX4 100 1000 0 S ~/data/stream_1M_2_2.txt  
./laser.sh bench EX4 100 1000 0 R ~/data/stream_1M_2_2.txt  
./laser.sh bench EX4 100 1000 0 I ~/data/stream_1M_2_2.txt  

