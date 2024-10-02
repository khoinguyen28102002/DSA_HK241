#include <iostream>
#include <cassert>
#include <sstream>
#include "DLinkedList.cpp" // Make sure this path is correct
#include "./include/util/Point.h"
using namespace std;

// Helper function to convert int to string
string intToString(int& value) {
    return to_string(value);
}

string LpointPtr2Str(Point*& ptr){
    stringstream os;
    os << "("   << ptr->getX() << ", " 
                << ptr->getY()
       << ")";
    return os.str();
}

// Test function
void runIntTests1() {
    // Test constructor and basic operations
    DLinkedList<int> list;
    assert(list.empty());
    assert(list.size() == 0);
    
    // Test add and size
    list.add(1);
    list.add(2);
    list.add(3);
    assert(list.size() == 3);
    assert(!list.empty());
    
    // Test get
    assert(list.get(0) == 1);
    assert(list.get(1) == 2);
    assert(list.get(2) == 3);
    
    // Test add at index
    list.add(0, 0);
    list.add(2, 5);
    assert(list.size() == 5);
    assert(list.get(0) == 0);
    assert(list.get(2) == 5);
    
    // Test removeAt
    assert(list.removeAt(2) == 5);
    assert(list.size() == 4);
    
    // Test indexOf and contains
    assert(list.indexOf(2) == 2);
    assert(list.contains(3));
    assert(!list.contains(5));
    
    // Test removeItem
    assert(list.removeItem(3));
    assert(!list.contains(3));
    assert(list.size() == 3);
    
    // Test clear
    list.clear();
    assert(list.empty());
    assert(list.size() == 0);
    
    // Test copy constructor
    DLinkedList<int> list1;
    list1.add(1);
    list1.add(2);
    list1.add(3);
    DLinkedList<int> list2(list1);
    assert(list2.size() == 3);
    assert(list2.get(0) == 1);
    assert(list2.get(1) == 2);
    assert(list2.get(2) == 3);
    
    // Test assignment operator
    DLinkedList<int> list3;
    list3 = list1;
    assert(list3.size() == 3);
    assert(list3.get(0) == 1);
    assert(list3.get(1) == 2);
    assert(list3.get(2) == 3);
    
    // Test toString
    list1.clear();
    list1.add(1);
    list1.add(2);
    list1.add(3);
    assert(list1.toString(intToString) == "[1, 2, 3]");
    
    // Test iterator
    DLinkedList<int>::Iterator it;
    int sum = 0;
    for(it = list1.begin(); it != list1.end(); it++) {
        sum += *it;
    }
    assert(sum == 6);
    
    // Test removing through iterator
    it = list1.begin();
    it.remove();
    assert(list1.size() == 2);
    assert(list1.get(0) == 2);
    
    // Test exception handling
    bool exceptionCaught = false;
    try {
        list1.get(5); // This should throw an out_of_range exception
    } catch (const out_of_range& e) {
        exceptionCaught = true;
    }
    assert(exceptionCaught);
    
    cout << "All tests passed successfully!" << endl;
}

template <typename T>
class TestableDLinkedList : public DLinkedList<T> {
public:
    void testCopyFrom(const DLinkedList<T>& other) {
        this->copyFrom(other);
    }

    void testRemoveInternalData() {
        this->removeInternalData();
    }

    typename DLinkedList<T>::Node *testGetPreviousNodeOf(int index) {
        return this->getPreviousNodeOf(index);
    }
};

void runIntTests2() {
    TestableDLinkedList<int> list1;

    // Test add(T e) - Basic appending test
    list1.add(1);
    list1.add(2);
    list1.add(3);
    std::cout << "Test add(T e): " << list1.toString() << " (Expected: [1, 2, 3])" << std::endl;

    // Test indexOf
    std::cout << "Test indexOf(2): " << list1.indexOf(2) << " (Expected: 1)" << std::endl;
    std::cout << "Test indexOf(4): " << list1.indexOf(4) << " (Expected: -1)" << std::endl;

    // Test copy constructor
    TestableDLinkedList<int> list2(list1);
    std::cout << "Test copy constructor: " << list2.toString() << " (Expected: [1, 2, 3])" << std::endl;

    // Test assignment operator
    TestableDLinkedList<int> list3;
    list3 = list1;
    std::cout << "Test assignment operator: " << list3.toString() << " (Expected: [1, 2, 3])" << std::endl;

    // Test removeInternalData
    list1.testRemoveInternalData();
    std::cout << "Test removeInternalData: " << list1.toString() << " (Expected: [])" << std::endl;

    // Test getPreviousNodeOf
    list1.add(4);
    list1.add(5);
    list1.add(6);
    // list1.add(7);
    std::cout << "Test getPreviousNodeOf(2): " << list1.testGetPreviousNodeOf(2)->data << " (Expected: 5)" << std::endl;

    // Test copyFrom
    TestableDLinkedList<int> list4;
    list4.testCopyFrom(list1);
    std::cout << "Test copyFrom: " << list4.toString() << " (Expected: [4, 5, 6])" << std::endl;

    // Rest of previous tests...
    list1.clear();
    list1.add(1);
    list1.add(2);
    list1.add(3);
    list1.add(1); // Adding duplicate for testing
    std::cout << "Test removeItem(1): " << list1.toString() << " (Expected: [1, 2, 3, 1])" << std::endl;

    // Check empty and size
    std::cout << "Test empty(): " << (list1.empty() ? "true" : "false") << " (Expected: false)" << std::endl;
    std::cout << "Test size(): " << list1.size() << " (Expected: 3)" << std::endl;

    // Test clear
    list1.clear();
    std::cout << "Test clear(): " << list1.toString() << " (Expected: [])" << std::endl;
    std::cout << "Test empty() after clear: " << (list1.empty() ? "true" : "false") << " (Expected: true)" << std::endl;

    // Edge case: removing from empty list
    try {
        list1.removeAt(0);  // Should throw out_of_range exception
    } catch (std::out_of_range &e) {
        std::cout << "Test removeAt(0) on empty list: Passed (Expected: Exception caught)" << std::endl;
    }
    // list1.removeAt(0);  

    // Large list
    for (int i = 0; i < 1000; i++) {
        list1.add(i);
    }
    std::cout << "Test large list size: " << list1.size() << " (Expected: 1000)" << std::endl;

    list1.clear();
    std::cout << "Test clear large list: " << list1.size() << " (Expected: 0)" << std::endl;
}

void runPointTests() {
    DLinkedList<Point*> pointList(&DLinkedList<Point*>::free, &Point::pointEQ);
    

    // Test add and size
    pointList.add(new Point(1, 2, 3));
    pointList.add(new Point(4, 5, 6));
    pointList.add(new Point(7, 8, 9));
    assert(pointList.size() == 3);
    assert(!pointList.empty());

    // Test get
    assert(*pointList.get(0) == Point(1, 2, 3));
    assert(*pointList.get(1) == Point(4, 5, 6));
    assert(*pointList.get(2) == Point(7, 8, 9));

    // Test add at index
    pointList.add(0, new Point(0, 0, 0));
    assert(pointList.size() == 4);
    assert(*pointList.get(0) == Point(0, 0, 0));

    // Test removeAt
    Point* removed = pointList.removeAt(1);
    assert(*removed == Point(1, 2, 3));
    delete removed;
    assert(pointList.size() == 3);

    // Test indexOf and contains
    Point *tPoint = new Point(4, 5, 6);
    assert(pointList.indexOf(tPoint) == 1);
    tPoint->setX(7); tPoint->setY(8); tPoint->setZ(9);
    assert(pointList.contains(tPoint));
    tPoint->setX(1); tPoint->setY(2); tPoint->setZ(3);
    assert(!pointList.contains(tPoint));

    // Test removeItem
    tPoint->setX(4); tPoint->setY(5); tPoint->setZ(6);
    assert(pointList.removeItem(tPoint, [](Point* p) { delete p; }));
    tPoint->setX(4); tPoint->setY(5); tPoint->setZ(6);
    assert(!pointList.contains(tPoint));
    assert(pointList.size() == 2);

    // Test clear
    pointList.clear();
    assert(pointList.empty());
    assert(pointList.size() == 0);

    // Test copy constructor
    DLinkedList<Point*> pointList1(&DLinkedList<Point*>::free, Point::pointEQ);
    pointList1.add(new Point(1, 1, 1));
    pointList1.add(new Point(2, 2, 2));
    DLinkedList<Point*> pointList2(pointList1);
    assert(pointList2.size() == 2);
    assert(*pointList2.get(0) == Point(1, 1, 1));
    assert(*pointList2.get(1) == Point(2, 2, 2));

    // Test assignment operator
    DLinkedList<Point*> pointList3(&DLinkedList<Point*>::free, Point::pointEQ);
    pointList3 = pointList1;
    assert(pointList3.size() == 2);
    assert(*pointList3.get(0) == Point(1, 1, 1));
    assert(*pointList3.get(1) == Point(2, 2, 2));

    // Test toString
    assert(pointList1.toString(Point::point2str) == "[P(  1.00;  1.00;  1.00), P(  2.00;  2.00;  2.00)]");

    // Test iterator
    DLinkedList<Point*>::Iterator it;
    float sum = 0;
    for(it = pointList1.begin(); it != pointList1.end(); it++) {
        sum += (*it)->radius();
    }
    float expected = sqrt(3) + sqrt(12);
    assert(abs(sum - expected) < EPSILON);

    // Test removing through iterator
    it = pointList1.begin();
    it.remove([](Point* p) { delete p; });
    assert(pointList1.size() == 1);
    assert(*pointList1.get(0) == Point(2, 2, 2));

    delete tPoint;
    cout << "All Point tests passed successfully!" << endl;
}


int main() {
    runPointTests();
    // runIntTests2();
    return 0;
}
