#include <sqlite3.h>
#include <iostream>
#include <string>
#include <regex>

class Validator {
public:
    static bool isSafe(const std::string& input) {
        if (input.find("--") != std::string::npos ||
            input.find(";") != std::string::npos ||
            input.find("/*") != std::string::npos) {
            return false;
        }
        return input.length() <= 255;
    }
    
    static bool isValidEmail(const std::string& email) {
        std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
        return std::regex_match(email, pattern) && isSafe(email);
    }
    
    static bool isValidGrade(int grade) {
        return grade >= 0 && grade <= 100;
    }
};

class SecureRepo {
private:
    sqlite3* db;
    
public:
    SecureRepo(sqlite3* database) : db(database) {}
    
    bool addStudent(const std::string& name, const std::string& email, const std::string& group) {
        if (!Validator::isSafe(name) || !Validator::isValidEmail(email) || !Validator::isSafe(group)) {
            std::cerr << "Ошибка валидации" << std::endl;
            return false;
        }
        
        const std::string sql = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }
        
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, group.c_str(), -1, SQLITE_TRANSIENT);
        
        bool ok = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);
        return ok;
    }
    
    bool addGrade(int student_id, const std::string& subject, int grade) {
        if (!Validator::isSafe(subject) || !Validator::isValidGrade(grade)) {
            return false;
        }
        
        const std::string sql = "INSERT INTO grades (student_id, subject, grade) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }
        
        sqlite3_bind_int(stmt, 1, student_id);
        sqlite3_bind_text(stmt, 2, subject.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, grade);
        
        bool ok = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);
        return ok;
    }
};

int main() {
    sqlite3* db;
    sqlite3_open("university.db", &db);
    
    SecureRepo repo(db);
    
    // Тесты
    std::cout << "1. Корректный студент: ";
    std::cout << (repo.addStudent("Иван", "ivan@mail.ru", "CS-101") ? "OK" : "FAIL") << std::endl;
    
    std::cout << "2. Некорректный email: ";
    std::cout << (repo.addStudent("Иван", "invalid-email", "CS-101") ? "FAIL" : "OK") << std::endl;
    
    std::cout << "3. SQL-инъекция в имени: ";
    std::cout << (repo.addStudent("Иван'; DROP TABLE students;--", "test@mail.ru", "CS-101") ? "FAIL" : "OK") << std::endl;
    
    std::cout << "4. Корректная оценка: ";
    std::cout << (repo.addGrade(1, "Math", 85) ? "OK" : "FAIL") << std::endl;
    
    std::cout << "5. Некорректная оценка: ";
    std::cout << (repo.addGrade(1, "Math", 150) ? "FAIL" : "OK") << std::endl;
    
    sqlite3_close(db);
    return 0;
}