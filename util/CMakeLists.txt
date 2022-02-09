add_executable(generate_data
    generate_data.cpp
)
target_link_libraries(generate_data PRIVATE
    project_options
    common
)
