#ifndef OSBUILD_DARWIN_BUILD_DEPS_H
#define OSBUILD_DARWIN_BUILD_DEPS_H
#if defined(__APPLE__) && defined(__MACH__)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "posix.h"
#include "build_deps.h"

inline bool osbuild_is_installed(const char *distribution) {
    return exists("/usr/bin/cc") && exists("/usr/bin/make") && exists("/Applications/Xcode.app/Contents/Developer");
}

inline int osbuild_install_build_dependencies(const char *distribution) {
    // Maybe rewrite https://github.com/Homebrew/install/blob/master/install.sh in C?
    if (osbuild_is_installed(distribution)) return EXIT_SUCCESS;
    fprintf(stderr,
            "macOS requires https://developer.apple.com/xcode & its developer tools to be installed manually.\n");
    return EXIT_FAILURE;
}

#endif
#endif
