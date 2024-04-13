project "Sol"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"sol/**.hpp",
		-- "%{IncludeDir.lua}/**.hpp",
		-- "%{IncludeDir.lua}/**.h",
		-- "%{IncludeDir.lua}/**.c",
	}

	includedirs
	{
		"sol",
		"%{IncludeDir.lua}"
	}

	links
	{
		"Lua"
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
		