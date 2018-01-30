//
//  Sequence.cpp
//  Project2
//
//  Created by SHUDAXUAN on 1/30/17.
//  Copyright © 2017 SHUDAXUAN. All rights reserved.
//

#include "Sequence.h"

Sequence::Sequence():m_size(0)
{
    m_head = nullptr;
}

Sequence::~Sequence()
{
    Node* p;
    p = m_head;
        while(p != nullptr)
            {
                Node* n = p->m_next;
                delete p;
                p = n;
            }
}

Sequence::Sequence(const Sequence& other): m_size(other.m_size)
{
    m_head = new Node;                                     // create new node to copy into
    m_head->m_value = other.m_head->m_value;
    m_head->m_prev = nullptr;
    Node *m = m_head;
    Node *n = other.m_head->m_next;
    while (n != nullptr)
    {
        m->m_next = new Node;
        m->m_next->m_value = n->m_value;
        m->m_next->m_prev = n;
        n = n->m_next;
        m = m->m_next;
    }
    m->m_next = nullptr;
   
}

Sequence& Sequence::operator=(const Sequence &src)
{
    if (this == &src)
        return *this;
    Sequence temp = src;
    swap(temp);
    return *this;
}

bool Sequence::insert(int pos, const ItemType &value)
{
    if (pos < 0 || pos > m_size)
        return false;
    uncheckedInsert(pos, value);
    return true;
}

int Sequence::insert(const ItemType &value)
{
    Node* p = m_head;
    int pos = 0;
    while (p != nullptr)
    {
        if (p->m_value < value)
        {
            p = p->m_next;
            pos++;
        }
        else
            break;
    }
    uncheckedInsert(pos, value);
    return pos;
}

bool Sequence::erase(int pos)
{
    if (pos < 0  ||  pos >= size())
        return false;
    Node*p = m_head;
    for (int i = 0; i < pos; i++)
        p = p->m_next;
    if (pos == 0)                           // if erase head, have to reset m_head
    {
        m_head = m_head->m_next;
        delete p;
    }
    else
    {
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p->m_prev;
         delete p;
    }
    m_size--;
    return true;
}

int Sequence::remove(const ItemType& value)
{
    int i_pos = find(value);
    int count = 0;
    if (i_pos == -1)
        return 0;
    while (i_pos >= 0)                      // have found value to remove
    {
        erase(i_pos);
        count++;
        i_pos = find(value);
    }
    return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0  ||  pos >= size())
        return false;
    Node* p;
    p = m_head;
    for (int i = 0; i < pos; i++)
        p = p->m_next;
    value = p->m_value;
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0  ||  pos >= size())
        return false;
    Node* p;
    p = m_head;
    for (int i = 0; i < pos; i++)
        p = p->m_next;
    p->m_value = value;
    return true;
}

int Sequence::find(const ItemType& value) const
{
    Node* p = m_head;
    int pos = 0;
    while(p != nullptr)
    {
        if (p->m_value == value)
            return pos;
        else
        {
            p = p->m_next;
            pos++;
        }
    }
    return -1;
}

void Sequence::swap(Sequence& other)
{
    std::swap(m_size, other.m_size);                // swich   size
    std::swap(m_head, other.m_head);                // for linklist, we only need to switch head.
 
}

void Sequence::uncheckedInsert(int pos, const ItemType &value)
{
    Node* n = new Node;
    n->m_value = value;
    if (m_head == nullptr)                  // Make the Node n to the head of the sequence
    {
        n->m_next = nullptr;
        n->m_prev = nullptr;
        m_head = n;
    }
    else if (pos == 0)                      // add at the head , Change the head
    {
        n->m_next = m_head;
        m_head->m_prev = n;
        m_head = n;
        m_head->m_prev = nullptr;
    }
        else if (pos == m_size)             // add at the rear
        {
            Node* p = m_head;
            while (p->m_next != nullptr)
                p = p->m_next;
            p->m_next = n;
            n->m_prev = p;
            n->m_next = nullptr;
        }
            else                            // add at middle
            {
                Node* p = m_head;
                for (int i = 0; i < pos-1; i++)
                    p = p->m_next;
                n->m_prev = p;
                n->m_next = p->m_next;
                p->m_next->m_prev = n;
                p->m_next = n;
            }
  m_size++;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    Sequence temp_result;                                   // make a copy avoid aliasing
    if (seq2.size() <= seq1.size())                         // seq1.size >= seq2.size
    {
        for (int i= 0; i < seq2.size(); i++)                // first to treat the equal leangth part of the two sequence
        {
            ItemType x,y;
            seq1.get(i, x);
            seq2.get(i, y);                                 // take out the values of two sequence and separately store in x and y
            temp_result.insert(2*i, x);                     // for seq1, put the value at position 0,1,3,5,7,etc.
            temp_result.insert(2*i + 1, y);                 // for seq1, put the value at position 2,4,6,8,10,etc.
        }
        for (int i = seq2.size(); i < seq1.size(); i++)     // second to deal with the remaining part
        {
            ItemType x;
            seq1.get(i, x);
            temp_result.insert(temp_result.size(), x);      // keep adding values at the rear
        }
    }
    else                                                    // seq1.size < seq2.size similarly
    {
        for (int i= 0; i < seq1.size(); i++)
        {
            ItemType x,y;
            seq1.get(i, x);
            seq2.get(i, y);
            temp_result.insert(2*i, x);
            temp_result.insert(2*i + 1, y);
        }
        for (int i = seq1.size(); i < seq2.size(); i++)
        {
            ItemType x;
            seq2.get(i, x);
            temp_result.insert(temp_result.size(), x);
        }

    }
    
    result.swap(temp_result);
}


























