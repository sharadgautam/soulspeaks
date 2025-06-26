cmake_minimum_required(VERSION 3.10)
project(soulspeaks_backend)

set(CMAKE_CXX_STANDARD 17)

# Find required libraries
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Threads REQUIRED)

# Manually specify include directories and library paths
include_directories(/usr/include/pistache)
include_directories(/usr/include/mysql-cppconn)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    /usr/local/include
    /usr/include
    /usr/include/pistache
    /usr/include/mysql-cppconn
)
link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

add_executable(backend
    src/main.cpp
    src/db.cpp
    src/api.cpp
    src/hash.cpp
)

target_link_libraries(backend PRIVATE
    pistache
    nlohmann_json::nlohmann_json
    ${CMAKE_THREAD_LIBS_INIT}
    mysqlcppconn
    -lssl
    -lcrypto
)

install(TARGETS backend DESTINATION bin) 