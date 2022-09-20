project "Game"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Game/**.h", "Game/**.cpp" }

    vpaths {
        ["Headers"] = { "Game/**.h", "Game/**.hpp" },
        ["Sources/*"] = { "Game/**.c", "Game/**.cpp" },
        ["Documentation"] = { "Game/**.md", "Game/**.txt" },
    }

    -- Manually resolve includes by target platform
    -- The following logic checks the premake target OS and matches it to the appropriate includes
    -- Warning: Using table.insert() and typical lua logic in normal filters may cause unexpected behavior 
    local includes = {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}",
    }

    if os.target() == "windows" then
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/win")
    else
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/mac")
    end

    includedirs { includes }

    -- Linking SDL
    -- Windows (.libs/.dll)
    -- Mac (.framework)
    filter "system:windows"
        libdirs { "%{prj.location}/../Vendor/SDL/lib/win" }
        links { 
            "Engine", 
            "SDL2", 
        }
    filter "system:macosx"
        frameworkdirs { "%{prj.location}/../Vendor/SDL/lib/mac" }
        links { 
            "Engine", 
            "SDL2.framework", 
        }
    filter {}

	filter "system:windows"
		defines { "_WINDOWS" }
        postbuildcommands {
            -- Copy the SDL .dll to the application directory where it can be found at runtime
            "{COPY} %{wks.location}/Vendor/SDL/lib/win/*.dll $(OutDir)", 
        }
    filter "system:macosx"
        defines { "_MAC" }
    filter {}
