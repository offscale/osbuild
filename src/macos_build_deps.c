#ifndef OSBUILD_DARWIN_BUILD_DEPS_H
#define OSBUILD_DARWIN_BUILD_DEPS_H
#if defined(__APPLE__) && defined(__MACH__)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "build_deps.h"
#include "posix.h"

inline bool osbuild_is_installed(const char *distribution) {
    return access("/usr/bin/cc", F_OK) == 0 && access("/usr/bin/make", F_OK) == 0
           && access("/Applications/Xcode.app/Contents/Developer", F_OK) == 0;
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs *args) {
    // Maybe rewrite https://github.com/Homebrew/install/blob/master/install.sh in C?

    if (osbuild_is_installed(args->distribution)) return EXIT_SUCCESS;
    static const char *const cli_args[3] = {"/usr/bin/xcode-select", "--install", NULL};
    return execute_bin(cli_args);
}

#endif
#endif
