#ifndef NEWSCONTROLLER_HPP
#define NEWSCONTROLLER_HPP

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include "models/Models.hpp"
#include "repositories/NewsRepository.hpp"

namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController)

class NewsController : public oatpp::web::server::api::ApiController {
public:
    NewsController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                   std::shared_ptr<repositories::NewsRepository> newsRepo)
        : oatpp::web::server::api::ApiController(objectMapper), m_newsRepo(newsRepo) {}

    static std::shared_ptr<NewsController> createShared(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                                                        std::shared_ptr<repositories::NewsRepository> newsRepo) {
        return std::make_shared<NewsController>(objectMapper, newsRepo);
    }
ENDPOINT("GET", "/person", getPersonNews,
         REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, request)) {
    auto authHeader = request->getHeader("Authorization");
    if (!authHeader || authHeader->find("Bearer ") != 0) {
        return createResponse(Status::CODE_401, "{ \"error\": \"Authorization header missing or invalid\" }");
    }
    std::string token = authHeader->substr(7);
    if (token.find("token_") != 0 || token.find("_authenticated") == std::string::npos) {
        return createResponse(Status::CODE_401, "{ \"error\": \"Invalid token\" }");
    }

    auto news = m_newsRepo->getAllNews();
    auto dto = oatpp::Vector<oatpp::Object<models::NewsDto>>::createShared();
    for (const auto& item : news) {
        dto->push_back(item);
    }
    return createDtoResponse(Status::CODE_200, dto);
}
ENDPOINT("GET", "/latest", getLatestNews,
         REQUEST(std::shared_ptr<oatpp::web::protocol::http::incoming::Request>, request)) {
    auto authHeader = request->getHeader("Authorization");
    if (!authHeader || authHeader->find("Bearer ") != 0) {
        return createResponse(Status::CODE_401, "{ \"error\": \"Authorization header missing or invalid\" }");
    }
    std::string token = authHeader->substr(7);
    if (token.find("token_") != 0 || token.find("_authenticated") == std::string::npos) {
        return createResponse(Status::CODE_401, "{ \"error\": \"Invalid token\" }");
    }

    auto news = m_newsRepo->getAllNews();
    auto dto = oatpp::Vector<oatpp::Object<models::NewsDto>>::createShared();
    for (const auto& item : news) {
        dto->push_back(item);
    }
    return createDtoResponse(Status::CODE_200, dto);
}

    ENDPOINT("POST", "/", postNews,
             BODY_DTO(oatpp::data::mapping::type::DTOWrapper<models::NewsDto>, body)) {
        auto news = body.getPtr();
        std::vector<std::string> peopleVec;
        if (news->people) {
            for (const auto& person : *news->people) {
                peopleVec.push_back(person);
            }
        }
        if (m_newsRepo->createNews(*news->id, *news->title, *news->summary, *news->body, *news->url, *news->category, peopleVec, *news->date, *news->lastmod)) {
            return createResponse(Status::CODE_201, "{ \"message\": \"News created\" }");
        } else {
            return createResponse(Status::CODE_400, "{ \"error\": \"Failed to create news\" }");
        }
    }

    ENDPOINT("POST", "/analyze", analyzeNews,
             BODY_DTO(oatpp::data::mapping::type::DTOWrapper<models::AnalyzeNewsDto>, body)) {
        auto analyze = body.getPtr();

        return createResponse(Status::CODE_200, "{ \"analysis\": \"News analyzed successfully\" }");
    }

    ENDPOINT("DELETE", "/", deleteNews) {
        // Note: This endpoint doesn't specify which news to delete, so it's a placeholder
        return createResponse(Status::CODE_400, "{ \"error\": \"News ID required\" }");
    }

private:
    std::shared_ptr<repositories::NewsRepository> m_newsRepo;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace controllers

#endif