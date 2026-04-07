/**
 * @file VulkanContext.hpp
 * @brief Class responsible for the core Vulkan API initialization.
 */
#ifndef VULKAN_CONTEXT_HPP
#define VULKAN_CONTEXT_HPP
#include "Window.h"
#include <cstring>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

/**
 * @class VulkanContext
 * @brief Handles the creation and destruction of the Vulkan instance,
 * physical/logical devices, and queues.
 */
class VulkanContext {
public:
  /**
   * @brief Constructor that initializes the entire Vulkan context.
   * @param window Reference to the application window needed for surface
   * creation.
   */
  VulkanContext(Window &window);

  /** @brief Destructor that cleans up the logical device, surface, and
   * instance. */
  ~VulkanContext();

  VulkanContext(const VulkanContext &) = delete;
  VulkanContext &operator=(const VulkanContext &) = delete;

  /** @brief Retrieves the logical Vulkan device. */
  VkDevice getDevice() const { return device; }

  /** @brief Retrieves the chosen physical Vulkan device (GPU). */
  VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }

  /** @brief Retrieves the Vulkan instance. */
  VkInstance getInstance() const { return instance; }

  /** @brief Retrieves the Vulkan presentation surface. */
  VkSurfaceKHR getSurface() const { return surface; }

  /** @brief Retrieves the graphics queue. */
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
