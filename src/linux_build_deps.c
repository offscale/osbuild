#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#ifdef __linux__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "posix.h"
#include "build_deps.h"
/*#include "linux_version.h"*/

#define PROGRAM "/usr/bin/apt-get"

inline bool osbuild_is_installed(const char* distribution) {
    return exists(PROGRAM) && (exists("/usr/bin/gcc") || exists("/usr/bin/clang"))
           && exists("/usr/bin/ld") && exists("/usr/bin/make") && exists("/usr/lib/libc.so");
}

inline int deb_install_build_dependencies(void) {
    static const char * const program = "/usr/bin/apt-get";

    static const char *const args0[4] = {PROGRAM, "update", "-q", NULL};
    const int ret = execute_bin(args0);
    if (ret != 0) return ret;

    static const char *const args1[5] = {PROGRAM, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

inline int osbuild_install_build_dependencies(const char* distribution) {
    if (osbuild_is_installed(distribution))
        return EXIT_SUCCESS;
    else if (strcmp(distribution, "debian") == 0)
        return deb_install_build_dependencies();
    else {
        fprintf(stderr, "Unsupported Linux distribution: %s", strlen(distribution) > 0 ? distribution : "<unknown>" );
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
