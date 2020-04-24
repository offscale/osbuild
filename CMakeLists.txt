cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.2"
        DESCRIPTION "Installs compiler and build tools for your platform"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")
set(CMAKE_C_STANDARD 90)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(src src/main.c src/version)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND src src/win_build_deps)
else()
    list(APPEND src src/posix)
endif()

message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND src src/systemd src/linux_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    list(APPEND src src/freebsd_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
endif()

add_executable(osbuild ${src})
