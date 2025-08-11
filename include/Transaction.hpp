#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <ctime>

class Transaction {
    private:
        int id;
        int userId;
        double amount;
        std::string description;
        std::string category;
        std::time_t date;
        bool isExpense;  // for not income

    public:
        Transaction();
        Transaction(double amount, const std::string& description, const std::string& category, bool isExpense = true);
        Transaction(int userId, double amount, const std::string& description, const std::string& category, bool isExpense = true);

        int getId() const { return id; }
        int getUserId() const { return userId; }
        double getAmount() const { return amount; }
        std::string getDescription() const { return description; }
        std::string getCategory() const { return category; }
        std::time_t getDate() const { return date; }
        bool getIsExpense() const { return isExpense; }

        void setId(int newId) { id = newId; }
        void setUserId(int newUserId) { userId = newUserId; }
        void setAmount(double newAmount) { amount = newAmount; }
        void setDescription(const std::string& newDescription) { description = newDescription; }
        void setCategory(const std::string& newCategory) { category = newCategory; }
        void setDate(std::time_t newDate) { date = newDate; }
        void setIsExpense(bool newIsExpense) { isExpense = newIsExpense; }

        std::string getFormattedDate() const;
        std::string getFormattedAmount() const;
        bool isValid() const;
};
#endif