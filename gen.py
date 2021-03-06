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

NUMBER_OF_TUPLES = 100000009

def gen_const():
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(2))

def main():
    random.seed()

    for i in range(1,NUMBER_OF_TUPLES):
        pred = random.randint(1, 6)
        if (pred == 1):
            print("%s %s %s %s" % ('a', gen_const(), gen_const(), gen_const() ))
        elif(pred == 2):
            print("%s %s %s" % ('b', gen_const(), gen_const() ))
        elif(pred == 3):
            print("%s %s %s" % ('c', gen_const(), gen_const() ))
        elif(pred == 4):
            print("%s %s %s" % ('d', gen_const(), gen_const() ))
        elif(pred == 5):
            print("%s %s %s" % ('e', gen_const(), gen_const() ))
        elif(pred == 6):
            print("%s %s" % ('f', gen_const()))


if __name__ == '__main__':
    main()
