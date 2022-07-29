workspace "Windflower"
    architecture "x86_64"
    configurations {"Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project "Windflower"
    kind "StaticLib"
    language "C"
    cdialect "C99"

    buildoptions {"-Wall", "-Wconversion", "-Werror"}

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
    }

    includedirs {
        "%{prj.name}/include",
        "%{prj.name}/src",
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "wftool"
    kind "ConsoleApp"
    language "C"
    cdialect "C99"

    buildoptions {"-Wall", "-Wconversion", "-Werror"}

    links { "Windflower" }

    sysincludedirs {
        "Windflower/include",
    }

    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir .. "/WfTool")

    files {
        "WfTool/src/**.h",
        "WfTool/src/**.c",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"