#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#if defined(__linux__)

#include <stddef.h>

#include "posix.h"
#include "build_deps.h"

// static const char const* const program = "/usr/bin/apt-get";
#define PROGRAM "/usr/bin/apt-get"

inline int install_build_dependencies(void) {
    static const char const*const args0[3] = {PROGRAM, "update", NULL};
    const int ret = execute_bin(args0);
    if (ret != 0) return ret;

    static const char const*const args1[5] = {PROGRAM, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

#endif
#endif
