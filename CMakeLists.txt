cmake_minimum_required(VERSION 3.10)
project(oatpp_news_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OATPP_DIR "/usr/local/lib/cmake/oatpp-1.3.0" CACHE PATH "Path to Oat++ CMake directory")
list(APPEND CMAKE_PREFIX_PATH "/usr/local")

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-swagger REQUIRED)

add_definitions(-DOATPP_SWAGGER_RES_PATH=\"/opt/oatpp-swagger/res\")

add_executable(backend
  src/main.cpp
  src/controllers/AuthController.cpp
  src/controllers/NewsController.cpp
  src/controllers/PersonController.cpp
  src/db/Database.cpp
  src/repositories/NewsRepository.cpp
  src/repositories/UserRepository.cpp
  src/models/Models.cpp
      #  src/main.cpp
)

#target_include_directories(backend PRIVATE
#  include
#  ${oatpp_INCLUDE_DIRS}
#)
target_include_directories(backend PRIVATE
        include
        ${oatpp_INCLUDE_DIRS}
        ${SQLite3_INCLUDE_DIR}
        C:/Users/mopc/Documents/Lib/nlohmann/include
)

target_link_libraries(backend PRIVATE oatpp::oatpp oatpp::oatpp-test oatpp::oatpp-swagger)

find_library(SQLITE3_LIB sqlite3
        PATHS
        "C:/Users/mopc/Documents/Lib/sqlite3/dll"
        "C:/Libraries/sqlite3/dll"
        NO_DEFAULT_PATH
)

#find_library(SQLITE3_LIB sqlite3)
#if(SQLITE3_LIB)
#  target_link_libraries(backend PRIVATE ${SQLITE3_LIB})
#else()
#  message(FATAL_ERROR "SQLite3 library not found")
#endif()

message(STATUS "Using Oat++ from: ${oatpp_DIR}")
message(STATUS "Oat++ include dirs: ${oatpp_INCLUDE_DIRS}")
message(STATUS "Oat++ libraries: ${oatpp_LIBRARIES}")
message(STATUS "SQLite3 library: ${SQLITE3_LIB}")

install(TARGETS backend DESTINATION bin)
