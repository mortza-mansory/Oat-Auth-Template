#ifndef AUTHCONTROLLER_HPP
#define AUTHCONTROLLER_HPP

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include "models/Models.hpp"
#include "repositories/UserRepository.hpp"

namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController)

class AuthController : public oatpp::web::server::api::ApiController {
public:
    AuthController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                   std::shared_ptr<repositories::UserRepository> userRepo)
        : oatpp::web::server::api::ApiController(objectMapper), m_userRepo(userRepo) {}

    static std::shared_ptr<AuthController> createShared(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                                                        std::shared_ptr<repositories::UserRepository> userRepo) {
        return std::make_shared<AuthController>(objectMapper, userRepo);
    }

    ENDPOINT("POST", "/register", postRegister,
                  BODY_DTO(oatpp::data::mapping::type::DTOWrapper<models::RegisterDto>, body)) {
        auto user = body.getPtr();
        if (m_userRepo->createUser(*user->username, *user->password)) {
            return createResponse(Status::CODE_201, "{ \"message\": \"User registered successfully\" }");
        } else {
            return createResponse(Status::CODE_400, "{ \"error\": \"Registration failed\" }");
        }
    }

ENDPOINT("POST", "/login", postLogin,
          BODY_DTO(oatpp::data::mapping::type::DTOWrapper<models::LoginDto>, body)) {
    auto user = body.getPtr();
    auto dbUser = m_userRepo->getUserByUsername(*user->username);
    if (dbUser && *dbUser->password == *user->password) {
        std::string token = "token_" + std::string(*user->username) + "_authenticated";
        return createResponse(Status::CODE_200, "{ \"access_token\": \"" + token + "\", \"token_type\": \"bearer\" }");
    } else {
        return createResponse(Status::CODE_401, "{ \"error\": \"Invalid credentials\" }");
    }
}

private:
    std::shared_ptr<repositories::UserRepository> m_userRepo;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace controllers

#endif