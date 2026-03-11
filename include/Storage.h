#ifndef STORAGE_H
#define STORAGE_H
#include <glm/glm.hpp>
#include <utility>

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

namespace CONSTANTS {

// INFO: Window variables
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

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

// INFO: decsriptor contants
struct DESCRIPTOR {

  typedef unsigned int ui;

  ui FENCE_COUNT : 1 = 1;
  ui WAIT_SEM_CNT : 1 = 1;
  ui CMD_BUFF_CNT : 1 = 1;
  ui SIGNAL_SEM_CNT : 1 = 1;
  ui SUBMIT_CNT : 1 = 1;
  ui SWP_CHN_CNT : 1 = 1;
  ui Q_CNT : 1 = 1;
  ui MIN_IMG_CNT : 2 = 2;
  ui IMG_ARR_LAYERS : 1 = 1;

  ui LVL_CNT : 1 = 1;
  ui LAYER_CNT : 1 = 1;

  ui DEPTH_ATTCH_REF : 1 = 1;
  ui CLR_ATTCH_CNT : 1 = 1;

  ui SBPSS_CNT : 1 = 1;
  ui DEP_CNT : 1 = 1;

  ui VRTX_BIND_DESC_CNT : 1 = 1;

  ui VW_PRT_CNT : 1 = 1;
  ui VW_PRT_SCISS_CNT : 1 = 1;

  ui LINE_WIDTH : 1 = 1;

  // COLOR BLENDING
  ui ATTCH_CNT : 1 = 1;
  ui DESC_CNT : 1 = 1;

  // MATRICES AND UBO's
  ui NORM_MAT_BIND : 1 = 1;
  ui LIGHT_UBO_BIND : 2 = 2;

  ui STAGE_CNT : 2 = 2;

  ui POOL_DESC_CNT : 2 = 3;

  ui DST_BIND : 1 = 1;
};

inline DESCRIPTOR descriptor_vals;

}; // namespace CONSTANTS

#endif // !STORAGE_H
