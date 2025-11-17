#ifndef VULKAN_APPLICATION_H
#define VULKAN_APPLICATION_H

#include "CameraController.h"
#include "FileParser.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

struct UniformBufferObject {
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 model;
  glm::mat4 normal;
};

class VulkanApplication {
private:
  static VulkanApplication *appInstance;

public:
  VulkanApplication(const VulkanApplication &) = delete;
  VulkanApplication &operator=(const VulkanApplication &) = delete;

  static VulkanApplication *getInstance() {
    return appInstance;
  }

  static void toggleWireframe();
  static void zoomIn();
  static void zoomOut();

  void recreateGeometryBuffers();
  void run();
  void cleanup();
  static const float &getDeltaTime();

  static bool renderWireframe;
  static std::vector<Vertex> vertices;
  static std::vector<uint32_t> indices;

  VulkanApplication() = default;

private:
  void instanceLoadNewModel(const char *filePath);
  void instanceToggleWireframe();
  void instanceZoomIn();
  void instanceZoomOut();
  void cleanupGeometryBuffers();

  void initVulkan();
  void mainLoop();
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
  void createCommandBuffers();
  void createSyncObjects();
  void createVertexBuffer();
  void createIndexBuffer();
  void createCameraUniformBuffer();
  void createDescriptorPool();
  void createDescriptorSets();
  void updateCameraUniformBuffer();
  void drawFrame();
  void createNormalMatrixUniformBuffer();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  VkShaderModule createShaderModule(const std::vector<char> &code);
  const VkViewport &getViewPortRef();

  std::unique_ptr<Window> window;
  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageViews;
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  std::vector<VkBuffer> cameraUniformBuffers;
  std::vector<VkDeviceMemory> cameraUniformBuffersMemory;
  std::vector<void *> cameraUniformBuffersMapped;
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  std::vector<VkBuffer> normalMatrixUniformBuffers;
  std::vector<VkDeviceMemory> normalMatrixUniformBuffersMemory;
  std::vector<void *> normalMatrixUniformBuffersMapped;

  Camera camera;
  size_t currentFrame = 0;
  VkViewport viewport;
  VkRect2D scissor;
  static float deltaTime;
  static PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
};

#endif
