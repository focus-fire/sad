project "SadScripting"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

	targetdir "%{prj.location}/Build/Scripts"
	objdir "%{prj.location}/Build/Scripts/Obj/%{prj.name}"

    files { "Source/**.cs" }

    vpaths {
        ["Source/*"] = { "Source/**.cs" },
        ["Docs"] = { "**.md", "**.txt", "Source/**.md", "Source/**.txt" },
    }

    filter "system:windows"
        defines {
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter "system:macosx"
        defines {
			"_MAC"
		}
    filter {}
