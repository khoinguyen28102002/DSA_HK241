#!/bin/bash

# build main.cpp
# g++ -Iinclude -Isrc -std=c++17 main.cpp -o program
# ./program

g++ -Iinclude -Isrc -std=c++17 assignment1.cpp -o assignment1

#check leak mem

echo "-------------------------------------------"
echo "------------CHECK LEAK MEM-----------------"
echo "-------------------------------------------"
echo ""
valgrind --track-origins=no  --leak-check=summary ./assignment1 > /dev/null

#run program
echo ""
echo "-------------------------------------------"
echo "-------------RUN PROGRAM-------------------"
echo "-------------------------------------------"
echo ""
./assignment1
