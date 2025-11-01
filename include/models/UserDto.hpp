#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace models {

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(String, id);
  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
  DTO_FIELD(String, role);
};

class RegisterDto : public oatpp::DTO {
  DTO_INIT(RegisterDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
};

class LoginDto : public oatpp::DTO {
  DTO_INIT(LoginDto, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, password);
};

}

#include OATPP_CODEGEN_END(DTO)