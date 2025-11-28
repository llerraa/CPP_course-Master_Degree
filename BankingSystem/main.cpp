#include <iostream>
#include <windows.h>

#include "Bank.h"
#include "Menu.h"
#include "TestBankSystem.h"

using namespace Banking;

int main() {
// Настройка консоли для UTF-8
SetConsoleOutputCP(CP_UTF8);
SetConsoleCP(CP_UTF8);

    std::cout << "Banking System Started!" << std::endl;

    // Start menu system
    Menu menu;
    menu.showMainMenu();

    std::cout << "System shutdown." << std::endl;
    return 0;
}