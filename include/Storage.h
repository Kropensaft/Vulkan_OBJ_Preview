#ifndef STORAGE_H
#define STORAGE_H
#include <glm/glm.hpp>
#include <utility>

/**
 * @brief Mathematical utility function to extract the sign of a given value
 * @tparam T The numeric type
 * @param val The value to evaluate
 * @return 1 if the value is positive, -1 if negative, and 0 if zero
 */
template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

/**
 * @namespace CONSTANTS
 * @brief Contains global configuration constants and default values
 */
namespace CONSTANTS {

// INFO: Window variables
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

constexpr double DEFAULT_WIN_SENSITIVITY = 20;
constexpr double DEFAULT_SENSITIVITY = 5;

// INFO: String literals
constexpr char DEFAULT_OBJ_PATH[] = "../../../../assets/teapot.obj";
constexpr char INITIAL_LIGHT[] = "Setting initial light direction to ";

constexpr short MATRIX_ALIGN_SIZE = 16;

constexpr unsigned short ATTACHMENT_ARR_SIZE = 2;
constexpr unsigned short LAYOUT_ARR_SIZE = 2;
constexpr unsigned short POOL_ARR_SIZE = 2;
constexpr unsigned short CLEAR_VAL_ARR_SIZE = 2;
constexpr unsigned short GLOBAL_WR_ARR_SIZE = 3;

constexpr std::pair<float, float> VIEWPORT_POS = std::make_pair(0.0f, 0.0f);
constexpr float VIEWPORT_MIN_DEPTH = 0.0f;
constexpr float VIEWPORT_MAX_DEPTH = 1.0f;

constexpr float Z_NEAR = 0.1f;
constexpr float Z_FAR = 100.0f;

constexpr float LIGHT_OFFSET_MULTI = 20.0f;

/**
 * @struct DESCRIPTOR
 * @brief Bit-field structure holding numerical limits and counts for Vulkan
 * setup
 * @details Centralizes various limits, layout counts, and binding indices used
 * when configuring Vulkan pipelines and descriptors. It tightly couples with
 * \ref VulkanApplication and \ref VulkanContext. Using bit-fields minimizes
 * memory footprint.
 */
struct DESCRIPTOR {

  typedef unsigned int ui;

  /// @brief Number of fences to wait for or reset in \ref
  /// VulkanApplication::drawFrame().
  ui FENCE_COUNT : 1 = 1;

  /// @brief Number of semaphores to wait on before execution in \ref
  /// VulkanApplication::drawFrame().
  ui WAIT_SEM_CNT : 1 = 1;

  /// @brief Number of command buffers to submit to the queue in \ref
  /// VulkanApplication::drawFrame().
  ui CMD_BUFF_CNT : 1 = 1;

  /// @brief Number of semaphores to signal after execution finishes in \ref
  /// VulkanApplication::drawFrame().
  ui SIGNAL_SEM_CNT : 1 = 1;

  /// @brief Number of submit info structures passed to the graphics queue.
  ui SUBMIT_CNT : 1 = 1;

  /// @brief Number of swapchains to present to in \ref
  /// VulkanApplication::drawFrame().
  ui SWP_CHN_CNT : 1 = 1;

  /// @brief Number of queues requested from the physical device in \ref
  /// VulkanContext.
  ui Q_CNT : 1 = 1;

  /// @brief Minimum number of images in the swapchain, used in \ref
  /// VulkanApplication::createSwapChain().
  ui MIN_IMG_CNT : 2 = 2;

  /// @brief Number of layers each image consists of (1 for standard 2D images).
  /// Used in \ref VulkanApplication::createSwapChain().
  ui IMG_ARR_LAYERS : 1 = 1;

  /// @brief Number of mipmap levels for the image views in \ref
  /// VulkanApplication::createImageViews().
  ui LVL_CNT : 1 = 1;

  /// @brief Number of array layers for the image views in \ref
  /// VulkanApplication::createImageViews() and framebuffers.
  ui LAYER_CNT : 1 = 1;

  /// @brief Attachment reference index for the depth buffer in \ref
  /// VulkanApplication::createRenderPass().
  ui DEPTH_ATTCH_REF : 1 = 1;

  /// @brief Number of color attachments used in the subpass in \ref
  /// VulkanApplication::createRenderPass().
  ui CLR_ATTCH_CNT : 1 = 1;

  /// @brief Total number of subpasses in the main render pass (\ref
  /// VulkanApplication::createRenderPass()).
  ui SBPSS_CNT : 1 = 1;

  /// @brief Total number of subpass dependencies in \ref
  /// VulkanApplication::createRenderPass().
  ui DEP_CNT : 1 = 1;

  /// @brief Number of vertex binding descriptions provided to the pipeline in
  /// \ref VulkanApplication::createGraphicsPipeline().
  ui VRTX_BIND_DESC_CNT : 1 = 1;

  /// @brief Number of viewports used in the graphics pipeline (\ref
  /// VulkanApplication::createGraphicsPipeline()).
  ui VW_PRT_CNT : 1 = 1;

  /// @brief Number of scissor rectangles used in the graphics pipeline (\ref
  /// VulkanApplication::createGraphicsPipeline()).
  ui VW_PRT_SCISS_CNT : 1 = 1;

  /// @brief Line width for rasterization (e.g., for wireframe mode) in \ref
  /// VulkanApplication::createGraphicsPipeline().
  ui LINE_WIDTH : 1 = 1;

  // COLOR BLENDING
  /// @brief Number of color blend attachments in \ref
  /// VulkanApplication::createGraphicsPipeline().
  ui ATTCH_CNT : 1 = 1;

  /// @brief Number of descriptors per layout binding in \ref
  /// VulkanApplication::createGraphicsPipeline().
  ui DESC_CNT : 1 = 1;

  // MATRICES AND UBO's
  /// @brief Binding index for the Normal Matrix Uniform Buffer in the shader
  /// (\ref VulkanApplication::createGraphicsPipeline()).
  ui NORM_MAT_BIND : 1 = 1;

  /// @brief Binding index for the Directional Light Uniform Buffer in the
  /// shader (\ref VulkanApplication::createGraphicsPipeline()).
  ui LIGHT_UBO_BIND : 2 = 2;

  /// @brief Number of shader stages (e.g., Vertex and Fragment) in \ref
  /// VulkanApplication::createGraphicsPipeline().
  ui STAGE_CNT : 2 = 2;

  /// @brief Pool size multiplier used when allocating descriptor sets in \ref
  /// VulkanApplication::createDescriptorPool().
  ui POOL_DESC_CNT : 2 = 3;

  /// @brief Base destination binding index used when updating descriptor sets
  /// in \ref VulkanApplication::createDescriptorSets().
  ui DST_BIND : 1 = 1;
};

constexpr inline DESCRIPTOR descriptor_vals;

}; // namespace CONSTANTS

#endif // !STORAGE_H
