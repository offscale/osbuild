cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.1"
        DESCRIPTION "Installs compiler and build tools for your platform"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")

set(CMAKE_C_STANDARD 90)

set(src src/main.c src/posix.c src/errors.h)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND src src/linux_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND src src/win_build_deps)
endif()

add_executable(osbuild ${src})
