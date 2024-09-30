#ifndef DBCONFIG_H
#define DBCONFIG_H

#include "sqlstatementbuilder.h"
#include "types.h"

namespace project {
    namespace sqltable {
		namespace name {
			const String stores{ "stores" };
			const String products{ "products" };
			const String stock{ "stock" };
			const String sales{ "sales" };
			const String sellers{ "sellers" };
			const String departments{ "departments" };
			const String passports{ "passports" };
		
			//constexpr std::vector<String> all{ stores, products, stock, sales , sellers , departments , passports };
		}
		namespace primarykey {
			const PrimaryKeySet default_key{ "id" };
			const PrimaryKeySet stock{ "store_id", "product_id" };
		}
		namespace builder {
			SqlStatementBuilder stores{ name::stores, primarykey::default_key };
			SqlStatementBuilder products{ name::products, primarykey::default_key };
			SqlStatementBuilder stock{ name::stock, primarykey::stock };
			SqlStatementBuilder sales{ name::sales, primarykey::default_key };
			SqlStatementBuilder sellers{ name::sellers, primarykey::default_key };
			SqlStatementBuilder departments{ name::departments, primarykey::default_key };
			SqlStatementBuilder passports{ name::passports, primarykey::default_key };
		}
	}
}

#endif // DBCONFIG_H