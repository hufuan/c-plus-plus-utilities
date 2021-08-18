#!/bin/bash
if [ -f nstest ]; then
  rm nstest
fi
g++ -o nstest nstest.cc  -lpthread -lrt -lstdc++
if [ ! -d "logs" ]; then
  mkdir logs 
fi
