#pragma once
#include <string>

std::string generateSalt();
std::string hashPassword(const std::string& password, const std::string& salt); 