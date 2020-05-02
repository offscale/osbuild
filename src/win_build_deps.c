#ifndef OSBUILD_WIN_BUILD_DEPS_H
#define OSBUILD_WIN_BUILD_DEPS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <Windows.h>
#include <tchar.h>
#include <windef.h>
#include <process.h>

#include "build_deps.h"
#include "errors.h"

int execute_bin(TCHAR *, TCHAR *);
bool exists(TCHAR *);

#define PROGRAM "C:\\ProgramData\\chocolatey\\bin\\choco.exe"

inline bool osbuild_is_installed(const char* distribution) {
    return exists("cl") && exists("nmake");
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs *args) {
    /* Maybe rewrite https://chocolatey.org/install.ps1 in C? */

    if (osbuild_is_installed(args->distribution))
        return EXIT_SUCCESS;
    else if (exists(PROGRAM))
        return execute_bin(PROGRAM, "install visualstudio2019buildtools");
    else {
        fprintf(stderr, "Build Tools for Visual Studio 2019 are available "
                        "https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019\n");
        /*
        static const char* url = "https://download.visualstudio.microsoft.com/download/pr/"
                                 "5e397ebe-38b2-4e18-a187-ac313d07332a/"
                                 "00945fbb0a29f63183b70370043e249218249f83dbc82cd3b46c5646503f9e27/"
                                 "vs_BuildTools.exe";
        static const char* sha256 = "00945fbb0a29f63183b70370043e249218249f83dbc82cd3b46c5646503f9e27";
        */
        return EXIT_FAILURE;
    }
}

int execute_bin(TCHAR *absolute_bin_path, TCHAR *input) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process.
    bool ret = CreateProcess(TEXT(absolute_bin_path),
        input,          // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure
    );

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (!ret) {
        fprintf(stderr, "CreateProcess failed (%d).\n", GetLastError() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool exists(TCHAR *absolute_bin_path) {
    // TODO: Check if path is accessible and executable
    return true;
}

#endif
#endif
