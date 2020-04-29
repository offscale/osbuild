#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#ifdef __linux__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "posix.h"
#include "build_deps.h"
/*#include "linux_version.h"*/

#define APT_GET "/usr/bin/apt-get"
#define ALPINE_APK "/sbin/apk"

inline bool osbuild_is_installed(const char* distribution) {
    return (access("/usr/bin/gcc", F_OK) == 0 || access("/usr/bin/clang", F_OK) == 0)
           && access("/usr/bin/ld", F_OK) == 0 && access("/usr/bin/make", F_OK) == 0
           && access("/usr/lib/libc.so", F_OK) == 0;
}

inline int alpine_install_build_dependencies(bool update) {
    if (access(ALPINE_APK, F_OK) != 0) return EXIT_FAILURE;
    static const char *const args1[4] = {ALPINE_APK, "add", "build-base", NULL};
    return execute_bin(args1);
}

inline int deb_install_build_dependencies(bool update) {
    if (access(APT_GET, F_OK) != 0) return EXIT_FAILURE;

    static const char *const args0[4] = {APT_GET, "update", "-q", NULL};
    const int ret = execute_bin(args0);
    if (ret != 0) return ret;

    static const char *const args1[5] = {APT_GET, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

inline int osbuild_install_build_dependencies(const char* distribution, const bool update) {
    if (osbuild_is_installed(distribution))
        return EXIT_SUCCESS;
    else if (strcmp(distribution, "alpine") == 0)
        return alpine_install_build_dependencies(update);
    else if (strcmp(distribution, "debian") == 0)
        return deb_install_build_dependencies(update);
    else {
        fprintf(stderr, "Unsupported Linux distribution: %s", strlen(distribution) > 0 ? distribution : "<unknown>" );
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
