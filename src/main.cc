#include <iostream>
#include <pqxx/pqxx>
#include <string>

using namespace std;


int main() {
	using namespace pqxx;
try {
// Connect to the database
pqxx::connection c("user=admin password=admin host=localhost port=5432 dbname=store_db");
pqxx::work w(c);
w.exec("CREATE TABLE cities (name varchar(80), location point, population int);");
w.commit();
} catch (const exception &e) {
cerr << e.what() << endl;
return 1;
}
}