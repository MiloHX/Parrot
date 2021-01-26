-- Workspace Definition --

workspace "Parrot"
    architecture "x64"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
include_dir = {}
include_dir["GLFW"] = "parrot/vendor/GLFW/include"
include_dir["Glad"] = "parrot/vendor/Glad/include"
include_dir["imgui"] = "parrot/vendor/imgui"

include "parrot/vendor/GLFW"
include "parrot/vendor/Glad"
include "parrot/vendor/imgui"


-- Parrot Project Definition --

project "parrot"
    location "parrot"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    pchheader "prpch.h"
    pchsource "parrot/src/prpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{include_dir.GLFW}",
        "%{include_dir.Glad}",
        "%{include_dir.imgui}"
    }

    links {
        "GLFW",
        "Glad",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PR_PLATFORM_WINDOWS",
            "PR_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/sandbox")
        }

    filter "configurations:Debug"
        defines "PR_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "PR_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "PR_DIST"
        buildoptions "/MD"
        optimize "On"


-- Sandbox Project Definition --

project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "parrot/vendor/spdlog/include",
        "parrot/src"
    }

    links {
        "Parrot"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PR_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "PR_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "PR_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "PR_DIST"
        buildoptions "/MD"
        optimize "On"


