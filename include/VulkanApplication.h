/**
 * @file VulkanApplication.h
 * @brief Core application class managing Vulkan API and the render loop.
 */
#ifndef VULKAN_APPLICATION_H
#define VULKAN_APPLICATION_H

#include "VulkanContext.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "CameraController.h"
#include "DirectionalLight.h"
#include "FileParser.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <memory>
#include <vector>

#ifndef VK_ENABLE_BETA_EXTENSIONS
#define VK_ENABLE_BETA_EXTENSIONS
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

constexpr float orthoSize = 15.f;
constexpr float nearPlane = .1f;
constexpr float farPlane = 50.f;

constexpr int SHADOW_MAP_WIDTH_HEIGHT = 4096;

/**
 * @struct UniformBufferObject
 * @brief Structure representing a uniform buffer object.
 * @details Contains transformational matrices needed for computing vertex and
 * normal positions inside a shader.
 */
struct UniformBufferObject {
  glm::mat4 view;  ///< View matrix (camera)
  glm::mat4 proj;  ///< Projection matrix
  glm::mat4 model; ///< Model matrix, transforms an object into world space
  glm::mat4
      normal; ///< Normal matrix, for correct lighting during object movement
};

/**
 * @class VulkanApplication
 * @brief Main class which handles VulkanAPI calls and main render loop.
 * @details Implemented as a Meyers singleton for safer memory management,
 * encapsulates the vulkan calls into private methods and a few higher level
 * public ones.
 */
class VulkanApplication {
public:
  /**
   * @brief Retrieves the singleton instance of the VulkanApplication.
   * @return Reference to the VulkanApplication instance.
   */
  static VulkanApplication &getInstance() {
    static VulkanApplication *instance = new VulkanApplication();
    return *instance;
  }

  VulkanApplication(const VulkanApplication &) = delete;
  VulkanApplication &operator=(const VulkanApplication &) = delete;

  /** @brief Toggles the rendering mode between solid polygons and wireframe. */
  static void toggleWireframe();

  /** @brief Zooms the camera in. */
  static void zoomIn();

  /** @brief Zooms the camera out. */
  static void zoomOut();

  /** @brief Switches the position of the directional light source. */
  static void switchLightSourcePosition();

  /** * @brief Sets the speed multiplier for camera zooming.
   * @param speed The new zoom speed value.
   */
  static void setZoomSpeed(double speed);

  /** @brief Recreates vertex and index buffers (called when a new 3D model is
   * loaded). */
  void recreateGeometryBuffers();

  /** @brief Initializes the Vulkan API and required resources. */
  void initVulkan();

  /** @brief Starts the main application and rendering loop. */
  void run();

  /** @brief Releases all allocated Vulkan resources and destroys the context.
   */
  void cleanup();

  /** * @brief Retrieves the time elapsed between the current and previous
   * frame.
   * @return Constant reference to the delta time float.
   */
  static const float &getDeltaTime();

  static bool renderWireframe;
  static std::vector<Vertex> vertices;
  static std::vector<uint32_t> indices;

  VulkanApplication() = default;

  std::unique_ptr<VulkanContext> context_;

private:
  void instanceLoadNewModel(const char *filePath);
  void instanceToggleWireframe();
  void instanceZoomIn();
  void instanceZoomOut();
  void instanceSetZoomSpeed(double);
  void instanceSwitchLS();
  void cleanupGeometryBuffers();

  void mainLoop();
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

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  void createCameraUniformBuffer();
  void createDescriptorPool();
  void createDescriptorSets();
  void createShadowRenderPass();
  void createShadowResources();
  void createShadowPipeline();
  void updateCameraUniformBuffer();
  void drawFrame();
  void createNormalMatrixUniformBuffer();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  void createDepthResources();
  VkFormat findDepthFormat();
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  bool hasStencilComponent(VkFormat format);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  VkShaderModule createShaderModule(const std::vector<char> &code);
  const VkViewport &getViewPortRef();

  // Variables...
  VkRenderPass shadowRenderPass;
  VkImage shadowImage;
  VkDeviceMemory shadowMemory;
  VkImageView shadowImageView;
  VkSampler shadowSampler;
  VkFramebuffer shadowFrameBuffer;

  VkPipeline shadowPipeline;
  VkPipelineLayout shadowPipelineLayout;

  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;

  std::unique_ptr<Window> window;
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
  VkDescriptorPool descriptorPool;

  VkDescriptorSetLayout globalSetLayout;
  VkDescriptorSetLayout textureSetLayout;

  std::vector<VkDescriptorSet> globalDescriptorSets;
  std::vector<VkDescriptorSet> textureDescriptorSets;

  std::vector<VkBuffer> normalMatrixUniformBuffers;
  std::vector<VkDeviceMemory> normalMatrixUniformBuffersMemory;
  std::vector<void *> normalMatrixUniformBuffersMapped;

  std::unique_ptr<DirectionalLight> light;
  Camera camera;
  size_t currentFrame = 0;
  VkViewport viewport;
  VkRect2D scissor;
  static float deltaTime;
  static PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
};

#endif
