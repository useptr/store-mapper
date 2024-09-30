#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint>

namespace project {
    using String = std::string;
    using Timestamp = std::string; // TODO std::chrono::time_point https://github.com/jtv/libpqxx/blob/master/include/pqxx/doc/datatypes.md
    using Date = std::string; // TODO std::chrono::year_month_day
    using BigInt = int8_t;
    using Field = std::pair<String, String>;
    using Fields = std::vector<Field>;
    //using ColumnName = String;
    using PrimaryKeySet = std::unordered_set<String>;
} // namespace project

#endif // TYPES_H