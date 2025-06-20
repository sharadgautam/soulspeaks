#ifndef DB_HPP
#define DB_HPP

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>

class Database {
public:
    Database(const std::string& host, const std::string& user, const std::string& password, const std::string& db);
    std::shared_ptr<sql::Connection> getConnection();
private:
    std::string host_, user_, password_, db_;
};

#endif