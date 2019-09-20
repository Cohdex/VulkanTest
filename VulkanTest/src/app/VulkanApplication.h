#pragma once

namespace vkt
{
	class VulkanApplication
	{
	private:
		void init();
		void mainLoop();
		void cleanup();

	protected:
		virtual void initApp() { }

	public:
		VulkanApplication() = default;
		virtual ~VulkanApplication();

		void run();
	};
}
