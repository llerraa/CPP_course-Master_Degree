#pragma once
#include <string>
#include <vector>
#include "Account.h"

// Предварительное объявление вместо включения 
namespace Banking {
    class Client;
}

namespace Banking {

    class SavingsAccount : public Account {
    private:
        double percentage;
        int months;
    
    public:
        
        SavingsAccount(const std::string& accountNumber, const int& client_id, double initialBalance=5000, int months=1);
        virtual ~SavingsAccount() = default;

        // функции полиморфима
        void deposit(double amount) override;
        bool withdraw(double amount) override;
        void displayinfo() const override;
        bool canClose() const override;

        // свои собственные функции
        void setPercentage();
        double getPercentage() const;

    };
}