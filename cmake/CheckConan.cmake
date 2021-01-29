# Download automatically, you can also just copy the conan.cmake file
set(CONAN_CMAKE_PATH "${PROJECT_SOURCE_DIR}/cmake/conan.cmake")
if(NOT EXISTS ${CONAN_CMAKE_PATH})
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(
       DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
       ${CONAN_CMAKE_PATH}
    )
endif()
include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)