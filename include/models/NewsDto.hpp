#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace models {

class NewsDto : public oatpp::DTO {
  DTO_INIT(NewsDto, DTO)

  DTO_FIELD(String, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, summary);
  DTO_FIELD(String, body);
  DTO_FIELD(String, url);
  DTO_FIELD(String, category);
  DTO_FIELD(Vector<String>, people);
  DTO_FIELD(String, date);
  DTO_FIELD(String, lastmod);
};

}

#include OATPP_CODEGEN_END(DTO)