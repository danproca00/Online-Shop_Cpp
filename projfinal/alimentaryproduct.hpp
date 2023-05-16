#ifndef __ALIMENTARY_PRODUCT__HPP
#define __ALIMENTARY_PRODUCT__HPP

#include <string>

#include "date.hpp"
#include "product.hpp"

class AlimentaryProduct : public Product {
private:
    int calories_;
    Date expiration_;

protected:
    void print(std::ostream&) const;

public:
    Product* make_copy();
    AlimentaryProduct();
    AlimentaryProduct(std::string, std::string, double, int, int, Date);
    int getCalories() const;
    void setCalories(int);
    Date getExpiration() const;
    void setExpiration(Date);
    friend std::ostream& operator<<(std::ostream&, const AlimentaryProduct&);
    friend std::istream& operator>>(std::istream&, AlimentaryProduct&);
};

#endif // __ALIMENTARY_PRODUCT__HPP
