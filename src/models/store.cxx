module;
#include <string>

// first thing after the Global module fragment must be a module command
export module store;

struct Store {
    using string = std::string;
    int id;
    string name;
    string adress;
};

struct Product {
    // enum class Unit{};
    using string = std::string;
    int id;
    string name;
    string unit;
    double price;
};

struct StoreProduct {
    using string = std::string;
    int id;
    int store_id;
    int product_id;
    double count;
};

struct Sales {
    using datetime = std::string;
    int id;
    datetime date;
    int store_id;
    int product_id;
    int seller_Id;
    int count;
};

struct Seller {
    using string = std::string;
    using date = std::string;
    int id;
    int store_id;
    int department_id;
    int passport_id;
};

struct Department {
    using string = std::string;
    int id;
    int store_id;
    string name;
};

struct Passport {
    using string = std::string;
    using date = std::string;
    int id;
    int number;
    string first_name;
    string middle_name;
    string last_name;
    date birth_date;
    string address;
    date issue_date;
    string issued_by;
};