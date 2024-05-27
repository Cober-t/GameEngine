project "ImGuizmo"
	kind "StaticLib"
	language "C++"
    staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"GraphEditor.h",
		"GraphEditor.cpp",
		"ImCurveEdit.cpp",
        "ImCurveEdit.h",
        "ImGradient.cpp",
		"ImGradient.h",
		"ImGuizmo.cpp",
        "ImGuizmo.h",
        "ImSequencer.cpp",
        "ImSequencer.h",
		"ImZoomSlider.h",
	}

	includedirs
	{
		"%{IncludeDir.ImGui}",
	}

	links
	{
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
