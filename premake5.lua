workspace "VulkanTest"
	architecture "x86_64"
	configurations { "Debug", "Develop", "Release" }
	startproject "VulkanTest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VulkanTest"
	location "VulkanTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "VulkanTest/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src/",
		"C:/VulkanSDK/1.1.121.0/Include/",
		"%{prj.name}/extlib/GLFW/include/",
		"%{prj.name}/extlib/GLM/include/"
	}

	libdirs {
		"C:/VulkanSDK/1.1.121.0/Lib/",
		"%{prj.name}/extlib/GLFW/"
	}

	links {
		"vulkan-1.lib",
		"glfw3"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "VKT_DEBUG"
		symbols "On"
		optimize "Off"
		runtime "Debug"
		
	filter "configurations:Develop"
		defines "VKT_DEVELOP"
		symbols "Off"
		optimize "On"
		runtime "Release"

	filter "configurations:Release"
		defines "VKT_RELEASE"
		symbols "Off"
		optimize "Full"
		runtime "Release"
