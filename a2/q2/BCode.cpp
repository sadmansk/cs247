#include "BCode.h"

BCode::InvalidFormatException::InvalidFormatException(const std::string& code) {
    message_ = "\nERROR: Building Code \"" + code + "\" has an invalid format.\n\t- must have length of 2-3 characters\n\t- must start with a capital letter\n\t- must consist of only capiral letters and digits";
}


BCode::BCode(const std::string& bcode) {
    // do a range check on the string
    if (bcode.length() < min_length_ || bcode.length() > max_length_)
        throw InvalidFormatException(bcode);
    // check if the starting character is a capital letter
    if (bcode[0] < 'A' || bcode[0] > 'Z')
        throw InvalidFormatException(bcode);

    //check if the all the preceding letters are of valid format
    for (int i = 1; i < bcode.length(); ++i) {
        if ((bcode[i] < 'A' || bcode[i] > 'Z') && (bcode[i] < '0' || bcode[i] > '9'))
            throw InvalidFormatException(bcode);
    }

    code_ = bcode;
}

std::string BCode::code() const {
    return code_;
}

// comparison operators
bool operator== (const BCode& a, const BCode& b) {
    return a.code().compare(b.code()) == 0;
}

bool operator< (const BCode& a, const BCode& b) {
    for (int i = 0; i < a.code().length() && i < b.code().length(); i++) {
        // compare character by character, assumes all characters are upper case for now
        if (a.code()[i] < b.code()[i]) {
            return true;
        }
        else if (a.code()[i] > b.code()[i]) {
            return false;
        }
    }
    // if one of the codes run out of characters, the bigger string is considered greater
    return a.code().length() < b.code().length();
}

bool operator!= (const BCode& a, const BCode& b) {
    return !(a == b);
}

bool operator> (const BCode& a, const BCode& b) {
    return (a != b) && (b < a);
}

bool operator>= (const BCode& a, const BCode& b) {
    return (a > b) || (a == b);
}

bool operator<= (const BCode& a, const BCode& b) {
    return (a < b) || (a == b);
}

// operator overloads
std::ostream& operator<< (std::ostream& os, const BCode& a) {
    os << a.code();
    return os;
} 
