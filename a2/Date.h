#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <map>


class Date {
public:
	Date (int day, std::string month, int year);    // constructor -- NO DEFAULT CONSTRUCTOR
	Date (const Date&);                             // copy constructor
	static Date today();                            // returns new Date value = today
	~Date();                                        // destructor
	int day() const;                                // accessor
	std::string month() const;                      // accessor
	int year() const;                               // accessor
	Date& operator= (const Date&);                  // assignment
private:
	struct	Impl;
	Impl*	pimpl_;
    static std::map <std::string, int> *months_map;
};

// helper functions

static Date::map <std::string, int> months_map = NULL;

Date incDays (const Date&, long);  // increment Date by num days
Date incMonths (const Date&, int); // increment Date by num months - round down if invalid, return new Date
Date incYears (const Date&, int);  // increment Date by num years - round down if invalid, return new Date


bool operator== (const Date&, const Date&);
bool operator!= (const Date&, const Date&);
bool operator< (const Date&, const Date&);
bool operator<= (const Date&, const Date&);
bool operator> (const Date&, const Date&);
bool operator>= (const Date&, const Date&);

std::ostream& operator<< (std::ostream&, const Date&);
std::istream& operator>> (std::istream&, Date&);

// Date Declaration

struct Date::Impl {
    int day_;
    std::string month_;
    int year_;
};

// Constructors, Destructors and Assignment

Date::Date(int day, std::string month, int year) : pimpl_(new Date::Impl) {
    // construct the month map
    if (months_map == NULL) {
        months_map = new std::map<std::string, int> ();
    }
    if (year < 0) {
        throw "Invalid year";
    }

    pimpl_->day_ = day;
    pimpl_->month_ = month;
    pimpl_->year_ = year;
}

Date::~Date() {
    delete pimpl_;
    pimpl_ = NULL;
}

Date::Date (const Date& d) : pimpl_(new Date::Impl (*d.impl_)) { }

Date& Date::operator= (const Date& d) {
    Date copy(r);
    Date::Impl* temp;

    temp = copy.pimpl_;
    copy.pimpl_ = pimpl_;
    pimpl_ = temp;

    return *this;
}

// Accessors

int Date::day() const {
    return pimpl_->day_;
}

std::string Date::month() const {
    return pimpl_->month_;
}

int Date::year() const {
    return pimpl_->year_;
}

// Streaming functions

std::ostream& operator<< (std::ostream& os, const Date& d) {
    const char space = ' ';
    const char comma = ',';

    os << d.day() << space << d.month() << comma << space << d.year();

    return os;
}

std::istream& operator>> (std::istream& is, Date& d) {
    int day;
    std::string month;
    int year;

    std::cout << "Enter day of month (1-31): ";
    is >> day;
    std::cout << "Enter month (January-December): ";
    is >> month;
    std::cout << "Enter year (1900-2100): ";
    is >> year;

    d = Date(day, month, year);

    return is;
}

// Comparison Operators

bool operator== (const Date& a, const Date& b) {
    return false;
}

bool operator!= (const Date& a, const Date& b) {
    return !(a == b);
}

bool operator< (const Date& a, const Date& b) {
    return true;
}

bool operator<= (const Date& a, const Date& b) {
    return (a < b) || (a == b);
}

bool operator> (const Date& a, const Date& b) {
    return !(a <= b);
}

bool operator>= (const Date& a, const Date& b) {
    return !(a < b);
}
    
#endif
