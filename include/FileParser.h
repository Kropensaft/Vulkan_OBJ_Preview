/**
 * @file FileParser.h
 * @brief Tools and data structures for parsing 3D OBJ models and MTL materials.
 */
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

/**
 * @struct SubMesh
 * @brief Data structure representing a sub-mesh (part of geometry often bound
 * to one material).
 */
struct SubMesh {
  std::string name;   ///< Name of the sub-mesh
  uint32_t first_idx; ///< Index of the first vertex in the global index buffer
  uint32_t idx_count; ///< Total number of indices belonging to this sub-mesh
};

/**
 * @struct Vertex
 * @brief Represents a single vertex of a 3D model.
 */
struct Vertex {
  glm::vec3 pos;      ///< 3D position (x, y, z)
  glm::vec3 color;    ///< Vertex color (r, g, b)
  glm::vec3 normal;   ///< Normal vector for lighting calculations
  glm::vec2 texCoord; ///< Texture UV coordinates

  /** @brief Returns the binding description for the Vulkan pipeline. */
  static VkVertexInputBindingDescription getBindingDescription();

  /** @brief Returns attribute descriptions for the Vulkan pipeline. */
  static std::array<VkVertexInputAttributeDescription, 4>
  getAttributeDescriptions();
};

/**
 * @struct VertexIndex
 * @brief Holds indices for position, texture coordinate, and normal, along with
 * the material name.
 */
struct VertexIndex {
  int v_idx = 0;
  int vt_idx = 0;
  int vn_idx = 0;
  std::string material_name = "";
};

/**
 * @struct Triangle
 * @brief Represents a face of a 3D model composed of 3 vertices.
 */
struct Triangle {
  VertexIndex vertices[3];
};

/**
 * @struct Material
 * @brief Stores material properties parsed from an MTL file.
 */
struct Material {
  std::string name;
  std::array<float, MATERIAL_ARR_SIZE> diffuse;
  std::array<float, MATERIAL_ARR_SIZE> ambient;
  std::array<float, MATERIAL_ARR_SIZE> specular;
  float shininess;
  std::string diffuse_map;
};

/**
 * @enum ObjLineType
 * @brief Enumeration of possible line types found in an OBJ file.
 */
enum class ObjLineType {
  VERTEX,
  TEXTURE_COORDINATE,
  VERTEX_NORMAL,
  FACE,
  GROUP,
  OBJECT,
  COMMENT,
  MATERIAL_LIBRARY,
  USE_MATERIAL,
  UNKNOWN
};

/**
 * @class FileParser
 * @brief Static utility class for loading and parsing .obj and .mtl files.
 */
class FileParser {
public:
  /** @brief Parses an OBJ file from the given file path. */
  static void parse_OBJ(const char *filepath);

  /** @brief Determines the line type based on its prefix. */
  static ObjLineType getLineType(std::string_view line) {
    if (line.empty())
      return ObjLineType::UNKNOWN;
    auto spacePos = line.find(' ');
    if (spacePos == std::string_view::npos)
      return ObjLineType::UNKNOWN;
    std::string_view prefix = line.substr(0, spacePos);
    auto it = prefixMap.find(prefix);
    return (it != prefixMap.end()) ? it->second : ObjLineType::UNKNOWN;
  }

  static std::map<std::string, std::vector<Triangle>> groupedTriangles;
  static std::vector<SubMesh> subMeshes;

  static void parse_face(std::string &line);
  static void load_material_library(std::string &line,
                                    const std::string &base_dir);
  static void set_current_material(std::string &line);
  static void parse_object(std::string &line);
  static void parse_group(std::string &line);
  static void parse_face(const char *&cursor);

private:
  static std::string current_material;
  static std::string current_group;
  static std::string current_object;

  static std::unordered_map<std::string, Material> materials;
  static const std::unordered_map<std::string_view, ObjLineType> prefixMap;

  static void readFileToBuffer(const char *filePath, std::vector<char> &buffer);
  static void parseBuffer(const std::vector<char> &buffer,
                          const std::string &base_dir);
  static void assembleMeshes();
};

#endif // FILE_PARSER_H
