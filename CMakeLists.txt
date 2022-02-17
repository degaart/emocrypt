cmake_minimum_required(VERSION 3.15)
project(emocrypt)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EMSCRIPTEN OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(EMSCRIPTEN ON)
endif()

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TESTING "Enable unit tests" OFF)

find_package(libsodium REQUIRED)
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_14)
target_link_libraries(project_options INTERFACE libsodium::libsodium)

if(WIN32)
    target_compile_definitions(project_options INTERFACE SODIUM_STATIC)
endif()

if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(project_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
endif()

add_subdirectory(third-party)
add_subdirectory(common)

if(EMSCRIPTEN)
    add_subdirectory(emscripten)
else()
    add_subdirectory(cli)
    add_subdirectory(util)
endif()

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

