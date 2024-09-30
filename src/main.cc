#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <any>
#include <tuple>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <limits>
#include <regex>
//#include "models.h"

#include "dbconfig.h"
#include "sqlmapper.h"


using namespace std;
using namespace project;

template<class T>
T ReadInRange(const T from, const T to) {
	T value = from-1;
	while(true) {
		value = Read<T>();
		if (value >= from && value <= to)
			break;
		std::cout << "index must be in the range from "<< from <<" to " << to <<'\n';
	}
	return value;
}

template<class T>
T Read() {
	T value;
	while (true) {
		std::cin >> value;
		if (!std::cin.fail())
			break;
		std::cout << "Enter again: ";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');				
	}
	return value;
}

template<class T>
void IndexedPrint(std::vector<T>const& vec) {
	for (auto i : std::views::iota(size_t{ 0 }, vec.size()))
		cout << '#' << i << '\n' << vec[i] << '\n';
}

template<class Adapter>
void ShowEntities(Adapter& adapter) {
	int page{ 0 };
	int max_page = std::numeric_limits<int>::max();
	const int limit{ 10 };
	int option{ -1 };
	auto entities = adapter.Select(1, 0);
	if (entities.empty()) {
		cout << "there is not a single entity\n";
		return;
	}
	while (true) {
		int offset = page * limit;
		entities = adapter.Select(limit, offset);
		if (entities.empty()) {
			max_page = page - 1;
			--page;
			offset = page * limit;
			entities = adapter.Select(limit, offset);
		}
		IndexedPrint(entities);
		cout << "0 - exit\n";
		cout << "1 - previous page\n";
		cout << "2 - next page\n";
		option = Read<int>();
		if (0 == option) {
			return;
		}
		else if (1 == option) {
			if (page > 0)
				--page;
		}
		else if (2 == option) {
			if (page <= max_page)
				++page;
		}
	}
}

template<class Adapter>
auto GetEntity(Adapter& adapter) -> Adapter::Entity {
	int page{ 0 };
	int max_page = std::numeric_limits<int>::max();
	const int limit{ 10 };
	int option{ -1 };
	auto entities = adapter.Select(1, 0);
	if (entities.empty())
		throw std::exception("there is not a single entity");
	//Adapter::Entity entity;
	while (true) {
		int offset = page * limit;
		entities = adapter.Select(limit, offset);
		if (entities.empty()) {
			max_page = page - 1;
			--page;
			offset = page * limit;
			entities = adapter.Select(limit, offset);
		}
		IndexedPrint(entities);
		cout << "1 - select entity\n";
		cout << "2 - previous page\n";
		cout << "3 - next page\n";
		option = Read<int>();
		if (1 == option) {
			cout << "enter entity index: ";
			auto index = ReadInRange<int>(0, entities.size() - 1);
			return entities[index];
		}
		else if (2 == option) {
			if (page > 0)
				--page;
		}
		else if (3 == option) {
			if (page <= max_page)
				++page;
		}
	}
	throw std::exception("there is not a single entity");
}

//bool IsValidDays(int month, int day, int year) {
//	if (month < 1 || day < 1 || year < 1)
//		return false;
//    // Количество дней в каждом месяце (индекс 0 не используется)
//    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//    if (month == 2) {
//		auto is_leap = [](int year) -> bool {
//			return (year%400==0) || ( (year%4==0) && (year%100!=0) );
//		};
//        if (is_leap(year))
//            return day <= 29;
//        return day <= 28;
//    }
//    return day <= days_in_month[month];
//}
//
//bool isValidDate(const std::string& date_str) {
//    std::regex date_pattern(R"(\d{2}-\d{2}-\d{2})");
//    if (!std::regex_match(date_str, date_pattern))
//        return false;    
//
//    std::istringstream iss(date_str);
//    int year, month, day;
//    char dash1, dash2;
//
//    iss >> year >> dash1 >> month >> dash2 >> day;
//
//    if (dash1 != '-' || dash2 != '-' || month < 1 || month > 12 || !IsValidDays(month, day, year))
//        return false;    
//    return true;
//}
//
//bool isValidTimestamp(const std::string& timestamp_str) {
//    std::regex timestamp_pattern(R"(\d{2}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
//    if (!std::regex_match(timestamp_str, timestamp_pattern))
//        return false;
//
//    std::istringstream iss(timestamp_str);
//    int year, month, day, hour, minute, second;
//    char dash1, dash2, space, colon1, colon2;
//
//    iss >> year >> dash1 >> month >> dash2 >> day >> space >> hour >> colon1 >> minute >> colon2 >> second;
//
//    if (dash1 != '-' || dash2 != '-' || space != ' ' ||
//        colon1 != ':' || colon2 != ':' ||
//        month < 1 || month > 12 ||
//        !IsValidDays(month, day, year) ||
//        hour < 0 || hour > 23 ||
//        minute < 0 || minute > 59 ||
//        second < 0 || second > 59) {
//        return false;
//    }
//
//    return true;
//}
//
//Timestamp CurrentTimestamp() {
//	auto now = std::chrono::system_clock::now();
//    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
//    std::tm now_tm = *std::localtime(&now_time_t);
//    std::ostringstream oss;
//    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
//    return oss.str();
//}
//

//
//struct Purshcase final {
//	int product_id;
//	double count;
//};
//
//void SaleMenu() {
//	auto sellers = adapter::sellers.Select();
//	if (sellers.empty()) {
//		cout << "there is not a single seller. add a seller\n";
//		return;
//	}
//	// show sellers
//	auto departments = adapter::departments.Select();
//	auto stores = adapter::stores.Select();
//	auto passports = adapter::passports.Select();
//	auto products = adapter::products.Select();
//	auto stocks = adapter::stock.Select();
//	auto find_by_id = [](auto& vec, int id) -> std::optional<std::decay_t<decltype(vec.front())>> {
//		auto it = std::ranges::find_if(vec, [id](auto const& item) {
//			return item.id == id;
//			});
//		if (it != vec.end()) {
//			return *it;
//		}
//		return std::nullopt;
//	};
//	for (auto i : std::views::iota(size_t{ 0 }, sellers.size())) {
//		cout << '#' << i << '\n';
//		auto seller = sellers[i];
//		auto passport = find_by_id(passports, seller.passport_id);
//		if (passport)
//			cout << "PASSPORT:\n" << *passport << '\n';
//		auto department = find_by_id(departments, seller.department_id);
//		if (department)
//			cout << "DEPARTMENT:\n" << *department << '\n';
//		auto store = find_by_id(stores, seller.passport_id);
//		if (store)
//			cout << "STORE:\n" << *store << '\n';
//		cout << '\n';
//	}
//	cout << "enter seller number: ";
//	int index = ReadInRange<int>(0, sellers.size());
//	auto seller = sellers[index];
//	auto stock_products = adapter::products.Select(StockMapper::ProductsIn(seller.store_id));
//	//auto stock = 
//	int option{ -1 };
//	while (option != 0) {		
//		std::vector<Stock> store_stock;
//		for (auto& stock : stocks) {
//			if (stock.count <= 0)
//				continue;
//			if (stock.store_id == seller.store_id)
//				store_stock.push_back(stock);
//		}
//		//cout << "cart:\n";
//		/*for (auto i : std::views::iota(size_t{ 0 }, cart.size())) {
//			cout << '#' << i << '\n';
//			auto purshcase = cart[i];
//		}*/
//		cout << "0 - exit\n";
//		cout << "1 - buy product\n";
//		//cout << "1 - add product to cart\n";
//		//cout << "2 - remove product from cart\n";
//		option = Read<int>();
//		if (1 == option) {
//			// show store stock products
//			for (auto i : std::views::iota(size_t{ 0 }, store_stock.size())) {
//				auto find_product_by_id = [](const std::vector<Product>& products, int stock_id) -> std::optional<Product> {
//					auto it = std::ranges::find_if(products, [&](const Product& product) {
//						return product.id == stock_id;
//						});
//
//					if (it != products.end()) {
//						return *it;
//					}
//					return std::nullopt;
//				};
//				auto stock = store_stock[i];
//				cout << '#' << i << '\n';
//				auto product = find_product_by_id(products, stock.product_id);
//				if (product)
//					cout << *product << '\n';
//				cout << "count: " << stock.count << '\n';
//			}
//			cout << "enter product in stock number\n";
//			int index = ReadInRange<int>(0, store_stock.size() - 1);
//			auto stock = store_stock[index];
//			cout << "enter product count\n";
//			auto purshcase_count = ReadInRange<double>(0, stock.count);
//			auto current_timestamp = CurrentTimestamp();
//			Sale sale{ -1, seller.id, stock.product_id, purshcase_count, current_timestamp };
//			adapter::sales.Insert(sale);
//			stocks = adapter::stock.Select();
//			//store_stock[index].count -= purshcase_count;
//			//auto result = std::ranges::find_if(cart, product_id_eq());
//			//if (cart.end() != result) {
//			//	(*result).count += product.count;
//			//}
//			//else
//			//	cart.push_back(product);
//		}
//		//else if (2 == option) {
//		//	int index = ReadInRange<int>(0, cart.size() - 1);
//		//	auto erased = *(std::next(cart.begin(), index));
//		//	cart.erase();
//		//}
//	}
//
//	//cout << "1 - buy products\n";
//	//cout << "default - cancel a purchase\n";
//	//cout << "enter option: ";
//	
//	//if (1 == option) {
//	//	for (auto const& purshcase : cart) {
//	//		Sale sale{ -1, seller.id, purshcase.product_id, purshcase.count, current_timestamp };
//	//		adapter::sales.Insert(sale);
//	//	}
//	//}
//	//while (option != 0) {
//	//	cout << "0 - exit\n";
//	//	cout << "1 - make a buy\n";
//	//	std::vector<Purshcase> cart;
//		
//		//if (1 == option) {
//			
//		//}
//	//}
//	
//	// get seller store stock
//	/*auto products = adapter::products.Select();
//	auto stocks = adapter::stock.Select();*/	
//}
//
//void StockMenu() {
//	auto stores = adapter::stores.Select(); 
//	if (stores.empty()) {
//		cout << "there is not a single store. add a store\n";
//		return;
//	}
//	IndexedPrint(stores);
//	cout << "enter store index: ";
//	int index = ReadInRange<int>(0, stores.size()-1);
//	auto store = stores[index];
//	int option{-1};	
//	while (option != 0)
//	{
//		cout << "0 - exit\n";
//		cout << "1 - insert new product to stock\n"; 
//		cout << "2 - update product in stock\n";
//		cout << "enter option: ";
//		option = Read<int>();
//		if (1 == option) {
//			auto products = adapter::products.Select(StockMapper::ProductsNotIn(store.id));
//			if (products.empty()) 
//				cout << "all products in stock\n";			
//			else {
//				IndexedPrint(products);
//				cout << "enter product number: ";
//				int option = ReadInRange<int>(0, products.size() - 1);
//				auto product = products[option];
//				cout << "enter product count: ";
//				auto count = ReadInRange<double>(0, std::numeric_limits<double>::max());
//				Stock stock{ store.id, product.id, count };
//				adapter::stock.Execute(StockMapper::Insert(stock));
//			}			
//		} else if (2 == option) {
//			auto products = adapter::products.Select();
//			auto stocks = adapter::stock.Select();			
//			for (auto i : std::views::iota(size_t{ 0 }, stocks.size())) {
//				auto find_product_by_id = [](const std::vector<Product>& products, int stock_id) -> std::optional<Product> {
//					auto it = std::find_if(products.begin(), products.end(), [&](const Product& product) {
//						return product.id == stock_id;
//						});
//
//					if (it != products.end()) {
//						return *it;
//					}
//					return std::nullopt;
//				};
//				auto stock = stocks[i];
//				cout << '#' << i << '\n';
//				auto product = find_product_by_id(products, stock.product_id);
//				if (product)
//					cout << *product << '\n';
//				cout << "count: " << stock.count << '\n';
//			}
//			cout << "enter product in stock number: ";
//			int index = ReadInRange<int>(0, stocks.size()-1);
//			auto updated = stocks[index];
//			cout << "enter new count value: ";
//			updated.count = ReadInRange<double>(0, std::numeric_limits<double>::max());
//			adapter::stock.Update(updated);
//			stocks[index] = updated;
//		}
//	}	
//}
//

//
//Seller GetSeller() {
//	int page{0};
//	const int limit{10};
//	int option{ -1 };
//	Seller seller;
//	while (option != 0) {
//		int offset = page * limit;
//		auto sellers = adapter::sellers.Select(limit, offset);
//		cout << "0 - exit\n";
//		cout << "1 - select seller\n";
//		cout << "2 - previous page\n";
//		cout << "2 - next page\n";
//		option = Read<int>();
//		if (1 == option) {
//
//		}
//	}
//	return seller;
//}
//
//void sellersMenu() {
//	auto sellers = adapter::sellers.Select();
//	auto departments = adapter::departments.Select();
//	auto stores = adapter::stores.Select();
//	auto passports = adapter::passports.Select();
//	int option{ -1 };
//	while (option != 0) {
//		auto find_by_id = [](auto& vec, int id) -> std::optional<std::decay_t<decltype(vec.front())>> {
//			auto it = std::ranges::find_if(vec, [id](auto const& item) {
//				return item.id == id;
//				});
//			if (it != vec.end()) {
//				return *it;
//			}
//			return std::nullopt;
//		};
//		for (auto i : std::views::iota(size_t{ 0 }, sellers.size())) {
//			cout << '#' << i << '\n';
//			auto seller = sellers[i];
//			auto passport = find_by_id(passports, seller.passport_id);
//			if (passport)
//				cout << "PASSPORT:\n" << *passport  << '\n';
//			auto department = find_by_id(departments, seller.department_id);
//			if (department)
//				cout << "DEPARTMENT:\n" << *department << '\n';
//			auto store = find_by_id(stores, seller.passport_id);
//			if (store)
//				cout << "STORE:\n" << *store << '\n';
//			cout << '\n';
//		}
//
//		cout << "0 - exit\n";
//		cout << "1 - insert new seller\n";
//		cout << "2 - update seller\n";
//		option = Read<int>();
//		if (1 == option) {			
//			cout << "enter passport\n";
//			auto passport = Read<Passport>();
//			adapter::passports.Insert(passport);		
//			auto passports = adapter::passports.Select(PassportMapper::ConditionByNumber(passport));
//			passport = passports[0];
//
//			
//			IndexedPrint(departments);
//			cout << "enter department index: ";
//			auto index = ReadInRange<int>(0, departments.size() - 1);
//			auto department = departments[index];
//
//			IndexedPrint(stores);
//			cout << "enter store index: ";
//			index = ReadInRange<int>(0, stores.size() - 1);
//			
//			auto store = stores[index];
//
//			Seller seller{-1, store.id, department.id, passport.id };
//			adapter::sellers.Insert(seller);
//
//			sellers = adapter::sellers.Select();
//			passports = adapter::passports.Select();
//		}
//		if (2 == option) {
//			cout << "enter seller index: ";
//			int index = ReadInRange<int>(0, sellers.size() - 1);
//			auto updated_seller = sellers[index];
//			while (option != 0) {
//				cout << "0 - exit\n";
//				cout << "1 - update passport\n";
//				cout << "2 - update department\n";
//				cout << "3 - update store\n";
//				option = Read<int>();
//				if (1 == option) {
//					cout << "enter passport\n";
//					auto updated_passport = Read<Passport>();
//					auto passports = adapter::passports.Select(PassportMapper::ConditionById(updated_seller.passport_id));
//					updated_passport.id = passports[0].id;
//					adapter::passports.Update(updated_passport);
//					//passports[index] = // TODO
//					passports = adapter::passports.Select();
//				}
//				if (2 == option) {
//					IndexedPrint(departments);
//					cout << "enter department index: ";
//					int index = ReadInRange<int>(0, departments.size() - 1);
//					updated_seller.department_id = departments[index].id;
//				}
//				if (3 == option) {
//					IndexedPrint(stores);
//					cout << "enter store index: ";
//					int index = ReadInRange<int>(0, stores.size() - 1);
//					updated_seller.store_id = stores[index].id;
//				}
//			}	
//			adapter::sellers.Update(updated_seller);
//			sellers[index] = updated_seller;
//		}
//	}
//}

void productMenu() {
	auto& adapter = adapter::products;
	cout << "select store\n";
	auto store = GetEntity(adapter::stores);
	cout << "selecd store:\n";
	cout << store << '\n';
	int option{-1};	
	while (option != 0)
	{
		cout << "0 - exit\n";
		cout << "1 - insert new product\n"; 
		cout << "2 - update products\n";
		cout << "3 - show products\n";
		cout << "enter option: ";
		option = Read<int>();
		if (1 == option) {
			cout << "enter product\n";
			auto product = Read<Product>();
			adapter.Insert(product);
		} else if (2 == option) {
			try {
				auto entitity = GetEntity(adapter);
				auto updated = Read<decltype(entitity)>();
				updated.id = entitity.id;
				updated.store_id = entitity.store_id;
				adapter.Update(updated);
			}
			catch (std::exception const& e) {
				std::cerr << e.what() << '\n';
			}
		} else if (3 == option) {
			//ShowEntities(adapter::stock);
		}
	}
}


template<class Adapter>
void TableMenu(Adapter& adapter, std::string const& table) {
	int option{-1};	
	while (option != 0) {
		cout << "0 - exit\n";
		cout << "1 - insert new " << table << "\n";
		cout << "2 - update  " << table << "\n";
		cout << "3 - show  " << table << "\n";
		cout << "enter option: ";
		option = Read<int>();
		if (1 == option) {
			cout << "enter new\n";
			auto entitity = Read<Adapter::Entity>();
			adapter.Insert(entitity);
		} else if (2 == option) {
			try {
				auto entitity = GetEntity(adapter);
				auto updated = Read<Adapter::Entity>();
				updated.id = entitity.id;
				adapter.Update(updated);
			}
			catch (std::exception const& e) {
				std::cerr << e.what() << '\n';
			}			
		} else if (3 == option) {
			ShowEntities(adapter);
		}
	}
}

void DefaultMenu() {
	int option{ -1 };
	while (option != 0) {
		cout << "0 - exit\n";
		cout << "1 - stores menu\n";
		cout << "2 - departments menu\n";
		cout << "3 - products menu\n";
		cout << "4 - sellers menu\n";
		cout << "5 - sale menu\n";
		cout << "6 - stock menu\n";
		cout << "enter option: ";
		option = Read<int>();
		switch (option)
		{
		case 1: {
			TableMenu(adapter::stores, sqltable::name::stores);
			break;
		}
		case 2: {			
			TableMenu(adapter::departments, sqltable::name::departments);
			break;
		}
		case 3: {
			productMenu();
			break;
		}
		case 4: {
			//sellersMenu();
			break;
		}			  
		case 5: {
			//SaleMenu();
			break;
		}
		case 6: {
			//StockMenu();
			break;
		}
		}
	}
	
}

// CRUD Store DELETE if not exist sellers with
// CRUD Product DELETE if not exist Stock or Sale with
// INSERT UPDATE (списать добавить) Stock
// CRUD Department DELETE if not exist sellers with Department
// Seller INSERT with (store Department Passport) UPDATE (store Department Passport) DELETE (Passport)
// Sale INSERT chose Seller show Store Stock chose count <= Stock UPDATE Stock count

int main() {
	DefaultMenu();
	// using namespace pqxx;
	// auto& table_names = sqltable::name::all;
	// Range<int> tables_range(0, table_names.size());
	// int option{ 0 };
	// while (tables_range.Contains(option)) {
	// 	PrintTables();
	// 	cout << "for exit, enter any another number\n";
	// 	cout << "enter table number: ";
	// 	option = ReadInt();
	// }

	// try {	
	//pqxx::work w(connection);

	
	//pqxx::row r = w.exec1("SELECT 1, 2, 34.5");
	//w.commit();
	//auto stock = mapper.Create<Stock>(r);

	//w.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
	//w.commit();
	// } catch (const exception &e) {
	// cerr << e.what() << endl;
	// }
}