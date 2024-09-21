import Types;

export module Models;

namespace project {

export struct Store final {
    int id;
    String name;
    String address;
};

export struct Product final {
    // enum class Unit{};
    int id;
    String name;
    String unit;
    double price;
};

export struct Stock final {
    int store_id;
    int product_id;
    double count;
};

export struct Sale final {
    DateTime date;
    int id;
    int store_id;
    int product_id;
    int seller_Id;
    int count;
};

export struct Seller final {
    int id;
    int store_id;
    int department_id;
    int passport_id;
};

export struct Department final {
    int id;
    int store_id;
    String name;
};

export struct Passport final {
    int id;
    int number;
    String first_name;
    String middle_name;
    String last_name;
    Date birth_date;
    String address;
    Date issue_date;
    String issued_by;
};

} // namespace project