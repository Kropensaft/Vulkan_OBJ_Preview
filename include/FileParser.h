#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

constexpr short MATERIAL_ARR_SIZE = 3;

struct SubMesh {
  std::string name;
  uint32_t first_idx;
  uint32_t idx_count;
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec3 normal;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription();
  static std::array<VkVertexInputAttributeDescription, 4>
  getAttributeDescriptions();
};

struct VertexIndex {
  int v_idx = 0;
  int vt_idx = 0;
  int vn_idx = 0;

  std::string material_name = "";
};

struct Triangle {
  VertexIndex vertices[3];
};

struct Material {
  std::string name;
  std::array<float, MATERIAL_ARR_SIZE> diffuse;
  std::array<float, MATERIAL_ARR_SIZE> ambient;
  std::array<float, MATERIAL_ARR_SIZE> specular;
  float shininess;
  std::string diffuse_map;
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
  COMMENT,
  UNKNOWN,
};

constexpr double SCALE = 0.1;

struct VertexIndexComparator {
  bool operator()(const VertexIndex &a, const VertexIndex &b) const {
    if (a.v_idx != b.v_idx)
      return a.v_idx < b.v_idx;
    if (a.vn_idx != b.vn_idx)
      return a.vn_idx < b.vn_idx;
    if (a.vt_idx != b.vt_idx)
      return a.vt_idx < b.vt_idx;

    return a.material_name < b.material_name;
  }
};

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
  static std::map<std::string, std::vector<Triangle>> groupedTriangles;
  static std::vector<SubMesh> subMeshes;

  static void parse_face(std::string &line);
  static void load_material_library(std::string &line,
                                    const std::string &base_dir);
  static void set_current_material(std::string &line);
  static void parse_object(std::string &line);

private:
  static std::string current_material;
  static std::string current_group;
  static std::string current_object;

  static std::unordered_map<std::string, Material> materials;
  static const std::unordered_map<std::string_view, ObjLineType> prefixMap;
};

#endif
