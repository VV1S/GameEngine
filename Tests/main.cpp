// Tests/main.cpp
#include <gtest/gtest.h>
#include <windows.h>
#include <filesystem>
#include <cstdio>

#include "Engine/Core/Log.h"

static void SetCwdToExeDir()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH);
    if (n == 0 || n == MAX_PATH) return;
    // remove file name, leave only directory
    for (int i = (int)wcslen(buf) - 1; i >= 0; --i) {
        if (buf[i] == L'\\' || buf[i] == L'/') { buf[i] = L'\0'; break; }
    }
    SetCurrentDirectoryW(buf);
}

int main(int argc, char** argv)
{
    SetCwdToExeDir();        // <<<<<< KEY
    Engine::Log::Init();     // before Application / Renderer is created

    // (optional) diagnostics – print current directory and check if shader exists
    std::error_code ec;
    auto abs = std::filesystem::weakly_canonical(
        std::filesystem::path("assets/shaders/Texture.glsl"), ec);
    std::fprintf(stderr, "[Tests] CWD=%ls\n", std::filesystem::current_path().c_str());
    std::fprintf(stderr, "[Tests] Shader exists: %s  (%ls)\n",
        std::filesystem::exists(abs) ? "YES" : "NO", abs.c_str());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
