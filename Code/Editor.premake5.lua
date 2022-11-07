project "Editor"
    kind "StaticLib"

    pchheader "sadpch.h"
    pchsource "sadpch.cpp"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Editor/**.h", "Editor/**.cpp", "*.cpp" }

    vpaths {
        ["Headers"] = { "Editor/**.h", "Editor/**.hpp" },
        ["Sources/*"] = { "Editor/**.c", "Editor/**.cpp" },
        ["Documentation"] = { "Editor/**.md", "Editor/**.txt" },
        ["Resources"] = { "**pch.cpp" },
    }

    local includes = {
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/imgui",
		"%{prj.location}/../Vendor/ImTerm/include/",
        "%{prj.location}/../Vendor/glm",
        "%{prj.location}/../Vendor/glad/include",
        "%{prj.location}/../Vendor/assimp/include",
        "%{prj.location}",
    }

    if os.target() == "windows" then
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/win")
    else
        table.insert(includes, "%{prj.location}/../Vendor/SDL/include/mac")
    end

    includedirs { includes }

    links {
        "imgui",
    }

	filter "system:windows"
		defines {
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter "system:macosx"
        defines {
	        "_MAC"
		}
    filter {}
