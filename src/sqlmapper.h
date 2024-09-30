#ifndef SQLMAPPER_H
#define SQLMAPPER_H

#include <iostream>
#include <ranges>
#include <stdexcept>

#include <pqxx/pqxx>

#include "types.h"
#include "models.h"
#include "sqlstatementbuilder.h"
#include "dbconfig.h"

namespace project {

using ConstSqlRowRef = pqxx::row const&;

template<class Mapper>
class PostgreSqlAdapter {
public:
	using Entity = Mapper::Entity;
	PostgreSqlAdapter(pqxx::connection& connection, SqlStatementBuilder const& sql_builder) : connection_(connection), sql_builder_(sql_builder)
	{
		if (!ExisTable())
			Run(mapper_.CreateTable());
	}
	using Entities = std::vector<Entity>;
	auto Insert(Entity const& entity) {
		auto query = sql_builder_.Insert(mapper_.GetFields(entity));
		return Run(query);
	}
	bool Update(Entity const & entity) {
		auto query = sql_builder_.Update(mapper_.GetFields(entity));
		return Run(query);
	}
	bool Delete(Entity const& entity) {
		auto query = sql_builder_.Delete(mapper_.GetFields(entity));
		Run(query);
	}
	[[nodiscard]] Entities Select(String const& condition) {
		auto query = sql_builder_.SelectByCondition(condition);
		Entities entities;
		try {
			auto result = Execute(query);
			for (auto const& row : result)
				entities.push_back(mapper_.GetEntity(row));
		}
		catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;
		}
		return entities;
	}
	[[nodiscard]] Entities Select(int limit, int offset) {
		auto query = sql_builder_.SelectPagging(limit, offset);
		Entities entities;
		try {
			auto result = Execute(query);
			for (auto const& row : result)
				entities.push_back(mapper_.GetEntity(row));
		}
		catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;
		}
		return entities;
	}
	
	// [[nodiscard]] Entities Select() {
	// 	auto query = sql_builder_.SelectAll();
	// 	Entities entities;
	// 	try {
	// 		auto result = Execute(query);
	// 		for (auto const& row : result)
	// 			entities.push_back(mapper_.GetEntity(row));
	// 	}
	// 	catch (std::exception const& e) {
	// 		std::cerr << e.what() << std::endl;
	// 	}
	// 	return entities;
	// }

	//[[nodiscard]] Entities CustomSelect(String const& query) {
	//	Entities entities;
	//	try {
	//		auto result = Execute(query);
	//		for (auto const& row : result)
	//			entities.push_back(mapper_.GetEntity(row));
	//	}
	//	catch (std::exception const& e) {
	//		std::cerr << e.what() << std::endl;
	//	}
	//	return entities;
	//}
	pqxx::result Execute(String const& query) {
			pqxx::work work(connection_);
			auto result = work.exec(query);
			work.commit();
			return result;
	}
private:
	// Entities Convert(pqxx::result& result)
	bool ExisTable() {
		auto query = sql_builder_.ExistTable();
		try {
			auto result = Execute(query);
			auto [is_table_exist] = result[0].as<bool>();
			if (is_table_exist)
				return true;
		} catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;			
		}
		return false;
	}
	bool Run(String const& query) {
		try {
			pqxx::work work(connection_);
			work.exec(query);
			work.commit();
		}
		catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;
			return false;
		}
		return true;
	}
	pqxx::connection& connection_;
	SqlStatementBuilder sql_builder_;
	Mapper mapper_;
};

struct StoreMapper {
	using Entity = Store;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.name = row["name"].as<String>();
		entity.address = row["address"].as<String>();		
		return entity;
	}
	[[nodiscard]] Fields GetFields(Entity const& entity) {
		return {
			{"id", std::to_string(entity.id)},
			{"name", '\'' + entity.name + '\''},
			{"address", '\'' + entity.address + '\''}
		};
	}
	String CreateTable() {
		const String create_check_ref_fn = "CREATE OR REPLACE FUNCTION check_store_references()\n"
                "RETURNS TRIGGER AS $$\n"
                "BEGIN\n"
                    "IF EXISTS (SELECT 1 FROM " + sqltable::name::sellers + " WHERE store_id = OLD.id) THEN\n"
                        "RAISE EXCEPTION 'This store is referenced by the " + sqltable::name::sellers + " table.';\n"
                    "END IF;\n"
                    "IF EXISTS (SELECT 1 FROM " + sqltable::name::stock + " WHERE store_id = OLD.id) THEN\n"
                        "RAISE EXCEPTION 'This store is referenced by the " + sqltable::name::stock + " table.';\n"
                    "END IF;\n"
                    "RETURN OLD;\n"
                "END;\n"
                "$$ LANGUAGE plpgsql;\n";
		const String create_trigger = "CREATE OR REPLACE TRIGGER check_store_references_trigger\n"
                "BEFORE DELETE ON\n" + sqltable::name::stores + '\n' +
                "FOR EACH ROW\n"
                "EXECUTE FUNCTION check_store_references();\n";

		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::stores + "(\n"
			"id SERIAL PRIMARY KEY,\n"
			"name VARCHAR(255) NOT NULL,\n"
			"address VARCHAR(255) NOT NULL);\n" + create_check_ref_fn + create_trigger;
	}
};

struct ProductMapper {
	using Entity = Product;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.store_id = row["store_id"].as<int>();
		entity.name = row["name"].as<String>();
		entity.unit = row["unit"].as<String>();
		entity.price = row["price"].as<double>();
		entity.count = row["count"].as<double>();		
		return entity;
	}
	[[nodiscard]] Fields GetFields(Entity const& entity) {
		return {
			{"id", std::to_string(entity.id)},
			{"store_id", std::to_string(entity.store_id)},
			{"name", '\'' + entity.name + '\''},
			{"unit", '\'' + entity.unit + '\''},
			{"count", std::to_string(entity.count)},
			{"price", std::to_string(entity.price)},			
		};
	}
	String CreateTable() {		
		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::products + "("
			"id SERIAL PRIMARY KEY,"
			"store_id INT NOT NULL,"
			"name VARCHAR(255) NOT NULL,"
			"unit VARCHAR(50) NOT NULL,"
			"count DOUBLE PRECISION NOT NULL,"
			"price DOUBLE PRECISION NOT NULL);";
	}
};

//struct StockMapper {
//	using Entity = Stock;
//	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
//		Entity entity;
//		entity.store_id = row["store_id"].as<int>();
//		entity.product_id = row["product_id"].as<int>();
//		entity.count = row["count"].as<double>();
//		return entity;
//	}
//	[[nodiscard]] Fields GetFields(Entity const& entity) {
//		return {
//			{"store_id", std::to_string(entity.store_id)},
//			{"product_id", std::to_string(entity.product_id)},
//			{"count", std::to_string(entity.count)}
//		};
//	}
//	static String Insert(Entity const& entity) {
//		return "INSERT INTO stock (store_id, product_id, count) VALUES ("+std::to_string(entity.store_id)+","+std::to_string(entity.product_id)+","+std::to_string(entity.count)+")";
//	}
//	String CreateTable() {
//		return "CREATE TABLE IF NOT EXISTS "+ sqltable::name::stock +" ("
//			"store_id INT NOT NULL,"
//			"product_id INT NOT NULL,"
//			"count DOUBLE PRECISION NOT NULL,"
//			"PRIMARY KEY(store_id, product_id),"
//			"FOREIGN KEY(store_id) REFERENCES " + sqltable::name::stores + "(id) ON DELETE CASCADE,"
//			"FOREIGN KEY(product_id) REFERENCES " + sqltable::name::products + "(id) ON DELETE CASCADE"
//			");";
//	}
//
//	[[nodiscard]] static String ProductsIn(int store_id) {
//		/*return "SELECT *"
//		"FROM products"
//		"JOIN stock ON products.id = stock.product_id"
//		"WHERE stock.store_id = stores."+std::to_string(store_id)+";";*/
//		return "products.id IN ("
//			"SELECT stock.product_id "
//			"FROM stock "
//			"WHERE stock.store_id = " + std::to_string(store_id) +
//			");";
//	}
//
//	[[nodiscard]] static String ProductsNotIn(int store_id) {
//		/*return "SELECT *"
//		"FROM products"
//		"WHERE products.id NOT IN ("
//			"SELECT stock.product_id"
//			"FROM stock"
//			"WHERE stock.store_id = "+ std::to_string(store_id) +
//		");";*/
//		return "products.id NOT IN ("
//			"SELECT stock.product_id "
//			"FROM stock "
//			"WHERE stock.store_id = " + std::to_string(store_id) +
//			");";
//	}
//};

struct StockMapper {
	using Entity = Product;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.name = row["name"].as<String>();
		return entity;
	}
	String Insert(Entity const& entity) {
		
	}

	int store_id;
};

struct DepartmentMapper {
	using Entity = Department;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.name = row["name"].as<String>();
		return entity;
	}
	[[nodiscard]] Fields GetFields(Entity const& entity) {
		return {
			{"id", std::to_string(entity.id)},
			{"name", '\'' + entity.name + '\''}
		};
	}
	String CreateTable() {
		const String create_check_ref_fn = "CREATE OR REPLACE FUNCTION check_department_references()\n"
                "RETURNS TRIGGER AS $$\n"
                "BEGIN\n"
                    "IF EXISTS (SELECT 1 FROM " + sqltable::name::sellers + " WHERE department_id = OLD.id) THEN\n"
                        "RAISE EXCEPTION 'This store is referenced by the " + sqltable::name::sellers + " table.';\n"
                    "END IF;\n"
                    "RETURN OLD;\n"
                "END;\n"
                "$$ LANGUAGE plpgsql;\n";
		const String create_trigger = "CREATE OR REPLACE TRIGGER check_department_references_trigger\n"
                "BEFORE DELETE ON " + sqltable::name::departments + '\n' +
                "FOR EACH ROW\n"
                "EXECUTE FUNCTION check_department_references();\n";

		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::departments + "("
			"id SERIAL PRIMARY KEY,"
			"name VARCHAR(255) NOT NULL);\n" + create_check_ref_fn + create_trigger;
	}
};

//struct PassportMapper {
//	using Entity = Passport;
//	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
//		Entity entity;
//		entity.id = row["id"].as<int>();
//		entity.number = row["number"].as<String>();
//		entity.first_name = row["first_name"].as<String>();
//		entity.middle_name = row["middle_name"].as<String>();
//		entity.last_name = row["last_name"].as<String>();
//		entity.address = row["address"].as<String>();
//		entity.issued_by = row["issued_by"].as<String>();
//		entity.birth_date = row["birth_date"].as<Date>();
//		entity.issue_date = row["issue_date"].as<Date>();
//		return entity;
//	}
//	[[nodiscard]] Fields GetFields(Entity const& entity) {
//		return {
//			{"id", std::to_string(entity.id)},
//			{"number", '\'' + entity.number + '\''},
//			{"first_name", '\'' + entity.first_name + '\''},
//			{"middle_name", '\'' + entity.middle_name + '\''},
//			{"last_name", '\'' + entity.last_name + '\''},
//			{"address", '\'' + entity.address + '\''},
//			{"issued_by", '\'' + entity.issued_by + '\''},
//			{"birth_date", '\'' + entity.birth_date + '\''},
//			{"issue_date", '\'' + entity.issue_date + '\''}
//		};
//	}
//	String CreateTable() {
//		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::passports + "("
//			"id SERIAL PRIMARY KEY,"
//			"number VARCHAR(255) UNIQUE NOT NULL,"
//			"first_name VARCHAR(255) NOT NULL,"
//			"middle_name VARCHAR(255),"
//			"last_name VARCHAR(255) NOT NULL,"
//			"birth_date DATE NOT NULL,"
//			"address VARCHAR(255) NOT NULL,"
//			"issue_date DATE NOT NULL,"
//			"issued_by VARCHAR(255) NOT NULL);";
//	}
//	static String ConditionByNumber(Entity const& entity) {
//		return "number = '"+ entity.number + "'";
//	}
//	static String ConditionById(int id) {
//		return "id = "+ id;
//	}
//};

struct SellerMapper {
	using Entity = Seller;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.store_id = row["store_id"].as<int>();
		entity.department_id = row["department_id"].as<int>();
		//entity.passport_id = row["passport_id"].as<int>();
		return entity;
	}
	[[nodiscard]] Fields GetFields(Entity const& entity) {
		return {
			{"id", std::to_string(entity.id)},
			{"store_id", std::to_string(entity.store_id)},
			{"department_id", std::to_string(entity.department_id)},
			//{"passport_id", std::to_string(entity.passport_id)}
		};
	}
	String CreateTable() {
		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::sellers + "("
			"id SERIAL PRIMARY KEY,"
			"store_id INT NOT NULL,"
			"department_id INT NOT NULL,"
			//"passport_id INT NOT NULL,"
			"FOREIGN KEY(store_id) REFERENCES " + sqltable::name::stores + "(id) ON DELETE CASCADE,"
			"FOREIGN KEY(department_id) REFERENCES " + sqltable::name::departments + "(id) ON DELETE CASCADE,"
			"FOREIGN KEY(passport_id) REFERENCES " + sqltable::name::passports + "(id) ON DELETE CASCADE);";
	}
};

struct SaleMapper {
	using Entity = Sale;
	[[nodiscard]] Entity GetEntity(ConstSqlRowRef row) {
		Entity entity;
		entity.id = row["id"].as<int>();
		entity.product_id = row["product_id"].as<int>();
		//entity.seller_Id = row["seller_Id"].as<int>();
		//entity.date = row["date"].as<Timestamp>();
		entity.count = row["count"].as<double>();
		return entity;
	}
	[[nodiscard]] Fields GetFields(Entity const& entity) {
		return {
			{"id", std::to_string(entity.id)},
			{"product_id", std::to_string(entity.product_id)},
			//{"seller_Id", std::to_string(entity.seller_Id)},
			//{"date", '\'' + entity.date + '\''},
			{"count", std::to_string(entity.count)}
		};
	}
	String CreateTable() {
		// TODO cur_store_id INT to SERIAL

		// const String create_decrease_stock_fn = "CREATE OR REPLACE FUNCTION decrease_stock_count()\n"
		// "RETURNS TRIGGER AS $$\n"
		// "DECLARE\n"
		// 	"cur_stock DOUBLE PRECISION;\n"
		// 	"cur_store_id INT;\n"
		// "BEGIN\n"
		// 	"SELECT store_id INTO cur_store_id\n"
		// 	"FROM sellers\n"
		// 	"WHERE id = NEW.seller_id;\n"
		// 	"-- Получаем текущее количество товара на складе\n"
		// 	"SELECT count INTO cur_stock\n"
		// 	"FROM stock\n"
		// 	"WHERE store_id = cur_store_id\n"
		// 	"AND product_id = NEW.product_id;\n"
		// 	"-- Проверяем, не станет ли количество товара отрицательным\n"
		// 	"IF cur_stock < NEW.count THEN\n"
		// 		"RAISE EXCEPTION 'Insufficient stock for product_id % in store_id %', NEW.product_id, cur_store_id;\n"
		// 	"END IF;\n"
		// 	"-- Уменьшаем количество товара в таблице stock\n"
		// 	"UPDATE stock\n"
		// 	"SET count = count - NEW.count\n"
		// 	"WHERE store_id = cur_store_id\n"
		// 	"AND product_id = NEW.product_id;\n"
		// 	"RETURN NEW;\n"
		// "END;\n"
		// "$$ LANGUAGE plpgsql;\n";

		// const String create_trigger = "CREATE OR REPLACE TRIGGER trigger_decrease_stock\n"
		// "AFTER INSERT ON sales\n"
		// "FOR EACH ROW\n"
		// "EXECUTE FUNCTION decrease_stock_count();\n";

		return "CREATE TABLE IF NOT EXISTS " + sqltable::name::sales + "("
			"id SERIAL PRIMARY KEY,"
			"date TIMESTAMP NOT NULL,"
			"product_id INT NOT NULL,"
			"seller_id INT NOT NULL,"
			"count DOUBLE PRECISION NOT NULL,"
			"FOREIGN KEY(product_id) REFERENCES " + sqltable::name::products + "(id) ON DELETE CASCADE,"
			"FOREIGN KEY(seller_id) REFERENCES " + sqltable::name::sellers + "(id) ON DELETE CASCADE);\n"; // + create_decrease_stock_fn + create_trigger;
	}
};

namespace adapter {
	pqxx::connection connection("user=admin password=admin host=localhost port=5432 dbname=store_db");
	PostgreSqlAdapter<StoreMapper> stores{ connection, sqltable::builder::stores };
	PostgreSqlAdapter<ProductMapper> products{ connection, sqltable::builder::products };
	//PostgreSqlAdapter<StockMapper> stock{ connection, sqltable::builder::stock };
	PostgreSqlAdapter<DepartmentMapper> departments{ connection, sqltable::builder::departments };
	//PostgreSqlAdapter<PassportMapper> passports{ connection, sqltable::builder::passports };
	PostgreSqlAdapter<SellerMapper> sellers{ connection, sqltable::builder::sellers };
	PostgreSqlAdapter<SaleMapper> sales{ connection, sqltable::builder::sales };
} // namespace adapter

} // namespace project

#endif // SQLMAPPER_H