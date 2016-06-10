#ifndef BASE_EXCEPTION_H_
#define BASE_EXCEPTION_H_

#include <string>

class BaseException {
public:
    BaseException (const std::string&) {}                   // empty constructor
    std::string message () const { return message_; }       // accessor
protected:
    std::string message_;                                   // buffer for error message
};

#endif // BASE_EXCEPTION_H_
