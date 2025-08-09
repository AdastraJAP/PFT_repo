#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <ctime>


class User {
    private:
        std::string name;
        std::string passwordHash;//never store plain text passwords
        std::string email;
        std::string displayName;
        std::string currency{"TWD"};//preference
        time_t createdAt_{0};
        time_t lastLoginTime_ {0};
        
        

    public:
        User(); // Default constructor
        explicit User(const std::string& username, const std::string& email = {},
               const std::string& displayName = {}, const std::string& currency = {"TWD"},
               time_t createdAt = 0, time_t lastLoginTime = 0) noexcept;
        //Getters
        const std::string& getUserName() const noexcept{return name;};
        const std::string& getEmail() const noexcept{return email;};
        const std::string& getDisplayName() const noexcept{return displayName;};
        const std::string& getCurrency() const noexcept{return currency;};
        time_t getCreatedAt() const noexcept{return createdAt_;};
        time_t getLastLoginTime() const noexcept{return lastLoginTime_;};

        //Setters
        void setEmail(const std::string &email);
        void setDisplayName(const std::string &displayName);
        void setCurrency(const std::string &currency);
        void setLastLoginTime(time_t lastLoginTime);
        //auth
        void setPasswordHash(const std::string & plaintextPassword);//hash internally
        bool authenticate(const std::string &attempt) const;//verify hash

};
#endif
