#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "ResNode.H"


class LinkedList
{
private:

    ResNode *head = nullptr;       //first node in linked list
    ResNode *tail = nullptr;       //last node

public:

    LinkedList();                           //default constructor
    LinkedList(ResNode*, ResNode*);         //overloaded constructor
    ~LinkedList();                          //destructor

    ResNode* getHead(){return head;}        //accessors
    ResNode* getTail(){return tail;}

    void setHead(ResNode* h) {head = h;}    //mutators
    void setTail(ResNode* t) {tail = t;}

    void operator+=(ResNode*&);             //append to list operator
    ResNode* operator[](int);               //random access operator

};
#endif // LINKEDLIST_H
