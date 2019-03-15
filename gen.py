'''
This script generates some random tuples that can be used for benchmarking. 
The total number of tuples generated can be set by modifying NUMBER_OF_TUPLES.
Each tuple has a random predicate from the set: ('a', 'b', 'c', 'd', 'e', 'f').
Each tuple has a number of variables determined by its predicate. 
The generated tuples are printed to stdin, separated by a new line character.
In order to generate a file useful for benchmarking, redirect this output to 
a file.
Example use:
    python gen.py > input.txt
'''

import random
import string
import sys
import os

def gen_const():
    return ''.join(random.choice(string.digits) for _ in range(2))

def gen_const_vect(num_var):
    vect = []
    for i in range(num_var):
        vect.append(gen_const())
    return vect

def gen_stream(num_fact, num_pred, num_var):
    random.seed()
    for i in range(num_fact):
        pred_id = random.randint(1, num_pred)
        if (pred_id == 1):
            vect = gen_const_vect(num_var)
            var = ' '.join(vect)
            pred = 'p'
        elif(pred_id == 2):
            var = gen_const()
            pred = 'q'
        elif(pred_id == 3):
            var = gen_const()
            pred = 'r'
        elif(pred_id == 4):
            var = gen_const()
            pred = 's'
        elif(pred_id == 5):
            var = gen_const()
            pred = 't'
        elif(pred_id == 6):
            var = gen_const()
            pred = 'u'
        elif(pred_id == 7):
            var = gen_const()
            pred = 'v'
        elif(pred_id == 8):
            var = gen_const()
            pred = 'w'
        print("%s %s" % (pred, var))

def main():
    if (len(sys.argv) < 4):
        print ('Usage: python gen.py total_fact_count num_pred num_var_of_first_pred')
    else:
        # TODO check sizes argv[2] < 8, etc
        gen_stream(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))


if __name__ == '__main__':
    main()
