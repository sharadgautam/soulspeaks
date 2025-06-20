#include <pistache/endpoint.h>
#include <iostream>
#include "db.hpp"
#include "api.hpp"

using namespace Pistache;

int main() {
    try {
        Database db("tcp://127.0.0.1:3306", "soulspeaksdb", "94019401", "soulspeaks");
        // Test DB connection
        auto con = db.getConnection();
        if (con && con->isValid()) {
            std::cout << "Database connection successful!" << std::endl;
        } else {
            std::cerr << "Database connection failed!" << std::endl;
            return 1;
        }
        Http::Endpoint server(Address("0.0.0.0", Port(9080)));
        auto opts = Http::Endpoint::options().threads(1);
        server.init(opts);
        server.setHandler(Http::make_handler<ApiHandler>(db));
        std::cout << "Server running on port 9080..." << std::endl;
        server.serve();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 