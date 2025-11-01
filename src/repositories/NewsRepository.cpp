#include "repositories/NewsRepository.hpp"
#include <sstream>
#include <map>
#include <nlohmann/json.hpp>
#include <iostream>

namespace repositories {

NewsRepository::NewsRepository(std::shared_ptr<db::Database> db) : m_db(db) {}

bool NewsRepository::createNews(const std::string& id, const std::string& title, const std::string& summary, const std::string& body, const std::string& url, const std::string& category, const std::vector<std::string>& people, const std::string& date, const std::string& lastmod) {
    nlohmann::json peopleJson = people;
    std::string peopleStr = peopleJson.dump();

    auto escapeSql = [](std::string str) {
        size_t pos = 0;
        while ((pos = str.find("'", pos)) != std::string::npos) {
            str.replace(pos, 1, "''");
            pos += 2;
        }
        return str;
    };

    std::stringstream ss;
    ss << "INSERT INTO news (id, title, summary, body, url, category, people, date, lastmod) VALUES ('"
       << escapeSql(id) << "', '" << escapeSql(title) << "', '" << escapeSql(summary) << "', '"
       << escapeSql(body) << "', '" << escapeSql(url) << "', '" << escapeSql(category) << "', '"
       << escapeSql(peopleStr) << "', '" << escapeSql(date) << "', '" << escapeSql(lastmod) << "');";

    std::string query = ss.str();
    std::cout << "Executing query: " << query << std::endl;

    return m_db->execute(query);
}

std::vector<std::shared_ptr<models::NewsDto>> NewsRepository::getAllNews() {
    auto results = m_db->query("SELECT id, title, summary, body, url, category, people, date, lastmod FROM news;");
    std::vector<std::shared_ptr<models::NewsDto>> newsList;
    for (const auto& row : results) {
        auto dto = models::NewsDto::createShared();
        dto->id = row.at("id");
        dto->title = row.at("title");
        dto->summary = row.at("summary");
        dto->body = row.at("body");
        dto->url = row.at("url");
        dto->category = row.at("category");
        try {
            nlohmann::json peopleJson = nlohmann::json::parse(row.at("people"));
            dto->people = oatpp::Vector<oatpp::String>::createShared();
            for (const auto& person : peopleJson) {
                dto->people->push_back(person.get<std::string>());
            }
        } catch (const std::exception& e) {
            dto->people = oatpp::Vector<oatpp::String>::createShared();
        }
        dto->date = row.at("date");
        dto->lastmod = row.at("lastmod");
        newsList.push_back(dto.getPtr());
    }
    return newsList;
}

std::map<std::string, std::string> NewsRepository::getNewsById(const std::string& id) {
    std::stringstream ss;
    ss << "SELECT * FROM news WHERE id = '" << id << "' LIMIT 1;";
    auto results = m_db->query(ss.str());
    return results.empty() ? std::map<std::string, std::string>() : results[0];
}

bool NewsRepository::updateNews(const std::string& id, const std::string& title, const std::string& summary, const std::string& body, const std::string& url, const std::string& category, const std::vector<std::string>& people, const std::string& date, const std::string& lastmod) {
    nlohmann::json peopleJson = people;
    std::stringstream ss;
    ss << "UPDATE news SET title = '" << title << "', summary = '" << summary << "', body = '" << body << "', url = '" << url << "', category = '" << category << "', people = '" << peopleJson.dump() << "', date = '" << date << "', lastmod = '" << lastmod << "' WHERE id = '" << id << "';";
    return m_db->execute(ss.str());
}

bool NewsRepository::deleteNews(const std::string& id) {
    std::stringstream ss;
    ss << "DELETE FROM news WHERE id = '" << id << "';";
    return m_db->execute(ss.str());
}

} // namespace repositories