#ifndef BCODE_H_
#define BCODE_H_

#include <string>
#include <ostream>
#include "BaseException.h"

class BCode {
public:
    // Exception handler for BCode
    class InvalidFormatException : public BaseException {
    public:
        InvalidFormatException (const std::string& code, const std::string& reason);
        std::string reason() const;
    private:
        std::string reason_;
    };

    BCode(const std::string&);                                       // constructor
    std::string code() const;

private:
    std::string code_;
    static int const min_length_ = 2;
    static int const max_length_ = 3;
};

// comparison operators
bool operator== (const BCode& a, const BCode& b);
bool operator< (const BCode& a, const BCode& b); 
bool operator!= (const BCode& a, const BCode& b);
bool operator> (const BCode& a, const BCode& b);
bool operator>= (const BCode& a, const BCode& b);
bool operator<= (const BCode& a, const BCode& b);

// operator overloads
std::ostream& operator<< (std::ostream& os, const BCode& a);

#endif // BCODE_H_
