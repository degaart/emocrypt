function(add_unit_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE project_options common)
    add_test(NAME ${name} COMMAND $<TARGET_FILE:${name}>)
endfunction()

add_unit_test(test_utf8 test_utf8.cpp)
add_unit_test(test_encrypt test_encrypt.cpp)


