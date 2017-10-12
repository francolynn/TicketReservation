#ifndef RESNODE_H
#define RESNODE_H
#include <iostream>
#include <fstream>
#include "BaseNode.h"

using namespace std;

class ResNode:public BaseNode
{
private:

    bool status = 0;         //seat status. true for open, false for reserved
    ResNode *prev = nullptr; //previous node
    ResNode *next = nullptr; //next node linked to

public:

    //constructor
    ResNode(){;}
    //overloaded constructor
    ResNode(int, int, bool, ResNode*, ResNode*);
    //destructor
    ~ResNode(){/*cout << "Deleting: " << getRow() << 'x' << getSeat() << endl*/;}

    //accessors
    bool getStatus(){return status;}
    ResNode* getPrev(){return prev;}
    ResNode* getNext(){return next;}

    //mutators
    void setRow(int r){row = r;}
    void setStatus(bool s){status = s;}
    void setPrev(ResNode* p){prev = p;}
    void setNext(ResNode* n){next = n;}

    //overloaded input output operator that can read, write and store seat statuses
    friend ifstream& operator>>(ifstream&, ResNode&);
    friend ofstream& operator<<(ofstream&, ResNode&);
};
#endif // RESNODE_h
