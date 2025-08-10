#ifndef APP_HPP
#define APP_HPP
#include "User.hpp"
#include "Transaction.hpp"
#include "BudgetManager.hpp"
#include "DBManager.hpp"
#include <iostream>
#include <vector>
using namespace std;
class App {
    private:
        //vector<User> users;
        //User* currentUser = nullptr;
    public:
        void run();
        void MainMenu();
        void AddTransaction();
        void ViewTransaction();
        void SetBudget();
        void Login();
        void Register();
        void Logout();
};
#endif