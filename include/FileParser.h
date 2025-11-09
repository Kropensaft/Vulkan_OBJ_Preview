#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <array>
#include <glm/glm.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription();
  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

struct VertexIndex {
  int v_idx = 0;
  int vt_idx = 0;
  int vn_idx = 0;
};

struct Triangle {
  VertexIndex vertices[3];
};

enum class ObjLineType {
  VERTEX,
  TEXTURE_COORDINATE,
  VERTEX_NORMAL,
  FACE,
  GROUP,
  OBJECT,
  MATERIAL_LIBRARY,
  USE_MATERIAL,
  UNKNOWN,
};

constexpr double SCALE = 0.1;

class FileParser {
public:
  static void read_OBJ();
  static ObjLineType getLineType(std::string_view line) {
    if (line.empty() || line[0] == '#')
      return ObjLineType::UNKNOWN;

    auto spacePos = line.find(' ');
    if (spacePos == std::string_view::npos)
      return ObjLineType::UNKNOWN;

    std::string_view prefix = line.substr(0, spacePos);
    auto it = prefixMap.find(prefix);
    return (it != prefixMap.end()) ? it->second : ObjLineType::UNKNOWN;
  }

  static void parse_OBJ(const char *filepath);

  static std::vector<Triangle> allTriangles;

private:
  static const std::unordered_map<std::string_view, ObjLineType> prefixMap;
};

#endif
