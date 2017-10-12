#include "LinkedList.h"
#include "ResNode.H"
#include <iostream>

//default constructor
LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
}

//overloaded constructor
LinkedList::LinkedList(ResNode* h, ResNode* t)
{
    head = h;
    tail = t;
}

//Destructor: deletes entire linked list from head to tail
LinkedList::~LinkedList()
{
    //pointed object will be deleted
    if (head == nullptr)
    {
        delete head;
        return;
    }

    //loop until list is empty
    while (head->getNext() != nullptr)
    {
        //move head down the list
        head = head->getNext();
        //delete previous node
        delete head->getPrev();
    }
    delete head;
}

//overloaded operator that adds a node to the end of the list
void LinkedList::operator+=(ResNode *&n)
{
    //if list is empty
    if (head == nullptr)
    {
        head = n;
        //tail = n;
    }
    //if there is only one item on the list
    else if (head->getNext()==nullptr)
    {
        tail = n;
        tail->setPrev(head);
        head->setNext(tail);
    }
    //if there are at least 2 nodes on linked list
    else
    {
        tail->setNext(n);
        n->setPrev(tail);
        tail = n;
    }
}

//overloaded operator that allows list to be accessed with bracket notation like an array
ResNode* LinkedList::operator[](int i)
{
    //maneuver node
    ResNode *cur = head;

    //for loop that moves down the list
    for (int j = 0; j<i; j++)
    {
        //move one node down the list
        cur = cur->getNext();
    }
    //return the resnode ptr in the i place on the list
    return cur;
}
