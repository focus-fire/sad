project "SadProject"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    -- Project bundles all the scripts required to run the game into an assembly
    -- Think of this as Unity's 'Scripts' folder
	targetdir "%{wks.location}/Data/Project/Resources/%{prj.name}"
	objdir "%{wks.location}/Data/Project/Resources/%{prj.name}/Obj"

    -- Any script located in the project's 'Assets' folder should be included
    files {
		"%{wks.location}/Data/Project/Assets/**.cs",
		"%{wks.location}/Data/Project/Assets/Scripts/**.cs"
	}

    vpaths {
        ["Source/*"] = { "%{wks.location}/Data/Project/Assets/**.cs" },
        ["Docs"] = { "%{wks.location}/Data/Project/Assets/**.md", "%{wks.location}/Data/Project/Assets/**.txt" },
    }

    links {
        "SadCSFramework"
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
