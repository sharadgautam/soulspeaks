#include "api.hpp"
#include <iostream>
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include "hash.hpp"
#include <cppconn/prepared_statement.h>
#include "jwt.h"
#include <unordered_map>
#include <mutex>
#include <chrono>

using json = nlohmann::json;

// Use the same strong secret for both signing and verifying JWTs
#define SOULSPEAKS_JWT_SECRET "a8f3b2c1d4e5f6g7h8i9j0k1l2m3n4o5"

// In-memory QR token store (thread-safe)
struct QrTokenInfo {
    std::string username;
    std::chrono::steady_clock::time_point expiresAt;
};
std::unordered_map<std::string, QrTokenInfo> qrTokenStore;
std::mutex qrTokenStoreMutex;

std::string generateRandomToken(size_t length = 32) {
    static const char chars[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    thread_local std::mt19937 rg{std::random_device{}()};
    thread_local std::uniform_int_distribution<> pick(0, sizeof(chars) - 2);
    std::string s;
    s.reserve(length);
    while(length--)
        s += chars[pick(rg)];
    return s;
}

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
    } else if (request.resource() == "/api/users" && request.method() == Pistache::Http::Method::Get) {
        handleListUsers(request, std::move(response));
    } else if (request.resource() == "/api/me" && request.method() == Pistache::Http::Method::Get) {
        handleMe(request, std::move(response));
    } else if (request.resource() == "/api/auth/qr-generate" && request.method() == Pistache::Http::Method::Post) {
        handleQrGenerate(request, std::move(response));
    } else if (request.resource() == "/api/auth/app-login" && request.method() == Pistache::Http::Method::Post) {
        handleAppLogin(request, std::move(response));
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
                // Passwords match - generate JWT
                auto token = jwt::create()
                    .set_issuer("soulspeaks")
                    .set_type("JWS")
                    .set_payload_claim("username", jwt::claim(username))
                    .set_issued_at(std::chrono::system_clock::now())
                    .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
                    .sign(jwt::algorithm::hs256{SOULSPEAKS_JWT_SECRET});
                json resp = {{"success", true}, {"token", token}};
                response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
                response.send(Pistache::Http::Code::Ok, resp.dump());
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

void ApiHandler::handleListUsers(const Pistache::Http::Request& /*request*/, Pistache::Http::ResponseWriter response) {
    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT customer_id, name, email, verified FROM customers"));

        json users = json::array();
        while (res->next()) {
            json user;
            user["customer_id"] = res->getInt("customer_id");
            user["name"] = res->getString("name");
            user["email"] = res->getString("email");
            user["verified"] = res->getBoolean("verified");
            users.push_back(user);
        }

        response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
        response.send(Pistache::Http::Code::Ok, users.dump());

    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Failed to retrieve users"})");
    }
}

void ApiHandler::handleMe(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    // Require JWT in Authorization header
    auto authHeader = request.headers().tryGetRaw("Authorization");
    if (!authHeader || !authHeader.get().starts_with("Bearer ")) {
        response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Missing or invalid Authorization header"})");
        return;
    }
    std::string token = authHeader.get().substr(7);
    std::string username;
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SOULSPEAKS_JWT_SECRET})
            .with_issuer("soulspeaks");
        verifier.verify(decoded);
        username = decoded.get_payload_claim("username").as_string();
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid token"})");
        return;
    }
    try {
        auto con = db_.getConnection();
        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
            "SELECT name, email FROM customers WHERE name = ? AND verified = 1"
        ));
        stmt->setString(1, username);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        if (res->next()) {
            json user;
            user["username"] = res->getString("name");
            user["email"] = res->getString("email");
            response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
            response.send(Pistache::Http::Code::Ok, user.dump());
        } else {
            response.send(Pistache::Http::Code::Not_Found, R"({"success":false,"error":"User not found"})");
        }
    } catch (const std::exception& e) {
        std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
        response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Failed to fetch user info"})");
    }
}

void ApiHandler::handleQrGenerate(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    // Extract JWT from Authorization header
    auto authHeader = request.headers().tryGetRaw("Authorization");
    if (!authHeader || !authHeader.get().starts_with("Bearer ")) {
        response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Missing or invalid Authorization header"})");
        return;
    }
    std::string token = authHeader.get().substr(7);
    std::string username;
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SOULSPEAKS_JWT_SECRET})
            .with_issuer("soulspeaks");
        verifier.verify(decoded);
        username = decoded.get_payload_claim("username").as_string();
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid token"})");
        return;
    }
    std::string qrToken = generateRandomToken();
    auto expiresAt = std::chrono::steady_clock::now() + std::chrono::minutes(2);
    {
        std::lock_guard<std::mutex> lock(qrTokenStoreMutex);
        qrTokenStore[qrToken] = {username, expiresAt};
    }
    nlohmann::json res;
    res["qrToken"] = qrToken;
    res["success"] = true;
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, res.dump());
}

void ApiHandler::handleAppLogin(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    using json = nlohmann::json;
    auto body = json::parse(request.body(), nullptr, false);
    if (body.is_discarded()) {
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Invalid JSON"})");
        return;
    }
    std::string username;
    if (body.contains("qrToken")) {
        // QR login
        std::string qrToken = body["qrToken"];
        std::lock_guard<std::mutex> lock(qrTokenStoreMutex);
        auto it = qrTokenStore.find(qrToken);
        if (it == qrTokenStore.end() || std::chrono::steady_clock::now() > it->second.expiresAt) {
            response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid or expired QR token"})");
            return;
        }
        username = it->second.username;
        qrTokenStore.erase(it); // Invalidate token after use
    } else if (body.contains("username") && body.contains("password")) {
        // Username/password login (reuse handleLogin logic)
        std::string uname = body["username"];
        std::string password = body["password"];
        try {
            auto con = db_.getConnection();
            std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
                "SELECT password_hash, salt FROM customers WHERE name = ? AND verified = 1"
            ));
            stmt->setString(1, uname);
            std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
            if (res->next()) {
                std::string db_password_hash = res->getString("password_hash");
                std::string db_salt = res->getString("salt");
                std::string provided_password_hash = hashPassword(password, db_salt);
                if (provided_password_hash == db_password_hash) {
                    username = uname;
                } else {
                    response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid credentials"})");
                    return;
                }
            } else {
                response.send(Pistache::Http::Code::Unauthorized, R"({"success":false,"error":"Invalid credentials"})");
                return;
            }
        } catch (const std::exception& e) {
            std::cerr << "!!! DATABASE ERROR: " << e.what() << std::endl;
            response.send(Pistache::Http::Code::Internal_Server_Error, R"({"success":false,"error":"Login failed"})");
            return;
        }
    } else {
        response.send(Pistache::Http::Code::Bad_Request, R"({"success":false,"error":"Missing credentials"})");
        return;
    }
    // Issue JWT
    auto token = jwt::create()
        .set_issuer("soulspeaks")
        .set_type("JWS")
        .set_payload_claim("username", jwt::claim(username))
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
        .sign(jwt::algorithm::hs256{SOULSPEAKS_JWT_SECRET});
    json resp = {{"success", true}, {"token", token}};
    response.headers().add<Pistache::Http::Header::ContentType>(MIME(Application, Json));
    response.send(Pistache::Http::Code::Ok, resp.dump());
} 