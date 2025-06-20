#pragma once
#include <pistache/http.h>
#include <pistache/endpoint.h>
#include "db.hpp"

class ApiHandler : public Pistache::Http::Handler {
public:
    HTTP_PROTOTYPE(ApiHandler)
    explicit ApiHandler(Database& db);
    void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) override;
private:
    Database& db_;
    void handleLogin(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void handleDidSearch(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void handleDidBuy(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void handleSignup(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void handleListUsers(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
    void handleMe(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response);
}; 