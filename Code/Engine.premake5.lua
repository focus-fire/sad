project "Engine"
    kind "StaticLib"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    pchheader "sadpch.h"
    pchsource "sadpch.cpp"

    files { "Engine/**.h", "Engine/**.cpp", "*.cpp" }

    vpaths {
        ["Headers/*"] = { "Engine/**.h", "Engine/**.hpp" },
        ["Sources/*"] = { "Engine/**.c", "Engine/**.cpp" },
        ["Docs"] = { "Engine/**.md", "Engine/**.txt" },
        ["Resources"] = { "**pch.cpp" },
    }

    -- Manually resolve includes and links by target platform
    -- The following logic checks the premake target OS and matches it to the appropriate includes
    -- Warning: Using table.insert() and typical lua logic in normal filters may cause unexpected behavior
    local includes = {
        "%{prj.location}/../Vendor/glad/include",
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/json/single_include",
        "%{prj.location}/../Vendor/entt/single_include",
        "%{prj.location}/../Vendor/glm",
        "%{prj.location}/../Vendor/stb_image",
        "%{prj.location}/../Vendor/imgui",
        "%{prj.location}",
    }

    local linkers = {
        "Core",
        "Editor",
        "glad",
        "stb_image",
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
		defines {
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter "system:macosx"
        defines { "_MAC" }
    filter {}
