#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP
#include "Window.h"
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanContext {
public:
  VulkanContext(Window &window);
  ~VulkanContext();

  VulkanContext(const VulkanContext &) = delete;
  VulkanContext &operator=(const VulkanContext &) = delete;

  VkDevice getDevice() const { return device; }
  VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
  VkInstance getInstance() const { return instance; }
  VkSurfaceKHR getSurface() const { return surface; }
  VkQueue getGraphicsQueue() const { return graphicsQueue; }

private:
  void createInstance();
  void createSurface(Window &window);
  void pickPhysicalDevice();
  void createLogicalDevice();

  VkInstance instance = VK_NULL_HANDLE;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device = VK_NULL_HANDLE;
  VkQueue graphicsQueue = VK_NULL_HANDLE;
};

#endif //! VULKAN_CONTEXT_HPP
