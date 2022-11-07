project "EngineTests"
    kind "ConsoleApp"
    dependson { "Engine" }

	targetdir "%{wks.location}/Build/Bin/Tests/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/Tests/%{prj.name}"

    defines { "_ENGINE_TEST" }

    files { "EngineTests/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/Catch2/single_include/",
        "%{prj.location}/../Code/",
        -- Engine Includes
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/glm",
        "%{prj.location}/../Vendor/entt/single_include",
    }

    vpaths {
        ["Headers"] = { "EngineTests/**.h", "EngineTests/**.hpp" },
        ["Sources/*"] = { "EngineTests/**.c", "EngineTests/**.cpp" },
        ["Docs"] = { "EngineTests/**.md", "EngineTests/**.txt" },
    }

    links {
		"Core",
        "Engine",
    }
