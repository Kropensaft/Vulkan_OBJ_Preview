#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>

// Forward-declare the main application class to avoid circular includes
class VulkanApplication;

// This is the data structure that will be copied to the GPU
struct DirectionalLightUBO {
  // Note: Use vec4 for alignment purposes in UBOs, even for 3D vectors.
  // The 'w' component is padding and can be ignored in the shader.
  alignas(16) glm::vec4 direction;
  alignas(16) glm::vec4 color;
};

class DirectionalLight {
public:
  // Constructor takes a reference to the main app to access Vulkan handles
  DirectionalLight(
      VulkanApplication &app,
      glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, -2.0f),
      glm::vec4 lightColor = glm::vec4(1.0f, 0.f, 0.0f, 1.0f));

  // Destructor cleans up the Vulkan buffer and memory
  ~DirectionalLight();
  void updateDirection(glm::vec3 newDirection);

  // Delete copy constructors to prevent accidental copying
  DirectionalLight(const DirectionalLight &) = delete;
  DirectionalLight &operator=(const DirectionalLight &) = delete;

  // Provides the buffer info needed to write to a descriptor set
  VkDescriptorBufferInfo getDescriptorInfo() const;

private:
  void createUniformBuffer();

  VulkanApplication &app;    // Reference to the main application
  DirectionalLightUBO ubo{}; // The CPU-side data

  // Vulkan handles for the buffer on the GPU
  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;
};

#endif
