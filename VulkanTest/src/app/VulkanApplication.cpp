#include "pch.h"

#include "VulkanApplication.h"

namespace vkt
{
	VulkanApplication::VulkanApplication(const std::string& appName, int width, int height)
		: m_appName(appName), m_width(width), m_height(height)
	{
	}

	VulkanApplication::~VulkanApplication()
	{

	}

	void VulkanApplication::run()
	{
		init();
		mainLoop();
		cleanup();
	}

	void VulkanApplication::init()
	{
		initWindow();
		initVulkan();
		initApp();
	}

	void VulkanApplication::initWindow()
	{
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(m_width, m_height, m_appName.c_str(), nullptr, nullptr);
		if (m_window == nullptr)
			throw std::runtime_error("Failed to create window");
	}

	void VulkanApplication::initVulkan()
	{
		if (!glfwVulkanSupported())
			throw std::runtime_error("Vulkan is not supported");

		createInstance();
	}

	void VulkanApplication::createInstance()
	{
		auto extensionProperties = vk::enumerateInstanceExtensionProperties();
		std::cout << extensionProperties.size() << " available extensions:" << std::endl;
		for (const auto& extensionProperty : extensionProperties)
			std::cout << "\t" << extensionProperty.extensionName << std::endl;

		vk::ApplicationInfo appInfo(m_appName.c_str());
		vk::InstanceCreateInfo createInfo;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		std::cout << glfwExtensionCount << " enabled extensions:" << std::endl;
		for (unsigned i = 0; i < glfwExtensionCount; i++)
			std::cout << "\t" << glfwExtensions[i] << std::endl;

		auto validationLayers = vk::enumerateInstanceLayerProperties();
		unsigned maxLayerNameLength = 0;
		for (const auto& validationLayer : validationLayers)
		{
			unsigned layerNameLength = static_cast<unsigned>(std::strlen(validationLayer.layerName));
			if (layerNameLength > maxLayerNameLength)
				maxLayerNameLength = layerNameLength;
		}
		std::cout << validationLayers.size() << " available validation layers:" << std::endl;
		for (const auto& validationLayer : validationLayers)
			std::cout << "\t" << std::left << std::setw(maxLayerNameLength) << validationLayer.layerName << " | " << validationLayer.description << " [" << validationLayer.implementationVersion << "]" << std::endl;

#ifdef VKT_DEBUG
		std::array<const char*, 1> enabledValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		createInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());
		createInfo.ppEnabledLayerNames = enabledValidationLayers.data();
#endif

		if (vk::createInstance(&createInfo, nullptr, &m_instance) != vk::Result::eSuccess)
			throw std::runtime_error("Failed to create instance");
	}

	void VulkanApplication::mainLoop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
		}
	}

	void VulkanApplication::cleanup()
	{
		m_instance.destroy();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}
