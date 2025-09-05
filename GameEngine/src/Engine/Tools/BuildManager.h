#pragma once
#include <string>
#include <atomic>

namespace Engine {

    class BuildManager
    {
    public:
        struct Options {
            std::string configuration = "Debug"; // "Debug" / "Release" / "Dist"
            std::string target = "Sandbox"; // msbuild target (project)
            bool        clean = false;     // MSBuild: Clean before Build
            bool        runAfter = false;     // after successful build run exe
        };

        // Starts an asynchronous build. Returns false if another build is already in progress.
        static bool Build(const Options&);

		// If worker thread is active.
        static bool IsBusy();

		// Last log message (append-only).
        static const std::string& LastLog();

    private:
        static void        Worker(Options opts);
        static void        AppendLog(const std::string& s);
        static int         Exec(const std::string& cmd, const std::string& cwd);
        static bool        RunGame(const Options& opts);

        static std::atomic<bool>  s_Busy;
        static std::string        s_Log;
    };

} // namespace Engine
