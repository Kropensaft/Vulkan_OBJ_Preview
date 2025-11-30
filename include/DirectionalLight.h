#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>

class VulkanApplication;

struct DirectionalLightUBO {
  alignas(16) glm::vec4 direction;
  alignas(16) glm::vec4 color;
};

class DirectionalLight {
public:
  DirectionalLight(
      VulkanApplication &app,
      glm::vec3 lightDirection = glm::vec3(-1.f, -1.0f, -1.f),
      glm::vec4 lightColor = glm::vec4(1.0f, 0.f, 0.0f, 1.f));

  ~DirectionalLight();
  void updateDirection(glm::vec3 newDirection);

  DirectionalLight(const DirectionalLight &) = delete;
  DirectionalLight &operator=(const DirectionalLight &) = delete;

  VkDescriptorBufferInfo getDescriptorInfo() const;

private:
  void createUniformBuffer();

  VulkanApplication &app;
  DirectionalLightUBO ubo{};

  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;
};

#endif
