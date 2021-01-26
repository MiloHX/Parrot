project "Glad"
    location "../Glad_prj"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("../Glad_prj/bin/" .. output_dir .. "/%{prj.name}")
    objdir ("../Glad_prj/bin-int/" .. output_dir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs {
        "include"
    }

    filter "system:windows"
        systemversion "latest"


    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"