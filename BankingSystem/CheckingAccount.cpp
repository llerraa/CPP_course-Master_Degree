#include "CheckingAccount.h"
#include "Client.h"  // Теперь включаем здесь
#include <stdexcept>
#include <iostream>

namespace Banking {

    CheckingAccount::CheckingAccount(const std::string& accountNumber, const int& client_id, double initialBalance)
        : Account(accountNumber, client_id, "Checking", initialBalance) {
        std::cout << "\n-----CheckingAccount constructor called. ";
        commission = 0.0;
        overdraft_limit = 0.0;
        available_overdraft = 0.0;
        set_overdraft_limit(); // Устанавливаем максимальный лимит
        available_overdraft = overdraft_limit; // изначально весь овердрафт доступен
        std::cout << "You created a new checking account." << std::endl;
        std::cout << "Overdraft limit: " << overdraft_limit << ", Available overdraft: " << available_overdraft << std::endl;
    };
    
    CheckingAccount::~CheckingAccount() {
        std::cout << "\n-----CheckingAccount destructor called. " << std::endl;
    }

    // функции полиморфима

    //увеличивает баланс счета на указанную сумму - увеличивает лимит овердрафта на 2% за каждые 5к при пополнении, если на счету больше 50к
    void CheckingAccount::deposit(double amount) {
        Account::deposit(amount); // Базовая проверка и изменение баланса

        // сохраняем старый лимит для сравнения
        double old_limit = overdraft_limit;

        // Пересчитываем лимит овердрафта
        set_overdraft_limit();

        // при пополнении ВОССТАНАВЛИВАЕМ доступный овердрафт
        // Если лимит увеличился, увеличиваем и доступный овердрафт
        if (overdraft_limit > old_limit) {
            double limit_increase = overdraft_limit - old_limit;
            available_overdraft += limit_increase;

            // Не может превышать лимит
            if (available_overdraft > overdraft_limit) {
                available_overdraft = overdraft_limit;
            }

            std::cout << "Overdraft limit increased from " << old_limit << " to " << overdraft_limit << std::endl;
            std::cout << "Available overdraft updated to: " << available_overdraft << std::endl;
        }
        else if (overdraft_limit < old_limit) {
            // Если лимит уменьшился, уменьшаем и доступный овердрафт
            if (available_overdraft > overdraft_limit) {
                available_overdraft = overdraft_limit;
            }
            std::cout << "Overdraft limit decreased from " << old_limit << " to " << overdraft_limit << std::endl;
            std::cout << "Available overdraft updated to: " << available_overdraft << std::endl;
        }

        std::cout << "Your new overdraft_limit: " << overdraft_limit << std::endl;
        std::cout << "Your available overdraft: " << available_overdraft << std::endl;
    }

    // уменьшает баланс счета на указанную сумму  - комиссия 2% за каждые 500 руб снятия, но не больше 20%
    bool CheckingAccount::withdraw(double amount) {
        setCommission(amount);
        std::cout << "\nYou want to withdraw: " << amount << ", commission: " << commission << std::endl;
        double total_amount = amount + commission;
        std::cout << "Total amount to withdraw: " << total_amount << std::endl;
        std::cout << "Balance: " << balance << std::endl;
        std::cout << "Maximum withdrawal amount (balance + available_overdraft): " << (balance + available_overdraft) << std::endl;

        double available_funds = balance + available_overdraft;
        std::cout << "Available funds (balance + overdraft): " << available_funds << std::endl;

        if (total_amount > available_funds) {
            std::cout << "Insufficient funds! Cannot withdraw " << total_amount << std::endl;
            return false;
        }

        // Выполняем снятие
        if (total_amount <= balance) {
            // Снимаем без использования овердрафта
            balance -= total_amount;
        }
        else {
            // Используем овердрафт
            double overdraft_needed = total_amount - balance;
            balance = 0;
            available_overdraft -= overdraft_needed; // уменьшаем доступный овердрафт

            // предотвращаем отрицательный овердрафт
            if (available_overdraft < 0) {
                available_overdraft = 0;
            }
        
        std::cout << "Overdraft used: " << overdraft_needed << ", Remaining overdraft: " << available_overdraft << std::endl;
        }

        // пересчитываем лимит после операции
        set_overdraft_limit();
        std::cout << "Withdrawal successful! New balance: " << balance << std::endl;
        std::cout << "Your new overdraft limit: " << overdraft_limit << std::endl;
        return true;
    }

    void CheckingAccount::displayinfo() const {
        Account::displayinfo();
        std::cout << "Available overdraft: " << available_overdraft << std::endl;
        std::cout << "Overdraft limit: " << overdraft_limit << std::endl;
    }

    bool CheckingAccount::canClose() const {
        return balance >= 0; // Можно закрыть если нет долгов
    }

    // свои собственные функции
    void CheckingAccount::set_overdraft_limit() { 
        double base_limit = 50000.0;

        // Бонус за баланс: +10% лимита за каждые 10 000 сверх 50 000
        if (balance > 50000) {
            double bonus_multiplier = ((balance - 50000) / 10000.0) * 0.1;
            overdraft_limit = base_limit * (1.0 + bonus_multiplier);
        }
        else {
            overdraft_limit = base_limit;
        }

        // Максимальный лимит
        if (overdraft_limit > 100000) {
            overdraft_limit = 100000;
        }

        // доступный овердрафт не может превышать лимит
        if (available_overdraft > overdraft_limit) {
            available_overdraft = overdraft_limit;
        }
    }

    void CheckingAccount::setCommission(double amount) {
        double perc_of_commission = 2 * (amount / 500);
        if (perc_of_commission > 20) {
            perc_of_commission = 20;
        }
        commission = (amount / 100) * perc_of_commission;
    }

    double CheckingAccount::get_overdraft_limit() const {
        return overdraft_limit;
    }

    double CheckingAccount::get_available_overdraft() const {
        return available_overdraft;
    }

}