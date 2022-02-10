cmake_minimum_required(VERSION 3.15)
project(emocrypt)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TESTING "Enable unit tests" OFF)

find_package(libsodium REQUIRED)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_14)
target_link_libraries(project_options INTERFACE libsodium::libsodium)

if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
endif()

add_subdirectory(common)
add_subdirectory(util)
add_subdirectory(cli)

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

