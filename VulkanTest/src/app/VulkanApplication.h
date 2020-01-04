#pragma once

#include <vulkan/vulkan.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace vkt
{
	class VulkanApplication
	{
	public:
		VulkanApplication(const std::string& appName, int width, int height);
		virtual ~VulkanApplication();

		const std::string& getAppName() const { return m_appName; }

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		void run();

	protected:
		virtual void onInit() { }
		virtual void onCleanup() { }
		virtual void onFrame() = 0;

	private:
		std::string m_appName;
		int m_width;
		int m_height;

		GLFWwindow* m_window = nullptr;

		vk::Instance m_instance;
		vk::SurfaceKHR m_surface;
		vk::PhysicalDevice m_physicalDevice;
		vk::Device m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;

		void init();
		void initWindow();
		void initVulkan();
		void createInstance();
		void createSurface();
		void createPhysicalDevice();
		void createDevice();

		void cleanup();

		void mainLoop();
	};
}
