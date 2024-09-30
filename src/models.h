#ifndef MODELS_H
#define MODELS_H

#include <ostream>
#include <limits>
#include "types.h"

namespace project
{

    struct Store final
    {
        int id;
        String name;
        String address;
    };

    struct Department final
    {
        int id;
        String name;
    };

    struct Seller final
    {
        int id;
        int store_id;
        int department_id;
        String number;
        Date birth_date;
        String first_name;
        String middle_name;
        String last_name;
        String address;
        Date issue_date;
        String issued_by;
    };

    struct Receipt final {
        int id;
        int seller_Id;
        Timestamp date;
    };

    // struct Stock final 
    // {
    //     int store_id;
    //     String name;
    //     String address;           
    //     int product_id;
    //     String name;
    //     String unit;
    //     double price;
    //     double count;
    // };
     
    struct Product final 
    {
        int id;      
        int store_id;
        String name;
        String unit;
        double price;
        double count;
    };

    struct Sale final
    {
        int id;
        int receipt_id;        
        int product_id;
        double count;        
    };

    //     struct Stock final
    // {
    //     int id;
    //     int store_id;     
    // };

    /*std::ostream& operator<<(std::ostream& os, const Passport& entity)
    {
        os << "number :" << entity.number << '\n';
        os << "birth date :" << entity.birth_date << '\n';
        os << "first name :"  << entity.first_name << '\n';
        os << "middle name :"   << entity.middle_name << '\n';
        os << "last name :"    << entity.last_name << '\n';
        os << "address :"     << entity.address << '\n';
        os << "issue date :"      << entity.issue_date << '\n';
        os << "issued by :"       << entity.issued_by;
        return os;
    }
    std::istream& operator>>(std::istream& in, Passport& passport) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "enter passport number: ";
        in >> passport.number;
        in.ignore();

        std::cout << "enter birth date (YYYY-MM-DD): ";
        std::getline(in, passport.birth_date);

        std::cout << "enter first name: ";
        std::getline(in, passport.first_name);

        std::cout << "enter middle name: ";
        std::getline(in, passport.middle_name);

        std::cout << "enter last name: ";
        std::getline(in, passport.last_name);

        std::cout << "enter address: ";
        std::getline(in, passport.address);

        std::cout << "enter issue date (YYYY-MM-DD): ";
        std::getline(in, passport.issue_date);

        std::cout << "enter issued by: ";
        std::getline(in, passport.issued_by);

        return in;
    }*/

    std::ostream &operator<<(std::ostream &os, const Department &entity)
    {
        os << "name: " << entity.name;
        return os;
    }

    std::istream& operator>>(std::istream& in, Department& department) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "enter department name: ";
        std::getline(in, department.name);
        return in;
    }

    std::ostream &operator<<(std::ostream &os, const Product &entity)
    {
        os << "name: "  << entity.name << "\nunit: " << entity.unit << "\nprice: " << entity.price;
        return os;
    }

    std::istream& operator>>(std::istream& in, Product& product) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "enter product name: ";
        std::getline(in, product.name);

        std::cout << "enter product unit: ";
        std::getline(in, product.unit);

        std::cout << "enter product price: ";
        in >> product.price;

        return in;
    }

    std::ostream &operator<<(std::ostream &os, const Store &entity)
    {
        os << "name: " << entity.name << "\naddress: " << entity.address;
        return os;
    }

    std::istream& operator>>(std::istream& in, Store& entity) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "enter name: ";
        std::getline(in, entity.name);

        std::cout << "enter address: ";
        std::getline(in, entity.address);

        return in;
    }

} // namespace project

#endif // MODELS_H