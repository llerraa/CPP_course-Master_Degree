cmake_minimum_required(VERSION 3.15)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги для русской кодировки
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-finput-charset=UTF-8)
    add_compile_options(-fexec-charset=UTF-8)
    add_compile_options(-fwide-exec-charset=UTF-8)
endif()

# Файлы с организацией по папкам
set(SOURCES
    src/main.cpp
    src/bank/Bank.cpp
    src/transaction/Transaction.cpp
    src/account/Account.cpp
    src/account/SavingsAccount.cpp
    src/account/CheckingAccount.cpp
    src/client/Client.cpp
    src/client/Structs.cpp
    src/client/PremiumClient.cpp
    src/menu/Menu.cpp
    src/TestBankSystem.cpp
)

set(HEADERS
    include/bank/Bank.h
    include/transaction/Transaction.h
    include/account/Account.h
    include/account/SavingsAccount.h
    include/account/CheckingAccount.h
    include/client/Client.h
    include/client/Structs.h
    include/client/PremiumClient.h
    include/menu/Menu.h
    include/TestBankSystem.h
)

# Создаем исполняемый файл
add_executable(BankingSystem ${SOURCES} ${HEADERS})

# Указываем пути для заголовков
target_include_directories(BankingSystem PRIVATE include)

# Настройки компилятора
if(MSVC)
    target_compile_options(BankingSystem PRIVATE /W4)
else()
    target_compile_options(BankingSystem PRIVATE -Wall -Wextra -Wpedantic)
endif()