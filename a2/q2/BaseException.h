#ifndef BASE_EXCEPTION_H_
#define BASE_EXCEPTION_H_

#include <string>

class BaseException {
public:
    BaseException (const std::string&) {}
    std::string message () const { return message_; }
protected:
    std::string message_;
};

#endif // BASE_EXCEPTION_H_
