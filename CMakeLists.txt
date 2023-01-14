cmake_minimum_required(VERSION 3.0.0)
project(PcJsApi VERSION 0.1.0)

include(CTest)
enable_testing()



add_executable(PcJsApi src/main.cpp)
# add_subdirectory($<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include)
include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
