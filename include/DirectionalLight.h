/**
 * @file DirectionalLight.h
 * @brief Management of global directional lighting and shadows.
 */
#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include <memory>
#include <vulkan/vulkan.h>

class VulkanApplication;

/**
 * @struct DirectionalLightUBO
 * @brief Uniform Buffer Object structure for directional light data.
 */
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

/**
 * @class DirectionalLight
 * @brief Represents a global directional light source in the scene.
 */
class DirectionalLight {
public:
  /**
   * @brief Constructor for the DirectionalLight.
   * @param app Reference to the main VulkanApplication.
   * @param lightDirection Initial direction vector of the light.
   * @param lightColor Initial color and intensity of the light.
   */
  DirectionalLight(VulkanApplication &app,
                   glm::vec3 lightDirection = glm::vec3(-1.f, -1.0f, -1.f),
                   glm::vec4 lightColor = glm::vec4(WHITE, 1.f));

  ~DirectionalLight();

  /** @brief Updates the direction of the light and refreshes the UBO. */
  void updateDirection(glm::vec3 newDirection);

  /** @brief Updates the light space matrix used for shadow mapping. */
  void updateLightSpaceMatrix(glm::mat4 mat);

  /** @brief Retrieves the current direction of the light. */
  glm::vec3 getDirection() const;

  /** @brief Sets a new direction without immediately updating the UBO. */
  void setDirection(glm::vec3 newDirection);

  DirectionalLight(const DirectionalLight &) = delete;
  DirectionalLight &operator=(const DirectionalLight &) = delete;

  /** @brief Retrieves the Vulkan descriptor buffer info for this light's UBO.
   */
  VkDescriptorBufferInfo getDescriptorInfo() const;

private:
  void createUniformBuffer();

  VulkanApplication &app;
  DirectionalLightUBO ubo{};

  VkBuffer uniformBuffer;
  VkDeviceMemory uniformBufferMemory;
};

#endif
