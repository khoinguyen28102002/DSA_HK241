#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory.h>
#include <type_traits>
#include "util/Point.h"

using namespace std;

template<class T>
class IList{
public:
    virtual ~IList(){};
    virtual void    add(T e)=0;
    virtual void    add(int index, T e)=0;
    virtual bool    empty()=0;
    virtual int     size()=0;
    virtual int     indexOf(T item)=0;
};

template <class T>
class XArrayList : public IList<T>
{
public:
    class Iterator; // forward declaration

protected:
    T *data;                                 // dynamic array to store the list's items
    int capacity;                            // size of the dynamic array
    int count;                               // number of items stored in the array
    bool (*itemEqual)(T &lhs, T &rhs);       // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(XArrayList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    XArrayList(
        void (*deleteUserData)(XArrayList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0,
        int capacity = 10);
    XArrayList(const XArrayList<T> &list);
    XArrayList<T> &operator=(const XArrayList<T> &list);
    ~XArrayList();

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
    // Inherit from IList: BEGIN

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, count);
    }

    /** free:
     * if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(XArrayList<T> *list)
    {
        typename XArrayList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    void checkIndex(int index);     // check validity of index for accessing
    void ensureCapacity(int index); // auto-allocate if needed

    /** equals:
     * if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     * if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     * if T: pointer type:
     *      indexOf, contains: will use function pointer "itemEqual"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass itemEqual to the constructor of XArrayList
     *      (2): must define a method for comparing
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }

    // void copyFrom(const XArrayList<T> &list);

    // void removeInternalData();

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    // Iterator: BEGIN
    class Iterator
    {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0)
        {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator)
        {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0)
                removeItemData(item);
            cursor -= 1; // MUST keep index of previous, for ++ later
        }

        T &operator*()
        {
            return pList->data[cursor];
        }
        bool operator!=(const Iterator &iterator)
        {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++;
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
    // Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(
    void (*deleteUserData)(XArrayList<T> *),
    bool (*itemEqual)(T &, T &),
    int capacity)
{
    // TODO
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->capacity = capacity;
    this->count = 0;
    this->data = new T[capacity];
}
// template <class T>
// void XArrayList<T>::copyFrom(const XArrayList<T> &list)
// {
//     /*
//      * Copies the contents of another XArrayList into this list.
//      * Initializes the list with the same capacity as the source list and copies all elements.
//      * Also duplicates user-defined comparison and deletion functions, if applicable.
//      */
//     // TODO
// }

// template <class T>
// void XArrayList<T>::removeInternalData()
// {
//     /*
//      * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
//      * If a custom deletion function is provided, it is used to free the stored elements.
//      * Finally, the dynamic array itself is deallocated from memory.
//      */
//     // TODO
// }

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    // TODO
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    // TODO
}


template <class T>
XArrayList<T>::~XArrayList()
{
    if(deleteUserData != 0)
        deleteUserData(this);
    if(data) delete[] data;
}

template <class T>
void XArrayList<T>::add(T e)
{
    ensureCapacity(count);
    data[count++] = e;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
    ensureCapacity(index);
    for(int i = count; i > index; i--)
        data[i] = data[i-1];
    data[index] = e;
    count++;
}


template <class T>
T XArrayList<T>::removeAt(int index)
{
    if(index < 0 || index >= count)
        throw std::out_of_range("Index out of range");
    else{
        T item = data[index];
        for(int i = index; i < count-1; i++)
            data[i] = data[i+1];
        count--;
        return item;
    }
    return nullptr;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    for (int i = 0; i < count; i++){
        if (itemEqual ? itemEqual(data[i], item) : data[i] == item){
            if (removeItemData){
                removeItemData(data[i]);
            }
            for (int j = i; j < count - 1; j++){
                data[j] = data[j + 1];
            }
            count--;
            return true;
        }
    }
    return false;
}

template <class T>
bool XArrayList<T>::empty()
{
    return this->count == 0;    
}

template <class T>
int XArrayList<T>::size()
{
    return this->count;
}

template <class T>
void XArrayList<T>::clear()
{
    if(data) delete[] data;
    count = 0;
    capacity = 10;
    data = new T[capacity];
}

template <class T>
T &XArrayList<T>::get(int index)
{
    checkIndex(index);
    return data[index];
}

template <class T>
int XArrayList<T>::indexOf(T item)
{
    for (int i = 0; i < count; i++)
        if (equals(data[i], item, itemEqual))
            return i;
    return -1;
}

template <class T>
bool XArrayList<T>::contains(T item)
{
    return indexOf(item) != -1;
}

template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
    /**
     * Converts the array list into a string representation, formatting each element using a user-defined function.
     * If no function is provided, it uses the default string representation for each element.
     * Example: Given an array list with elements {1, 2, 3} and a function that converts integers to strings, calling toString would return "[1, 2, 3]".
     *
     * @param item2str A function pointer for converting items of type T to strings. If null, default to the string conversion of T.
     * @return A string representation of the array list with elements separated by commas and enclosed in square brackets.
     */

    string result = "[";
    for (int i = 0; i < count; i++){
        if (item2str){
            result += item2str(data[i]);
        } else {
            stringstream ss;
            ss << data[i];
            result += ss.str();
        }
        if (i < count - 1) result += ", ";
    }
    result += "]"; 
    return result;
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
void XArrayList<T>::checkIndex(int index)
{
    /**
     * Validates whether the given index is within the valid range of the list.
     * Throws an std::out_of_range exception if the index is negative or exceeds the number of elements.
     * Ensures safe access to the list's elements by preventing invalid index operations.
     */
    // TODO
    if (index < 0 || index > count) {
        throw std::out_of_range("Index out of range");
    }
}
template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
    /**
     * Ensures that the list has enough capacity to accommodate the given index.
     * If the index is out of range, it throws an std::out_of_range exception. If the index exceeds the current capacity,
     * reallocates the internal array with increased capacity, copying the existing elements to the new array.
     * In case of memory allocation failure, catches std::bad_alloc.
     */
    // TODO
    checkIndex(index);
    try {
        if (index >= capacity) {
            capacity = index*2;
            T *newData = new T[capacity];
            memcpy(newData, data, count * sizeof(T));
            delete[] data;
            data = newData;
        }
    } catch (std::bad_alloc& ba) {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
    }
}

void xlistDemo1(){
    XArrayList<int> iList;
    for(int i = 0; i< 10 ; i++)
        iList.add(i*i);
    
    //iList.dump();
    for(XArrayList<int>::Iterator it=iList.begin(); it != iList.end(); it++ )
        cout << *it << ", found at: " << iList.indexOf(*it) << endl;
    cout << endl;
    int item = 120;
    int foundIdx = iList.indexOf(item);
    cout    << "lookup for " << item  << " found at: " << foundIdx << endl;
}
void xlistDemo4(){
    XArrayList<Point*> list1(&XArrayList<Point*>::free, &Point::pointEQ);
    list1.add(new Point(23.2f, 25.4f));
    list1.add(new Point(24.6f, 23.1f));  
    list1.add(new Point(12.5f, 22.3f)); 
    
    for(XArrayList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
        cout << **it << endl;
    
    Point* p1 = new Point(24.6f, 23.1f); //found in list
    Point* p2 = new Point(124.6f, 23.1f); //not found
    cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p2) << endl;
    
    ///Different results if not pass &Point::equals
    cout << endl << endl;
    XArrayList<Point*> list2(&XArrayList<Point*>::free);
    list2.add(new Point(23.2f, 25.4f));
    list2.add(new Point(24.6f, 23.1f));  
    list2.add(new Point(12.5f, 22.3f)); 
    
    for(XArrayList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
        cout << **it << endl;
    
    cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p2) << endl;
    cout << endl;
    
    list1.toString(Point::point2str);
    delete p1; delete p2;
}
int main(int argc, char** argv) {
    // xlistDemo1();
    xlistDemo4();
    cout << "Assignment-1" << endl;
    return 0;
}
