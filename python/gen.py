import random
import string
import sys
import os

def gen_const():
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(2))

def main():
    random.seed()

    for i in range(1,10000):
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
