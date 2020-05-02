cmake_minimum_required(VERSION 3.0)
project(osbuild
        VERSION "0.0.6"
        LANGUAGES C)
set(HOMEPAGE_URL "https://github.com/offscale/osbuild")
set(DESCRIPTION "Installs compiler and build tools for your platform")
set(CMAKE_C_STANDARD 90)
set(CMAKE_VERBOSE_MAKEFILE ON)

configure_file(src/version.h.in ${PROJECT_BINARY_DIR}/src/version.h)

set(src src/main.c src/docopt src/distribution)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND src src/win_build_deps)
else()
    list(APPEND src src/posix)
endif()

# message("-- CMAKE_SYSTEM:           ${CMAKE_SYSTEM}")
# message("-- CMAKE_SYSTEM_NAME:      ${CMAKE_SYSTEM_NAME}")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND src src/systemd src/linux_build_deps)
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    list(APPEND src src/freebsd_build_deps)
elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    list(APPEND src src/netbsd_build_deps)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND src src/macos_build_deps)
elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    list(APPEND src src/solaris_build_deps.c)
endif()

if(CMAKE_COMPILER_IS_GNUCC)
    add_compile_options("$<$<CONFIG:DEBUG>:-Wall>" "$<$<CONFIG:RELEASE>:-O3>")
    add_compile_options("$<$<CONFIG:DEBUG>:-Werror>")

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_compile_options("$<$<CONFIG:DEBUG>:-pedantic>")
    else()
        add_compile_options("$<$<CONFIG:DEBUG>:-Wpedantic>")
    endif()
else()
    # TODO: Configure compiler flags for this one
    message("TODO: Configure compiler flags for this one")
endif()

add_executable(osbuild ${src})
target_include_directories(osbuild PUBLIC ${PROJECT_BINARY_DIR}/src)
