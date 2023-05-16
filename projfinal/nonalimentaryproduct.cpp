#include <iostream>

#include "nonalimentaryproduct.hpp"

NonalimentaryProduct::NonalimentaryProduct() {}

NonalimentaryProduct::NonalimentaryProduct(
    std::string barcode,
    std::string name,
    double price,
    int amount,
    std::string instructions
) : Product(barcode, name, price, amount)
  , instructions_(instructions) {
}

Product* NonalimentaryProduct::make_copy() {
    return new NonalimentaryProduct(barcode_, name_, price_, amount_, instructions_);
}

std::string NonalimentaryProduct::getInstructions() const {
    return this->instructions_;
}
void NonalimentaryProduct::setInstructions(std::string instructions) {
    this->instructions_ = instructions;
}

void NonalimentaryProduct::print(std::ostream& os) const {
    os << name_ << " [ " << barcode_ << " ] " << price_ << " RON "
        << amount_ << " buc. " << '\n' << "Instructiuni de folosire: " << instructions_;
}

std::ostream& operator<<(std::ostream& os, const NonalimentaryProduct& p) {
    p.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, NonalimentaryProduct& p) {
    std::getline(is, p.name_);
    std::getline(is, p.barcode_);
    is >> p.price_ >> p.amount_;
    is.ignore();
    std::getline(is, p.instructions_);
    return is;
}
