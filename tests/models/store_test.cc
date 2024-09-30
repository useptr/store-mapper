
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

//#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pqxx/pqxx>

// import databasefixture;

namespace {

//class DatabaseFixture : public ::testing::Test {
//protected:
//    static pqxx::connection connection_;
//};
//pqxx::connection DatabaseFixture::connection_ = std::move(pqxx::connection("user=admin password=admin host=localhost port=5432 dbname=store_db"));

pqxx::connection connection = std::move(pqxx::connection("user=admin password=admin host=localhost port=5432 dbname=store_db"));
TEST(Database, CheckProductsTableExists) {
    // Arrange
    pqxx::work work(connection);
    const std::string table_name = "cities";
    const std::string query = "SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = " + work.quote(table_name) + ");";
    // Act
    try {
        auto result = work.exec(query);
        work.commit();
        auto [is_table_exist] = result[0].as<bool>();
        // Assert
        EXPECT_TRUE(is_table_exist);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
// TEST(Database, SelectTest) {
//     // Arrange
//     pqxx::work work(connection);
//     const std::string table_name = "test";
//     const std::string query = "SELECT * FROM " + work.quote(table_name) + ";";
//     // Act
//     try {
//         auto result = work.exec(query);
//         work.commit();
//         // auto [id, number, birth_date, first_name, middle_name, last_name, address, issue_date, issued_by] = result[0].as<bool>();
//         // Assert
//         // EXPECT_TRUE(is_table_exist);
//     } catch (const std::exception &e) {
//         std::cerr << e.what() << std::endl;
//     }
// }

//TEST_F(DatabaseFixture, CheckStoreTableExists) {
//    // Arrange
//    const std::string table_name = "store";
//    const std::string query = "SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + table_name + "');";
//    pqxx::work w(DatabaseFixture::connection_);
//    // Act
//    try {
//        auto row = w.exec(query);
//        w.commit();
//        auto [is_tabale_exist] = row[0].as<bool>();
//        std::cout << "ABOBA\n";
//        // Assert
//        EXPECT_TRUE(is_tabale_exist);
//        EXPECT_FALSE(is_tabale_exist);
//    }
//    catch (const std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
//}

// TEST_F(DatabaseFixture, CheckProductsTableExists) {
//     // Arrange
//     const std::string table_name = "";
//     auto expected_sql = "";
//     // Act
//     auto sql = .CreateTable();
//     // Assert
//     EXPECT_EQ(expected_sql, sql);
// }

}  // namespace