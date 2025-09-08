-- Tests/premake5.tests.lua
local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Tests"
project "GameEngineTests"
    location "Tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "Tests/unit/**.cpp",
        "Tests/helpers/**.h",
        "Tests/integration/**.cpp",
        "Tests/main.cpp",
        "Tests/TestPch.h",

        -- GoogleTest (kompilujemy do EXE, bez agregatów)
        "Tests/third_party/googletest/googletest/src/*.cc",
        "Tests/third_party/googletest/googletest/src/**.h",
        "Tests/third_party/googletest/googletest/include/**.h"
    }

    removefiles {
        "Tests/third_party/googletest/googletest/src/gtest-all.cc",
        "Tests/third_party/googletest/googletest/src/gtest_main.cc"
    }

    includedirs {
        "Tests",              
        "Tests/helpers",       -- TestPch.h
        "GameEngine/src",
        "GameEngine/src/Engine",      -- pozwala #include "Application.h"
        "GameEngine/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "Tests/third_party/googletest/googletest",
        "Tests/third_party/googletest/googletest/include"
    }

    -- Link: sam silnik + jego zależności (statyczne liby nie propagują zależności)
    links {
        "GameEngine",
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "Shell32.lib"
    }
    dependson { "GameEngine", "GLFW", "Glad", "ImGui" }

    -- Wstrzykuj nasz nagłówek tylko do plików testowych
    filter "files:Tests/unit/**.cpp"
        forceincludes { "TestPch.h" }
    filter "files:Tests/integration/**.cpp"
        forceincludes { "TestPch.h" }
    filter {}

    exceptionhandling "On"
    rtti "On"

    filter "system:windows"
        systemversion "latest"
        defines {
            "EG_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE",
            "_CRT_SECURE_NO_WARNINGS",
            "NOMINMAX",
            "EG_STATIC",
            "EG_TESTS" 
        }
        -- potrzebne przez Renderer/GLFW, gdy obiekty z GameEngine.lib odwołują się do GL:
        links { "opengl32.lib", "Shell32.lib" }

        -- Wymuś dołączenie WSZYSTKICH obiektów z GameEngine.lib
        -- (czasem inaczej MSVC nie podciąga TU z prywatnymi metodami, jeśli nie widzi „twardego” użycia)

        -- Szukamy main(), nie WinMain
        linkoptions { "/SUBSYSTEM:CONSOLE" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        linkoptions { "/DEBUG:FULL" }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

    -- 🔽🔽🔽 BARDZO WAŻNE: wyczyść filtry, żeby komendy dotyczyły KAŻDEJ konfiguracji
    filter {}

    -- Katalog roboczy = folder z exe
    debugdir "%{cfg.targetdir}"

    -- Kopiowanie assets po buildzie (ze źródła Sandbox/assets)
    postbuildcommands {
        '{MKDIR} "%{cfg.targetdir}/assets"',
        '{COPYDIR} "%{path.getabsolute(' .. "'" .. 'Sandbox/assets' .. "'" .. ')}" "%{cfg.targetdir}/assets"'
    }
