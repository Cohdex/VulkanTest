#include "pch.h"

#include "VulkanApplication.h"

namespace vkt
{
	VulkanApplication::~VulkanApplication()
	{

	}

	void VulkanApplication::run()
	{
		init();
		initApp();
		mainLoop();
		cleanup();
	}

	void VulkanApplication::init()
	{

	}

	void VulkanApplication::mainLoop()
	{

	}

	void VulkanApplication::cleanup()
	{

	}
}
