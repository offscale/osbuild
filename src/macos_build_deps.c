#ifndef OSBUILD_DARWIN_BUILD_DEPS_H
#define OSBUILD_DARWIN_BUILD_DEPS_H
#if defined(__APPLE__) && defined(__MACH__)

#include <stdlib.h>

#include "posix.h"
#include "build_deps.h"


inline int install_build_dependencies(const char* distribution) {
    // Maybe rewrite https://github.com/Homebrew/install/blob/master/install.sh in C?
    if (exists("/usr/bin/gcc")) return EXIT_SUCCESS;
    puts("macOS requires https://developer.apple.com/xcode and its developer tools to be installed manually.");
    return EXIT_FAILURE;
}

#endif
#endif
