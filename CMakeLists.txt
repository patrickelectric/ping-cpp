cmake_minimum_required(VERSION 3.12)
project(ping-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

# Build messages
include(FindPythonInterp)
if (NOT PYTHONINTERP_FOUND)
    message(STATUS "Could not find Python.")
    message(ERROR "Failed to generate messages!")
endif()

add_custom_target(PING_MESSAGES
    COMMAND
    ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/generate/generate-message.py
        --output-dir ${PROJECT_SOURCE_DIR}/src/message
)

# Add warning checks
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror")
endif()

include_directories(src)

add_subdirectory(test)
