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
    } else if (request.resource() == "/api/verify" && request.method() == Pistache::Http::Method::Post) {
        handleVerify(request, std::move(response));
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

    // Generate verification code (6-digit)
    std::uniform_int_distribution<> code_dis(100000, 999999);
    int verification_code = code_dis(gen);
    std::string verification_code_str = std::to_string(verification_code);

    // Generate salt and hash password
    std::string salt = generateSalt();
    std::string password_hash = hashPassword(body["password"].get<std::string>(), salt);

    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
            "INSERT INTO customers (customer_id, name, email, password_hash, salt, verified, verification_code) VALUES (?, ?, ?, ?, ?, 0, ?)"
        ));
        stmt->setInt(1, customer_id);
        stmt->setString(2, body["username"].get<std::string>());
        stmt->setString(3, body["email"].get<std::string>());
        stmt->setString(4, password_hash);
        stmt->setString(5, salt);
        stmt->setString(6, verification_code_str);
        stmt->execute();
        
        // TODO: Send verification email using SMTP
        // For now, just log the verification code
        std::cout << "=== VERIFICATION EMAIL INFO ===" << std::endl;
        std::cout << "To: " << body["email"].get<std::string>() << std::endl;
        std::cout << "Subject: Welcome to SoulSpeaks - Verify Your Account" << std::endl;
        std::cout << "Verification Code: " << verification_code_str << std::endl;
        std::cout << "Verification Link: http://localhost:8081/verify?code=" << verification_code_str << std::endl;
        std::cout << "===============================" << std::endl;
        
        response.send(Pistache::Http::Code::Ok, R"({"success":true})");
    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Signup failed"})");
    }
}

void ApiHandler::handleVerify(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    auto body = json::parse(request.body(), nullptr, false);
    if (body.is_discarded() || !body.is_object() || !body.contains("code")) {
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid request"})");
        return;
    }

    std::string verification_code = body["code"].get<std::string>();

    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
            "UPDATE customers SET verified = 1 WHERE verification_code = ? AND verified = 0"
        ));
        stmt->setString(1, verification_code);
        int rows_affected = stmt->executeUpdate();
        
        if (rows_affected > 0) {
            response.send(Pistache::Http::Code::Ok, R"({"success":true,"message":"Account verified successfully"})");
        } else {
            response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid or expired verification code"})");
        }
    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Verification failed"})");
    }
} 