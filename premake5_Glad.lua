project "Glad"
    location "../Glad_prj"
    kind "StaticLib"
    language "C"

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
    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"