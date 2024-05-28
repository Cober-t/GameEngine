-- 'name' and 'buildPath' comes from main premake, 
-- that received this commands as arguments from projectSettings.lua
project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir (buildPath .. "/" .. outputdir)
	objdir (buildPath .. "/" .. outputdir .. "/obj")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}

	includedirs
	{
		"include",
		"%{wks.location}/libraries",
		"%{wks.location}/Engine",
        "%{wks.location}/Engine/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.stb_truetype}",
        "%{IncludeDir.lua}",
		"%{IncludeDir.sol}",
        "%{IncludeDir.spdlog}",
	}

	links
	{
		"Engine",
	}

	postbuildcommands
	{
		"del \"./**.vcxproj\"",
	}

	defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
