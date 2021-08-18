#!/bin/bash
rm ace_compile_test
g++ -o ace_compile_test ace_compile_test.cc  -lpthread -lrt -lstdc++
