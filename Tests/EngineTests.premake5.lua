project "EngineTests"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/Tests/"
	objdir "%{wks.location}/Build/Obj/Tests/%{prj.name}"

    defines { "_TEST" }

    files { "EngineTests/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/Catch2/single_include/",
        "%{prj.location}/../Code/Engine/",
    }

    vpaths {
        ["Headers"] = { "**.h", "**.hpp" },
        ["Sources/*"] = { "**.c", "**.cpp" },
        ["Docs"] = { "**.md", "**.txt" },
    }

    links {
        "Engine",
    }