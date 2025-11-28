#pragma once

#include "Bank.h"
#include <cassert>
#include <iostream>

class TestBankSystem {
private:
    Banking::Bank bank;

    void testClientCreation();
    void testAccountCreation();
    void testTransactions();
    void testDeletion();
    void testErrorHandling();

public:
    void runAllTests();
};