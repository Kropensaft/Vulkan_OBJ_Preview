#include "VulkanApplication.h"
#include <iostream>
#include <new>

int main() {

  try {
    VulkanApplication::getInstance().initVulkan();
    VulkanApplication::getInstance().run();
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
