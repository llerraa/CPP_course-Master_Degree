#include <sqlite3.h>
#include <iostream>
#include <string>
#include <memory>

class DatabaseManager {
private:
    sqlite3* db;
    
public:
    DatabaseManager() : db(nullptr) {}
    
    ~DatabaseManager() {
        if (db) {
            sqlite3_close(db);
        }
    }
    
    bool connect(const std::string& filename) {
        if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Ошибка подключения: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        return true;
    }
    
    void optimize() {
        execute("PRAGMA foreign_keys = ON;");
        execute("PRAGMA journal_mode = WAL;");
        execute("PRAGMA cache_size = -64000;");
        sqlite3_busy_timeout(db, 3000);
    }
    
    bool createTables() {
        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS students (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT UNIQUE,
                group_name TEXT
            );
            
            CREATE TABLE IF NOT EXISTS grades (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                student_id INTEGER NOT NULL,
                subject TEXT NOT NULL,
                grade INTEGER NOT NULL,
                FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE
            );
        )";
        
        return execute(sql);
    }
    
private:
    bool execute(const std::string& sql) {
        char* errMsg = nullptr;
        int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        
        if (result != SQLITE_OK) {
            std::cerr << "SQL ошибка: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
};

int main() {
    DatabaseManager db;
    
    if (!db.connect("university.db")) {
        return 1;
    }
    
    db.optimize();
    
    if (!db.createTables()) {
        std::cerr << "Ошибка создания таблиц" << std::endl;
        return 1;
    }
    
    std::cout << "База данных успешно инициализирована" << std::endl;
    return 0;
}