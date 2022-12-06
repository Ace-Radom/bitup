cmake_minimum_required(VERSION 3.10)
project(bitup VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(BUILD_DEBUG ON)

if(${BUILD_DEBUG})

set(CMAKE_BUILD_TYPE "Debug")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/debug)

else()

set(CMAKE_BUILD_TYPE "Release")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)

endif()

include_directories(include)
add_subdirectory(src)

add_library(renaini SHARED
    ${INI_SRC}
)

add_library(formatext SHARED
    ${FORMATEXT_SRC}
)

add_library(shellext SHARED
    ${SHELLEXT_SRC}
)

add_executable(bitup_shell
    ${SHELL_MAIN}
)

target_link_libraries(bitup_shell
    shellext
    formatext
)