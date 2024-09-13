#!/bin/bash

# build main.cpp
# g++ -Iinclude -Isrc -std=c++17 main.cpp -o program
# ./program

g++ -Iinclude -Isrc -std=c++17 assignment1.cpp -o assignment1

#check leak mem
valgrind --leak-check=full ./assignment1

#run program
./assignment1