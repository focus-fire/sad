project "CodeTests"
    kind "ConsoleApp"
    dependson { "Catch2", "Engine" }

	targetdir "%{wks.location}/Build/Bin/Tests/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/Tests/%{prj.name}"

    defines { "_TEST" }

    files { "**.cpp" }

    includedirs {
        "%{prj.location}/../../Vendor/Catch2/single_include/",
        "%{prj.location}/../../Code/Game/",
    }

    vpaths {
        ["Headers"] = { "**.h", "**.hpp" },
        ["Sources/*"] = { "**.c", "**.cpp" },
        ["Documentation"] = { "**.md", "**.txt" },
    }

    links {
        "Game",
    }