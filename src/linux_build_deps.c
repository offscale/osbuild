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
#define YUM "/usr/bin/yum"

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define LIBC "/usr/lib/x86_64-linux-gnu/libc.a"
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define LIBC "/usr/lib/i386-linux-gnu/libc.a"
#else
#define LIBC "/usr/lib/libc.a"
#endif

inline bool osbuild_is_installed(const char* distribution) {
    return (access("/usr/bin/gcc", F_OK) == 0 || access("/usr/bin/clang", F_OK) == 0)
           && access("/usr/bin/ld", F_OK) == 0 && access("/usr/bin/make", F_OK) == 0
           && (access("/usr/lib/libc.so", F_OK) == 0 || access(LIBC, F_OK) == 0);
}

inline int alpine_install_build_dependencies(const bool no_update) {
    if (access(ALPINE_APK, F_OK) != 0) return ENOENT;
    else if (!no_update) {
        static const char *const args[3] = {ALPINE_APK, "update", NULL};
        return execute_bin(args);
    } else if (getenv("NO_CACHE") != NULL) {
        static const char *const args[5] = {ALPINE_APK, "--no-cache", "add", "build-base", NULL};
        return execute_bin(args);
    }

    static const char *const args[4] = {ALPINE_APK, "add", "build-base", NULL};
    return execute_bin(args);
}

inline int deb_install_build_dependencies(const bool no_update) {
    if (access(APT_GET, F_OK) != 0) return ENOENT;
    else if (!no_update) {
        static const char *const args0[4] = {APT_GET, "update", "-qq", NULL};
        const int ret = execute_bin(args0);
        if (ret != 0) return ret;
    }
    static const char *const args1[5] = {APT_GET, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

inline int yum_install_build_dependencies(const bool no_update) {
    if (access(YUM, F_OK) != 0) return ENOENT;
    else if (no_update) {
        static const char *const args[5] = {YUM, "groupinstall", "-C", "Development Tools", NULL};
        const int ret = execute_bin(args);
        return ret;
    }
    static const char *const args[4] = {YUM, "groupinstall", "Development Tools", NULL};
    return execute_bin(args);
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs args) {
    printf("osbuild_is_installed:\t\t\t\t\t%d\n", osbuild_is_installed(args.distribution));
    printf("access(\"/usr/bin/clang\", F_OK) == 0:\t%d\n", access("/usr/bin/clang", F_OK) == 0);
    printf("access(\"/usr/bin/gcc\", F_OK) == 0:\t\t%d\n", access("/usr/bin/gcc", F_OK) == 0);
    printf("access(\"/usr/bin/ld\", F_OK) == 0:\t\t%d\n", access("/usr/bin/ld", F_OK) == 0);
    printf("access(\"/usr/bin/make\", F_OK) == 0:\t\t%d\n", access("/usr/bin/make", F_OK) == 0);
    printf("access(\"/usr/lib/libc.so\", F_OK) == 0:\t%d\n", access("/usr/lib/libc.so", F_OK) == 0);
    printf("access(\"%s\", F_OK) == 0:\t%d\n", LIBC, access(LIBC, F_OK) == 0);

    if (osbuild_is_installed(args.distribution))
        return EXIT_SUCCESS;
    else if (strcmp(args.distribution, "alpine") == 0)
        return alpine_install_build_dependencies(args.no_update);
    else if (strcmp(args.distribution, "centos") == 0 || strcmp(args.distribution, "redhat") == 0
             || strcmp(args.distribution, "rhel") == 0 || strcmp(args.distribution, "RHEL") == 0)
        return yum_install_build_dependencies(args.no_update);
    else if (strcmp(args.distribution, "debian") == 0)
        return deb_install_build_dependencies(args.no_update);
    else {
        fprintf(stderr, "Unsupported Linux distribution: %s", strlen(args.distribution) > 0 ?
                                                              args.distribution : "<unknown>");
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
