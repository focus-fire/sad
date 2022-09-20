project "SDL"
    kind "None" -- Placeholder project for SDL defines
    systemversion "latest"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    filter "system:windows"
        defines {
            "_SDL_WINDOWS"
        }
    filter "system:macosx"
        defines {
            "_SDL_MAC"
        }
    filter {}
