set(RENAINI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/renaini.cpp
PARENT_SCOPE)

set(FORMATEXT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/formatext.cpp
PARENT_SCOPE)

set(SHELL_MAIN
    ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell.cpp
PARENT_SCOPE)

set(SHELLEXT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/shell/commandlist.cpp
PARENT_SCOPE)

set(RENALOG_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/renalog_core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renalog_read.cpp
PARENT_SCOPE)

set(FILE_OPERATION_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/dirop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fop.cpp
PARENT_SCOPE)

set(HASH_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/md5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/sha1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/sha224.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/sha256.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/sha384.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hash/sha512.cpp
PARENT_SCOPE)

set(PY_EXT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/PyAPI_Encapsulation.cpp
PARENT_SCOPE)