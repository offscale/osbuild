#if __linux__
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "systemd.h"
#elif !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <sys/param.h>
#include <sys/utsname.h>
#endif

#include "distribution.h"


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
else if (access("/etc/arch-release", F_OK) == 0)
    return "arch";
else if (access("/etc/altlinux-release", F_OK) == 0)
    return "altlinux";
else if (access("/etc/angstrom-version", F_OK) == 0)
    return "angstrom";
else if (access("/etc/annvix-release", F_OK) == 0)
    return "annvix";
else if (access("/etc/arklinux-release", F_OK) == 0)
    return "arklinux";
else if (access("/etc/aurox-release", F_OK) == 0)
    return "aurox";
else if (access("/etc/blackcat-release", F_OK) == 0)
    return "blackcat";
else if (access("/etc/cobalt-release", F_OK) == 0)
    return "cobalt";
else if (access("/etc/conectiva-release", F_OK) == 0)
    return "conectiva";
else if (access("/etc/debian_version", F_OK) == 0 || access("/etc/debian_release", F_OK) == 0)
    return "debian";
else if (access("/etc/fedora-release", F_OK) == 0)
    return "fedora";
else if (access("/etc/eos-version", F_OK) == 0)
    return "FreeEOS";
else if (access("/etc/frugalware-release", F_OK) == 0)
    return "frugalware";
else if (access("/etc/gentoo-release", F_OK) == 0)
    return "gentoo";
else if (access("/etc/hlfs-release", F_OK) == 0 || access("/etc/hlfs_release", F_OK) == 0)
    return "HLFS";
else if (access("/etc/immunix-release", F_OK) == 0)
    return "immunix";
else if (access("knoppix_version", F_OK) == 0)
    return "knoppix";
else if (access("/etc/lfs-release", F_OK) == 0 || access("/etc/lfs_version", F_OK) == 0)
    return "LFS";
else if (access("/etc/linuxppc-release", F_OK) == 0)
    return "Linux-PPC";
else if (access("/etc/mageia-release", F_OK) == 0)
    return "mageia";
else if (access("/etc/mandrake-release", F_OK) == 0)
    return "mandrake";
else if (access("/etc/mandriva-release", F_OK) == 0 || access("/etc/mandakelinux-release", F_OK) == 0)
    return "mandriva";
else if (access("/etc/meego-release", F_OK) == 0)
    return "meego";
else if (access("/etc/mklinux-release", F_OK) == 0)
    return "MkLinux";
else if (access("/etc/nld-release", F_OK) == 0)
    return "Novell Linux Desktop";
else if (access("/etc/pld-release", F_OK) == 0)
    return "PLD Linux";
else if (access("/etc/redhat-release", F_OK) == 0 || access("/etc/redhat_version", F_OK) == 0)
    return "RHEL";
else if (access("/etc/rubix-version", F_OK) == 0)
    return "rubix";
else if (access("/etc/slackware-version", F_OK) == 0 || access("/etc/slackware-release", F_OK) == 0)
    return "slackware";
else if (access("/etc/e-smith-release", F_OK) == 0)
    return "SME Server";
else if (access("/etc/release", F_OK) == 0)
    return "Solaris SPARC";
else if (access("/etc/sun-release", F_OK) == 0)
    return "Sun JDS";
else if (access("/etc/SuSE-release", F_OK) == 0 || access("/etc/novell-release", F_OK) == 0
         || access("/etc/sles-release", F_OK) == 0)
    return "SuSE";
else if (access("/etc/synoinfo.conf", F_OK) == 0)
    return "Synology";
else if (access("/etc/tinysofa-release", F_OK) == 0)
    return "tinysofa";
else if (access("/etc/trustix-release", F_OK) == 0 || access("/etc/trustix-version", F_OK) == 0)
    return "trustix";
else if (access("/etc/turbolinux-release", F_OK) == 0)
    return "TurboLinux";
else if (access("/etc/ultrapenguin-release", F_OK) == 0)
    return "ultrapenguin";
else if (access("/etc/UnitedLinux-release", F_OK) == 0)
    return "UnitedLinux";
else if (access("/etc/va-release", F_OK) == 0)
    return "VA-Linux/RH-VALE";
else if (access("/etc/yellowdog-release", F_OK) == 0)
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
