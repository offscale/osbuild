#ifndef OSBUILD_FREEBSD_BUILD_DEPS_H
#define OSBUILD_FREEBSD_BUILD_DEPS_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__) && !defined(_MSC_VER)
#include <sys/param.h>
#endif

#ifdef __FREEBSD__

#include <stddef.h>

#include "posix.h"
#include "build_deps.h"

// static const char const* const program = "/usr/bin/apt-get";
#define PROGRAM "/usr/local/sbin/pkg"

inline int install_build_dependencies(const char* distribution) {
    static const char const*const args[4] = {PROGRAM, "info", "pkg", NULL};
    return execute_bin(args);
}

#endif
#endif
