#include "Bank.h"

#include "Client.h"  // Теперь включаем здесь
#include "PremiumClient.h" // Теперь включаем здесь

#include "Account.h"  // Теперь включаем здесь
#include "SavingsAccount.h"  // Теперь включаем здесь
#include "CheckingAccount.h"  // Теперь включаем здесь

#include "Transaction.h"  // Теперь включаем здесь

#include <stdexcept>
#include <iostream>

namespace Banking {

    // поиск клиента по айди (вспомогательная функция)
    std::shared_ptr<Client> Bank::find_client_by_id(const int& id) {
        for (auto& client : all_clients) {
            if (client->getId() == id) {
                return client;
            }
        }
        return nullptr;
    }
    
    // поиск счета по номеру (вспомогательная функция)
    std::shared_ptr<Account> Bank::find_acc_by_number(const std::string& accountNumber) {
        for (auto& account : all_accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }
        return nullptr;
    }

    // создать клиента
    std::shared_ptr<Client> Bank::createClient(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value) {
        auto client = std::make_shared<Client>(id_value, name_value, surname_value, address_value, date_value);
        addClient_in_bank(client);
        return client;
    }
    
    // создать премиум клиента
    std::shared_ptr<PremiumClient> Bank::createPremiumClient(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value, const std::string& level, double discount) {
        auto client = std::make_shared<PremiumClient>(id_value, name_value, surname_value, address_value, date_value, level, discount);
        addClient_in_bank(client);
        return client;
    }

    // добавить клиента в банк
    void Bank::addClient_in_bank(std::shared_ptr<Client> client) {
        if (find_client_by_id(client->getId()) != nullptr) {
            throw std::invalid_argument("You already have this client in bank");
        }
        all_clients.push_back(client);
        std::cout << "Client " << client->getSurname() << " added to bank. Total clients in bank: " << getClientsCount() << std::endl;
    }

    // добавить премиум клиента в банк
    void Bank::addClient_in_bank(std::shared_ptr<PremiumClient> client) {
        // Преобразуем к базовому типу и вызываем основную версию
        addClient_in_bank(std::static_pointer_cast<Client>(client));
    }
    
    // создать расчетный аккаунт (счет)
    std::shared_ptr<CheckingAccount> Bank::createCheckAccount(const std::string& accountNumber, const int& client_id, double initialBalance, double overdraft_value) {  // чтобы добавить в банк через умный указатель
        auto client = Bank::find_client_by_id(client_id);
        if (!client) {
            throw std::invalid_argument("Client with id " + std::to_string(client_id) + " not found");
        }
        auto account = std::make_shared<CheckingAccount>(accountNumber, client_id, initialBalance);
        addAccount_in_bank(account);
        client->addAccount_to_client(account);
        return account;
    }

    // создать сберегательный аккаунт (счет)
    std::shared_ptr<SavingsAccount> Bank::createSavAccount(const std::string& accountNumber, const int& client_id, double initialBalance, int months) {  // чтобы добавить в банк через умный указатель
        auto client = Bank::find_client_by_id(client_id);
        if (!client) {
            throw std::invalid_argument("Client with id " + std::to_string(client_id) + " not found");
        }
        auto account = std::make_shared<SavingsAccount>(accountNumber, client_id, initialBalance, months);
        addAccount_in_bank(account);
        client->addAccount_to_client(account);
        return account;
    }

    // добавить аккаунт (счет) в банк
    void Bank::addAccount_in_bank(std::shared_ptr<Account> account) {
        if (find_acc_by_number(account->getAccountNumber()) != nullptr) {
            throw std::invalid_argument("You already have an account with this number");
        }
        all_accounts.push_back(account);
        std::cout << "Account " << account->getAccountNumber() << " added to bank. Total accounts in bank: " << getAccountCount() << std::endl;
    }

    // перевод со счета на счет
    void Bank::transfer(const std::string& accountNumber_from, const std::string& accountNumber_to, double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Transfer amount must be positive");
        }
        if (accountNumber_from == accountNumber_to) {
            throw std::invalid_argument("Cannot transfer to the same account");
        }

        std::shared_ptr<Account> client1 = find_acc_by_number(accountNumber_from);
        std::shared_ptr<Account> client2 = find_acc_by_number(accountNumber_to);

        if (!client1) {
            throw std::invalid_argument("Source account not found: " + accountNumber_from);
        }
        if (!client2) {
            throw std::invalid_argument("Destination account not found: " + accountNumber_to);
        }

        if (client1->withdraw(amount)) { // если удалось снять (true)
            // Если снятие успешно - зачисляем средства
            try {
                client2->deposit(amount);
                auto transaction1 = Transaction::createTransaction("TRANSFER_OUT", amount, accountNumber_from, accountNumber_to); // умный ук на транзакцию
                addTransaction_in_bank(transaction1); // добавили в банк
                client1->addTransaction_in_account(transaction1); // добавили в аккаунт
                auto transaction2 = Transaction::createTransaction("TRANSFER_IN", amount, accountNumber_from, accountNumber_to); // умный ук на транзакцию
                addTransaction_in_bank(transaction2); // добавили в банк
                client2->addTransaction_in_account(transaction2); // добавили в аккаунт
                std::cout << "Transfer completed successfully!" << std::endl;
            }
            catch (const std::exception& e) {
                // Если депозит не удался - ВОЗВРАЩАЕМ средства обратно
                client1->deposit(amount); // Возвращаем снятые средства
                throw std::runtime_error("Transfer failed during deposit: " + std::string(e.what()) + ". Funds returned to source account.");
            }
        }
        else {
            throw std::runtime_error("Insufficient funds in account: " + accountNumber_from);;
        }
    }

    void Bank::registerDeposit(std::shared_ptr<Account> account, double amount) {
        account->deposit(amount); // deposit из Account
        auto transaction = Transaction::createTransaction("DEPOSIT", amount, account->getAccountNumber()); // умный указатель на транзакцию
        addTransaction_in_bank(transaction);
        account->addTransaction_in_account(transaction);
    }

    void Bank::registerWithdraw(std::shared_ptr<Account> account, double amount) {
        if (account->withdraw(amount)) { // withdraw из Account
            std::cout << "Withdrawal successful! Amount: " << amount << ", New balance: " << account->getBalance() << std::endl;
            auto transaction = Transaction::createTransaction("WITHDRAW", amount, account->getAccountNumber()); // умный указатель на транзакцию
            addTransaction_in_bank(transaction);
            account->addTransaction_in_account(transaction);
        }
    }

    // добавить транзакцию
    void Bank::addTransaction_in_bank(std::shared_ptr<Transaction> transaction) {
        all_banking_transactions.push_back(transaction);
        std::cout << "Transaction " << transaction->getType() << ", summa: " << transaction->getSumma() << " added to bank. Total transactions in bank: " << all_banking_transactions.size() << std::endl;
    }

    size_t Bank::getClientsCount() {
        return all_clients.size();
    }

    size_t Bank::getAccountCount() {
        return all_accounts.size();
    }

    void Bank::display_all_clients_in_bank() {
        std::cout << "\nInformation about ALL clients IN BANK: " << std::endl;
        std::cout << "Amount of clients: " << getClientsCount() << std::endl;
        for (auto& client : all_clients) {
            client->displayinfo();
        }
    }

    void Bank::display_all_accounts_in_bank() {
        std::cout << "\nInformation about ALL accounts IN BANK: " << std::endl;
        std::cout << "Amount of account: " << getAccountCount() << std::endl;
        for (auto& account : all_accounts) {
            account->displayinfo();
        }
    }

    void Bank::displayinfo_about_transactions_in_bank() {
        std::cout << "\nInformation about ALL transactions IN BANK: " << std::endl;
        std::cout << "Amount of transactions: " << all_banking_transactions.size() << std::endl;
        for (auto& transaction : all_banking_transactions) {
            transaction->displayinfo();
        }
    }

    // УДАЛЕНИЕ СЧЕТА
    bool Bank::deleteAccount(const std::string& accountNumber) {
        auto account = find_acc_by_number(accountNumber);
        if (!account) {
            std::cout << "Account not found: " << accountNumber << std::endl;
            return false;
        }
        // Проверяем, можно ли удалить счет
        if (!account->canClose()) {
            std::cout << "Cannot delete account " << accountNumber << ". Account has debt or restrictions." << std::endl;
            return false;
        }
        // Проверяем баланс
        if (account->getBalance() != 0) {
            std::cout << "Cannot delete account " << accountNumber << ". Balance must be zero." << std::endl;
            return false;
        }
        // Удаляем счет из банка
        for (auto it = all_accounts.begin(); it != all_accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accountNumber) {
                all_accounts.erase(it);
                std::cout << "Account " << accountNumber << " successfully deleted." << std::endl;
                return true;
            }
        }
        return false;
    }

    // УДАЛЕНИЕ КЛИЕНТА
    bool Bank::deleteClient(int client_id) {
        auto client = find_client_by_id(client_id);
        if (!client) {
            std::cout << "Client not found with ID: " << client_id << std::endl;
            return false;
        }
        // Проверяем, есть ли у клиента счета
        int accountCount = 0;
        for (const auto& account : all_accounts) {
            if (account->getClientId() == client_id) {
                accountCount++;
            }
        }
        if (accountCount > 0) {
            std::cout << "Cannot delete client " << client_id << ". Client has " << accountCount << " active accounts." << std::endl;
            return false;
        }
        // Удаляем клиента
        for (auto it = all_clients.begin(); it != all_clients.end(); ++it) {
            if ((*it)->getId () == client_id) {
                all_clients.erase(it);
                std::cout << "Client " << client_id << " successfully deleted." << std::endl;
                return true;
            }
        }
        return false;
    }

}