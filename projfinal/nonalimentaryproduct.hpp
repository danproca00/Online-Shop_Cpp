#ifndef __NON_ALIMENTARY_PRODUCT__HPP
#define __NON_ALIMENTARY_PRODUCT__HPP

#include <string>

#include "product.hpp"

class NonalimentaryProduct : public Product {
private:
    std::string instructions_;

protected:
    void print(std::ostream&) const;

public:
    Product* make_copy();
    NonalimentaryProduct();
    NonalimentaryProduct(std::string, std::string, double, int, std::string);
    std::string getInstructions() const;
    void setInstructions(std::string);
    friend std::ostream& operator<<(std::ostream&, const NonalimentaryProduct&);
    friend std::istream& operator>>(std::istream&, NonalimentaryProduct&);
};

#endif // __NON_ALIMENTARY_PRODUCT__HPP
