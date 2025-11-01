#include "repositories/UserRepository.hpp"
#include <sstream>

namespace repositories {

UserRepository::UserRepository(std::shared_ptr<db::Database> db) : m_db(db) {}

bool UserRepository::createUser(const std::string& username, const std::string& password) {
    std::stringstream ss;
    ss << "INSERT INTO users (username, password) VALUES ('" << username << "', '" << password << "');";
    return m_db->execute(ss.str());
}

std::shared_ptr<models::UserDto> UserRepository::getUserByUsername(const std::string& username) {
    auto result = m_db->query("SELECT id, username, password FROM users WHERE username = '" + username + "';");
    if (result.empty()) {
        return nullptr;
    }
    auto row = result[0];
    auto dto = models::UserDto::createShared();
    dto->username = row.at("username");
    dto->password = row.at("password");
    return dto.getPtr();
}

std::vector<std::shared_ptr<models::UserDto>> UserRepository::getAllUsers() {
    auto results = m_db->query("SELECT id, username, password FROM users;");
    std::vector<std::shared_ptr<models::UserDto>> users;
    for (const auto& row : results) {
        auto dto = models::UserDto::createShared();
        dto->username = row.at("username");
        dto->password = row.at("password");
        users.push_back(dto.getPtr());
    }
    return users;
}
bool UserRepository::updateUser(int id, const std::string& username, const std::string& password) {
    std::stringstream ss;
    ss << "UPDATE users SET username = '" << username << "', password = '" << password << "' WHERE id = " << id << ";";
    return m_db->execute(ss.str());
}

bool UserRepository::deleteUser(int id) {
    std::stringstream ss;
    ss << "DELETE FROM users WHERE id = " << id << ";";
    return m_db->execute(ss.str());
}

} // namespace repositories