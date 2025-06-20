#include "api.hpp"
#include <iostream>
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include "hash.hpp"
#include <cppconn/prepared_statement.h>

using json = nlohmann::json;

ApiHandler::ApiHandler(Database& db) : db_(db) {}

void ApiHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    if (request.resource() == "/api/login" && request.method() == Pistache::Http::Method::Post) {
        handleLogin(request, std::move(response));
    } else if (request.resource() == "/api/did/search" && request.method() == Pistache::Http::Method::Post) {
        handleDidSearch(request, std::move(response));
    } else if (request.resource() == "/api/did/buy" && request.method() == Pistache::Http::Method::Post) {
        handleDidBuy(request, std::move(response));
    } else if (request.resource() == "/api/signup" && request.method() == Pistache::Http::Method::Post) {
        handleSignup(request, std::move(response));
    } else {
        response.send(Pistache::Http::Code::Not_Found, "Endpoint not found");
    }
}

void ApiHandler::handleLogin(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    auto body = json::parse(request.body(), nullptr, false);
    if (!body.is_object() || !body.contains("username") || !body.contains("password")) {
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid request format"})");
        return;
    }
    
    std::string username = body["username"].get<std::string>();
    std::string password = body["password"].get<std::string>();

    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
            "SELECT password_hash, salt FROM customers WHERE name = ? AND verified = 1"
        ));
        stmt->setString(1, username);
        
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (res->next()) {
            std::string db_password_hash = res->getString("password_hash");
            std::string db_salt = res->getString("salt");
            
            std::string provided_password_hash = hashPassword(password, db_salt);
            
            if (provided_password_hash == db_password_hash) {
                // Passwords match
                response.send(Pistache::Http::Code::Ok, R"({"success":true})");
            } else {
                // Passwords do not match
                response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid credentials"})");
            }
        } else {
            // User not found or not verified
            response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid credentials"})");
        }
    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Login failed"})");
    }
}

void ApiHandler::handleDidSearch(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    // Dummy: return a list of available DIDs
    response.send(Pistache::Http::Code::Ok, R"({"dids":[{"number":"+1234567890","country":"US"},{"number":"+1987654321","country":"US"}]})");
}

void ApiHandler::handleDidBuy(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    // Dummy: always succeed
    response.send(Pistache::Http::Code::Ok, R"({"success":true,"number":"+1234567890"})");
}

void ApiHandler::handleSignup(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    std::cout << "--- SIGNUP REQUEST RECEIVED ---" << std::endl;
    std::cout << "Request Body: " << request.body() << std::endl;

    auto body = json::parse(request.body(), nullptr, false);
    if (body.is_discarded()) {
        std::cout << "Error: JSON parsing failed." << std::endl;
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid JSON format"})");
        return;
    }

    std::cout << "Parsed JSON: " << body.dump(4) << std::endl;

    if (!body.is_object() || !body.contains("username") || !body.contains("email") || !body.contains("password")) {
        std::cout << "Error: Validation failed. Missing required fields." << std::endl;
        std::cout << "Has username: " << std::boolalpha << body.contains("username") << std::endl;
        std::cout << "Has email: " << std::boolalpha << body.contains("email") << std::endl;
        std::cout << "Has password: " << std::boolalpha << body.contains("password") << std::endl;
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid request"})");
        return;
    }
    
    std::cout << "Validation successful. Proceeding with signup..." << std::endl;

    // Generate random 8-digit customer ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000000, 99999999);
    int customer_id = dis(gen);

    // Generate salt and hash password
    std::string salt = generateSalt();
    std::string password_hash = hashPassword(body["password"].get<std::string>(), salt);

    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
            "INSERT INTO customers (customer_id, name, email, password_hash, salt, verified) VALUES (?, ?, ?, ?, ?, 1)"
        ));
        stmt->setInt(1, customer_id);
        stmt->setString(2, body["username"].get<std::string>());
        stmt->setString(3, body["email"].get<std::string>());
        stmt->setString(4, password_hash);
        stmt->setString(5, salt);
        stmt->execute();
        
        response.send(Pistache::Http::Code::Ok, R"({"success":true})");
    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Signup failed"})");
    }
} 