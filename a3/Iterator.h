#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <iterator>

// base component class
class Component {
public:
    virtual ~Component() {}
};

class Iterator : public std::{
public:
    virtual ~Iterator() {}
    virtual Iterator* next() {return nullptr;}
    virtual bool hasNext() const { return false; }

    // operator overloading
    virtual bool operator== (const Iterator&) const { return false; }
    virtual bool operator!= (const Iterator& a) const { return true; }
    virtual Component* operator* () { return nullptr; }
    virtual Component* operator-> () { return nullptr; }
    virtual Iterator& operator++ () { return *this; }  // prefix
    virtual Iterator operator++ (int) { return *this; }   // postfix

private:

};

#endif
