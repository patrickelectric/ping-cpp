cmake_minimum_required(VERSION 2.8)
project(HAL)

add_library(
    HAL
    link/ping-port-linux.cpp
    link/desktop/abstractlink.cpp
    link/desktop/seriallink.cpp
    link/desktop/udplink.cpp
    time/ping-time-linux.cpp
)

target_include_directories(
    HAL
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(HAL)
