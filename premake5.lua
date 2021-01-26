-- Workspace Definition --

workspace "Parrot"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "sandbox"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
include_dir = {}
include_dir["GLFW"] = "parrot/vendor/GLFW/include"
include_dir["Glad"] = "parrot/vendor/Glad/include"
include_dir["imgui"] = "parrot/vendor/imgui"
include_dir["glm"] = "parrot/vendor/glm"

group "Dependencies"
    include "parrot/vendor/GLFW"
    include "parrot/vendor/Glad"
    include "parrot/vendor/imgui"
    
group ""

-- Parrot Project Definition --

project "parrot"
    location "parrot"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    pchheader "prpch.h"
    pchsource "parrot/src/prpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{include_dir.GLFW}",
        "%{include_dir.Glad}",
        "%{include_dir.imgui}",
        "%{include_dir.glm}"
    }

    links {
        "GLFW",
        "Glad",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"

        systemversion "latest"
        defines {
            "PR_PLATFORM_WINDOWS",
            "PR_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "PR_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PR_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PR_DIST"
        runtime "Release"
        optimize "on"


-- Sandbox Project Definition --

project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "parrot/vendor/spdlog/include",
        "parrot/src",
        "%{include_dir.glm}",
        "%{include_dir.imgui}"
    }

    links {
        "Parrot"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "PR_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "PR_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PR_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PR_DIST"
        runtime "Release"
        optimize "on"


