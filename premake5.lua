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

    filter "system:windows"
        defines {
            "_SAD_WINDOWS"
        }
    filter "system:macosx"
        defines {
            "_SAD_MAC"
        }
    filter {}

    filter "configurations:Debug"
        defines {
            "_SAD_DEBUG",
            "_SAD_ENABLE_LOGGING",
        }
        symbols "On"
        optimize "Off"
        symbols "On"
    filter "configurations:Release"
        defines {
            "_SAD_RELEASE"
        }
        optimize "On"
        symbols "Off"
    filter "configurations:Test"
        defines {
            "_SAD_TEST"
        }
        optimize "Off"
        symbols "On"
    filter {}

    -- Core Projects
    include "Code/Game.premake5.lua"
    include "Code/Engine.premake5.lua"
    include "Code/Core.premake5.lua"

    -- Tests
    include "Tests/Tests.premake5.lua"

    group "Vendor" -- Individual Deps
        include "Vendor/glad.premake5.lua"
        include "Vendor/glm.premake5.lua"
        include "Vendor/stb_image.premake5.lua"
        include "Vendor/imgui.premake5.lua"
        -- Placeholders
        include "Vendor/SDL.premake5.lua"
        -- Header Only
        -- include "Vendor/spdlog.premake5.lua"
        -- include "Vendor/Catch2.premake5.lua"
    group ""
