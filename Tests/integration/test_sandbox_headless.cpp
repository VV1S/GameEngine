#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

static std::string findSandboxExecutable() {
    const fs::path cwd = fs::current_path();

    // Candidates relative to current CWD (test debugdir = bin/.../GameEngineTests)
    const fs::path p1 = cwd / "Sandbox.exe";                              // if you ever copy it next to this
    const fs::path p2 = cwd.parent_path() / "Sandbox" / "Sandbox.exe";    // bin/.../Sandbox/Sandbox.exe
    const fs::path p3 = cwd.parent_path().parent_path() / "Sandbox" / "Sandbox.exe"; // fallback

    for (const auto& p : { p1, p2, p3 }) {
        if (fs::exists(p)) return p.string();
    }
    return {}; // not found
}

static HWND FindMainWindowByPid(DWORD pid) {
    struct Ctx { DWORD pid; HWND hwnd = nullptr; } ctx{ pid, nullptr };
    EnumWindows([](HWND h, LPARAM lp) -> BOOL {
        auto* c = reinterpret_cast<Ctx*>(lp);
        DWORD wpid = 0;
        GetWindowThreadProcessId(h, &wpid);
        if (wpid == c->pid && GetWindow(h, GW_OWNER) == nullptr && IsWindowVisible(h)) {
            c->hwnd = h;
            return FALSE;
        }
        return TRUE;
        }, reinterpret_cast<LPARAM>(&ctx));
    return ctx.hwnd;
}

TEST(Integration, SandboxRunsHeadlessForAFewFrames) {
    const std::string exe = findSandboxExecutable();
    ASSERT_FALSE(exe.empty()) << "Sandbox.exe not found. CWD=" << fs::current_path().string();

    std::string cmd = "\"" + exe + "\"";

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    // Prepare a modifiable character buffer for CreateProcessA
    std::vector<char> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back('\0');

    ASSERT_TRUE(CreateProcessA(nullptr, cmdBuf.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
        << "CreateProcessA failed for: " << exe << " (CWD=" << fs::current_path().string() << ")";

    // Give it a moment to create the window, then close it politely
    HWND hwnd = nullptr;
    for (int i = 0; i < 50 && !hwnd; ++i) { // up to ~5s
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        hwnd = FindMainWindowByPid(pi.dwProcessId);
    }
    if (hwnd) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // a few frames
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }

    DWORD wait = WaitForSingleObject(pi.hProcess, 15000);
    if (wait == WAIT_TIMEOUT) {
        // Emergency close so the test doesn't hang
        TerminateProcess(pi.hProcess, 0);
        WaitForSingleObject(pi.hProcess, 2000);
    }

    DWORD code = 1;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    EXPECT_EQ(code, 0u);
}
