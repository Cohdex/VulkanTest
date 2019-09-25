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
		createPhysicalDevice();
		createDevice();
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

#ifdef VKT_DEBUG
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

		std::array<const char*, 1> enabledValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		std::cout << enabledValidationLayers.size() << " enabled validation layers:" << std::endl;
		for (const auto& validationLayer : enabledValidationLayers)
			std::cout << "\t" << validationLayer << std::endl;
		createInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());
		createInfo.ppEnabledLayerNames = enabledValidationLayers.data();
#endif

		m_instance = vk::createInstance(createInfo);
	}

	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsIndex;

		bool hasAllIndices()
		{
			return graphicsIndex.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& physicalDevice)
	{
		auto queueFamilies = physicalDevice.getQueueFamilyProperties();
		QueueFamilyIndices queueIndices;
		int index = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				queueIndices.graphicsIndex = index;
			index++;
			if (queueIndices.hasAllIndices())
				break;
		}
		return queueIndices;
	}

	static bool isPhysicalDeviceSuitable(const vk::PhysicalDevice& physicalDevice)
	{
		vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
		//vk::PhysicalDeviceFeatures deviceFeatures = physicalDevice.getFeatures();

		auto queueFamilies = findQueueFamilies(physicalDevice);

		return deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu
			&& queueFamilies.hasAllIndices();
	}

	void VulkanApplication::createPhysicalDevice()
	{
		auto physicalDevices = m_instance.enumeratePhysicalDevices();
		if (physicalDevices.empty())
			throw std::runtime_error("Failed to find a GPU with Vulkan support");

		bool suitableDeviceFound = false;
		for (const auto& physicalDevice : physicalDevices)
		{
			if (isPhysicalDeviceSuitable(physicalDevice))
			{
				m_physicalDevice = physicalDevice;
				suitableDeviceFound = true;
				break;
			}
		}

		if (!suitableDeviceFound)
			throw std::runtime_error("Failed to find a suitable Vulkan GPU");
	}

	void VulkanApplication::createDevice()
	{
		vk::DeviceQueueCreateInfo queueCreateInfo;
		QueueFamilyIndices queueIndices = findQueueFamilies(m_physicalDevice);
		queueCreateInfo.queueFamilyIndex = queueIndices.graphicsIndex.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.queueCreateInfoCount = 1;

		vk::PhysicalDeviceFeatures deviceFeatures;
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		m_device = m_physicalDevice.createDevice(deviceCreateInfo);
		m_graphicsQueue = m_device.getQueue(queueIndices.graphicsIndex.value(), 0);
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
		m_device.destroy();
		m_instance.destroy();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}
