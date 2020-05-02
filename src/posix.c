#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)

#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#ifndef ENOPKG
#define ENOPKG 6565
#endif

#include "posix.h"
#include "errors.h"

int execute_bin(const char *const *const args) {
    int status;
    extern char **environ;
    pid_t ret;
    const char *const env[] = {NULL};
    const pid_t pid = fork();

    if (!exists(args[0])) return ENOPKG;

    if (pid == -1) return pid;
    else if (pid != 0) {
        while ((ret = waitpid(pid, &status, 0)) == -1) {
            if (errno != EINTR) return errno;
        }
        if ((ret != -1) &&
            (!WIFEXITED(status) || !WEXITSTATUS(status)) )
            return UNEXPECTED_CHILD_STATUS;
        return EXIT_FAILURE;
    } else
    if (execve(args[0], (char *const *) args, (char *const *) env) == -1)
        return 127;
    else
        return EXIT_SUCCESS;
}

// Edited from https://svnweb.freebsd.org/base/stable/12/usr.bin/which/which.c
// ?revision=339434&view=markup&pathrev=339434#l105
bool exists(const char *candidate) {
    struct stat fin;

    // XXX work around access(2) false positives for superuser
    return access(candidate, X_OK) == 0 && stat(candidate, &fin) == 0 && S_ISREG(fin.st_mode)
           && (getuid() != 0 || (fin.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0);
}

#endif
