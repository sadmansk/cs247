#include <string>
#include "MenuComponent.h"

// Menu iterator implementation
MenuComponent::Iterator* MenuComponent::Iterator::next() {
    if (!hasNext()) {
        return NULL;
    }
    cursor_++;

    return this;
}

bool MenuComponent::Iterator::hasNext() const {
    return cursor_ < menu_->size();
}

MenuComponent* MenuComponent::Iterator::getMenu() {
    return menu_->getChild(cursor_);
}

bool MenuComponent::Iterator::operator== (const Iterator& other) const {
    return true;//*this == other;
}

bool MenuComponent::Iterator::operator!= (const Iterator& other) const {
    return !(*this == other);
}

MenuComponent* MenuComponent::Iterator::operator*() {
    return nullptr;
}

MenuComponent* MenuComponent::Iterator::operator->() { 
    return nullptr;
}

MenuComponent::Iterator& MenuComponent::Iterator::operator++ () {
    cursor_++;

    return *this;
}

MenuComponent::Iterator MenuComponent::Iterator::operator++ (int) {
    Iterator temp(menu_, cursor_);
    cursor_++;

    return temp;
}

MenuComponent::MenuComponent( std::string name ) : name_(name), menuDepth_("") {}

void MenuComponent::menuDepthInc () {
    menuDepth_ = "\t" + menuDepth();
}

MenuComponent::Iterator* MenuComponent::begin() {
    return nullptr;
}

MenuComponent::Iterator* MenuComponent::end() {
    return nullptr;
}

// operator<< simply calls mc->print(), and the different subclasses will override print() to specialize what is actually printed
std::ostream& operator<< ( std::ostream& sout, const MenuComponent* mc ) {
    mc->print(sout);
    return sout;
}
