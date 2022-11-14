project "SadCSFramework"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    -- SadCSFramework has all of the utilities exposed by the engine into scripting
    -- This is why it's exposed like an asset or unit of data in the data directory
	targetdir "%{wks.location}/Data/Resources/%{prj.name}"
	objdir "%{wks.location}/Data/Resources/%{prj.name}/Obj"

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
