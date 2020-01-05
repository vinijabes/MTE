workspace "Kinematics"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kinematics/vendor/GLFW/include"
IncludeDir["Glad"] = "Kinematics/vendor/Glad/include"
IncludeDir["ImGui"] = "Kinematics/vendor/imgui"
IncludeDir["glm"] = "Kinematics/vendor/glm"
IncludeDir["lua"] = "Kinematics/vendor/lua"
IncludeDir["stb_image"] = "Kinematics/vendor/stb_image"
IncludeDir["tinyxml2"] = "Kinematics/vendor/tinyxml2"

group "Dependencies"
    include "Kinematics/vendor/GLFW"
    include "Kinematics/vendor/Glad"
    include "Kinematics/vendor/tinyxml2"
    include "Kinematics/vendor/lua"

group ""

project "Kinematics"
    location "Kinematics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"    

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mtepch.h"
    pchsource "Kinematics/src/mtepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.lua}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.tinyxml2}"
    }

    links
    {
        "GLFW",
        "Glad",
        "tinyxml2",
        "lua"
    }    

    filter "system:linux"
--		pic "on"

		links 
		{ 
			"Xrandr",
			"Xi",
			"GLEW",
			"GLU",
			"GL",
			"X11"
		}

		defines
		{
            "KINEMATICS_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
        
        excludes
        {
            "%{prj.name}/src/Platform/Windows/**.h",
            "%{prj.name}/src/Platform/Windows/**.cpp",
        }


    filter "system:windows"        
        systemversion "latest"

        links 
		{ 
			"opengl32.lib"
		}

        defines
        {
            "KINEMATICS_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

        excludes
        {
            "%{prj.name}/src/Platform/Linux/**.h",
            "%{prj.name}/src/Platform/Linux/**.cpp",
        }

    filter "configurations:Debug"
        defines "KINEMATICS_DEBUG"
        runtime "Debug"        
        symbols "on"

    filter "configurations:Release"
        defines "KINEMATICS_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "KINEMATICS_DIST"
        runtime "Release"
        optimize "on"


project "Game"
    location "Game"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"        
    }

    includedirs
    {
        "Kinematics/vendor/spdlog/include",
        "Kinematics/src",
        "Kinematics/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Kinematics"
    }

    filter "system:windows"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "KINEMATICS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KINEMATICS_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "KINEMATICS_DIST"
        runtime "Release"
        optimize "on"        

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"        
    }

    includedirs
    {
        "Kinematics/vendor/spdlog/include",
        "Kinematics/src",
        "Kinematics/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Kinematics"
    }

    filter "system:linux"
		links
		{
			"GLFW",
			"Glad",
			"Xrandr",
			"Xi",
			"GLEW",
			"GLU",
			"GL",
			"X11",
			"dl",
			"pthread",
			"stdc++fs"
		}
		defines
		{
		}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "KINEMATICS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KINEMATICS_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "KINEMATICS_DIST"
        runtime "Release"
        optimize "on"

project "Client"
    location "Client"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"        
    }

    includedirs
    {
        "Kinematics/vendor/spdlog/include",
        "Kinematics/src",
        "Kinematics/vendor",
        "Game/src",
        "%{IncludeDir.glm}",
    }

    links
    {
        "Kinematics"
    }

    filter "system:linux"
		links
		{
			"GLFW",
			"Glad",
			"Xrandr",
			"Xi",
			"GLEW",
			"GLU",
			"GL",
			"X11",
			"dl",
			"pthread",
			"stdc++fs"
        }
        
		defines
		{
		}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "KINEMATICS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KINEMATICS_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "KINEMATICS_DIST"
        runtime "Release"
        optimize "on"

project "Server"
    location "Server"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"        
    }

    includedirs
    {
        "Kinematics/vendor/spdlog/include",
        "Kinematics/src",
        "Kinematics/vendor",
        "Game/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Kinematics"
    }

    filter "system:linux"
		links
		{
			"GLFW",
			"Glad",
			"Xrandr",
			"Xi",
			"GLEW",
			"GLU",
			"GL",
			"X11",
			"dl",
			"pthread",
			"stdc++fs"
        }
        
		defines
		{
		}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "KINEMATICS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KINEMATICS_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "KINEMATICS_DIST"
        runtime "Release"
        optimize "on"