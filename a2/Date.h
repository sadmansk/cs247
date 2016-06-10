#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <utility>
#include <array>
#include <ctime>

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
	struct	Impl;                                   // struct for private implementation
	Impl*	pimpl_;
};

Date incDays (const Date&, long);  // increment Date by num days
Date incMonths (const Date&, int); // increment Date by num months - round down if invalid, return new Date
Date incYears (const Date&, int);  // increment Date by num years - round down if invalid, return new Date

// comparison operators
bool operator== (const Date&, const Date&);
bool operator!= (const Date&, const Date&);
bool operator< (const Date&, const Date&);
bool operator<= (const Date&, const Date&);
bool operator> (const Date&, const Date&);
bool operator>= (const Date&, const Date&);

// streaming operators
std::ostream& operator<< (std::ostream&, const Date&);
std::istream& operator>> (std::istream&, Date&);


#endif
