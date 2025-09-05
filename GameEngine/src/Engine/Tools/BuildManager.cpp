#include "enginepch.h"
#include "BuildManager.h"

#include <filesystem>
#include <thread>
#include <cstdlib>

#ifdef _WIN32
#  include <windows.h>
#  include <shellapi.h>
#  pragma comment(lib, "Shell32.lib")
#endif

using namespace std::string_literals;
namespace fs = std::filesystem;

namespace Engine {

    std::atomic<bool> BuildManager::s_Busy{ false };
    std::string       BuildManager::s_Log;

	// search for premake5.lua in current or parent directories
    static fs::path FindRepoRoot(fs::path start)
    {
        for (int i = 0; i < 5; ++i) {
            if (fs::exists(start / "premake5.lua"))
                return start;
            if (!start.has_parent_path()) break;
            start = start.parent_path();
        }
        return {};
    }

    static std::string Quote(const std::string& p)
    {
        if (!p.empty() && p.front() == '"' && p.back() == '"') return p;
        return "\"" + p + "\"";
    }

    void BuildManager::AppendLog(const std::string& s)
    {
        s_Log += s;
    }

    int BuildManager::Exec(const std::string& cmd, const std::string& cwd)
    {
        auto old = fs::current_path();
        fs::current_path(cwd);
        AppendLog("> " + cmd + "\n");
        int code = std::system(cmd.c_str());
        fs::current_path(old);
        return code;
    }

    bool BuildManager::IsBusy()
    {
        return s_Busy.load();
    }

    const std::string& BuildManager::LastLog()
    {
        return s_Log;
    }

    bool BuildManager::Build(const Options& opts)
    {
        bool expected = false;
        if (!s_Busy.compare_exchange_strong(expected, true))
            return false;

        s_Log.clear();
        std::thread(Worker, opts).detach();
        return true;
    }

    void BuildManager::Worker(Options opts)
    {
        try {
            fs::path root = FindRepoRoot(fs::current_path());
            if (root.empty()) {
                AppendLog("Cannot locate premake5.lua (searched current and parent directories).\n");
                s_Busy.store(false);
                return;
            }
            AppendLog("Repo root: " + root.string() + "\n");

            // 1) Premake — generate solution for VS2022.
            if (Exec("premake5 vs2022", root.string()) != 0) {
                AppendLog("premake5 vs2022 failed.\n");
                s_Busy.store(false);
                return;
            }

#ifdef _WIN32
            // 2) MSBuild — build solution (enforce x64)
            const std::string cfg = opts.configuration.empty() ? "Debug" : opts.configuration;
            const std::string plat = "x64";

            const std::string sln = (root / "GameEngine.sln").string();
            const std::string msbuild = "msbuild";

            // Optional: Clean
            if (opts.clean) {
                std::string cleanCmd = msbuild + " " + Quote(sln)
                    + " /m /p:Configuration=" + cfg
                    + " /p:Platform=" + plat
                    + " /t:Clean";
                if (Exec(cleanCmd, root.string()) != 0) {
                    AppendLog("MSBuild Clean failed.\n");
                    s_Busy.store(false);
                    return;
                }
            }

            // Build (default in folder Sandbox)
            const std::string target = opts.target.empty() ? "Sandbox" : opts.target;
            std::string buildCmd = msbuild + " " + Quote(sln)
                + " /m /p:Configuration=" + cfg
                + " /p:Platform=" + plat
                + " /t:" + target;

            if (Exec(buildCmd, root.string()) != 0) {
                AppendLog("MSBuild Build failed.\n");
                s_Busy.store(false);
                return;
            }
#else
            AppendLog("Windows-only build flow (MSBuild).\n");
            s_Busy.store(false);
            return;
#endif

            AppendLog("Build successful.\n");

            if (opts.runAfter) {
                if (!RunGame(opts))
                    AppendLog("Launch failed.\n");
            }
        }
        catch (const std::exception& ex) {
            AppendLog(std::string("Exception: ") + ex.what() + "\n");
        }
        catch (...) {
            AppendLog("Unknown exception.\n");
        }

        s_Busy.store(false);
    }

    bool BuildManager::RunGame(const Options& opts)
    {
#ifdef _WIN32
        // Premake: bin/<Config>-windows-x86_64/<Target>/<Target>.exe
        const std::string cfg = opts.configuration.empty() ? "Debug" : opts.configuration;
        const std::string target = opts.target.empty() ? "Sandbox" : opts.target;
        const std::string archToken = "x86_64"; // enforce x64

        fs::path exe = fs::current_path()
            / ("bin/" + cfg + "-windows-" + archToken + "/" + target + "/" + target + ".exe");

        if (!fs::exists(exe)) {
            // Fallback, for other outputdir
            fs::path fallback = fs::current_path()
                / ("bin/" + cfg + "-windows-x64/" + target + "/" + target + ".exe");
            if (fs::exists(fallback)) exe = fallback;
        }

        if (!fs::exists(exe)) {
            AppendLog("Executable not found: " + exe.string() + "\n");
            return false;
        }

        HINSTANCE h = ShellExecuteA(nullptr, "open",
            exe.string().c_str(), nullptr,
            exe.parent_path().string().c_str(), SW_SHOWDEFAULT);
        if ((INT_PTR)h <= 32)
            return false;

        AppendLog("Launched: " + exe.string() + "\n");
        return true;
#else
        (void)opts;
        return false;
#endif
    }

} // namespace Engine
