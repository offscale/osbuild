#if __linux__
#include <string.h>

#include "systemd.h"
#elif !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#endif

#include "version.h"


inline const char* get_distribution(void) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return "Windows";
#elif defined(__FreeBSD__)
    return "FreeBSD";
#elif defined(__NetBSD__)
    return "NetBSD";
#elif defined (__APPLE__) && defined (__MACH__)
    return "macOS";
#elif __linux__
    const char* dist = dist_from_os_release();
    if (strlen(dist) != 0)
        return dist;

    /* TODO:
       - /etc/redhat-release   # first line only; to detect RHEL/Fedora/CentOS
       - /etc/SuSE-release     # detect non-LSB SuSE
       - /etc/debian_version   # first line only; to detect non-LSB Debian
       - uname                 # fallback instead of "linux"
    */

    return "linux"; /* Unknown distribution */
#elif defined (__unix__)
    return "UNIX";
#else
    return "Unknown";
#endif
}
