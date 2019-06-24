#!/bin/bash

python gen_stream.py > ~/samples/simple.stream
../cmake-build-release/demoapp-build/demoapp

