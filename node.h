#ifndef NODE_H
#define NODE_H

#include "vector.h"

class Node
{
protected:
    Vector position;
    Node* previous = nullptr;
    Node* next = nullptr;
public:
    Node();
    Node(const Vector& position);
    virtual ~Node();
    void setNext(Node* object);
    void setPrevious(Node* object);
    Node* getNext() const;
    Node* getPrevious() const;
};


#endif // NODE_H
