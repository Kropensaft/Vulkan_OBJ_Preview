#ifndef VULKAN_APP_H
#define VULKAN_APP_H

#include "CameraController.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // FIX: Was R32G32
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
};

struct UniformBufferObject {
  glm::mat4 proj;
  glm::mat4 view;
};
/*
INFO:
   v_idx Index into the vertex position list ('v')
   vt_idx Index into the texture coordinate list ('vt')
   vn_idx Index into the vertex normal list ('vn')

  NOTE: Initializing to 0 signals absence
*/
struct VertexIndex {
  int v_idx = 0;
  int vt_idx = 0;
  int vn_idx = 0;
};
struct Face {
  std::vector<VertexIndex> vertexIndices;
};

class VulkanApplication {
public:
  void run();
  static std::vector<Vertex> vertices;
  static std::vector<uint32_t> indices;

  // INFO: Getters and setters
  const VkViewport &getViewPortRef();

private:
  void initVulkan();
  void mainLoop();
  void cleanup();
  void drawFrame();

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  VkShaderModule createShaderModule(const std::vector<char> &code);

  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  void createVertexBuffer();
  void createIndexBuffer();
  void createCommandBuffers();
  void createSyncObjects();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCameraUniformBuffer();
  void updateCameraUniformBuffer();
  void recordCommandBuffer(VkCommandBuffer, uint32_t);
  std::unique_ptr<Window> window;

  Camera camera;

  VkDescriptorPool descriptorPool;
  VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
  std::vector<VkDescriptorSet> descriptorSets;

  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;
  VkViewport viewport;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;

  std::vector<void *> cameraUniformBuffersMapped;
  std::vector<VkBuffer> cameraUniformBuffers;
  std::vector<VkDeviceMemory> cameraUniformBuffersMemory;

  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;

  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  uint32_t currentFrame = 0;
};

#endif // !VULKAN_APP_H
