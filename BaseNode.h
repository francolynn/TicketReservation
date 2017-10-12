#ifndef BASENODE_H
#define BASENODE_H

//each BaseNode represents a seat in an auditorium
class BaseNode
{
protected:

    int row = 0;
    int seat = 0;

public:

    //constructors
    BaseNode();
    BaseNode(int, int);
    virtual ~BaseNode() = 0;

    //mutators
    virtual void setRow(int r) = 0;
    void setSeat(int s){seat = s;}

    //accessors
    int getRow(){return row;}
    int getSeat(){return seat;}
};
#endif
