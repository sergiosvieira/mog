#include "circularlist.h"

#include "node.h"

CircularList::CircularList()
{

}

void CircularList::insert(Node *node)
{
    if (node)
    {
        if (this->first == nullptr)
        {
            this->first = node;
        }
        else
        {
            Node* aux = this->first;
            while (aux->getNext() != nullptr
                   && aux->getNext() != this->first)
            {
                aux = aux->getNext();
            }
            aux->setNext(node);
            node->setPrevious(aux);
            node->setNext(this->first);
            this->first->setPrevious(node);
        }
    }
}
