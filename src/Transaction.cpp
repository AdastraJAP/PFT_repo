#include "Transaction.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

Transaction::Transaction() : id(0), userId(0), amount(0.0), description(""), category(""), date(std::time(nullptr)), isExpense(true) {}

Transaction::Transaction(double amount, const std::string& description, const std::string& category, bool isExpense) 
    : id(0), userId(0), amount(amount), description(description), category(category), date(std::time(nullptr)), isExpense(isExpense) {}

Transaction::Transaction(int userId, double amount, const std::string& description, const std::string& category, bool isExpense) 
    : id(0), userId(userId), amount(amount), description(description), category(category), date(std::time(nullptr)), isExpense(isExpense) {}


std::string Transaction::getFormattedDate() const {
    std::tm* timeinfo = std::localtime(&date);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d %H:%M");
    return oss.str();
}

std::string Transaction::getFormattedAmount() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    if (isExpense) {
        oss << "-$" << amount;
    } else {
        oss << "+$" << amount;
    }
    return oss.str();
}

bool Transaction::isValid() const {
    return amount > 0.0 && 
           !description.empty() && 
           !category.empty() && 
           userId > 0;
}