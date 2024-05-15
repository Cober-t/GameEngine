project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"include",
		"%{wks.location}/libraries",
		"%{wks.location}/libraries/spdlog/include",
		"%{wks.location}/Engine",
        "%{wks.location}/Engine/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.raudio}",
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

	defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

	filter { "system:windows", "configurations:Debug" }
		buildoptions "/MDd"
		defines { "COBER_TEST" }

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MD"

	filter "system:windows"
		systemversion "latest"		


	filter "configurations:Debug"
		defines {
			"GLFW_INCLUDE_NONE",
			"COBER_DEBUG",
			"COBER_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "COBER_RELEASE"
		runtime "Release"
		optimize "on"


	filter "configurations:Dist"
		defines "COBER_DIST"
		runtime "Release"
		optimize "on"