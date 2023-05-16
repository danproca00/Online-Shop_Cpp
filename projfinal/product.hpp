#ifndef __PRODUCT__HPP
#define __PRODUCT__HPP

#include <string>

class Product {
protected:
    std::string barcode_;
    std::string name_;
    double price_;
    int amount_;

    virtual void print(std::ostream&) const;

public:
    virtual Product* make_copy();
    Product();
    Product(std::string, std::string, double, int);
    std::string getBarcode() const;
    void setBarcode(std::string);
    std::string getName() const;
    void setName(std::string);
    double getPrice() const;
    void setPrice(double);
    int getAmount() const;
    void setAmount(int);
    friend std::ostream& operator<<(std::ostream&, const Product&);
    friend std::istream& operator>>(std::istream&, Product&);
};

#endif // __PRODUCT__HPP
