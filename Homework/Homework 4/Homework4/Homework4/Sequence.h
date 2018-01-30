//
//  Sequence.h
//  Homework4
//
//  Created by SHUDAXUAN on 2/16/17.
//  Copyright © 2017 SHUDAXUAN. All rights reserved.
//

#ifndef Sequence_h
#define Sequence_h

// Sequence.h

#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

template <typename ItemType>

class Sequence
{
public:
    Sequence();          // Create an empty sequence (i.e., one whose size() is 0).
    bool empty() const;  // Return true if the sequence is empty, otherwise false.
    int size() const;    // Return the number of items in the sequence.
    
    bool insert(int pos, const ItemType& value);
    // Insert value into the sequence so that it becomes the item at
    // position pos.  The original item at position pos and those that
    // follow it end up at positions one higher than they were at before.
    // Return true if 0 <= pos <= size() and the value could be
    // inserted.  (It might not be, if the sequence has a fixed capacity
    // (e.g., because it's implemented using a fixed-size array) and is
    // full.)  Otherwise, leave the sequence unchanged and return false.
    // Notice that if pos is equal to size(), the value is inserted at the
    // end.
    
    int insert(const ItemType& value);
    // Let p be the smallest integer such that value <= the item at
    // position p in the sequence; if no such item exists (i.e.,
    // value > all items in the sequence), let p be size().  Insert
    // value into the sequence so that it becomes the item at position
    // p.  The original item at position p and those that follow it end
    // up at positions one higher than before.  Return p if the value
    // was actually inserted.  Return -1 if the value was not inserted
    // (perhaps because the sequence has a fixed capacity and is full).
    
    bool erase(int pos);
    // If 0 <= pos < size(), remove the item at position pos from
    // the sequence (so that all items that followed this item end up at
    // positions one lower than they were at before), and return true.
    // Otherwise, leave the sequence unchanged and return false.
    
    int remove(const ItemType& value);
    // Erase all items from the sequence that == value.  Return the
    // number of items removed (which will be 0 if no item == value).
    
    bool get(int pos, ItemType& value) const;
    // If 0 <= pos < size(), copy into value the item at position pos
    // in the sequence and return true.  Otherwise, leave value unchanged
    // and return false.
    
    bool set(int pos, const ItemType& value);
    // If 0 <= pos < size(), replace the item at position pos in the
    // sequence with value and return true.  Otherwise, leave the sequence
    // unchanged and return false.
    
    int find(const ItemType& value) const;
    // Let p be the smallest integer such that value == the item at
    // position p in the sequence; if no such item exists, let p be -1.
    // Return p.
    
    void swap(Sequence& other);
    // Exchange the contents of this sequence with the other one.
    
    // Housekeeping functions
    ~Sequence();
    Sequence(const Sequence& other);
    Sequence& operator=(const Sequence& rhs);
    
private:
    // Representation:
    //   a circular doubly-linked list with a dummy node.
    //   m_head points to the dummy node.
    //   m_head->m_prev->m_next == m_head and m_head->m_next->m_prev == m_head
    //   m_size == 0  iff  m_head->m_next == m_head->m_prev == m_head
    //   if m_size > 0
    //       m_head->next points to the node at position 0.
    //       m_head->prev points to the node at position m_size-1.
    
    struct Node
    {
        ItemType m_value;
        Node*    m_next;
        Node*    m_prev;
    };
    
    Node* m_head;
    int   m_size;
    
    void createEmpty();
    // Create an empty list.  (Should be called only by constructors.)
    
    void insertBefore(Node* p, const ItemType& value);
    // Insert value in a new Node before Node p, incrementing m_size.
    
    Node* doErase(Node* p);
    // Remove the Node p, decrementing m_size.  Return the Node that
    // followed p.
    
    Node* nodeAtPos(int pos) const;
    // Return pointer to Node at position pos.  If pos == m_size, return
    // m_head.  (Will be called only when 0 <= pos <= size().)
};

// Declarations of non-member functions



int subsequence(const Sequence& seq1, const Sequence& seq2);
// If seq2 is a contiguous subsequence of seq1, return the position in
// seq1 where that subsequence starts (the earliest such position if more
// than one).  If not, or if seq2 is empty, return -1.


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);
// Set result to a Sequence that interleaves seq1 and seq2.


// Inline implementations
template <typename ItemType>
inline
int Sequence<ItemType>::size() const
{
    return m_size;
}
template <typename ItemType>
inline
bool Sequence<ItemType>::empty() const
{
    return size() == 0;
}

#endif // SEQUENCE_INCLUDED

#endif /* Sequence_h */
