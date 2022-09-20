project "stb_image"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	architecture "x86_64"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

	files {
		"stb_image/stb_image.h",
		"stb_image/stb_image.cpp"
	}

	filter "system:windows"
		defines {
			"_STB_IMAGE_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter "system:macosx"
		pic "On"
		defines {
			"_STB_IMAGE_MAC"
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
