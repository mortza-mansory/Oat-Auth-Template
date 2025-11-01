#include "controllers/AuthController.hpp"
#include "controllers/NewsController.hpp"
#include "controllers/PersonController.hpp"
#include "repositories/UserRepository.hpp"
#include "repositories/NewsRepository.hpp"
#include "db/Database.hpp"

#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

#include <memory>
#include <string>

int main() {

    oatpp::base::Environment::init();

    // --- Network & HTTP setup ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::tcp::server::ConnectionProvider>, connectionProvider)([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpConnectionHandler>, connectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    // --- Global CORS interceptor ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::interceptor::AllowCorsGlobal>, corsInterceptor)([] {
        return std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>();
    }());

    // --- JSON object mapper ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper)([] {
        return oatpp::parser::json::mapping::ObjectMapper::createShared();
    }());

    // --- Database path ---
    OATPP_CREATE_COMPONENT(std::string, dbPath)([] {
        return std::string("/app/data/news.db");
    }());

    // --- Database and repositories ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<db::Database>, database)([] {
        OATPP_COMPONENT(std::string, dbPath);
        return std::make_shared<db::Database>(dbPath);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<repositories::UserRepository>, userRepository)([] {
        OATPP_COMPONENT(std::shared_ptr<db::Database>, database);
        return std::make_shared<repositories::UserRepository>(database);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<repositories::NewsRepository>, newsRepository)([] {
        OATPP_COMPONENT(std::shared_ptr<db::Database>, database);
        return std::make_shared<repositories::NewsRepository>(database);
    }());

    // --- Swagger resources and document info ---
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
        oatpp::swagger::DocumentInfo::Builder builder;
        builder
            .setTitle("Oat++ News Backend API")
            .setDescription("A REST API for managing users and news articles built with Oat++")
            .setVersion("1.0.0")
            .setContactName("Developer")
            .setContactUrl("https://example.com")
            .setLicenseName("MIT")
            .setLicenseUrl("https://opensource.org/licenses/MIT")
            .addServer("http://0.0.0.0:8080", "Local server");
        return builder.build();
    }());

    // --- Resolve components for app ---
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, routerPtr);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapperPtr);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::tcp::server::ConnectionProvider>, connectionProviderPtr);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpConnectionHandler>, connectionHandlerPtr);
    OATPP_COMPONENT(std::shared_ptr<repositories::UserRepository>, userRepoPtr);
    OATPP_COMPONENT(std::shared_ptr<repositories::NewsRepository>, newsRepoPtr);

    oatpp::web::server::api::Endpoints docEndpoints;

    // --- Controllers ---
    auto authController = controllers::AuthController::createShared(objectMapperPtr, userRepoPtr);
    routerPtr->addController(authController);
    docEndpoints.append(authController->getEndpoints());

    auto newsController = controllers::NewsController::createShared(objectMapperPtr, newsRepoPtr);
    routerPtr->addController(newsController);
    docEndpoints.append(newsController->getEndpoints());

    auto personController = controllers::PersonController::createShared(objectMapperPtr, userRepoPtr);
    routerPtr->addController(personController);
    docEndpoints.append(personController->getEndpoints());

    // --- Swagger Controller ---
    OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocInfo);
    OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerRes);
    auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints, swaggerDocInfo, swaggerRes);
    routerPtr->addController(swaggerController);

    // --- Start the server ---
    oatpp::network::Server server(connectionProviderPtr, connectionHandlerPtr);
    OATPP_LOGI("Server", "✅ Running on port %s ...", connectionProviderPtr->getProperty("port").getData());

    server.run();

    oatpp::base::Environment::destroy();
    return 0;
}
