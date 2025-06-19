#include "hash.hpp"
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>

std::string generateSalt() {
    unsigned char salt[32];
    RAND_bytes(salt, sizeof(salt));
    std::ostringstream oss;
    for (int i = 0; i < 32; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)salt[i];
    return oss.str();
}

std::string hashPassword(const std::string& password, const std::string& salt) {
    std::string salted = salt + password;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)salted.c_str(), salted.size(), hash);
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
} 