project "Catch2"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    architecture "x86_64"

    targetdir "%{wks.location}/Build/Bin/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/Build/Obj/%{prj.name}/%{cfg.buildcfg}"
    
    includedirs {
        "Catch2/single_include/"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "_CATCH2_WIN32",
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