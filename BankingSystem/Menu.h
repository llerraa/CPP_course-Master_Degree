#pragma once
#include "Bank.h"

class Menu {
private:
    Banking::Bank bank;

    // вспомогательные функции
    void clearInput();
    int getNumber(const std::string& message);
    double getDouble(const std::string& message);
    std::string getString(const std::string& message);

    // секция меню
    void clientMenu();
    void accountMenu();
    void transactionMenu();
    void reportMenu();

    // операции с клиентами
    void createClient();
    void createPremiumClient();
    void deleteClient();
    void showAllClients();

    // операции с аккаунтами
    void createCheckingAccount();
    void createSavingsAccount();
    void deleteAccount();
    void showAllAccounts();

    // операции с транзакциями
    void makeDeposit();
    void makeWithdrawal();
    void makeTransfer();

    // отчет
    void showAllTransactions();

public:
    void showMainMenu();
};
