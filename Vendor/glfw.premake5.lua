project "glfw"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	architecture "x86_64"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

	includedirs { "glfw/include" }

	files {
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c"
	}

	filter "system:Windows"
		systemversion "latest"

		files {
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines {
			"_GLFW_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter "system:Mac"
		pic "On"
		systemversion "latest"

		files {
			"glfw/src/cocoa_init.m",
			"glfw/src/cocoa_monitor.m",
			"glfw/src/cocoa_window.m",
			"glfw/src/cocoa_joystick.m",
			"glfw/src/cocoa_time.c",
			"glfw/src/poix_thread.c",
			"glfw/src/nsgl_context.m",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines {
			"_GLFW_MAC"
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