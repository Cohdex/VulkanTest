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
		virtual void onInit() override;
		virtual void onCleanup() override;
		virtual void onFrame() override;

	private:
	};
}
