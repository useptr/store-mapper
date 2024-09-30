#ifndef SQLSTATEMENTBUILDER_H
#define SQLSTATEMENTBUILDER_H

#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <ranges>
#include <utility>

#include "types.h"

namespace project {

class SqlStatementBuilder {
public:
	SqlStatementBuilder(String table_name, PrimaryKeySet primary_key) : table_name_(std::move(table_name)), primary_key_(std::move(primary_key)) {}
	using SizeType = Fields::size_type;
	String Insert(Fields const& fields) const {
		std::ostringstream sql;
		sql << "INSERT INTO " << table_name_ << " (";
		auto last_index = fields.size() - 1;
		for (auto i : std::views::iota(SizeType{ 0 }, fields.size())) {
			auto const& [name, value] = fields[i];
			if (primary_key_.contains(name))
				continue;
			sql << name;
			if (i != last_index)
				sql  << ", ";
		}
		sql << ") VALUES (";
		for (auto i : std::views::iota(SizeType{ 0 }, fields.size())) {
			auto const& [name, value] = fields[i];
			if (primary_key_.contains(name))
				continue;
			sql << value;
			if (i != last_index)
				sql << ", ";
		}
		sql << ");";
		return sql.str();
	}
	String ExistTable() const {
		return "SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + table_name_ + "');";
	}
	String SelectPagging(int limit, int offset) const {
		std::ostringstream sql;
		sql << "SELECT * FROM " << table_name_ << " LIMIT " << limit << " OFFSET " << offset << ';';
		return sql.str();
	}
	String SelectAll() const {
		std::ostringstream sql;
		sql << "SELECT * FROM " << table_name_ << ';';
		return sql.str();
	}
	String SelectByCondition(String const & condition) const {
		std::ostringstream sql;
		sql << "SELECT * FROM " << table_name_ << " WHERE " << condition << ';';
		return sql.str();
	}
	String Update(Fields const & fields) const {
		std::ostringstream sql;
		sql << "UPDATE " << table_name_ << " SET ";
		SizeType pk_counter{ 0 };
		String condition;
		for (auto i : std::views::iota(SizeType{ 0 }, fields.size())) {
			auto const& [name, value] = fields[i];
			if (primary_key_.contains(name)) {
				++pk_counter;
				condition += name + " = " + value;
				if (pk_counter < primary_key_.size())
					condition += " AND ";		
				continue;
			}
			sql << name << " = " << value << ", ";
		}
		sql.seekp(-2, std::ios::end); // remove last comma		
		sql << " WHERE " << condition << ";";
		return sql.str();
	}
	String Delete(Fields const& fields) const {
		std::ostringstream sql;
		sql << "DELETE FROM " << table_name_ << " WHERE ";
		SizeType pk_counter{ 0 };
		for (auto i : std::views::iota(SizeType{ 0 }, fields.size())) {
			auto const& [name, value] = fields[i];
			if (!primary_key_.contains(name))
				continue;
				sql << name << " = " << value;
				if (pk_counter < primary_key_.size()) {
					sql << " AND ";
					++pk_counter;
				}			
		}
		sql << ";";
		return sql.str();
	}
private:
	String table_name_;
	PrimaryKeySet primary_key_;
};

} // namespace project

#endif // SQLSTATEMENTBUILDER_H