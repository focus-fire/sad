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

    filter "system:windows"
        defines {
            "_IMGUI_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }
    filter "system:macosx"
        pic "On"
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