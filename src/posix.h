#ifndef OSBUILD_POSIX_H
#define OSBUILD_POSIX_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "build_deps.h"

int execute_bin(const char *const *const args) {
    int status;
    extern char **environ;
    pid_t ret;
    const char *const env[] = {NULL};
    const pid_t pid = fork();

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

#endif //OSBUILD_POSIX_H
