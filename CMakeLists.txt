cmake_minimum_required(VERSION 3.10)
project(bitup VERSION 1.0)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/release_package)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(BUILD_DEBUG OFF)
set(BUILD_TEST  ON)

set(MAKE_COMMAND mingw32-make)

if(${BUILD_DEBUG})

    set(CMAKE_BUILD_TYPE "Debug")
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/debug)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/debug)

else()

    set(CMAKE_BUILD_TYPE "Release")
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)

endif()

find_package(OpenSSL REQUIRED)
if(NOT OpenSSL_FOUND)
    message(FATAL "OpenSSL Not Found")
endif()
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Python3 REQUIRED
             COMPONENTS Interpreter Development)
if(NOT Python3_FOUND)
    message(FATAL "Python3 Not Found")
endif()
include_directories(${Python3_INCLUDE_DIRS})

find_package(Boost REQUIRED
             COMPONENTS system filesystem thread)
if(NOT Boost_FOUND)
    message(FATAL "Boost Not Found")
endif()
include_directories(${BOOST_INCLUDE_DIRS})

include_directories(include)
add_subdirectory(src)

add_library(renaini SHARED
    ${RENAINI_SRC}
)

add_library(formatext SHARED
    ${FORMATEXT_SRC}
)

add_library(shellext SHARED
    ${SHELLEXT_SRC}
)

add_library(renalog SHARED
    ${RENALOG_SRC}
)

add_library(fdiropext SHARED
    ${FILE_OPERATION_SRC}
)

add_library(hash4cxx SHARED
    ${HASH_SRC}
)

add_library(pyapi SHARED
    ${PY_EXT_SRC}
)

add_executable(bitup_shell
    ${SHELL_MAIN}
)

add_custom_command(
    TARGET      pyapi
    POST_BUILD
    COMMAND     ${MAKE_COMMAND} -C ${PROJECT_SOURCE_DIR}/py
)

target_link_libraries(fdiropext
    Boost::filesystem
)

target_link_libraries(bitup_shell
    shellext
    formatext
)

target_link_libraries(pyapi
    ${Python3_LIBRARIES}
)

if(${BUILD_TEST})

    add_executable(test
        ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    )

    target_link_libraries(test
        fdiropext
        renalog
        hash4cxx
        pyapi
        OpenSSL::Crypto
        OpenSSL::SSL
    )

endif()

if(NOT ${BUILD_DEBUG})

    install(
        FILES
            ${CMAKE_SOURCE_DIR}/release/bitup_shell.exe
            ${CMAKE_SOURCE_DIR}/release/libfdiropext.dll
            ${CMAKE_SOURCE_DIR}/release/libformatext.dll
            ${CMAKE_SOURCE_DIR}/release/libhash4cxx.dll
            ${CMAKE_SOURCE_DIR}/release/libpyapi.dll
            ${CMAKE_SOURCE_DIR}/release/librenaini.dll
            ${CMAKE_SOURCE_DIR}/release/librenalog.dll
            ${CMAKE_SOURCE_DIR}/release/libshellext.dll
            ${CMAKE_SOURCE_DIR}/lib/libcrypto-1_1-x64.dll
            ${CMAKE_SOURCE_DIR}/lib/libssl-1_1-x64.dll
            ${CMAKE_SOURCE_DIR}/lib/python310.dll
        DESTINATION
            bin
    )

    install(
        FILES
            ${CMAKE_SOURCE_DIR}/py/gui.pyc
            ${CMAKE_SOURCE_DIR}/py/hashfile.pyc
        DESTINATION
            py
    )

    install(
        FILES
            ${CMAKE_SOURCE_DIR}/LICENSE
            ${CMAKE_SOURCE_DIR}/Privacy_Policy.txt
            ${CMAKE_SOURCE_DIR}/README.md
        DESTINATION
            .
    )

endif()