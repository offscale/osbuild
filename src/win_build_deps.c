#ifndef OSBUILD_WIN_BUILD_DEPS_H
#define OSBUILD_WIN_BUILD_DEPS_H
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <stdlib.h>

#include <Windows.h>
#include <tchar.h>
#include <windef.h>

#include "build_deps.h"
#include "errors.h"

int execute_bin(const TCHAR *, const TCHAR *);
BOOL exists(const TCHAR *);

#define PROGRAM "C:\\ProgramData\\chocolatey\\bin\\choco.exe"

inline int install_build_dependencies(void) {
    // Maybe check if chocolately is installed, and if it is, run `choco install visualstudio2019buildtools` in PS
    // Maybe rewrite https://chocolatey.org/install.ps1 in C?
    if (exists("cl")) {
        return EXIT_SUCCESS;
    } else if (exists(PROGRAM)) {
        return execute_bin(PROGRAM, "install visualstudio2019buildtools")
    } else {
        fprintf(stderr, "Build Tools for Visual Studio 2019 are available https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019\n");
        return EXIT_FAILURE;
    }
}

int execute_bin(const TCHAR *absolute_bin_path, const TCHAR *input) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process.
    bool ret = CreateProcess( TEXT(absolute_bin_path),
        input,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
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

BOOL exists(const TCHAR *absolute_bin_path) {
    // TODO: Check if path is accessible and executable
    return FALSE;
}

#endif
#endif
