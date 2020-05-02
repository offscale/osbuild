#ifndef OSBUILD_WIN_BUILD_DEPS_H
#define OSBUILD_WIN_BUILD_DEPS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <io.h>
#include <Windows.h>

#include "build_deps.h"
#include "errors.h"

int execute_bin(TCHAR *, TCHAR *);

#define PROGRAM "C:\\ProgramData\\chocolatey\\bin\\choco.exe"

const LPCSTR find_cl() {
    /* Originally from https://gitlab.kitware.com/cmake/cmake/-/blob/417b765f/Modules/GetPrerequisites.cmake#L670 */
    static const LPCSTR cl_paths[12] = {
        "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\bin",
        "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin",
        "C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\bin",
        "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\bin",
        "C:\\Program Files\\Microsoft Visual Studio 9.0\\VC\\bin",
        "C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\VC\\bin",
        "C:\\Program Files\\Microsoft Visual Studio 8\\VC\\BIN",
        "C:\\Program Files (x86)\\Microsoft Visual Studio 8\\VC\\BIN",
        "C:\\Program Files\\Microsoft Visual Studio .NET 2003\\VC7\\BIN",
        "C:\\Program Files (x86)\\Microsoft Visual Studio .NET 2003\\VC7\\BIN",
        "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.25.28610\\bin\\Hostx86\\x64",
        "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.25.28610\\bin\\Hostx86\\x86",
    };

    size_t i;
    for (i = 0; i < sizeof cl_paths / sizeof cl_paths[0]; i++)
        if (_access(cl_paths[i], 0) == 0)
            return cl_paths[i];
    return NULL;
}

inline bool osbuild_is_installed(const char* distribution) {
    const LPCSTR const cl_location = find_cl();
    printf("cl at %s\n", cl_location);
    if (cl_location == NULL) return false;

    LPCSTR joined[150];
    const size_t len = strlen(cl_location);
    static const LPCSTR executables[3] = {
        "\\cl.exe",
        "\\link.exe",
        "\\nmake.exe"
    };
    size_t i;
    for (i = 0; i < sizeof executables / sizeof executables[0]; i++) {
        strncpy(joined, cl_location, len + 1);
        strncat(joined, executables[i], 10);
        if (_access(joined, 0) != 0) return false;
    }
    return true;
}

inline int osbuild_install_build_dependencies(const struct DocoptArgs *args) {
    /* Maybe rewrite https://chocolatey.org/install.ps1 in C? */

    /* if (osbuild_is_installed(args->distribution)) return EXIT_SUCCESS; */
    if (_access(PROGRAM, 0) == 0)
        return execute_bin(PROGRAM, " install visualstudio2019buildtools");
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

#endif
#endif
