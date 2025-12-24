#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class OptimizedRepo {
private:
    sqlite3* db;
    
public:
    OptimizedRepo(sqlite3* database) : db(database) {}
    
    void createIndexes() {
        sqlite3_exec(db, "CREATE INDEX IF NOT EXISTS idx_email ON students(email);", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "CREATE INDEX IF NOT EXISTS idx_group ON students(group_name);", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "CREATE INDEX IF NOT EXISTS idx_subject ON grades(subject);", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "CREATE INDEX IF NOT EXISTS idx_student_id ON grades(student_id);", nullptr, nullptr, nullptr);
    }
    
    void batchInsertStudents(int count) {
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        
        const std::string sql = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        
        for (int i = 0; i < count; i++) {
            std::string name = "Student" + std::to_string(i);
            std::string email = "student" + std::to_string(i) + "@university.ru";
            std::string group = "GROUP-" + std::to_string(i % 10);
            
            sqlite3_reset(stmt);
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, group.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
        }
        
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    }
    
    void testPerformance() {
        std::cout << "=== Тестирование производительности ===\n";
        
        // Тест 1: Пакетная вставка 1000 записей
        auto start = std::chrono::high_resolution_clock::now();
        batchInsertStudents(1000);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Пакетная вставка 1000 студентов: " << duration_ms.count() << " мс\n";
        
        // Тест 2: Поиск по email
        start = std::chrono::high_resolution_clock::now();
        const std::string searchSQL = "SELECT COUNT(*) FROM students WHERE email LIKE '%500%'";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, searchSQL.c_str(), -1, &stmt, nullptr);
        sqlite3_step(stmt);
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        end = std::chrono::high_resolution_clock::now();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Поиск по email: " << duration_us.count() << " мкс (найдено: " << count << ")\n";
        
        // Тест 3: Группировка
        start = std::chrono::high_resolution_clock::now();
        const std::string groupSQL = "SELECT group_name, COUNT(*) FROM students GROUP BY group_name";
        sqlite3_prepare_v2(db, groupSQL.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {}
        sqlite3_finalize(stmt);
        end = std::chrono::high_resolution_clock::now();
        duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Группировка по группам: " << duration_ms.count() << " мс\n";
        
        // Тест 4: JOIN запрос
        start = std::chrono::high_resolution_clock::now();
        const std::string joinSQL = R"(
            SELECT s.name, COUNT(g.id) 
            FROM students s 
            LEFT JOIN grades g ON s.id = g.student_id 
            GROUP BY s.id 
            LIMIT 100
        )";
        sqlite3_prepare_v2(db, joinSQL.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {}
        sqlite3_finalize(stmt);
        end = std::chrono::high_resolution_clock::now();
        duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "JOIN запрос (100 записей): " << duration_ms.count() << " мс\n";
    }
    
    void runAllTests() {
        std::cout << "=== Базовые тесты ===\n";
        
        // Тест 1: Добавление студента
        {
            const std::string sql = "INSERT INTO students (name, email, group_name) VALUES ('Test', 'test@test.ru', 'TEST')";
            bool ok = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
            std::cout << "Тест 1 - Добавление: " << (ok ? "PASS" : "FAIL") << "\n";
        }
        
        // Тест 2: Получение студента
        {
            const std::string sql = "SELECT name FROM students WHERE email = 'test@test.ru'";
            sqlite3_stmt* stmt;
            bool ok = false;
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    ok = true;
                }
                sqlite3_finalize(stmt);
            }
            std::cout << "Тест 2 - Получение: " << (ok ? "PASS" : "FAIL") << "\n";
        }
        
        // Тест 3: Уникальность email
        {
            const std::string sql = "INSERT INTO students (name, email, group_name) VALUES ('Duplicate', 'test@test.ru', 'TEST')";
            bool ok = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK;
            std::cout << "Тест 3 - Уникальность email: " << (ok ? "PASS" : "FAIL") << "\n";
        }
        
        // Тест 4: Внешний ключ
        {
            const std::string sql = "INSERT INTO grades (student_id, subject, grade) VALUES (9999, 'Math', 100)";
            bool ok = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK;
            std::cout << "Тест 4 - Внешний ключ: " << (ok ? "PASS" : "FAIL") << "\n";
        }
    }
};

int main() {
    sqlite3* db;
    sqlite3_open("university.db", &db);
    
    // Включаем поддержку внешних ключей
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    
    OptimizedRepo repo(db);
    repo.createIndexes();
    std::cout << "Индексы созданы\n";
    
    repo.runAllTests();
    repo.testPerformance();
    
    sqlite3_close(db);
    return 0;
}