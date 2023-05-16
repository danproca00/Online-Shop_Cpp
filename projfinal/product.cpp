#include <iostream>

#include "product.hpp"

Product::Product() {}

Product::Product(std::string barcode, std::string name, double price, int amount)
    : barcode_(barcode)
    , name_(name)
    , price_(price)
    , amount_(amount) {
}

Product* Product::make_copy() {
    return new Product(barcode_, name_, price_, amount_);
}

std::string Product::getBarcode() const {
    return this->barcode_;
}

void Product::setBarcode(std::string barcode) {
    this->barcode_ = barcode;
}

std::string Product::getName() const {
    return this->name_;
}

void Product::setName(std::string name) {
    this->name_ = name;
}

double Product::getPrice() const {
    return this->price_;
}

void Product::setPrice(double price) {
    this->price_ = price;
}

int Product::getAmount() const {
    return this->amount_;
}

void Product::setAmount(int amount) {
    this->amount_ = amount;
}

void Product::print(std::ostream& os) const {
    os << name_ << " [ " << barcode_ << " ] " << price_ << " RON "
        << amount_ << " buc.";
}

std::ostream& operator<<(std::ostream& os, const Product& p) {
    p.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Product& p) {
    std::getline(is, p.name_);
    std::getline(is, p.barcode_);
    is >> p.price_ >> p.amount_;
    return is;
}
