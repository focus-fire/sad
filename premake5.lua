workspace "sad"
    language "C++"
    cppdialect "C++20"
    architecture "x86_64"
	flags { "MultiProcessorCompile" }
    startproject "Game"

    targetdir "%{wks.location}/Build/Bin%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}"

    vpaths {
        ["./"] = { "**premake5.lua" },
    }

    configurations {
		"Debug",
		"Release",
        "Test",
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    filter "configurations:Test"
        defines { "NDEBUG" }
        optimize "Off"
        symbols "On"
    filter {}

    -- Core Projects 
    include "Code/Game.premake5.lua"
    include "Code/Engine.premake5.lua"

    -- Tests
    include "Tests/Tests.premake5.lua"

    group "Vendor" 
        -- Individual Deps
        include "Vendor/glad.premake5.lua"
        include "Vendor/glm.premake5.lua"
        include "Vendor/stb_image.premake5.lua"
        include "Vendor/imgui.premake5.lua"
        -- Placeholders
        include "Vendor/SDL.premake5.lua"
        -- Header Only Deps
        -- spdlog
        -- json
        -- entt
        -- Catch2
    group ""
