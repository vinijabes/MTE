workspace "Kinematics"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Kinematics/vendor/GLFW/include"
IncludeDir["Glad"] = "Kinematics/vendor/Glad/include"
IncludeDir["ImGui"] = "Kinematics/vendor/imgui"
IncludeDir["glm"] = "Kinematics/vendor/glm"
IncludeDir["stb_image"] = "Kinematics/vendor/stb_image"

group "Dependencies"
    include "Kinematics/vendor/GLFW"
    include "Kinematics/vendor/Glad"

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
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"        
        staticruntime "On"
        systemversion "latest"

    defines
    {
        "KINEMATICS_BUILD_DLL",
        "GLFW_INCLUDE_NONE"
    }

    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
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