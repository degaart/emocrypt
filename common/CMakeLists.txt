add_library(common
    encrypt.cpp
    fd.cpp
    options.cpp
    term_echo.cpp
    utf8.cpp
)
target_link_libraries(common PUBLIC project_options)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)

