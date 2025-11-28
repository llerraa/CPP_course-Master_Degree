#include "Account.h"
#include "Client.h"  // Теперь включаем здесь
#include "Transaction.h"  // Теперь включаем здесь
#include <stdexcept>
#include <iostream>

namespace Banking {


    Account::Account(const std::string& accNumber, const int& client_id, const std::string& type, double initialBalance)
        : accountNumber(accNumber), client_id(client_id), type(type), balance(initialBalance) {
        std::cout << "\n-----Account constructor called. " << std::endl;
        if (initialBalance < 0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
        std::cout << "You created a new account: " << accountNumber << " for client: " << client_id << std::endl;
    }

    Account::~Account() {
        std::cout << "\n-----Account destructor called. "  << std::endl;
    }

    //увеличивает баланс счета на указанную сумму
    void Account::deposit(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Deposit amount must be positive");
        }
        balance += amount; 
        std::cout << "\nBalance for account " << accountNumber << " increased by " << amount << std::endl;
        std::cout << "Current balance = " << balance << std::endl;
    }

    // уменьшает баланс счета на указанную сумму
    bool Account::withdraw(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        if (amount > balance) {
            std::cout << "\nYou are trying to withdraw more than available. " << std::endl;
            return false;
        }
        balance -= amount;
        std::cout << "\nBalance for account " << accountNumber << " decreased by " << amount << std::endl;
        std::cout << "Current balance = " << balance << std::endl;
        return true;
    }


    double Account::getBalance() const {
        return balance;
    }

    std::string Account::getAccountNumber() const {
        return accountNumber;
    }

    void Account::displayinfo() const {
        std::cout << "\nInformation about an account: " << std::endl; 
        std::cout << "number: " << accountNumber << ", \nclient_id: " << client_id << ", \nbalance: " << balance << ", \ntype: " << type << std::endl;

    }
 
    void Account::addTransaction_in_account(std::shared_ptr<Transaction> transaction) {
        all_account_transactions.push_back(transaction);
        std::cout << "Transaction " << transaction->getType() << ", summa: " << transaction->getSumma() << " added to account. Total transactions in account: " << all_account_transactions.size() << std::endl;
    }

    void Account::displayinfo_about_transactions_in_account() {
        std::cout << "\nInformation about transactions for account: " << accountNumber << std::endl;
        std::cout << "Amount of transactions: " << all_account_transactions.size() << std::endl;
        for (auto& transaction : all_account_transactions) {
            transaction->displayinfo();
        }
    }

} // namespace Banking