#include "pch.h"

#include "app/HelloTriangleApplication.h"

int main() {
	try
	{
		std::unique_ptr<vkt::VulkanApplication> app = std::make_unique<vkt::HelloTriangleApplication>();
		app->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
