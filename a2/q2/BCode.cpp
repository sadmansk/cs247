#include "BCode.h"

BCode::InvalidFormatException::InvalidFormatException(const std::string& code, const std::string& reason) : BaseException(code) {
    message_ = "\nERROR: Building Code \"" + code + "\" has an invalid format.";
    reason_ = reason;
}

std::string BCode::InvalidFormatException::reason() const {
    return reason_;
}

BCode::BCode(const std::string& bcode) {
    // do a range check on the string
    std::string reason = "";
    if (bcode.length() < min_length_ || bcode.length() > max_length_)
        reason += "\n\t- must have length of 2-3 characters";
    // check if the starting character is a capital letter
    if (bcode[0] < 'A' || bcode[0] > 'Z')
        reason += "\n\t- must start with a capital letter";

    //check if the all the preceding letters are of valid format
    for (unsigned int i = 1; i < bcode.length(); ++i) {
        if ((bcode[i] < 'A' || bcode[i] > 'Z') && (bcode[i] < '0' || bcode[i] > '9')) {
            reason += "\n\t- must consist of only capital letters and digits";
            break;
        }
    }
    if (reason.length() > 0) {
        throw InvalidFormatException(bcode, reason);
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
    for (unsigned int i = 0; i < a.code().length() && i < b.code().length(); i++) {
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
