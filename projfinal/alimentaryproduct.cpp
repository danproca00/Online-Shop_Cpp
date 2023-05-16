#include <iostream>

#include "alimentaryproduct.hpp"

AlimentaryProduct::AlimentaryProduct() {}

AlimentaryProduct::AlimentaryProduct(
    std::string barcode,
    std::string name,
    double price,
    int amount,
    int calories,
    Date expiration
) : Product(barcode, name, price, amount)
  , calories_(calories)
  , expiration_(expiration) {
}

Product* AlimentaryProduct::make_copy() {
    return new AlimentaryProduct(barcode_, name_, price_, amount_, calories_, expiration_);
}

int AlimentaryProduct::getCalories() const {
    return this->calories_;
}

void AlimentaryProduct::setCalories(int calories) {
    this->calories_ = calories;
}

Date AlimentaryProduct::getExpiration() const {
    return this->expiration_;
}

void AlimentaryProduct::setExpiration(Date expiration) {
    this->expiration_ = expiration;
}

void AlimentaryProduct::print(std::ostream& os) const {
    os << name_ << " [ " << barcode_ << " ] " << price_ << " RON "
        << amount_ << " buc. " << calories_ << " kcal / 100g exp: "
        << expiration_;
}

std::ostream& operator<<(std::ostream& os, const AlimentaryProduct& p) {
    p.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, AlimentaryProduct& p) {
    std::getline(is, p.name_);
    std::getline(is, p.barcode_);
    is >> p.price_ >> p.amount_ >> p.calories_ >> p.expiration_;
    return is;
}
