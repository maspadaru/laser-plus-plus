# Laser++ 

**Laser++** is a C++ implementation of the [Laser](https://research.vu.nl/en/publications/expressive-stream-reasoning-with-laser) a stream reasoning engine 
based on the [LARS logic framework](http://www.kr.tuwien.ac.at/research/projects/dhsr/pub/2015/bdef2015-aaai.pdf)   

### Prerequisites
Laser++ requires:
* C++ compiler compliant with C++17 standard (tested on GCC 7.3.0 and 
Clang 6.0.0)
* GTest suite (tested on version 1.8.0)
* CMake (minimum version: 3.7, tested on 3.10)
* gdb (optional)

## Getting Started

Interact with Laser++ by using the `laser.sh` script. 

Build and test Laser++ by executing:
```
./laser.sh r 
```

Run all test cases by executing:
```
./laser.sh t 
```

See all options by running:
```
./laser.sh h 
```

### Quick start with Sample reasoning application

The Laser++ source-code includes **Simpleapp**, a Laser++-based application that 
is meant as a demo. It can be used to test the Laser++ reasoning
engine on programs and input streams provided in text files. To run this 
application first compile Laser++ as well as simpleapp by
entering the application directory `cd app_simple` and running `./simpleapp.sh`.
This will compile the application and run all the sample programs.
You can run other programs and streams by passing them as parameters to Simpleapp:
```
cmake-build-release/simpleapp CHASE_ALGORITHM PROGRAM_PATH INPUT_PATH OUTPUT_PATH
```
The three arguments represent the paths to the respective input and output files.
The Chase Algorithm parameter can be either *S* for Skolem Chase, or *R* for 
Restricted Chase.
The source code of *Simpleapp* can be found in *app-simple/main.cpp*, as well as
the sub directories *include* and *src* of the *app-simple* directory. Some 
sample programs and inputs can be found in *app-simple/samples/*.

Example execution:
```
cmake-build-release/simpleapp R app_simple/samples/bicycle.laser app_simple/samples/bicycle.stream output.txt
```

**Program**

Programs are composed of one or more rules, separated by new-line character '\n'.

Each rule is written as: `head := body`. Both *head* and *body* are formulas. 

The simplest type of formula is an Atom, which is composed of a predicate and
some variables, e.g.: `a(X, Y, X)`. In this atom, *a* is the predicate, while 
*X*, *Y*, and *Z* are variables.

More complex formulas can be represented by adding a unary operator in front of another
formula, or a binary operator between two formulas. The result is an extended
formula. These operators are written as: 
```
Diamond: [D]
Box: [B]
Time Window: [$, size]
Tuple Window: [#, size]
Time reference: [@, time_variable]
Conjunction: &&
```
Please note that the head of a Laser++ rule is limited to conjunctions of atomic 
and time reference formulas.
Example program:
```
u(X) := [#, 1][D]f(X)
f(Y) := [$, 5] [B] e(Y, Y)
a(T, Z) := [@, T] b(Z)
[@, T]c(S) := [@, T] d(S)
i(V, W) := g(W) && h(V)
```

**Input Stream**

The file containing the input stream needs to start with a line containing two 
numbers: the first and last time-point in the stream.
Each line after has the following format: `time-point : facts`. Facts are grounded
atoms separated by commas. Time-points should be in order.
Example input stream:
```1 14 "
1 : q(1), p(1), e(a4, b6)
2 : p(2), a(3, 1, 8)
3 : 
4 : p(41), q(1) 
4 : p(8)
5 : a(4, 8, 12)
12 : a(56, 99, 0), p(1)
```

### Project organization
The code of Laser++ can be found in directories **include** (header files) and **src** 
(C++ source files). The project is divided in 4 modules (*core*, *rule*, *forumula*, 
*util*). Each module can be found in a dedicated sub-directory of *include*
and *src* respectively. The sub-directory *example* contains implementations of 
*rule::RuleReader* and *core::IOManager* interfaces. 

The directory **test** contains contains some test cases which can be found in 
*test/testcase/*. The directory also contains and application that runs some 
hardcoded programs on hard-coded input. The source-code of this application can 
be found in *test/main.cpp*.

### Laser++ modules:

**Core** - handles input-output as well as execution of the Laser++ program. 
References modules: Rule, Util.

**Rule** - parses laser programs and evaluates the rules on the input facts
provided by the stream. References modules: Formula, Util.

**Formula** - Evaluates the different formulas found in the program and stores
the result. References module Util.

**Util** - Contains classes useful in other modules such as exceptions, timeline.
Instances of the Grounding class present in this module are used as the basic 
block of data throughout the Laser++ reasoning engine. It is used to represent
such data as input facts, output conclusions or store annotated groudings of 
formulas present in the program being evaluated. 

### Implementing reasoner applications using the  Laser++ engine

Laser++ is a reasoning engine that can be used to implement systems capable of
real-time semantic inference on stream of data. 

In order to implement a reasoning application, the user need only to implement the 
`laser::core::IOManager` and `laser::rule::RuleReader` interfaces found in 
*include/core/io_manager.h* and *include/rule/rule_reader.h*. This 
implementation should reflect the application specific needs regarding reading
and parsing of the input stream, as well as formatting and writing of the output
stream. An example of such an implementation can be found in the folder 
*include/example* and *src/example*.  

Once the *IOManager* and *RuleReader* interfaces are implemented, 
an application can use the Laser++
reasoning engine by constructing an instance of the `laser::core::Reasoner` class
found in *include/core/reasoner.h* and *src/core/reasoner.cpp* respectively.
An example application can be found in *app-sample/main.cpp*

### Debuging

Laser++ comes with a testing application. 
The source code for this application can be found in the directory 
*test/* and can be executed by running:
```
./laser.sh r 
```

The test application runs a series of hardcoded Laser programs on hardcoded input.
This is useful for debuging, as a specific Laser program can be run in GDB. 
Simply edit the code in *test/main.cpp*, build and execute:
```
./laser.sh d 
```
