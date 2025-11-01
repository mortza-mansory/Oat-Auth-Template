#ifndef PERSONCONTROLLER_HPP
#define PERSONCONTROLLER_HPP

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include "models/Models.hpp"
#include "repositories/UserRepository.hpp"

namespace controllers {

#include OATPP_CODEGEN_BEGIN(ApiController)

class PersonController : public oatpp::web::server::api::ApiController {
public:
    PersonController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                     std::shared_ptr<repositories::UserRepository> userRepo)
        : oatpp::web::server::api::ApiController(objectMapper), m_userRepo(userRepo) {}

    static std::shared_ptr<PersonController> createShared(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                                                          std::shared_ptr<repositories::UserRepository> userRepo) {
        return std::make_shared<PersonController>(objectMapper, userRepo);
    }

ENDPOINT("GET", "/users", listPersons) {
    auto users = m_userRepo->getAllUsers();
    return createResponse(Status::CODE_200, "Users list");
}
ENDPOINT("GET", "/", listPersonsOld) {
    auto users = m_userRepo->getAllUsers();
    return createResponse(Status::CODE_200, "Users list");
}

ENDPOINT("GET", "/users/{id}", getPerson, PATH(String, id)) {
    auto user = m_userRepo->getUserByUsername(*id);
    if (user) {
        return createResponse(Status::CODE_200, "User found");
    } else {
        return createResponse(Status::CODE_404, "{ \"error\": \"Person not found\" }");
    }
}
ENDPOINT("PUT", "/users/{id}", updatePerson,
         PATH(String, id),
         BODY_DTO(oatpp::data::mapping::type::DTOWrapper<models::UserDto>, body)) {
    auto user = body.getPtr();
    int userId = std::stoi(*id);
    if (m_userRepo->updateUser(userId, *user->username, *user->password)) {
        return createResponse(Status::CODE_200, "{ \"message\": \"Person updated\" }");
    } else {
        return createResponse(Status::CODE_400, "{ \"error\": \"Failed to update person\" }");
    }
}

ENDPOINT("DELETE", "/users/{id}", deletePerson,
         PATH(String, id)) {
    int userId = std::stoi(*id);
    if (m_userRepo->deleteUser(userId)) {
        return createResponse(Status::CODE_200, "{ \"message\": \"Person deleted\" }");
    } else {
        return createResponse(Status::CODE_400, "{ \"error\": \"Failed to delete person\" }");
    }
}
    ENDPOINT("GET", "/performance", getPersonPerformance) {
        return createResponse(Status::CODE_200, "{ \"performance\": \"Good\" }");
    }

private:
    std::shared_ptr<repositories::UserRepository> m_userRepo;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace controllers

#endif