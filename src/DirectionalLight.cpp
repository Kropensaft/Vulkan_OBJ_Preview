#include "DirectionalLight.h"
#include "VulkanApplication.h"
#include <stdexcept>

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("failed to find suitable memory type!");
}

DirectionalLight::DirectionalLight(VulkanApplication &app, glm::vec3 lightDirection, glm::vec4 lightColor)
    : app(app) {

  // NOTE:  Initialize the Uniform Buffer Object data
  ubo.direction = glm::normalize(glm::vec4(lightDirection, 0.0f));
  ubo.color = lightColor;

  createUniformBuffer();
}

DirectionalLight::~DirectionalLight() {
  vkDestroyBuffer(app.getDevice(), uniformBuffer, nullptr);
  vkFreeMemory(app.getDevice(), uniformBufferMemory, nullptr);
}

void DirectionalLight::createUniformBuffer() {
  VkDeviceSize bufferSize = sizeof(DirectionalLightUBO);

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = bufferSize;
  bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(app.getDevice(), &bufferInfo, nullptr, &uniformBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create uniform buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(app.getDevice(), uniformBuffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(app.getPhysicalDevice(), memRequirements.memoryTypeBits,
                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(app.getDevice(), &allocInfo, nullptr, &uniformBufferMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate uniform buffer memory!");
  }

  vkBindBufferMemory(app.getDevice(), uniformBuffer, uniformBufferMemory, 0);

  void *data;
  vkMapMemory(app.getDevice(), uniformBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, &ubo, bufferSize);
  vkUnmapMemory(app.getDevice(), uniformBufferMemory);
}

VkDescriptorBufferInfo DirectionalLight::getDescriptorInfo() const {
  return VkDescriptorBufferInfo{
      uniformBuffer,
      0,
      sizeof(DirectionalLightUBO)};
}

void DirectionalLight::setDirection(glm::vec3 newDirection) {
  ubo.direction = glm::vec4(glm::normalize(newDirection), 0.0f);
}

glm::vec3 DirectionalLight::getDirection() const {
  return glm::vec3(ubo.direction);
}

void DirectionalLight::updateLightSpaceMatrix(glm::mat4 matrix) {
  ubo.lightSpaceMatrix = matrix;

  void *data;
  VkDeviceSize bufferSize = sizeof(DirectionalLightUBO);
  vkMapMemory(app.getDevice(), uniformBufferMemory, 0, bufferSize, 0, &data);
  // NOTE: The UBO is fully replaced to ensure consistency
  memcpy(data, &ubo, bufferSize);
  vkUnmapMemory(app.getDevice(), uniformBufferMemory);
}

void DirectionalLight::updateDirection(glm::vec3 newDirection) {
  ubo.direction = glm::normalize(glm::vec4(newDirection, 0.0f));

  void *data;
  VkDeviceSize bufferSize = sizeof(DirectionalLightUBO);
  vkMapMemory(app.getDevice(), uniformBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, &ubo, bufferSize);
  vkUnmapMemory(app.getDevice(), uniformBufferMemory);
}
