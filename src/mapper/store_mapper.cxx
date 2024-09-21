#include <iostream>
#include <pqxx/pqxx>
#include <exception>


class StoreMapper
{
public:
    bool CreateTable(pqxx::connection& connection)
    {
        try
        {
            pqxx::work work(connection);
            work.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
            work.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    bool SelectAll(pqxx::connection& connection)
    {
        try
        {
            pqxx::work work(connection);
            work.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
            work.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    bool Insert(pqxx::connection& connection)
    {
        try
        {
            pqxx::work work(connection);
            work.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
            work.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    bool Update(pqxx::connection& connection)
    {
        try
        {
            pqxx::work work(connection);
            work.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
            work.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    bool Delete(pqxx::connection& connection)
    {
        try
        {
            pqxx::work work(connection);
            work.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
            work.commit();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    
private:
};