project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	architecture "x86_64"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

	includedirs { "glad/include" }
	files { "glad/src/glad.c" }

	filter "system:windows"
		defines {
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter "system:macosx"
		pic "On"
		defines { 
			"_GLAD_MAC" 
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
