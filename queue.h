#ifndef QUEUE_H
#define QUEUE_H

#include<iostream>

template <class T>
struct Node
{
    T data;
    Node<T> *next;
};

template <class T>
class queue
{
private:
    Node<T> *head;
public:
    queue(void);
    void push(T data);
    void pop();
    T front();
    void print();
    bool empty();
};

template <class T>
queue<T>::queue(void)
{
    head=new Node<T>;
    head->next=NULL;
}

template <class T>
void queue<T>::push(T data)
{
    Node<T> *p=new Node<T>;
    p->data=data;
    Node<T> *t=head;
    while(t->next!=NULL)
        t=t->next;
    p->next=NULL;
    t->next=p;
}

template <class T>
void queue<T>::pop()
{
    if(head->next!=NULL)
        head=head->next;
}

template <class T>
T queue<T>::front()
{
   return head->next->data;
}

template <class T>
void queue<T>::print()
{
    Node<T> *p=head->next;
    while(p!=NULL)
    {
        std::cout<<p->data<<" ";
        p=p->next;
    }
    std::cout<<std::endl;
}

template <class T>
bool queue<T>::empty()
{
    if(head->next==NULL)
        return true;
    else return false;
}

#endif // QUEUE_H
