#include "User.hpp"
#include <functional>
#include <string>

// --- Local helper (toy hash; NOT secure for production)
namespace {
    std::string simpleHash(const std::string& s) {
        auto hashValue = std::hash<std::string>{}(s);
        return std::to_string(hashValue); // numeric string
    }
}

User::User() = default;

User::User(const std::string& username, const std::string& email)
    : name(username), email(email) {}

// Set email
void User::setEmail(const std::string& e) {
    email = e;
}

// Store hash of the password
void User::setPasswordHash(const std::string& plaintextPassword) {
    passwordHash = simpleHash(plaintextPassword);
}

// Verify by hashing the attempt and comparing
bool User::authenticate(const std::string& attempt) const {
    return passwordHash == simpleHash(attempt);
}
