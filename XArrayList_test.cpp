#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include "ArrayList.cpp" // Giả sử XArrayList của bạn được định nghĩa trong file này
#include "./include/util/Point.h"

// Hàm hỗ trợ cho chuyển đổi chuỗi
std::string intToString(int& item) {
    return std::to_string(item);
}

// Hàm hỗ trợ cho so sánh con trỏ
bool pointerEqual(int* &lhs, int* &rhs) {
    return *lhs == *rhs;
}

// Hàm hỗ trợ để xóa con trỏ int
void deleteIntPointer(XArrayList<int*>* list) {
    for(int i = 0; i < list->size(); i++) {
        delete list->get(i);
    }
}

void testInitialState() {
    XArrayList<int> list;
    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "Test Initial State: Passed" << std::endl;
}

void testAddingElements() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.size() == 3);
    assert(!list.empty());
    assert(list.get(0) == 1);
    assert(list.get(1) == 2);
    assert(list.get(2) == 3);
    std::cout << "Test Adding Elements: Passed" << std::endl;
}

void testAddingAtIndex() {
    XArrayList<int> list;
    list.add(0, 1);
    list.add(1, 3);
    list.add(1, 2);
    assert(list.size() == 3);
    assert(list.get(0) == 1);
    assert(list.get(1) == 2);
    assert(list.get(2) == 3);
    std::cout << "Test Adding At Index: Passed" << std::endl;
}

void testRemovingElements() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.removeAt(1) == 2);
    assert(list.size() == 2);
    assert(list.get(0) == 1);
    assert(list.get(1) == 3);
    std::cout << "Test Removing Elements: Passed" << std::endl;
}

void testRemoveItem() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.removeItem(2));
    assert(!list.removeItem(4));
    assert(list.size() == 2);
    assert(list.get(0) == 1);
    assert(list.get(1) == 3);
    std::cout << "Test Remove Item: Passed" << std::endl;
}

void testClear() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.clear();
    assert(list.empty());
    assert(list.size() == 0);
    std::cout << "Test Clear: Passed" << std::endl;
}

void testIndexOfAndContains() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.indexOf(2) == 1);
    assert(list.indexOf(4) == -1);
    assert(list.contains(3));
    assert(!list.contains(4));
    std::cout << "Test IndexOf and Contains: Passed" << std::endl;
}

void testToString() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.toString(intToString) == "[1, 2, 3]");
    std::cout << "Test ToString: Passed" << std::endl;
}

void testEdgeCases() {
    XArrayList<int> list;
    try {
        list.add(-1, 1);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    try {
        list.add(1, 1);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    try {
        list.removeAt(0);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    try {
        list.get(-1);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    try {
        list.get(0);
        assert(false); // Shouldn't reach here
    } catch (const std::out_of_range&) {
        // Expected
    }
    std::cout << "Test Edge Cases: Passed" << std::endl;
}

void testCapacityIncrease() {
    XArrayList<int> list;
    for (int i = 0; i < 20; i++) {
        list.add(i);
    }
    assert(list.size() == 20);
    for (int i = 0; i < 20; i++) {
        assert(list.get(i) == i);
    }
    std::cout << "Test Capacity Increase: Passed" << std::endl;
}

void testWithPointers() {
    XArrayList<int*> list(deleteIntPointer, pointerEqual);
    int* p1 = new int(1);
    int* p2 = new int(2);
    int* p3 = new int(3);
    list.add(p1);
    list.add(p2);
    list.add(p3);
    assert(list.size() == 3);
    assert(*list.get(0) == 1);
    assert(*list.get(1) == 2);
    assert(*list.get(2) == 3);
    int* removed = list.removeAt(1);
    assert(*removed == 2);
    delete removed;
    assert(list.size() == 2);
    list.clear();
    assert(list.empty());
    std::cout << "Test With Pointers: Passed" << std::endl;
}

void testCopyConstructorAndAssignment() {
    XArrayList<int> list1;
    list1.add(1);
    list1.add(2);
    list1.add(3);
    
    // Copy constructor
    XArrayList<int> list2(list1);
    assert(list2.size() == 3);
    assert(list2.get(0) == 1);
    assert(list2.get(1) == 2);
    assert(list2.get(2) == 3);
    list2.add(4);
    assert(list2.size() == 4);
    assert(list1.size() == 3);
    
    // Assignment operator
    XArrayList<int> list3;
    list3 = list1;
    assert(list3.size() == 3);
    assert(list3.get(0) == 1);
    assert(list3.get(1) == 2);
    assert(list3.get(2) == 3);
    list3.add(4);
    assert(list3.size() == 4);
    assert(list1.size() == 3);
    
    std::cout << "Test Copy Constructor and Assignment: Passed" << std::endl;
}

void testIterator() {
    XArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    
    auto it = list.begin();
    assert(*it == 1);
    ++it;
    assert(*it == 2);
    ++it;
    assert(*it == 3);
    // ++it;
    // assert(it == list.end());
    
    it = list.begin();
    ++it; // Point to second element
    it.remove();
    assert(list.size() == 2);
    assert(list.get(0) == 1);
    assert(list.get(1) == 3);
    
    std::cout << "Test Iterator: Passed" << std::endl;
}
bool almostEqual(float a, float b, float epsilon = EPSILON) {
    return std::fabs(a - b) < epsilon;
}

void testXArrayListAddWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    list.add(Point(7.0f, 8.0f, 9.0f));
    
    assert(list.size() == 3);
    assert(list.get(0) == Point(1.0f, 2.0f, 3.0f));
    assert(list.get(1) == Point(4.0f, 5.0f, 6.0f));
    assert(list.get(2) == Point(7.0f, 8.0f, 9.0f));
    
    list.add(1, Point(10.0f, 11.0f, 12.0f));
    
    assert(list.size() == 4);
    assert(list.get(1) == Point(10.0f, 11.0f, 12.0f));
    
    std::cout << "Test XArrayList Add with Points: Passed" << std::endl;
}

void testXArrayListRemoveWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    list.add(Point(7.0f, 8.0f, 9.0f));
    
    Point removed = list.removeAt(1);
    assert(removed == Point(4.0f, 5.0f, 6.0f));
    assert(list.size() == 2);
    assert(list.get(1) == Point(7.0f, 8.0f, 9.0f));
    
    bool result = list.removeItem(Point(1.0f, 2.0f, 3.0f));
    assert(result);
    assert(list.size() == 1);
    assert(list.get(0) == Point(7.0f, 8.0f, 9.0f));
    
    result = list.removeItem(Point(10.0f, 11.0f, 12.0f));
    assert(!result);
    
    std::cout << "Test XArrayList Remove with Points: Passed" << std::endl;
}

void testXArrayListContainsWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    list.add(Point(7.0f, 8.0f, 9.0f));
    
    assert(list.contains(Point(1.0f, 2.0f, 3.0f)));
    assert(list.contains(Point(4.0f, 5.0f, 6.0f)));
    assert(list.contains(Point(7.0f, 8.0f, 9.0f)));
    assert(!list.contains(Point(10.0f, 11.0f, 12.0f)));
    
    std::cout << "Test XArrayList Contains with Points: Passed" << std::endl;
}

void testXArrayListIndexOfWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    list.add(Point(7.0f, 8.0f, 9.0f));
    
    assert(list.indexOf(Point(1.0f, 2.0f, 3.0f)) == 0);
    assert(list.indexOf(Point(4.0f, 5.0f, 6.0f)) == 1);
    assert(list.indexOf(Point(7.0f, 8.0f, 9.0f)) == 2);
    assert(list.indexOf(Point(10.0f, 11.0f, 12.0f)) == -1);
    
    std::cout << "Test XArrayList IndexOf with Points: Passed" << std::endl;
}

void testXArrayListClearWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    list.add(Point(7.0f, 8.0f, 9.0f));
    
    assert(list.size() == 3);
    
    list.clear();
    
    assert(list.empty());
    assert(list.size() == 0);
    
    std::cout << "Test XArrayList Clear with Points: Passed" << std::endl;
}

void testXArrayListToStringWithPoints() {
    XArrayList<Point> list;
    
    list.add(Point(1.0f, 2.0f, 3.0f));
    list.add(Point(4.0f, 5.0f, 6.0f));
    
    std::string result = list.toString(Point::point2str);
    assert(result == "[P(  1.00;  2.00;  3.00), P(  4.00;  5.00;  6.00)]");
    
    std::cout << "Test XArrayList ToString with Points: Passed" << std::endl;
}

void testXArrayListWithPointPointers() {
    XArrayList<Point*> list(&XArrayList<Point*>::free, Point::pointEQ);
    
    list.add(new Point(1.0f, 2.0f, 3.0f));
    list.add(new Point(4.0f, 5.0f, 6.0f));
    list.add(new Point(7.0f, 8.0f, 9.0f));
    
    assert(list.size() == 3);
    assert(*list.get(0) == Point(1.0f, 2.0f, 3.0f));
    
    Point* toRemove = new Point(4.0f, 5.0f, 6.0f);
    bool removed = list.removeItem(toRemove);
    assert(removed);
    assert(list.size() == 2);
    delete toRemove;
    
    Point* toFind = new Point(7.0f, 8.0f, 9.0f);
    assert(list.contains(toFind));
    delete toFind;
    
    list.clear();
    assert(list.empty());
    
    std::cout << "Test XArrayList with Point Pointers: Passed" << std::endl;
}
int main() {
    testInitialState();
    testAddingElements();
    testAddingAtIndex();
    testRemovingElements();
    testRemoveItem();
    testClear();
    testIndexOfAndContains();
    testToString();
    testEdgeCases();
    testCapacityIncrease();
    testWithPointers();
    testCopyConstructorAndAssignment();
    testIterator();
    testXArrayListAddWithPoints();
    testXArrayListRemoveWithPoints();
    testXArrayListContainsWithPoints();
    testXArrayListIndexOfWithPoints();
    testXArrayListClearWithPoints();
    testXArrayListToStringWithPoints();
    testXArrayListWithPointPointers();
    
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
