#include <iostream>
#include <iomanip>
#include "./include/util/Point.h"
// #include "ArrayList.cpp"
#include "DLinkedList.cpp"

using namespace std;

string int2str(int* & i) {
    return to_string(*i);
}

// bool pointComparator(Point*& p1, Point*& p2){
//     return (p1->getX() == p2->getX()) && (p1->getY() == p2->getY());
// }

// string LpointPtr2Str(Point*& ptr){
//     stringstream os;
//     os << "("   << ptr->getX() << ", " 
//                 << ptr->getY()
//        << ")";
//     return os.str();
// }

// void xlistDemo1(){
//     XArrayList<int> iList;
//     for(int i = 0; i< 10 ; i++)
//         iList.add(i, i*i);
    
//     //iList.dump();
//     for(XArrayList<int>::Iterator it=iList.begin(); it != iList.end(); it++ )
//         cout << *it << ", found at: " << iList.indexOf(*it) << endl;
//     cout << endl;
//     int item = 120;
//     int foundIdx = iList.indexOf(item);
//     cout    << "lookup for " << item  << " found at: " << foundIdx << endl;
// }

// void xlistDemo2(){
//     XArrayList<Point> alist;
//     alist.add(Point(23.2f, 25.4f));
//     alist.add(Point(24.6f, 23.1f));  
//     alist.println();
// }

// void xlistDemo3(){
//     XArrayList<Point> alist;
//     alist.add(Point(23.2f, 25.4f));
//     alist.add(Point(24.6f, 23.1f));  
    
//     int idx1 = alist.indexOf(Point(24.6f, 23.1f));
//     int idx2 = alist.indexOf(Point(24.61f, 23.1f));
    
//     cout << "result 1 : " << idx1 << endl;
//     cout << "result 2 : " << idx2 << endl;
// }

// void xlistDemo4(){
//     XArrayList<Point*> list1(&XArrayList<Point*>::free, &Point::pointEQ);
//     list1.add(new Point(23.2f, 25.4f));
//     list1.add(new Point(24.6f, 23.1f));  
//     list1.add(new Point(12.5f, 22.3f)); 
    
//     for(XArrayList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
//         cout << **it << endl;
    
//     Point* p1 = new Point(24.6f, 23.1f); //found in list
//     Point* p2 = new Point(124.6f, 23.1f); //not found
//     cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p1) << endl;
//     cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p2) << endl;
    
//     ///Different results if not pass &Point::equals
//     cout << endl << endl;
//     XArrayList<Point*> list2(&XArrayList<Point*>::free);
//     list2.add(new Point(23.2f, 25.4f));
//     list2.add(new Point(24.6f, 23.1f));  
//     list2.add(new Point(12.5f, 22.3f)); 
//     // list2.println(&LpointPtr2Str);
//     for(XArrayList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
//         cout << **it << endl;
    
//     cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p1) << endl;
//     cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p2) << endl;
    
//     delete p1; delete p2;
// }

// void xlistDemo5(){
//     XArrayList<Point> list1;
//     list1.add(Point(23.2f, 25.4f));
//     list1.add(Point(24.6f, 23.1f));  
//     list1.add(Point(12.5f, 22.3f)); 
    
//     for(XArrayList<Point>::Iterator it = list1.begin(); it != list1.end(); it++)
//         cout << *it << endl;
    
//     Point p1 = Point(24.6f, 23.1f); //found in list
//     Point p2 = Point(124.6f, 23.1f); //not found
//     cout << p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p1) << endl;
//     cout << p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p2) << endl;
    
//     ///Different results if not pass &Point::equals
//     cout << endl << endl;
//     XArrayList<Point> list2(list1);
//     list2.add(Point(23.2f, 25.4f));
//     list2.add(Point(24.6f, 23.1f));  
//     list2.add(Point(12.5f, 22.3f)); 
    
//     for(XArrayList<Point>::Iterator it = list2.begin(); it != list2.end(); it++)
//         cout << *it << endl;
    
//     cout << p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p1) << endl;
//     cout << p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p2) << endl;
    
    
//     cout << "List1 size: " << list1.size() << endl;
//     cout << "List2 size: " << list2.size() << endl;
//     list1 = list2;
//     cout << "List1 after assignment: " << list1.toString(&Point::point2str) << endl; 
//     XArrayList<Point> list3 = list2;
//     cout << "List3: " << list3.toString(&Point::point2str) << endl;  
    
//     list1.clear();
//     cout << "List1 after clear: " << list1.size() << endl;
// }

// void xlistDemo6(){
//     XArrayList<Point*> list1(&XArrayList<Point*>::free, &Point::pointEQ);
//     list1.add(new Point(23.2f, 25.4f));
//     list1.add(new Point(24.6f, 23.1f));  
//     list1.add(new Point(12.5f, 22.3f)); 
    
//     for(XArrayList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
//         cout << **it << endl;
    
//     Point* p1 = new Point(24.6f, 23.1f); //found in list
//     Point* p2 = new Point(124.6f, 23.1f); //not found
//     cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p1) << endl;
//     cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list1.indexOf(p2) << endl;
    
//     ///Different results if not pass &Point::equals
//     cout << endl << endl;
//     XArrayList<Point*> list2(list1);
//     list2.add(new Point(23.3f, 22.4f));
//     list2.add(new Point(14.6f, 23.1f));  
//     list2.add(new Point(12.5f, 22.3f)); 
    
//     for(XArrayList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
//         cout << **it << endl;
    
//     cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p1) << endl;
//     cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
//                 << " indexOf returns: " << list2.indexOf(p2) << endl;
    
//     delete p1; delete p2;
// }

// void xlistDemo7(){
//     XArrayList<int*> list1(&XArrayList<int*>::free, nullptr, 15);
//     for(int i = 0; i < 8; i++){
//         int* ptr = new int(i);
//         list1.add(ptr);
//     }
//     list1.println(int2str);
//     list1.clear();
//     cout << list1.size() << endl;
//     for(int i = 8; i < 13; i++){
//         int* ptr = new int(i);
//         list1.add(ptr);
//     }
//     list1.println(int2str);
//     cout << "List size: " << list1.size() << endl;
//     XArrayList<int*> list2(list1);
//     cout << "List2 size: " << list2.size() << endl;
//     list2.add(5, new int(10));
//     list1 = list2;
//     XArrayList<int*> list3 = list2;
//     cout << "List3: " << list3.toString(&int2str) << endl;  
//     list1 = list2;
//     cout << "List1: " << list1.toString(&int2str) << endl; 
//     list1.clear();
//     cout << "List1 after clear: " << list1.size() << endl;
// }

void dlistDemo1(){
    List<int> dlist;
    for(int i = 0; i< 20 ; i++)
        dlist.add(i, i*i);
    dlist.println();
    
    for(List<int>::Iterator it=dlist.begin(); it != dlist.end(); it++ )
        cout << *it << " ";
    cout << endl;
}
void dlistDemo2(){
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

void dlistDemo3(){
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
void dlistDemo4(){
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
void dlistDemo5(){
    DLinkedList<float> dList;
    dList.add(3.2);
    dList.add(5.5);
    dList.println();
    cout << "index of 5.5: " << dList.indexOf(5.5) << endl;
    cout << "index of 15.5: " << dList.indexOf(15.5) << endl;
}

void dlistDemo6(){
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

void dListDemo7(){
    DLinkedList<int*> list1(&DLinkedList<int*>::free, nullptr);
    for(int i = 0; i < 8; i++){
        int* ptr = new int(i);
        list1.add(ptr);
    }
    list1.println(int2str);
    DLinkedList<int*> list2(list1);
    cout << list2.size() << endl;
    list2.removeAt(3);
    list2.println(int2str);
    cout << list1.size() << endl;
}


int main(){
    // xlistDemo1();
    // xlistDemo2();
    // xlistDemo3();
    // xlistDemo4();
    // xlistDemo5();
    // xlistDemo6();
    // xlistDemo7();

    dlistDemo1();
    dlistDemo2();
    dlistDemo3();
    dlistDemo4();
    dlistDemo5();
    dlistDemo6();
    return 0;
}
