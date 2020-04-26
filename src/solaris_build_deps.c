#ifndef OSBUILD_SOLARIS_BUILD_DEPS_H
#define OSBUILD_SOLARIS_BUILD_DEPS_H

#if defined(__sun) && defined(__SVR4)

#include <stddef.h>
#include <sys/utsname.h>
#include <string.h>

#include "build_deps.h"
#include "errors.h"
#include "posix.h"

#define PROGRAM "/usr/bin/pkg"

inline int install_build_dependencies(const char* distribution) {
    struct utsname unameD;
    uname(&unameD);
    if (strcmp(unameD.nodename, "openindiana") == 0) {
        static const char *const args[5] = {
            PROGRAM, "pkg", "install", "pkg://openindiana.org/metapackages/build-essential", NULL
        };
        return execute_bin(args);
    }
    return UNIMPLEMENTED;
}

#endif
#endif
