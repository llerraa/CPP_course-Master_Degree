#include "Client.h"
#include <stdexcept>
#include <iostream>
#include "Account.h"  // “еперь включаем здесь


namespace Banking {
    
    Client::Client(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value)
        : id(id_value), name(name_value), surname(surname_value), address(address_value), registration_date(date_value)
    {
        // проверка данных
        if (name.empty() || surname.empty()) {
            throw std::invalid_argument("Name and surname cannot be empty");
        }
        if (id <= 0) {
            throw std::invalid_argument("ID must be positive");
        }
        if (!registration_date.isValid()) {
            throw std::invalid_argument("Invalid registration date");
        }

        std::cout << "Client constructor called for: " << getSurname() << std::endl;
    }

    // добавл€€ем счета клиенту
    void Client::addAccount_to_client(std::shared_ptr<Account> account) {
        for (const auto& existing_account : all_client_accounts) {
            if (existing_account->getAccountNumber() == account->getAccountNumber()) {
                throw std::invalid_argument("Account with number " + account->getAccountNumber() + " already exists for this client");
            }
        }

        all_client_accounts.push_back(account);
        std::cout << "Account " << account->getAccountNumber() << " added to client " << getSurname() << " Total accounts in client: " << all_client_accounts.size() << std::endl;
    };
    
    // показываем счета клиента
    void Client::displayinfo_about_client_accounts() {
        std::cout << "\nInformation about accounts for client: " << getSurname() << std::endl;
        std::cout << "Amount of accounts: " << all_client_accounts.size() << std::endl;
        for (auto& account : all_client_accounts) {
            account->displayinfo();
        }
    };
}