module;

#include <string>
#include <string_view>
#include <sstream>
#include <vector>

import Types;

export module SqlStatementBuilder;

namespace project {
export struct Field final {
    using string = String;
    char delimiter = ' ';
    string name;
    string options;  // e.g. "VARCHAR(255) NOT NULL", "INTEGER PRIMARY KEY"

    const string& Name() const {
        return name;
    }
    string Definition() const {
        std::ostringstream sql;
        sql << name << " " << options;
        return sql.str(); 
    }
};

// template <class Entity, class Mapping>
export class SqlStatementBuilder
{
public:
    using Fields = std::vector<Field>;
    /*
        CREATE TABLE IF NOT EXISTS products (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100),
                quantity INTEGER,
                price DECIMAL
            );
    */
    String CreateTable(const Fields& fields) {
        std::ostringstream sql;
        sql << "CREATE TABLE IF NOT EXISTS " << table_name_ << " (";
        for (size_t i = 0; i < fields.size(); ++i) {
            sql << fields[i].name << " " << fields[i].options;
            if (i < fields.size() - 1) {
                sql << ", ";
            }
        }
        sql << '\n' << options_;
        sql << ");";
        return sql.str();
    }
    /*
        INSERT INTO table1 (column1, column2, …)
        VALUES (value1, value2, …);
    */
    String Insert(const Fields& fields) {
        std::ostringstream sql;
        sql << "INSERT INTO " << table_name_ << " (";

        // Field names
        for (size_t i = 0; i < fields.size(); ++i) {
            sql << fields[i];
            if (i < fields.size() - 1) {
                sql << ", ";
            }
        }

        sql << ") VALUES (";

        // Placeholders for values
        for (size_t i = 0; i < fields.size(); ++i) {
            sql << "$" << i + 1;
            if (i < fields.size() - 1) {
                sql << ", ";
            }
        }

        sql << ");";
        return sql.str();
    }
    /*
        
    */
    String SelectAll() {
        std::ostringstream sql;
        sql << "SELECT * FROM " << table_name_ << ';';
        return sql.str();
    }
    /*
        UPDATE table_name_
        SET column1 = value1,
            column2 = value2,
            ...
        WHERE condition;
    */
    String Update(const Fields& fields, const String& condition) {
        std::ostringstream sql;
        sql << "UPDATE " << table_name_ << " SET ";

        for (size_t i = 0; i < fields.size(); ++i) {
            sql << fields[i].Name() << " = " << fields[i].Value();
            if (i < fields.size() - 1) {
                sql << ", ";
            }
        }

        if (!condition.empty()) {
            sql << " WHERE " << condition;
        }

        sql << ";";
        return sql.str();


        std::ostringstream sql;

        sql << "UPDATE " << table_name_ << " SET ";

        UpdateStatementFieldBuilder fieldBuilder(sql);
        Mapping::accept(fieldBuilder, _dummy_entity);

        long pos = sql.tellp();
        sql.seekp(pos - 1); // remove last comma

        sql << " WHERE id=?";

        return sql.str();
    }
    /*
        
    */
    String Delete(const String& condition) {
        std::ostringstream sql;
        sql << "DELETE FROM " << table_name_;

        if (!condition.empty()) {
            sql << " WHERE " << condition;
        }

        sql << ";";
        return sql.str();
    }
private:
    String table_name_;
    String options_;
};

} // namespace project