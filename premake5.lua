workspace "Xash"
    architecture "x86_64"
    filter "action:vs*"
        buildoptions { "/utf-8" }
    filter {} 

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Core_Utils"
    location "Core_Utils"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/core/**.h",
        "%{prj.name}/src/core/**.cpp"
    }
    includedirs
    {
        "%{prj.name}/include"
    }
    pchheader "crpch.h"
    pchsource "%{prj.name}/src/crpch.cpp"

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "XS_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "XS_RELEASE"
        symbols "On"
    filter "configurations:Dist"
        defines "XS_DIST"
        symbols "On"

project "Xash"
    location "Xash"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "%{prj.name}/include",
        "Core_Utils/include"
    }
    pchheader "xspch.h"
    pchsource "%{prj.name}/src/xspch.cpp"
    links
    {
        "Core_Utils"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "XS_DEBUG"
        symbols "On"
    filter "configurations:Release"
        defines "XS_RELEASE"
        symbols "On"
    filter "configurations:Dist"
        defines "XS_DIST"
        symbols "On"