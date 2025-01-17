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

// TODO: Implement the XArrayList class
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
        void    add(T e);
        void    add(int index, T e);
        T       removeAt(int index);
        bool    removeItem(T item, void (*removeItemData)(T) = 0);
        bool    empty();
        int     size();
        void    clear();
        T &     get(int index);
        int     indexOf(T item);
        bool    contains(T item);
        string  toString(string (*item2str)(T &) = 0);
        // Inherit from IList: END

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

        void copyFrom(const XArrayList<T> &list);

        void removeInternalData();

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

// TODO: Implement the XArrayList class's methods
//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////


//TODO: Constructor, Destructor, Copy Constructor, Assignment Operator
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

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
    /*
     * Copies the contents of another XArrayList into this list.
     * Initializes the list with the same capacity as the source list and copies all elements.
     * Also duplicates user-defined comparison and deletion functions, if applicable.
     */
    // TODO
    clear();
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    for (int i = 0; i < list.count; i++){
        if constexpr (std::is_pointer<T>::value) {
            data[i] = new std::remove_pointer_t<T>(*(list.data[i]));
        } else {
            data[i] = list.data[i];
        }
    }
}

template <class T>
void XArrayList<T>::removeInternalData()
{
    /*
     * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
     * If a custom deletion function is provided, it is used to free the stored elements.
     * Finally, the dynamic array itself is deallocated from memory.
     */
    // TODO
    if (deleteUserData) {
        deleteUserData(this);
    }
    delete [] data;
    data = new T[capacity];
    count = 0;
}

template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list){
    deleteUserData = list.deleteUserData;
    itemEqual = list.itemEqual;
    capacity = list.capacity;
    count = list.count;

    data = new T[capacity];
    
    for (int i = 0; i < count; i++){
        if constexpr (std::is_pointer<T>::value) {
            data[i] = new std::remove_pointer_t<T>(*(list.data[i]));
        } else {
            data[i] = list.data[i];
        }
    }
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list)
{
    if (this == &list)
        return *this; 

    if(deleteUserData != 0){
        if constexpr (std::is_pointer<T>::value){
            deleteUserData(this);
        }
    }
    if(data)
        delete[] data;

    count = list.count;
    capacity = list.capacity;
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    data = new T[capacity];

    for (int i = 0; i < count; i++){
        if constexpr (std::is_pointer<T>::value) {
            data[i] = new std::remove_pointer_t<T>(*(list.data[i]));
        } else {
            data[i] = list.data[i];
        }
    }

    return *this;
}


template <class T>
XArrayList<T>::~XArrayList()
{
    if(deleteUserData != 0){
        if constexpr (std::is_pointer<T>::value){
            deleteUserData(this);
        }
    }
    if(data) delete[] data;
}


// TODO: Inherrit methods
template <class T>
void XArrayList<T>::add(T e)
{
    ensureCapacity(count);
    data[count++] = e;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
    if(index < 0 || index > count)
        throw std::out_of_range("Index out of range");
    ensureCapacity(index);
    for(int i = count; i > index; i--)
        data[i] = data[i-1];
    data[index] = e;
    count++;
}


template <class T>
T XArrayList<T>::removeAt(int index)
{
    checkIndex(index);
    T item = data[index];
    for(int i = index; i < count-1; i++)
        data[i] = data[i+1];
    count--;
    return item;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    int index = indexOf(item); 
    if(index == -1) {
        return false;
    }
    if(removeItemData) {
        removeItemData(data[index]);
    }else if constexpr (std::is_pointer<T>::value){
        delete data[index];
    }
    for(int i = index; i < count-1; i++) {
        data[i] = data[i+1];
    }
    count--;
    return true;
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
    if(deleteUserData != 0)
        if constexpr (std::is_pointer<T>::value){
            deleteUserData(this);
        }
    delete [] data;
    data = new T[capacity];
    count = 0;
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

// TODO: other methods
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
    if (index < 0 || index >= count) {
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
    if (index < 0 || index > count) {
        throw std::out_of_range("Index out of range");
    }
    if (index >= capacity) {
        capacity = index*2;
        T *newData = new T[capacity];
        for (int i = 0; i < count; i++){
            if constexpr (std::is_pointer<T>::value) {
                newData[i] = new std::remove_pointer_t<T>(*(data[i]));
            } else {
                newData[i] = data[i];
            }
        }
        delete[] data;
        data = newData;
    }
}

