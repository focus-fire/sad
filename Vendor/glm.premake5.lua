project "glm"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

	targetdir "%{wks.location}/../Build/%{prj.name}"
	objdir "%{wks.location}/../Obj/%{prj.name}"

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
		systemversion "latest"
		staticruntime "On"

		defines {
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
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