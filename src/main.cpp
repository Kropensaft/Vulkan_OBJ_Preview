#include "VulkanApplication.h"
#include <iostream>

int main() {
  VulkanApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
