project "SDL"
    kind "None" -- Placeholder project for SDL defines
    systemversion "latest"

	targetdir "%{wks.location}/Build/Bin/%{prj.name}"
	objdir "%{wks.location}/Build/Obj/%{prj.name}"

    filter "system:Windows"
        defines {
            "_SDL_WINDOWS"
        }
    filter "system:Mac"
        defines {
            "_SDL_MAC"
        }
    filter {}
