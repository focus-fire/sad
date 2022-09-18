project "Engine" 
    kind "StaticLib"
    staticruntime "on"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files { "Engine/**.h", "Engine/**.cpp" }

    includedirs {
        "%{prj.location}/../Vendor/glfw/include",
        "%{prj.location}/../Vendor/glad/include",
        "%{prj.location}/../Vendor/spdlog/include",
        "%{prj.location}/../Vendor/glm",
        "%{prj.location}/../Vendor/stb_image",
        "%{prj.location}/../Vendor/imgui",
        "%{prj.location}/../Vendor/SDL/include",
        "%{prj.location}",
    }

    libdirs {
        "%{prj.location}/../Vendor/SDL/lib"
    }

	links {
		"glfw",
        "glad",
        "stb_image",
        "imgui",
        "SDL2", -- Lib
	}

    vpaths {
        ["Headers"] = { "Engine/**.h", "Engine/**.hpp" },
        ["Sources/*"] = { "Engine/**.c", "Engine/**.cpp" },
        ["Documentation"] = { "Engine/**.md", "Engine/**.txt" },
    }

	filter "system:Windows"
		defines { "_WINDOWS" }
    filter "system:Mac"
        defines { "_MAC" }
    filter {}
