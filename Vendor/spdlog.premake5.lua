project "spdlog"
    kind "StaticLib"
    language "C"
    staticruntime "On"
    architecture "x86_64"

    targetdir "%{wks.location}/Build/%{prj.name}/%{cfg.buildcfg}"
    objdir "%{wks.location}/Obj/%{prj.name}/%{cfg.buildcfg}"

    includedirs {
        "spdlog/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "_SPDLOG_WIN32",
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