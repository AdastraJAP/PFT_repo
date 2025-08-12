#include "DBManager.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
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
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.getEmail().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.getPasswordHash().c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

bool DBManager::getUser(const std::string& username, User& outUser) {
    if (!isConnected()) return false;
    
    const char* query = "SELECT username, email, password_hash FROM users WHERE username = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = (const char*)sqlite3_column_text(stmt, 0);
        std::string email = (const char*)sqlite3_column_text(stmt, 1);
        std::string passwordHash = (const char*)sqlite3_column_text(stmt, 2);
        
        outUser = User(name, email);
        outUser.setPasswordHash(passwordHash);
        sqlite3_finalize(stmt);
        return true;
    }
    
    sqlite3_finalize(stmt);
    return false;
}

bool DBManager::updateUser(const User& user) {
    if (!isConnected()) return false;
    
    const char* query = "UPDATE users SET email = ?, password_hash = ? WHERE username = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, user.getEmail().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.getPasswordHash().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.getName().c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

bool DBManager::deleteUser(const std::string& username) {
    if (!isConnected()) return false;
    
    const char* query = "DELETE FROM users WHERE username = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

// Transaction
bool DBManager::addTransaction(const std::string& username, const Transaction& transaction) {
    if (!isConnected()) return false;
    
    // First get the user ID
    int userId = getUserIdFromUsername(username);
    if (userId == -1) return false;
    
    const char* query = "INSERT INTO transactions (user_id, amount, description, category, date, is_expense) VALUES (?, ?, ?, ?, ?, ?)";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_double(stmt, 2, transaction.getAmount());
    sqlite3_bind_text(stmt, 3, transaction.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, transaction.getCategory().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 5, transaction.getDate());
    sqlite3_bind_int(stmt, 6, transaction.getIsExpense() ? 1 : 0);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

bool DBManager::getTransactions(const std::string& username, std::vector<Transaction>& outTransactions) {
    if (!isConnected()) return false;
    
    int userId = getUserIdFromUsername(username);
    if (userId == -1) return false;
    
    const char* query = "SELECT id, amount, description, category, date, is_expense FROM transactions WHERE user_id = ? ORDER BY date DESC";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, userId);
    outTransactions.clear();
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction trans(
            sqlite3_column_double(stmt, 1),
            (const char*)sqlite3_column_text(stmt, 2),
            (const char*)sqlite3_column_text(stmt, 3),
            sqlite3_column_int(stmt, 5) == 1
        );
        trans.setId(sqlite3_column_int(stmt, 0));
        trans.setUserId(userId);
        trans.setDate(sqlite3_column_int64(stmt, 4));
        
        outTransactions.push_back(trans);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBManager::updateTransaction(const Transaction& transaction) {
    if (!isConnected()) return false;
    
    const char* query = "UPDATE transactions SET amount = ?, description = ?, category = ?, date = ?, is_expense = ? WHERE id = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_double(stmt, 1, transaction.getAmount());
    sqlite3_bind_text(stmt, 2, transaction.getDescription().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, transaction.getCategory().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 4, transaction.getDate());
    sqlite3_bind_int(stmt, 5, transaction.getIsExpense() ? 1 : 0);
    sqlite3_bind_int(stmt, 6, transaction.getId());
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

bool DBManager::deleteTransaction(int transactionId) {
    if (!isConnected()) return false;
    
    const char* query = "DELETE FROM transactions WHERE id = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, transactionId);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

// Budget
bool DBManager::setBudget(const std::string& username, double amount) {
    if (!isConnected()) return false;
    
    int userId = getUserIdFromUsername(username);
    if (userId == -1) return false;
    
    // Get current month/year
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m");
    std::string monthYear = oss.str();
    
    const char* query = "INSERT OR REPLACE INTO budgets (user_id, amount, month_year) VALUES (?, ?, ?)";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_text(stmt, 3, monthYear.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        lastError = sqlite3_errmsg(db);
        return false;
    }
    
    return true;
}

double DBManager::getBudget(const std::string& username) {
    if (!isConnected()) return 0.0;
    
    int userId = getUserIdFromUsername(username);
    if (userId == -1) return 0.0;
    
    const char* query = "SELECT amount FROM budgets WHERE user_id = ? ORDER BY month_year DESC LIMIT 1";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return 0.0;
    }
    
    sqlite3_bind_int(stmt, 1, userId);
    
    double amount = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        amount = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return amount;
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

// Helper method to get user ID from username
int DBManager::getUserIdFromUsername(const std::string& username) {
    if (!isConnected()) return -1;
    
    const char* query = "SELECT id FROM users WHERE username = ?";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        lastError = sqlite3_errmsg(db);
        return -1;
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    int userId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return userId;
}