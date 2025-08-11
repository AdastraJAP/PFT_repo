#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP
#include "User.hpp"
#include "Transaction.hpp"
#include "BudgetManager.hpp"
#include <string>
#include <vector>
#include <sqlite3.h>

class DBManager {
    private:
        sqlite3* db;
        static const std::string DB_PATH;

    public:
        // Constructor and destructor
        DBManager();
        ~DBManager();

        // Database initialization
        bool initSchema();
        bool openDatabase();
        void closeDatabase();

        // User
        bool addUser(const User& user);
        bool getUser(const std::string& username, User& outUser);
        bool updateUser(const User& user);
        bool deleteUser(const std::string& username);

        // Transaction
        bool addTransaction(const std::string& username, const Transaction& transaction);
        bool getTransactions(const std::string& username, std::vector<Transaction>& outTransactions);
        bool updateTransaction(const Transaction& transaction);
        bool deleteTransaction(int transactionId);

        // Budget
        bool setBudget(const std::string& username, double amount);
        double getBudget(const std::string& username);

        bool isConnected() const;
        std::string getLastError() const;

    private:
        std::string lastError;
        bool executeQuery(const std::string& query);
        bool beginTransaction();
        bool commitTransaction();
        bool rollbackTransaction();
};
#endif