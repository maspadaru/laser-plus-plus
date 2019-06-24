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

def main():
    random.seed()
    for i in range(1000):
        pred_id = random.randint(1, 5)
        if (pred_id == 1):
            vect = gen_const_vect(3)
            pred = 'p'
        elif(pred_id == 2):
            vect = gen_const_vect(2)
            pred = 'q'
        elif(pred_id == 3):
            vect = gen_const_vect(2)
            pred = 'r'
        elif(pred_id == 4):
            vect = gen_const_vect(4)
            pred = 's'
        elif(pred_id == 5):
            vect = gen_const_vect(1)
            pred = 't'
        var = ' '.join(vect)
        print("%s %s" % (pred, var))


if __name__ == '__main__':
    main()
