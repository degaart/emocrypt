add_executable(emocrypt
    emocrypt.cpp
)
target_link_libraries(emocrypt PRIVATE
    common
    libsodium::libsodium
)
target_link_options(emocrypt PRIVATE
    --bind -s ALLOW_MEMORY_GROWTH=1
)
