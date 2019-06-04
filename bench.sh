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
# pypy gen_transitive.py 100000009 > ~/stream_transitive.txt

# ============================================================================

# I Benchmark diferent atomic formulas
#./laser.sh bench p1 1000 1000 0 O ~/stream_100M_1_2.txt  
#./laser.sh bench p2 1000 1000 0 O ~/stream_100M_1_1.txt 
#./laser.sh bench p3 1000 1000 0 O ~/stream_100M_1_2.txt  
#./laser.sh bench p4 1000 1000 0 O ~/stream_100M_1_2.txt  

# II. Benchmark Conjunction
# ! p20 crashes (OOM) for 1000 facts 
./laser.sh bench CON1 10 10000 0 O ~/stream_100M_2_2.txt  
./laser.sh bench CON2 10 1000 0 O ~/stream_100M_2_2.txt  
./laser.sh bench TRAN 10 100 0 O ~/stream_transitive.txt  

# III. Benchmark Diamond on diferent input shapes
#./laser.sh bench p7 10 100000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p7 1000 1000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p7 100000 10 3 O ~/stream_100M_1_1.txt  

# IV. Benchmark Diamond diferent window sizes
#./laser.sh bench p7 1000 1000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p7 1000 1000 9 O ~/stream_100M_1_1.txt  
#./laser.sh bench p7 1000 1000 27 O ~/stream_100M_1_1.txt  

# IV. Benchmark Box on diferent input shapes
#./laser.sh bench p8 10 100000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p8 1000 1000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p8 100000 10 3 O ~/stream_100M_1_1.txt  

# V. Benchmark Box diferent window sizes
#./laser.sh bench p8 1000 1000 3 O ~/stream_100M_1_1.txt  
#./laser.sh bench p8 1000 1000 9 O ~/stream_100M_1_1.txt  
#./laser.sh bench p8 1000 1000 27 O ~/stream_100M_1_1.txt  

# VI. Benchmark on diferent number of simple rules
#./laser.sh bench p9 1000 1000 0 O ~/stream_100M_2_1.txt  
#./laser.sh bench p10 1000 1000 0 O ~/stream_100M_4_1.txt  
#./laser.sh bench p11 1000 1000 0 O ~/stream_100M_8_1.txt  

# VII. Benchmark on number of variables present in atoms
#./laser.sh bench p12 1000 1000 0 O ~/stream_100M_1_2.txt  
#./laser.sh bench p13 1000 1000 0 O ~/stream_100M_1_4.txt  
#./laser.sh bench p14 1000 1000 0 O ~/stream_100M_1_8.txt  

# VIII. Benchmark to see influence of noise in the input stream
#./laser.sh bench p15 1000 1000 0 O ~/stream_100M_2_1.txt  
#./laser.sh bench p15 1000 1000 0 O ~/stream_100M_4_1.txt  
#./laser.sh bench p15 1000 1000 0 O ~/stream_100M_8_1.txt  

# IX. Benchmark cahse algorithms on single complex rule
#./laser.sh bench EX1 1000 1000 0 S ~/stream_100M_1_2.txt  
#./laser.sh bench EX1 1000 1000 0 R ~/stream_100M_1_2.txt  
#./laser.sh bench EX1 1000 1000 0 I ~/stream_100M_1_2.txt  

# X Benchmark chase performance on large databases
# ! Skolem chase will not terminate in this case
#./laser.sh bench EX2 10 10 2 R ~/stream_100M_4_1.txt  
#./laser.sh bench EX2 10 10 2 I ~/stream_100M_4_1.txt  
