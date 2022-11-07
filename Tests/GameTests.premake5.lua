project "GameTests"
    kind "ConsoleApp"
    dependson { "Game" }

	targetdir "%{wks.location}/Build/Bin/Tests/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/Tests/%{prj.name}"

    defines { "_GAME_TEST" }

    files { "GameTests/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/Catch2/single_include/",
        "%{prj.location}/../Code/",
        -- Game Includes
        "%{prj.location}/../Vendor/spdlog/include",
    }

    vpaths {
        ["Headers"] = { "GameTests/**.h", "GameTests/**.hpp" },
        ["Sources/*"] = { "GameTests/**.c", "GameTests/**.cpp" },
        ["Docs"] = { "GameTests/**.md", "GameTests**.txt" },
    }

    links {
		"Core",
        "Game",
    }
