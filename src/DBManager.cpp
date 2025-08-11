#include "DBManager.hpp"
#include <iostream>
#include <sqlite3.h>

// Static member initialization
const std::string DBManager::DB_PATH = "data/PFT.db";

// Constructor
DBManager::DBManager() : db(nullptr) {
    openDatabase();
}

// Destructor
DBManager::~DBManager() {
    closeDatabase();
}

// Database connection methods
bool DBManager::openDatabase() {
    int rc = sqlite3_open(DB_PATH.c_str(), &db);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    return true;
}

void DBManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

// Database initialization
bool DBManager::initSchema() {
    if (!isConnected()) return false;
    
    const char* schema = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            email TEXT NOT NULL,
            password_hash TEXT NOT NULL
        );
        
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            amount REAL NOT NULL,
            description TEXT NOT NULL,
            category TEXT NOT NULL,
            date INTEGER NOT NULL,
            is_expense INTEGER NOT NULL DEFAULT 1,
            FOREIGN KEY (user_id) REFERENCES users (id)
        );
        
        CREATE TABLE IF NOT EXISTS budgets (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            amount REAL NOT NULL,
            month_year TEXT NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users (id)
        );
    )";
    
    return executeQuery(schema);
}

// User
bool DBManager::addUser(const User& user) {
    if (!isConnected()) return false;
    
    const char* query = "INSERT INTO users (username, email, password_hash) VALUES (?, ?, ?)";
    // To be implemented...
    return true;
}

bool DBManager::getUser(const std::string& username, User& outUser) {
    if (!isConnected()) return false;
    
    const char* query = "SELECT username, email, password_hash FROM users WHERE username = ?";
    //  To be implemented...
    return false;
}

bool DBManager::updateUser(const User& user) {
    if (!isConnected()) return false;
    
    const char* query = "UPDATE users SET email = ?, password_hash = ? WHERE username = ?";
    //  To be implemented...
    return true;
}

bool DBManager::deleteUser(const std::string& username) {
    if (!isConnected()) return false;
    
    const char* query = "DELETE FROM users WHERE username = ?";
    //  To be implemented...
    return true;
}

// Transaction
bool DBManager::addTransaction(const std::string& username, const Transaction& transaction) {
    if (!isConnected()) return false;
    
    const char* query = "INSERT INTO transactions (user_id, amount, description, category, date) VALUES (?, ?, ?, ?, ?)";
    // To be implemented...
    return true;
}

bool DBManager::getTransactions(const std::string& username, std::vector<Transaction>& outTransactions) {
    if (!isConnected()) return false;
    
    const char* query = "SELECT * FROM transactions WHERE user_id = (SELECT id FROM users WHERE username = ?)";
    //  To be implemented...
    return true;
}

bool DBManager::updateTransaction(const Transaction& transaction) {
    if (!isConnected()) return false;
    
    const char* query = "UPDATE transactions SET amount = ?, description = ?, category = ?, date = ? WHERE id = ?";
    //  To be implemented...
    return true;
}

bool DBManager::deleteTransaction(int transactionId) {
    if (!isConnected()) return false;
    
    const char* query = "DELETE FROM transactions WHERE id = ?";
    // To be implemented...
    return true;
}

// Budget
bool DBManager::setBudget(const std::string& username, double amount) {
    if (!isConnected()) return false;
    
    const char* query = "INSERT OR REPLACE INTO budgets (user_id, amount, month_year) VALUES (?, ?, ?)";
    // To be implemented...
    return true;
}

double DBManager::getBudget(const std::string& username) {
    if (!isConnected()) return false;
    
    const char* query = "SELECT amount FROM budgets WHERE user_id = (SELECT id FROM users WHERE username = ?) ORDER BY month_year DESC LIMIT 1";
    //  To be implemented...
    return 0.0;
}

bool DBManager::isConnected() const {
    return db != nullptr;
}

std::string DBManager::getLastError() const {
    return lastError;
}

bool DBManager::executeQuery(const std::string& query) {
    if (!isConnected()) return false;
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        lastError = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool DBManager::beginTransaction() {
    return executeQuery("BEGIN TRANSACTION");
}

bool DBManager::commitTransaction() {
    return executeQuery("COMMIT");
}

bool DBManager::rollbackTransaction() {
    return executeQuery("ROLLBACK");
}
