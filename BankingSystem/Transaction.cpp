#include "Transaction.h"
#include "Bank.h"  // Теперь включаем здесь

#include <stdexcept>
#include <iostream>
#include <iomanip> // для времени
#include <sstream> // для времени

namespace Banking {

    Transaction::Transaction(const std::string& type, double summa, const std::string& acc1, const std::string& acc2)
        : acc1(acc1), acc2(acc2), timestamp(std::time(nullptr))  // Текущее время
    {
        // Валидация через сеттеры
        setType(type);
        setSumma(summa);
        setAcc1(acc1);
        setAcc2(acc2);
        
        // уникальный ID на основе времени
        static int counter = 1000; 
        id = ++counter;

        std::cout << "\n-----Transaction constructor called. ID: " << getFormattedId() << std::endl;
    }

    std::shared_ptr<Transaction> Transaction::createTransaction(const std::string& type, double summa, const std::string& acc1, const std::string& acc2) {  // чтобы добавить в банк через умный указатель
        auto transaction = std::make_shared<Transaction>(type, summa, acc1, acc2);
        return transaction;
    }

    Transaction::~Transaction() {
        std::cout << "\n-----Transaction destructor called. ID: " << id << std::endl;
    }

    std::string Transaction::getFormattedTime() const {
        std::tm local_time;
        localtime_s(&local_time, &timestamp);  // Windows
        // localtime_r(&timestamp, &local_time);  // Linux/Mac
        std::ostringstream oss;
        oss << std::put_time(&local_time, "%d.%m.%Y %H:%M:%S");
        return oss.str();
    }

    void Transaction::displayinfo() {
        std::cout << "id: " << getFormattedId() << std::endl;
        std::cout << "time: " << getFormattedTime() << std::endl;
        std::cout << "type: " << getType() << std::endl;
        std::cout << "amount: " << getSumma() << std::endl;
        std::cout << "account(-s): " << getAccounts() << std::endl;
        std::cout << "-----" << std::endl;
    }
}