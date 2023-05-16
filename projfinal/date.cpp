#include <iostream>

#include "date.hpp"

Date::Date() {}

Date::Date(int day, int month, int year)
    : day_(day)
    , month_(month)
    , year_(year) {
}

int Date::getDay() const {
    return this->day_;
}

void Date::setDay(int day) {
    this->day_ = day;
}

int Date::getMonth() const {
    return this->month_;
}

void Date::setMonth(int month) {
    this->month_ = month;
}

int Date::getYear() const {
    return this->year_;
}

void Date::setYear(int year) {
    this->year_ = year;
}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.day_ << '/' << d.month_ << '/' << d.year_;
    return os;
}

std::istream& operator>>(std::istream& is, Date& d) {
    is >> d.day_ >> d.month_ >> d.year_;
    return is;
}
