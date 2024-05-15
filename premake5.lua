function inArray(list, item)
    for key, value in pairs(list) do
        if value == item then return true end
    end
    return false
end

workspace "Cober"
    architecture "x64"
    startproject "Editor"

    configurations { 
        "Debug", 
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}


-- This is a helper variable, to concatenate the sys-arch
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- Overwritte with command arg from editor when compile
buildPath = "%{prj.location}/build"

IncludeDir = {}
IncludeDir["GLFW"]          = "%{wks.location}/libraries/glfw/include"
IncludeDir["Glad"]          = "%{wks.location}/libraries/glad/include"
IncludeDir["ImGui"]         = "%{wks.location}/libraries/imgui"
IncludeDir["ImGuizmo"]      = "%{wks.location}/libraries/ImGuizmo"
IncludeDir["glm"]           = "%{wks.location}/libraries/glm"
IncludeDir["entt"]          = "%{wks.location}/libraries/entt"
IncludeDir["Box2D"]         = "%{wks.location}/libraries/box2d/include/box2d"
IncludeDir["spdlog"]        = "%{wks.location}/libraries/spdlog/include"
IncludeDir["raudio"]        = "%{wks.location}/libraries/raudio"
IncludeDir["stb_image"]     = "%{wks.location}/libraries/stb_image/include"
IncludeDir["stb_truetype"]  = "%{wks.location}/libraries/stb_truetype"
IncludeDir["lua"]           = "%{wks.location}/libraries/lua/lua"
IncludeDir["sol"]           = "%{wks.location}/libraries/sol"


group "Dependencies"
	-- include "libraries/premake"
    include "libraries/glfw"
    include "libraries/glad"
    include "libraries/imgui"
    include "libraries/ImGuizmo"
    include "libraries/box2D"
    include "libraries/lua"
    include "libraries/sol"
    include "libraries/entt"
group ""


group "Core"
    include "Engine"
group ""

if inArray(_ARGS, "game") == false then
    group "Tools"
        include "Editor"
    group ""
end

if inArray(_ARGS, "game") then
    group "Application"
        include ("" .. _ARGS[4] .. "")
    group ""
end