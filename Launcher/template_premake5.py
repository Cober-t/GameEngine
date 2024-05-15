class Premake5Template():

   def __init__(self, name):
        self.template = """\
-- 'name' and 'buildPath' comes from main premake, 
-- that received this commands as arguments from projectSettings.lua
project "{0}"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir (buildPath)
	objdir (buildPath .. "/obj")

	files
	{{
		"%{{prj.name}}/**.h",
		"%{{prj.name}}/**.cpp",
	}}

	includedirs
	{{
		"include",
		"%{{wks.location}}/libraries",
		"%{{wks.location}}/Engine",
        "%{{wks.location}}/Engine/include",
		"%{{IncludeDir.glm}}",
		"%{{IncludeDir.entt}}",
        "%{{IncludeDir.Glad}}",
        "%{{IncludeDir.Box2D}}",
        "%{{IncludeDir.GLFW}}",
        "%{{IncludeDir.raudio}}",
        "%{{IncludeDir.stb_image}}",
        "%{{IncludeDir.stb_truetype}}",
        "%{{IncludeDir.lua}}",
		"%{{IncludeDir.sol}}",
        "%{{IncludeDir.spdlog}}",
	}}

	links
	{{
		"Engine",
	}}

	defines 
    {{
        "_CRT_SECURE_NO_WARNINGS",
    }}

	filter {{ "system:windows", "configurations:Debug" }}
	buildoptions "/MDd"

	filter {{ "system:windows", "configurations:Release" }}
		buildoptions "/MD"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {{
			"GLFW_INCLUDE_NONE",
		}}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "COBER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "COBER_DIST"
		runtime "Release"
		optimize "on"\
""".format(name)