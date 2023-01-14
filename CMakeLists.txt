cmake_minimum_required(VERSION 3.0.0)
project(PcJsApi VERSION 0.1.0)

include(CTest)
enable_testing()


include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/**
)

include_directories(${CURL_INCLUDE_DIR})

find_package(CURL REQUIRED) 

add_subdirectory(${PROJECT_SOURCE_DIR}/include/nlohmann)

add_executable(PcJsApi src/main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src/**)
# add_subdirectory($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)


target_link_libraries(PcJsApi PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(PcJsApi PRIVATE ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
