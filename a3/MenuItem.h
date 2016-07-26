#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <iostream>
#include <string>

#include "MenuComponent.h"

typedef MenuComponent::Iterator BaseIterator;

class MenuItem : public MenuComponent {
public:
    // iterator class
    class ItemIterator : public BaseIterator {
    public:
        ItemIterator();
        virtual ~ItemIterator();
        virtual MenuComponent*  next();
    private:
        MenuItem* item_;
        MenuItem* cursor_;
    };

    // constructor
    MenuItem(const std::string, float, int, bool);

    // implement leaf operations
    virtual bool isVeggie() const;
    virtual float price() const;
    virtual void priceIs( float p );
    virtual int calories() const;
    virtual void caloriesIs( int c );
    virtual BaseIterator* begin();
    virtual BaseIterator* end();

protected:
    virtual void print ( std::ostream& ) const;

private:
    float price_;
    int calories_;
    bool isVeggie_;
};

#endif
