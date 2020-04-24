cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.1"
        DESCRIPTION "Installs compiler and build tools for your platform"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")

add_compile_definitions(SVN_ERR__TRACING)
#include_directories(/usr/local/apr/lib)
include_directories(/usr/local/apr/include/apr-2)
#include_directories(/Volumes/WD/repos/apr/include)
include_directories(src/sys)
include_directories(src/sys/private)

find_library(APR_LIBRARY apr-1 PATHS /usr/local/apr/lib/)

set(src src/main
        src/sys/sysinfo.h
        src/sys/svn_error.h
        src/sys/svn_io.h
        src/sys/svn_string.h
        src/sys/ctype.c
        src/sys/svn_ctype.h
        src/sys/string.c
        src/linux_version.h
        src/version.h)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_C_STANDARD C11)
    list(APPEND src src/win_build_deps)
else()
    set(CMAKE_C_STANDARD 90)
    list(APPEND src src/posix)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND src src/linux_build_deps)
    #message("-- CMAKE_SYSTEM_INFO_FILE: ${CMAKE_SYSTEM_INFO_FILE)
    #message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")
    #message("-- CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCES#SOR}")
    message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

    string (REGEX MATCH "\\.el[1-9]" os_version_suffix ${CMAKE_SYSTEM})
    message("-- os_version_suffix:      ${os_version_suffix}")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    list(APPEND src src/freebsd_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
endif()

add_executable(osbuild ${src})

target_link_libraries(osbuild ${APR_LIBRARY})
