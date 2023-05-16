#include <iostream>

#include "order.hpp"
#include "alimentaryproduct.hpp"
#include "nonalimentaryproduct.hpp"

Order::Order() {}

Order::Order(std::vector<Product*> products, Date date, std::string customer)
    : products_(products)
    , date_(date)
    , customer_(customer) {
}

std::vector<Product*> Order::getProducts() const {
    return this->products_;
}

void Order::setProducts(std::vector<Product*> products) {
    this->products_ = products;
}

Date Order::getDate() const {
    return this->date_;
}

void Order::setDate(Date date) {
    this->date_ = date;
}

std::string Order::getCustomer() const {
    return this->customer_;
}

void Order::setCustomer(std::string customer) {
    this->customer_ = customer;
}

std::ostream& operator<<(std::ostream& os, const Order& o) {
    os << "Ordered by " << o.customer_ << " on " << o.date_ << '\n';
    for (const auto& product : o.products_) {
        std::cout << *product << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, Order& o) {
    std::getline(is, o.customer_);
    is >> o.date_;
    int n; is >> n;
    is.ignore();
    std::string type;
    for (int i = 0; i < n; i++) {
        std::getline(is, type);
        if (type == "product") {
            Product* p = new Product();
            is >> *p;
            o.products_.push_back(p);
            is.ignore();
        } else if (type == "alimentaryproduct") {
            AlimentaryProduct* p = new AlimentaryProduct();
            is >> *p;
            o.products_.push_back(p);
            is.ignore();
        } else if (type == "nonalimentaryproduct") {
            NonalimentaryProduct* p = new NonalimentaryProduct();
            is >> *p;
            o.products_.push_back(p);
        }
    }
    return is;
}
