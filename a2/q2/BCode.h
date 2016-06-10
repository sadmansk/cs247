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
        InvalidFormatException (const std::string& code, const std::string& reason); // constructor
        std::string reason() const;                                 // accessor
    private:
        std::string reason_;                                        // holds the reason(s) of invalidness
    };

    BCode(const std::string&);                                      // constructor
    std::string code() const;                                       // accessor

private:
    std::string code_;
    // for range checks on the length
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
