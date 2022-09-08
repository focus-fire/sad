workspace "sad"
    language "C++"
    cppdialect "C++20"
    architecture "x86_64"
	flags { "MultiProcessorCompile" }
    startproject "Game"

    targetdir "%{wks.location}/Build/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/Obj/%{prj.name}/%{cfg.buildcfg}"

    vpaths {
        ["./"] = { "**premake5.lua" },
    }

    configurations {
		"Debug",
		"Release"
	}

    filter "Debug"
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"
    filter "Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    filter {}

    -- Core Projects 
    include "%{wks.location}/../Code/Engine.premake5.lua"
    include "%{wks.location}/../Code/Game.premake5.lua"

    group "Vendor" -- Individual Deps
        include "%{wks.location}/../Vendor/glfw.premake5.lua"
        include "%{wks.location}/../Vendor/glad.premake5.lua"
        include "%{wks.location}/../Vendor/spdlog.premake5.lua"
        include "%{wks.location}/../Vendor/glm.premake5.lua"
        include "%{wks.location}/../Vendor/stb_image.premake5.lua"
        include "%{wks.location}/../Vendor/imgui.premake5.lua"
        include "%{wks.location}/../Vendor/Catch2.premake5.lua"
    group ""

