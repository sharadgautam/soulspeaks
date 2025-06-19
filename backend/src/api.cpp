#include "api.hpp"
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
        response.send(Pistache::Http::Code::Bad_Request, "Invalid request");
        return;
    }
    std::string username = body["username"], password = body["password"];
    // Dummy auth: username=admin, password=admin
    if (username == "admin" && password == "admin") {
        response.send(Pistache::Http::Code::Ok, R"({"success":true})");
    } else {
        response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid credentials"})");
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
    auto body = json::parse(request.body(), nullptr, false);
    if (!body.is_object() || !body.contains("company") || !body.contains("name") || !body.contains("email") ||
        !body.contains("phone") || !body.contains("country") || !body.contains("state") ||
        !body.contains("address") || !body.contains("zip") || !body.contains("password")) {
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid request"})");
        return;
    }
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
            "INSERT INTO customers (customer_id, company, name, email, phone, country, state, address, zip, password_hash, salt, verified) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 0)"
        ));
        stmt->setInt(1, customer_id);
        stmt->setString(2, body["company"].get<std::string>());
        stmt->setString(3, body["name"].get<std::string>());
        stmt->setString(4, body["email"].get<std::string>());
        stmt->setString(5, body["phone"].get<std::string>());
        stmt->setString(6, body["country"].get<std::string>());
        stmt->setString(7, body["state"].get<std::string>());
        stmt->setString(8, body["address"].get<std::string>());
        stmt->setString(9, body["zip"].get<std::string>());
        stmt->setString(10, password_hash);
        stmt->setString(11, salt);
        stmt->execute();
        // Placeholder: send verification email
        // sendVerificationEmail(body["email"], customer_id);
        response.send(Pistache::Http::Code::Ok, R"({"success":true})");
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Signup failed"})");
    }
} 