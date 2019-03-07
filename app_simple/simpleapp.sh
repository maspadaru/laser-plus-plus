#!/bin/bash
../cmake-build-release/simpleapp samples/program_simple.laser samples/simple.stream samples/out.txt
cat samples/out.txt
