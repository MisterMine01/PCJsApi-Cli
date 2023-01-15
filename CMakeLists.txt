cmake_minimum_required(VERSION 3.0.0)
project(PcJsApi VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIR})
add_subdirectory(${PROJECT_SOURCE_DIR}/include/nlohmann)


find_package(CURL REQUIRED) 

add_executable(${PROJECT_NAME} src/main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src/**)

file(
     GLOB_RECURSE source_list RELATIVE
     "${CMAKE_CURRENT_SOURCE_DIR}"
     *.c *.h
    )
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()  
message(STATUS "Tree reorganized")


target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
