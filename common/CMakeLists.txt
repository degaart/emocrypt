list(APPEND SRCS
    encrypt.cpp
    fd.cpp
    term_echo.cpp
    options.cpp
    utf8.cpp
)
add_library(common ${SRCS})
target_link_libraries(common PUBLIC project_options)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/..)

if(WIN32)
    target_link_libraries(common PUBLIC getopt)
endif()
