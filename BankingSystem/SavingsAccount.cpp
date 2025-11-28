#include "SavingsAccount.h"
#include "Client.h"  // Теперь включаем здесь
#include <stdexcept>
#include <iostream>

namespace Banking {

    SavingsAccount::SavingsAccount(const std::string& accountNumber, const int& client_id, double initialBalance, int months_value)
        : Account(accountNumber, client_id, "Savings", initialBalance), months(months_value) {
        std::cout << "\n-----SavingsAccount constructor called. " << std::endl;
        if (initialBalance < 5000) {
            throw std::invalid_argument("Balance in SavingsAccount cannot be <5000");
        }
        else if (months_value < 1) {
            throw std::invalid_argument("Months value cannot be <1");
        }
        setPercentage();
        std::cout << "You created a new saving account for " << months << " months, your annual percentage: " << percentage << "%" << std::endl;
    };
    
    // функции полиморфима
    
    //увеличивает баланс счета на указанную сумму - на сбер счете логика такая что за каждую новую 1000 на счету процентная ставка растет на 1%
    void SavingsAccount::deposit(double amount) {
        Account::deposit(amount); // Базовая проверка и изменение баланса
        // Дополнительная логика
        std::cout << "Recalculating percentage after deposit..." << std::endl;
        setPercentage();
        std::cout << "Your new annual percentage: " << percentage << "%" << std::endl;

    }

    // уменьшает баланс счета на указанную сумму  - если будет меньше 30к то за снятие большее 1000 процентная ставка падает на 1%
    bool SavingsAccount::withdraw(double amount) {
        if (balance - amount < 5000) {
            std::cout << "You can't leave less than 5000 in your account" << std::endl;
            std::cout << "Maximum withdrawal amount: " << balance - 5000 << std::endl;
            return false;
        }
        // Базовая проверка и изменение баланса
        bool result = Account::withdraw(amount);
        if (result) {
            std::cout << "Recalculating percentage after withdrawal..." << std::endl;
            // Дополнительная логика
            setPercentage();
            std::cout << "Your new annual percentage: " << percentage << "%" << std::endl;
        }
        return result;
    }

    void SavingsAccount::displayinfo() const {
        Account::displayinfo();
        std::cout << "percentage_value: " << percentage << std::endl;
        std::cout << "months: " << months << std::endl;
    }

    bool SavingsAccount::canClose() const {
        return balance >= 0; // Можно закрыть если нет долгов
    }
    
    // реализация собственных функций
    void SavingsAccount::setPercentage() {
        double base_percentage = 5.0; // 5% базовая ставка

        // Бонус за сумму: +0.1% за каждые 10,000 сверх минимального баланса 5,000
        double amount_bonus = ((balance - 5000) / 10000.0) * 0.1;
        if (amount_bonus > 5.0) amount_bonus = 5.0; // Максимум +5% за сумму

        // Итоговая процентная ставка
        percentage = base_percentage + amount_bonus;

        // Максимальная ставка не более 20%
        if (percentage > 20.0) {
            percentage = 20.0;
        }

        std::cout << "Base: " << base_percentage << "%, Amount bonus: " << amount_bonus << "%" << std::endl;
    }

    double SavingsAccount::getPercentage() const {
        return percentage;
    }

}