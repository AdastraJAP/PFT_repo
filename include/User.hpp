#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {

    private:
    std::string name;
    std::string email;
    std::string passwordHash;

public:
    User();
    User(const std::string& username, const std::string& email);

    // Setters
    void setEmail(const std::string& e);
    void setPasswordHash(const std::string& plaintextPassword);

    // Getters
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }

    // Authentication
    bool authenticate(const std::string& attempt) const;


};
#endif // USER_HPP
