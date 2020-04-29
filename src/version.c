#if __linux__
#include <string.h>
#include <sys/utsname.h>

#include "systemd.h"
#include "posix.h"
#elif !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#include <sys/utsname.h>
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
    /* List compiled from:
     * - https://www.freedesktop.org/software/systemd/man/os-release.html
     * - http://0pointer.de/blog/projects/os-release.html
     * - http://linuxmafia.com/faq/Admin/release-files.html
     */

    const char* dist = dist_from_os_release();
    if (strlen(dist) != 0)
        return dist;
    else if (exists("/etc/arch-release"))
        return "arch";
    else if (exists("/etc/altlinux-release"))
        return "altlinux";
    else if (exists("/etc/angstrom-version"))
        return "angstrom";
    else if (exists("/etc/annvix-release"))
        return "annvix";
    else if (exists("/etc/arklinux-release"))
        return "arklinux";
    else if (exists("/etc/aurox-release"))
        return "aurox";
    else if (exists("/etc/blackcat-release"))
        return "blackcat";
    else if (exists("/etc/cobalt-release"))
        return "cobalt";
    else if (exists("/etc/conectiva-release"))
        return "conectiva";
    else if (exists("/etc/debian_version") || exists("/etc/debian_release"))
        return "debian";
    else if (exists("/etc/fedora-release"))
        return "fedora";
    else if (exists("/etc/eos-version"))
        return "FreeEOS";
    else if (exists("/etc/frugalware-release"))
        return "frugalware";
    else if (exists("/etc/gentoo-release"))
        return "gentoo";
    else if (exists("/etc/hlfs-release") || exists("/etc/hlfs_release"))
        return "HLFS";
    else if (exists("/etc/immunix-release"))
        return "immunix";
    else if (exists("knoppix_version"))
        return "knoppix";
    else if (exists("/etc/lfs-release") || exists("/etc/lfs_version"))
        return "LFS";
    else if (exists("/etc/linuxppc-release"))
        return "Linux-PPC";
    else if (exists("/etc/mageia-release"))
        return "mageia";
    else if (exists("/etc/mandrake-release"))
        return "mandrake";
    else if (exists("/etc/mandriva-release") || exists("/etc/mandakelinux-release"))
        return "mandriva";
    else if (exists("/etc/meego-release"))
        return "meego";
    else if (exists("/etc/mklinux-release"))
        return "MkLinux";
    else if (exists("/etc/nld-release"))
        return "Novell Linux Desktop";
    else if (exists("/etc/pld-release"))
        return "PLD Linux";
    else if (exists("/etc/redhat-release") || exists("/etc/redhat_version"))
        return "RHEL";
    else if (exists("/etc/rubix-version"))
        return "rubix";
    else if (exists("/etc/slackware-version") || exists("/etc/slackware-release"))
        return "slackware";
    else if (exists("/etc/e-smith-release"))
        return "SME Server";
    else if (exists("/etc/release"))
        return "Solaris SPARC";
    else if (exists("/etc/sun-release"))
        return "Sun JDS";
    else if (exists("/etc/SuSE-release") || exists("/etc/novell-release") || exists("/etc/sles-release"))
        return "SuSE";
    else if (exists("/etc/synoinfo.conf"))
        return "Synology";
    else if (exists("/etc/tinysofa-release"))
        return "tinysofa";
    else if (exists("/etc/trustix-release") || exists("/etc/trustix-version"))
        return "trustix";
    else if (exists("/etc/turbolinux-release"))
        return "TurboLinux";
    else if (exists("/etc/ultrapenguin-release"))
        return "ultrapenguin";
    else if (exists("/etc/UnitedLinux-release"))
        return "UnitedLinux";
    else if (exists("/etc/va-release"))
        return "VA-Linux/RH-VALE";
    else if (exists("/etc/yellowdog-release"))
        return "yellowdog";
    else {
        struct utsname unameD;
        uname(&unameD);
        return strlen(unameD.nodename) > 0 ? unameD.nodename : unameD.sysname;
    }
#elif defined(__sun) && defined(__SVR4)
    struct utsname unameD;
    uname(&unameD);
    return unameD.nodename;
#elif defined (__unix__)
    return "UNIX";
#else
    return "Unknown";
#endif
}
