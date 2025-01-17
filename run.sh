#!/bin/bash

# build main.cpp
# g++ -Iinclude -Isrc -std=c++17 main.cpp -o program
# ./program

# $PROGRAM = "ArrayList.cpp"

# PROGRAM = "DLinkedList.cpp"

# echo "-------------------------------------------"
# echo "----------DOUBLY LINKED LIST---------------"
# g++ -Iinclude -Isrc -std=c++17 DLinkedList.cpp -o assignment1
# g++ -Iinclude -Isrc -std=c++17 DLinkedList_test.cpp -o assignment1


# echo "-------------------------------------------"
# echo "--------------ARRAY LIST-------------------"
# g++ -Iinclude -Isrc -std=c++17 ArrayList.cpp -o assignment1
# g++ -Iinclude -Isrc -std=c++17 IListDemo.cpp -o assignment1
# g++ -Iinclude -Isrc -std=c++17 XArrayList_test.cpp -o assignment1


echo "-------------------------------------------"
echo "-----------------MAIN----------------------"
g++ -Iinclude -Isrc -std=c++17 main.cpp -o assignment1

#check leak mem

echo "-------------------------------------------"
echo "------------CHECK LEAK MEM-----------------"
echo "-------------------------------------------"
echo ""
valgrind --track-origins=no  --leak-check=full ./assignment1 > /dev/null

#run program
echo ""
echo "-------------------------------------------"
echo "-------------RUN PROGRAM-------------------"
echo "-------------------------------------------"
echo ""
./assignment1
