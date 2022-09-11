project "Game"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Game/**.h", "Game/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}"
    }

    vpaths {
        ["Headers"] = { "Game/**.h", "Game/**.hpp" },
        ["Sources/*"] = { "Game/**.c", "Game/**.cpp" },
        ["Documentation"] = { "Game/**.md", "Game/**.txt" },
    }

    links {
        "Engine",
    }

	filter "system:windows"
		defines { "_WINDOWS" }
    filter {}