project "Game"
    kind "WindowedApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    pchheader "sadpch.h"
    pchsource "sadpch.cpp"

    files { "Game/**.h", "Game/**.cpp", "*.cpp" }

    vpaths {
        ["Headers"] = { "Game/**.h", "Game/**.hpp" },
        ["Sources/**"] = { "Game/**.c", "Game/**.cpp" },
        ["Docs"] = { "Game/**.md", "Game/**.txt" },
        ["Resources"] = { "**pch.cpp" },
    }

    -- Manually resolve includes and links by target platform
    -- The following logic checks the premake target OS and matches it to the appropriate includes
    -- Warning: Using table.insert() and typical lua logic in normal filters may cause unexpected behavior
    local includes = {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/glad/include",
        "%{prj.location}/../Vendor/glm",
        "%{prj.location}/../Vendor/json/single_include",
        "%{prj.location}/../Vendor/entt/single_include",
        "%{prj.location}/../Vendor/imgui",
        "%{prj.location}/../Vendor/ImTerm/include",
        "%{prj.location}/../Vendor/assimp/include",
        "%{prj.location}/../Vendor/mono/include",
		"%{prj.location}/../Vendor/filewatch/",
        "%{prj.location}",
    }

    local linkers = {
        "Core",
        "Engine",
        "Editor",
        "glad",
        "stb_image",
        "imgui",
		"ImGuizmo",

		-- CS Projects
		"SadProject",
		"SadCSFramework",
    }

    if os.target() == "windows" then
        -- Win32
        table.insert(linkers, "Ws2_32") -- .lib
        table.insert(linkers, "winmm") -- .lib
        table.insert(linkers, "version") -- .lib
        table.insert(linkers, "Bcrypt") -- .lib

        -- SDL
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/win")
        table.insert(linkers, "SDL2") -- .dll
		table.insert(linkers, "SDL2_mixer") -- .dll

        -- Assimp
		table.insert(linkers, "assimp-vc143-mt") -- .dll

        -- Mono
        table.insert(linkers, "libmono-static-sgen") -- .lib
    else
        -- Mac
        table.insert(linkers, "Foundation.framework") -- .framework
        table.insert(linkers, "iconv") -- .a
        table.insert(linkers, "z") -- .a

        -- SDL
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/mac")
        table.insert(linkers, "SDL2.framework") -- .framework
		table.insert(linkers, "SDL2_mixer.framework") -- .framework

        -- Assimp
        table.insert(linkers, "assimp.framework") -- .framework

        -- Mono
        table.insert(linkers, "monosgen-2.0") -- .a
    end

    includedirs { includes }

    links { linkers }

    filter "system:windows"
        libdirs {
			"%{prj.location}/../Vendor/SDL/lib/win",
			"%{prj.location}/../Vendor/assimp/lib/win",
            "%{prj.location}/../Vendor/mono/lib/win/%{cfg.buildcfg}"
		}
    filter "system:macosx"
        frameworkdirs {
			"%{prj.location}/../Vendor/SDL/lib/mac",
			"%{prj.location}/../Vendor/assimp/lib/mac",
		}
        libdirs {
            "%{prj.location}/../Vendor/mono/lib/mac",
        }
    filter "system:windows"
        defines {
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
        postbuildcommands {
            -- Copy the SDL .dll to the application directory
            "{COPY} %{wks.location}/Vendor/SDL/lib/win/*.dll $(OutDir)",

			-- Copy the assimp .dll to the application directory
			"{COPY} %{wks.location}/Vendor/assimp/lib/win/*.dll $(OutDir)",
        }
    filter "system:macosx"
        defines {
			"_MAC"
		}
        -- Copy required mac frameworks to local framework directory before linking begins
        prebuildcommands {
            -- Copy the SDL frameworks to the local framework directory
            "{COPY} %{wks.location}/Vendor/SDL/lib/mac/SDL2.framework ~/Library/Frameworks",
            "{COPY} %{wks.location}/Vendor/SDL/lib/mac/SDL2_mixer.framework ~/Library/Frameworks",

            -- Copy the assimp framework to the local framework directory
            "{COPY} %{wks.location}/Vendor/assimp/lib/mac/assimp.framework ~/Library/Frameworks",
        }
    filter {}
