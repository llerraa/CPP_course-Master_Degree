#include "Menu.h"
#include <iostream>
#include <limits>

using namespace Banking;

void Menu::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int Menu::getNumber(const std::string& message) {
    int value;
    while (true) { //  бесконечный цикл, который будет повторяться до успешного ввода 
        std::cout << message;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        else {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInput();
        }
    }
}

double Menu::getDouble(const std::string& message) {
    double value;
    while (true) {
        std::cout << message;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        else {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInput();
        }
    }
}

std::string Menu::getString(const std::string& message) {
    std::string value;
    std::cout << message;
    std::getline(std::cin, value);
    return value;
}

void Menu::showMainMenu() {
    while (true) {
        std::cout << "\nBANKING SYSTEM" << std::endl;
        std::cout << "1. Client Management" << std::endl;
        std::cout << "2. Account Management" << std::endl;
        std::cout << "3. Banking Operations" << std::endl;
        std::cout << "4. Reports" << std::endl;
        std::cout << "5. Exit" << std::endl;

        int choice = getNumber("Select option: ");

        switch (choice) {
        case 1: clientMenu(); break;
        case 2: accountMenu(); break;
        case 3: transactionMenu(); break;
        case 4: reportMenu(); break;
        case 5:
            std::cout << "Goodbye!" << std::endl;
            return;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    }
}

void Menu::clientMenu() {
    while (true) {
        std::cout << "\nCLIENT MANAGEMENT" << std::endl;
        std::cout << "1. Create Regular Client" << std::endl;
        std::cout << "2. Create Premium Client" << std::endl;
        std::cout << "3. Delete Client" << std::endl;
        std::cout << "4. Show All Clients" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;

        int choice = getNumber("Select option: ");

        switch (choice) {
        case 1: createClient(); break;
        case 2: createPremiumClient(); break;
        case 3: deleteClient(); break;
        case 4: showAllClients(); break;
        case 5: return;
        default: std::cout << "Invalid choice." << std::endl;
        }
    }
}

void Menu::createClient() {
    std::cout << "\nCREATE CLIENT" << std::endl;

    int id = getNumber("Client ID: ");
    std::string name = getString("First name: ");
    std::string surname = getString("Last name: ");

    std::string street = getString("Street: ");
    std::string city = getString("City: ");
    std::string country = getString("Country: ");
    int postId = getNumber("Postal code: ");

    int day = getNumber("Registration day: ");
    int month = getNumber("Registration month: ");
    int year = getNumber("Registration year: ");

    try {
        Address address(street, city, country, postId);
        Date date(day, month, year);

        auto client = bank.createClient(id, name, surname, address, date);
        std::cout << "Client created successfully!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "Error creating client: " << e.what() << std::endl;
    }
}

void Menu::createPremiumClient() {
    std::cout << "\nCREATE PREMIUM CLIENT" << std::endl;

    int id = getNumber("Client ID: ");
    std::string name = getString("First name: ");
    std::string surname = getString("Last name: ");

    std::string street = getString("Street: ");
    std::string city = getString("City: ");
    std::string country = getString("Country: ");
    int postId = getNumber("Postal code: ");

    int day = getNumber("Registration day: ");
    int month = getNumber("Registration month: ");
    int year = getNumber("Registration year: ");

    std::cout << "\nSelect premium level:" << std::endl;
    std::cout << "1. Silver (5% discount)" << std::endl;
    std::cout << "2. Gold (10% discount)" << std::endl;
    std::cout << "3. Platinum (15% discount)" << std::endl;

    int levelChoice = getNumber("Select level (1-3): ");
    std::string level;

    switch (levelChoice) {
    case 1: level = "Silver"; break;
    case 2: level = "Gold"; break;
    case 3: level = "Platinum"; break;
    default:
        std::cout << "Invalid choice, setting to Silver by default." << std::endl;
        level = "Silver";
    }

    try {
        Address address(street, city, country, postId);
        Date date(day, month, year);

        auto client = bank.createPremiumClient(id, name, surname, address, date, level);
        std::cout << "Premium client created successfully!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "Error creating premium client: " << e.what() << std::endl;
    }
}

void Menu::deleteClient() {
    std::cout << "\nDELETE CLIENT" << std::endl;
    int clientId = getNumber("Enter client ID to delete: ");

    if (bank.deleteClient(clientId)) {
        std::cout << "Client deleted successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to delete client." << std::endl;
    }
}

void Menu::showAllClients() {
    bank.display_all_clients_in_bank();
}

void Menu::accountMenu() {
    while (true) {
        std::cout << "\nACCOUNT MANAGEMENT" << std::endl;
        std::cout << "1. Create Checking Account" << std::endl;
        std::cout << "2. Create Savings Account" << std::endl;
        std::cout << "3. Delete Account" << std::endl;
        std::cout << "4. Show All Accounts" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;

        int choice = getNumber("Select option: ");

        switch (choice) {
        case 1: createCheckingAccount(); break;
        case 2: createSavingsAccount(); break;
        case 3: deleteAccount(); break;
        case 4: showAllAccounts(); break;
        case 5: return;
        default: std::cout << "Invalid choice." << std::endl;
        }
    }
}

// отдельная функция для создания расчетного счета
void Menu::createCheckingAccount() {
    std::cout << "\nCREATE CHECKING ACCOUNT" << std::endl;

    std::string accountNumber = getString("Account number: ");
    int clientId = getNumber("Client ID: ");
    double balance = getDouble("Initial balance: ");

    try {
        auto account = bank.createCheckAccount(accountNumber, clientId, balance);
        std::cout << "Checking account created successfully!" << std::endl;
        std::cout << "Overdraft limit will be calculated automatically based on balance." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating checking account: " << e.what() << std::endl;
    }
}

// отдельная функция для создания сберегательного счета
void Menu::createSavingsAccount() {
    std::cout << "\nCREATE SAVINGS ACCOUNT" << std::endl;

    std::string accountNumber = getString("Account number: ");
    int clientId = getNumber("Client ID: ");
    double balance = getDouble("Initial balance: ");
    int months = getNumber("Term in months: ");

    try {
        auto account = bank.createSavAccount(accountNumber, clientId, balance, months);
        std::cout << "Savings account created successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating savings account: " << e.what() << std::endl;
    }
}

void Menu::deleteAccount() {
    std::cout << "\nDELETE ACCOUNT" << std::endl;
    std::string accountNumber = getString("Enter account number to delete: ");

    if (bank.deleteAccount(accountNumber)) {
        std::cout << "Account deleted successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to delete account." << std::endl;
    }
}

void Menu::showAllAccounts() {
    bank.display_all_accounts_in_bank();
}

void Menu::transactionMenu() {
    while (true) {
        std::cout << "\nBANKING OPERATIONS" << std::endl;
        std::cout << "1. Make Deposit" << std::endl;
        std::cout << "2. Make Withdrawal" << std::endl;
        std::cout << "3. Make Transfer" << std::endl;
        std::cout << "4. Back to Main Menu" << std::endl;

        int choice = getNumber("Select option: ");

        switch (choice) {
        case 1: makeDeposit(); break;
        case 2: makeWithdrawal(); break;
        case 3: makeTransfer(); break;
        case 4: return;
        default: std::cout << "Invalid choice." << std::endl;
        }
    }
}

void Menu::makeDeposit() {
    std::cout << "\nMAKE DEPOSIT" << std::endl;
    std::string accountNumber = getString("Account number: ");
    double amount = getDouble("Amount: ");

    try {
        auto account = bank.find_acc_by_number(accountNumber);
        if (account) {
            bank.registerDeposit(account, amount);
            std::cout << "Deposit successful!" << std::endl;
        }
        else {
            std::cout << "Account not found!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error making deposit: " << e.what() << std::endl;
    }
}

void Menu::makeWithdrawal() {
    std::cout << "\nMAKE WITHDRAWAL" << std::endl;
    std::string accountNumber = getString("Account number: ");
    double amount = getDouble("Amount: ");

    try {
        auto account = bank.find_acc_by_number(accountNumber);
        if (account) {
            bank.registerWithdraw(account, amount);
            std::cout << "Withdrawal successful!" << std::endl;
        }
        else {
            std::cout << "Account not found!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error making withdrawal: " << e.what() << std::endl;
    }
}

void Menu::makeTransfer() {
    std::cout << "\nMAKE TRANSFER" << std::endl;
    std::string fromAccount = getString("From account: ");
    std::string toAccount = getString("To account: ");
    double amount = getDouble("Amount: ");

    try {
        bank.transfer(fromAccount, toAccount, amount);
        std::cout << "Transfer successful!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error making transfer: " << e.what() << std::endl;
    }
}

void Menu::reportMenu() {
    while (true) {
        std::cout << "\nREPORTS" << std::endl;
        std::cout << "1. Show All Clients" << std::endl;
        std::cout << "2. Show All Accounts" << std::endl;
        std::cout << "3. Show All Transactions" << std::endl;
        std::cout << "4. Back to Main Menu" << std::endl;

        int choice = getNumber("Select option: ");

        switch (choice) {
        case 1: showAllClients(); break;
        case 2: showAllAccounts(); break;
        case 3: showAllTransactions(); break;
        case 4: return;
        default: std::cout << "Invalid choice." << std::endl;
        }
    }
}

void Menu::showAllTransactions() {
    bank.displayinfo_about_transactions_in_bank();
}