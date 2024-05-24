project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
	pchsource "src/pch.cpp"

	files 
	{
		"src/**.cpp",
		"include/**.h",

        "%{wks.location}/libraries/stb_image/**.h",
		"%{wks.location}/libraries/stb_image/**.cpp",

		"%{wks.location}/libraries/glm/glm/**.hpp",
		"%{wks.location}/libraries/glm/glm/**.inl",
	}

    disablewarnings
	{
        "4251",
		"4131",
		"4267"
    }

	defines 
	{
        "_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"_CB_BUILD_DLL",
	}

	includedirs 
	{
        "include",
		"include/Core",
		"include/ImGui",
		"include/Platforms",
		"include/Platforms/OpenGL",
		"include/Render",
		"include/Render/Camera",
		"include/Scene",
		"include/Scene/Systems",
		"%{wks.location}/libraries",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
        "%{IncludeDir.Box2D}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.raudio}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.stb_truetype}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.lua}",
        "%{IncludeDir.sol}",
	}

	links
	{
		"GLFW",
        "opengl32.lib",
        "Glad",
        "Box2D",
        "ImGui",
		"Lua",
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Editor/\""),
		-- Copy to the project game folder with ARGS in the future
		("{COPY} %{cfg.buildtarget.relpath} \"../Game/build/" .. outputdir .. "\""),
	}

    filter "files:libraries/ImGuizmo/**.cpp"
	flags { "NoPCH" }


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {
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