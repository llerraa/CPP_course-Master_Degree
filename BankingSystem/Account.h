#pragma once
#include <string>
#include <vector>
#include <memory> 

// Предварительное объявление вместо включения
namespace Banking {
    class Transaction; 
    class Client;
}

namespace Banking {

    class Account {
    private:
        std::string accountNumber;
        int client_id;
        std::string type;
        std::vector<std::shared_ptr<Transaction>> all_account_transactions; // все транзакции аккаунта через умный указатель

    protected:
        // для доступа в наследниках
    double balance; 
    
    public:
        Account(const std::string& accountNumber, const int& client_id, const std::string& type, double initialBalance = 0);
        virtual ~Account();

        // Виртуальные функции для полиморфизма
        virtual void deposit(double amount);
        virtual bool withdraw(double amount);
        virtual void displayinfo() const;

        // Можно ли закрыть счет
        virtual bool canClose() const = 0;             
        
        // Геттеры
        std::string getAccountNumber() const;
        double getBalance() const;
        std::string getType() const { return type; }
        int getClientId() const { return client_id; }

        // Не виртуальные функции
        void addTransaction_in_account(std::shared_ptr<Transaction> transaction); // делаем не статичную в отличие от банковской функции (так как нужно индивидуально под каждый объект = под каждый счет)
        void displayinfo_about_transactions_in_account();
    };

} // namespace Banking