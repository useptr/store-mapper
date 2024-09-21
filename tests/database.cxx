module;

#include <gtest/gtest.h>
#include <pqxx/pqxx>

export module databasefixture;
// Fixture class for database tests
export class DatabaseFixture : public ::testing::Test {
protected:
    static pqxx::connection connection_;
    // Performs shared setup for all tests in the test suite. GoogleTest calls SetUpTestSuite() before running the first test in the test suite.
    static void SetUpTestSuite() {
        connection_ = std::move(pqxx::connection("user=admin password=admin host=localhost port=5432 dbname=store_db"));
    }
};