#ifndef MODELS_H
#define MODELS_H

#include <ostream>
#include "types.h"

namespace project {

 struct Store final {
    String name;
    String address;
};

 struct Product final {
    String name;
    String unit;
    double price;
    double count;
};

 struct Stock final {
    Product& product;
    double count;
};

 struct Department final {
    String name;
};

 struct Passport final {
    int number;
    Date birth_date;
    String first_name;
    String middle_name;
    String last_name;
    String address;
    Date issue_date;
    String issued_by;
};

 struct Seller final {
    int store;
    Department& department;
    Passport& passport;
};

 struct Sale final {
    Store& store;
    Product& product;
    Seller& seller;
    Timestamp date;
    double count;
};

} // namespace project

#endif // MODELS_H