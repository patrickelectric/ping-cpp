cmake_minimum_required(VERSION 3.12)
project(ping-cpp LANGUAGES CXX)

# Use standard C++ version and remove extensions to allow better compatibility
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Default.cmake)
include(cmake/CheckConan.cmake)

conan_cmake_run(
    REQUIRES
        boost_asio/1.69.0@bincrafters/stable
        boost_atomic/1.69.0@bincrafters/stable
        boost_filesystem/1.69.0@bincrafters/stable
        boost_program_options/1.69.0@bincrafters/stable
        boost_system/1.69.0@bincrafters/stable
        boost_thread/1.69.0@bincrafters/stable
    BASIC_SETUP
    BUILD missing
)

# Enable compile checks for msvc, clang and gcc
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
  # Can be removed moving to C++17, or we could move this library out from header only
  add_compile_options(-Wno-unused-function)
endif()

# Used for the command line interface
find_package(Boost 1.67 COMPONENTS filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Include modules
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add fmt
include_directories(lib/fmt/include/)
add_subdirectory(lib/fmt/)

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
    ${Boost_LIBRARIES}
    fmt::fmt
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
    ${Boost_LIBRARIES}
    fmt::fmt
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
