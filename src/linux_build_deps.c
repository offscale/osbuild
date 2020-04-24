#ifndef OSBUILD_LINUX_BUILD_DEPS_H
#define OSBUILD_LINUX_BUILD_DEPS_H
#ifdef __linux__

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "posix.h"
#include "build_deps.h"
/*#include "linux_version.h"*/

#define PROGRAM "/usr/bin/apt-get"


inline int deb_install_build_dependencies(void) {
    static const char * const program = "/usr/bin/apt-get";

    static const char *const args0[4] = {PROGRAM, "update", "-q", NULL};
    const int ret = execute_bin(args0);
    if (ret != 0) return ret;

    static const char *const args1[5] = {PROGRAM, "install", "-y", "build-essential", NULL};
    return execute_bin(args1);
}

inline int install_build_dependencies(const char* distribution) {
    /*
    static const FILE *fp = fopen("/etc/os-release", "r");
    if (!fp) {
        fprintf(stderr, "/etc/os-release not found. Are the details in any of these?: /etc/redhat-release, "
                        "/etc/SuSE-release, /etc/debian_version, /etc/arch-release, /etc/gentoo-release, "
                        "/etc/slackware-version, /etc/frugalware-release, /etc/altlinux-release, "
                        "/etc/mandriva-release, /etc/meego-release, /etc/angstrom-version, /etc/mageia-release");
        return ENOENT;
    }
     */
    if (strcmp(distribution, "debian") == 0)
        return deb_install_build_dependencies();
    else {
        fprintf(stderr, "Unsupported Linux distribution: %s", distribution);
        return EPROTONOSUPPORT;
    }
}

#endif
#endif
