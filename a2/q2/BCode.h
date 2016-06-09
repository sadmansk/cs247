#ifndef BCODE_H_
#define BCODE_H_

#include <string>
#include <ostream>

class BCode {
public:
    // Exception handler for BCode
    class InvalidFormatException {
    public:
        InvalidFormatException (const std::string& code);
        std::string message() const { return message_; }
    private:
        std::string message_;
    };

    BCode(const std::string&);                                       // constructor
    std::string code() const;

private:
    std::string code_;
    static int const min_length_ = 1;
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
