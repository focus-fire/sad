project "mono"
    kind "None" -- Placeholder project for mono defines
    systemversion "latest"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    filter "system:windows"
        defines {
            "_ASSIMP_WINDOWS"
        }
    filter "system:macosx"
        defines {
            "_ASSIMP_MAC"
        }
    filter {}
