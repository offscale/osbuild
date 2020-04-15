#ifndef OSBUILD_WIN_BUILD_DEPS_H
#define OSBUILD_WIN_BUILD_DEPS_H
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#include "build_deps.h"

int install_build_dependencies(void) {
    puts("Windows\n");
    return UNIMPLEMENTED;
}

#endif
#endif
