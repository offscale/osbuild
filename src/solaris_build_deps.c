#ifndef OSBUILD_SOLARIS_BUILD_DEPS_H
#define OSBUILD_SOLARIS_BUILD_DEPS_H

#if defined(__sun) && defined(__SVR4)

#include <stddef.h>
#include <string.h>
#include <sys/utsname.h>

#include "build_deps.h"
#include "errors.h"
#include "posix.h"

#define PROGRAM "/usr/bin/pkg"

inline bool osbuild_is_installed(const char* distribution) {
    return exist(PROGRAM) && (exists("/usr/bin/gcc") || exists("/usr/bin/clang"))
           && exists("/usr/bin/ld") && exists("/usr/bin/make") && exists("/usr/lib/libc.so");
}

inline int osbuild_install_build_dependencies(const char* distribution) {
    if (osbuild_is_installed(distribution)) return EXIT_SUCCESS;

    struct utsname unameD;
    uname(&unameD);
    if (strcmp(unameD.nodename, "openindiana") == 0) {
        static const char *const args[4] = {
            PROGRAM, "install", "pkg://openindiana.org/metapackages/build-essential", NULL
        };
        return execute_bin(args);
    } else {
        fprintf(stderr, "Unsupported Solaris/OpenSolaris/illumos distribution: %s", distribution);
        return EPROTONOSUPPORT;
    }
}

#endif
