#ifndef OSBUILD_WIN_BUILD_DEPS_H
#define OSBUILD_WIN_BUILD_DEPS_H
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>

#include "build_deps.h"
#include "errors.h"

inline int install_build_dependencies(void) {
    // Maybe check if chocolately is installed, and if it is, run `choco install visualstudio2019buildtools` in PS
    // Maybe rewrite https://chocolatey.org/install.ps1 in C?
    puts("Build Tools for Visual Studio 2019 are available https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019\n");
    return UNIMPLEMENTED;
}

#endif
#endif
