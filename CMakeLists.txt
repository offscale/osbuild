cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.1"
        DESCRIPTION "Installs compiler and build tools for your platform"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")

set(src src/main)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_C_STANDARD C11)
    list(APPEND src src/win_build_deps)
else()
    set(CMAKE_C_STANDARD 90)
    list(APPEND src src/posix)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND src src/linux_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
endif()

add_executable(osbuild ${src})
