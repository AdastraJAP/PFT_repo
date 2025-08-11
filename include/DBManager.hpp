#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP
#include "User.hpp"
#include "Transaction.hpp"
#include "BudgetManager.hpp"
#include <iostream>
#include <string>
#include <sqlite3.h>

class Database {
    public:
        Database(const string db_path);
        ~Database();

        bool initSchema();
        bool addUser(const User& user);
        bool getUser(const string& username, User& outUser);
        bool addTransaction(const string& username/*, const Transaction& transact*/);
        bool getTransaction(const string& username/*, vector<Transaction>& outTransactions*/);
    private:
        sqlite3* db;
};
#endif