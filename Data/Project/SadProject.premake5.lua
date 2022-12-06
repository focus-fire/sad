project "SadProject"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    -- Project bundles all the scripts required to run the game into an assembly
    -- Think of this as Unity's 'Scripts' folder
	targetdir "%{prj.location}/Resources/%{prj.name}"
	objdir "%{prj.location}/Resources/%{prj.name}/Obj"

    -- Any script located in the project's 'Assets' folder should be included
    files {
		"%{prj.location}/Assets/**.cs",
		"%{prj.location}/Assets/Scripts/**.cs"
	}

    vpaths {
        ["Source/*"] = { "%{prj.location}/Assets/**.cs" },
        ["Docs"] = { "%{prj.location}/Assets/**.md", "%{prj.location}/Assets/**.txt" },
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
