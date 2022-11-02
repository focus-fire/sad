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
            "_SAD_ENABLE_ASSERTS",
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
            "_SAD_TEST",
            "_SAD_ENABLE_LOGGING",
            "_SAD_ENABLE_ASSERTS",
        }
        optimize "Off"
        symbols "On"
    filter {}

    -- Core Projects
    include "Code/Core.premake5.lua"
    include "Code/Engine.premake5.lua"
    include "Code/Editor.premake5.lua"
    include "Code/Game.premake5.lua"

    -- Tests
    group "Tests"
        include "Tests/CoreTests.premake5.lua"
        include "Tests/GameTests.premake5.lua"
        include "Tests/EngineTests.premake5.lua"

    group "Vendor"
        -- Individual Deps
        include "Vendor/glad.premake5.lua"
        include "Vendor/glm.premake5.lua"
        include "Vendor/stb_image.premake5.lua"
        include "Vendor/imgui.premake5.lua"
        -- Placeholders
        include "Vendor/SDL.premake5.lua"
		include "Vendor/assimp.premake5.lua"
        -- Header Only Deps
        -- spdlog
        -- json
        -- entt
        -- Catch2
		-- ImTerm
    group ""
