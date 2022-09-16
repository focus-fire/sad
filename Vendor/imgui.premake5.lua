project "imgui"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    architecture "x86_64"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    files {
        "imgui/*.cpp",
    }

    includedirs {
        "imgui/",
        "glad/include",
        "glfw/include/",
    }

    defines {
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
    }

    filter "system:Windows"
        systemversion "latest"
        defines {
            "_IMGUI_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
    filter "system:Mac"
        pic "On"
        systemversion "latest"
        defines {
            "_IMGUI_MAC"
        }
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"
	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "On"
    filter {}