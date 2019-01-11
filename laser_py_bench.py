import sys
import json
import timeit
import time

from stream.file_stream import FileStream
from evalunit.program import Program

END_TIME = 1000000
NFACTS = 1
OUTPUT = False 

def run_atom(end_time, facts_per_timepoint, window_size):
    print ("Starting run Atom")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "t(X,Y) :- d(X,Y)",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def run_diamond(end_time, facts_per_timepoint, window_size):
    print ("Starting run Diamond")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "t(X, Y)   :- time_win(" + str(window_size) + ",0,1,diamond(d(X, Y)))",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def run_conjunction(end_time, facts_per_timepoint, window_size):
    print ("Starting run Conjunction")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "t(X, Y) :- d(X,Y) and f(X)",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def run_box(end_time, facts_per_timepoint, window_size):
    print ("Starting run Box")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "t(X) :- time_win( " + str(window_size) + ", 0, 1, box(f(X)))",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def run_time_reference(end_time, facts_per_timepoint, window_size):
    print ("Starting run Time Reference")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "t(X, Y, T)   :- @(T, d(X, Y))",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def run_multi_atom(end_time, facts_per_timepoint, window_size):
    print ("Starting run Multi Atoms")
    print("timepoints: %d, facts_per_timepoint: %d, win_size: %d" % (end_time, \
            facts_per_timepoint, window_size))
    start_time = 0 
    is_output_enabled = OUTPUT 
    s = FileStream("/home/mike/stream_file.txt", facts_per_timepoint, start_time, end_time)

    rules = [
        "q(X, Y, Z)   :- a(X, Y, Z)",
        "r(Y) :- c (X, Y)",
        "t(Y,X) :- d(X,Y)",
        "u(X,X) :- f(X)",
        "v(X, Y, X, Y) :- e(X,Y)",
    ]

    prog = Program(rules, s)
        
    start = time.time()
    for t in range(start_time, end_time + 1):
            res, tuples = prog.evaluate(t)
            if (is_output_enabled):
                print(tuples)
    end = time.time()
    elapsed_secs = end - start
    print("Elapsed time (sec) = %f" % elapsed_secs )
    print("************************************************************")
    print("")
    print("")

def main():
    run_atom(1000000, 1, 0)
    run_atom(100000, 10, 0)
    run_atom(10000, 100, 0)
    run_atom(1000, 1000, 0)
    run_atom(100, 10000, 0)
    run_atom(10, 100000, 0)
    run_atom(1, 1000000, 0)

    run_diamond(100000, 10, 5)
    run_diamond(1000, 1000, 5)
    run_diamond(10, 100000, 5)

    run_box(100000, 10, 5)
    run_box(1000, 1000, 5)
    run_box(10, 100000, 5)

    run_conjunction(1000000, 1, 0)
    run_conjunction(100000, 10, 0)
    run_conjunction(10000, 100, 0)
    run_conjunction(1000, 1000, 0)
    run_conjunction(100, 10000, 0)
    run_conjunction(10, 100000, 0)
    run_conjunction(1, 1000000, 0)

    run_diamond(1000, 1000, 1)
    run_diamond(1000, 1000, 10)
    run_diamond(1000, 1000, 100)

    run_box(1000, 1000, 1)
    run_box(1000, 1000, 10)
    run_box(1000, 1000, 100)


if __name__ == '__main__':
    main()
