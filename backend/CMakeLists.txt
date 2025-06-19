cmake_minimum_required(VERSION 3.10)
project(soulspeaks_backend)

set(CMAKE_CXX_STANDARD 17)

# Remove find_package commands since we'll link manually
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(
    /usr/local/include
    /usr/include
)
link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

add_executable(backend
    src/main.cpp
    src/db.cpp
    src/api.cpp
    src/hash.cpp
)

target_link_libraries(backend
    pistache
    mysqlcppconn
    nlohmann_json::nlohmann_json
    OpenSSL::SSL 
    OpenSSL::Crypto
) 