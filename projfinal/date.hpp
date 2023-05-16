#ifndef __DATE__HPP
#define __DATE__HPP

class Date {
private:
    int day_;
    int month_;
    int year_;

public:
    Date();
    Date(int, int, int);
    int getDay() const;
    void setDay(int);
    int getMonth() const;
    void setMonth(int);
    int getYear() const;
    void setYear(int);
    friend std::ostream& operator<<(std::ostream&, const Date&);
    friend std::istream& operator>>(std::istream&, Date&);
};

#endif // __DATE__HPP
