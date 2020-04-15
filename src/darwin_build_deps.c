#ifndef OSBUILD_DARWIN_BUILD_DEPS_H
#define OSBUILD_DARWIN_BUILD_DEPS_H

#if defined(__APPLE__) && defined(__MACH__)

#include <stdlib.h>

#include "build_deps.h"
#include "math.h"
#include "posix.h"

int install_build_dependencies(void) {
    if (exists("gcc")) return EXIT_SUCCESS;
    puts("macOS requires https://developer.apple.com/xcode and its developer tools to be installed manually.");
    return EXIT_FAILURE;
}

#endif
#endif
