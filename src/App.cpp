#include "App.hpp"
#include "User.hpp"
#include "Transaction.hpp"
#include "BudgetManager.hpp"
#include "DBManager.hpp"
#include <iostream>
using namespace std;

void App::run(){
    DBManager database;
    database.initSchema();
}
/*void MainMenu();
void AddTransaction();
void ViewTransaction();
void SetBudget();
void Login();
void Register();
void Logout();*/