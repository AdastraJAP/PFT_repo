#include "include/App.hpp"
#include "include/User.hpp"
#include "include/Transaction.hpp"
#include "include/BudgetManager.hpp"
#include "include/DBManager.hpp"
#include <iostream>
using namespace std;

void App::run(){
    //DBManager database("data/PFT.db");
    //database.initSchema();
}
void MainMenu();
void AddTransaction();
void ViewTransaction();
void SetBudget();
void Login();
void Register();
void Logout();