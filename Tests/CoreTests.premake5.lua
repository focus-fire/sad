project "CoreTests"
    kind "ConsoleApp"
    dependson { "Core" }

	targetdir "%{wks.location}/Build/Bin/Tests/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/Tests/%{prj.name}"

    defines { "_CORE_TEST" }

    files { "CoreTests/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/Catch2/single_include/",
        "%{prj.location}/../Code/",
        -- Core Includes
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/glm",
    }


    vpaths {
        ["Headers"] = { "CoreTests/**.h", "CoreTests/**.hpp" },
        ["Sources/*"] = { "CoreTests/**.c", "CoreTests/**.cpp" },
        ["Docs"] = { "CoreTests/**.md", "CoreTests/**.txt" },
    }

    links {
        "Core",
		"Engine"
    }
