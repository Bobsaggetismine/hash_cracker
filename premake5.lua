workspace "hash_cracker"
    architecture "x64"
    startproject "cpu_benchmark"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["BOOST"] = "/usr/include/boost"
project "app"
	location "app"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	links
	{
		"app_lib"
	}

	files
	{
		"%{prj.name}/source/**.hpp",
		"%{prj.name}/source/**.cpp",
    }
    includedirs
	{
        "%{prj.name}/source/",
		"%{IncludeDir.openssl}",
		"app_lib/source"
	}

	filter "system:windows"
		systemversion "latest"
		defines{ "PLATFORM_WINDOWS" }
	
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
 
		
project "test"
	location "test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")


	
	links
	{
		"app_lib",
		"boost_unit_test_framework"
	}
	
	files
	{
		"%{prj.name}/source/**.hpp",
		"%{prj.name}/source/**.cpp",
    }
    includedirs
	{
        "%{prj.name}/source/",
		"app_lib/source",
		"%{IncludeDir.BOOST}"
	}

	filter "system:windows"
		systemversion "latest"
		defines{ "PLATFORM_WINDOWS" }
	
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
project "app_lib"
	location "app_lib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/source/**.hpp",
		"%{prj.name}/source/**.cpp",
    }
    includedirs
	{
        "%{prj.name}/source/",
	}

	filter "system:windows"
		systemversion "latest"
		defines{ "PLATFORM_WINDOWS" }
	
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"
        


