#include <iostream>
#include "BaseNode.h"

//default constructor
BaseNode::BaseNode()
{
    row = 0;
    seat = 0;
}

//overloaded constructor
BaseNode::BaseNode(int r, int s)
{
    row = r;
    seat = s;
}

//destructor
BaseNode::~BaseNode()
{
    //std::cout << "Deleting BaseNode: " << getRow() << 'x' << getSeat() << std::endl;
}
