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
		objdir("bin/obj")

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
			"C:/Users/Jorge/Documents/GameEngine/libraries",
			"C:/Users/Jorge/Documents/GameEngine/libraries/glfw/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/glad/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/glm",
			"C:/Users/Jorge/Documents/GameEngine/libraries/entt",
			"C:/Users/Jorge/Documents/GameEngine/libraries/box2d/include/box2d",
			"C:/Users/Jorge/Documents/GameEngine/libraries/stb_image/include",
			"C:/Users/Jorge/Documents/GameEngine/libraries/stb_truetype",
			"C:/Users/Jorge/Documents/GameEngine/libraries/lua/lua",
			"C:/Users/Jorge/Documents/GameEngine/libraries/imgui",
			"C:/Users/Jorge/Documents/GameEngine/libraries/sol",
			"C:/Users/Jorge/Documents/GameEngine/libraries/spdlog/include"
		}

		defines
		{
			"_WIN32",
			"_CB_SCRIPT_DLL",
			"CB_SCRIPT",
		}


		symbolspath 'C:/Users/Jorge/Documents/GameEngine/bin/Debug-windows-x86_64/Engine/Engine.pdb'
		-- symbolspath '$(OutDir)$(TargetName)-$([System.DateTime]::Now.ToString(\"HH_mm_ss_fff\")).pdb'

		links
		{
			-- Generate in the future
			"C:/Users/Jorge/Documents/GameEngine/bin/Debug-windows-x86_64/Engine/Engine.lib",
			"C:/Users/Jorge/Documents/GameEngine/libraries/glad/bin/Debug-windows-x86_64/Glad/Glad.lib",
			"C:/Users/Jorge/Documents/GameEngine/libraries/glfw/bin/Debug-windows-x86_64/GLFW/GLFW.lib",
			"C:/Users/Jorge/Documents/GameEngine/libraries/imgui/bin/Debug-windows-x86_64/ImGui/ImGui.lib"
		}

		postbuildcommands
		{
			"del \"./**.sln\"";
			"del \"./**.vcxproj\"";
			"del \"./**.vcxproj.filters\"";
		}

		filter "system:windows"

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