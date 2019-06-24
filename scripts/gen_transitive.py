'''
This script generates some random tuples that can be used for benchmarking. 
Example use:
    python gen.py > input.txt
'''

import random
import string
import sys
import os


def gen_const_vect(current):
    vect = []
    vect.append(str(current))
    vect.append(str(current+1))
    return vect

def gen_stream(num_fact):
        current = 0
        for i in range(num_fact):
            vect = gen_const_vect(current)
            current += 1
            var = ' '.join(vect)
            pred = 'p'
            print("%s %s" % (pred, var))
            if (current >= 100000):
                current = 0

def main():
    if (len(sys.argv) < 1):
        print ('Usage: python gen.py total_fact_count')
    else:
        gen_stream(int(sys.argv[1]))


if __name__ == '__main__':
    main()
