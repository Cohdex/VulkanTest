#pragma once

#include "VulkanApplication.h"

namespace vkt
{
	class HelloTriangleApplication : public VulkanApplication
	{
	private:

	protected:
		virtual void initApp();

	public:
		HelloTriangleApplication() = default;
		virtual ~HelloTriangleApplication();
	};
}
