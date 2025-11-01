#include "db/Database.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

namespace db {

Database::Database(const std::string& path) : m_path(path), m_db(nullptr) {
    if (sqlite3_open(m_path.c_str(), &m_db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }
    init();
}

Database::~Database() {
    if (m_db) {
        sqlite3_close(m_db);
    }
}

void Database::init() {
    const char* users_sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT);";
    execute(users_sql);
    const char* news_sql = R"(
        CREATE TABLE IF NOT EXISTS news (
            id TEXT PRIMARY KEY,
            title TEXT NOT NULL,
            summary TEXT,
            body TEXT,
            url TEXT,
            category TEXT,
            people TEXT,
            date TEXT,
            lastmod TEXT
        );
    )";
    execute(news_sql);
}

bool Database::execute(const std::string& query) {
    char* err = nullptr;
    if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::string error = err ? err : "Unknown error";
        std::cout << "SQL Error: " << error << std::endl;
        std::cout << "Query: " << query << std::endl;
        sqlite3_free(err);
        return false;
    }
    return true;
}

std::vector<std::map<std::string, std::string>> Database::query(const std::string& sql) {
    std::vector<std::map<std::string, std::string>> results;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return results;
    }
    int cols = sqlite3_column_count(stmt);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int i = 0; i < cols; ++i) {
            const char* col_name = sqlite3_column_name(stmt, i);
            const unsigned char* col_value = sqlite3_column_text(stmt, i);
            row[col_name] = col_value ? reinterpret_cast<const char*>(col_value) : "";
        }
        results.push_back(row);
    }
    sqlite3_finalize(stmt);
    return results;
}

} // namespace db