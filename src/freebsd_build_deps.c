#ifndef OSBUILD_FREEBSD_BUILD_DEPS_H
#define OSBUILD_FREEBSD_BUILD_DEPS_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#ifdef __FreeBSD__

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "posix.h"
#include "build_deps.h"
#include "bsd.h"

#define PROGRAM "/usr/local/sbin/pkg"

inline bool osbuild_is_installed(const char *distribution) {
    return access(PROGRAM, F_OK) == 0 && osbuild_for_bsd_is_installed();
}

inline int osbuild_install_build_dependencies(struct DocoptArgs args) {
    if (osbuild_is_installed(args->distribution)) return EXIT_SUCCESS;
    fprintf(stderr,
            "Congratulations: you built a custom FreeBSD without build-tools. You're on your own!\n");
    return EXIT_FAILURE;
}

#endif
#endif
