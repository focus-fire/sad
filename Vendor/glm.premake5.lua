project "glm"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
	staticruntime "On"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

	includedirs { 
		"glm/" 
	}

	files { 
		"glm/glm/**" 
	}

	defines {
		"GLM_FORCE_SWIZZLE"
	}

	filter "system:windows"
		defines {
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
	filter "system:macosx"
		pic "On"
		defines {
			"_GLM_MAC"
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