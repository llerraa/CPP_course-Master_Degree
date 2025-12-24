#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>

struct Grade {
    std::string subject;
    int grade;
};

class StudentRepo {
private:
    sqlite3* db;
    
    bool execSQL(const std::string& sql) {
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }
    
public:
    StudentRepo(sqlite3* database) : db(database) {}
    
    bool addStudentWithGrades(const std::string& name, const std::string& email, 
                             const std::string& group, const std::vector<Grade>& grades) {
        // Начало транзакции
        if (!execSQL("BEGIN TRANSACTION;")) return false;
        
        try {
            // Добавляем студента
            const std::string sql = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
            sqlite3_stmt* stmt;
            
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error("Ошибка подготовки запроса");
            }
            
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, group.c_str(), -1, SQLITE_TRANSIENT);
            
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                throw std::runtime_error("Ошибка добавления студента");
            }
            
            int student_id = sqlite3_last_insert_rowid(db);
            sqlite3_finalize(stmt);
            
            // Добавляем оценки
            for (const auto& grade : grades) {
                const std::string gradeSQL = "INSERT INTO grades (student_id, subject, grade) VALUES (?, ?, ?)";
                if (sqlite3_prepare_v2(db, gradeSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
                    throw std::runtime_error("Ошибка подготовки запроса оценки");
                }
                
                sqlite3_bind_int(stmt, 1, student_id);
                sqlite3_bind_text(stmt, 2, grade.subject.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, grade.grade);
                
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    sqlite3_finalize(stmt);
                    throw std::runtime_error("Ошибка добавления оценки");
                }
                sqlite3_finalize(stmt);
            }
            
            // Фиксация
            if (!execSQL("COMMIT;")) {
                throw std::runtime_error("Ошибка коммита");
            }
            return true;
            
        } catch (const std::exception& e) {
            execSQL("ROLLBACK;");
            std::cerr << "Ошибка транзакции: " << e.what() << std::endl;
            return false;
        }
    }
    
    std::vector<std::string> getStudentsByGroup(const std::string& group_name) {
        std::vector<std::string> students;
        const std::string sql = "SELECT name FROM students WHERE group_name = ?";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return students;
        }
        
        sqlite3_bind_text(stmt, 1, group_name.c_str(), -1, SQLITE_TRANSIENT);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            students.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
        
        sqlite3_finalize(stmt);
        return students;
    }
    
    double getAverageGradeBySubject(const std::string& subject) {
        const std::string sql = "SELECT AVG(grade) FROM grades WHERE subject = ?";
        sqlite3_stmt* stmt;
        double avg = 0.0;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return avg;
        }
        
        sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_TRANSIENT);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            avg = sqlite3_column_double(stmt, 0);
        }
        
        sqlite3_finalize(stmt);
        return avg;
    }
    
    std::vector<std::pair<std::string, double>> getTopStudents(int limit) {
        std::vector<std::pair<std::string, double>> top;
        const std::string sql = R"(
            SELECT s.name, AVG(g.grade) as avg_grade 
            FROM students s 
            JOIN grades g ON s.id = g.student_id 
            GROUP BY s.id 
            ORDER BY avg_grade DESC 
            LIMIT ?
        )";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return top;
        }
        
        sqlite3_bind_int(stmt, 1, limit);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            top.emplace_back(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                sqlite3_column_double(stmt, 1)
            );
        }
        
        sqlite3_finalize(stmt);
        return top;
    }
};

int main() {
    sqlite3* db;
    sqlite3_open("university.db", &db);
    
    StudentRepo repo(db);
    
    // Тест транзакции
    std::vector<Grade> grades = {{"Math", 85}, {"Physics", 90}};
    if (repo.addStudentWithGrades("Петр Петров", "petr@university.ru", "CS-101", grades)) {
        std::cout << "✓ Студент с оценками добавлен" << std::endl;
    }
    
    // Тест статистики
    auto cs101 = repo.getStudentsByGroup("CS-101");
    std::cout << "✓ Студентов в CS-101: " << cs101.size() << std::endl;
    
    double avgMath = repo.getAverageGradeBySubject("Math");
    std::cout << "✓ Средняя по Math: " << avgMath << std::endl;
    
    auto top = repo.getTopStudents(3);
    std::cout << "✓ Топ 3 студента:" << std::endl;
    for (const auto& [name, grade] : top) {
        std::cout << "  " << name << ": " << grade << std::endl;
    }
    
    sqlite3_close(db);
    return 0;
}