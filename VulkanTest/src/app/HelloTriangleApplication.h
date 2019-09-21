#pragma once

#include "VulkanApplication.h"

namespace vkt
{
	class HelloTriangleApplication : public VulkanApplication
	{
	public:
		HelloTriangleApplication(int width, int height);
		virtual ~HelloTriangleApplication();

	protected:
		virtual void initApp() override;

	private:
	};
}
