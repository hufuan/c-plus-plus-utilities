#!/bin/bash
rm nsDuration
g++ -o nsDuration ns_duration_test.cc  -lpthread -lrt -lstdc++
