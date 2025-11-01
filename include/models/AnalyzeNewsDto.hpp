#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace models {

class AnalyzeNewsDto : public oatpp::DTO {
  DTO_INIT(AnalyzeNewsDto, DTO)

  DTO_FIELD(String, url);
  DTO_FIELD(String, language);
};

}

#include OATPP_CODEGEN_END(DTO)