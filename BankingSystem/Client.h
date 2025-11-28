#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Structs.h"

// Предварительное объявление вместо включения
namespace Banking {
    class Account;
}

namespace Banking {
    class Client {

    private:
        int id;
        std::string name;
        std::string surname;
        Address address; // структура
        Date registration_date; // структура
        std::vector<std::shared_ptr<Account>> all_client_accounts; // все счета клиента через умный указатель

    public:
        Client(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value);
        virtual ~Client() = default;

        ////Не виртуальные функции
        void addAccount_to_client(std::shared_ptr<Account> account); // делаем не статичную так как нужно индивидуально под каждый объект = под каждого клиента
        void displayinfo_about_client_accounts();

        // ГЕТТЕРЫ для всех атрибутов
        int getId() const { return id; }
        std::string getName() const { return name; }
        std::string getSurname() const { return surname; }
        const Address& getAddress() const { return address; }
        const Date& getRegistrationDate() const { return registration_date; }

        // СЕТТЕРЫ для изменяемых атрибутов
        void setName(const std::string& name_value) { name = name_value; }
        void setSurname(const std::string& surname_value) { surname = surname_value; }
        void setAddress(const Address& address_value) { address = address_value; }
        void setRegistrationDate(const Date& date_value) { registration_date = date_value; }

        // ВИРТУАЛЬНЫЕ методы отображения информации
        virtual void displayinfo() const {
            std::cout << "\nClient Information:" << std::endl;
            std::cout << "ID: " << id << std::endl;
            std::cout << "Name: " << name << " " << surname << std::endl;
            std::cout << "Address: " << address.getFullAddress() << std::endl;
            std::cout << "Registration Date: " << registration_date.getFormattedDate() << std::endl;
            std::cout << "Amount of accounts: " << all_client_accounts.size() << std::endl;
        }

        // проверка для конструктора
        bool isAddressValid() const {
            return !address.street.empty() && !address.city.empty() && !address.country.empty();
        }
    
    };

};