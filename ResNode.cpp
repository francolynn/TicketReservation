#include "ResNode.H"
#include "BaseNode.h"
#include <fstream>

using namespace std;

//overloaded constructor that calls the basenode constructor first
ResNode::ResNode(int r, int s, bool st, ResNode* p, ResNode* n):BaseNode(r, s)
{
    row = r;
    seat = s;
    status = st;
    prev = p;
    next = n;
}

//overloaded operator
ifstream& operator>>(ifstream &fin, ResNode &rnode)
{
    //maneuver variable
    char symbol = 'x';
    //read next char on the file
    fin >> symbol;
    //if seat is open
    if(symbol == '#')
    {
        rnode.setStatus(true);
    }
    //if seat is taken
    if (symbol == '.')
    {
        rnode.setStatus(false);
    }
    //return input
    return fin;
}

//operator for writing seat status to file
ofstream& operator<<(ofstream &output, ResNode &Node)
{
    //if seat is open
    if (Node.getStatus() == true)
    {
        output << "#";
    }
    //if seat is taken
    else if (Node.getStatus() == false)
    {
        output << ".";
    }
    return output;
}
