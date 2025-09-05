workspace "GameEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]      = "GameEngine/vendor/GLFW/include"
IncludeDir["Glad"]      = "GameEngine/vendor/Glad/include"
IncludeDir["ImGui"]     = "GameEngine/vendor/imgui"
IncludeDir["glm"]       = "GameEngine/vendor/glm"
IncludeDir["stb_image"] = "GameEngine/vendor/stb_image"

include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"

project "GameEngine"
    location "GameEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    targetname "GameEngine"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "enginepch.h"
    pchsource "%{prj.location}/src/enginepch.cpp"

    files {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",

        -- third-party bundled sources
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",

        -- ImGui stdlib binding (needed for ImGui::InputText(std::string*))
        "%{prj.name}/vendor/imgui/misc/cpp/imgui_stdlib.cpp",

        -- Build tools (Variant A)
        "%{prj.location}/src/Engine/Tools/**.h",
        "%{prj.location}/src/Engine/Tools/**.cpp"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "EG_WITH_BUILD_TOOLS" -- enable in-engine BuildManager panel
    }

    includedirs {
        "%{prj.location}/src",
        "%{prj.location}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "EG_PLATFORM_WINDOWS",
            "EG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        -- BuildManager::RunGame uses ShellExecuteA
        links { "Shell32.lib" }
    filter {}

    filter "configurations:Debug"
        defines "EG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EG_DIST"
        runtime "Release"
        optimize "on"

    -- Disable PCH just for ImGui's stdlib helper
    filter { "files:GameEngine/vendor/imgui/misc/cpp/imgui_stdlib.cpp" }
        flags { "NoPCH" }
    filter {}

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "GameEngine/vendor/spdlog/include",
        "GameEngine/src",
        "GameEngine/vendor",
        "%{IncludeDir.glm}"
    }

    links { "GameEngine" }

    -- >>> Variant 2: copy assets next to the executable after build
    -- Source:  D:\dev\c++\GameEngine\Sandbox\assets
    -- Goal:     %{cfg.targetdir}\assets
    postbuildcommands {
        '{MKDIR} "%{cfg.targetdir}/assets"',
        '{COPYDIR} "%{wks.location}Sandbox/assets" "%{cfg.targetdir}/assets"'
    }

    filter "system:windows"
        systemversion "latest"
        defines { "EG_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines "EG_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EG_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EG_DIST"
        runtime "Release"
        optimize "on"
