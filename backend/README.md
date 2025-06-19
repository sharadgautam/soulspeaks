# Backend (C++ Pistache API)

This backend uses [Pistache](http://pistache.io/) for the REST API and MySQL for the database.

## Structure
- `src/` - Source code (main.cpp, db.cpp/hpp, api.cpp/hpp)
- `CMakeLists.txt` - Build configuration

## Setup
1. Install dependencies: Pistache, MySQL Connector/C++
2. Build with CMake
3. Configure your MySQL connection in the code
4. Run the server 