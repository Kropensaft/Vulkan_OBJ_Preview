#ifndef VULKAN_APP_H
#define VULKAN_APP_H

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "CameraController.h"
#include "Window.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject {
  glm::mat4 view;
  glm::mat4 proj;
};

class VulkanApplication {
public:
  void run();

  static bool renderWireframe;
  static std::vector<Vertex> vertices;
  static std::vector<uint32_t> indices;
  static float deltaTime;

  const VkViewport &getViewPortRef();
  static const float &getDeltaTime();

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

  static PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
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
  VkRect2D scissor;

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

#endif
