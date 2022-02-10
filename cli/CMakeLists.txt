set(PROGRAM emocrypt)
add_executable(${PROGRAM}
    main.cpp
)
target_link_libraries(${PROGRAM} PRIVATE
    project_options
    common
)

