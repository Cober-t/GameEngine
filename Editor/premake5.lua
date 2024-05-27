project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    -- fullOutputDir = "%{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "include/**.h",
        "src/**.cpp",
        "%{wks.location}/libraries/stb_image/**.h",
		"%{wks.location}/libraries/stb_image/**.cpp",
		"%{wks.location}/libraries/glm/glm/**.hpp",
		"%{wks.location}/libraries/glm/glm/**.inl",
    }

    -- disablewarnings { 
    --     "4251",
    --     "4131",
    --     "4267"
    -- }

    -- libdirs {
    --     fullOutputDir
    -- }

    includedirs 
    {
        "include",
        "include/Panels",
        "%{wks.location}/Engine",
        "%{wks.location}/Engine/include",
        "%{wks.location}/libraries",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGuizmo}",
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
        "ImGui",
        "ImGuizmo",
		"GLFW",
        "opengl32.lib",
        "Glad",
        "Box2D",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "__EDITOR__",
        'SOLUTION_DIR=R"($(SolutionDir))"',
        'EDITOR_ICONS_DIR=R"($(SolutionDir)Editor\\icons\\)"',
    }

    -- postbuildcommands
	-- {
	-- 	"del \"./**.vcxproj\"",
	-- 	"del \"./**.vcxproj.filters\"",
	-- 	"del \"./**.vcxproj.user\"",
	-- }

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MDd"       

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