#ifndef OSBUILD_NETBSD_BUILD_DEPS_H
#define OSBUILD_NETBSD_BUILD_DEPS_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#ifdef __NetBSD__

#include <stddef.h>
#include "stdbool.h"
#include <unistd.h>

#include "build_deps.h"
#include "bsd.h"
#include "errors.h

inline bool osbuild_is_installed(const char *distribution) {
    return access("/usr/sbin/pkg_info", F_OK) && osbuild_for_bsd_is_installed();
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs *args) {
    /* if (!args->no_check && osbuild_is_installed(args->distribution)) return EXIT_SUCCESS; */
    /* TODO: If false, download http://<mirror>/pub/NetBSD/NetBSD-<VERSION>/<ARCH>/binary/sets/comp.tgz
             && tar --unlink -zxvpf .../comp.tgz */
    return UNIMPLEMENTED;
}

#endif
#endif
