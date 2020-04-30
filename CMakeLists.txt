cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.5"
        DESCRIPTION "Installs compiler and build tools for your platform"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")
set(CMAKE_C_STANDARD 90)
set(CMAKE_VERBOSE_MAKEFILE ON)

# configure_file(src/version.h.in src/version.h)

set(src src/main.c src/docopt src/distribution)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND src src/win_build_deps)
else()
    list(APPEND src src/posix)
endif()

message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")
message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND src src/systemd src/linux_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    list(APPEND src src/freebsd_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "NetBSD")
    list(APPEND src src/netbsd_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "SunOS")
    list(APPEND src src/solaris_build_deps.c)
endif()

add_executable(osbuild ${src})
target_include_directories(osbuild PUBLIC "${PROJECT_BINARY_DIR}")
