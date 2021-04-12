project "ImGuizmo"
	location "../ImGuizmo_prj"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../ImGuizmo_prj/bin/" .. output_dir .. "/%{prj.name}")
	objdir ("../ImGuizmo_prj/bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"ImCurveEdit.cpp",
		"ImCurveEdit.h",
		"ImGradient.cpp",
		"ImGradient.h",
		"ImGuizmo.cpp",
		"ImGuizmo.h",
		"ImSequencer.cpp",
		"ImSequencer.h",
		"ImZoomSlider.h"
	}

	includedirs {
        "../imgui"
	}

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
