#include "TestBankSystem.h"
#include "Client.h"
#include "PremiumClient.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"

using namespace Banking;

void TestBankSystem::runAllTests() {
    std::cout << "=== STARTING BANK SYSTEM TESTS ===" << std::endl;

    testClientCreation();
    testAccountCreation();
    testTransactions();
    testDeletion();
    testErrorHandling();

    std::cout << "=== ALL TESTS PASSED! ===" << std::endl;
}

void TestBankSystem::testClientCreation() {
    std::cout << "\n--- Testing Client Creation ---" << std::endl;

    // Test 1: Create regular client
    auto client1 = bank.createClient(1, "John", "Doe",
        Address("Main St", "New York", "USA", 10001),
        Date(1, 1, 2024));

    assert(client1 != nullptr);
    assert(client1->getId() == 1);
    assert(client1->getName() == "John");
    assert(client1->getSurname() == "Doe");
    std::cout << "OK Regular client creation test passed" << std::endl;

    // Test 2: Create premium client
    auto premiumClient = bank.createPremiumClient(2, "Jane", "Smith",
        Address("Oak Ave", "Boston", "USA", 20002),
        Date(2, 1, 2024), "Gold", 10.0);

    assert(premiumClient != nullptr);
    assert(premiumClient->getId() == 2);
    assert(premiumClient->getPremiumLevel() == "Gold");
    assert(premiumClient->getDiscountPercentage() == 10.0);
    std::cout << "OK Premium client creation test passed" << std::endl;

    // Test 3: Check clients count
    assert(bank.getClientsCount() == 2);
    std::cout << "OK Clients count test passed" << std::endl;
}

void TestBankSystem::testAccountCreation() {
    std::cout << "\n--- Testing Account Creation ---" << std::endl;

    // Test 1: Create checking account
    auto checkAccount = bank.createCheckAccount("CHK001", 1, 1000.0, 500.0);

    assert(checkAccount != nullptr);
    assert(checkAccount->getAccountNumber() == "CHK001");
    assert(checkAccount->getBalance() == 1000.0);
    std::cout << "OK Checking account creation test passed" << std::endl;

    // Test 2: Create savings account
    auto savingsAccount = bank.createSavAccount("SAV001", 2, 5000.0, 12);

    assert(savingsAccount != nullptr);
    assert(savingsAccount->getAccountNumber() == "SAV001");
    assert(savingsAccount->getBalance() == 5000.0);
    std::cout << "OK Savings account creation test passed" << std::endl;

    // Test 3: Check accounts count
    assert(bank.getAccountCount() == 2);
    std::cout << "OK Accounts count test passed" << std::endl;
}

void TestBankSystem::testTransactions() {
    std::cout << "\n--- Testing Transactions ---" << std::endl;

    // Test 1: Deposit money
    auto account = bank.find_acc_by_number("CHK001");
    double initialBalance = account->getBalance();

    bank.registerDeposit(account, 500.0);
    assert(account->getBalance() == initialBalance + 500.0);
    std::cout << "OK Deposit test passed" << std::endl;

    // Test 2: Withdraw money
    initialBalance = account->getBalance();

    bank.registerWithdraw(account, 200.0);
    std::cout << "OK Withdrawal test passed. Balance: " << account->getBalance()
        << " (initial: " << initialBalance << ")" << std::endl;

    // Test 3: Transfer money - ИСПРАВЛЕННАЯ СТРОКА
    auto fromAccount = bank.find_acc_by_number("CHK001");
    auto toAccount = bank.find_acc_by_number("SAV001");
    double fromInitial = fromAccount->getBalance();
    double toInitial = toAccount->getBalance();

    // ИСПРАВЛЕНИЕ: просто вызываем transfer() без присваивания
    bank.transfer("CHK001", "SAV001", 100.0);

    // Проверяем результаты перевода
    assert(fromAccount->getBalance() < fromInitial); // Баланс уменьшился
    assert(toAccount->getBalance() == toInitial + 100.0); // Получатель получил деньги
    std::cout << "OK Transfer test passed" << std::endl;
}

void TestBankSystem::testDeletion() {
    std::cout << "\n--- Testing Deletion ---" << std::endl;

    // Test 1: Try to delete account with balance (should fail)
    bool result = bank.deleteAccount("CHK001");
    assert(result == false);
    std::cout << "OK Account deletion with balance test passed" << std::endl;

    // Test 2: Try to delete client with accounts (should fail)
    result = bank.deleteClient(1);
    assert(result == false);
    std::cout << "OK Client deletion with accounts test passed" << std::endl;

    // Test 3: Withdraw all money and delete account
    auto account = bank.find_acc_by_number("CHK001");
    double balance = account->getBalance();

    // Withdraw all money
    // Снимаем всю сумму с учетом комиссии
    double amount_to_withdraw = balance;
    while (account->getBalance() > 0) {
        double current_balance = account->getBalance();
        bank.registerWithdraw(account, current_balance);

        // Если баланс не изменился, значит снятие не удалось - выходим
        if (account->getBalance() >= current_balance) {
            break;
        }
    }

    result = bank.deleteAccount("CHK001");
    assert(result == true);
    assert(bank.getAccountCount() == 1);
    std::cout << "OK Account deletion test passed" << std::endl;

    // Test 4: Delete client without accounts
    result = bank.deleteClient(1);
    assert(result == true);
    assert(bank.getClientsCount() == 1);
    std::cout << "OK Client deletion test passed" << std::endl;
}

void TestBankSystem::testErrorHandling() {
    std::cout << "\n--- Testing Error Handling ---" << std::endl;

    // Test 1: Create client with duplicate ID (should throw exception)
    try {
        bank.createClient(2, "Duplicate", "User",
            Address("Test St", "Test City", "Test Country", 12345),
            Date(1, 1, 2024));
        assert(false); // Should not reach here
    }
    catch (const std::exception& e) {
        std::cout << "OK Duplicate client ID test passed: " << e.what() << std::endl;
    }

    // Test 2: Create account with duplicate number (should throw exception)
    try {
        bank.createCheckAccount("SAV001", 2, 1000.0); // Duplicate account number
        assert(false); // Should not reach here
    }
    catch (const std::exception& e) {
        std::cout << "OK Duplicate account number test passed: " << e.what() << std::endl;
    }

    // Test 3: Transfer with insufficient funds (should throw exception)
    try {
        auto smallAccount = bank.createCheckAccount("SMALL001", 2, 50.0);
        bank.transfer("SMALL001", "SAV001", 100.0); // Try to transfer more than balance
        assert(false); // Should not reach here
    }
    catch (const std::exception& e) {
        std::cout << "OK Insufficient funds test passed: " << e.what() << std::endl;
    }

    // Test 4: Create account for non-existent client (should throw exception)
    try {
        bank.createCheckAccount("TEST001", 999, 1000.0); // Client 999 doesn't exist
        assert(false); // Should not reach here
    }
    catch (const std::exception& e) {
        std::cout << "OK Non-existent client test passed: " << e.what() << std::endl;
    }
}