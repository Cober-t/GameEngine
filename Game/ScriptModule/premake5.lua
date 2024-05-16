workspace "ScriptingWorkspace"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "ScriptModule"

	project "ScriptModule"
		kind "SharedLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir("bin/%{prj.name}")
		objdir("bin-int/%{prj.name}")

		files
		{
			"**.cpp",
			"**.hpp",
			"**.h",

			-- Generate in the future
			-- "C:/Users/Jorge/Documents/GameEngine/libraries/stb_image/**.h",
			-- "C:/Users/Jorge/Documents/GameEngine/libraries/....
		}

		disablewarnings 
		{
			"4251",
			"4131",
			"4267"
		}

		includedirs 
		{
			"%{prj.name}/scripts",
			-- Generate in the future
			"C:/Users/Jorge/Documents/GameEngine/Engine",
			"C:/Users/Jorge/Documents/GameEngine/Engine/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/glfw/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/glad/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/glm",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/entt",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/raudio",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/box2d/include/box2d",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/stb_image/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/stb_truetype",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/lua/lua",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/sol",
			"C:/Users/Jorge/Documents/GameEngine/libraries/libraries/spdlog/include"
		}

		-- symbolspath '$(OutDir)$(TargetName)-$([System.DateTime]::Now.ToString(\"HH_mm_ss_fff\")).pdb'

		links 
		{
			-- Generate in the future
			"C:/Users/Jorge/Documents/GameEngine/bin/Debug-windows-x86_64/Engine/Engine.lib",
			-- "ImGui.lib",
		}

		filter "system:windows"

		defines
		{
			"_COBER_SCRIPT_DLL"
		}

		-- buildcommands 
		-- {
		-- 	"del /S *.pdb"
		-- }

		-- postbuildcommands
		-- {
		-- 	"copy /y \\\"$(SolutionDir)bin\\\\ScriptModule\\\\ScriptModule.dll\\\" \\\"" << engineExeDir << "/ScriptModuleTmp.dll\\\""
		-- }

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			runtime "Release"
			symbols "on"

		filter "configurations:Dist"
			runtime "Release"
			symbols "on"