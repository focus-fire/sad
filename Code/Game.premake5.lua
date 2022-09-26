project "Game"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Game/**.h", "Game/**.cpp" }

    vpaths {
        ["Headers"] = { "Game/**.h", "Game/**.hpp" },
        ["Sources/*"] = { "Game/**.c", "Game/**.cpp" },
        ["Docs"] = { "Game/**.md", "Game/**.txt" },
    }

    -- Manually resolve includes and links by target platform
    -- The following logic checks the premake target OS and matches it to the appropriate includes
    -- Warning: Using table.insert() and typical lua logic in normal filters may cause unexpected behavior 
    local includes = {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/json/single_include",
        "%{prj.location}/../Vendor/entt/single_include",
        "%{prj.location}",
    }

    local linkers = {
        "Engine",
        "glad",
        "stb_image",
        "imgui",
    }

    if os.target() == "windows" then
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/win")
        table.insert(linkers, "SDL2") -- .dll
    else
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/mac")
        table.insert(linkers, "SDL2.framework")
    end

    includedirs { includes }

    links { linkers }

    filter "system:windows"
        libdirs { "%{prj.location}/../Vendor/SDL/lib/win" }
    filter "system:macosx"
        frameworkdirs { "%{prj.location}/../Vendor/SDL/lib/mac" }
    filter {}

	filter "system:windows"
		defines { "_WINDOWS" }
        postbuildcommands {
            -- Copy the SDL .dll to the application directory where it can be found at runtime
            "{COPY} %{wks.location}/Vendor/SDL/lib/win/*.dll $(OutDir)", 
        }
    filter "system:macosx"
        defines { "_MAC" }
        postbuildcommands {
            -- Copy the SDL framework to the local framework directory where it can be found at runtime
            "{COPY} %{wks.location}/Vendor/SDL/lib/mac/SDL2.framework ~/Library/Frameworks"
        }
    filter {}
