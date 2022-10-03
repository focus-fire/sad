project "Core"
    kind "StaticLib"

    pchheader "sadpch.h"
    pchsource "sadpch.cpp"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Core/**.h", "Core/**.cpp", "*.cpp" }

    vpaths {
        ["Headers"] = { "Core/**.h", "Core/**.hpp" },
        ["Sources/*"] = { "Core/**.c", "Core/**.cpp" },
        ["Documentation"] = { "Core/**.md", "Core/**.txt" },
        ["Resources"] = { "**pch.cpp" },
    }

    -- Realistically the 'Core' library should only be creating standard utils
    -- Only link/include library headers as needed here...
    includedirs {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/glad/include", 
        "%{prj.location}",
    }

    links {
        "glad",
    }

	filter "system:windows"
		defines { 
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter "system:macosx"
        defines { "_MAC" }
    filter {}
