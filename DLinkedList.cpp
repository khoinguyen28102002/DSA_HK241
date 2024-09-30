
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory.h>
#include <type_traits>
#include "testcase.h"

using namespace std;

// TODO: Implement the IList interface
template<class T>
class IList{
    public:
        virtual ~IList(){};
        /* add(T e): append item "e" to the list
        */
        virtual void    add(T e)=0;
        
        
        
        /* add(int index, T e): insert item "e" at location "index";
        *      location is an integer started from 0
        */
        virtual void    add(int index, T e)=0;
        
        
        
        /* removeAt(int index): remove the item at location "index"
        * 
        * return:
        *  >> the item stored at index
        *  >> throw an exception (std::out_of_range) if index is invalid
        */
        virtual T       removeAt(int index)=0;
        
        
        /* removeItem(T item, void (*removeItemData)(T)=0): remove item stored in the list
        *   >> removeItemData: a function pointer (maybe NULL, default),
        *          that will be called inside of removeItem to delete the item's data stored in the list
        *   >> removeItemData, why need? because we DO NOT type T is a pointer or not.
        * 
        * return:
        *   >> true if "item" stored in the list; 
        *   >> otherwise, return false
        */
        virtual bool    removeItem(T item, void (*removeItemData)(T)=0)=0;
        
        
        
        /* empty(): return true if the list is empty; otherwise, return false
        */
        virtual bool    empty()=0;
        
        
        
        /* size(): return number of items stored in the list
        */
        virtual int     size()=0;
        
        
        
        /* clear(): make the list empty by clearing all data and putting the list to the initial condition
        */
        virtual void    clear()=0;
        
        
        
        /* get(int index): return a reference to the item at location "index"
        *      if index is invalid, this function will throw an exception "std::out_of_range"
        * 
        * NOTE: programmers can change the item returned by this function
        */
        virtual T&      get(int index)=0;
        
        
        /* indexOf(T item): return the index of item
        *      if item is not found, then return -1
        */
        virtual int     indexOf(T item)=0;
        
        
        
        /* contains(T item): return true if the list contains "item", else: return false
        */
        virtual bool    contains(T item)=0;
        
        
        
        /* toString(string (*item2str)(T&)=0 ): return a string describing the list.
        *    >> We do not know the item type, so we do not know how to convert each item to string.
        *       Therefore, we need a pointer to a function
        *          that can convert the item (passed to that function) to a string
        */
        virtual string  toString(string (*item2str)(T&)=0 )=0;
};

// TODO: Implement the DLinkedList class
template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    // DLinkedList();
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
};
//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
    void (*deleteUserData)(DLinkedList<T> *),
    bool (*itemEqual)(T &, T &))
{
    // TODO
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    this->count = 0;
    this->itemEqual = itemEqual;
    this->deleteUserData = deleteUserData;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // TODO
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    count = list.count;
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
    Node *p = list.head->next;
    while(p != list.tail){
        if constexpr (std::is_pointer<T>::value) {
            add(new std::remove_pointer_t<T>(*(p->data)));
        } else {
            add(p->data);
        }
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this == &list)
        return *this;
    if(deleteUserData != 0)
        deleteUserData(this);
    clear();
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    Node *p = list.head->next;
    while(p != list.tail){
        if constexpr (std::is_pointer<T>::value) {
            add(new std::remove_pointer_t<T>(*(p->data)));
        } else {
            add(p->data);
        }
        p = p->next;
    }
    this->count = list.count;
    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    // TODO
    if(deleteUserData != 0)
        deleteUserData(this);
    Node* current = head->next;
    while (current != tail) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    delete this->head, delete this->tail;
}

template <class T>
void DLinkedList<T>::add(T e)
{
    // TODO
    Node* newNode = new Node(e, nullptr, nullptr);

    if (head->next == tail) {
        head->next = newNode;
        newNode->prev = head;
        newNode->next = tail;
        tail->prev = newNode;
    } 
    else {
        newNode->prev = tail->prev;
        newNode->next = tail;
        tail->prev->next = newNode;
        tail->prev = newNode;
    }

    count++;
}

template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // TODO
    if(index < 0 || index > count){
        throw std::out_of_range("Index out of range");
    }else{
        if(index == count){
            add(e);
        }else{
            Node *newNode = new Node(e, nullptr, nullptr);
            Node* current = head->next;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;

            count++;
        }
    }
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    /**
     * Returns the node preceding the specified index in the doubly linked list.
     * If the index is in the first half of the list, it traverses from the head; otherwise, it traverses from the tail.
     * Efficiently navigates to the node by choosing the shorter path based on the index's position.
     */
    // TODO

    if (index < 0 || index >= count) {
        throw std::out_of_range("Index out of range");
    }
    if(index < count / 2){
        Node* current = head->next;
        for(int i = 0; i < index; i++){
            current = current->next;
        }
        return current->prev;
    }else{
        Node* current = tail->prev;
        for(int i = count - 1; i > index; i--){
            current = current->prev;
        }
        return current;
    }
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
    // TODO
    if(index < 0 || index >= count){
        throw std::out_of_range("Index out of range");
    }
    Node* current = head->next;
    for (int i = 0; i < index; i++)
        current = current->next;

    T removedData = current->data;

    // Update previous and next pointers
    if (current->prev)
        current->prev->next = current->next;
    if (current->next)
        current->next->prev = current->prev;

    delete current;
    count--;
    return removedData;
}

template <class T>
bool DLinkedList<T>::empty()
{
    // TODO
    return count == 0;
}

template <class T>
int DLinkedList<T>::size()
{
    // TODO
    return count;
}

template <class T>
void DLinkedList<T>::clear()
{
    // TODO
    if (deleteUserData) {
        deleteUserData(this);
    } 
    Node* current = head->next;
    while (current != tail) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    // TODO
    if(index < 0 || index >= count){
        throw std::out_of_range("Index out of range");
    }
    Node* current = head->next;
    for(int i = 0; i < index; i++){
        current = current->next;
    }
    return current->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    // TODO
    Node* current = head->next;
    for(int i = 0; i < count; i++){
        if(equals(current->data, item, itemEqual)){
            return i;
        }
        current = current->next;
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    // TODO
    int index = indexOf(item);
    if(head == nullptr || index == -1){
        return false;
    }
    Node* current = head->next;
    for(int i = 0; i < index; i++){
        current = current->next;
    }
    T removedData = current->data;

    // Update previous and next pointers
    if (current->prev)
        current->prev->next = current->next;
    if (current->next)
        current->next->prev = current->prev;

    if (removeItemData != 0)
        removeItemData(current->data);
    delete current;
    count--;
    return true;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    // TODO
    return indexOf(item) != -1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the list into a string representation, where each element is formatted using a user-provided function.
     * If no custom function is provided, it directly uses the element's default string representation.
     * Example: If the list contains {1, 2, 3} and the provided function formats integers, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function that converts an item of type T to a string. If null, default to string conversion of T.
     * @return A string representation of the list with elements separated by commas and enclosed in square brackets.
     */
    // TODO
    string result = "[";
    for (Node *current = head->next; current != tail; current = current->next){
        if (item2str){
            result += item2str(current->data);
        } else {
            stringstream ss;
            ss << current->data;
            result += ss.str();
        }
        if (current->next != tail) result += ", ";
    }
    result += "]"; 
    return result;
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    /**
     * Copies the contents of another doubly linked list into this list.
     * Initializes the current list to an empty state and then duplicates all data and pointers from the source list.
     * Iterates through the source list and adds each element, preserving the order of the nodes.
     */
    // TODO
    clear();
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    Node *p = list.head->next;
    while(p != list.tail){
        if constexpr (std::is_pointer<T>::value) {
            add(new std::remove_pointer_t<T>(*(p->data)));
        } else {
            add(p->data);
        }
    }
}

// template <class T>
// void DLinkedList<T>::removeInternalData()
// {
//     /**
//      * Clears the internal data of the list by deleting all nodes and user-defined data.
//      * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
//      * Traverses and deletes each node between the head and tail to release memory.
//      */
//     // TODO
// }


int main(int argc, char** argv) {
    // if(argc < 2){
    //     cout << "Please provide the test number" << endl;
    //     return 1;
    // }else{
    //     int testNum = atoi(argv[1]);
    //     switch(testNum){
    //         case 1:
    //             dListDemo1();
    //             break;
    //         case 2:
    //             dListDemo2();
    //             break;
    //         case 3:
    //             dListDemo3();
    //             break;
    //         case 4:
    //             dListDemo4();
    //             break;
    //         case 5:
    //             dListDemo5();
    //             break;
    //         case 6:
    //             dListDemo6();
    //             break;
    //         case 7:
    //             dListDemo7();
    //             break;
    //         default:
    //             cout << "Invalid test number" << endl;
    //     }
    // }
    dListDemo7();
    cout << "Assignment-1" << endl;
    return 0;
}
void dListDemo1(){
    List<int> dlist;
    for(int i = 0; i< 20 ; i++)
        dlist.add(i, i*i);
        // dlist.add(i);
    dlist.println();
    
    for(List<int>::Iterator it=dlist.begin(); it != dlist.end(); it++ )
        cout << *it << " ";
    cout << endl;
}
void dListDemo7(){
    DLinkedList<int*> list1(&DLinkedList<int*>::free, nullptr);
    for(int i = 0; i < 8; i++){
        int* ptr = new int(i);
        list1.add(ptr);
    }
    list1.println();
    list1.clear();
    cout << list1.size() << endl;
}
void dListDemo2(){
    DLinkedList<Point*> list1(&DLinkedList<Point*>::free, &Point::pointEQ);
    list1.add(new Point(23.2f, 25.4f));
    list1.add(new Point(24.6f, 23.1f));  
    list1.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
        cout << **it << endl;
    
    Point* p1 = new Point(24.6f, 23.1f); //found in list
    Point* p2 = new Point(124.6f, 23.1f); //not found
    cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p2) << endl;
    
    ///Different results if not pass &Point::equals
    cout << endl << endl;
    DLinkedList<Point*> list2(&DLinkedList<Point*>::free);
    list2.add(new Point(23.2f, 25.4f));
    list2.add(new Point(24.6f, 23.1f));  
    list2.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
        cout << **it << endl;
    
    cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p2) << endl;
    
    delete p1; delete p2;
}

void dListDemo3(){
    DLinkedList<Point> dList;
    dList.add(Point(1.5, 3.5));
    dList.add(Point(2.5, 4.5));
    dList.add(Point(1.6, 3.1));
    
    cout << "test for indexOf: " << endl;
    Point p(1.6, 3.1);
    cout << p << " at: " << dList.indexOf(p);
}
bool pointComparator(Point*& p1, Point*& p2){
    return (p1->getX() == p2->getX()) && (p1->getY() == p2->getY());
}
string LpointPtr2Str(Point*& ptr){
    stringstream os;
    os << "("   << ptr->getX() << ", " 
                << ptr->getY()
       << ")";
    return os.str();
}
void dListDemo4(){
    DLinkedList<Point*> dList(&DLinkedList<Point*>::free, &pointComparator);
    dList.add(new Point(1.5, 3.5));
    dList.add(new Point(2.5, 4.5));
    dList.add(new Point(1.6, 3.1));
    dList.println(&LpointPtr2Str);
    
    cout << "test for indexOf: " << endl;
    Point* p = new Point(1.6, 3.1);
    cout << *p << " at: " << dList.indexOf(p) << endl;
    delete p;
}
void dListDemo5(){
    DLinkedList<float> dList;
    dList.add(3.2);
    dList.add(5.5);
    dList.println();
    cout << "index of 5.5: " << dList.indexOf(5.5) << endl;
    cout << "index of 15.5: " << dList.indexOf(15.5) << endl;
}

void dListDemo6(){
    List<int> list;
    for(int i = 0; i< 10 ; i++)
        list.add(i, i*i);
    
    cout << setw(25) << left << "Original list: ";
    list.println();
    
    //
    int& item = list.get(5);
    item = 999;
    cout << setw(25) << left << "After changing an item: ";
    list.println();
}
