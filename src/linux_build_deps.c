#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#ifdef __linux__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "posix.h"
#include "build_deps.h"
#include "docopt.h"
/*#include "linux_version.h"*/

#define APT_GET "/usr/bin/apt-get"
#define ALPINE_APK "/sbin/apk"

inline bool osbuild_is_installed(const char* distribution) {
    return (access("/usr/bin/gcc", F_OK) == 0 || access("/usr/bin/clang", F_OK) == 0)
           && access("/usr/bin/ld", F_OK) == 0 && access("/usr/bin/make", F_OK) == 0
           && access("/usr/lib/libc.so", F_OK) == 0;
}

inline int alpine_install_build_dependencies(bool update) {
    if (access(ALPINE_APK, F_OK) != 0) return ENOENT;
    else if (update) {
        static const char *const args1[3] = {ALPINE_APK, "update", NULL};
        return execute_bin(args1);
    } else if (getenv("NO_CACHE") != NULL) {
        static const char *const args1[5] = {ALPINE_APK, "--no-cache", "add", "build-base", NULL};
        return execute_bin(args1);
    }

    static const char *const args1[4] = {ALPINE_APK, "add", "build-base", NULL};
    return execute_bin(args1);
}

inline int deb_install_build_dependencies(bool update) {
    if(access(APT_GET, F_OK) != 0) return ENOENT;
    else if(update) {
        static const char *const args0[4] = {APT_GET, "update", "-qq", NULL};
        const int ret = execute_bin(args0);
        if (ret != 0) return ret;
    }
    static const char *const args1[5] = {APT_GET, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

inline int osbuild_install_build_dependencies(const char* distribution, const DocoptArgs args) {
    printf("osbuild_is_installed:\t\t\t\t\t%d\n", osbuild_is_installed(distribution));
    printf("access(\"/usr/bin/clang\", F_OK) == 0:\t%d\n", access("/usr/bin/clang", F_OK) == 0);
    printf("access(\"/usr/bin/gcc\", F_OK) == 0:\t%d\n", access("/usr/bin/gcc", F_OK) == 0);
    printf("access(\"/usr/bin/ld\", F_OK) == 0:\t%d\n", access("/usr/bin/ld", F_OK) == 0);
    printf("access(\"/usr/bin/make\", F_OK) == 0:\t%d\n", access("/usr/bin/make", F_OK) == 0);
    printf("access(\"/usr/lib/libc.so\", F_OK) == 0:\t%d\n", access("/usr/lib/libc.so", F_OK) == 0);

    if (osbuild_is_installed(distribution))
        return EXIT_SUCCESS;
    else if (strcmp(distribution, "alpine") == 0)
        return alpine_install_build_dependencies(args.update);
    else if (strcmp(distribution, "debian") == 0)
        return deb_install_build_dependencies(args.update);
    else {
        fprintf(stderr, "Unsupported Linux distribution: %s", strlen(distribution) > 0 ? distribution : "<unknown>" );
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
