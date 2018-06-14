#include "node.h"

Node::Node()
{

}

Node::Node(const Vector& position)
{
    this->position = position;
}

Node::~Node()
{
}

void Node::setNext(Node *object)
{
    this->next = object;
}

void Node::setPrevious(Node *object)
{
    this->previous = object;
}

Node *Node::getNext() const
{
    return this->next;
}

Node *Node::getPrevious() const
{
    return this->previous;
}
