project "SadCSFramework"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

	targetdir "%{prj.location}/Build/Scripts/"
	objdir "%{prj.location}/Build/Scripts/Obj/%{prj.name}"

    files { "Sad/**.cs" }

    vpaths {
        ["Source/*"] = { "Sad/**.cs" },
        ["Docs"] = { "**.md", "**.txt", "Sad/**.md", "Sad/**.txt" },
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
