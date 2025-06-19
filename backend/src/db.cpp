#include "db.hpp"

Database::Database(const std::string& host, const std::string& user, const std::string& password, const std::string& db)
    : host_(host), user_(user), password_(password), db_(db) {}

std::shared_ptr<sql::Connection> Database::getConnection() {
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    std::shared_ptr<sql::Connection> con(driver->connect(host_, user_, password_));
    con->setSchema(db_);
    return con;
} 