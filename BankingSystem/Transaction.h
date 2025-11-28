#pragma once
#include <string>
#include <vector>
#include <memory>
#include <ctime>  // для std::time_t
#include <stdexcept> 
#include <algorithm>  // для std::find

// Предварительное объявление вместо включения Bank.h
namespace Banking {
    class Bank;
}

namespace Banking {

    class Transaction {
    private:
        int id;
        std::string acc1;
        std::string acc2;
        double summa;
        std::time_t timestamp;
        std::string type;

    public:
        Transaction(const std::string& type, double summa, const std::string& acc1, const std::string& acc2 = " ");
        static std::shared_ptr<Transaction> createTransaction(const std::string& type, double summa, const std::string& acc1, const std::string& acc2 = " "); // чтобы добавить в банк через умный указатель
        virtual ~Transaction();

        void displayinfo();

        //Геттеры
        int getId() const { return id; }
        double getSumma() const { return summa; }
        std::string getType() const { return type; }
        std::string getAcc1() const { return acc1; }
        std::string getAcc2() const { return acc2; }
        std::time_t getTimestamp() const { return timestamp; }

        std::string getFormattedTime() const; // красивое отображение времени
        std::string getFormattedId() const { // красивое создание айди
            return "T-" + std::to_string(id); }

        // для отображения информации о транзакциях
        std::string getAccounts() const {
            if (acc2 == " ") {
                return acc1;
            }
            else {
                return acc1 + " -> " + acc2;
            }
        }

        // Сеттеры с валидацией
        void setSumma(double newSumma) {
            if (newSumma <= 0) {
                throw std::invalid_argument("Transaction amount must be positive"); }
            summa = newSumma;
        }

        void setType(const std::string& newType) {
            if (newType.empty()) {
                throw std::invalid_argument("Transaction type cannot be empty");
            }
            // проверка допустимых типов
            static const std::vector<std::string> validTypes = {
                "DEPOSIT", "WITHDRAW", "TRANSFER_IN", "TRANSFER_OUT"
            };
            if (std::find(validTypes.begin(), validTypes.end(), newType) == validTypes.end()) {
                throw std::invalid_argument("Invalid transaction type");
            }
            type = newType;
        }

        void setAcc1(const std::string& newAcc1) {
            if (newAcc1.empty()) {
                throw std::invalid_argument("Account number cannot be empty");
            }
            acc1 = newAcc1;
        }

        void setAcc2(const std::string& newAcc2) {
            acc2 = newAcc2; // acc2 может быть пустым для некоторых операций
        }

    };

}
