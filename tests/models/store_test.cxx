
#include <iostream>
#include <string>
#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <pqxx/pqxx>

// import databasefixture;

namespace {

class DatabaseFixture : public ::testing::Test {
protected:
    static pqxx::connection connection_;
    // Performs shared setup for all tests in the test suite. GoogleTest calls SetUpTestSuite() before running the first test in the test suite.
    static void SetUpTestSuite() {
        connection_ = std::move(pqxx::connection("user=admin password=admin host=localhost port=5432 dbname=store_db"));
    }
};
pqxx::connection DatabaseFixture::connection_;

TEST_F(DatabaseFixture, CheckProductsTableExists) {
    // Arrange
    const std::string table_name = "cities";
    const std::string query = "SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = '" + table_name + "');";
    pqxx::work w(DatabaseFixture::connection_);
    // Act
    try {
        auto row = w.exec(query);
        w.commit();
        auto [is_tabale_exist] = row[0].as<bool>();
        // Assert
        EXPECT_TRUE(is_tabale_exist);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

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