#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Student {
    int id;
    std::string name;
    std::string email;
    std::string group_name;
};

class StudentRepository {
private:
    sqlite3* db;
    
public:
    StudentRepository(sqlite3* database) : db(database) {}
    
    bool addStudent(const std::string& name, const std::string& email, const std::string& group) {
        const std::string sql = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, group.c_str(), -1, SQLITE_TRANSIENT);
        
        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        
        if (!success) {
            std::cerr << "Ошибка добавления студента: " << sqlite3_errmsg(db) << std::endl;
        }
        
        sqlite3_finalize(stmt);
        return success;
    }
    
    std::unique_ptr<Student> getStudent(int id) {
        const std::string sql = "SELECT id, name, email, group_name FROM students WHERE id = ?";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return nullptr;
        }
        
        sqlite3_bind_int(stmt, 1, id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            auto student = std::make_unique<Student>();
            student->id = sqlite3_column_int(stmt, 0);
            student->name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            student->email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            student->group_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            sqlite3_finalize(stmt);
            return student;
        }
        
        sqlite3_finalize(stmt);
        return nullptr;
    }
    
    bool updateStudent(int id, const std::string& newName, const std::string& newEmail, const std::string& newGroup) {
        const std::string sql = "UPDATE students SET name = ?, email = ?, group_name = ? WHERE id = ?";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }
        
        sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, newEmail.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, newGroup.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, id);
        
        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);
        return success;
    }
    
    bool deleteStudent(int id) {
        const std::string sql = "DELETE FROM students WHERE id = ?";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }
        
        sqlite3_bind_int(stmt, 1, id);
        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);
        return success;
    }
    
    std::vector<Student> getAllStudents() {
        const std::string sql = "SELECT id, name, email, group_name FROM students ORDER BY id";
        std::vector<Student> students;
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return students;
        }
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student student;
            student.id = sqlite3_column_int(stmt, 0);
            student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            student.group_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            students.push_back(student);
        }
        
        sqlite3_finalize(stmt);
        return students;
    }
};

// Тестирование
int main() {
    sqlite3* db;
    if (sqlite3_open("university.db", &db) != SQLITE_OK) {
        std::cerr << "Не удалось открыть базу данных" << std::endl;
        return 1;
    }
    
    StudentRepository repo(db);
    
    // Добавление
    if (repo.addStudent("Иван Иванов", "ivan@university.ru", "CS-101")) {
        std::cout << "Студент добавлен" << std::endl;
    }
    
    // Получение
    auto student = repo.getStudent(1);
    if (student) {
        std::cout << "Найден студент: " << student->name << std::endl;
    }
    
    // Обновление
    if (repo.updateStudent(1, "Иван Петров", "ivan@newmail.ru", "CS-102")) {
        std::cout << "Студент обновлен" << std::endl;
    }
    
    // Получение всех
    auto allStudents = repo.getAllStudents();
    std::cout << "Всего студентов: " << allStudents.size() << std::endl;
    
    // Проверка уникальности email (должно не сработать)
    if (!repo.addStudent("Другой", "ivan@newmail.ru", "CS-101")) {
        std::cout << "Проверка уникальности email работает" << std::endl;
    }
    
    sqlite3_close(db);
    return 0;
}