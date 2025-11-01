#ifndef UserRepository_hpp
#define UserRepository_hpp

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "db/Database.hpp"
#include "models/Models.hpp"

namespace repositories {

class UserRepository {
public:
    UserRepository(std::shared_ptr<db::Database> db);
    bool createUser(const std::string& username, const std::string& password);
std::shared_ptr<models::UserDto> getUserByUsername(const std::string& username);
std::vector<std::shared_ptr<models::UserDto>> getAllUsers();
    bool updateUser(int id, const std::string& username, const std::string& password);
    bool deleteUser(int id);
private:
    std::shared_ptr<db::Database> m_db;
};

} // namespace repositories

#endif // UserRepository_hpp