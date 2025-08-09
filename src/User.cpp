#include "include/User.hpp"
#include<ctime>
#include<functional>
#include<sstream>
// ---local helper (toy hash; replace with a real KDF for production);

namespace{
    std::string simpleHash(const std::string& s){
        static const std::string pepper ="pftracker_v1";
        std::hash<std::string> h;
        std::size_t hashValue = h(s + pepper);
        std::ostringstream oss;
        oss << std::hex << hashValue;
        return oss.str();
    }
}// namespace

User::User()=default;
User::User(const std::string& username, const std::string& email,
           const std::string& displayName, const std::string& currency,
           std::time_t createdAt, std::time_t lastLoginTime) noexcept
    : name(username), email(email), displayName(displayName),
      currency(currency), createdAt_(createdAt ? createdAt : std::time(nullptr)),
      lastLoginTime_(lastLoginTime ) {}
      
      //---------------Setters
      void User::setEmail(const std::string& e){
          email = e;
      }
      void User::setDisplayName(const std::string& d){
          displayName = d;
      }

      void User::setCurrency(const std::string& c){
          currency = c;
      }
      
      void User::setLastLoginTime(std::time_t lastLoginTime) {
          lastLoginTime_ = lastLoginTime;
      }

      //store a hash  of the password
      void User::setPasswordHash(const std::string& plaintextPassword) {
          passwordHash = simpleHash(plaintextPassword);
      }

      //Verify by hashing the attempt and comparing
      bool User::authenticate(const std::string& attempt) const {
          return passwordHash == simpleHash(attempt);
      }