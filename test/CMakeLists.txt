add_executable(test-message
    test-message.cpp
)

add_dependencies(test-message PING_MESSAGES)

target_link_libraries(
    test-message
)
