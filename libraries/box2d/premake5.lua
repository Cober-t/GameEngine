project "Box2D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Collision/**.cpp",
		"Collision/Shapes/**.cpp",
		"Common/**.cpp",
		"Dynamics/**.cpp",
		"Dynamics/Contacts/**.cpp",
		"Dynamics/Joints/**.cpp",
		"Rope/**.cpp",
		"includes/**.h",
		"**.h"
	}

	includedirs
	{
		"includes",
		"Collision",
		"Collision/Shapes",
		"Dynamics",
		"Dynamics/Contacts",
		"Dynamics/Joints",
		"Rope",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"