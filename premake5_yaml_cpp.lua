project "yaml_cpp"
	location "../yaml_cpp_prj"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../yaml_cpp_prj/bin/" .. output_dir .. "/%{prj.name}")
	objdir ("../yaml_cpp_prj/bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"include/**.h"
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
