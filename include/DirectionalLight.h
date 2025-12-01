#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>

class VulkanApplication;

struct DirectionalLightUBO {
  alignas(16) glm::vec4 direction;
  alignas(16) glm::vec4 color;
  alignas(16) glm::mat4 lightSpaceMatrix;
};

constexpr glm::vec3 WHITE(1.f, 1.f, 1.f);
constexpr glm::vec3 RED(1.f, 0.f, 0.f);
constexpr glm::vec3 BLUE(0.f, 0.f, 1.f);
constexpr glm::vec3 GREEN(0.f, 1.f, 0.f);
constexpr glm::vec3 OFFWHITE(0.5f, 0.5f, 0.5f);

class DirectionalLight {
public:
  DirectionalLight(
      VulkanApplication &app,
      glm::vec3 lightDirection = glm::vec3(-1.f, -1.0f, -1.f),
      glm::vec4 lightColor = glm::vec4(WHITE, 1.f));

  ~DirectionalLight();

  void updateDirection(glm::vec3 newDirection);
  void updateLightSpaceMatrix(glm::mat4 mat);
  glm::vec3 getDirection() const;

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
