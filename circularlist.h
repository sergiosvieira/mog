#ifndef CIRCULARLIST_H
#define CIRCULARLIST_H

class Node;

class CircularList
{
protected:
    Node* first = nullptr;
public:
    CircularList();
    void insert(Node* node);
};

#endif // CIRCULARLIST_H
