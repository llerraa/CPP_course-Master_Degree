#pragma once
#include <string>
#include <vector>
#include "Account.h"

// Предварительное объявление вместо включения 
namespace Banking {
    class Client;
}

namespace Banking {

    class CheckingAccount : public Account {
    private:
        double commission;
        double available_overdraft;
        double overdraft_limit;

    public:

        CheckingAccount(const std::string& accountNumber, const int& client_id, double initialBalance = 0);
        virtual ~CheckingAccount();

        // функции полиморфима
        void deposit(double amount) override;
        bool withdraw(double amount) override;
        void displayinfo() const override;
        bool canClose() const override;

        // свои собственные функции
        void set_overdraft_limit();
        void setCommission(double amount);
        double get_overdraft_limit() const;
        double get_available_overdraft() const;
        
    };
}
