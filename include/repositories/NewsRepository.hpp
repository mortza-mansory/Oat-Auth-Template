#pragma once

#include "models/NewsDto.hpp"
#include "db/Database.hpp"
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace repositories {

class NewsRepository {
public:
    NewsRepository(std::shared_ptr<db::Database> db);
    bool createNews(const std::string& id, const std::string& title, const std::string& summary, const std::string& body, const std::string& url, const std::string& category, const std::vector<std::string>& people, const std::string& date, const std::string& lastmod);
    std::vector<std::shared_ptr<models::NewsDto>> getAllNews();
    std::map<std::string, std::string> getNewsById(const std::string& id);
    bool updateNews(const std::string& id, const std::string& title, const std::string& summary, const std::string& body, const std::string& url, const std::string& category, const std::vector<std::string>& people, const std::string& date, const std::string& lastmod);
    bool deleteNews(const std::string& id);
private:
    std::shared_ptr<db::Database> m_db;
};

}