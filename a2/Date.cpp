#include "Date.h"

// static helper functions and members
namespace date_helpers {
// store all the months and the number of days
static const std::array< std::pair<std::string, int>, 12 > month_name = {
    std::make_pair("January", 31), std::make_pair("February", 28), std::make_pair("March", 31), std::make_pair("April", 30), std::make_pair("May", 31), std::make_pair("June", 30),
    std::make_pair("July", 31), std::make_pair("August", 31), std::make_pair("September", 30), std::make_pair("October", 31), std::make_pair("November", 30), std::make_pair("December", 31)
};
// get the number of days in a month
static int get_days (std::string mon) {
    for (unsigned int i = 0; i < month_name.size(); i++) {
        if (month_name[i].first.compare(mon) == 0) {
            return month_name[i].second;
        }
    }
    return -1;
}

// get the total number of days leading upto a given month
static int get_total_days (std::string mon, bool leap = false) {
    int count = 0;
    for (unsigned int i = 0; i < month_name.size(); i++) {
        if (month_name[i].first.compare(mon) == 0) {
            return count;
        }
        else {
            count += month_name[i].second;
            if (i == 1 && leap) {
                count++;
            }
        }
    }
    return -1;
}

// get the position of a month in the year
static int get_index (std::string mon) {
    for (unsigned int i = 0; i < month_name.size(); i++) {
        if (month_name[i].first.compare(mon) == 0) {
            return i;
        }
    }
    return -1;
}

// get the month from a given absolute day
static int get_index_from_days (int days, bool leap = false) {
    for (unsigned int i = 0; i < month_name.size(); i++) {
        if (i == 1 && leap) {
            if (days > 29)
                days -= 29;
            else return 1;
        }
        else if (days > month_name[i].second) {
            days -= month_name[i].second;
        }
        else {
            return i;
        }
    }
    return -1;
}

// returns if a year is leap
static bool is_leap_year (int year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

} // namespace date_helpers

// Date Pimpl Definition
struct Date::Impl {
    int day_;
    std::string month_;
    int year_;
};

// Constructors, Destructors and Assignment

Date::Date(int day, std::string month, int year)  {
    // validate the range on the parameters
    if (year < 1900 || year > 2100) {
        throw "Invalid year";
    }

    int max_day = date_helpers::get_days(month);
    if (max_day < 1) {
        throw "Invalid month";
    }
    bool is_leap = date_helpers::is_leap_year(year);
    if (max_day < 30 && is_leap) max_day++;

    if (day < 1 || day > max_day) {
        throw "Invalid day of the month";
    }
    pimpl_ = new Date::Impl;
    pimpl_->day_ = day;
    pimpl_->month_ = month;
    pimpl_->year_ = year;
}

Date::~Date() {
    delete pimpl_;
    pimpl_ = NULL;
}

Date::Date (const Date& d) : pimpl_(new Date::Impl (*d.pimpl_)) { }

Date& Date::operator= (const Date& d) {
    Date copy(d);
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
    char comma;

    is >> day >> month >> comma >> year;

    // check the fail bit of istream
    if (!is.fail()) {
        d = Date(day, month, year);
    }

    return is;
}

// Comparison Operators

bool operator== (const Date& a, const Date& b) {
    if (a.year() != b.year()) {
        return false;
    }
    if (a.month().compare(b.month()) != 0) {
        return false;
    }
    if (a.day() != b.day()) {
        return false;
    }
    return true;
}

bool operator!= (const Date& a, const Date& b) {
    return !(a == b);
}

bool operator< (const Date& a, const Date& b) {
    //check year first, then month, then day
    if (a.year() < b.year()) {
        return true;
    }
    else if (a.year() == b.year()) {
        int a_month = date_helpers::get_index(a.month());
        int b_month = date_helpers::get_index(b.month());
        if (a_month < b_month) {
            return true;
        }
        else if (a_month == b_month) {
            return a.day() < b.day();
        }
    }
    return false;
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

// Other member functions

Date incDays (const Date& d, long val) {
    // work with absolute number of days
    int new_year = d.year();
    int new_day = date_helpers::get_total_days(d.month(), date_helpers::is_leap_year(new_year)) + d.day();
    int days_in_year;
    std::string new_month;

    // go through the years and decrement val while incrementing years
    while (val + new_day > 0) {
        days_in_year = 365;
        if (date_helpers::is_leap_year(new_year)) {
            days_in_year++;
        }

        // if we are in the right year
        if (val + new_day <= days_in_year) {
            // get the correct absolute day
            new_day = val + new_day;
            // take the month out of the absolute day
            new_month = date_helpers::month_name[date_helpers::get_index_from_days(new_day, date_helpers::is_leap_year(new_year))].first;
            new_day = new_day - date_helpers::get_total_days(new_month, date_helpers::is_leap_year(new_year));
            return Date(new_day, new_month, new_year);
        }
        else {
            // decrement val and increment years
            val -= days_in_year;
            new_year++;
        }
    }

    return Date(d);
}

Date incMonths (const Date& d, int val) {
    // start month from January
    val = date_helpers::get_index(d.month()) + val;
    int years = d.year() + (val / 12);
    std::string new_month = date_helpers::month_name[val % 12].first;

    int new_day = date_helpers::get_days(new_month);
    if (date_helpers::is_leap_year(years) && new_month.compare("February") == 0) {
        new_day = 29;
    }
    if (d.day() < new_day) {
        new_day = d.day();
    }

    return Date(new_day, new_month, years);
}

Date incYears (const Date& d, int val) {
    // add the year
    int new_year = d.year() + val;

    // check if previous year was a leap year and the date was a leap day
    int new_day = d.day();
    if (date_helpers::is_leap_year(d.year()) && !date_helpers::is_leap_year(new_year)) {
        if (d.month().compare("February") == 0 && new_day == 29) {
            new_day = 28;
        }
    }
    return Date(new_day, d.month(), new_year);
}

Date Date::today() {
    // get the raw time from OS
    time_t rawtime;
    struct tm* timeinfo;
    time (&rawtime);

    // convert seconds into a tm struct
    timeinfo = localtime (&rawtime);

    return Date(timeinfo->tm_mday, date_helpers::month_name[timeinfo->tm_mon].first, timeinfo->tm_year);
}
    
