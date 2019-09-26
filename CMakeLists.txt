cmake_minimum_required(VERSION 3.12)
project(ping-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# test-device
add_executable(test-device)
target_sources(
    test-device
PRIVATE
    test/test-device.cpp
)
target_link_libraries(
    test-device
PRIVATE
    DEVICE
    HAL
    PING_MESSAGES
)

# test-device-ping1d
add_executable(test-device-ping1d)
target_sources(
    test-device-ping1d
PRIVATE
    test/test-device-ping1d.cpp
)
target_link_libraries(
    test-device-ping1d
PRIVATE
    DEVICE
    HAL
    PING_MESSAGES
)

# test-message
add_executable(test-message)
target_sources(
    test-message
PRIVATE
    test/test-message.cpp
)
target_link_libraries(
    test-message
PRIVATE
    PING_MESSAGES
)
