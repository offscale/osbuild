#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#if defined(__linux__)

#include "build_deps.h"
#include "posix.h"

// static const char const* const program = "/usr/bin/apt-get";
#define program "/usr/bin/apt-get"

int install_build_dependencies(void) {
    static const char const*const args0[3] = {program, "update", NULL};
    const int ret = execute_bin(args0);
    if (ret != 0) return ret;

    static const char const*const args1[5] = {program, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

#endif
#endif
