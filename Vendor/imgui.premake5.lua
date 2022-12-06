project "imgui"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    architecture "x86_64"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    local filers = {
        "imgui/*.cpp",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_sdl.cpp",
    }

    local includes = {
        "imgui/",
        "glad/include",
    }

    -- ImGui requires includes from GLAD and SDL
    -- Add OS specific implementations of ImGui and SDL
    if os.target() == "windows" then
        table.insert(filers, "imgui/backends/imgui_impl_win32.cpp")
        table.insert(includes, "SDL/include/win")
        table.insert(includes, "SDL/include/win/SDL2")
    else
        table.insert(filers, "imgui/backends/imgui_impl_osx.mm")
        table.insert(includes, "SDL/include/mac")
        table.insert(includes, "SDL/include/mac/SDL2")
    end

    files { filers }

    includedirs { includes }

    defines {
        "IMGUI_IMPL_OPENGL_LOADER_GLAD"
    }

    filter "system:windows"
        defines {
            "_IMGUI_WIN32",
            "_CRT_SECURE_NO_WARNINGS",
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
