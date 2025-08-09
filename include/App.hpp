#ifndef APP_HPP
#define APP_HPP
#include "include/App.hpp"
#include "include/User.hpp"
#include "include/Transaction.hpp"
#include "include/BudgetManager.hpp"
#include "include/DBManager.hpp"
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