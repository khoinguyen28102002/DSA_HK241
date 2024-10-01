#include <iostream>
#include "DLinkedList.cpp"  // Assuming the class is implemented in this file

// Class kế thừa để truy cập các hàm protected
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

void runTests() {
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

int main() {
    runTests();
    return 0;
}
