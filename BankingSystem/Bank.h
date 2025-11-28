#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Structs.h"

// Предварительное объявление
namespace Banking {
	class Account;
	class CheckingAccount;
	class SavingsAccount;
	class Transaction;
	class Client;
	class PremiumClient;	
}

namespace Banking {

	class Bank {
	
	private:
		std::vector<std::shared_ptr<Client>> all_clients; // все клиенты банка через умный указатель
		std::vector<std::shared_ptr<Account>> all_accounts; // все аккаунты банка через умный указатель
		std::vector<std::shared_ptr<Transaction>> all_banking_transactions; // все транзакции банка через умный указатель

	public:
		Bank() = default;
		~Bank() = default;

		// вспомогательные функции для поиска клиента по айди и аккаунта по номеру
		std::shared_ptr<Client> find_client_by_id(const int& id);
		std::shared_ptr<Account> find_acc_by_number(const std::string& accountNumber);

		// статические методы для работы с клиентами
		std::shared_ptr<Client> createClient(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value); // чтобы добавить в банк через умный указатель
		std::shared_ptr<PremiumClient> createPremiumClient(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value, const std::string& level = "Silver", double discount = 5.0);
		void addClient_in_bank(std::shared_ptr<Client> client); // Для обычных клиентов
		void addClient_in_bank(std::shared_ptr<PremiumClient> client); // Для премиум-клиентов
		size_t getClientsCount();
		bool deleteClient(int client_id);
		
		// статические методы для работы с аккаунтами (счетами)
		std::shared_ptr<CheckingAccount> createCheckAccount(const std::string& accountNumber, const int& client_id, double initialBalance = 0, double overdraft_value = 0); // чтобы добавить в банк через умный указатель
		std::shared_ptr<SavingsAccount> createSavAccount(const std::string& accountNumber, const int& client_id, double initialBalance = 5000, int months = 1); // чтобы добавить в банк через умный указатель
		void addAccount_in_bank(std::shared_ptr<Account> account);
		size_t getAccountCount();
		bool deleteAccount(const std::string& accountNumber);

		// статические операции с аккаунтами (счетами)
		void transfer(const std::string& accountNumber_from, const std::string& accountNumber_to, double amount);
		void registerDeposit(std::shared_ptr<Account> account, double amount);
		void registerWithdraw(std::shared_ptr<Account> account, double amount);

		// статические методы для работы с транзакциями
		void addTransaction_in_bank(std::shared_ptr<Transaction> transaction);

		// для отображения информации
		void display_all_clients_in_bank();
		void display_all_accounts_in_bank();
		void displayinfo_about_transactions_in_bank();

	};
};