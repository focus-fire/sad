project "Core"
    kind "StaticLib"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Core/**.h", "Core/**.cpp" }

    vpaths {
        ["Headers"] = { "Core/**.h", "Core/**.hpp" },
        ["Sources/*"] = { "Core/**.c", "Core/**.cpp" },
        ["Documentation"] = { "Core/**.md", "Core/**.txt" },
    }

    -- Realistically the 'Core' library should only be creating standard utils
    -- Only link/include library headers as needed here...
    includedirs {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}",
    }

	filter "system:windows"
		defines { "_WINDOWS" }
    filter "system:macosx"
        defines { "_MAC" }
    filter {}
