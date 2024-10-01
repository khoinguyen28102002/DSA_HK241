
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory.h>
#include <type_traits>

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
    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
    this->count = 0;
    this->head = new Node();
    this->tail = new Node();
    this->head->next = this->tail;
    this->tail->prev = this->head;
    Node *p = list.head->next;
    while(p != list.tail){
        if constexpr (std::is_pointer<T>::value) {
            add(new std::remove_pointer_t<T>(*(p->data)));
        } else {
            add(p->data);
        }
        p = p->next;
    }
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // TODO
    if (this == &list)
        return *this;

    clear();

    this->itemEqual = list.itemEqual;
    this->deleteUserData = list.deleteUserData;
    this->count = 0;

    Node *p = list.head->next;
    while(p != list.tail){
        if constexpr (std::is_pointer<T>::value) {
            add(new std::remove_pointer_t<T>(*(p->data)));
        } else {
            add(p->data);
        }
        p = p->next;
    }
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
    Node* newNode = new Node(e);
    newNode->prev = tail->prev;
    newNode->next = tail;
    tail->prev->next = newNode;
    tail->prev = newNode;
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
        for(int i = count - 1; i >= index; i--){
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
    if (head->next == tail){
        return T();
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

    // if constexpr (std::is_pointer<T>::value) {
    //     delete current->data;
    // }
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
        p = p->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    /**
     * Clears the internal data of the list by deleting all nodes and user-defined data.
     * If a custom deletion function is provided, it is used to free the user's data stored in the nodes.
     * Traverses and deletes each node between the head and tail to release memory.
     */
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




// int main(int argc, char** argv) {

//     cout << "Assignment-1" << endl;
//     return 0;
// }
