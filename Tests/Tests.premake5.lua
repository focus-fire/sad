project "Tests"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    defines { "_TEST" }

    files { "**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/Catch2/single_include/",
        "%{prj.location}/../Code/Engine/",
    }

    vpaths {
        ["Headers"] = { "**.h", "**.hpp" },
        ["Sources/*"] = { "**.c", "**.cpp" },
        ["Documentation"] = { "**.md", "**.txt" },
    }

    links {
        "Engine",
    }

    filter "system:Windows"
        defines { "_WINDOWS" }
    filter "system:Mac"
        defines { "_MAC" }
    filter {}