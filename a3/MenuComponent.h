#ifndef MENU_COMPONENT_H
#define MENU_COMPONENT_H

#include <string>
#include <iterator>

class MenuComponent {
public:
    friend std::ostream& operator<< ( std::ostream&, const MenuComponent* );

    // Exception to be throw if container operation is called on a leaf node
    class InvalidOp {};
    class Iterator : public std::iterator<std::forward_iterator_tag, int> {
    public:
        Iterator(MenuComponent* menu = NULL, int cursor = 0) : menu_(menu), cursor_(cursor) {}
        virtual ~Iterator() {}
        virtual Iterator* next();
        virtual bool hasNext() const;
        virtual MenuComponent* getMenu();

        // operator overloading
        virtual bool operator== (const Iterator&) const;
        virtual bool operator!= (const Iterator&) const;
        virtual MenuComponent* operator* ();
        virtual MenuComponent* operator-> ();
        virtual Iterator& operator++ ();                        // prefix
        virtual Iterator operator++ (int);                      // postfix

    private:
        MenuComponent* menu_;
        int cursor_;
    };

    // Constructor and destructor; destructor is useful in subclasses to cleanup pointer elements in collection
    MenuComponent( std::string );
    virtual ~MenuComponent() {}

    // leaf-only operations
    virtual bool isVeggie() const {return false;}               // is the menu item vegetarian?
    virtual float price() const { return 0; }                   // what is the price?
    virtual void priceIs( float ) {}                            // set the price of the item
    virtual int calories() const { return 0; }                  // how many calories?
    virtual void caloriesIs( int ) {}                           // modify the number of calories

    // component-only operations
    virtual void add( MenuComponent* ) { throw InvalidOp(); }   // add a menuitem or submenu to a menu
    virtual void remove( std::string ) { throw InvalidOp(); }   // remove a menuitem or submenu from a menu; delete it
    virtual int numChildren() const {return 0;}                 // number of children in the menu
    virtual MenuComponent* getChild(int) const {return NULL;}   // the ith child of the menu

    // shared operations
    std::string name() const { return name_; }                  // return name of menu or menuitem
    std::string menuDepth() const { return menuDepth_; }        // depth of menu component in composite structure (used to indent the element when printing
    virtual void menuDepthInc ();                               // increment the depth of the menu component
    virtual int size() const { return 1; }                      // number of elements in menu component, including all descendents and including self

    // iterators
    virtual Iterator* begin() = 0;                                  // return an iterator to the first element
    virtual Iterator* end() = 0;                                    // return an iterator to the last elemen

protected:
    virtual void print ( std::ostream& ) const = 0;             // used to within operator<< to create a polymorphic operator<<

private:
    std::string name_;
    std::string menuDepth_;
};

std::ostream& operator<< ( std::ostream&, const MenuComponent* );

#endif
