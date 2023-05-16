#ifndef __ORDER__HPP
#define __ORDER__HPP

#include <string>
#include <vector>

#include "date.hpp"
#include "product.hpp"

class Order {
private:
    std::vector<Product*> products_;
    Date date_;
    std::string customer_;

public:
    Order();
    Order(std::vector<Product*>, Date, std::string);
    std::vector<Product*> getProducts() const;
    void setProducts(std::vector<Product*>);
    Date getDate() const;
    void setDate(Date);
    std::string getCustomer() const;
    void setCustomer(std::string);
    friend std::ostream& operator<<(std::ostream&, const Order&);
    friend std::istream& operator>>(std::istream&, Order&);
};

#endif // __ORDER__HPP
