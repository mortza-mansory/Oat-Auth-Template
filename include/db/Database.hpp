#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>
#include <map>
#include <sqlite3.h>

namespace db {

class Database {
public:
    Database(const std::string& path);
    ~Database();
    bool execute(const std::string& query);
    std::vector<std::map<std::string, std::string>> query(const std::string& sql);
private:
    void init();
    std::string m_path;
    sqlite3* m_db;
};

} // namespace db

#endif