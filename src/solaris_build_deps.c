#ifndef OSBUILD_SOLARIS_BUILD_DEPS_H
#define OSBUILD_SOLARIS_BUILD_DEPS_H

#if defined(__sun) && defined(__SVR4)

#include <stddef.h>
#include <string.h>

#include "build_deps.h"
#include "errors.h"
#include "posix.h"

#define PROGRAM "/usr/bin/pkg"

inline bool osbuild_is_installed(const char* distribution) {
    return exist(PROGRAM, F_OK) == 0
           && (access("/usr/bin/gcc", F_OK) == 0 || access("/usr/bin/clang", F_OK) == 0)
           && access("/usr/bin/ld", F_OK) == 0 && access("/usr/bin/make", F_OK) == 0
           && access("/usr/lib/libc.so", F_OK) == 0;
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs *args) {
    if (osbuild_is_installed(args->distribution))
        return EXIT_SUCCESS;
    else if (strcmp(args->distribution, "openindiana") == 0) {
        static const char *const cli_args[4] = {
            PROGRAM, "install", "pkg://openindiana.org/metapackages/build-essential", NULL
        };
        return execute_bin(cli_args);
    } else {
        fprintf(stderr, "Unsupported Solaris/OpenSolaris/illumos distribution: %s", distribution);
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
