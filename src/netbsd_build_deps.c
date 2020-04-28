#ifndef OSBUILD_FREEBSD_BUILD_DEPS_H
#define OSBUILD_FREEBSD_BUILD_DEPS_H

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#ifdef __NetBSD__

#include <stddef.h>
#include <stdbool.h>

#include "posix.h"
#include "build_deps.h"
#include "bsd.h"

inline bool osbuild_is_installed(const char *distribution) {
    return exist("/usr/sbin/pkg_info") && osbuild_for_bsd_is_installed();
}

inline int osbuild_install_build_dependencies(const char* distribution) {
    return osbuild_is_installed(distribution);
    // TODO: If false, download http://<mirror>/pub/NetBSD/NetBSD-<VERSION>/<ARCH>/binary/sets/comp.tgz
    //       && tar --unlink -zxvpf .../comp.tgz
}

#endif
