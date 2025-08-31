#pragma once

#ifdef EG_PLATFORM_WINDOWS

#include <memory>
#include <exception>
#include <cstdlib>

#include "Core.h"
#include "Log.h"
#include "Application.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
    (void)argc; (void)argv; // unused for now

    // Initialize logging
    Engine::Log::Init();

    int exitCode = EXIT_SUCCESS;

    try
    {
        // Create and run the application (RAII via unique_ptr)
        std::unique_ptr<Engine::Application> app{ Engine::CreateApplication() };
        app->Run(); // blocks until close
        // app is destroyed automatically here
    }
    catch (const std::exception& ex)
    {
        EG_CORE_ERROR("Unhandled std::exception: {}", ex.what());
        exitCode = EXIT_FAILURE;
    }
    catch (...)
    {
        EG_CORE_ERROR("Unhandled unknown exception.");
        exitCode = EXIT_FAILURE;
    }

    return exitCode;
}

#endif // EG_PLATFORM_WINDOWS
